#ifndef ECHOTESTHANDLER_H_
#define ECHOTESTHANDLER_H_
#include "../AsioTcpConnection.hpp"
#include <iostream>
class EchoTestHandler
{
public:
	EchoTestHandler(AsioTcpConnection *connection):connection_(connection){}
    void AcceptHandler( const asio::error_code& error)
    {
        if (!error)
		{
			start();
		} else 
		{
			std::cout<<error.message()<<std::endl;
		}
    }
	void start()
	{
        connection_->read_some(data_, sizeof(data_)-1, 
        std::bind(&EchoTestHandler::ReadsomeHandler, this,
          std::placeholders::_1,
          std::placeholders::_2));
	}
    void ReadsomeHandler(const asio::error_code& error, size_t bytes_transferred)
    {
        if (!error)
		{
            data_[bytes_transferred]='\0';
            std::cout<<data_;
            connection_->send(data_, bytes_transferred, 
            std::bind(&EchoTestHandler::HandleWrite, this,
              std::placeholders::_1));
        }else
        {
            delete this;
        }
    }
	void HandleWrite(const asio::error_code& error)
	{
		if (!error)
		{
			connection_->read_some(data_, sizeof(data_)-1, 
            std::bind(&EchoTestHandler::ReadsomeHandler, this,
              std::placeholders::_1,
              std::placeholders::_2));
		}else
		{
			delete this;
		}
		
	}
private:
    AsioTcpConnection *connection_;
    char data_[512];
};

#endif /* ECHOTESTHANDLER_H_ */
