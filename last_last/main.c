#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.2 //0.3
#define JUMP_FORCE -10

int currentImageIndex = 0;
int animationFrameCount = 7; // Number of animation frames
const int ANIMATION_SPEED = 250; // milliseconds per frame
bool go_left_animation = false;
bool go_right_animation = false;
bool go_jump = false;
bool go_left_for_jump = false;
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
int size;
int arr1_size = 9;
int arr2_size = 6;
int arr3_size = 22;

void apply_gravity(Player *player) {
    player->dy += GRAVITY;
}

bool isJumping = false;

void jump(Player *player) {
    if (player->dy == 0 && !isJumping) {
        player->dy = JUMP_FORCE;
        isJumping = true;
        go_jump = true;
        go_right_animation = false;
        go_left_animation = false;
    }
}


bool loadMedia(char** fileNames, SDL_Renderer *renderer) {   
    int count = 30; 
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

    int count = 3; //////////////////
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
    SDL_Rect pRect_platform3 = {640, 460, 128, 16}; 
    SDL_Rect pRect_platform4 = {890, 410, 96, 16}; 
    SDL_RenderCopy(renderer, platforms_t[5], NULL, &pRect2_l);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform1);
    SDL_RenderCopy(renderer, platforms_t[3], NULL, &pRect_platform2);
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect_platform3); 
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform4);
    SDL_RenderCopy(renderer, platforms_t[6], NULL, &pRect2_r); 
} 

void render_arena_3(SDL_Renderer * renderer, Platform *platforms, SDL_Texture *platforms_t[])
{
    SDL_Rect pRect_l_top = {0, 205, 160, 64}; // left_top_platform
    SDL_Rect pRect_l_bottom = {0, SCREEN_HEIGHT-64, 128, 64}; // left_top_platform
    SDL_Rect pRect_platform1_4_top = {240, 280, 128, 16}; // 4-long left up
    SDL_Rect pRect_platform1_4_bottom = {210, 575, 128, 16}; // 4-long left bottom
    SDL_Rect pRect_platform1_3_mid = {420, 350, 96, 16}; // 3-long mid
    SDL_Rect pRect_platform1_2_mid = {530, 405, 64, 16}; // 2-long mid
    SDL_Rect pRect_platform1_4_mid = {655, 490, 128, 16}; // 4-long mid
    SDL_Rect pRect_platform1_sq_mid = {440, SCREEN_HEIGHT-160, 160, 160}; // square mid
    SDL_Rect pRect_platform1_2_right_1 = {835, 410, 64, 16}; // 2-long right mid //
    SDL_Rect pRect_platform1_2_right_2 = {940, 350, 64, 16}; // 2-long right up
    SDL_Rect pRect_platform1_2_right_3 = {1050, 300, 64, 16}; // 2-long right up up //
    SDL_Rect pRect_platform1_2_right_4 = {910, 450, 64, 16}; // 2-long right down //
    SDL_Rect pRect_platform1_2_right_5 = {990, 500, 64, 16}; // 2-long right down down //
    SDL_Rect pRect_room_top = {SCREEN_WIDTH-256-33, SCREEN_HEIGHT-128-32, 256, 32}; // room_top//
    SDL_Rect pRect_room_right = {SCREEN_WIDTH-33, SCREEN_HEIGHT-128-32, 32, 128}; // room_right //
    SDL_Rect pRect_room_bottom = {SCREEN_WIDTH-256-33, SCREEN_HEIGHT-32, 256, 32}; // room_bottom //
    SDL_Rect pRect_platform_finish = {SCREEN_WIDTH-128, 280, 128, 64}; // final_platform 
    SDL_Rect pRect_box1 = {20, 174, 32, 32}; // box left
    SDL_Rect pRect_box2 = {1197, 657, 32, 32}; // box right
    SDL_Rect pRect_ladder = {SCREEN_WIDTH-159, SCREEN_HEIGHT-171, 32, 75}; //ladder 
    SDL_Rect pRect_bord = {0, 0, 1280, 720};


    /*
    SDL_Rect pRect_platform2 = {380, 400, 192, 16}; // 2
    SDL_Rect pRect_platform3 = {640, 455, 128, 16}; 
    SDL_Rect pRect_platform4 = {890, 410, 96, 16};
    */ 
    SDL_RenderCopy(renderer, platforms_t[10], NULL, &pRect_l_top); //
    SDL_RenderCopy(renderer, platforms_t[9], NULL, &pRect_l_bottom); //
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect_platform1_4_top); //
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect_platform1_4_bottom); //
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform1_3_mid); //
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_mid); //
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect_platform1_4_mid); //
    SDL_RenderCopy(renderer, platforms_t[19], NULL, &pRect_platform1_sq_mid); //
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_1);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_2);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_3);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_4);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_5);
    SDL_RenderCopy(renderer, platforms_t[14], NULL, &pRect_room_top);
    SDL_RenderCopy(renderer, platforms_t[12], NULL, &pRect_room_right);
    SDL_RenderCopy(renderer, platforms_t[13], NULL, &pRect_room_bottom);
    SDL_RenderCopy(renderer, platforms_t[11], NULL, &pRect_platform_finish);
    SDL_RenderCopy(renderer, platforms_t[15], NULL, &pRect_box1);
    SDL_RenderCopy(renderer, platforms_t[15], NULL, &pRect_box2);
    SDL_RenderCopy(renderer, platforms_t[17], NULL, &pRect_ladder);
    SDL_RenderCopy(renderer, platforms_t[20], NULL, &pRect_bord);
} 


