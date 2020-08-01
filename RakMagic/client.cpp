#include "main.h"

namespace RakMagic {

	RakClientInterface *pRakInterface;
	bool bConnected;
	bool bConnectionRequested;
	bool bGameInited;
	bool bSpawned;
	PLAYERID iPlayerID;

	int iMode;
	int iState = RSTATE_VOID;
	float fHealth = 0.f;
	float fArmor = 0.f;
	int iWeapon = 0;
	short sAnimIndex;
	short sAnimFlags;
	float fPosition[3];
	float fQuaternion[4];
	int iVehicle = -1;

	int iFollow = 0;

	unsigned int uRespawnTick = 0;
	unsigned int uUpdateScoreTick = 0;
	unsigned int uUpdateTick = 0;

	bool Initialize() {
		bConnected = bConnectionRequested = bGameInited = false;

		pRakInterface = RakNetworkFactory::GetRakClientInterface();
		if (pRakInterface == nullptr)
			return false;

		pRakInterface->SetMTUSize(576);

		registerRPC();

		if (!LoadAnimationFromConfig(&gConfig))
			log("Invalid animation config");

		return true;
	}

	void Uninitialize() {
		if (pRakInterface != nullptr) {
			//unregisterRPC();
			RakNetworkFactory::DestroyRakClientInterface(pRakInterface);
		}
	}

	bool IsConnected() {
		return bConnected;
	}

	bool Connect() {
		pRakInterface->SetPassword(gConfig.szServerPassword);
		bConnectionRequested = true;
		return pRakInterface->Connect(gConfig.szServerAddress, gConfig.iPort, 0, 0, 8);
	}

	void Disconnect() {
		gPlayer[iPlayerID].bIsConnected = false;
		pRakInterface->Disconnect(16);
	}

	void Reset() {
		bGameInited = bConnected = bConnectionRequested = false;
		gConfig.uReconnectTick = GetTickCount();
		iState = RSTATE_VOID;
	}

