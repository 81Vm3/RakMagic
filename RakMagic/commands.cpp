#include "main.h"

void cmd_unloadcmd(const char *args) {
	Interface::Console::UnregisterCommmands();
}

void cmd_helloworld(const char *args) {
	logPrintf("Hello world !");
}

void cmd_exit(const char *args) {
	Interface::bExit = true;
}

void cmd_kepos(const char *args) {
	gConfig.bKeepPosition ^= true;
	if (gConfig.bKeepPosition)
		logPrintf("[-] Enabled keeping position");
	else
		logPrintf("[x] Disabled keeping position");
}

void cmd_teleport(const char *args) {
	float pos[3];
	if (sscanf(args, "%f%f%f", &pos[0], &pos[1], &pos[2]) < 3) {
		logPrintf("Teleport to coords: [x] [y] [z]");
	}
	else {
		memcpy(RakMagic::fPosition, pos, (sizeof pos[0]) * 3);
	}
}

void cmd_tp(const char *args) {
	int playerid;
	if (sscanf(args, "%d", &playerid) < 1) {
		logPrintf("Teleport to a player: [player id]");
	} else {
		if (playerid < 0 || playerid >= MAX_PLAYERS) {
			logPrintf(ERROR_INCORRECTPID);
			return;
		}
		if (!gPlayer[playerid].Teleport())
			logPrintf("[x] Failed to obtain the player's location");
		else
			logPrintf("Teleported to %s", gPlayer[playerid].szPlayerName);
	}
}

void cmd_follow(const char *args) {
	int playerid;
	if (sscanf(args, "%d", &playerid) < 1) {
		logPrintf("Teleport to a player: [player id]");
	}
	else {
		if (playerid < 0 || playerid >= MAX_PLAYERS) {
			logPrintf(ERROR_INCORRECTPID);
			return;
		}
		RakMagic::AI::iFollow = playerid;
		logPrintf("You are now following %s (%d)", gPlayer[RakMagic::AI::iFollow].szPlayerName, RakMagic::AI::iFollow);
	}
}

void cmd_pick(const char *args) {
	int pickup;
	if (sscanf(args, "%d", &pickup) < 1) {
		logPrintf("Pick up a pickup object: [pickup id]");
	}
	else {
		RakNet::BitStream bsSend;
		bsSend.Write(pickup);
		RakMagic::pRakInterface->RPC(&RPC_PickedUpPickup, &bsSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, FALSE, UNASSIGNED_NETWORK_ID, NULL);
	}
}

void cmd_reconnect(const char *args) {
	char server[32];
	int port = 7777;
	if (sscanf(args, "%s%d", &server, &port) >= 0)
		RakMagic::ReconnectTo(server, port);
	else
		RakMagic::Reconnect();
}

void cmd_frame(const char *args) {
	logPrintf("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

void cmd_anim(const char *args) {
	char name[MAX_ANIMATION_NAME] = "\0";
	int bLoop = 1; int bLockX = 0; int bLockY = 0; int iTime = 0;
	if (sscanf(args, "%s%d%d%d%d", &name, &bLoop, &bLockX, &bLockY, &iTime) < 1) {
		logPrintf("Set an animation by name:");
		logPrintf("[lib:name] (loop=1) (lockX=0) (lockY=0) (time=0)");
	}
	else {
		WORD wAnimationId = CAnimationInfo::GetIndexByName(name);
		if (wAnimationId == 0) {
			logPrintf("[x] Could not find any animation named \"%s\"", name);
		}
		else {
			logPrintf("Animation has been changed to \"%s\" (%d)", name, wAnimationId);
			RakMagic::SetAnimation(wAnimationId, 4.1f, (bool)bLoop, (bool)bLockX, (bool)bLockY, false, iTime);
		}
	}
}

void cmd_ianim(const char *args) {
	int index = 0;
	int bLoop = 1; int bLockX = 0; int bLockY = 0; int iTime = 0;
	if (sscanf(args, "%d%d%d%d%d", &index, &bLoop, &bLockX, &bLockY, &iTime) < 1) {
		logPrintf("Set an animation by index:");
		logPrintf("[index] (loop=1) (lockX=0) (lockY=0) (time=0)");
	}
	else {
		if (index < 0 || index >= MAX_ANIMATIONS) {
			logPrintf("[x] Invalid Animation index");
		}
		else {
			if (index == 0)
				logPrintf("You stopped animating");
			else
				logPrintf("Animation has been changed to %d", index);
			RakMagic::SetAnimation((WORD)index, 4.1f, (bool)bLoop, (bool)bLockX, (bool)bLockY, false, iTime);
		}
	}
}

void cmd_ganim(const char *args) {
	if (RakMagic::sAnimIndex <= 0) {
		logPrintf("You aren't doing any animation");
	}
	else {
		char *name = CAnimationInfo::GetNameByIndex(RakMagic::sAnimIndex);
		logPrintf("You are doing \"%s\" (%d)", name, RakMagic::sAnimIndex);
	}
}

void cmd_health(const char *args) {
	float amount;
	if (sscanf(args, "%f", &amount) < 1) {
		logPrintf("Set health: [health (0.0 - 100.0)]");
		return;
	}

	if (amount > 100.0f || amount < 0.0f) {
		logPrintf("[x] Invalid value, value must set between 0.0 and 100.0");
		return;
	}

	RakMagic::fHealth = amount;
}

void cmd_armor(const char *args) {
	float amount;
	if (sscanf(args, "%f", &amount) < 1) {
		logPrintf("Set armor: [armor (0.0 - 100.0)]");
		return;
	}

	if (amount > 100.0f || amount < 0.0f) {
		logPrintf("[x] Invalid value, value must set between 0.0 and 100.0");
		return;
	}

	RakMagic::fArmor = amount;
}

void cmd_fillhp(const char *args) {
	RakMagic::fHealth = RakMagic::fArmor = 100.0f;
}

void cmd_veh(const char *args) {
	int vehicleid; 
	if (sscanf(args, "%d", &vehicleid) < 1) {
		logPrintf("Sync as a driver: [vehicle id (-1 for cancel)]");
	}
	else {
		RakMagic::iVehicle = vehicleid;
	}
}

void cmd_foffset(const char *args) {
	float pos[3];
	if (sscanf(args, "%f%f%f", &pos[0], &pos[1], &pos[2]) < 3) {
		logPrintf("Set follow offset: [x] [y] [z]");
	}
	else {
		memcpy(gConfig.fFollowOffset, pos, (sizeof pos[0]) * 3);
	}
}

void registerAllCommands() {
	using namespace Interface::Console;
	AddCommand("unload-cmd", cmd_unloadcmd);
	AddCommand("hello", cmd_helloworld);
	AddCommand("reconnect", cmd_reconnect);
	AddCommand("exit", cmd_exit);
	AddCommand("kepos", cmd_kepos);
	AddCommand("teleport", cmd_teleport);
	AddCommand("tp", cmd_tp);
	AddCommand("follow", cmd_follow);
	AddCommand("pick", cmd_pick);
	AddCommand("frame", cmd_frame);
	AddCommand("anim", cmd_anim);
	AddCommand("ianim", cmd_ianim);
	AddCommand("ganim", cmd_ganim);
	AddCommand("health", cmd_health);
	AddCommand("armor", cmd_armor);
	AddCommand("fillhp", cmd_fillhp);
	AddCommand("veh", cmd_veh);
	AddCommand("foffset", cmd_foffset);
}