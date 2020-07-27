#ifndef SAMP_STRUCTURES_H
#define SAMP_STRUCTURES_H

#define NETCODE_CONNCOOKIELULZ 0x6969
#define NETGAME_VERSION 4057

#define REJECT_REASON_BAD_VERSION	1
#define REJECT_REASON_BAD_NICKNAME	2
#define REJECT_REASON_BAD_MOD		3
#define REJECT_REASON_BAD_PLAYERID	4

#define BP _asm int 3

#define MAX_PLAYERS 1004
#define MAX_VEHICLES 2000
#define MAX_PICKUPS 512 //Is it possible to stream over 1k pickups?
#define MAX_SCRIPTS 32

typedef unsigned short PLAYERID;
typedef unsigned short VEHICLEID;

typedef signed char		int8_t;
typedef unsigned char		uint8_t;
typedef signed short		int16_t;
typedef unsigned short		uint16_t;
typedef signed int		int32_t;
typedef unsigned int		uint32_t;
typedef signed long long	int64_t;
typedef unsigned long long	uint64_t;
#define snprintf	_snprintf
#define vsnprintf	_vsnprintf
#define isfinite	_finite

#define EVENT_TYPE_PAINTJOB				1
#define EVENT_TYPE_CARCOMPONENT			2
#define EVENT_TYPE_CARCOLOR				3
#define EVENT_ENTEREXIT_MODSHOP			4

#define BULLET_HIT_TYPE_NONE			0
#define BULLET_HIT_TYPE_PLAYER			1
#define BULLET_HIT_TYPE_VEHICLE			2
#define BULLET_HIT_TYPE_OBJECT			3
#define BULLET_HIT_TYPE_PLAYER_OBJECT   4

#define KEY_UP							-128
#define KEY_DOWN						128
#define KEY_LEFT						-128
#define KEY_RIGHT						128
#define KEY_ACTION						1
#define KEY_CROUCH						2
#define KEY_FIRE						4
#define KEY_SPRINT						8
#define KEY_SECONDARY_ATTACK			16
#define KEY_JUMP						32
#define KEY_LOOK_RIGHT					64
#define KEY_HANDBRAKE					128
#define KEY_LOOK_LEFT					256
#define KEY_SUBMISSION					512
#define KEY_LOOK_BEHIND					512
#define KEY_WALK						1024
#define KEY_ANALOG_UP					2048
#define KEY_ANALOG_DOWN					4096
#define KEY_ANALOG_LEFT					8192
#define KEY_ANALOG_RIGHT				16384

#pragma pack(1)
typedef struct _ONFOOT_SYNC_DATA
{
	uint16_t	sLeftRightKeys;
	uint16_t	sUpDownKeys;
	uint16_t	sKeys;
	float		vecPos[3];
	float		fQuaternion[4];
	uint8_t		byteHealth;
	uint8_t		byteArmor;
	uint8_t		byteCurrentWeapon;
	uint8_t		byteSpecialAction;
	float		vecMoveSpeed[3];
	float		vecSurfOffsets[3];
	uint16_t	wSurfInfo;
	short		sAnimIndex;
	short		sAnimFlags;

} ONFOOT_SYNC_DATA;

#pragma pack(1)
typedef struct _INCAR_SYNC_DATA
{
	VEHICLEID VehicleID;
	WORD lrAnalog;
	WORD udAnalog;
	WORD wKeys;
	float fQuaternion[4];
	float vecPos[3];
	float vecMoveSpeed[3];
	float fCarHealth;
	BYTE bytePlayerHealth;
	BYTE bytePlayerArmor;
	BYTE byteCurrentWeapon;
	BYTE byteSirenOn;
	BYTE byteLandingGearState;
	WORD TrailerID_or_ThrustAngle;
	FLOAT fTrainSpeed;
} INCAR_SYNC_DATA;

#pragma pack(1)
typedef struct _PASSENGER_SYNC_DATA
{
	VEHICLEID VehicleID;
	BYTE byteSeatFlags : 7;
	BYTE byteDriveBy : 1;
	BYTE byteCurrentWeapon;
	BYTE bytePlayerHealth;
	BYTE bytePlayerArmour;
	WORD lrAnalog;
	WORD udAnalog;
	WORD wKeys;
	float vecPos[3];
} PASSENGER_SYNC_DATA;

