#include <iostream>
//#include "multisprite.h"
#include "twoWaySprite.h"
#include "gameData.h"

//class Bullet : public MultiSprite {
class Bullet : public TwoWaySprite {
public:
  explicit Bullet(const string& name, bool direction) :
    //MultiSprite(name), 
    TwoWaySprite(name),
    distance(0), 
    maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")), 
    tooFar(false),
    goingLeft(direction)
  { }
  virtual void update(Uint32 ticks);
  virtual void draw() const;
  bool goneTooFar() const { return tooFar; }
  void reset(bool direction) {
    tooFar = false;
    distance = 0;
    goingLeft = direction;
  }

private:
  float distance;
  float maxDistance;
  bool tooFar;
  bool goingLeft;
};
