#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "player.h"
#include "hud.h"
#include "sound.h"

class CollisionStrategy;
class SmartSprite;
class SubjectSprite;

class Engine {
public:
  Engine ();
  ~Engine ();
  bool play();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
private:
  const RenderContext& rc;
  const IoMod& io;
  Clock& clock;
  HUD& hud;
  SDLSound sound;

  SDL_Renderer * const renderer;
  World field;
  World hills;
  World sky;
  World wonScreen;
  World lostScreen;
  Viewport& viewport;
  SubjectSprite* player;
  bool isPlayerAlive;
  bool inGodMode;

  int playFinish;
  int livesRemaining;
  bool decrementLife;
  int score;
  int winningScore;

  std::vector<Drawable*> scaledSprites;
  std::vector<SmartSprite*> mySprites;
  CollisionStrategy* strategy;
  bool collision;
  bool drawHud;
  bool drawDirections;
  bool makeVideo;

  void draw() const;
  void update(Uint32);

  void printScales() const;
  void checkForCollisions();
};
