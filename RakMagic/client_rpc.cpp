#include "main.h"


namespace RakMagic {
	namespace RPC {
		void initGame(RPCParameters *arg) {
			PCHAR Data = reinterpret_cast<PCHAR>(arg->input);

			RakNet::BitStream bsInitGame((unsigned char *)Data, ((int)arg->numberOfBitsOfData / 8) + 1, false);

			PLAYERID MyPlayerID;
			bool bLanMode, bStuntBonus;
			//BYTE byteVehicleModels[212];

			bool m_bZoneNames, m_bUseCJWalk, m_bAllowWeapons, m_bLimitGlobalChatRadius;
			float m_fGlobalChatRadius, m_fNameTagDrawDistance;
			bool m_bDisableEnterExits, m_bNameTagLOS, m_bManualVehicleEngineAndLight;
			bool m_bShowPlayerTags;
			int m_iShowPlayerMarkers;
			BYTE m_byteWorldTime, m_byteWeather;
			float m_fGravity;
			int m_iDeathDropMoney;
			bool m_bInstagib;
			int iSpawnsAvailable;
			BYTE m_bLagCompensation;

			bsInitGame.ReadCompressed(m_bZoneNames);
			bsInitGame.ReadCompressed(m_bUseCJWalk);
			bsInitGame.ReadCompressed(m_bAllowWeapons);
			bsInitGame.ReadCompressed(m_bLimitGlobalChatRadius);
			bsInitGame.Read(m_fGlobalChatRadius);
			bsInitGame.ReadCompressed(bStuntBonus);
			bsInitGame.Read(m_fNameTagDrawDistance);
			bsInitGame.ReadCompressed(m_bDisableEnterExits);
			bsInitGame.ReadCompressed(m_bNameTagLOS);
			bsInitGame.ReadCompressed(m_bManualVehicleEngineAndLight); // 
			bsInitGame.Read(iSpawnsAvailable);
			bsInitGame.Read(MyPlayerID);
			bsInitGame.ReadCompressed(m_bShowPlayerTags);
			bsInitGame.Read(m_iShowPlayerMarkers);
			bsInitGame.Read(m_byteWorldTime);
			bsInitGame.Read(m_byteWeather);
			bsInitGame.Read(m_fGravity);
			bsInitGame.ReadCompressed(bLanMode);
			bsInitGame.Read(m_iDeathDropMoney);
			bsInitGame.ReadCompressed(m_bInstagib);

			bsInitGame.SetReadOffset(bsInitGame.GetReadOffset() + 4 * 32);

			bsInitGame.Read(m_bLagCompensation);

			BYTE unk;
			bsInitGame.Read(unk);
			bsInitGame.Read(unk);
			bsInitGame.Read(unk);

			BYTE byteStrLen;
			bsInitGame.Read(byteStrLen);
			if (byteStrLen) {
				memset(gConfig.szServerName, 0, sizeof(gConfig.szServerName));
				bsInitGame.Read(gConfig.szServerName, byteStrLen);
			}
			gConfig.szServerName[byteStrLen] = '\0';

			bGameInited = true;

			iState = RSTATE_NOT_SPAWNED;

			log("[-] You are now connected to \"%s\".", gConfig.szServerName);
			if (gConfig.bEncodeGB2312) {
				char *unicoded = g2u(gConfig.szServerName);
				strcpy(gConfig.szServerName, unicoded);
				delete[] unicoded;

				unicoded = g2u(gConfig.szNickname);
				strcpy(gPlayer[iPlayerID].szPlayerName, unicoded);
				delete[] unicoded;
			}
			Interface::Console::AddLog("[-] You are now connected to \"%s\".", gConfig.szServerName);

			gPlayer[iPlayerID].bIsConnected = true;
			RequestClass(gConfig.iDefaultClass);
		}

		void serverMessage(RPCParameters *arg) {
			PCHAR Data = reinterpret_cast<PCHAR>(arg->input);

			RakNet::BitStream bsData((unsigned char *)Data, ((int)arg->numberOfBitsOfData / 8) + 1, false);
			DWORD dwStrLen, dwColor;
			char szMsg[257];
			memset(szMsg, 0, 257);

			bsData.Read(dwColor);
			bsData.Read(dwStrLen);
			if (dwStrLen > 256) return;

			bsData.Read(szMsg, dwStrLen);
			szMsg[dwStrLen] = 0;

			/*(dwColor = dwColor >> 8;

			if (dwColor != 0xff) {
				sprintf(szMsg, "{%06x}%s ", dwColor, szMsg);
			}*/
			if (dwColor != -1) {
				char hexColored[257];
				memset(hexColored, 0, 257);
				sprintf(hexColored, "{%06x}%s", dwColor >> 8, szMsg);
				strcpy(szMsg, hexColored);
			}

			log(szMsg);
			if (gConfig.bEncodeGB2312) {
				char *unicoded = g2u(szMsg);
				Interface::Console::AddLog(unicoded);
				delete[] unicoded;
			}
			else {
				Interface::Console::AddLog(szMsg);
			}
		}

