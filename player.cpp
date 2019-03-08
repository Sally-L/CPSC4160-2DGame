#include "player.h"
#include "gameData.h"
#include "imageFactory.h"

Player::~Player() {if(explosion) delete explosion; }

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % (numberOfFrames / 2);
		timeSinceLastFrame = 0;
	}
}

Player::Player( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  images( ImageFactory::getInstance().getImages(name) ),
  bulletName(Gamedata::getInstance().getXmlStr(name+"/bullet") ),
  bullets(bulletName),
  minSpeed( Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  goingLeft(false),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  initialVelocity(getVelocity()),
  explosion(nullptr)
{}

Player::Player(const Player& s) :
  Drawable(s), 
  images(s.images),
  bulletName(s.bulletName),
  bullets(s.bullets),
  minSpeed(s.minSpeed),
  goingLeft(s.goingLeft),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  initialVelocity( s.initialVelocity ),
  explosion(nullptr)
  { }

Player& Player::operator=(const Player& s) {
  Drawable::operator=(s);
  images = (s.images);
  bulletName = (s.bulletName);
  //bullets = (s.bullets);
  minSpeed = (s.minSpeed);
  goingLeft = (s.goingLeft);
  currentFrame = (s.currentFrame);
  numberOfFrames = ( s.numberOfFrames );
  frameInterval = ( s.frameInterval );
  timeSinceLastFrame = ( s.timeSinceLastFrame );
  worldWidth = ( s.worldWidth );
  worldHeight = ( s.worldHeight );
  initialVelocity = ( s.initialVelocity );
  explosion = (nullptr);
  return *this;
}

/*inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}*/

void Player::draw() const { 
  //if(getScale() < SCALE_EPSILON) return;
  if (explosion) {
    explosion->draw();
  } else if(!goingLeft){
    images[currentFrame]->draw(getX(), getY(), getScale());
  } else {
    images[numberOfFrames -(1 + currentFrame)]->draw(getX(),getY(),getScale());
  }
  bullets.draw();
}

void Player::stop(Uint32 ticks) { 
  advanceFrame(ticks);
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0.5*getVelocityX() );
  setVelocityY(0);
}

void Player::right(Uint32 ticks) { 
  advanceFrame(ticks);
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    goingLeft = false;
  }
} 
void Player::left(Uint32 ticks)  { 
  advanceFrame(ticks);
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    goingLeft = true;
  }
} 
void Player::up(Uint32 ticks)    { 
  advanceFrame(ticks);
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
  }
} 
void Player::down(Uint32 ticks)  { 
  advanceFrame(ticks);
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
  }
}

void Player::update(Uint32 ticks) { 
  advanceFrame(ticks);
  if (explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
  } else {
    //advanceFrame(ticks);
    Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
    setPosition(getPosition() + incr);

    if ( getY() < 0) {
      setVelocityY( 0 );
    }
    if ( getY() > worldHeight-getScaledHeight()) {
      setVelocityY( 0 );
    }

    if ( getX() < 0) {
      setVelocityX( 0 );
    }
    if ( getX() > worldWidth-getScaledWidth()) {
      setVelocityX( 0 );
    }
    stop(ticks);
  }
  bullets.update(ticks);
}

void Player::shoot() {
  if (!explosion) {
    float x = getX();
    float y = getY()+getImage()->getHeight()/2;
    float velocityX;

    if(goingLeft) {
      x -= getImage()->getWidth()/4;
      velocityX = getVelocityX() - minSpeed;
    } else {
      x += getImage()->getWidth()/2;
      velocityX = getVelocityX() + minSpeed;
    }
    // I'm not adding minSpeed to y velocity:
    bullets.shoot( Vector2f(x, y),
      Vector2f(velocityX, 0), goingLeft
    );
  }
}

bool Player::collidedWith(const Drawable* obj) const {
  return bullets.collidedWith( obj );
}

void Player::explode(){
  if (!explosion) {
    if(!goingLeft) {
      Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
      sprite.setScale( getScale() );
      explosion = new ExplodingSprite(sprite);
    } else {
      Sprite sprite(getName(), getPosition(), getVelocity(), images[numberOfFrames -(1 + currentFrame)]);
      sprite.setScale( getScale() );
      explosion = new ExplodingSprite(sprite);
    }
  }
}

bool Player::isExploded() {
/*  if (!(!explosion) == true) {
    std::cout << "I exploded!" << std::endl;
  }*/
  return !(!explosion);
}
