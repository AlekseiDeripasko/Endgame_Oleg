#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.3
#define JUMP_FORCE -15

int currentImageIndex = 0;
int animationFrameCount = 7; // Number of animation frames
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

Platform *current_p = NULL; /////////
int *size = NULL;
int arr1_size = 7;
int arr2_size = 6;

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
    SDL_Rect pRect1_l = {0, 656, 288, 64};
    SDL_Rect pRect1_r = {1280-288, 663, 288, 64};
    SDL_Rect pRect_platform1 = {300, 565, 96, 16};
    SDL_Rect pRect_platform2 = {505, 505, 96, 16};
    SDL_Rect pRect_platform3 = {682, 450, 128, 16};
    SDL_Rect pRect_platform4 = {860, 556, 96, 16};
    SDL_Rect Rect_bord = {0, 0, 1280, 720};
    SDL_RenderCopy(renderer, platforms_t[0], NULL, &pRect1_l); 
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform1); // p 1 ar3
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform2); // p 2 ar3
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect_platform3); // p 3 ar4
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform4);
    SDL_RenderCopy(renderer, platforms_t[4], NULL, &pRect1_r);
    SDL_RenderCopy(renderer, platforms_t[7], NULL, &Rect_bord); ///////////
}

void render_arena_2(SDL_Renderer * renderer, Platform *platforms, SDL_Texture *platforms_t[])
{
    SDL_Rect pRect2_l = {0, 185, 128, 96}; // left plato
    SDL_Rect pRect2_r = {1280-192, 350, 192, 96}; // right plato
    SDL_Rect pRect_platform1 = {190, 305, 96, 16}; // 1
    SDL_Rect pRect_platform2 = {380, 400, 192, 16}; // 2
    SDL_Rect pRect_platform3 = {640, 455, 128, 16}; 
    SDL_Rect pRect_platform4 = {890, 410, 96, 16}; 
    SDL_RenderCopy(renderer, platforms_t[5], NULL, &pRect2_l);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform1);
    SDL_RenderCopy(renderer, platforms_t[3], NULL, &pRect_platform2);
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect_platform3); 
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform4);
    SDL_RenderCopy(renderer, platforms_t[6], NULL, &pRect2_r); 
}   


