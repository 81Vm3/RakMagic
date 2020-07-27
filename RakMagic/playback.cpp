#include "main.h"

namespace Playback {
	const char szPlaybackDirectory[] = "playbacks";
	int iPlaybackType = PLAYBACK_TYPE_NONE;
	
	DWORD dwStartTime;
	bool bPlaying = false;
	int iCurrentIndex = 0;

	std::vector<DWORD> v_dwTime;
	std::vector<ONFOOT_SYNC_DATA> v_playerSyncData;
	std::vector<INCAR_SYNC_DATA> v_vehicleSyncData;

	bool Load(const char *szFile, char *errmsg) {

		if (iPlaybackType != PLAYBACK_TYPE_NONE) {
			if (errmsg != nullptr)
				strcpy(errmsg, "There is playback that has been load already, please unload it manually");
			return false;
		}

		FILE *pFile;
		// Try to open the playback file

		char cache[512];
		sprintf(cache, "%s\\%s.rec", szPlaybackDirectory, szFile);
		fopen_s(&pFile, cache, "rb");
		if (!pFile) {
			if (errmsg != nullptr)
				strcpy(errmsg, "Unable to open the file");
			return false;
		}

		// Check the file size
		fseek(pFile, 0, SEEK_END);
		size_t sSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);
		if (!sSize) {
			fclose(pFile);
			if (errmsg != nullptr)
				strcpy(errmsg, "Not a valid playback file");
			return false;
		}

		size_t sElementsRead = 0;
		// Read the playback type and file info ?
		DWORD dwFile;
		int iFilePlaybackType = 0;
		sElementsRead = fread(&dwFile, sizeof(DWORD), 1, pFile);
		if (sElementsRead != 1) {
			if (errmsg != nullptr)
				strcpy(errmsg, "Unable to copy dwFile data to memory.");
		}

		sElementsRead = fread(&iFilePlaybackType, sizeof(int), 1, pFile);
		if (sElementsRead != 1) {
			if (errmsg != nullptr)
				strcpy(errmsg, "Unable to copy iPlaybackType data to memory.");
		}
		// Check the playback type
		if (iFilePlaybackType != PLAYBACK_TYPE_DRIVER && iFilePlaybackType != PLAYBACK_TYPE_ONFOOT) {
			fclose(pFile);
			if (errmsg != nullptr)
				strcpy(errmsg, "Not a valid playback type");
			return false;
		}

		// add new record
		DWORD dwTime;
		iPlaybackType = iFilePlaybackType;

		if (iPlaybackType == PLAYBACK_TYPE_DRIVER) {
			INCAR_SYNC_DATA vehicleSyncData;

			while (!feof(pFile)) {
				if (!fread(&dwTime, sizeof(DWORD), 1, pFile)) {
					break;
				}
				else if (!fread(&vehicleSyncData, sizeof(INCAR_SYNC_DATA), 1, pFile)) {
					break;
				}

				v_dwTime.push_back(dwTime);
				v_vehicleSyncData.push_back(vehicleSyncData);
			}
		}
		else if (iPlaybackType == PLAYBACK_TYPE_ONFOOT) { //-V547
			ONFOOT_SYNC_DATA playerSyncData;

			while (!feof(pFile)) {
				if (!fread(&dwTime, sizeof(DWORD), 1, pFile)) {
					break;
				}
				else if (!fread(&playerSyncData, sizeof(ONFOOT_SYNC_DATA), 1, pFile)) {
					break;
				}

				v_dwTime.push_back(dwTime);
				v_playerSyncData.push_back(playerSyncData);
			}
		}

		fclose(pFile);
		iCurrentIndex = 0;

		return true;
	}

	bool Unload() {
		if (iPlaybackType != PLAYBACK_TYPE_NONE) {
			v_dwTime.clear();
			v_playerSyncData.clear();
			v_vehicleSyncData.clear();

			bPlaying = false;
			iPlaybackType = PLAYBACK_TYPE_NONE;
			return true;
		}
		return false;
	}

	void StartFirstEver() {
		bPlaying = true;
		dwStartTime = GetTickCount();
	}

	void Update() {
		dwStartTime = (GetTickCount() - v_dwTime[iCurrentIndex]);
	}

	void Process() {

		using namespace RakMagic;

		if (iCurrentIndex >= (int)v_dwTime.size()) {
			iCurrentIndex = 0;
			StartFirstEver();
			return;
		}
		
		// Check the time
		if ((GetTickCount() - dwStartTime) >= v_dwTime[iCurrentIndex]) {
			// Read the first recording data
			if (iPlaybackType == PLAYBACK_TYPE_DRIVER) {
				// Get the vehicle interface
				if (!IsVehicleIndexValid(RakMagic::iVehicle)) {
					AI::UpdateOnfoot();
					Update();
					return;
				}

				// Get the in car sync data
				INCAR_SYNC_DATA data = v_vehicleSyncData[iCurrentIndex];

				memcpy(fPosition, data.vecPos, (sizeof(float)) * 3);
				memcpy(gVehiclePool[RakMagic::iVehicle].fPos, data.vecPos, (sizeof(float)) * 3);

				// Apply the sync data
				data.VehicleID = RakMagic::iVehicle;
				data.fCarHealth = gVehiclePool[RakMagic::iVehicle].fHealth;
				data.bytePlayerHealth = (BYTE)fHealth;
				data.bytePlayerArmor = (BYTE)fArmor;

				RakNet::BitStream bs;
				bs.Write((BYTE)ID_VEHICLE_SYNC);
				bs.Write((PCHAR)&data, sizeof(INCAR_SYNC_DATA));
				pRakInterface->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

				iState = RSTATE_DRIVER;
			}
			else if (iPlaybackType == PLAYBACK_TYPE_ONFOOT) {
				// Get the on foot sync data
				ONFOOT_SYNC_DATA data = v_playerSyncData[iCurrentIndex];

				memcpy(fPosition, data.vecPos, (sizeof(float)) * 3);
				
				data.byteHealth = (BYTE)fHealth;
				data.byteArmor = (BYTE)fArmor;

				RakNet::BitStream bs;
				bs.Write((BYTE)ID_PLAYER_SYNC);
				bs.Write((PCHAR)&data, sizeof(ONFOOT_SYNC_DATA));
				pRakInterface->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

				iVehicle = -1;
				iState = RSTATE_ONFOOT;
			}
			// Update the index
			iCurrentIndex++;
		}
	}

	int GetTotalPacket() {
		return (int)v_dwTime.size();
	}

	int GetCurrentIndex() {
		return iCurrentIndex;
	}

	float GetProgessPercent() {
		if (v_dwTime.size() == 0)
			return 0.f;
		return (float)iCurrentIndex / (float)v_dwTime.size();
	}
}