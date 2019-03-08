#ifndef PLAYER__H
#define PLAYER__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "bulletPool.h"
#include "explodingSprite.h"

class ExplodingSprite;

class Player : public Drawable {
public:
  Player(const std::string&);
  Player(const Player&);
  ~Player();

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual void explode();
  virtual bool isExploded();

  virtual const Image* getImage() const { 
    return images[currentFrame]; 
  }
  int getScaledWidth()  const { 
    return getScale()*images[currentFrame]->getWidth(); 
  } 
  int getScaledHeight()  const { 
    return getScale()*images[currentFrame]->getHeight(); 
  } 
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }

  void right(Uint32 ticks);
  void left(Uint32 ticks);
  void up(Uint32 ticks);
  void down(Uint32 ticks);
  void stop(Uint32 ticks);

  void shoot();
  bool collidedWith(const Drawable*) const;

private:
  std::vector<Image *> images;

  std::string bulletName;
  BulletPool bullets;
  float minSpeed;

  bool goingLeft;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  Vector2f initialVelocity;

  ExplodingSprite* explosion;

  void advanceFrame(Uint32 ticks);
  Player& operator=(const Player&);
};
#endif
