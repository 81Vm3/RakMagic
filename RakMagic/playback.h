
#ifndef PLAYBACK_H
#define PLAYBACK_H

namespace Playback {
	extern const char szPlaybackDirectory[];

	enum {
		PLAYBACK_TYPE_NONE,
		PLAYBACK_TYPE_DRIVER,
		PLAYBACK_TYPE_ONFOOT          
	};

	extern const char szPlaybackDirectory[];
	extern int iPlaybackType;
	extern bool bPlaying;

	bool Load(const char *szFile, char *errmsg = nullptr);
	bool Unload();
	void StartFirstEver();
	void Update();
	void Process();

	int GetTotalPacket();
	int GetCurrentIndex();
	float GetProgessPercent();
}

#endif
