#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.2
#define JUMP_FORCE -10

int N = 2;

typedef struct {
    float x, y, dx, dy;
    int w, h;
    SDL_Texture *texture;
} Player;

typedef struct {
    int x, y, w, h;
    SDL_Color color;
} Platform;

void apply_gravity(Player *player) {
    player->dy += GRAVITY;
}

bool isJumping = false;

void jump(Player *player) {
    if (player->dy == 0 && !isJumping) {
        player->dy = JUMP_FORCE;
        isJumping = true;
    }
}

SDL_Texture *init_background(SDL_Renderer *renderer, char const *path) {
    SDL_Surface *back_surface = NULL;
    SDL_Texture *background = NULL;
    back_surface = IMG_Load(path);
    if (!back_surface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
        return NULL;
    }
    background = SDL_CreateTextureFromSurface(renderer, back_surface);
    if (!background) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
        return NULL;
    }
    SDL_FreeSurface(back_surface);
    return background;
}

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Platformer Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture *background_menu = init_background(renderer,"background.png" );
    SDL_Surface *spriteSurface = IMG_Load("Oleg.png");
    SDL_Surface *bs = IMG_Load("left_platform.png");
    SDL_Surface *bs_r = IMG_Load("right_platform.png");
    SDL_Surface *area3_s = IMG_Load("area3.png");

    SDL_Texture *platform1 = SDL_CreateTextureFromSurface(renderer, bs);
    SDL_Texture *platform2 = SDL_CreateTextureFromSurface(renderer, area3_s);
    SDL_Texture *platform3 = SDL_CreateTextureFromSurface(renderer, area3_s);
    SDL_Texture *platform5 = SDL_CreateTextureFromSurface(renderer, area3_s);
    SDL_Texture *platform6 = SDL_CreateTextureFromSurface(renderer, bs_r);
    SDL_FreeSurface(bs);
    SDL_FreeSurface(area3_s);
    if (spriteSurface == NULL) {
        SDL_Log("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    SDL_FreeSurface(spriteSurface);
    //SDL_FreeSurface(background_t);

    int spriteWidth, spriteHeight;
    SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteWidth, &spriteHeight);
    Player player = {320, 240, 0, 0, spriteWidth, spriteHeight, spriteTexture};

    Platform platforms[] = {
    {0, 656, 288, 64, {255, 255, 255, 255}}, 
    {300, 556, 96, 16, {255,255,255,255}},
    {500, 556, 96, 16, {255,255,255,255}},
    {1280-288, 656, 288, 64, {255,255,255,255}}  
    };

    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    player.dx = -2;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    player.dx = 2;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    jump(&player);
                }
            } else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT) {
                    player.dx = 0;
                }
            }
        }

        
        apply_gravity(&player);
        player.x += player.dx;
        player.y += player.dy;

        if (player.x < 0) {
            player.x = 0;
        }
        if (player.x + player.w > SCREEN_WIDTH) {
            player.x = SCREEN_WIDTH - player.w;
        }
        if (player.y < 0) {
            player.y = 0;
        }
        if (player.y + player.h > SCREEN_HEIGHT) {
            player.y = SCREEN_HEIGHT - player.h;
            player.dy = 0;
            isJumping = false;
        }

        for (unsigned long i = 0; i < sizeof(platforms) / sizeof(Platform); i++) {
            if (player.x + player.w > platforms[i].x && player.x < platforms[i].x + platforms[i].w &&
                player.y + player.h > platforms[i].y && player.y < platforms[i].y + platforms[i].h) {
                if (player.dy > 0 && player.y + player.h < platforms[i].y + platforms[i].h) {
                    player.y = platforms[i].y - player.h;
                    player.dy = 0;
                    isJumping = false;
                }
            }
            if (player.x + player.w > platforms[i].x && player.x < platforms[i].x + platforms[i].w &&
                player.y < platforms[i].y + platforms[i].h && player.y > platforms[i].y) {
                    if (player.dy < 0) {
                        player.y = platforms[i].y + platforms[i].h + player.h / 2;
                        player.dy = 0;
                    }
            }

            if (player.y + player.h > platforms[i].y && player.y < platforms[i].y + platforms[i].h) {
                // Проверка коллизии с левой стороной платформы
                if (player.x + player.w > platforms[i].x && player.x + player.w < platforms[i].x + platforms[i].w) {
                    player.x = platforms[i].x - player.w;
                }
                // Проверка коллизии с правой стороной платформы
                else if (player.x < platforms[i].x + platforms[i].w && player.x > platforms[i].x) {
                    player.x = platforms[i].x + platforms[i].w;
                }
            }

        }


        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        

        SDL_Rect playerRect = {player.x, player.y, player.w, player.h};
        SDL_RenderCopy(renderer, background_menu, NULL, NULL);
        SDL_RenderCopy(renderer, player.texture, NULL, &playerRect);
        
        SDL_QueryTexture(platform1, NULL, NULL, 0, 0);
        SDL_Rect pRect1 = {0, 656, 288, 64};
        SDL_Rect pRect1_l = {0, 656, 288, 64};
        SDL_Rect pRect1_r = {1280-288, 656, 288, 64};
        SDL_Rect pRect_platform1 = {300, 556, 96, 16};
        SDL_Rect pRect_platform2 = {500, 556, 96, 16};
        SDL_RenderCopy(renderer, platform1, NULL, &pRect1);
        SDL_RenderCopy(renderer, platform2, NULL, &pRect_platform1);
        SDL_RenderCopy(renderer, platform3, NULL, &pRect_platform2);
        SDL_RenderCopy(renderer, platform6, NULL, &pRect1_r);
        /*for (unsigned long i = 0; i < sizeof(platforms) / sizeof(Platform); i++) {

            SDL_SetRenderDrawColor(renderer, platforms[i].color.r, platforms[i].color.g, platforms[i].color.b, platforms[i].color.a);
            SDL_Rect platformRect = {platforms[i].x, platforms[i].y, platforms[i].w, platforms[i].h};
            SDL_RenderFillRect(renderer, &platformRect);
        }*/

        SDL_RenderPresent(renderer);
        SDL_Delay(12);
    }

    SDL_DestroyTexture(player.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
