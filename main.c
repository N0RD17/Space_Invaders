#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

const int SCREEN_WIDTH	= 640;
const int SCREEN_HEIGHT	= 480;

SDL_Window* window;
SDL_Renderer* renderer;

struct Entity{
   SDL_Rect rect;
};

struct Bullet{
   int posX, posY, Speed;
   uint8_t active;
};

struct Bullet bullet = {0, 0, 5, 0};

// Handle entity
void drawEntity(struct Entity* entity);

// Handle bullet
void updateBulletPosition();
void drawBullet();

int main(void)
{
   /** Handle SDL2 Initialization of Stuff **/
   const uint32_t WINDOW_FLAG = SDL_WINDOW_SHOWN;
   const uint32_t RENDERER_FLAG = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
   if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0){
      perror("SDL_INIT ERROR!");
      printf("Unable to initialize SDL2! SDL_ERROR: %s", SDL_GetError());
      return 1;
   }

   window = SDL_CreateWindow("Space-Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_FLAG);
   if(window == NULL){
      perror("SDL_WINDOW CREATION ERROR!");
      printf("Failed to create SDL2_Window! SDL_ERROR: %s", SDL_GetError());
      return 1;
   }

   renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAG);
   if(renderer == NULL){
      perror("SDL_RENDERER CREATION ERROR!");
      printf("Failed to create SDL2_Renderer! SDL_ERROR: %s", SDL_GetError());
      SDL_DestroyWindow(window);
      return 1;
   }

   /** Handle Player **/
   struct Entity player;
   player.rect.x = SCREEN_WIDTH / 2;
   player.rect.y = 400;
   player.rect.w = 32;
   player.rect.h = 32;

   /** Handle Bullet **/
   uint8_t bullet_shot = 0;

   /** Handle Game loop **/
   uint8_t running = 1;
   while(running)
   {
      SDL_Event event;
      while(SDL_PollEvent(&event))
      {
         switch(event.type)
         {
            case SDL_QUIT:{
               running = 0;
            }break;

            // Look for a keypress
            case SDL_KEYDOWN:{
               switch(event.key.keysym.sym)
               {
                  case SDLK_LEFT:{
                     if(player.rect.x > 0)
                     {
                        player.rect.x -= 8;
                     }
                  }break;

                  case SDLK_RIGHT:{
                     if(player.rect.x < SCREEN_WIDTH - player.rect.w)
                     {
                        player.rect.x += 8;
                     }
                  }break;

                  case SDLK_SPACE:{
                     if(!bullet.active)
                     {
                        bullet_shot = 1;
                     }
                  }break;

                  default: break;
               }
            }break;
               
            default: break;
         }
      }

      SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
      SDL_RenderClear(renderer);

      /** Drawings **/
      drawEntity(&player);
      if(bullet_shot){
         // Activate the bullet and set initial position
         bullet.active = 1;
         bullet.posX = player.rect.x + 14;
         bullet.posY = player.rect.y - 16;
      }

      if(bullet.active){
         updateBulletPosition();
         drawBullet();
      }

      SDL_RenderPresent(renderer);

      bullet_shot = 0;
   }

   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();

   return 0;
}


void drawEntity(struct Entity* entity)
{
   SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
   SDL_RenderDrawRect(renderer, &entity->rect);
}

void updateBulletPosition()
{
   bullet.posY -= bullet.Speed;

   // Check if bullet has reached top of window
   if(bullet.posY < 0){
      // Deactivate the bullet
      bullet.active = 0;
   }
}

void drawBullet()
{
   SDL_Rect tmpRect = {bullet.posX, bullet.posY, 8, 8};
   SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//   SDL_RenderDrawPoint(renderer, bullet.posX, bullet.posY);
   SDL_RenderDrawRect(renderer, &tmpRect);
}

