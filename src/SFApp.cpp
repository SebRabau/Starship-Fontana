#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0), is_running(true), sf_window(window), lives(3), score(0), continueGame(true) {
  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  backgroundSF  = make_shared<SFAsset>(SFASSET_BACKGROUND, sf_window);
  auto background_pos = Point2(canvas_w/2, canvas_h/2);
  backgroundSF->SetPosition(background_pos);

  GAMEOVER  = make_shared<SFAsset>(SFASSET_GAME_OVER, sf_window);
  auto GAMEOVER_pos = Point2(canvas_w/2, canvas_h/2);
  GAMEOVER->SetPosition(GAMEOVER_pos);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);

  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 50);
  player->SetPosition(player_pos);

  bomb = make_shared<SFAsset>(SFASSET_BOMB, sf_window);
  auto bomb_pos = Point2(canvas_w/4, 42);
  bomb->SetPosition(bomb_pos);

  const int number_of_aliens = 8;
  for(int i=0; i<number_of_aliens; i++) {
    // place an alien at width/number_of_aliens * i
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    auto pos   = Point2((canvas_w/number_of_aliens) * i + alien->getAssetWidth(), 300.0f);
    alien->SetPosition(pos);
    aliens.push_back(alien);
  }

  auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
  //auto pos  = Point2((canvas_w/4), 100);
  srand(time(NULL));
  auto pos  = Point2(rand() % (canvas_w - coin->getAssetWidth()) + coin->getAssetWidth(), canvas_h);
  coin->SetPosition(pos);
  coins.push_back(coin);

    // Load Sound Effects

  gunFire = Mix_LoadWAV("assets/Gun.wav");
  deadAlien = Mix_LoadWAV("assets/GlassBreak.wav");
  explosion = Mix_LoadWAV("assets/Explosion.wav");
  coinSound = Mix_LoadWAV("assets/Coin.wav");
}

SFApp::~SFApp() {
}

void SFApp::ScreenWrite(string myText, SDL_Color text_color) {
   TTF_Font* font = TTF_OpenFont("assets/ASMAN.ttf", 24);


   //const char *cchar = myText.c_str(); 
   const char * cchar = "hi";
   SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, cchar , text_color);
   SDL_Texture* Message = SDL_CreateTextureFromSurface(sf_window->getRenderer(), surfaceMessage);

   SDL_Rect Message_rect;
   Message_rect.x = 0;
   Message_rect.y = 0;
   Message_rect.w = 680;
   Message_rect.h = 100;

   SDL_RenderCopy(sf_window->getRenderer(), Message, NULL, &Message_rect);
 }

/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {

fireEnable = true;

  SFEVENT the_event = event.GetCode();
    switch (the_event) {
    case SFEVENT_QUIT:
      is_running = false;
      break;
    case SFEVENT_RESTART:
      break;
    case SFEVENT_UPDATE:
      OnUpdateWorld();
      OnRender();
      break;
    case SFEVENT_PLAYER_LEFT_PRESSED:
      left = true;
      break;
    case SFEVENT_PLAYER_LEFT_RELEASED:
      left = false;
      break;
    case SFEVENT_PLAYER_RIGHT_PRESSED:
      right = true;
      break;
    case SFEVENT_PLAYER_RIGHT_RELEASED:
      right = false;
      break;
    case SFEVENT_PLAYER_UP_PRESSED:
      up = true;
      break;
    case SFEVENT_PLAYER_UP_RELEASED:
      up = false;
      break;
    case SFEVENT_PLAYER_DOWN_PRESSED:
      down = true;
      break;
    case SFEVENT_PLAYER_DOWN_RELEASED:
      down = false;
      break;
    case SFEVENT_FIRE_PRESSED:
    if (fireEnable) {
      fire++;
      FireProjectile();
      Mix_PlayChannel(-1, gunFire, 0);
      fireEnable = false;
    }
      break;
    case SFEVENT_FIRE_RELEASED:
      fireEnable = true;
      break;  
  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // wrap an SDL_Event with our SFEvent
    SFEvent sfevent((const SDL_Event) event);
    // handle our SFEvent
    OnEvent(sfevent);  

  if (right && continueGame) {
    player->GoEast();
  }

  if (left && continueGame) {
    player->GoWest();
  }

  if (up && continueGame) {
    player->PlayerGoNorth();
  }

  if (down && continueGame) {
    player->PlayerGoSouth();
  }
 } 
}

