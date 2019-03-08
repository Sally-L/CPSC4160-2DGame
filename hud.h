#ifndef HUD__H
#define HUD__H
#include <sstream>
#include <iostream>
#include <string>
#include <SDL.h>
#include "ioMod.h"
#include "gameData.h"
#include "viewport.h"
#include "clock.h"

class HUD {
public:
  static HUD& getInstance();
  //~HUD();
  void draw();
  void drawDirections();
  bool togglePresent();
  bool toggleDirections();
private:
  bool isPresent;
  bool isDirectionsPresent;
  int height;
  int width;
  int xLoc;
  int yLoc;
  int heightDir;
  int widthDir;
  int xLocDir;

  string move;
  string shoot;
  string toggle;
  string toggleDir;
  string toggleSound;
  string pause;
  string restart;
  string godMode;
  string quit;
  string direction1;
  string direction2;
  string direction3;

  //Viewport& viewport;
  Clock& clock;
  SDL_Renderer* renderer;
  IoMod& io;

  SDL_Color boxColor;

  HUD();
  HUD(const HUD&) = delete;
  HUD& operator=(const HUD&) = delete;
};
#endif
