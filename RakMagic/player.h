#ifndef PLAYER_H
#define PLAYER_H

class CPlayer {
public:
	CPlayer ();

	bool bIsConnected;
	bool bIsStreamedIn;
	bool bGotMarkersPos;
	char szPlayerName[64];
	int iScore;
	DWORD dwPing;
	bool bAreWeInAVehicle;
	bool bPassenger;
	BYTE byteInteriorId;
	BYTE byteIsNPC;

	// STORED INFO
	ONFOOT_SYNC_DATA onfootData;
	INCAR_SYNC_DATA incarData;
	PASSENGER_SYNC_DATA passengerData;
	AIM_SYNC_DATA aimData;

	const float *GetPosition();
	bool Teleport();  //note: this just modifies the local position, does not synchronizations.
	static int GetTotalPlayer();
};

extern CPlayer gPlayer[MAX_PLAYERS];

#endif