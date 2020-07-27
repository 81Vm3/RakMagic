#include "main.h"

CClientManager gClient;

int CClientManager::AddClient() {
	for (int i = 0; i < iMaxClients; i++) {
		if (clients[i] == nullptr) {
			clients[i] = new CClient;
			return i;
		}
	}
}

bool CClientManager::RemoveClient(int index) {
	if (index < 0 || index >= iMaxClients)
		return false;

	if (clients[index] == nullptr)
		return false;

	delete clients[index];
	clients[index] = nullptr;
	return true;
}