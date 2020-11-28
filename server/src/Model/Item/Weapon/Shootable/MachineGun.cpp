#include "../../../../../includes/Model/Item/Weapon/Shootable/MachineGun.h"

MachineGun::MachineGun(int newAmmo) : Shootable(newAmmo) {}

MachineGun::~MachineGun() {}

void MachineGun::shoot() {
  ammo -= 5;
  if (ammo < 0) ammo = 0;
}