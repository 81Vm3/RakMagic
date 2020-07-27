#ifndef CLIENT_H
#define CLIENT_H

namespace RakMagic {

	enum {
		RUNMODE_NORMAL,
		RUNMODE_FOLLOW,
		RUNMODE_AFK
	};

	enum {
		RSTATE_VOID, // (not connected and haven't initalize the game)
		RSTATE_NOT_SPAWNED, // (connected, but haven't spawned yet, and the game has been initialized)
		RSTATE_DEAD,
		RSTATE_ONFOOT,
		RSTATE_DRIVER,
		RSTATE_PASSENGER
	};

	extern RakClientInterface *pRakInterface;
	extern bool bConnected;
	extern bool bConnectionRequested;
	extern bool bGameInited;
	extern bool bSpawned;
	extern PLAYERID iPlayerID; //Bot's ID on server
	extern stPlayerInfo *pInfo; //A pointer to bot's stPlayerInfo 
	
	extern int iMode; //Running mode
	extern int iState; //Bot's state, onfoot, incar, passenger...
	extern float fHealth;
	extern float fArmor;
	extern int iWeapon;
	extern int iVehicle;

	extern short sAnimIndex;
	extern short sAnimFlags;
	extern float fPosition[3];
	extern float fQuaternion[4];

	bool Initialize();
	void Uninitialize();

	bool IsConnected();
	bool Connect();
	void Disconnect();
	void Reset();

	void Update();

	void RespondJoin(Packet *p);
	void AuthorizeKey(Packet *p);

	void registerRPC();
	void unregisterRPC();

	void ReconnectTo(const char *addr, int port);
	void Reconnect();

	void RequestClass(int classid);
	void RequestSpawn();
	void Spawn();
	void SetToDeathState(int murderer = -1, int reason = -1);
	void SendWastedNotification(BYTE byteDeathReason, PLAYERID WhoWasResponsible);
	void SendServerCommand(char *szCommand);
	void SendChat(char *szMessage);
	void SendScoreboardClick(int playerid);

	//-------FROM FCNPC-------
	void SetAnimation(short sAnimationId, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime);
	//void SetAnimationByName(char *szName, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime);
	void ResetAnimation();
	//-------FROM FCNPC-------
	bool LoadAnimationFromConfig(const stConfig *config);
	void TakeBulletDamage(int attacker, int weaponid);
};

#endif