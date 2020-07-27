#include "main.h"

const struct weapon_entry weapon_list[] = {
	{ 0, 0, -1, "Fist" },
	{ 1, 0, 331, "Brass Knuckles" },
	{ 2, 1, 333, "Golf Club" },
	{ 3, 1, 334, "Nitestick" },
	{ 4, 1, 335, "Knife" },
	{ 5, 1, 336, "Baseball Bat" },
	{ 6, 1, 337, "Shovel" },
	{ 7, 1, 338, "Pool Cue" },
	{ 8, 1, 339, "Katana" },
	{ 9, 1, 341, "Chainsaw" },
	{ 10, 10, 321, "Dildo 1" },
	{ 11, 10, 322, "Dildo 2" },
	{ 12, 10, 323, "Vibe 1" },
	{ 13, 10, 324, "Vibe 2" },
	{ 14, 10, 325, "Flowers" },
	{ 15, 10, 326, "Cane" },
	{ 16, 8, 342, "Grenade" },
	{ 17, 8, 343, "Teargas" },
	{ 18, 8, 344, "Molotov Cocktail" },
	{ -1, -1, -1, NULL },
	{ -1, -1, -1, NULL },
	{ -1, -1, -1, NULL },
	{ 22, 2, 346, "Pistol" },
	{ 23, 2, 347, "Silenced Pistol" },
	{ 24, 2, 348, "Desert Eagle" },
	{ 25, 3, 349, "Shotgun" },
	{ 26, 3, 350, "Sawn-Off Shotgun" },
	{ 27, 3, 351, "SPAZ12" },
	{ 28, 4, 352, "Micro UZI" },
	{ 29, 4, 353, "MP5" },
	{ 30, 5, 355, "AK47" },
	{ 31, 5, 356, "M4" },
	{ 32, 4, 372, "Tech9" },
	{ 33, 6, 357, "Country Rifle" },
	{ 34, 6, 358, "Sniper Rifle" },
	{ 35, 7, 359, "Rocket Launcher" },
	{ 36, 7, 360, "Heat Seeking RPG" },
	{ 37, 7, 361, "Flame Thrower" },
	{ 38, 7, 362, "Minigun" },
	{ 39, 8, 363, "Remote Explosives" },
	{ 40, 12, 364, "Detonator" },
	{ 41, 9, 365, "Spray Can" },
	{ 42, 9, 366, "Fire Extinguisher" },
	{ 43, 9, 367, "Camera" },
	{ 44, 11, 368, "NV Goggles" },
	{ 45, 11, 369, "IR Goggles" },
	{ 46, 11, 371, "Parachute" },
	{ -1, -1, -1, NULL }
};

const char *getWeaponName(int weaponid) {
	return (weapon_list + weaponid)->name;
}

void removeEmbeddedColor(char *dest, char *src, size_t length) {
	int iNonColorEmbeddedMsgLen = 0;
	for (size_t pos = 0; pos < length && src[pos] != '\0'; pos++) {
		if (!((*(unsigned char*)(&src[pos]) - 32) >= 0 && (*(unsigned char*)(&src[pos]) - 32) < 224))
			continue;

		if (pos + 7 < length) {
			if (src[pos] == '{' && src[pos + 7] == '}') {
				pos += 7;
				continue;
			}
		}

		dest[iNonColorEmbeddedMsgLen] = src[pos];
		iNonColorEmbeddedMsgLen++;
	}
}

bool IsVehicleIndexValid(int vehicleid) {
	if (vehicleid >= MAX_VEHICLES || vehicleid < 1) {
		return false;
	}
	if (!gVehiclePool[vehicleid].bExist) {
		return false;
	}
	return true;
}