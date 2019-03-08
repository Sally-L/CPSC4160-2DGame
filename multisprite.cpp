#include "multisprite.h"
#include "gameData.h"
#include "imageFactory.h"

void MultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

MultiSprite::MultiSprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  minY(0),
  maxY(Gamedata::getInstance().getXmlInt("world/height")),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ 
if (Gamedata::getInstance().getXmlInt(name+"/isRand") == 1){
    int Xmin = Gamedata::getInstance().getXmlInt(name+"/rangeX/min");
    int Xmax = Gamedata::getInstance().getXmlInt(name+"/rangeX/max");
    setX(Xmin + (rand()%(Xmax-Xmin)));

    int Ymin = Gamedata::getInstance().getXmlInt(name+"/rangeY/min");
    int Ymax = Gamedata::getInstance().getXmlInt(name+"/rangeY/max");
    setY(Ymin + (rand()%(Ymax-Ymin)));

    currentFrame = rand() % (numberOfFrames / 2);

    int randXspeed = Gamedata::getInstance().getXmlInt(name+"/rangeX/speed");
    if(randXspeed != 0){
      setVelocityX(getVelocityX() - (rand() % randXspeed));
    }
    int randYspeed = Gamedata::getInstance().getXmlInt(name+"/rangeY/speed");
    if(randYspeed != 0){
      setVelocityY(getVelocityY() - (rand() % randYspeed));
    }
  }

  if (Gamedata::getInstance().getXmlInt(name+"/isBound") == 1){
//    isBounded = true;
    minY = Gamedata::getInstance().getXmlInt(name+"/rangeY/min");
    maxY = Gamedata::getInstance().getXmlInt(name+"/rangeY/max");
  }
}

MultiSprite::MultiSprite(const MultiSprite& s) :
  Drawable(s), 
  images(s.images),
  minY(s.minY),
  maxY(s.maxY),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight )
  { }

MultiSprite& MultiSprite::operator=(const MultiSprite& s) {
  Drawable::operator=(s);
  images = (s.images);
  minY = (s.minY);
  maxY = (s.maxY);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  return *this;
}

void MultiSprite::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale());
}

void MultiSprite::update(Uint32 ticks) { 
  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0 || getY() < minY) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight() || getY() > maxY) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
  }  

}
