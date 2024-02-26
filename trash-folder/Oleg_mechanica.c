#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRAVITY 0.5
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

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("Platformer Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *spriteSurface = IMG_Load("Oleg.png");
    if (spriteSurface == NULL) {
        SDL_Log("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    SDL_FreeSurface(spriteSurface);

    int spriteWidth, spriteHeight;
    SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteWidth, &spriteHeight);
    Player player = {320, 240, 0, 0, spriteWidth, spriteHeight, spriteTexture};

    Platform platforms[] = {
        {100, 200, 100, 20, {255, 255, 255, 255}},
        {250, 300, 100, 20, {255, 255, 255, 255}},
        {400, 250, 100, 20, {255, 255, 255, 255}},
        {200, 400, 100, 20, {255, 255, 255, 255}},
        {350, 150, 100, 20, {255, 255, 255, 255}}
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
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (unsigned long i = 0; i < sizeof(platforms) / sizeof(Platform); i++) {
            SDL_SetRenderDrawColor(renderer, platforms[i].color.r, platforms[i].color.g, platforms[i].color.b, platforms[i].color.a);
            SDL_Rect platformRect = {platforms[i].x, platforms[i].y, platforms[i].w, platforms[i].h};
            SDL_RenderFillRect(renderer, &platformRect);
        }

        SDL_Rect playerRect = {player.x, player.y, player.w, player.h};
        SDL_RenderCopy(renderer, player.texture, NULL, &playerRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(player.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
