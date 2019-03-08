#include "twoWaySprite.h"
#include "gameData.h"
#include "imageFactory.h"

void TwoWaySprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % (numberOfFrames / 2);
		timeSinceLastFrame = 0;
	}
}

TwoWaySprite::TwoWaySprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
//  isBounded(false),
  minY(0),
  maxY(Gamedata::getInstance().getXmlInt("world/height")),
//  minX(0),
//  maxX(Gamedata::getInstance().getXmlInt("world/width")),
  walkingLeft(false),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ 
  std::cout<< "Initialized 2way of name " << name << std::endl;
  if (Gamedata::getInstance().getXmlInt(name+"/isRand") == 1){
    int Xmin = Gamedata::getInstance().getXmlInt(name+"/rangeX/min");
    int Xmax = Gamedata::getInstance().getXmlInt(name+"/rangeX/max");
    setX(Xmin + (rand()%(Xmax-Xmin)));

    int Ymin = Gamedata::getInstance().getXmlInt(name+"/rangeY/min");
    int Ymax = Gamedata::getInstance().getXmlInt(name+"/rangeY/max");
    setY(Ymin + (rand()%(Ymax-Ymin)));

    currentFrame = rand() % (numberOfFrames / 2);

    int randXspeed = Gamedata::getInstance().getXmlInt(name+"/rangeX/speed");
    int randDir = (rand() % 2) == 0 ? -1 : 1;
    if(randXspeed != 0){
      setVelocityX((getVelocityX() - (rand() % randXspeed)) * randDir);
    }
    randDir = (rand() % 2) == 0 ? -1 : 1;
    int randYspeed = Gamedata::getInstance().getXmlInt(name+"/rangeY/speed");
    if(randYspeed != 0){
      setVelocityY((getVelocityY() - (rand() % randYspeed)) * randDir);
    }
  }
  if (Gamedata::getInstance().getXmlInt(name+"/isBound") == 1){
//    isBounded = true;
    minY = Gamedata::getInstance().getXmlInt(name+"/rangeY/min");
    maxY = Gamedata::getInstance().getXmlInt(name+"/rangeY/max");
//    minX = Gamedata::getInstance().getXmlInt(name+"/rangeX/min");
//    maxX = Gamedata::getInstance().getXmlInt(name+"/rangeX/max");
  }
  if (getVelocityX() < 0){
    walkingLeft = true;
  }
}

TwoWaySprite::TwoWaySprite(const TwoWaySprite& s) :
  Drawable(s), 
  images(s.images),
//  isBounded(s.isBounded),
  minY(s.minY),
  maxY(s.maxY),
//  minX(s.minX),
//  maxX(s.maxX),
  walkingLeft(s.walkingLeft),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

TwoWaySprite& TwoWaySprite::operator=(const TwoWaySprite& s) {
  Drawable::operator=(s);
  images = (s.images);
//  isBounded = (s.isBounded);
  minY = (s.minY);
  maxY = (s.maxY);
//  minX = (s.minX);
//  maxX = (s.maxX);
  walkingLeft = (s.walkingLeft);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void TwoWaySprite::draw() const { 
  if(!walkingLeft){
    images[currentFrame]->draw(getX(), getY(), getScale());
  } else {
    images[numberOfFrames -(1 + currentFrame)]->draw(getX(),getY(),getScale());
  }
}

void TwoWaySprite::draw(bool directionLeft) const {
  if(!directionLeft){
    images[currentFrame]->draw(getX(), getY(), getScale());
  } else {
    images[numberOfFrames -(1 + currentFrame)]->draw(getX(),getY(),getScale());
  }
}

void TwoWaySprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0 || getY() < minY) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight() || getY() > maxY) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0 ){// || getX() < minX) {
    setVelocityX( fabs( getVelocityX() ) );
    walkingLeft = false;
  }
  if ( getX() > worldWidth-getScaledWidth()){// || getX() > maxX) {
    setVelocityX( -fabs( getVelocityX() ) );
    walkingLeft = true;
  }  

}