		void chatMessage(RPCParameters *arg) {
			PCHAR Data = reinterpret_cast<PCHAR>(arg->input);

			PlayerID sender = arg->sender;

			RakNet::BitStream bsData((unsigned char *)Data, ((int)arg->numberOfBitsOfData / 8) + 1, false);
			PLAYERID playerId;
			BYTE byteTextLen;

			unsigned char szText[256];
			memset(szText, 0, 256);

			bsData.Read(playerId);
			bsData.Read(byteTextLen);
			bsData.Read((char*)szText, byteTextLen);
			szText[byteTextLen] = 0;

			if (playerId < 0 || playerId >= MAX_PLAYERS)
				return;

			log((char *)szText);
			if (gConfig.bEncodeGB2312) {
				char *unicoded = g2u((char *)szText);
				Interface::Console::AddLog("[CHAT] %s: %s", gPlayer[playerId].szPlayerName, unicoded);
				delete[] unicoded;
			}
			else {
				Interface::Console::AddLog("[CHAT] %s: %s", gPlayer[playerId].szPlayerName, szText);
			}
		}

		void serverJoin(RPCParameters *rpcParams) {

			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);

			char szPlayerName[256];
			PLAYERID playerId;
			BYTE byteNameLen = 0;

			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);

			bsData.Read(playerId);
			int iUnk = 0;
			bsData.Read(iUnk);
			BYTE bIsNPC = 0;
			bsData.Read(bIsNPC);
			bsData.Read(byteNameLen);
			if (byteNameLen > 20) return;
			bsData.Read(szPlayerName, byteNameLen);
			szPlayerName[byteNameLen] = '\0';

			if (playerId < 0 || playerId >= MAX_PLAYERS) return;