int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    char* player_animation[] = {"1.png", "2.png", "3.png","4.png", "5.png", "6.png","7.png", "go_left_1.png", "go_left_2.png", "go_left_3.png", "go_left_4.png", "go_left_5.png", "go_left_6.png", "go_left_7.png"};
    char* background[] = {"background.png", "background2.png"};
        
    SDL_Window *window = SDL_CreateWindow("Platformer Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    loadMedia(player_animation, renderer);
    init_background(background, renderer);////////

    SDL_Surface *spriteSurface = IMG_Load("Oleg.png");
    if (!spriteSurface) printf("Error sprite\n");
    SDL_Surface *bs_l1 = IMG_Load("left_platform1.png");
    if (!bs_l1) printf("Error left\n");
    SDL_Surface *bs_r1 = IMG_Load("right_platform1.png");
    if (!bs_r1) printf("Error right\n");
    SDL_Surface *area3_s = IMG_Load("area3.png");
    if(!area3_s) printf("Error area3\n");
    SDL_Surface *area4_s = IMG_Load("area4.png");
    if(!area4_s) printf("Error area4\n");
    SDL_Surface *area6_s = IMG_Load("area6.png");
    if(!area6_s) printf("Error area6\n");
    SDL_Surface *bs_l2 = IMG_Load("left_platform2.png");
    if(!bs_l2) printf("Error left_platform2\n");
    SDL_Surface *bs_r2 = IMG_Load("right_platform2.png");
    if(!bs_r2) printf("Error right_platform2\n");
    SDL_Surface *border1 = IMG_Load("border_lvl_1.png");
    if(!border1) printf("Error border1\n");


    SDL_Texture *platforms_t[8];
    platforms_t[0] = SDL_CreateTextureFromSurface(renderer, bs_l1);
    platforms_t[1] = SDL_CreateTextureFromSurface(renderer, area3_s);
    platforms_t[2] = SDL_CreateTextureFromSurface(renderer, area4_s);
    platforms_t[3] = SDL_CreateTextureFromSurface(renderer, area6_s); 
    platforms_t[4] = SDL_CreateTextureFromSurface(renderer, bs_r1);
    platforms_t[5] = SDL_CreateTextureFromSurface(renderer, bs_l2);
    platforms_t[6] = SDL_CreateTextureFromSurface(renderer, bs_r2);
    platforms_t[7] = SDL_CreateTextureFromSurface(renderer, border1);

    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    Player player = {40, 540, 0, 0, 26*3, 36*3, spriteTexture};
     

    SDL_FreeSurface(bs_l1);
    SDL_FreeSurface(bs_r1);
    SDL_FreeSurface(area3_s);
    SDL_FreeSurface(area4_s);
    SDL_FreeSurface(area6_s);
    SDL_FreeSurface(bs_l2);
    SDL_FreeSurface(bs_r2);
    SDL_FreeSurface(border1);


    if (spriteSurface == NULL) {
        SDL_Log("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
        return -1;
    }
    
    SDL_FreeSurface(spriteSurface);

    int spriteWidth, spriteHeight;
    SDL_QueryTexture(spriteTexture, NULL, NULL, &spriteWidth, &spriteHeight);
    

    Platform platforms1[] = {
    {0, 656, 288, 64, {255, 255, 255, 255}}, 
    {300, 565, 96, 16, {255,255,255,255}},
    {505, 505, 96, 16, {255,255,255,255}},
    {682, 450, 128, 16, {255,255,255,255}}, // 4 plato 
    {860, 556, 96, 16, {255,255,255,255}}, // 5 plato
    {1280-288, 663, 288, 64, {255,255,255,255}},
    {0, 0, 16 , SCREEN_HEIGHT, {255, 255, 255, 255}} 
    };

    Platform platforms2[] = {
    {0, 185, 128, 96, {255, 255, 255, 255}}, // left plato
    {190, 305, 96, 16, {255,255,255,255}}, // 1
    {380, 400, 192, 16, {255,255,255,255}}, // 2
    {640, 455, 128, 16, {255,255,255,255}}, 
    {890, 410, 96, 16, {255,255,255,255}}, 
    {1280-192, 350, 192, 96, {255,255,255,255}}  // right plato
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
        /*if (player.y + player.h > SCREEN_HEIGHT) {
            player.y = SCREEN_HEIGHT - player.h;
            player.dy = 0;
            isJumping = false;
        }*/


        if (arena_switch == 1) {
            current_p = platforms1;
            size = &arr1_size;
        }
        else if (arena_switch == 2) {
            current_p = platforms2;
            size = &arr2_size;
        }
        

        if (player.y >  SCREEN_HEIGHT + 50 && player.x < SCREEN_WIDTH) {
            SDL_Delay(500);
            if (arena_switch == 1)
            {
                player.x = 40;
                player.y = 500;
            }
            else if (arena_switch == 2)
            {
                player.x = 40;
                player.y = 20;
            }
            
            
        }


        //Player life
        for (unsigned long i = 0; i < *size; i++) {
            if (player.x + player.w > current_p[i].x && player.x < current_p[i].x + current_p[i].w &&
                player.y + player.h > current_p[i].y && player.y < current_p[i].y + current_p[i].h) {
                if (player.dy > 0 && player.y + player.h < current_p[i].y + current_p[i].h) {
                    player.y = current_p[i].y - player.h;
                    player.dy = 0;
                    isJumping = false;
                }
            }
            if (player.x + player.w > current_p[i].x && player.x < current_p[i].x + current_p[i].w &&
                player.y < current_p[i].y + current_p[i].h && player.y > current_p[i].y) {
                    if (player.dy < 0) {
                        player.y = current_p[i].y + current_p[i].h + player.h / 2;
                        player.dy = 0;
                    }
            }

            if (player.y + player.h > current_p[i].y && player.y < current_p[i].y + current_p[i].h) {
                // Проверка коллизии с левой стороной платформы
                if (player.x + player.w > current_p[i].x && player.x + player.w < current_p[i].x + current_p[i].w) {
                    player.x = current_p[i].x - player.w;
                }
                // Проверка коллизии с правой стороной платформы
                else if (player.x < current_p[i].x + current_p[i].w && player.x > current_p[i].x) {
                    player.x = current_p[i].x + current_p[i].w;
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
            render_arena_1(renderer, platforms1, platforms_t); ///////
            
        }
        else if (arena_switch == 2)
        {
            if (first_load_player)
            {
                player.x = 0;
                player.y = 185-player.h;

                first_load_player = false;
            }
            SDL_RenderCopy(renderer, background_menu[1], NULL, NULL);   
            render_arena_2(renderer, platforms2, platforms_t);
            //arena_switch = 2;        
        }
        

        //Animation Player
        Uint32 currentTime = SDL_GetTicks();
        if(go_right_animation)
        {
            if (currentTime - startTime >= ANIMATION_SPEED) {
                currentImageIndex = (currentImageIndex + 1) % animationFrameCount;
                startTime = currentTime;
            }
            playerRect.w = 27*3;
            SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex], NULL, &playerRect);
        
        }
        else if (go_left_animation)
        {
            if (currentTime - startTime >= ANIMATION_SPEED) {            
                currentImageIndex = (currentImageIndex + 1) % animationFrameCount;
                startTime = currentTime;
            }
            playerRect.w = 27*3;
            SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex+6], NULL, &playerRect);
        }
        else
        {
            player.w = 17*3;
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
