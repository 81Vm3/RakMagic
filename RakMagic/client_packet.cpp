#include "main.h"

namespace RakMagic {
	namespace Sync {
		void PlayerSync(Packet *p) {

			RakNet::BitStream bsPlayerSync((unsigned char *)p->data, p->length, false);
			PLAYERID playerId;
			bool bHasLR, bHasUD;
			bool bHasSurfInfo, bAnimation;
			//The packet received from server is different

			bsPlayerSync.IgnoreBits(8); //ignore the packet id
			bsPlayerSync.Read(playerId);


			// LEFT/RIGHT KEYS
			bsPlayerSync.Read(bHasLR);
			if (bHasLR) bsPlayerSync.Read(gPlayer[playerId].onfootData.sLeftRightKeys);

			// UP/DOWN KEYS
			bsPlayerSync.Read(bHasUD);
			if (bHasUD) bsPlayerSync.Read(gPlayer[playerId].onfootData.sUpDownKeys);

			// GENERAL KEYS
			bsPlayerSync.Read(gPlayer[playerId].onfootData.sKeys);

			// VECTOR POS
			bsPlayerSync.Read(gPlayer[playerId].onfootData.vecPos[0]);
			bsPlayerSync.Read(gPlayer[playerId].onfootData.vecPos[1]);
			bsPlayerSync.Read(gPlayer[playerId].onfootData.vecPos[2]);

			// ROTATION
			bsPlayerSync.ReadNormQuat(
				gPlayer[playerId].onfootData.fQuaternion[0],
				gPlayer[playerId].onfootData.fQuaternion[1],
				gPlayer[playerId].onfootData.fQuaternion[2],
				gPlayer[playerId].onfootData.fQuaternion[3]);

			// HEALTH/Armor (COMPRESSED INTO 1 BYTE)
			BYTE byteHealthArmor;
			BYTE byteHealth, byteArmor;
			BYTE byteArmTemp = 0, byteHlTemp = 0;

			bsPlayerSync.Read(byteHealthArmor);
			byteArmTemp = (byteHealthArmor & 0x0F);
			byteHlTemp = (byteHealthArmor >> 4);

			if (byteArmTemp == 0xF) byteArmor = 100;
			else if (byteArmTemp == 0) byteArmor = 0;
			else byteArmor = byteArmTemp * 7;

			if (byteHlTemp == 0xF) byteHealth = 100;
			else if (byteHlTemp == 0) byteHealth = 0;
			else byteHealth = byteHlTemp * 7;

			gPlayer[playerId].onfootData.byteHealth = byteHealth;
			gPlayer[playerId].onfootData.byteArmor = byteArmor;

			// CURRENT WEAPON
			bsPlayerSync.Read(gPlayer[playerId].onfootData.byteCurrentWeapon);

			// Special Action
			bsPlayerSync.Read(gPlayer[playerId].onfootData.byteSpecialAction);

			// READ MOVESPEED VECTORS
			bsPlayerSync.ReadVector(
				gPlayer[playerId].onfootData.vecMoveSpeed[0],
				gPlayer[playerId].onfootData.vecMoveSpeed[1],
				gPlayer[playerId].onfootData.vecMoveSpeed[2]);

			bsPlayerSync.Read(bHasSurfInfo);
			if (bHasSurfInfo) {
				bsPlayerSync.Read(gPlayer[playerId].onfootData.vecSurfOffsets[0]);
				bsPlayerSync.Read(gPlayer[playerId].onfootData.vecSurfOffsets[1]);
				bsPlayerSync.Read(gPlayer[playerId].onfootData.vecSurfOffsets[2]);
				bsPlayerSync.Read(gPlayer[playerId].onfootData.wSurfInfo);
			}
			else
				gPlayer[playerId].onfootData.wSurfInfo = -1;

			bsPlayerSync.Read(bAnimation);
			if (bAnimation) {
				bsPlayerSync.Read(gPlayer[playerId].onfootData.sAnimIndex);
				bsPlayerSync.Read(gPlayer[playerId].onfootData.sAnimFlags);
			}

			gPlayer[playerId].bPassenger = gPlayer[playerId].bAreWeInAVehicle = false;

			if (PBRecorder::iPlaybackType == Playback::PLAYBACK_TYPE_ONFOOT)
				if (playerId == PBRecorder::iGoalPlayer)
					PBRecorder::AddOnfootPacket(&gPlayer[playerId].onfootData);
		}

