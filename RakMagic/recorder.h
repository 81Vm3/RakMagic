#ifndef RECORDER_H
#define RECORDER_H

namespace PBRecorder {
	extern int iPlaybackType;
	extern int iPacketRecorded;
	extern int iGoalPlayer;

	bool StartRecording(int type);
	void AddOnfootPacket(const ONFOOT_SYNC_DATA *data);
	void AddVehiclePacket(const INCAR_SYNC_DATA *data);

	void Reset();
	bool Finalize(const char *szFile, char *errmsg = nullptr);
}

#endif