#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.3
#define JUMP_FORCE -10

int currentImageIndex = 0;
int animationFrameCount = 6; // Number of animation frames
const int ANIMATION_SPEED = 250; // milliseconds per frame
bool go_left_animation = false;
bool go_right_animation = false;
int arena_switch = 1;
bool first_load_player = 0;

SDL_Renderer* gRenderer = NULL;
SDL_Texture** gPlayerTextures = NULL;// Dynamically allocated array for player textures
SDL_Texture **background_menu = NULL;

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


bool loadMedia(char** fileNames, SDL_Renderer *renderer) {   
    int count = 12; 
    gPlayerTextures = malloc(sizeof(SDL_Texture*) * count);
    if (gPlayerTextures == NULL) {
        printf("Memory allocation failed for textures array!\n");
        return false;
    }
    for (int i = 0; i < count; ++i) {
        SDL_Surface* surface = IMG_Load(fileNames[i]);
        printf("%s\n", fileNames[i]);
        if (surface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", fileNames[i], IMG_GetError());
            return false;
        }

        gPlayerTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (gPlayerTextures[i] == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", fileNames[i], SDL_GetError());
            return false;
        }
    }
    printf("Media loaded successfully.\n");
    return true;
}

bool init_background(char** fileNames, SDL_Renderer *renderer) {

    int count = 2; //////////////////
    background_menu  = malloc(sizeof(SDL_Texture*) * count);
    if (gPlayerTextures == NULL) {
        printf("B: Memory allocation failed for textures array!\n");
        return false;
    }
    printf("B: Memory loaded!\n");
    for (int i = 0; i < count; ++i) {
        printf("%s\n", fileNames[i]);
        SDL_Surface* temp = IMG_Load(fileNames[i]);

        if (!temp) printf("ERROR!\n");
        if (temp == NULL) {
            printf("B: Unable to load image %s! SDL_image Error: %s\n", fileNames[i], IMG_GetError());
            return false;
        }

        background_menu[i] = SDL_CreateTextureFromSurface(renderer, temp);
        SDL_FreeSurface(temp);
    }
    return true;
}

void render_arena_1(SDL_Renderer * renderer, Platform *platforms, SDL_Texture *platforms_t[])
{
    SDL_Rect pRect1 = {0, 656, 288, 64};
    //SDL_Rect pRect1_l = {0, 656, 288, 64};
    SDL_Rect pRect1_r = {1280-288, 656, 288, 64};
    SDL_Rect pRect_platform1 = {300, 556, 96, 16};
    SDL_Rect pRect_platform2 = {500, 556, 96, 16};
    SDL_RenderCopy(renderer, platforms_t[0], NULL, &pRect1);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform1);
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect_platform2);
    SDL_RenderCopy(renderer, platforms_t[4], NULL, &pRect1_r);
}