		void VehicleSync(Packet *p) {
			RakNet::BitStream bsSync((unsigned char *)p->data, p->length, false);
			PLAYERID playerId;

			VEHICLEID VehicleID;
			bool bLandingGear;
			bool bHydra, bTrain, bTrailer;
			bool bSiren;

			bsSync.IgnoreBits(8);
			bsSync.Read(playerId);
			bsSync.Read(VehicleID);

			if (playerId < 0 || playerId >= MAX_PLAYERS) return;
			if (VehicleID < 0 || VehicleID >= MAX_VEHICLES) return;

			memset(&gPlayer[playerId].incarData, 0, sizeof(INCAR_SYNC_DATA));

			gPlayer[playerId].incarData.VehicleID = VehicleID;

			gVehiclePool[VehicleID].bExist = true;

			// LEFT/RIGHT KEYS
			bsSync.Read(gPlayer[playerId].incarData.lrAnalog);

			// UP/DOWN KEYS
			bsSync.Read(gPlayer[playerId].incarData.udAnalog);

			// GENERAL KEYS
			bsSync.Read(gPlayer[playerId].incarData.wKeys);

			// ROLL / DIRECTION
			// ROTATION
			bsSync.ReadNormQuat(
				gPlayer[playerId].incarData.fQuaternion[0],
				gPlayer[playerId].incarData.fQuaternion[1],
				gPlayer[playerId].incarData.fQuaternion[2],
				gPlayer[playerId].incarData.fQuaternion[3]);

			// POSITION
			bsSync.Read(gPlayer[playerId].incarData.vecPos[0]);
			bsSync.Read(gPlayer[playerId].incarData.vecPos[1]);
			bsSync.Read(gPlayer[playerId].incarData.vecPos[2]);
			vect3_copy(gPlayer[playerId].incarData.vecPos, gVehiclePool[VehicleID].fPos);

			// SPEED
			bsSync.ReadVector(
				gPlayer[playerId].incarData.vecMoveSpeed[0],
				gPlayer[playerId].incarData.vecMoveSpeed[1],
				gPlayer[playerId].incarData.vecMoveSpeed[2]);

			// VEHICLE HEALTH
			WORD wTempVehicleHealth;
			bsSync.Read(wTempVehicleHealth);
			gPlayer[playerId].incarData.fCarHealth = (float)wTempVehicleHealth;
			gVehiclePool[VehicleID].fHealth = gPlayer[playerId].incarData.fCarHealth;

			// HEALTH/Armor (COMPRESSED INTO 1 BYTE)
			BYTE byteHealthArmor;
			BYTE bytePlayerHealth, bytePlayerArmor;
			BYTE byteArmTemp = 0, byteHlTemp = 0;

			bsSync.Read(byteHealthArmor);
			byteArmTemp = (byteHealthArmor & 0x0F);
			byteHlTemp = (byteHealthArmor >> 4);

			if (byteArmTemp == 0xF) bytePlayerArmor = 100;
			else if (byteArmTemp == 0) bytePlayerArmor = 0;
			else bytePlayerArmor = byteArmTemp * 7;

			if (byteHlTemp == 0xF) bytePlayerHealth = 100;
			else if (byteHlTemp == 0) bytePlayerHealth = 0;
			else bytePlayerHealth = byteHlTemp * 7;

			gPlayer[playerId].incarData.bytePlayerHealth = bytePlayerHealth;
			gPlayer[playerId].incarData.bytePlayerArmor = bytePlayerArmor;

			// CURRENT WEAPON
			bsSync.Read(gPlayer[playerId].incarData.byteCurrentWeapon);

			// SIREN
			bsSync.ReadCompressed(bSiren);
			if (bSiren)
				gPlayer[playerId].incarData.byteSirenOn = 1;

			// LANDING GEAR
			bsSync.ReadCompressed(bLandingGear);
			if (bLandingGear)
				gPlayer[playerId].incarData.byteLandingGearState = 1;

			// HYDRA THRUST ANGLE AND TRAILER ID
			bsSync.ReadCompressed(bHydra);
			bsSync.ReadCompressed(bTrailer);

			DWORD dwTrailerID_or_ThrustAngle;
			bsSync.Read(dwTrailerID_or_ThrustAngle);
			gPlayer[playerId].incarData.TrailerID_or_ThrustAngle = (WORD)dwTrailerID_or_ThrustAngle;

			// TRAIN SPECIAL
			WORD wSpeed;
			bsSync.ReadCompressed(bTrain);
			if (bTrain)
			{
				bsSync.Read(wSpeed);
				gPlayer[playerId].incarData.fTrainSpeed = (float)wSpeed;
			}

			gPlayer[playerId].bPassenger = false;
			gPlayer[playerId].bAreWeInAVehicle = true;

			if (PBRecorder::iPlaybackType == Playback::PLAYBACK_TYPE_DRIVER)
				if (playerId == PBRecorder::iGoalPlayer)
					PBRecorder::AddVehiclePacket(&gPlayer[playerId].incarData);
		}

