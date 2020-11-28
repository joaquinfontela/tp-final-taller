#ifndef TP_FINAL_CHEST_H
#define TP_FINAL_CHEST_H

#include "Item.h"

class Chest : public Item {
 public:
  Chest();
  ~Chest();
  void pickUp(Player& player) override;
  bool canBePickedUpBy(Player& player);
};

#endif  // TP_FINAL_CHEST_H