int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    char* player_animation[] = {"1.png", "2.png", "3.png","4.png", "5.png", "6.png", "go_left_1.png", "go_left_2.png", "go_left_3.png", "go_left_4.png", "go_left_5.png", "go_left_6.png"};
    char* background[] = {"background.png", "background2.png"};
        
    SDL_Window *window = SDL_CreateWindow("Platformer Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    loadMedia(player_animation, renderer);
    init_background(background, renderer);////////

    SDL_Surface *spriteSurface = IMG_Load("Oleg.png");
    if (!spriteSurface) printf("Error sprite\n");
    SDL_Surface *bs = IMG_Load("left_platform1.png");
    if (!bs) printf("Error left\n");
    SDL_Surface *bs_r = IMG_Load("right_platform1.png");
    if (!bs_r) printf("Error right\n");
    SDL_Surface *area3_s = IMG_Load("area3.png");
    if(!area3_s) printf("Error area3\n");
    SDL_Texture *platforms_t[5];
    platforms_t[0] = SDL_CreateTextureFromSurface(renderer, bs);
    platforms_t[1] = SDL_CreateTextureFromSurface(renderer, area3_s);
    platforms_t[2] = SDL_CreateTextureFromSurface(renderer, area3_s);
    platforms_t[3] = SDL_CreateTextureFromSurface(renderer, area3_s);
    platforms_t[4] = SDL_CreateTextureFromSurface(renderer, bs_r);

    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    Player player = {320, 240, 0, 0, 26*5, 36*5, spriteTexture};
     

    SDL_FreeSurface(bs);
    SDL_FreeSurface(area3_s);


    if (spriteSurface == NULL) {
        SDL_Log("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    
    SDL_FreeSurface(spriteSurface);

    int spriteWidth, spriteHeight;
    SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteWidth, &spriteHeight);
    

    Platform platforms[] = {
        {0, 656, 288, 64, {255, 255, 255, 255}}, 
        {300, 556, 96, 16, {255,255,255,255}},
        {500, 556, 96, 16, {255,255,255,255}},
        {1280-288, 656, 288, 64, {255,255,255,255}}  
    };

    SDL_Event event;
    Uint32 startTime = SDL_GetTicks();
    //Uint32 s_t = SDL_GetTicks();
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    player.dx = -1.5;
                    go_left_animation = true;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    player.dx = 1.5;
                    go_right_animation = true;
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    jump(&player);
                }
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    player.dx = 0;
                    go_left_animation = false;
                    currentImageIndex = 0;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    player.dx = 0;
                    go_right_animation = false;
                    currentImageIndex = 0;
                }
            }
        }

        apply_gravity(&player);
        player.x += player.dx;
        player.y += player.dy;
        SDL_Rect playerRect = {player.x, player.y, player.w, player.h}; 
        if (player.x < -40) {
            if (arena_switch == 2)
            {
                arena_switch = 1;
                first_load_player = true;
            }
                
        
            player.x = 0;
        }
        if (player.x + player.w > SCREEN_WIDTH + 40) {
            player.x = SCREEN_WIDTH - player.w;
            if (arena_switch == 1)
            {
                first_load_player = true;
                arena_switch = 2;
            }
                
        }
        if (player.y < 0) {
            player.y = 0;
        }
        if (player.y + player.h > SCREEN_HEIGHT) {
            player.y = SCREEN_HEIGHT - player.h;
            player.dy = 0;
            isJumping = false;
        }

        //Player life
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

        

        //Uint32 t = SDL_GetTicks();

        if (arena_switch == 1) {
            SDL_RenderCopy(renderer, background_menu[0], NULL, NULL);
            if (first_load_player)
            {
                player.x = SCREEN_WIDTH - 50;
                player.y = 550;
                first_load_player = false;
            }
            render_arena_1(renderer, platforms, platforms_t); ///////
            
        }
        else
        {
            if (first_load_player)
            {
                player.x = 10;

                first_load_player = false;
            }
            SDL_RenderCopy(renderer, background_menu[1], NULL, NULL);           
        }

        //Animation Player
        Uint32 currentTime = SDL_GetTicks();
        if(go_right_animation)
        {
            if (currentTime - startTime >= ANIMATION_SPEED) {
                currentImageIndex = (currentImageIndex + 1) % animationFrameCount;
                startTime = currentTime;
            }
            playerRect.w = 27*5;
            SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex], NULL, &playerRect);
        
        }
        else if (go_left_animation)
        {
            if (currentTime - startTime >= ANIMATION_SPEED) {            
                currentImageIndex = (currentImageIndex + 1) % animationFrameCount;
                startTime = currentTime;
            }
            playerRect.w = 27*5;
            SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex+6], NULL, &playerRect);
        }
        else
        {
            player.w = 17*5;
            SDL_RenderCopy(renderer, player.texture, NULL, &playerRect);
        }
        

        SDL_RenderPresent(renderer);
        SDL_Delay(12);
    }

    SDL_DestroyTexture(player.texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
