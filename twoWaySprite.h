#ifndef TWOWAYSPRITE__H
#define TWOWAYSPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class TwoWaySprite : public Drawable {
public:
  TwoWaySprite(const std::string&);
  TwoWaySprite(const TwoWaySprite&);

  virtual void draw() const;
  virtual void draw(bool direction) const;
  virtual void update(Uint32 ticks);

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

protected:
  std::vector<Image *> images;

//  bool isBounded;
  int minY;
  int maxY;
//  int minX;
//  int maxX;
  bool walkingLeft;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
  TwoWaySprite& operator=(const TwoWaySprite&);
};
#endif
