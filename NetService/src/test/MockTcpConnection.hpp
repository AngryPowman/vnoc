#ifndef MOCK_TCP_CONNECTION_H
#define MOCK_TCP_CONNECTION_H
#include "../SocketServer.hpp"

class MockTcpConnection: public Connection
{
public:
    template <typename Handler> int recv(char * buf, size_t len, Handler handler)
    {
        
        return 0;
    }
    template <typename Handler> int read_some(char * buf, size_t len, Handler handler)
    {
        
        return 0;
    }
    template <typename Handler> int send(char * buf, size_t len, Handler handler)
    {
        
        return 0;
    }
private:

};

#endif /*MOCK_TCP_CONNECTION_H*/
