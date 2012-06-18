#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_
class Connection
{
public:
    Connection(){};
    ~Connection(){};
private:
    //disable copy and assignment
    Connection(const Connection& ){}
    Connection& operator=(const Connection&){}
public:
    template <typename Handler> int recv(char * buf, size_t len, Handler handler){}
    template <typename Handler> int read_some(char * buf, size_t len, Handler handler){}
    template <typename Handler> int send(char * buf, size_t len, Handler handler){}
};
class SocketServer
{
public:
    SocketServer(){};
    virtual ~SocketServer(){};
private:
    //disable copy and assignment
    SocketServer(const SocketServer& ){}
    SocketServer& operator=(const SocketServer&){}
public:    
    virtual bool start(unsigned int port) = 0;
};


#endif /* SOCKETSERVER_H_ */