			gPlayer[playerId].bIsConnected = true;
			gPlayer[playerId].byteIsNPC = bIsNPC;
			if (gConfig.bEncodeGB2312) {
				char *unicoded = g2u(szPlayerName);
				strcpy((char *)gPlayer[playerId].szPlayerName, unicoded);
				delete[] unicoded;
			}
			else strcpy((char *)gPlayer[playerId].szPlayerName, szPlayerName);
		}

		void serverQuit(RPCParameters *rpcParams) {
			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);


			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);
			PLAYERID playerId;
			BYTE byteReason;

			bsData.Read(playerId);
			bsData.Read(byteReason);

			if (playerId < 0 || playerId >= MAX_PLAYERS) return;

			gPlayer[playerId].bIsConnected = false;
			gPlayer[playerId].byteIsNPC = 0;
			memset(gPlayer[playerId].szPlayerName, 0, 64);
		}

		void updateScoresPingsIPs(RPCParameters *rpcParams) {

			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			int iBitLength = rpcParams->numberOfBitsOfData;

			RakNet::BitStream bsData((unsigned char *)Data, (iBitLength / 8) + 1, false);

			PLAYERID playerId;
			int iPlayerScore;
			DWORD dwPlayerPing;

			for (PLAYERID i = 0; i < (iBitLength / 8) / 9; i++) {
				bsData.Read(playerId);
				bsData.Read(iPlayerScore);
				bsData.Read(dwPlayerPing);

				if (playerId < 0 || playerId >= MAX_PLAYERS)
					continue;

				gPlayer[playerId].iScore = iPlayerScore;
				gPlayer[playerId].dwPing = dwPlayerPing;
			}
		}

		void setPosition(RPCParameters *rpcParams) {
			if (gConfig.bKeepPosition)
				return;

			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			int iBitLength = rpcParams->numberOfBitsOfData;

			RakNet::BitStream bsData((unsigned char *)Data, (iBitLength / 8) + 1, false);

			bsData.Read(RakMagic::fPosition[0]);
			bsData.Read(RakMagic::fPosition[1]);
			bsData.Read(RakMagic::fPosition[2]);
		}

		void dialogBox(RPCParameters *args) {

			if (!Interface::Dialog::bEnabled)
				return;

			PCHAR Data = reinterpret_cast<PCHAR>(args->input);
			int iBitLength = args->numberOfBitsOfData;
			RakNet::BitStream bsData((unsigned char *)Data, (iBitLength / 8) + 1, false);

			using namespace Interface;

			bsData.Read(Dialog::dialog.wDialogID);
			bsData.Read(Dialog::dialog.bDialogStyle);

			bsData.Read(Dialog::dialog.bTitleLength);
			bsData.Read(Dialog::dialog.szTitle, Dialog::dialog.bTitleLength);
			Dialog::dialog.szTitle[Dialog::dialog.bTitleLength] = 0;

			bsData.Read(Dialog::dialog.bButton1Len);
			bsData.Read(Dialog::dialog.szButton1, Dialog::dialog.bButton1Len);
			Dialog::dialog.szButton1[Dialog::dialog.bButton1Len] = 0;

			bsData.Read(Dialog::dialog.bButton2Len);
			bsData.Read(Dialog::dialog.szButton2, Dialog::dialog.bButton2Len);
			Dialog::dialog.szButton2[Dialog::dialog.bButton2Len] = 0;

			stringCompressor->DecodeString(Dialog::dialog.szInfo, 256, &bsData);

			if (gConfig.bEncodeGB2312) {
				char *unicoded = g2u(Dialog::dialog.szTitle);
				strcpy(Dialog::dialog.szTitle, unicoded);
				delete[] unicoded;

				unicoded = g2u(Dialog::dialog.szButton1);
				strcpy(Dialog::dialog.szButton1, unicoded);
				delete[] unicoded;

				unicoded = g2u(Dialog::dialog.szButton2);
				strcpy(Dialog::dialog.szButton2, unicoded);
				delete[] unicoded;

				unicoded = g2u(Dialog::dialog.szInfo);
				strcpy(Dialog::dialog.szInfo, unicoded);
				delete[] unicoded;
			}

			Dialog::iSelectedItem = -1;
			Dialog::bShowDialog = true;
		}

		void worldPlayerAdd(RPCParameters *args) {
			PCHAR Data = reinterpret_cast<PCHAR>(args->input);
			int iBitLength = args->numberOfBitsOfData;

			RakNet::BitStream bsData((unsigned char *)Data, (iBitLength / 8) + 1, false);

			PLAYERID playerId;
			BYTE byteFightingStyle = 4;
			BYTE byteTeam = 0;
			int iSkin = 0;
			float vecPos[3];
			float fRotation = 0;
			DWORD dwColor = 0;

			bsData.Read(playerId);
			if (playerId < 0 || playerId >= MAX_PLAYERS) return;
			bsData.Read(byteTeam);
			bsData.Read(iSkin);
			bsData.Read(vecPos[0]);
			bsData.Read(vecPos[1]);
			bsData.Read(vecPos[2]);
			bsData.Read(fRotation);
			bsData.Read(dwColor);
			bsData.Read(byteFightingStyle);

			gPlayer[playerId].bIsStreamedIn = true;
			gPlayer[playerId].onfootData.vecPos[0] =
				gPlayer[playerId].incarData.vecPos[0] = vecPos[0];
			gPlayer[playerId].onfootData.vecPos[1] =
				gPlayer[playerId].incarData.vecPos[1] = vecPos[1];
			gPlayer[playerId].onfootData.vecPos[2] =
				gPlayer[playerId].incarData.vecPos[2] = vecPos[2];
		}

		void worldPlayerRemove(RPCParameters *args) {
			PCHAR Data = reinterpret_cast<PCHAR>(args->input);

			RakNet::BitStream bsData((unsigned char *)Data, ((int)args->numberOfBitsOfData / 8) + 1, false);

			PLAYERID playerId = 0;
			bsData.Read(playerId);

			if (playerId < 0 || playerId >= MAX_PLAYERS) return;

			gPlayer[playerId].bIsStreamedIn = false;
			gPlayer[playerId].incarData.vecPos[0] = 0.0f;
			gPlayer[playerId].incarData.vecPos[1] = 0.0f;
			gPlayer[playerId].incarData.vecPos[2] = 0.0f;
		}

		void worldVehicleAdd(RPCParameters *args) {
			PCHAR Data = reinterpret_cast<PCHAR>(args->input);

			RakNet::BitStream bsData((unsigned char *)Data, ((int)args->numberOfBitsOfData / 8) + 1, false);

			NEW_VEHICLE NewVehicle;

			bsData.Read((char *)&NewVehicle, sizeof(NEW_VEHICLE));

			if (NewVehicle.VehicleId < 0 || NewVehicle.VehicleId >= MAX_VEHICLES) return;

			gVehiclePool[NewVehicle.VehicleId].bExist = true;

			memcpy(gVehiclePool[NewVehicle.VehicleId].fPos, NewVehicle.vecPos, sizeof(float) * 3);
			gVehiclePool[NewVehicle.VehicleId].fHealth = NewVehicle.fHealth;
			gVehiclePool[NewVehicle.VehicleId].iModelID = NewVehicle.iVehicleType;
		}

		void worldVehicleRemove(RPCParameters *rpcParams) {

			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);

			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);

			VEHICLEID VehicleID;
			bsData.Read(VehicleID);
			if (VehicleID < 0 || VehicleID >= MAX_VEHICLES) return;

			gVehiclePool[VehicleID].bExist = false;
			vect3_zero(gVehiclePool[VehicleID].fPos);
		}

		void worldPickupCreate(RPCParameters *rpcParams) {
			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);

			int PickupID;
			PICKUP Pickup;

			bsData.Read(PickupID);
			if (PickupID >= MAX_PICKUPS)
				return;

			bsData.Read((PCHAR)&Pickup, sizeof(PICKUP));

			gPickupPool[PickupID].bExist = true;
			gPickupPool[PickupID].fPos[0] = Pickup.fX;
			gPickupPool[PickupID].fPos[1] = Pickup.fY;
			gPickupPool[PickupID].fPos[2] = Pickup.fZ;
			gPickupPool[PickupID].iModelID = Pickup.iModel;
		}

		void worldPickupDestroy(RPCParameters *rpcParams) {
			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);
			int PickupID;
			bsData.Read(PickupID);
			if (PickupID >= MAX_PICKUPS)
				return;

			gPickupPool[PickupID].bExist = false;
			vect3_zero(gPickupPool[PickupID].fPos);
		}

		void setPlayerHealth(RPCParameters *rpcParams) {
			if (!gConfig.bDamage)
				return;

			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);
			bsData.Read(RakMagic::fHealth);
		}

		void setPlayerArmor(RPCParameters *rpcParams) {
			if (!gConfig.bDamage)
				return;

			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);
			bsData.Read(RakMagic::fArmor);
		}

		/*void setSkin(RPCParameters *rpcParams) {
			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);
			bsData.Read(RakMagic::iSkin);
		}

		void setInterior(RPCParameters *rpcParams) {
			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);
			bsData.Read(RakMagic::iInterior);

			RakNet::BitStream bsSend;
			bsSend.Write((uint8_t)RakMagic::iInterior);
			pRakInterface->RPC(&RPC_SetInteriorId, &bsSend, HIGH_PRIORITY, RELIABLE, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
		}

		void haveCash(RPCParameters *rpcParams) {
			unsigned int addition;
			PCHAR Data = reinterpret_cast<PCHAR>(rpcParams->input);
			RakNet::BitStream bsData((unsigned char *)Data, ((int)rpcParams->numberOfBitsOfData / 8) + 1, false);
			bsData.Read(addition);

			RakMagic::iCash += addition;
		}

		void resetCash(RPCParameters *rpcParams) {
			RakMagic::iCash = 0;
		}*/
	}

	void registerRPC() {
		using namespace RPC;
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_InitGame, initGame);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ClientMessage, serverMessage);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_Chat, chatMessage);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ServerJoin, serverJoin);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ServerQuit, serverQuit);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_UpdateScoresPingsIPs, updateScoresPingsIPs);

		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ScrSetPlayerPos, setPosition);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ScrDialogBox, dialogBox);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_WorldPlayerAdd, worldPlayerAdd);
		//pRakInterface->RegisterAsRemoteProcedureCall(&RPC_WorldPlayerDeath, worldPlayerDeath);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_WorldPlayerRemove, worldPlayerRemove);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_WorldVehicleAdd, worldVehicleAdd);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_WorldVehicleRemove, worldVehicleRemove);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_Pickup, worldPickupCreate);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_DestroyPickup, worldPickupDestroy);

		/*pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ScrSetPlayerSkin, setSkin);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ScrSetInterior, setInterior);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ScrHaveSomeMoney, haveCash);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ScrResetMoney, resetCash);*/

		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ScrSetPlayerHealth, setPlayerHealth);
		pRakInterface->RegisterAsRemoteProcedureCall(&RPC_ScrSetPlayerArmour, setPlayerArmor);
	}

	void unregisterRPC() {
		//pRakInterface->UnregisterAsRemoteProcedureCall(&RPC_InitGame);
		//pRakInterface->UnregisterAsRemoteProcedureCall(&RPC_ClientMessage);
	}
}