void SFApp::OnUpdateWorld() {
  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorth();
  }

  for(auto c: coins) {
    c->GoSouth();
  }

  for(auto b: alienBombs) {
    b->BombGoSouth();
  }

  // Update enemy positions
  for(auto a : aliens) {
    
    // do something here
    if (right) {
      a->AlienGoWest();
    }
    if (left) {
      a->AlienGoEast();
    }
    if (rand() % 1000 <= 0.01) {
      auto alienBomb = make_shared<SFAsset>(SFASSET_ALIEN_BOMB,sf_window); //Drop Bomb on death
      auto alien_bomb_pos = a->GetPosition();
      alienBomb->SetPosition(alien_bomb_pos);
      alienBombs.push_back(alienBomb);
    }
  }

  // Detect collisions
  for(auto p : projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
        Mix_PlayChannel(-1, deadAlien, 0);
        score += 100;
        string text = "You killed an alien! +100 score. Your current score: " + score;
        ScreenWrite(text, {255, 0, 0});
        cout << "You killed an alien! +100 score" << endl;
        cout << "Your current score: " << score << endl;
        cout << "------------------------------" << endl;
      }
    }
  }

   for(auto a : aliens) {
    if(player->CollidesWith(a)) {
      player->SetNotAlive();
    }
  }

  for(auto c : coins) {
    if (c->CollidesWith(player)) {
      c->HandleCollision();
      Mix_PlayChannel(-1, coinSound, 0);
      score += 50;
      cout << "You have collected a rare item! Now kill the rest of the aliens to make your escape! +50 score" << endl;
      cout << "Your current score: " << score << endl;
      cout << "------------------------------" << endl;;
    }
  }	  

  // remove dead aliens (the long way)
  list<shared_ptr<SFAsset>> tmp;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp.push_back(a);
    }
  }
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp);

  // remove dead coins
  list<shared_ptr<SFAsset>> rdc;
    for(auto c : coins) {
      if(c->IsAlive()) {
        rdc.push_back(c);
      }
    }
    coins.clear();
    coins = list<shared_ptr<SFAsset>>(rdc);

    // remove dead alien bombs
  list<shared_ptr<SFAsset>> rdb;
    for(auto b : alienBombs) {
      if(b->IsAlive()) {
        rdb.push_back(b);
      }
    }
    alienBombs.clear();
    alienBombs = list<shared_ptr<SFAsset>>(rdb);

    // remove dead projectiles
  list<shared_ptr<SFAsset>> rdp;
    for(auto p: projectiles) {
      if(p->IsAlive()) {
        rdp.push_back(p);
      }
    }
    projectiles.clear();
    projectiles = list<shared_ptr<SFAsset>>(rdp);
}

void SFApp::OnRender() {
  SDL_SetRenderDrawColor(sf_window->getRenderer(), 20, 198, 42, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(sf_window->getRenderer());

  int canvas_w, canvas_h;
  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);
  auto player_pos = Point2(canvas_w/2, 50);

  // draw the background
  backgroundSF->OnRender();

  // draw the player
  player->OnRender();

  bomb->OnRender();

  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }

  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
  }

  for(auto c: coins) {
    if(c->IsAlive()) {c->OnRender();}
  }

  for(auto b: alienBombs) {
    b->OnRender();
  }

  // Game Over Screens
  if (player->CollidesWith(bomb)) {
    
    Mix_PlayChannel(-1, explosion, 0);
    lives -= 1;
    score -= 100;
    cout << "You have lost a life!! You have " << lives << " life points remaining. -100 score" << endl;
    cout << "Your current score: " << score << endl; 
    cout << "------------------------------" << endl;;   
    player->SetPosition(player_pos);
  }

for(auto b: alienBombs) {
  if (player->CollidesWith(b)) {
    b->SetNotAlive();
    Mix_PlayChannel(-1, explosion, 0);
    lives -= 1;
    score -= 100;
    cout << "You have lost a life!! You have " << lives << " life points remaining. -100 score" << endl;
    cout << "Your current score: " << score << endl; 
    cout << "------------------------------" << endl;;   
    player->SetPosition(player_pos);
  }
}

  for(auto a : aliens) {
    if(player->CollidesWith(a)) {
      Mix_PlayChannel(-1, deadAlien, 0);
      lives -= 1;
      score -= 100;
      cout << "You have lost a life!! You have " << lives << " life points remaining. -100 score" << endl;
      cout << "Your current score: " << score << endl;
      cout << "------------------------------" << endl;
      player->SetPosition(player_pos);
    }
  }

  if (lives <= 0){
    GAMEOVER->OnRender();
    continueGame = false;
    while (lives == 0) {
      lives--;
      cout << "You have lost the game.... You finsihed with " << score << " points" << endl;
    }    
  }  

  if ((aliens.size() == 0)) {
    //You Win
  }

  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetProjectilePosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}
