#include <cmath>
#include <random>
#include <functional>
#include "smartSprite.h"
#include "gameData.h"
#include "renderContext.h"

SmartSprite::~SmartSprite() { if(explosion) delete explosion; }

float distance(float x1, float y1, float x2, float y2) {
  float x = x1-x2;
  float y = y1-y2;
  return hypot(x, y);
}

void SmartSprite::goLeft()  { 
  setVelocityX( -abs(getVelocityX()) );
  walkingLeft = true;
}
void SmartSprite::goRight() { 
  setVelocityX( fabs(getVelocityX()) );
  walkingLeft = false;
}
void SmartSprite::goUp()    { setVelocityY( -fabs(getVelocityY()) ); }
void SmartSprite::goDown()  { setVelocityY( fabs(getVelocityY()) );  }


SmartSprite::SmartSprite(const std::string& name, const Vector2f& pos, 
  int w, int h) :
  TwoWaySprite(name),
  playerPos(pos),
  playerWidth(w),
  playerHeight(h),
  currentMode(NORMAL),
  notifyDistance(Gamedata::getInstance().getXmlFloat(name+"/notifyDistance")),
  knowledgeUpDown(Gamedata::getInstance().getXmlInt(name+"/knowledgeOdds/upDown")),
  knowledgeLeftRight(Gamedata::getInstance().getXmlInt(name+"/knowledgeOdds/leftRight")),
  dumbX(Gamedata::getInstance().getXmlInt(name+"/dumbRange/x")),
  dumbY(Gamedata::getInstance().getXmlInt(name+"/dumbRange/y")),
  pointVal(Gamedata::getInstance().getXmlInt(name+"/pointVal")),
  toReplace(Gamedata::getInstance().getXmlBool(name+"/toReplace")),
  isAvoiding(Gamedata::getInstance().getXmlBool(name+"/isAvoiding")),
  bulletKill(Gamedata::getInstance().getXmlBool(name+"/bulletKill")),
  isDead(false),
  explosion(nullptr)
{}


SmartSprite::SmartSprite(const SmartSprite& s) : 
  TwoWaySprite(s),
  playerPos(s.playerPos),
  playerWidth(s.playerWidth),
  playerHeight(s.playerHeight),
  currentMode(s.currentMode),
  notifyDistance(s.notifyDistance),
  knowledgeUpDown(s.knowledgeUpDown),
  knowledgeLeftRight(s.knowledgeLeftRight),
  dumbX(s.dumbX),
  dumbY(s.dumbY),
  pointVal(s.pointVal),
  toReplace(s.toReplace),
  isAvoiding(s.isAvoiding),
  bulletKill(s.bulletKill),
  isDead(s.isDead),
  explosion(nullptr)
{}

SmartSprite& SmartSprite::operator=(const SmartSprite& s) {
  TwoWaySprite::operator=(s);
  playerPos = (s.playerPos);
  playerWidth = (s.playerWidth);
  playerHeight = (s.playerHeight);
  currentMode = (s.currentMode);
  notifyDistance = (s.notifyDistance);
  knowledgeUpDown = (s.knowledgeUpDown);
  knowledgeLeftRight = (s.knowledgeLeftRight);
  dumbX = (s.dumbX);
  dumbY = (s.dumbY);
  pointVal = (s.pointVal);
  toReplace = (s.toReplace);
  isAvoiding = (s.isAvoiding);
  bulletKill = (s.bulletKill);
  isDead = (s.isDead);
  explosion = (nullptr);
  return *this;
}

int SmartSprite::earnPoints() const {
  return pointVal;
}

bool SmartSprite::toKill() const {
  return bulletKill;
}

bool SmartSprite::toAvoid() const {
  return isAvoiding;
}

bool SmartSprite::isExploding() const {
  return isDead;
}

void SmartSprite::draw() const {
  if (explosion) {
    explosion->draw();
  } else {
    TwoWaySprite::draw();
  }
}

void SmartSprite::update(Uint32 ticks, bool isPlayerAlive) { 
  if (explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
      isDead = false;
    }
    return;
  }

  TwoWaySprite::update(ticks);
  float x= getX()+getImage()->getWidth()/2;
  float y= getY()+getImage()->getHeight()/2;
  float ex= playerPos[0]+playerWidth/2;
  float ey= playerPos[1]+playerHeight/2;
  float distanceToEnemy = ::distance( x, y, ex, ey );

  if  ( currentMode == NORMAL ) {
    if(distanceToEnemy < notifyDistance && isAvoiding) currentMode = EVADE;
    else if(distanceToEnemy < notifyDistance && !isAvoiding) currentMode = ATTACK;
  }
  else if  ( currentMode == EVADE || !isPlayerAlive) {
    if(distanceToEnemy > notifyDistance) currentMode=NORMAL;
    else {
      int hasKnowledgeLR = rand() % knowledgeLeftRight;
      if ( x+dumbX < ex && hasKnowledgeLR == 0) goLeft();
      else if ( x-dumbX > ex && hasKnowledgeLR == 0) goRight();

      int hasKnowledgeUD = rand() % knowledgeUpDown;
      if ( y+dumbY < ey && hasKnowledgeUD == 0) goUp();
      else if ( y-dumbY > ey && hasKnowledgeUD == 0) goDown();
    }
  } else if (isPlayerAlive) {
    // Mode == ATTACK 
    if(distanceToEnemy > notifyDistance) currentMode=NORMAL;
    else {
      int hasKnowledgeLR = rand() % knowledgeLeftRight;
      if ( x+dumbX < ex && hasKnowledgeLR == 0) goRight();
      else if ( x-dumbX > ex && hasKnowledgeLR == 0) goLeft();
      int hasKnowledgeUD = rand() % knowledgeUpDown;
      if ( y+dumbY < ey && hasKnowledgeUD == 0) goDown();
      else if ( y-dumbY > ey && hasKnowledgeUD == 0) goUp();
    }
  }
}

void SmartSprite::explode(){
  if (!explosion) {
    isDead = true;
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
  }
}
