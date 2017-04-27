#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>
#include <stdlib.h> 
#include <string>
#include <time.h>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFAsset.h"

/**
 * Represents the StarshipFontana application.  It has responsibilities for
 * * Creating and destroying the app window
 * * Processing game events
 */
class SFApp {
public:
  SFApp(std::shared_ptr<SFWindow>);
  virtual ~SFApp();
  void    OnEvent(SFEvent &);
  int     OnExecute();
  void    OnUpdateWorld();
  void    OnRender();
  void    FireProjectile();
  void    ScreenWrite(string, SDL_Color);
private:
  bool                    is_running;

  bool left = false, right = false, up = false, down = false, fireEnable = false, continueGame, completed;

  shared_ptr<SFWindow>       sf_window;
  shared_ptr<SFAsset>        bomb;
  list<shared_ptr<SFAsset> > alienBombs;
  shared_ptr<SFAsset>        backgroundSF;
  shared_ptr<SFAsset>        GAMEOVER;
  shared_ptr<SFAsset>        YOUWIN;
  shared_ptr<SFAsset>        player;
  shared_ptr<SFBoundingBox>  app_box;
  list<shared_ptr<SFAsset> > projectiles;
  list<shared_ptr<SFAsset> > aliens;
  list<shared_ptr<SFAsset> > coins;

  int fire;
  int lives;
  int score;

  Mix_Chunk *gunFire;
  Mix_Chunk *deadAlien;
  Mix_Chunk *explosion;
  Mix_Chunk *coinSound;
  Mix_Chunk *youWin;
  Mix_Chunk *loser;
};
#endif
