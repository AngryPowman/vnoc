#include "Core/OpcodesDefine.h"
#include "Handlers/LoginHandler.h"

int main()
{
	LoginHandler loginHandler;
	loginHandler.execute(Opcodes::LOGIN_AUTH, NULL);

	system("pause");
	return 0;
}