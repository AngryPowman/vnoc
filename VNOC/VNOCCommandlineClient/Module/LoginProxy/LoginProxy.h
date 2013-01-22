/*
 * File : LoginProxy.h
 *
 *  Created on: 2013/1/5
 *      Author: AngryPowman
 *  
 */

#include "../../Core/Common.h"

class LoginProxy
{
public:
	static LoginProxy& instance();

private:
	LoginProxy();
	virtual ~LoginProxy();

public:
	uint32 login(std::string userName, std::string password);

private:
	static LoginProxy _instance;
};