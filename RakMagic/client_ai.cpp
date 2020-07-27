#include "main.h"

namespace RakMagic {
	namespace AI {
		int iFollow = 0;

		void UpdateOnfoot() {
			if (gConfig.bInvisible) {
				SPECTATOR_SYNC_DATA data;
				memset(&data, 0, sizeof(SPECTATOR_SYNC_DATA));
				memcpy(data.vecPos, fPosition, (sizeof fPosition[0]) * 3);

				RakNet::BitStream bs;
				bs.Write((BYTE)ID_SPECTATOR_SYNC);
				bs.Write((PCHAR)&data, sizeof(SPECTATOR_SYNC_DATA));
				pRakInterface->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			}
			else {
				ONFOOT_SYNC_DATA data;
				memset(&data, 0, sizeof(ONFOOT_SYNC_DATA));

				data.byteHealth = (BYTE)fHealth;
				data.byteArmor = (BYTE)fArmor;
				memcpy(data.vecPos, fPosition, (sizeof fPosition[0]) * 3);

				data.sAnimIndex = sAnimIndex;
				data.sAnimFlags = sAnimFlags;

				RakNet::BitStream bs;
				bs.Write((BYTE)ID_PLAYER_SYNC);
				bs.Write((PCHAR)&data, sizeof(ONFOOT_SYNC_DATA));
				pRakInterface->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			}
			iState = RSTATE_ONFOOT;
		}

		void UpdateVehicle(int vehicleid) {
			if (!IsVehicleIndexValid(vehicleid))
				return;

			INCAR_SYNC_DATA data;
			memset(&data, 0, sizeof(INCAR_SYNC_DATA));

			data.VehicleID = vehicleid;
			data.fCarHealth = gVehiclePool[vehicleid].fHealth;

			data.bytePlayerHealth = (BYTE)fHealth;
			data.bytePlayerArmor = (BYTE)fArmor;

			memcpy(fPosition, gVehiclePool[vehicleid].fPos, (sizeof(float)) * 3);
			memcpy(data.vecPos, fPosition, (sizeof(float)) * 3);

			RakNet::BitStream bs;
			bs.Write((BYTE)ID_VEHICLE_SYNC);
			bs.Write((PCHAR)&data, sizeof(INCAR_SYNC_DATA));
			pRakInterface->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
			iState = RSTATE_DRIVER;
		}

		void UpdateFollow() {
			if (!gPlayer[iFollow].bIsConnected)
				return;

			if (!gPlayer[iFollow].bIsStreamedIn) {
				//Too far away
				gPlayer[iFollow].Teleport();
				UpdateOnfoot();
				return;
			}

			if (gPlayer[iFollow].bAreWeInAVehicle) {
				//The target is in a vehicle
				if (gVehiclePool[gPlayer[iFollow].incarData.VehicleID].bExist) {
					//If we are in the vehicle as a passenger, then we skip the distance examination
					if ((iState == RSTATE_PASSENGER) || (vect3_dist(gPlayer[iFollow].GetPosition(), fPosition) < gConfig.fEnterDistance)) {
						if (!gPlayer[iFollow].bPassenger) {
							PASSENGER_SYNC_DATA psSync;
							memset(&psSync, 0, sizeof(PASSENGER_SYNC_DATA));

							psSync.VehicleID = gPlayer[iFollow].incarData.VehicleID;
							psSync.byteSeatFlags = 1;

							//Remember to sync the local position:
							vect3_copy(gPlayer[iFollow].incarData.vecPos, psSync.vecPos);
							vect3_copy(gPlayer[iFollow].incarData.vecPos, fPosition);

							psSync.bytePlayerHealth = (BYTE)fHealth;
							psSync.bytePlayerArmour = (BYTE)fArmor;

							RakNet::BitStream bsPassengerSync;
							bsPassengerSync.Write((BYTE)ID_PASSENGER_SYNC);
							bsPassengerSync.Write((PCHAR)&psSync, sizeof(PASSENGER_SYNC_DATA));
							pRakInterface->Send(&bsPassengerSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

							iState = RSTATE_PASSENGER;
						}
						else {

						}
					}
					else {
						UpdateOnfoot();
						//Go to the vehicle
					}
				}
			}
			else {
				//The target is walking, not in a vehicle
				//All we have to do is copy his packet and modify a bit
				ONFOOT_SYNC_DATA data = gPlayer[iFollow].onfootData;

				if (gConfig.bCopyHealth) {
					fHealth = data.byteHealth;
					fArmor = data.byteArmor;
				}
				else {
					data.byteHealth = (BYTE)fHealth;
					data.byteArmor = (BYTE)fArmor;
				}

				if (gConfig.bCopyWeapon) {
					iWeapon = data.byteCurrentWeapon;
				}
				else {
					data.byteCurrentWeapon = iWeapon;
				}

				vect3_vect3_add(gConfig.fFollowOffset, data.vecPos, data.vecPos);
				vect3_copy(data.vecPos, fPosition);

				RakNet::BitStream bs;
				bs.Write((BYTE)ID_PLAYER_SYNC);
				bs.Write((PCHAR)&data, sizeof(ONFOOT_SYNC_DATA));
				pRakInterface->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

				bs.Reset();
				AIM_SYNC_DATA aimSync = gPlayer[iFollow].aimData;
				bs.Write((BYTE)ID_AIM_SYNC);
				bs.Write((PCHAR)&aimSync, sizeof(AIM_SYNC_DATA));
				pRakInterface->Send(&bs, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);

				iState = RSTATE_ONFOOT;
			}
		}
		void UpdateFollowBullet(BULLET_SYNC_DATA *data) {

			if (data->bHitType == BULLET_HIT_TYPE_PLAYER && data->iHitID == iPlayerID)
				return;
			
			//If we enabled bCopyWeapon, the following should be equivalent
			if (data->bWeaponID != iWeapon)
				return;

			RakNet::BitStream bsBulletSync;

			bsBulletSync.Write((BYTE)ID_BULLET_SYNC);
			bsBulletSync.Write((PCHAR)data, sizeof(BULLET_SYNC_DATA));

			pRakInterface->Send(&bsBulletSync, HIGH_PRIORITY, UNRELIABLE_SEQUENCED, 0);
		}
	}
}