	void Update() {
		if (!bConnectionRequested) {
			if (GetTickCount() - gConfig.uReconnectTick > gConfig.uReconnectTime) {
				logPrintf("Connecting to %s", gConfig.szServerAddress);
				Connect();
			}
		}

		unsigned char packetIdentifier;
		Packet *pkt;

		while (pkt = pRakInterface->Receive()) {
			if ((unsigned char)pkt->data[0] == ID_TIMESTAMP) {
				if (pkt->length > sizeof(unsigned char) + sizeof(unsigned int))
					packetIdentifier = (unsigned char)pkt->data[sizeof(unsigned char) + sizeof(unsigned int)];
				else
					return;
			}
			else
				packetIdentifier = (unsigned char)pkt->data[0];

			switch (packetIdentifier) {
			case ID_DISCONNECTION_NOTIFICATION:
				logPrintf("[x] Connection was closed by the server.");
				Reset();
				break;
			case ID_CONNECTION_BANNED:
				logPrintf("[x] You are banned");
				Reset();
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				logPrintf("[x] Connection attempt failed.");
				Reset();
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				logPrintf("[x] The server is full.");
				Reset();
				break;
			case ID_INVALID_PASSWORD:
				logPrintf("[x] Invalid password.");
				Reset();
				break;
			case ID_CONNECTION_LOST:
				logPrintf("[x] The connection was lost.");
				Reset();
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				logPrintf("[-] The connection was accepted.");
				RespondJoin(pkt);
				break;
			case ID_AUTH_KEY:
				AuthorizeKey(pkt);
				break;
			
			case ID_PLAYER_SYNC:
				Sync::PlayerSync(pkt);
				break;
			case ID_VEHICLE_SYNC:
				Sync::VehicleSync(pkt);
				break;
			case ID_PASSENGER_SYNC:
				Sync::PassengerSync(pkt);
				break;
			case ID_UNOCCUPIED_SYNC:
				Sync::UnoccupiedSync(pkt);
				break;
			case ID_AIM_SYNC:
				Sync::AimSync(pkt);
				break;
			case ID_MARKERS_SYNC:
				Sync::MarkersSync(pkt);
				break;
			case ID_BULLET_SYNC:
				Sync::BulletSync(pkt);
				break;
			}

			pRakInterface->DeallocatePacket(pkt);
		}

		if (RakMagic::bGameInited && iState != RSTATE_VOID) {
			if (GetTickCount() - uUpdateScoreTick > 3000) {
				RakNet::BitStream bsParams;
				pRakInterface->RPC(&RPC_UpdateScoresPingsIPs, &bsParams, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
				uUpdateScoreTick = GetTickCount();
			}
			if (bSpawned) {
				if (Playback::iPlaybackType != Playback::PLAYBACK_TYPE_NONE && Playback::bPlaying) {
					Playback::Process();
				}
				else if (GetTickCount() - uUpdateTick > gConfig.iUpdateRate) {
					switch (iMode) {
					case RUNMODE_NORMAL:
						if (iVehicle != -1)
							AI::UpdateVehicle(iVehicle);
						else
							AI::UpdateOnfoot();
						break;
					case RUNMODE_FOLLOW:
						AI::UpdateFollow();
						break;

					/*case RUNMODE_AFK:
						break;
						//Do nothing */
					}
					uUpdateTick = GetTickCount();
				}
				if (Playback::iPlaybackType != Playback::PLAYBACK_TYPE_NONE && !Playback::bPlaying) {
					Playback::Update();
				}
			}
			else {
				if (iState == RSTATE_DEAD) {
					if (GetTickCount() - uRespawnTick > gConfig.uRespawnTick) {
						Spawn();
					}
				}
			}
		}
	}

	void ReconnectTo(const char *addr, int port) {
		strcpy(gConfig.szServerAddress, addr);
		gConfig.iPort = port;
		logPrintf("Reconnect to %s in %0.1fs", gConfig.szServerAddress, (float)gConfig.uReconnectTime / 1000.0f);
		RakMagic::Disconnect();
		RakMagic::Reset();
	}

	void Reconnect() {
		logPrintf("Reconnect to the server in %0.1fs", (float)gConfig.uReconnectTime / 1000.0f);
		RakMagic::Disconnect();
		RakMagic::Reset();
	}

	void RespondJoin(Packet *p) {
		RakNet::BitStream bsSuccAuth((unsigned char *)p->data, p->length, false);
		unsigned int uiChallenge;

		bsSuccAuth.IgnoreBits(8); // ID_CONNECTION_REQUEST_ACCEPTED
		bsSuccAuth.IgnoreBits(32); // binaryAddress
		bsSuccAuth.IgnoreBits(16); // port

		bsSuccAuth.Read(iPlayerID);
		bsSuccAuth.Read(uiChallenge);

		//logPrintf("Connected as ID %d. ", iPlayerID);

		char auth_bs[4 * 16] = { 0 };
		gen_gpci(auth_bs, 0x3e9);

		BYTE byteNameLen = (BYTE)strlen(gConfig.szNickname);
		BYTE byteAuthBSLen = (BYTE)strlen(auth_bs);
		BYTE iClientVerLen = (BYTE)strlen(gConfig.szClientVersion);

		RakNet::BitStream bsSend;

		bsSend.Write(gConfig.iNetGameVersion);
		bsSend.Write((BYTE)(1));
		bsSend.Write(byteNameLen);
		bsSend.Write(gConfig.szNickname, byteNameLen);

		bsSend.Write((unsigned int)(uiChallenge ^ gConfig.iNetGameVersion));
		bsSend.Write(byteAuthBSLen);
		bsSend.Write(auth_bs, byteAuthBSLen);
		bsSend.Write(iClientVerLen);
		bsSend.Write(gConfig.szClientVersion, iClientVerLen);

		pRakInterface->RPC(&RPC_ClientJoin, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);

		fHealth = 100.f;
		fArmor = 0.f;
		iWeapon = 0;
		memcpy(fPosition, gConfig.fPosition, (sizeof fPosition[0]) * 3);

		bConnected = true;
	}

	void AuthorizeKey(Packet *p) {
		char* auth_key;
		bool found_key = false;

		for (int x = 0; x < 512; x++) {
			if (!strcmp(((char*)p->data + 2), AuthKeyTable[x][0])) {
				auth_key = AuthKeyTable[x][1];
				found_key = true;
				break;
			}
		}

		if (found_key) {
			RakNet::BitStream bsKey;
			BYTE byteAuthKeyLen;

			byteAuthKeyLen = (BYTE)strlen(auth_key);

			bsKey.Write((BYTE)ID_AUTH_KEY);
			bsKey.Write((BYTE)byteAuthKeyLen);
			bsKey.Write(auth_key, byteAuthKeyLen);

			pRakInterface->Send(&bsKey, SYSTEM_PRIORITY, RELIABLE, NULL);
		}
		else {
			logPrintf("[x] Unknown AUTH_IN! (%s)", ((char*)p->data + 2));
		}
	}

	void RequestClass(int classid) {
		RakNet::BitStream bsSpawnRequest;
		bsSpawnRequest.Write(classid);
		pRakInterface->RPC(&RPC_RequestClass, &bsSpawnRequest, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}
	void RequestSpawn() {
		RakNet::BitStream bsSendRequestSpawn;
		pRakInterface->RPC(&RPC_RequestSpawn, &bsSendRequestSpawn, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}
	void Spawn() {
		if (iState == RSTATE_DEAD)
			fHealth = 100.f;

		RakNet::BitStream bsSendSpawn;
		pRakInterface->RPC(&RPC_Spawn, &bsSendSpawn, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
		bSpawned = true;
		iState = RSTATE_ONFOOT;
	}
	void SetToDeathState(int murderer, int reason) {
		if (iState != RSTATE_DEAD) {
			iVehicle = -1;
			iState = RSTATE_DEAD;
			bSpawned = false;

			if (gConfig.bPlaybackDeathPause)
				Playback::bPlaying = false;

			if (murderer != -1) {
				SendWastedNotification(reason, murderer);
				logPrintf("*** You were killed by {FF6347}%s {ffffff}using {FFFF66}%s", gPlayer[murderer].szPlayerName, getWeaponName(reason));
			}

			uRespawnTick = GetTickCount();
		}
	}

	void SendWastedNotification(BYTE byteDeathReason, PLAYERID WhoWasResponsible) {
		RakNet::BitStream bsPlayerDeath;
		bsPlayerDeath.Write(byteDeathReason);
		bsPlayerDeath.Write(WhoWasResponsible);
		pRakInterface->RPC(&RPC_Death, &bsPlayerDeath, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}

	void SendServerCommand(char *szCommand) {
		RakNet::BitStream bsParams;
		int iStrlen = strlen(szCommand);
		bsParams.Write(iStrlen);
		bsParams.Write(szCommand, iStrlen);
		pRakInterface->RPC(&RPC_ServerCommand, &bsParams, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}

	void SendChat(char *szMessage) {
		RakNet::BitStream bsSend;
		BYTE byteTextLen = (BYTE)strlen(szMessage);
		bsSend.Write(byteTextLen);
		bsSend.Write(szMessage, byteTextLen);
		pRakInterface->RPC(&RPC_Chat, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}

	void SendScoreboardClick(int playerid) {
		RakNet::BitStream bsSend;
		bsSend.Write((BYTE)playerid);
		bsSend.Write((uint8_t)0);
		pRakInterface->RPC(&RPC_ClickPlayer, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}

	void SetAnimation(short sAnimationId, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime) {
		sAnimIndex = sAnimationId;
		if (sAnimationId == 0) {
			sAnimFlags = 0;
		}
		else {
			sAnimFlags = (static_cast<BYTE>(fDelta) & 0xFF)
				| (bLoop << 8)
				| (bLockX << 9)
				| (bLockY << 10)
				| (bFreeze << 11)
				| (static_cast<BYTE>(iTime) << 12);
		}
	}

	/*void SetAnimationByName(char *szName, float fDelta, bool bLoop, bool bLockX, bool bLockY, bool bFreeze, int iTime) {
		WORD wAnimationId = CAnimationInfo::GetIndexByName(szName);
		SetAnimation(wAnimationId, fDelta, bLoop, bLockX, bLockY, bFreeze, iTime);
	}*/

	void ResetAnimation() {
		sAnimIndex = sAnimFlags = 0;
	}

	bool LoadAnimationFromConfig(const stConfig *config) {
		if (config->sAnimIndex == 0)
			return true;
		else if (config->sAnimIndex < 0 || config->sAnimIndex >= MAX_ANIMATIONS)
			return false;
		SetAnimation(config->sAnimIndex, 4.1f, config->bAnimLoop, config->bAnimLockX, config->bAnimLockY, false, config->iAnimTime);
		return true;
	}

	void TakeBulletDamage(int attacker, int weaponid) {
		float amount;
		switch (weaponid) {
		case 24://DesertEagle
			amount = 46.200013f;
			break;
		case 22://Colt45
			amount = 8.25f;
			break;
		case 32://Tec9
			amount = 6.599976f;
			break;
		case 28://Uzi
			amount = 6.599976f;
			break;
		case 23://SilencedColt
			amount = 13.200012f;
			break;
		case 31://M4
			amount = 9.900024f;
			break;
		case 30://AK
			amount = 9.900024f;
			break;
		case 29://MP5
			amount = 8.25f;
			break;
		case 34://SniperRifle
			amount = 41.299987f;
			break;
		case 33://CuntGun
			amount = 24.799987f;
			break;
		case 25://PumpShotgun
			amount = 30.0f;
			break;
		case 26://Sawnoff
			amount = 30.0f;
		case 27://Spaz12
			amount = 30.0f;
			break;
		case 38://Minigun
			amount = 46.200013f;
			break;
		default:
			amount = -1;
			break;
		}
		if (amount > 0.f) {
			RakNet::BitStream bsParams;
			bsParams.Write(true);
			bsParams.Write((UINT16)attacker);
			bsParams.Write(amount);
			bsParams.Write(weaponid);
			pRakInterface->RPC(&RPC_PlayerGiveTakeDamage, &bsParams, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);

			float remain = amount;
			if (fArmor > 0.f) {
				fArmor -= amount;
				if (fArmor <= 0.f) {
					remain = abs(fArmor);
					fArmor = 0.f;
				}
				else {
					remain = 0.f;
				}
			}
			if (remain > 0.f) {
				fHealth -= remain;
				if (fHealth <= 0.f) {
					fHealth = 0.f;
					SetToDeathState(attacker, weaponid);
				}
			}
		}
	}
}