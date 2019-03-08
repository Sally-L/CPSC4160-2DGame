#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "smartSprite.h"
#include "subjectSprite.h"
#include "sprite.h"
#include "multisprite.h"
#include "twoWaySprite.h"
#include "gameData.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "player.h"
#include "hud.h"
#include "shootingSprite.h"

Engine::~Engine() {
  delete player;
  delete strategy;
  for( auto& s : scaledSprites) {
    delete s;
  }
  for( auto& s : mySprites ) {
    delete s;
  } 
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  hud( HUD::getInstance() ),
  sound(),
  renderer( rc.getRenderer() ),
  field("field", Gamedata::getInstance().getXmlInt("field/factor") ),
  hills("hills", Gamedata::getInstance().getXmlInt("hills/factor") ),
  sky("sky", Gamedata::getInstance().getXmlInt("sky/factor") ),
  wonScreen("won", Gamedata::getInstance().getXmlInt("won/factor") ),
  lostScreen("lost", Gamedata::getInstance().getXmlInt("lost/factor") ),
  viewport( Viewport::getInstance() ),
  player(new SubjectSprite("Harry")),
  isPlayerAlive(true),
  inGodMode(false),
  playFinish(0),
  livesRemaining(Gamedata::getInstance().getXmlInt("totalLives")),
  decrementLife(false),
  score(0),
  winningScore(Gamedata::getInstance().getXmlInt("winningScore")),
  scaledSprites(),
  mySprites(),
  strategy(new PerPixelCollisionStrategy ),
  collision(false),
  drawHud(true),
  drawDirections(true),
  makeVideo( false )
{
  std::srand(Gamedata::getInstance().getXmlInt("randSeed")); // 21 or 19
  int numQuaffles = Gamedata::getInstance().getXmlInt("numberOfQuaffles");
  int numDementors = Gamedata::getInstance().getXmlInt("numberOfDementors");
  int numBludgers = Gamedata::getInstance().getXmlInt("numberOfBludgers");
  int numSnitch = Gamedata::getInstance().getXmlInt("numberOfSnitch");
  mySprites.reserve(numQuaffles + numDementors + numBludgers + numSnitch);
  Vector2f pos = player->getPosition();
  int w = player->getScaledWidth();
  int h = player->getScaledHeight();
  int spriteIndex = 0;
  for (int i = spriteIndex; i < (numDementors + spriteIndex); ++i) {
    mySprites.push_back( new SmartSprite("dementor", pos, w, h) );
    player->attach( mySprites[i] );
  }
  spriteIndex += numDementors;
  for (int i = spriteIndex; i < (numQuaffles + spriteIndex); ++i) {
    mySprites.push_back( new SmartSprite("quaffle", pos, w, h) );
    player->attach( mySprites[i] );
  }
  spriteIndex += numQuaffles;
  for (int i = spriteIndex; i < (numBludgers + spriteIndex); ++i) {
    mySprites.push_back( new SmartSprite("bludger", pos, w, h) );
    player->attach( mySprites[i] );
  }
  spriteIndex += numBludgers;
  for (int i = spriteIndex; i < (numSnitch + spriteIndex); ++i) {
    mySprites.push_back( new SmartSprite("snitch", pos, w, h) );
    player->attach( mySprites[i] );
  }

  // If I have scaled sprites, set scale here through v[0]->setScale(#);
  Viewport::getInstance().setObjectToTrack(player);
  std::cout << "Loading complete" << std::endl;

  // Create scaled sprites for painters algorithm
  numQuaffles = Gamedata::getInstance().getXmlInt("scaled/numberQuaffles");
  numDementors = Gamedata::getInstance().getXmlInt("scaled/numberDementors");
  numBludgers = Gamedata::getInstance().getXmlInt("scaled/numberBludgers");
  mySprites.reserve(numQuaffles + numDementors + numBludgers);
  float maxScale = Gamedata::getInstance().getXmlFloat("scaled/rangeScale/max");
  float minScale = Gamedata::getInstance().getXmlFloat("scaled/rangeScale/min");

  spriteIndex = 0;
  for (int i = spriteIndex; i < (numDementors + spriteIndex); ++i) {
    scaledSprites.push_back( new TwoWaySprite("dementor") );
    scaledSprites[i]->setScale(Gamedata::getInstance().getRandFloat(maxScale,minScale));
  }
  spriteIndex += numDementors;
  for (int i = spriteIndex; i < (numQuaffles + spriteIndex); ++i) {
    scaledSprites.push_back( new TwoWaySprite("quaffle") );
    scaledSprites[i]->setScale(Gamedata::getInstance().getRandFloat(maxScale,minScale));
  }
  spriteIndex += numQuaffles;
  for (int i = spriteIndex; i < (numBludgers + spriteIndex); ++i) {
    scaledSprites.push_back( new Sprite("bludger") );
    scaledSprites[i]->setScale(Gamedata::getInstance().getRandFloat(maxScale,minScale));
  }

}

void Engine::draw() const {
  sky.draw();
  hills.draw();
  field.draw();

  if (livesRemaining == 0){
    lostScreen.draw();
//    playFinish = 1;
    //sound[1];  // Death sound
    io.writeText("You lost! Press R to restart the game!", 350, 200);
    clock.pause();
  } else if (score >= winningScore) {
    wonScreen.draw();
    io.writeText("Gryffindor wins the Quidditch cup!", 350, 550);
    io.writeText("Press R to play again!", 375, 580);
    clock.pause();
  } else {
    for( auto& s : scaledSprites) {
      s->draw();
    }

    for( auto& s : mySprites ) {
      s->draw();
    }
    player->draw();

    viewport.draw();

    if (drawHud) { hud.draw();}
    if (drawDirections) {hud.drawDirections();}
    io.writeText("Sally Lee", 25, 
       (Gamedata::getInstance().getXmlInt("view/height")) - 50,
       {0xff, 0x11, 0x99, 255});

    std::stringstream strm;
    strm << "Score: " << score;
    io.writeText(strm.str(), 25, 25);
    if (inGodMode) {
      io.writeText("God Mode", 25, 50);
    } else {
      strm.clear();
      strm.str("");
      strm << "Lives: " << livesRemaining;
      io.writeText(strm.str(), 25, 50);
    }
  }
  SDL_RenderPresent(renderer);
}

void Engine::checkForCollisions() {
  auto it = mySprites.begin();
  while ( it != mySprites.end() ) {
    if ( static_cast<SmartSprite*>(*it)->toKill() 
         && static_cast<Player*>(player)->collidedWith(*it)
         && !(static_cast<SmartSprite*>(*it)->isExploding())) {
      // if obj hit by a bullet and bullet can explode it, explode obj
      static_cast<SmartSprite*>(*it)->explode();
      sound[6];
    } else if ( static_cast<SmartSprite*>(*it)->toAvoid() 
         && strategy->execute(*player, **it) 
         && !(static_cast<SmartSprite*>(*it)->isExploding())) {
      // if player collides with obj trying to avoide it, explode obj
      static_cast<SmartSprite*>(*it)->explode();
      score += static_cast<SmartSprite*>(*it)->earnPoints();
      if ((*it)->getName() == "quaffle"){
        sound[3];
      } else {
        sound[4];
      }
      //sound[1];
    } else if (!inGodMode && !(static_cast<SmartSprite*>(*it)->toAvoid()) 
         && strategy->execute(*player, **it) 
         && !(static_cast<SmartSprite*>(*it)->isExploding())) {
      // if player collides with obj trying to capture it, explode player
      // unless the game is in God mode
      static_cast<Player*>(player)->explode();
      //livesRemaining -= 1;
      decrementLife = true;
      sound[5];
    }
    ++it;
  }
}

void Engine::update(Uint32 ticks) {
  checkForCollisions();
  if (livesRemaining ==0 && playFinish == 0) {
    sound[1];  // Death or victory sound
    playFinish = 1;
  } else if ( score >= winningScore && playFinish == 0) {
    sound[2];
    playFinish = 2;
  }

  isPlayerAlive = !(player->Player::isExploded());
  for( auto& s : scaledSprites) {
    s->update(ticks);
  }

  for( auto& s : mySprites) {
    s->update(ticks, isPlayerAlive);
  }
  // If isPlayerAlive just changed from false to true, decrement life
  if (decrementLife && isPlayerAlive) {
    livesRemaining -= 1;
    decrementLife = false;
  }

  player->update(ticks);
  sky.update();
  hills.update();
  field.update();
  viewport.update(); // always update viewport last
}

bool Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  /*if (livesRemaining == 0 && playFinish == 0) {
    sound[1];  // Death or victory sound
    playFinish = 1;
  }*/

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_R] ) {
          clock.unpause();
          return true;
        }
        if (keystate[SDL_SCANCODE_G] ) {
          inGodMode = !inGodMode;
        }
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_SPACE] ) {
          player->shoot();
          sound[0];  // Expecto Patronum
        }
        if (keystate[SDL_SCANCODE_F1] ) {
          drawHud = hud.togglePresent();
        }
        if (keystate[SDL_SCANCODE_F2] ) {
          drawDirections = hud.toggleDirections();
        }
        if (keystate[SDL_SCANCODE_F3] ) {
          sound.toggleMusic();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // Eventually add I for invisibility cloak (god mode)
    // Spacebar to fire a patronus charm/ attack dementors

    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left(ticks);
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right(ticks);
      }
      if (keystate[SDL_SCANCODE_W]) {
        (player)->up(ticks);
      }
      if (keystate[SDL_SCANCODE_S]) {
        (player)->down(ticks);
      }
      draw();
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  return false;
}
