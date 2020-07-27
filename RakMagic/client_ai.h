#ifndef CLIENT_AI
#define CLIENT_AI

namespace RakMagic {
	namespace AI {
		extern int iFollow; //Player ID used in UpdateFollow

		//UpdateOnfoot: synchronize the local position to server
		void UpdateOnfoot();

		//UpdateVehicle: synchronize in a vehicle as the driver
		void UpdateVehicle(int vehicleid);

		//UpdateFollow: An AI function that make the bot follow a player
		void UpdateFollow();

		//UpdateFollowBullet: Copy & send a bullet which comes from the followed player
		void UpdateFollowBullet(BULLET_SYNC_DATA *data);
	}
}

#endif