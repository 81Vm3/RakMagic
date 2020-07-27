#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

class CClientManager {
public:
	const static int iMaxClients = 256;

	//I believe you don't want to run thousands of bots :(
	CClient* clients[iMaxClients];
	int  AddClient();
	bool RemoveClient(int index);
};

extern CClientManager gClient;

#endif