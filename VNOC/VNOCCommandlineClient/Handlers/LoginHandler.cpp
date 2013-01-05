#include "LoginHandler.h"
#include "../Core/OpcodesDefine.h"
#include "../Module/LoginProxy/LoginProxy.h"

LoginHandler::LoginHandler()
{
	this->bind(Opcodes::LOGIN_AUTH, &LoginHandler::auth);
}

LoginHandler::~LoginHandler()
{

}

void LoginHandler::auth(const ExtraData* data)
{
	//...
	std::cout << "do something here" << std::endl;

	LoginProxy::instance().login("À×·æÊåÊå", "123456");
}