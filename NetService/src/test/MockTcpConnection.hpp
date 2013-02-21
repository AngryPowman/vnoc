#ifndef MOCK_TCP_CONNECTION_H
#define MOCK_TCP_CONNECTION_H
#include "../SocketServer.hpp"
#include <asio.hpp>
#include <asio/error.hpp>
#include <memory>
#include <ezlogger_headers.hpp>
class EventOperator
{
public:
    virtual void operator()(const asio::error_code& error, size_t bytes_transferred) = 0;
};
template <class T >
class HandlerWrapper : public EventOperator
{
public:
    HandlerWrapper(const T& handler):handler_(handler){}
    virtual void operator()(const asio::error_code& error, size_t bytes_transferred)
    {
        handler_(error, bytes_transferred);
    }
private:
    T handler_;
};

class MockTcpConnection: public Connection
{
public:
    template <typename Handler> int recv(char * buf, size_t len, Handler handler)
    {
        recvBuf_ = buf;
        recvBufLen = len;
        recvHandler_.reset(new HandlerWrapper<Handler>(handler));
        isReadSome_ = false;
        receivedLenth_ = 0;
        return 0;
    }
    template <typename Handler> int read_some(char * buf, size_t len, Handler handler)
    {
        recvBuf_ = buf;
        recvBufLen = len;
        recvHandler_.reset(new HandlerWrapper<Handler>(handler));
        isReadSome_ = true;
        receivedLenth_ = 0;
        return 0;
    }
    template <typename Handler> int send(char * buf, size_t len, Handler handler)
    {
        sendHandler_.reset(new HandlerWrapper<Handler>(handler));
        sendLen_ = len;
        memcpy(sendBuf_, buf, len);
        sendBuf_[len]=0;
        EZLOGGERSTREAM<<"send: "<<sendBuf_<<std::endl;
        return 0;
    }
    bool expectSend(const char * buf, size_t len)
    {
        if (len <= sizeof(sendBuf_)){
            return memcmp(sendBuf_, buf, len) == 0;
        }
        return false;
    }
    void setRecv(const char * buf, size_t len)
    {
        if (len <= recvBufLen - receivedLenth_){
            memcpy(recvBuf_ + receivedLenth_, buf, len);
            EZLOGGERSTREAM<<"recv: "<< recvBuf_<<std::endl;
            asio::error_code ec;
            if (isReadSome_) {
                (*recvHandler_)(ec, len);
            } else {
                if (len == recvBufLen - receivedLenth_){
                    (*recvHandler_)(ec, recvBufLen);
                } else {
                    receivedLenth_ += len;
                }
            }
        } else {
            size_t offset = 0;
            while (len > 0 && recvBufLen > 0){
                size_t putLen = 0;
                if (len >= recvBufLen){
                    putLen = recvBufLen;
                    len -= recvBufLen;
                } else {
                    putLen = len;
                    len = 0;
                }
                memcpy(recvBuf_, buf + offset, putLen);
                offset += putLen;
                EZLOGGERSTREAM<<"recv: "<< recvBuf_<<std::endl;
                asio::error_code ec;
                if (isReadSome_) {
                    (*recvHandler_)(ec, putLen);
                } else {
                    if (putLen == recvBufLen - receivedLenth_){
                        (*recvHandler_)(ec, recvBufLen);
                    } else {
                        receivedLenth_ += putLen;
                    }
                }
            }
        }
    }

    void close()
    {
        asio::error_code ec(asio::error::shut_down);
        (*recvHandler_)(ec, 0);
    }

    size_t getSendLen(){return sendLen_;}
    const char *getSendBuf(){return sendBuf_;}
private:
    std::shared_ptr<EventOperator> recvHandler_;
    std::shared_ptr<EventOperator> sendHandler_;
    char* recvBuf_;
    size_t recvBufLen;
    size_t sendLen_;
    char sendBuf_[102400];
    bool isReadSome_;
    size_t receivedLenth_;
};

#endif /*MOCK_TCP_CONNECTION_H*/