int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    char* player_animation[] = {"1.png", "2.png", "3.png","4.png", "5.png", "6.png","7.png", 
                                "go_left_1.png", "go_left_2.png", "go_left_3.png", "go_left_4.png", "go_left_5.png", "go_left_6.png", "go_left_7.png",
                                "jump_up_1.png","jump_up_2.png","jump_up_3.png","jump_up_4.png","jump_up_5.png","jump_up_6.png","jump_up_7.png","jump_up_8.png",
                                "jump_left_up_1.png","jump_left_up_2.png","jump_left_up_3.png","jump_left_up_4.png","jump_left_up_5.png","jump_left_up_6.png",
                                "jump_left_up_7.png","jump_left_up_8.png"};
    char* background[] = {"background.png", "background2.png", "background_3.png"};
        
    SDL_Window *window = SDL_CreateWindow("Platformer Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    loadMedia(player_animation, renderer);
    init_background(background, renderer);////////

    SDL_Surface *spriteSurface = IMG_Load("Oleg.png");
    if (!spriteSurface) printf("Error sprite\n");
    SDL_Surface *bs_l1 = IMG_Load("left_platform1.png"); // 0
    if (!bs_l1) printf("Error left\n");
    SDL_Surface *bs_r1 = IMG_Load("right_platform1.png"); // 4
    if (!bs_r1) printf("Error right\n");
    SDL_Surface *area3_s = IMG_Load("area3.png"); // 1
    if(!area3_s) printf("Error area3\n");
    SDL_Surface *area4_s = IMG_Load("area4.png"); // 2
    if(!area4_s) printf("Error area4\n");
    SDL_Surface *area6_s = IMG_Load("area6.png");  // 3
    if(!area6_s) printf("Error area6\n");
    SDL_Surface *bs_l2 = IMG_Load("left_platform2.png"); // 5
    if(!bs_l2) printf("Error left_platform2\n");
    SDL_Surface *bs_r2 = IMG_Load("right_platform2.png"); // 6
    if(!bs_r2) printf("Error right_platform2\n");
    SDL_Surface *border1 = IMG_Load("border_lvl_1.png"); // 7
    if(!border1) printf("Error border1\n");
    SDL_Surface *area2_s = IMG_Load("area2.png"); // 8
    if(!area2_s) printf("Error area2\n");
    SDL_Surface *bs_down_l3 = IMG_Load("left_platform_3.png"); // 9
    if(!bs_down_l3) printf("Error left_platform_3\n");
    SDL_Surface *bs_up_l3 = IMG_Load("left_up_platform_3.png"); // 10 
    if(!bs_up_l3) printf("Error left_up_platform_3\n");
    SDL_Surface *bs_r3 = IMG_Load("right_platform3.png"); // 11 
    if(!bs_r3) printf("Error left_up_platform_3\n");
    SDL_Surface *room_right3 = IMG_Load("room_right3.png"); // 12;
    if(!room_right3) printf("Error room_right3\n");
    SDL_Surface *room_bot3 = IMG_Load("room_bot3.png"); // 13 
    if(!room_bot3) printf("Error room_bot3\n");
    SDL_Surface *room_top3 = IMG_Load("room_top3.png");  // 14
    if(!room_top3) printf("Error room_top3\n");
    SDL_Surface *box = IMG_Load("box.png"); // 15
    if(!box) printf("Error box\n");
    SDL_Surface *five = IMG_Load("five.png"); // 16
    if(!five) printf("Error five\n");
    SDL_Surface *ladder = IMG_Load("ladder.png"); // 17
    if(!ladder) printf("Error ladder\n");
    SDL_Surface *hp = IMG_Load("hp.png"); // 18
    if(!hp) printf("Error hp\n");
    SDL_Surface *mid_platform3 = IMG_Load("mid_platform3.png"); // 19
    if(!mid_platform3) printf("Error mid_platform3\n");
    SDL_Surface *border3 = IMG_Load("border3.png"); // 20
    if(!border3) printf("Error border3\n");

    SDL_Texture *platforms_t[8];
    platforms_t[0] = SDL_CreateTextureFromSurface(renderer, bs_l1);
    platforms_t[1] = SDL_CreateTextureFromSurface(renderer, area3_s); //3-long
    platforms_t[2] = SDL_CreateTextureFromSurface(renderer, area4_s); //4-long
    platforms_t[3] = SDL_CreateTextureFromSurface(renderer, area6_s); //6-long
    platforms_t[4] = SDL_CreateTextureFromSurface(renderer, bs_r1);
    platforms_t[5] = SDL_CreateTextureFromSurface(renderer, bs_l2);
    platforms_t[6] = SDL_CreateTextureFromSurface(renderer, bs_r2);
    platforms_t[7] = SDL_CreateTextureFromSurface(renderer, border1);
    platforms_t[8] = SDL_CreateTextureFromSurface(renderer, area2_s); // 2-long
    platforms_t[9] = SDL_CreateTextureFromSurface(renderer, bs_down_l3);
    platforms_t[10] = SDL_CreateTextureFromSurface(renderer, bs_up_l3);
    platforms_t[11] = SDL_CreateTextureFromSurface(renderer, bs_r3);
    platforms_t[12] = SDL_CreateTextureFromSurface(renderer, room_right3);
    platforms_t[13] = SDL_CreateTextureFromSurface(renderer, room_bot3);
    platforms_t[14] = SDL_CreateTextureFromSurface(renderer, room_top3);
    platforms_t[15] = SDL_CreateTextureFromSurface(renderer, box);
    platforms_t[16] = SDL_CreateTextureFromSurface(renderer, five);
    platforms_t[17] = SDL_CreateTextureFromSurface(renderer, ladder);
    platforms_t[18] = SDL_CreateTextureFromSurface(renderer, hp);
    platforms_t[19] = SDL_CreateTextureFromSurface(renderer, mid_platform3);
    platforms_t[20] = SDL_CreateTextureFromSurface(renderer, border3);

    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    Player player = {40, 540, 0, 0, 26*2, 36*2, spriteTexture};
     
    SDL_FreeSurface(bs_l1);
    SDL_FreeSurface(bs_r1);
    SDL_FreeSurface(area3_s);
    SDL_FreeSurface(area4_s);
    SDL_FreeSurface(area6_s);
    SDL_FreeSurface(bs_l2);
    SDL_FreeSurface(bs_r2);
    SDL_FreeSurface(border1);
    SDL_FreeSurface(area2_s);
    SDL_FreeSurface(bs_down_l3);
    SDL_FreeSurface(bs_up_l3);
    SDL_FreeSurface(bs_r3);
    SDL_FreeSurface(room_right3);
    SDL_FreeSurface(room_bot3);
    SDL_FreeSurface(room_top3);
    SDL_FreeSurface(box);
    SDL_FreeSurface(five);
    SDL_FreeSurface(ladder);
    SDL_FreeSurface(hp);
    SDL_FreeSurface(mid_platform3);
    SDL_FreeSurface(border3);

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
    {0, 0, 16 , SCREEN_HEIGHT, {255, 255, 255, 255}},
    {0, 0, SCREEN_WIDTH, 16, {255, 255, 255, 255}},
    {SCREEN_WIDTH - 16, 0, 16, 540, {255, 255, 255, 255}} 
    };

    Platform platforms2[] = {
    {0, 185, 128, 96, {255, 255, 255, 255}}, // left plato
    {190, 305, 96, 16, {255,255,255,255}}, // 1
    {380, 400, 192, 16, {255,255,255,255}}, // 2
    {640, 460, 128, 16, {255,255,255,255}}, // 3
    {890, 410, 96, 16, {255,255,255,255}}, 
    {1280-192, 350, 192, 96, {255,255,255,255}}  // right plato
    };

    Platform platforms3[] = {
    {20, 174, 32, 32, {255, 255, 255, 255}}, //left box 
    {0, 205, 160, 64, {255, 255, 255, 255}},
    {0, SCREEN_HEIGHT-64, 128, 64, {255, 255, 255, 255}},
    {240, 280, 128, 16, {255, 255, 255, 255}},
    {210, 575, 128, 16, {255, 255, 255, 255}},
    {420, 350, 96, 16, {255, 255, 255, 255}},
    {530, 405, 64, 16, {255, 255, 255, 255}},
    {655, 490, 128, 16, {255, 255, 255, 255}},
    {440, SCREEN_HEIGHT-160, 160, 160, {255, 255, 255, 255}},
    {835, 410, 64, 16, {255, 255, 255, 255}},
    {940, 350, 64, 16, {255, 255, 255, 255}},
    {1050, 300, 64, 16, {255, 255, 255, 255}},
    {910, 450, 64, 16, {255, 255, 255, 255}},
    {990, 500, 64, 16, {255, 255, 255, 255}},
    {SCREEN_WIDTH-256-33, SCREEN_HEIGHT-128-32, 256, 32, {255, 255, 255, 255}},
    {SCREEN_WIDTH-33, SCREEN_HEIGHT-128-32, 32, 128, {255, 255, 255, 255}},
    {SCREEN_WIDTH-256-33, SCREEN_HEIGHT-32, 256, 32, {255, 255, 255, 255}},
    {SCREEN_WIDTH-128, 280, 128, 64, {255, 255, 255, 255}},
    {0, 0, SCREEN_WIDTH, 16, {255, 255, 255, 255}},
    {0, 0, 16 , SCREEN_HEIGHT - 190, {255, 255, 255, 255}},
    {SCREEN_WIDTH - 16, 0, 16, 190, {255, 255, 255, 255}}, 
    {SCREEN_WIDTH - 16, 280, 16, SCREEN_HEIGHT - 280, {255, 255, 255, 255}}
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
                    go_left_for_jump = true;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    player.dx = 1.5;
                    go_right_animation = true;
                    go_left_for_jump = false;
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    jump(&player);
                }
            }
            else if (event.type == SDL_KEYUP) {
                if (event.key.keysym.sym == SDLK_LEFT) {
                    player.dx = 0;
                    go_left_animation = false;
                    go_left_for_jump = false;
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
            else if (arena_switch == 3)
            {
                arena_switch = 2;
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
            else if(arena_switch == 2)
            {
                first_load_player = true;
                arena_switch = 3;
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
            size = arr1_size;
        }
        else if (arena_switch == 2) {
            current_p = platforms2;
            size = arr2_size;
        }
        else if (arena_switch == 3) {
            current_p = platforms3;
            size = arr3_size;
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
            else if (arena_switch == 3)
            {
                player.x = 60;
                player.y = 510;
            }
            
            
        }


        //Player life
        for (unsigned long i = 0; i < size; i++) {
            if (player.x + player.w > current_p[i].x && player.x < current_p[i].x + current_p[i].w &&
                player.y + player.h > current_p[i].y && player.y < current_p[i].y + current_p[i].h) {
                if (player.dy > 0 && player.y + player.h < current_p[i].y + current_p[i].h) {
                    player.y = current_p[i].y - player.h;
                    player.dy = 0;
                    isJumping = false;
                    go_jump = false; // ???
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
                player.x = 50;
                player.y = 185-player.h;

                first_load_player = false;
            }
            SDL_RenderCopy(renderer, background_menu[1], NULL, NULL);   
            render_arena_2(renderer, platforms2, platforms_t);
            //arena_switch = 2;        
        }
        else if (arena_switch == 3)
        {
            if (first_load_player)
            {
                player.x = 50;
                player.y = 685-player.h;

                first_load_player = false;
            }
            SDL_RenderCopy(renderer, background_menu[2], NULL, NULL);   
            render_arena_3(renderer, platforms3, platforms_t);
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
            playerRect.w = 27*2;
            SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex], NULL, &playerRect);
        
        }
        else if (go_left_animation)
        {
            if (currentTime - startTime >= ANIMATION_SPEED) {            
                currentImageIndex = (currentImageIndex + 1) % animationFrameCount;
                startTime = currentTime;
            }
            playerRect.w = 27*2;
            SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex+7], NULL, &playerRect);
        }
        else if (go_jump)
        {
            if (currentTime - startTime >= ANIMATION_SPEED) {            
                currentImageIndex = (currentImageIndex + 1) % 8;
                startTime = currentTime;
            }
            playerRect.w = 20*2;
            if(go_left_for_jump)
            {
                SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex+14+8], NULL, &playerRect);
            }
            else
            {
                SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex+14], NULL, &playerRect);
            }
            
        }
        else
        {
            player.w = 17*2;
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
