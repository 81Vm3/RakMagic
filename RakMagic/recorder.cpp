#include "main.h"

namespace PBRecorder {
	std::vector<DWORD> v_dwTime;
	std::vector<ONFOOT_SYNC_DATA> v_playerSyncData;
	std::vector<INCAR_SYNC_DATA> v_vehicleSyncData;

	//The process of recording can not be paused
	int iPlaybackType = Playback::PLAYBACK_TYPE_NONE;
	DWORD dwStartTime;
	int iPacketRecorded = 0;
	int iGoalPlayer = 0;

	bool StartRecording(int type) {
		if (iPlaybackType == Playback::PLAYBACK_TYPE_NONE) {
			iPlaybackType = type;
			dwStartTime = GetTickCount();
			iPacketRecorded = 0;
			return true;
		}
		return false;
	}

	void AddOnfootPacket(const ONFOOT_SYNC_DATA *data) {
		if (iPlaybackType != Playback::PLAYBACK_TYPE_ONFOOT)
			return;
		v_dwTime.push_back(GetTickCount() - dwStartTime);
		v_playerSyncData.push_back(*data);
		iPacketRecorded++;
	}

	void AddVehiclePacket(const INCAR_SYNC_DATA *data) {
		if (iPlaybackType != Playback::PLAYBACK_TYPE_DRIVER)
			return;
		v_dwTime.push_back(GetTickCount() - dwStartTime);
		v_vehicleSyncData.push_back(*data);
		iPacketRecorded++;
	}

	void Reset() {
		v_dwTime.clear();
		v_playerSyncData.clear();
		v_vehicleSyncData.clear();

		iPlaybackType = Playback::PLAYBACK_TYPE_NONE;
	}

	bool Finalize(const char *szFile, char *errmsg) {
		if (iPlaybackType == Playback::PLAYBACK_TYPE_NONE) {
			if (errmsg != nullptr)
				strcpy(errmsg, "Nothing is being recorded");
			return false;
		}

		FILE *pFile;

		char cache[512];
		sprintf(cache, "%s\\%s.rec", Playback::szPlaybackDirectory, szFile);
		fopen_s(&pFile, cache, "wb");
		if (!pFile) {
			if (errmsg != nullptr)
				strcpy(errmsg, "Unable to open the file");
			return false;
		}

		// Write the playback type and file info
		DWORD dwFile;
		if (fwrite(&dwFile, sizeof(DWORD), 1, pFile) == 0) {
			if (errmsg != nullptr)
				strcpy(errmsg, "Unable to write the record to the file");
			return false;
		}

		//OK, we can write file
		fwrite(&iPlaybackType, sizeof(int), 1, pFile);

		if (iPlaybackType == Playback::PLAYBACK_TYPE_DRIVER) {
			for (int i = 0; i < iPacketRecorded; i++) {
				if (!fwrite(&v_dwTime[i], sizeof(DWORD), 1, pFile))
					break;

				if (!fwrite(&v_vehicleSyncData[i], sizeof(INCAR_SYNC_DATA), 1, pFile))
					break;
			}
		}
		else if (iPlaybackType == Playback::PLAYBACK_TYPE_ONFOOT) {
			for (int i = 0; i < iPacketRecorded; i++) {
				if (!fwrite(&v_dwTime[i], sizeof(DWORD), 1, pFile))
					break;

				//logPrintf("%0.2f %0.2f %0.2f", v_playerSyncData[i].vecPos[0], v_playerSyncData[i].vecPos[1], v_playerSyncData[i].vecPos[2]);
				if (!fwrite(&v_playerSyncData[i], sizeof(ONFOOT_SYNC_DATA), 1, pFile))
					break;
			}
		}

		iPlaybackType = Playback::PLAYBACK_TYPE_NONE;

		fclose(pFile);
		return true;
	}
}