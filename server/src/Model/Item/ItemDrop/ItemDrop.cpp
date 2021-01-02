#include "../../../../includes/Model/Item/ItemDrop/ItemDrop.h"

ItemDrop::ItemDrop(int itemId) : id(itemId) {
  ItemFactory it;

  this->item = it.getItem(itemId);
}

bool ItemDrop::canBePickedUpBy(Player &player) {
  return item->canBePickedUpBy(player);
}

void ItemDrop::pickUp(Player &player) { item->pickUp(player); }

bool ItemDrop::pickUpIfPossible(Player &player) {
  bool wasPickedUp;
  if ((wasPickedUp = canBePickedUpBy(player))) pickUp(player);
  return wasPickedUp;
}

// ItemDrop::~ItemDrop() { delete this->item; }