enum eWeaponState
{
	WS_NO_BULLETS = 0,
	WS_LAST_BULLET = 1,
	WS_MORE_BULLETS = 2,
	WS_RELOADING = 3,
};

#pragma pack(1)
typedef struct _AIM_SYNC_DATA
{
	BYTE byteCamMode;
	float vecAimf1[3];
	float vecAimPos[3];
	float fAimZ;
	BYTE byteCamExtZoom : 6;	// 0-63 normalized
	BYTE byteWeaponState : 2;	// see eWeaponState
	BYTE bUnk;
} AIM_SYNC_DATA;

#pragma pack(1)
typedef struct _UNOCCUPIED_SYNC_DATA // 67
{
	VEHICLEID VehicleID;
	short cvecRoll[3];
	short cvecDirection[3];
	BYTE unk[13];
	float vecPos[3];
	float vecMoveSpeed[3];
	float vecTurnSpeed[3];
	float fHealth;
} UNOCCUPIED_SYNC_DATA;

#pragma pack(1)
typedef struct _SPECTATOR_SYNC_DATA
{
	WORD lrAnalog;
	WORD udAnalog;
	WORD wKeys;
	float vecPos[3];
} SPECTATOR_SYNC_DATA;

#pragma pack(1)
typedef struct _BULLET_SYNC_DATA {
	BYTE bHitType;
	unsigned short iHitID;
	float fHitOrigin[3];
	float fHitTarget[3];
	float fCenterOfHit[3];
	BYTE bWeaponID;
} BULLET_SYNC_DATA;

#pragma pack(1)
typedef struct _PLAYER_SPAWN_INFO
{
	BYTE byteTeam;
	int iSkin;
	BYTE unk;
	float vecPos[3];
	float fRotation;
	int iSpawnWeapons[3];
	int iSpawnWeaponsAmmo[3];
} PLAYER_SPAWN_INFO;

#pragma pack(1)
typedef struct _PICKUP
{
	int iModel;
	int iType;
	float fX;
	float fY;
	float fZ;
} PICKUP;

#pragma pack(1)
typedef struct _TEXT_DRAW_TRANSMIT
{
	union
	{
		BYTE byteFlags;
		struct
		{
			BYTE byteBox : 1;
			BYTE byteLeft : 1;
			BYTE byteRight : 1;
			BYTE byteCenter : 1;
			BYTE byteProportional : 1;
			BYTE bytePadding : 3;
		};
	};
	float fLetterWidth;
	float fLetterHeight;
	DWORD dwLetterColor;
	float fLineWidth;
	float fLineHeight;
	DWORD dwBoxColor;
	BYTE byteShadow;
	BYTE byteOutline;
	DWORD dwBackgroundColor;
	BYTE byteStyle;
	BYTE byteSelectable;
	float fX;
	float fY;
	WORD wModelID;
	float fRotX;
	float fRotY;
	float fRotZ;
	float fZoom;
	WORD wColor1;
	WORD wColor2;
} TEXT_DRAW_TRANSMIT;

#pragma pack(1)
typedef struct _NEW_VEHICLE {
	VEHICLEID VehicleId;
	int		  iVehicleType;
	float	  vecPos[3];
	float	  fRotation;
	BYTE	  aColor1;
	BYTE	  aColor2;
	float	  fHealth;
	BYTE	  byteInterior;
	DWORD	  dwDoorDamageStatus;
	DWORD	  dwPanelDamageStatus;
	BYTE	  byteLightDamageStatus;
	BYTE	  byteTireDamageStatus;
	BYTE	  byteAddSiren;
	BYTE      byteModSlots[14];
	BYTE	  bytePaintjob;
	DWORD	  cColor1;
	DWORD	  cColor2;
	BYTE	  byteUnk;
} NEW_VEHICLE;

struct stPlayerInfo
{
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
};

struct stVehiclePool {
	bool bExist;
	float fHealth;
	float fPos[3];
	int iModelID;
};

struct stPickupPool {
	bool bExist;
	float fPos[3];
	int iModelID;
};

#endif