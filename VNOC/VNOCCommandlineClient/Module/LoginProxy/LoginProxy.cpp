#include "LoginProxy.h"

LoginProxy LoginProxy::_instance;
LoginProxy& LoginProxy::instance()
{
	return _instance;
}

LoginProxy::LoginProxy()
{

}

LoginProxy::~LoginProxy()
{

}

uint32 LoginProxy::login(std::string userName, std::string password)
{
	// ...
	std::cout << "Username = " << userName << ", Password = " << password << endl;

	return 0;
}