#include "subjectSprite.h"
#include "smartSprite.h"
#include "gameData.h"
#include "renderContext.h"

SubjectSprite::SubjectSprite( const std::string& name) :
  Player(name), 
  observers()
//  explosion(nullptr)
{ }

SubjectSprite::SubjectSprite(const SubjectSprite& s) :
  Player(s), 
  observers( s.observers )
//  explosion(nullptr)
  { }

void SubjectSprite::detach( SmartSprite* o ) {
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    if ( *ptr == o ) {
      ptr = observers.erase(ptr);
      return;
    }
    ++ptr;
  }
}

/*void SubjectSprite::draw() const {
  if(explosion) {
    explosion->draw();
  } else {
    Player::draw();
  }
}*/

void SubjectSprite::update(Uint32 ticks) { 
  /*if (explosion) {
    explosion->update(ticks);
    if(explosion->chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }*/

  Player::update(ticks);
  std::list<SmartSprite*>::iterator ptr = observers.begin();
  while ( ptr != observers.end() ) {
    int offset = rand()%5;
    offset = offset*(rand()%2?-1:1);
    const Vector2f off(offset, offset);
    (*ptr)->setPlayerPos( getPosition()+off );
    ++ptr;
  }
}

/*void SubjectSprite::explode(){
  if(!explosion) {
    Sprite sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    sprite.setScale(getScale() );
    explosion = new ExplodingSprite(sprite);
  }
}*/
