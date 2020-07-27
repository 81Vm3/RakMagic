#ifndef COMMON_H
#define COMMON_H

struct weapon_entry
{
	int			id;
	int			slot;
	int			model_id;
	const char	*name;
};

extern const struct weapon_entry weapon_list[];

const char *getWeaponName(int weaponid);
void removeEmbeddedColor(char *dest, char *src, size_t length);
bool IsVehicleIndexValid(int vehicleid);

#endif