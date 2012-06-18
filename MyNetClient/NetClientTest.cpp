#include "ClientConnection.h"
#include "NetClient.h"

int main(int argc, TCHAR* argv[])
{
	VNOC_CLIENT::CClientConnection connection;
	connection.Create();
	return 0;
}
