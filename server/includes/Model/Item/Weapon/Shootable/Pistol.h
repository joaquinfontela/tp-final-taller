#include "Shootable.h"

class Pistol : public Shootable {
 public:
  Pistol(int newAmmo);
  ~Pistol();
  void shoot();
};