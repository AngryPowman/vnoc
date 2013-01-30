/*
 * File : Login.h
 *
 *  Created on: 2013/1/5
 *      Author: AngryPowman
 *
 */

#include "../Core/Common.h"
#include "../Core/LogicRunner.h"

class LoginHandler : public LogicRunner<LoginHandler>
{
public:
	LoginHandler();
	~LoginHandler();

public:
	void auth(const ExtraData* data);
};