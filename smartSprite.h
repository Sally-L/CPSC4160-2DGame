#ifndef SMARTSPRITE__H
#define SMARTSPRITE__H
#include <string>
#include "twoWaySprite.h"
#include "explodingSprite.h"

class ExplodingSprite;

class SmartSprite : public TwoWaySprite {
public:
  SmartSprite(const std::string&, const Vector2f& pos, int w, int h);
  SmartSprite(const SmartSprite&);
  SmartSprite& operator=(const SmartSprite&);
  virtual ~SmartSprite(); 

  virtual void draw() const;
  virtual void update(Uint32 ticks, bool isPlayerAlive);
  virtual void explode();
  void setPlayerPos(const Vector2f& p) { playerPos = p; }
  int earnPoints() const;
  virtual bool toKill() const;
  virtual bool toAvoid() const;
  virtual bool isExploding() const;
private:
  enum MODE {NORMAL, EVADE, ATTACK};
    Vector2f playerPos;
  int playerWidth;
  int playerHeight;
  MODE currentMode;
  float notifyDistance;
  int knowledgeUpDown;
  int knowledgeLeftRight;
  int dumbX;
  int dumbY;
  int pointVal;

  bool toReplace;
  bool isAvoiding;
  bool bulletKill;
  bool isDead;

  ExplodingSprite* explosion;

  void goLeft();
  void goRight();
  void goUp();
  void goDown();
};
#endif
