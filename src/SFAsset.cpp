#include "SFAsset.h"


int SFAsset::SFASSETID=0;

SFAsset::SFAsset(SFASSETTYPE type, std::shared_ptr<SFWindow> window): type(type), sf_window(window) {
  this->id   = ++SFASSETID;

  switch (type) {
  case SFASSET_PLAYER:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/player.png");
    break;
  case SFASSET_PROJECTILE:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/laser.png");
    break;
  case SFASSET_ALIEN:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/alien.png");
    break;
  case SFASSET_COIN:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/coin.png");
    break;  
  case SFASSET_BACKGROUND:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/background2.jpg");
    break;
  case SFASSET_BOMB:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/glowtube.png");
    break;
  case SFASSET_GAME_OVER:
    sprite = IMG_LoadTexture(sf_window->getRenderer(), "assets/GameOver.png");
    break;
  }

  if(!sprite) {
    cerr << "Could not load asset of type " << type << endl;
    throw SF_ERROR_LOAD_ASSET;
  }

  // Get texture width & height
  int w, h;
  SDL_QueryTexture(sprite, NULL, NULL, &w, &h);

  // Initialise bounding box
  bbox = make_shared<SFBoundingBox>(SFBoundingBox(Vector2(0.0f, 0.0f), w, h));
}

SFAsset::SFAsset(const SFAsset& a) {
  sprite = a.sprite;
  sf_window = a.sf_window;
  bbox   = a.bbox;
  type   = a.type;
}

SFAsset::~SFAsset() {
  bbox.reset();
  if(sprite) {
    SDL_DestroyTexture(sprite);
    sprite = nullptr;
  }
}

/**
 * The logical coordinates in the game assume that the screen
 * is indexed from 0,0 in the bottom left corner.  The blittable
 * coordinates of the screen map 0,0 to the top left corner. We
 * need to convert between the two coordinate spaces.  We assume
 * that there is a 1-to-1 quantisation.
 */
Vector2 GameSpaceToScreenSpace(SDL_Renderer* renderer, Vector2 &r) {
  int w, h;
  SDL_GetRendererOutputSize(renderer, &w, &h);

  return Vector2 (
                  r.getX(),
                  (h - r.getY())
                  );
}

void SFAsset::SetPosition(Point2 & point) {
  Vector2 v(point.getX(), point.getY());
  bbox->SetCentre(v);
}

Point2 SFAsset::GetPosition() {
  return Point2(bbox->centre->getX(), bbox->centre->getY());
}

Point2 SFAsset::GetProjectilePosition() {
  return Point2(bbox->centre->getX(), (45 + bbox->centre->getY()));
}

SFAssetId SFAsset::GetId() {
  return id;
}

void SFAsset::OnRender() {
  // 1. Get the SDL_Rect from SFBoundingBox
  SDL_Rect rect;

  Vector2 gs = (*(bbox->centre) + (*(bbox->extent_x) * -1)) + (*(bbox->extent_y) * 1);
  Vector2 ss = GameSpaceToScreenSpace(sf_window->getRenderer(), gs);
  rect.x = ss.getX();
  rect.y = ss.getY();
  rect.w = bbox->extent_x->getX() * 2;
  rect.h = bbox->extent_y->getY() * 2;

  // 2. Blit the sprite onto the level
  SDL_RenderCopy(sf_window->getRenderer(), sprite, NULL, &rect);


  // 3. Draw Bounding Box
  /*SDL_SetRenderDrawColor(sf_window->getRenderer(), 255, 0, 0, SDL_ALPHA_OPAQUE);

  SDL_RenderDrawLine(sf_window->getRenderer(), ss.getX(), ss.getY(), (ss.getX() + rect.w), ss.getY());
  SDL_RenderDrawLine(sf_window->getRenderer(), ss.getX(), ss.getY(), ss.getX(), (ss.getY() + rect.h));
  SDL_RenderDrawLine(sf_window->getRenderer(), (ss.getX() + rect.w), ss.getY(), (ss.getX() + rect.w), (ss.getY() + rect.h));
  SDL_RenderDrawLine(sf_window->getRenderer(), ss.getX(), (ss.getY() + rect.h), (ss.getX() + rect.w), (ss.getY() + rect.h)); 
  SDL_RenderDrawLine(sf_window->getRenderer(), ss.getX(), ss.getY(), (ss.getX() + rect.w), (ss.getY() + rect.h));
  SDL_RenderDrawLine(sf_window->getRenderer(), ss.getX(), (ss.getY() + rect.h), (ss.getX() + rect.w), ss.getY()); */
}

int SFAsset::getAssetWidth() {
  return (bbox->extent_x->getX() * 2);
}

int SFAsset::getAssetHeight() {
  return (bbox->extent_y->getY() * 2);
}

void SFAsset::GoWest() {
  Vector2 c = *(bbox->centre) + Vector2(-5.0f, 0.0f);

  if(!((c.getX() - bbox->extent_x->getX()) < 0)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);
  }
}

void SFAsset::GoEast() {
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

  Vector2 c = *(bbox->centre) + Vector2(5.0f, 0.0f);

  if(!((c.getX() + bbox->extent_x->getX()) > w)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);    
  }
}

void SFAsset::AlienGoWest() {
  Vector2 c = *(bbox->centre) + Vector2(-1.0f, 0.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::AlienGoEast() {
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

  Vector2 c = *(bbox->centre) + Vector2(1.0f, 0.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::GoNorth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, 7.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::PlayerGoNorth() {
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

  Vector2 c = *(bbox->centre) + Vector2(0.0f, 5.0f);

  if(((c.getY() + bbox->extent_y->getY()) < h)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c); 
  }
}

void SFAsset::GoSouth() {
  Vector2 c = *(bbox->centre) + Vector2(0.0f, -1.0f);
  bbox->centre.reset();
  bbox->centre = make_shared<Vector2>(c);
}

void SFAsset::PlayerGoSouth() {
  int w, h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &w, &h);

  Vector2 c = *(bbox->centre) + Vector2(0.0f, -5.0f);

  if(((c.getY() - bbox->extent_y->getY()) > 0)) {
    bbox->centre.reset();
    bbox->centre = make_shared<Vector2>(c);  
  }
}

bool SFAsset::CollidesWith(shared_ptr<SFAsset> other) {
  return bbox->CollidesWith(other->bbox);
}

shared_ptr<SFBoundingBox> SFAsset::GetBoundingBox() {
  return bbox;
}

void SFAsset::SetNotAlive() {
  type = SFASSET_DEAD;
}

bool SFAsset::IsAlive() {
  return (SFASSET_DEAD != type);
}

void SFAsset::HandleCollision() {
 /* if(SFASSET_PROJECTILE == type || SFASSET_ALIEN == type || SFASSET_COIN == type || SFASSET_PLAYER == type) {
    SetNotAlive();
 */ 
   SetNotAlive();
 }
