#include "../../../includes/Control/UpdatableEvent/ChangeDoorStatus.h"
#include "../../../includes/Control/Notification/DoorMoving.h"


ChangeDoorStatus::ChangeDoorStatus(int x, int y) : Updatable(), timeRequired(2.0f), x(x), y(y){}

void ChangeDoorStatus::update(float timeElapsed, Game& game){

  this->timeElapsed += timeElapsed;

  std::cout<<"Door Time elapsed: "<<this->timeElapsed<<std::endl;

  if(this->timeElapsed > this->timeRequired && !done){
    std::cout<<"Closing Door."<<std::endl;
    game.forceDoorStatusChange(x, y);
    this->hasToBeNotified = true;
    this->done = true;
  }


}

bool ChangeDoorStatus::notify(WaitingQueue<Notification*>& notif){

  if(!this->hasToBeNotified)
    return false;

  DoorMoving* noti = new DoorMoving(x, y);
  notif.push(noti);

  if(done)            // Si devuelve True, deberia eliminarse de la lista de Updatables.
    return true;

  return false;
}
