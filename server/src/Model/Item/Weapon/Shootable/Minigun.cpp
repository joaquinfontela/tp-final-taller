#include "../../../../../includes/Model/Item/Weapon/Shootable/Minigun.h"

int Minigun::getID() { return this->ID; }

bool Minigun::hasAmmo() { return this->ammo > 0; }
