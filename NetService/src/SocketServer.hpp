#ifdef SOCKETSERVER_H_
#define SOCKETSERVER_H_
class Connection
{
public:
    Connection();
    ~Connection();
private:
    //disable copy and assignment
    Connection(const Connection& ){}
    Connection& operator=(const Connection&){}
public:
    virtual bool start();
    int recv();
    int send();
    
    
};
class SocketServer
{
public:
    SocketServer();
    ~SocketServer();
private:
    //disable copy and assignment
    SocketServer(const SocketServer& ){}
    SocketServer& operator=(const SocketServer&){}
public:    
    virtual bool start();
};


#endif /* SOCKETSERVER_H_ */