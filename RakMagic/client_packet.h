#ifndef CLIENT_PACKET
#define CLIENT_PACKET

namespace RakMagic {
	namespace Sync {
		void PlayerSync(Packet *p);
		void VehicleSync(Packet *p);
		void PassengerSync(Packet *p);
		void UnoccupiedSync(Packet *p);
		void AimSync(Packet *p);
		void MarkersSync(Packet *p);
		void BulletSync(Packet *p);
	}
}

#endif