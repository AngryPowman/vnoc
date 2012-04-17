#ifndef MOCK_TCP_CONNECTION_H
#define MOCK_TCP_CONNECTION_H
#include "../SocketServer.hpp"
#include <asio.hpp>
#include <memory>
class EventOperator
{
public:
    virtual void operator()(const asio::error_code& error, size_t bytes_transferred)=0;
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
        return 0;
    }
    template <typename Handler> int read_some(char * buf, size_t len, Handler handler)
    {
        recvBuf_ = buf;
		recvBufLen = len;
		recvHandler_.reset(new HandlerWrapper<Handler>(handler));
        return 0;
    }
    template <typename Handler> int send(char * buf, size_t len, Handler handler)
    {
        sendHandler_.reset(new HandlerWrapper<Handler>(handler));
		memcpy(sendBuf_, buf, len);
		sendBuf_[len]=0;
		std::cout<<"send: "<<sendBuf_<<std::endl;
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
        if (len <= recvBufLen){
            memcpy(recvBuf_, buf, len);
			std::cout<<"recv: "<< recvBuf_<<std::endl;
			asio::error_code ec;
			(*recvHandler_)(ec, len);
        }		
    }
private:
    std::shared_ptr<EventOperator> recvHandler_;
    std::shared_ptr<EventOperator> readSomeHandler_;
    std::shared_ptr<EventOperator> sendHandler_;
    char* recvBuf_;
	size_t recvBufLen;
    char sendBuf_[1024];
};

#endif /*MOCK_TCP_CONNECTION_H*/