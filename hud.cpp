#include "hud.h"
#include "renderContext.h"
#include <SDL2/SDL.h>

HUD& HUD::getInstance() {
  static HUD instance;
  return instance;
}

HUD::HUD() :
  isPresent(true),
  isDirectionsPresent(true),
  height(Gamedata::getInstance().getXmlInt("HUD/height")),
  width(Gamedata::getInstance().getXmlInt("HUD/width")),
  xLoc(Gamedata::getInstance().getXmlInt("HUD/xLoc")),
  yLoc(Gamedata::getInstance().getXmlInt("HUD/yLoc")),
  heightDir(Gamedata::getInstance().getXmlInt("HUD/direction/height")),
  widthDir(Gamedata::getInstance().getXmlInt("HUD/direction/width")),
  xLocDir(Gamedata::getInstance().getXmlInt("HUD/direction/xLoc")),
  move(Gamedata::getInstance().getXmlStr("HUD/move")),
  shoot(Gamedata::getInstance().getXmlStr("HUD/shoot")),
  toggle(Gamedata::getInstance().getXmlStr("HUD/toggle")),
  toggleDir(Gamedata::getInstance().getXmlStr("HUD/toggleDir")),
  toggleSound(Gamedata::getInstance().getXmlStr("HUD/toggleSound")),
  pause(Gamedata::getInstance().getXmlStr("HUD/pause")),
  restart(Gamedata::getInstance().getXmlStr("HUD/restart")),
  godMode(Gamedata::getInstance().getXmlStr("HUD/godMode")),
  quit(Gamedata::getInstance().getXmlStr("HUD/quit")),
  direction1(Gamedata::getInstance().getXmlStr("HUD/direction/dir1")),
  direction2(Gamedata::getInstance().getXmlStr("HUD/direction/dir2")),
  direction3(Gamedata::getInstance().getXmlStr("HUD/direction/dir3")),
  clock(Clock::getInstance()),
  renderer(RenderContext::getInstance().getRenderer() ),
  io(IoMod::getInstance()),
  boxColor({0xff, 0, 0, 0})
{
  boxColor.r = Gamedata::getInstance().getXmlInt("HUD/red");
  boxColor.g = Gamedata::getInstance().getXmlInt("HUD/green");
  boxColor.b = Gamedata::getInstance().getXmlInt("HUD/blue");
  boxColor.a = Gamedata::getInstance().getXmlInt("HUD/transparency");
}

bool HUD::togglePresent() {
  isPresent = !isPresent;
  return isPresent;
}

bool HUD::toggleDirections() {
  isDirectionsPresent = !isDirectionsPresent;
  return isDirectionsPresent;
}

void HUD::draw() {
  int x = xLoc+5;
  int y = yLoc+5;

  SDL_Rect r;
  r.x = xLoc;
  r.y = yLoc;
  r.w = width;
  r.h = height;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);

  // Render rect
  SDL_RenderFillRect( renderer, &r );

  SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
  SDL_RenderDrawRect( renderer, &r );

  // Draw text to screen with frames per second
  std::stringstream strm0;
  strm0 << "fps: " << clock.getFps();
  const std::string& toPrint( strm0.str() );
  io.writeText(toPrint, x, y);

  // Give directions on how to move
  y += 30;
  io.writeText(move, x, y);
  y += 30;
  io.writeText(shoot, x, y);
  y += 30;
  io.writeText(toggle, x, y);
  y += 30;
  io.writeText(toggleDir, x, y);
  y += 30;
  io.writeText(toggleSound, x, y);
  y += 30;
  io.writeText(pause, x, y);
  y += 30;
  io.writeText(restart, x, y);
  y += 30;
  io.writeText(godMode, x, y);
  y += 30;
  io.writeText(quit, x, y);
}

void HUD::drawDirections() {
  int x = xLocDir+5;
  int y = yLoc+5;

  SDL_Rect r;
  r.x = xLocDir;
  r.y = yLoc;
  r.w = widthDir;
  r.h = heightDir;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  SDL_SetRenderDrawColor( renderer, boxColor.r, boxColor.g, boxColor.b, boxColor.a);

  // Render rect
  SDL_RenderFillRect( renderer, &r );

  SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
  SDL_RenderDrawRect( renderer, &r );

  io.writeText(direction1, x, y);
  y += 30;
  io.writeText(direction2, x, y);
  y += 30;
  io.writeText(direction3, x, y);
}
