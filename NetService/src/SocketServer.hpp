#ifndef SOCKETSERVER_H_
#define SOCKETSERVER_H_
class Connection
{
public:
    Connection(){};
    virtual ~Connection(){};
private:
    //disable copy and assignment
    Connection(const Connection& ){}
    Connection& operator=(const Connection&){}
public:
    virtual bool start() = 0;
    int recv();
    int send();
    
    
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