#ifndef CONFIG_H
#define CONFIG_H

extern const char *gDefaultConfigPath;

struct stConfig {
	char szServerAddress[32], szServerPassword[32], szNickname[32], szClientVersion[16];
	int iNetGameVersion;
	unsigned int iPort;
	unsigned int iUpdateRate;
	int iNicknameStyle;

	unsigned int uReconnectTick;
	unsigned int uReconnectTime;
	unsigned int uRespawnTick;

	int iDefaultClass;

	float fFollowOffset[3];
	float fEnterDistance;
	bool bKeepPosition, bCopyHealth, bCopyWeapon, bDamage;

	short sAnimIndex;
	bool bAnimLoop, bAnimLockX, bAnimLockY;
	int iAnimTime;

	char szFont[32];
	float fFontSize;
	bool bEncodeGB2312;
	ImVec4 InterfaceClearColor;

	bool bPlaybackAutoPlay;
	bool bPlaybackDeathPause;

	//###The following variables can not be change by loadConfig in runtime###
	bool bInitializedFirstEver;
	char szServerName[128];
	float fPosition[3]; //Where the bot keeps at. This is not the location of the bot
	bool bInvisible;
};

bool loadConfig(stConfig *config, const char *path);
extern stConfig gConfig;

#endif