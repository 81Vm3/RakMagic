#include "main.h"

struct stVehiclePool gVehiclePool[MAX_VEHICLES];
struct stPickupPool gPickupPool[MAX_PICKUPS];

int WINAPI wWinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	PWSTR       lpCmdLine,
	int         nCmdShow
) {

	if (!logOpen("log.txt"))
		return 1;

	if (!loadConfig(&gConfig, gDefaultConfigPath)) {
		log("Failed to load XML configurations");
		logClose();
		return 1;
	}

	if (!RakMagic::Initialize()) {
		log("Failed to initalize RakNet");
		logClose();
		return 1;
	}

	if (!Interface::Initialize()) {
		log("Failed to initalize interface");
		logClose();
		return 1;
	}

	while (!Interface::ShouldClose()) {
		RakMagic::Update();
		Interface::Render();
	}
	Interface::Uninitialize();

	if (Playback::iPlaybackType != Playback::PLAYBACK_TYPE_NONE)
		Playback::Unload();

	if (RakMagic::IsConnected())
		RakMagic::Disconnect();

	RakMagic::Uninitialize();

	logClose();
	return 0;
}