		void PassengerSync(Packet *p) {
			RakNet::BitStream bsPassengerSync((unsigned char *)p->data, p->length, false);
			PLAYERID	playerId;
			PASSENGER_SYNC_DATA psSync;

			bsPassengerSync.IgnoreBits(8);
			bsPassengerSync.Read(playerId);

			if (playerId < 0 || playerId >= MAX_PLAYERS) return;

			bsPassengerSync.Read((PCHAR)&psSync, sizeof(PASSENGER_SYNC_DATA));

			// Followed wants to drive the vehicle
			gPlayer[playerId].passengerData.VehicleID = psSync.VehicleID;
			gPlayer[playerId].bAreWeInAVehicle = gPlayer[playerId].bPassenger = true;
		}

		void UnoccupiedSync(Packet *p) {
			RakNet::BitStream bsUnocSync((unsigned char *)p->data, p->length, false);
			PLAYERID playerId;

			bsUnocSync.IgnoreBits(8);
			bsUnocSync.Read(playerId);

			if (playerId < 0 || playerId >= MAX_PLAYERS) return;

			UNOCCUPIED_SYNC_DATA cache;
			bsUnocSync.Read((char *)&cache, sizeof(UNOCCUPIED_SYNC_DATA));

			vect3_copy(cache.vecPos, gVehiclePool[cache.VehicleID].fPos);
		}
	
		void AimSync(Packet *p) {
			RakNet::BitStream bsAimSync((unsigned char *)p->data, p->length, false);
			PLAYERID playerId;

			bsAimSync.IgnoreBits(8);
			bsAimSync.Read(playerId);

			if (playerId < 0 || playerId >= MAX_PLAYERS) return;

			memset(&gPlayer[playerId].aimData, 0, sizeof(AIM_SYNC_DATA));

			bsAimSync.Read((PCHAR)&gPlayer[playerId].aimData, sizeof(AIM_SYNC_DATA));
		}

		void MarkersSync(Packet *p) {
			RakNet::BitStream bsMarkersSync((unsigned char *)p->data, p->length, false);

			int i, iNumberOfPlayers;
			PLAYERID playerID;
			short sPosX, sPosY, sPosZ;
			bool bIsPlayerActive;

			bsMarkersSync.IgnoreBits(8);
			bsMarkersSync.Read(iNumberOfPlayers);

			if (iNumberOfPlayers < 0 || iNumberOfPlayers > MAX_PLAYERS) return;

			for (i = 0; i < iNumberOfPlayers; i++) {
				bsMarkersSync.Read(playerID);

				if (playerID < 0 || playerID >= MAX_PLAYERS) return;

				bsMarkersSync.ReadCompressed(bIsPlayerActive);
				if (bIsPlayerActive == 0) {
					gPlayer[playerID].bGotMarkersPos = false;
					continue;
				}

				bsMarkersSync.Read(sPosX);
				bsMarkersSync.Read(sPosY);
				bsMarkersSync.Read(sPosZ);

				gPlayer[playerID].bGotMarkersPos = true;
				gPlayer[playerID].onfootData.vecPos[0] = (float)sPosX;
				gPlayer[playerID].onfootData.vecPos[1] = (float)sPosY;
				gPlayer[playerID].onfootData.vecPos[2] = (float)sPosZ;

				//Log("Packet_MarkersSync: %d %d %0.2f, %0.2f, %0.2f", playerID, bIsPlayerActive, (float)sPosX, (float)sPosY, (float)sPosZ);
			}
		}

		void BulletSync(Packet *p) {
			RakNet::BitStream bsBulletSync((unsigned char *)p->data, p->length, false);
			
			PLAYERID PlayerID;
			
			bsBulletSync.IgnoreBits(8);
			bsBulletSync.Read(PlayerID);
			
			if (PlayerID < 0 || PlayerID >= MAX_PLAYERS) return;
			
			BULLET_SYNC_DATA bulletData;
			memset(&bulletData, 0, sizeof(BULLET_SYNC_DATA));
			
			bsBulletSync.Read((PCHAR)&bulletData, sizeof(BULLET_SYNC_DATA));

			if (gConfig.bDamage && iMode != RUNMODE_AFK) {
				if (fHealth > 0) {
					if (bulletData.iHitID == iPlayerID && bulletData.bHitType == BULLET_HIT_TYPE_PLAYER) {
						TakeBulletDamage(PlayerID, bulletData.bWeaponID);
					}
				}
			}

			if (iMode == RUNMODE_FOLLOW) {
				if (PlayerID == AI::iFollow) {
					AI::UpdateFollowBullet(&bulletData);
				}
			}
		}
	}
}