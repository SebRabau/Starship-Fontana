#ifndef SFASSET_H
#define SFASSET_H

#include <string>
#include <memory>
#include <iostream>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFWindow.h"
#include "SFBoundingBox.h"

/**
 * We could create SFPlayer, SFProjectile and SFAsset which are subclasses
 * of SFAsset.  However, I've made an implementation decision to use this
 * enum to mark the type of the SFAsset.  If we add more asset types then
 * the subclassing strategy becomes a better option.
 */
enum SFASSETTYPE {SFASSET_DEAD, SFASSET_PLAYER, SFASSET_PROJECTILE, SFASSET_ALIEN, SFASSET_COIN, SFASSET_BACKGROUND, SFASSET_BOMB, SFASSET_GAME_OVER, SFASSET_ALIEN_BOMB, SFASSET_YOU_WIN};

class SFAsset {
public:
  SFAsset(const SFASSETTYPE, const std::shared_ptr<SFWindow>);
  SFAsset(const SFAsset&);
  virtual ~SFAsset();

  virtual void      SetPosition(Point2 &);
  virtual Point2    GetPosition();
  virtual Point2    GetProjectilePosition();
  virtual SFAssetId GetId();
  virtual void      OnRender();
  virtual void      GoEast();
  virtual void      GoWest();
  virtual void      AlienGoEast();
  virtual void      AlienGoWest();
  virtual void      BombGoSouth();
  virtual void      GoNorth();
  virtual void      GoSouth();
  virtual void      PlayerGoNorth();
  virtual void      PlayerGoSouth();
  virtual void      SetNotAlive();
  virtual bool      IsAlive();
  virtual void      HandleCollision();
  virtual bool                      CollidesWith(shared_ptr<SFAsset>);
  virtual shared_ptr<SFBoundingBox> GetBoundingBox();
  int getAssetWidth();
  int getAssetHeight();
private:
  // it would be nice if we could make this into a smart pointer,
  // but, because we need to call SDL_FreeSurface on it, we can't.
  // (or we could use a std::shared_ptr with a custom Deleter, but
  // that's a little too much right now)
  SDL_Texture               * sprite;
  shared_ptr<SFBoundingBox>   bbox;
  SFASSETTYPE                 type;
  SFAssetId                   id;
  std::shared_ptr<SFWindow>   sf_window;

  int counter1 = 0, counter2 = 0;

  static int SFASSETID;
};

#endif
