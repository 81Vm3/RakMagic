#include "main.h"

CPlayer gPlayer[MAX_PLAYERS];

CPlayer::CPlayer() {
	bIsConnected = bIsStreamedIn = bGotMarkersPos = false;
}

const float *CPlayer::GetPosition() {
	float *pos_ptr = onfootData.vecPos;
	if (bAreWeInAVehicle) {
		if (bPassenger) {
			if (gVehiclePool[passengerData.VehicleID].bExist)
				pos_ptr = gVehiclePool[passengerData.VehicleID].fPos;
		}
		else
			pos_ptr = incarData.vecPos;
	}
	return pos_ptr;
}

bool CPlayer::Teleport() {
	if (!bIsConnected)
		return false;

	if (bIsStreamedIn || bGotMarkersPos) {
		vect3_copy(GetPosition(), RakMagic::fPosition);
		return true;
	}
	return false;
}

int CPlayer::GetTotalPlayer() {
	int count = 0;
	for (int i = 0; i < MAX_PLAYERS; i++) {
		if (!gPlayer[i].bIsConnected) continue;
		count++;
	}
	return count;
}