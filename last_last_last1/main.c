#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.4 //0.3
#define JUMP_FORCE -11 //const 0.4 gravity -11 jump force

int currentImageIndex = 0;
int currentCircularIndex = 0;
int currentSteamIndex = 0;
int animationFrameCount = 7; // Number of animation frames
const int ANIMATION_SPEED = 250; // milliseconds per frame
const int ANIMATION_SPEED_FOR_CIRCULAR = 50; // milliseconds per frame
const int ANIMATION_SPEED_FOR_STEAM = 500;
bool go_left = false;
bool go_left_animation = false;
bool go_right_animation = false;
bool go_jump = false;
bool go_left_for_jump = false;
bool block_animation_while_jumping = false;
int arena_switch = 3;
bool first_load_player_back = 0;
bool first_load_player_front = 0;

Uint32 currentTime_for_circular = 0;
Uint32 startTime_for_circular = 0;

Uint32 currentTime_for_steam = 0;
Uint32 startTime_for_steam = 0;

int y_min = 130;
int y_max = 170;

int y_hp_arena_3_1 = 150;

int k = 1;


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
int arr2_size = 10;
int arr3_size = 23;
int arr4_size = 27;
int arr5_size = 25;

void apply_gravity(Player *player) {
    player->dy += GRAVITY;
}

bool isJumping = false;

void jump(Player *player) {
    if (player->dy == 0 && !isJumping) {
        player->dy = JUMP_FORCE;
        isJumping = true;
        go_jump = true;
        block_animation_while_jumping = true;
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

    int count = 5; //////////////////
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
    SDL_RenderCopy(renderer, platforms_t[7], NULL, &Rect_bord); 
}

void render_arena_2(SDL_Renderer * renderer, Platform *platforms, SDL_Texture *platforms_t[])
{
    SDL_Rect pRect2_l = {0, 184, 128, 96}; // left plato
    SDL_Rect pRect2_r = {1280-192, 362, 192, 96}; // right plato
    SDL_Rect pRect_platform1 = {190, 305, 96, 16}; // 1
    SDL_Rect pRect_platform2 = {380, 400, 192, 16}; // 2
    SDL_Rect pRect_platform3 = {640, 460, 160, 16}; 
    SDL_Rect pRect_platform4 = {890, 410, 96, 16}; 
    SDL_Rect pRect_bord = {0, 0, 1280, 720};    ////////////////
    SDL_RenderCopy(renderer, platforms_t[5], NULL, &pRect2_l);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform1);
    SDL_RenderCopy(renderer, platforms_t[3], NULL, &pRect_platform2);
    SDL_RenderCopy(renderer, platforms_t[36], NULL, &pRect_platform3); 
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform4);
    SDL_RenderCopy(renderer, platforms_t[6], NULL, &pRect2_r); 
    SDL_RenderCopy(renderer, platforms_t[21], NULL, &pRect_bord); 
    
} 

void render_arena_3(SDL_Renderer * renderer, Platform *platforms, SDL_Texture *platforms_t[], SDL_Texture *circulars_t[], SDL_Texture *steam_t[])
{
    SDL_Rect pRect_l_top = {0, 205, 160, 64}; // left_top_platform
    SDL_Rect pRect_l_bottom = {0, SCREEN_HEIGHT-64, 128, 64}; // left_top_platform
    SDL_Rect pRect_platform1_4_top = {240, 280, 128, 16}; // 4-long left up
    SDL_Rect pRect_platform1_4_bottom = {210, 575, 128, 16}; // 4-long left bottom
    SDL_Rect pRect_platform1_3_mid = {420, 350, 96, 16}; // 3-long mid
    //SDL_Rect pRect_platform1_2_mid = {530, 405, 64, 16}; // 2-long mid
    SDL_Rect pRect_platform1_4_mid = {655, 490, 128, 16}; // 4-long mid
    SDL_Rect pRect_platform1_sq_mid = {440, SCREEN_HEIGHT-160, 160, 160}; // square mid
    SDL_Rect pRect_platform1_2_right_1 = {835, 410, 64, 16}; // 2-long right mid //
    SDL_Rect pRect_platform1_2_right_2 = {940, 350, 64, 16}; // 2-long right up
    SDL_Rect pRect_platform1_2_right_3 = {1050, 300, 64, 16}; // 2-long right up up //
    SDL_Rect pRect_platform1_2_right_4 = {910, 450, 64, 16}; // 2-long right down //
    SDL_Rect pRect_platform1_2_right_5 = {990, 500, 64, 16}; // 2-long right down down //
    SDL_Rect pRect_platform1_3_right_6 = {815, 655, 96, 16}; // 3-long
    SDL_Rect pRect_room_top = {SCREEN_WIDTH-256-33, SCREEN_HEIGHT-128-32, 256, 32}; // room_top//
    SDL_Rect pRect_room_right = {SCREEN_WIDTH-33, SCREEN_HEIGHT-128-32, 32, 128}; // room_right //
    SDL_Rect pRect_room_bottom = {SCREEN_WIDTH-256-33, SCREEN_HEIGHT-32, 256, 32}; // room_bottom //
    SDL_Rect pRect_platform_finish = {SCREEN_WIDTH-128, 280, 128, 64}; // final_platform 
    SDL_Rect pRect_box1 = {20, 181, 32, 25}; // box left
    SDL_Rect pRect_box2 = {1197, 664, 32, 25}; // box right
    SDL_Rect pRect_ladder = {SCREEN_WIDTH-159, SCREEN_HEIGHT-171, 32, 75}; //ladder 
    SDL_Rect pRect_bord = {0, 0, 1280, 720};
    SDL_Rect pRect_circular1 = {273, 542, 32, 32};
    SDL_Rect pRect_circular2 = {464, 317, 32, 32};
    SDL_Rect pRect_circular3 = {707, 458, 32, 32};
    SDL_Rect pRect_circular4 = {835, 378, 32, 32};
    SDL_Rect pRect_circular5 = {545, 528, 32, 32};
    SDL_Rect pRect_steam1 = {1088, 264, 32, 36};
    SDL_Rect pRect_hp = {33, y_hp_arena_3_1, 64, 64};
    

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
    //SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_mid); //
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect_platform1_4_mid); //
    SDL_RenderCopy(renderer, platforms_t[19], NULL, &pRect_platform1_sq_mid); //
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_1);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_2);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_3);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_4);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect_platform1_2_right_5);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect_platform1_3_right_6);
    SDL_RenderCopy(renderer, platforms_t[14], NULL, &pRect_room_top);
    SDL_RenderCopy(renderer, platforms_t[12], NULL, &pRect_room_right);
    SDL_RenderCopy(renderer, platforms_t[13], NULL, &pRect_room_bottom);
    SDL_RenderCopy(renderer, platforms_t[11], NULL, &pRect_platform_finish);
    SDL_RenderCopy(renderer, platforms_t[15], NULL, &pRect_box1);
    SDL_RenderCopy(renderer, platforms_t[15], NULL, &pRect_box2);
    SDL_RenderCopy(renderer, platforms_t[17], NULL, &pRect_ladder);
    SDL_RenderCopy(renderer, platforms_t[20], NULL, &pRect_bord);

    if (currentTime_for_circular - startTime_for_circular >= ANIMATION_SPEED_FOR_CIRCULAR)
    {
        currentCircularIndex = (currentCircularIndex + 1) % 3;
        startTime_for_circular = currentTime_for_circular;
    }
    if (currentTime_for_steam - startTime_for_steam >= ANIMATION_SPEED_FOR_STEAM);
    {
        currentSteamIndex = (currentSteamIndex + 1) % 6;
        startTime_for_steam = currentTime_for_steam;
    }
    if (y_hp_arena_3_1 >= y_max);
    {
        printf("y_max = %d\n", y_hp_arena_3_1);
        //printf("%d k=%d\n", y_hp_arena_3_1, k);
        //printf("y_hp_arena_3_1 = %d,  y_max = %d, y_min = %d\n", y_hp_arena_3_1, y_max, y_min);
        k = -1;
    }
    //
    if (y_hp_arena_3_1 <= y_min);
    {
        printf("y_min = %d\n", y_hp_arena_3_1);
        //printf("%d k=%d\n", y_hp_arena_3_1, k);
        //printf("y_hp_arena_3_1 = %d,  y_max = %d, y_min = %d\n", y_hp_arena_3_1, y_max, y_min);
        k = 1;
    }
    SDL_RenderCopy(renderer, circulars_t[currentCircularIndex], NULL, &pRect_circular1);
    SDL_RenderCopy(renderer, circulars_t[currentCircularIndex], NULL, &pRect_circular2);
    SDL_RenderCopy(renderer, circulars_t[currentCircularIndex], NULL, &pRect_circular3);
    SDL_RenderCopy(renderer, circulars_t[currentCircularIndex], NULL, &pRect_circular4);
    SDL_RenderCopy(renderer, circulars_t[currentCircularIndex], NULL, &pRect_circular5);

    SDL_RenderCopy(renderer, steam_t[currentSteamIndex], NULL, &pRect_steam1);

    y_hp_arena_3_1 += k;
    SDL_RenderCopy(renderer, platforms_t[18], NULL, &pRect_hp);

}

void render_arena_4(SDL_Renderer * renderer, Platform *platforms, SDL_Texture *platforms_t[])
{
    SDL_Rect pRect4_l_up = {0, 145, 128, 64}; 
    SDL_Rect pRect4_l_top = {0, 0, 256, 32}; 
    SDL_Rect pRect4_top_left_p = {256, 0, 576, 160};
    SDL_Rect pRect4_left_p = {256, 160, 96, 128}; 
    SDL_Rect pRect4_mid_l_p = {96, 287, 256, 160};
    SDL_Rect pRect4_mid_r_b = {352, 287, 224, 64}; 
    SDL_Rect pRect4_mid_r_r = {512, 255, 64, 32};
    SDL_Rect pRect4_l_room_b = {0, 688, 416, 32};
    SDL_Rect pRect4_l_room_top = {0, 528, 384, 32};
    SDL_Rect pRect4_l_room_reft = {224, 560, 160, 32};
    SDL_Rect pRect4_r_p = {1120, 155, 160, 576};
    SDL_Rect pRect4_room_l_p = {0, 560, 32, 128};
    SDL_Rect pRect4_mid_p = {635, 528, 160, 192};
    SDL_Rect pRect4_platform_up1_2 = {605, 263, 64, 16};
    SDL_Rect pRect4_platform_up2_2 = {705, 280, 64, 16};
    SDL_Rect pRect4_platform_up3_2 = {945, 270, 64, 16};
    SDL_Rect pRect4_platform_up4_2 = {1040, 217, 64, 16};
    SDL_Rect pRect4_platform_down1_2 = {450, 640, 64, 16};
    SDL_Rect pRect4_platform_down2_2 = {530, 585, 64, 16};
    SDL_Rect pRect4_platform1_3 = {705, 382, 96, 16};
    SDL_Rect pRect4_platform2_3 = {816, 328, 96, 16};
    SDL_Rect pRect4_box1 = {113, 663, 32, 25};
    SDL_Rect pRect4_box2 = {416, 262, 32, 25};
    SDL_Rect pRect4_border4 = {0, 0, 1280, 720};
    

    SDL_RenderCopy(renderer, platforms_t[22], NULL, &pRect4_l_up);
    SDL_RenderCopy(renderer, platforms_t[23], NULL, &pRect4_l_top);
    SDL_RenderCopy(renderer, platforms_t[24], NULL, &pRect4_top_left_p);
    SDL_RenderCopy(renderer, platforms_t[25], NULL, &pRect4_left_p);
    SDL_RenderCopy(renderer, platforms_t[26], NULL, &pRect4_mid_l_p);
    SDL_RenderCopy(renderer, platforms_t[27], NULL, &pRect4_mid_r_b);
    SDL_RenderCopy(renderer, platforms_t[28], NULL, &pRect4_mid_r_r);
    SDL_RenderCopy(renderer, platforms_t[29], NULL, &pRect4_l_room_b);
    SDL_RenderCopy(renderer, platforms_t[30], NULL, &pRect4_l_room_top);
    SDL_RenderCopy(renderer, platforms_t[31], NULL, &pRect4_l_room_reft);
    SDL_RenderCopy(renderer, platforms_t[32], NULL, &pRect4_r_p);
    SDL_RenderCopy(renderer, platforms_t[33], NULL, &pRect4_room_l_p);
    SDL_RenderCopy(renderer, platforms_t[34], NULL, &pRect4_mid_p);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect4_platform_up1_2);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect4_platform_up2_2);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect4_platform_up3_2);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect4_platform_up4_2);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect4_platform_down1_2);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect4_platform_down2_2);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect4_platform1_3);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect4_platform2_3);
    SDL_RenderCopy(renderer, platforms_t[15], NULL, &pRect4_box1);
    SDL_RenderCopy(renderer, platforms_t[15], NULL, &pRect4_box2);
    SDL_RenderCopy(renderer, platforms_t[35], NULL, &pRect4_border4);
}

void render_arena_5(SDL_Renderer * renderer, Platform *platforms, SDL_Texture *platforms_t[])
{
    SDL_Rect pRect5_top_part = {0, 0, 192, 288}; 
    SDL_Rect pRect5_room_left = {0, 320, 32, 128}; 
    SDL_Rect pRect5_room_top = {0, 288, 608, 32};
    SDL_Rect pRect5_room_bot = {0, 432, 606, 32}; 
    SDL_Rect pRect5_mid_up = {574, 400, 512, 32};
    SDL_Rect pRect5_left_platform = {0, 624, 192, 96};
    SDL_Rect pRect5_mid_platform1 = {355, 624, 96, 96};
    SDL_Rect pRect5_mid_platform2 = {607, 624, 96, 96};
    SDL_Rect pRect5_border5 = {0, 0, 1280, 720};
    SDL_Rect pRect5_right_platform = {1150, 188, 128, 96};
    SDL_Rect pRect5_2platform_1 = {525, 205, 64, 16};
    SDL_Rect pRect5_2platform_2 = {846, 320, 64, 16};
    SDL_Rect pRect5_2platform_3 = {1007, 228, 64, 16};
    SDL_Rect pRect5_3platform_1 = {670, 162, 96, 16};
    SDL_Rect pRect5_3platform_2 = {687, 290, 96, 16};
    SDL_Rect pRect5_3platform_3 = {831, 195, 96, 16};
    SDL_Rect pRect5_2down_platform_1 = {240, 672, 64, 16};
    SDL_Rect pRect5_2down_platform_2 = {495, 657, 64, 16};
    SDL_Rect pRect5_3down_platform_1 = {768, 584, 96, 16};
    SDL_Rect pRect5_3down_platform_2 = {959, 550, 96, 16};
    SDL_Rect pRect5_4down_platform_1 = {1152, 535, 128, 16};
    SDL_Rect pRect5_3down_platform_3 = {1184, 432, 96, 16};

    
    SDL_RenderCopy(renderer, platforms_t[37], NULL, &pRect5_top_part);
    SDL_RenderCopy(renderer, platforms_t[38], NULL, &pRect5_room_left);
    SDL_RenderCopy(renderer, platforms_t[39], NULL, &pRect5_room_top);
    SDL_RenderCopy(renderer, platforms_t[40], NULL, &pRect5_room_bot);
    SDL_RenderCopy(renderer, platforms_t[41], NULL, &pRect5_mid_up);
    SDL_RenderCopy(renderer, platforms_t[42], NULL, &pRect5_left_platform);
    SDL_RenderCopy(renderer, platforms_t[43], NULL, &pRect5_mid_platform1);
    SDL_RenderCopy(renderer, platforms_t[43], NULL, &pRect5_mid_platform2);
    SDL_RenderCopy(renderer, platforms_t[45], NULL, &pRect5_right_platform);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect5_2platform_1);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect5_2platform_2);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect5_2platform_3);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect5_3platform_1);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect5_3platform_2);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect5_3platform_3);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect5_2down_platform_1);
    SDL_RenderCopy(renderer, platforms_t[8], NULL, &pRect5_2down_platform_2);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect5_3down_platform_1);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect5_3down_platform_2);
    SDL_RenderCopy(renderer, platforms_t[2], NULL, &pRect5_4down_platform_1);
    SDL_RenderCopy(renderer, platforms_t[1], NULL, &pRect5_3down_platform_3);
    SDL_RenderCopy(renderer, platforms_t[44], NULL, &pRect5_border5);
}

int main(void) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    char* player_animation[] = {"1.png", "2.png", "3.png","4.png", "5.png", "6.png","7.png", 
                                "go_left_1.png", "go_left_2.png", "go_left_3.png", "go_left_4.png", "go_left_5.png", "go_left_6.png", "go_left_7.png",
                                "jump_up_1.png","jump_up_2.png","jump_up_3.png","jump_up_4.png","jump_up_5.png","jump_up_6.png","jump_up_7.png","jump_up_8.png",
                                "jump_left_up_1.png","jump_left_up_2.png","jump_left_up_3.png","jump_left_up_4.png","jump_left_up_5.png","jump_left_up_6.png",
                                "jump_left_up_7.png","jump_left_up_8.png"};
    char* background[] = {"background.png", "background2.png", "background_3.png", "background4.png","background5.png"};
        
    SDL_Window *window = SDL_CreateWindow("Platformer Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    loadMedia(player_animation, renderer);
    init_background(background, renderer);////////

    SDL_Surface *spriteSurface = IMG_Load("Oleg.png");
    if (!spriteSurface) printf("Error sprite\n");
    SDL_Surface *Oleg_reverse_s = IMG_Load("Oleg_reverse.png");
    if (!Oleg_reverse_s) printf("Error sprite\n");
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
    SDL_Surface *hp = IMG_Load("hp.png"); // 18 /////// hp
    if(!hp) printf("Error hp\n");
    SDL_Surface *mid_platform3 = IMG_Load("mid_platform3.png"); // 19
    if(!mid_platform3) printf("Error mid_platform3\n");
    SDL_Surface *border3 = IMG_Load("border3.png"); // 20
    if(!border3) printf("Error border3\n");
    SDL_Surface *border2 = IMG_Load("border2.png"); // 21
    if(!border2) printf("Error border2\n");
    SDL_Surface *bs_l_up4 = IMG_Load("left_up_platform4.png"); // 22
    if(!bs_l_up4) printf("Error bs_l_up4\n");
    SDL_Surface *bs_l_top4 = IMG_Load("left_up_top4.png"); // 23
    if(!bs_l_up4) printf("Error left_up_top4\n");
    SDL_Surface *top_l_p4 = IMG_Load("top_left_part4.png"); // 24
    if(!top_l_p4) printf("Error top_left_part4\n");
    SDL_Surface *l_p4 = IMG_Load("left_part4.png"); // 25
    if(!l_p4) printf("Error left_part4\n");
    SDL_Surface *mid_l_p4 = IMG_Load("mid_left_part4.png"); // 26
    if(!mid_l_p4) printf("Error mid_left_part4\n");
    SDL_Surface *mid_r_b4 = IMG_Load("mid_room_bot4.png"); // 27
    if(!mid_r_b4) printf("Error mid_room_bot4\n");
    SDL_Surface *mid_r_r4 = IMG_Load("mid_room_right4.png"); // 28
    if(!mid_r_r4) printf("Error mid_room_right4\n");
    SDL_Surface *l_room_b4 = IMG_Load("left_room_bot_part4.png"); // 29
    if(!l_room_b4) printf("Error left_room_bot_part4\n");
    SDL_Surface *l_room_top4 = IMG_Load("left_room_top4.png"); // 30
    if(!l_room_top4) printf("Error left_room_top4\n");
    SDL_Surface *l_room_reft4 = IMG_Load("left_room_reft_part4.png"); // 31
    if(!l_room_reft4) printf("Error left_room_reft_part4\n");
    SDL_Surface *right_p4 = IMG_Load("right_platform4.png"); // 32
    if(!right_p4) printf("Error right_platform4\n");
    SDL_Surface *l_room_p4 = IMG_Load("left_room_part4.png"); // 33
    if(!l_room_p4) printf("Error left_room_part4\n");
    SDL_Surface *mid_platform4 = IMG_Load("mid_platform4.png"); // 34
    if(!mid_platform4) printf("Error mid_platform4\n");
    SDL_Surface *border4 = IMG_Load("border4.png"); // 35
    if(!border4) printf("Error border4\n");
    SDL_Surface *area5 = IMG_Load("area5.png"); // 36
    if(!area5) printf("Error area5\n");    
    SDL_Surface *top_part5 = IMG_Load("top_part5.png"); // 37
    if(!top_part5) printf("Error top_part5\n");    
    SDL_Surface *room_left5 = IMG_Load("room_left5.png"); // 38
    if(!room_left5) printf("Error room_left5\n");    
    SDL_Surface *room_top5 = IMG_Load("room_top5.png"); // 39
    if(!room_top5) printf("Error room_top5\n");    
    SDL_Surface *room_bot5 = IMG_Load("room_bot5.png"); // 40
    if(!room_bot5) printf("Error room_bot5\n");    
    SDL_Surface *mid_up_p5 = IMG_Load("mid_up_platform5.png"); // 41
    if(!mid_up_p5) printf("Error mid_up_p5\n");   
    SDL_Surface *left_platform5 = IMG_Load("left_platform5.png"); // 42
    if(!left_platform5) printf("left_platform5\n");    
    SDL_Surface *mid_platform5 = IMG_Load("mid_platform5.png"); // 43
    if(!mid_platform5) printf("mid_platform5\n");    
    SDL_Surface *border5 = IMG_Load("border5.png"); // 44
    if(!border5) printf("border5\n");  
    SDL_Surface *right_platform5 = IMG_Load("right_platform5.png"); // 45
    if(!right_platform5) printf("right_platform5\n");
    SDL_Surface *circular1_s = IMG_Load("circular1.png"); // 46
    if(!circular1_s) printf("Error circular1.png\n");
    SDL_Surface *circular2_s = IMG_Load("circular2.png"); // 47
    if(!circular2_s) printf("Error circular2.png\n");
    SDL_Surface *circular3_s = IMG_Load("circular3.png"); // 48
    if(!circular3_s) printf("Error circular3.png\n");   

    
    SDL_Surface *steam1_s = IMG_Load("steam1.png"); // 49
    if(!steam1_s) printf("Error steam1.png\n");

    SDL_Surface *steam2_s = IMG_Load("steam2.png"); // 50
    if(!steam2_s) printf("Error steam2.png\n");   
    
    SDL_Surface *steam3_s = IMG_Load("steam3.png"); // 51
    if(!steam3_s) printf("Error steam3.png\n");   
    
    SDL_Surface *steam4_s = IMG_Load("steam4.png"); // 52
    if(!steam4_s) printf("Error steam4.png\n");   

    SDL_Surface *steam5_s = IMG_Load("steam5.png"); // 53
    if(!steam5_s) printf("Error steam5.png\n");   

    SDL_Surface *steam6_s = IMG_Load("steam6.png"); // 54
    if(!steam6_s) printf("Error steam6.png\n");
    

    SDL_Texture *platforms_t[46];
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
    platforms_t[15] = SDL_CreateTextureFromSurface(renderer, box); // box
    platforms_t[16] = SDL_CreateTextureFromSurface(renderer, five);
    platforms_t[17] = SDL_CreateTextureFromSurface(renderer, ladder);
    platforms_t[18] = SDL_CreateTextureFromSurface(renderer, hp); ////////// hp
    platforms_t[19] = SDL_CreateTextureFromSurface(renderer, mid_platform3);
    platforms_t[20] = SDL_CreateTextureFromSurface(renderer, border3);
    platforms_t[21] = SDL_CreateTextureFromSurface(renderer, border2);
    platforms_t[22] = SDL_CreateTextureFromSurface(renderer, bs_l_up4);
    platforms_t[23] = SDL_CreateTextureFromSurface(renderer, bs_l_top4);
    platforms_t[24] = SDL_CreateTextureFromSurface(renderer, top_l_p4);
    platforms_t[25] = SDL_CreateTextureFromSurface(renderer, l_p4);
    platforms_t[26] = SDL_CreateTextureFromSurface(renderer, mid_l_p4);
    platforms_t[27] = SDL_CreateTextureFromSurface(renderer, mid_r_b4);
    platforms_t[28] = SDL_CreateTextureFromSurface(renderer, mid_r_r4);
    platforms_t[29] = SDL_CreateTextureFromSurface(renderer, l_room_b4);
    platforms_t[30] = SDL_CreateTextureFromSurface(renderer, l_room_top4);
    platforms_t[31] = SDL_CreateTextureFromSurface(renderer, l_room_reft4);
    platforms_t[32] = SDL_CreateTextureFromSurface(renderer, right_p4);
    platforms_t[33] = SDL_CreateTextureFromSurface(renderer, l_room_p4);
    platforms_t[34] = SDL_CreateTextureFromSurface(renderer, mid_platform4);
    platforms_t[35] = SDL_CreateTextureFromSurface(renderer, border4);
    platforms_t[36] = SDL_CreateTextureFromSurface(renderer, area5); //5-long
    platforms_t[37] = SDL_CreateTextureFromSurface(renderer, top_part5);
    platforms_t[38] = SDL_CreateTextureFromSurface(renderer, room_left5);
    platforms_t[39] = SDL_CreateTextureFromSurface(renderer, room_top5);
    platforms_t[40] = SDL_CreateTextureFromSurface(renderer, room_bot5);
    platforms_t[41] = SDL_CreateTextureFromSurface(renderer, mid_up_p5);
    platforms_t[42] = SDL_CreateTextureFromSurface(renderer, left_platform5);
    platforms_t[43] = SDL_CreateTextureFromSurface(renderer, mid_platform5);
    platforms_t[44] = SDL_CreateTextureFromSurface(renderer, border5);
    platforms_t[45] = SDL_CreateTextureFromSurface(renderer, right_platform5);

    SDL_Texture *circulars_t[3];
    circulars_t[0] = SDL_CreateTextureFromSurface(renderer, circular1_s);
    circulars_t[1] = SDL_CreateTextureFromSurface(renderer, circular2_s);
    circulars_t[2] = SDL_CreateTextureFromSurface(renderer, circular3_s);

    SDL_Texture *steam_t[6];
    steam_t[0] = SDL_CreateTextureFromSurface(renderer, steam1_s);
    steam_t[1] = SDL_CreateTextureFromSurface(renderer, steam2_s);
    steam_t[2] = SDL_CreateTextureFromSurface(renderer, steam3_s);
    steam_t[3] = SDL_CreateTextureFromSurface(renderer, steam4_s);
    steam_t[4] = SDL_CreateTextureFromSurface(renderer, steam5_s);
    steam_t[5] = SDL_CreateTextureFromSurface(renderer, steam6_s);

    SDL_Texture *Oleg_reverse = SDL_CreateTextureFromSurface(renderer, Oleg_reverse_s);

    SDL_Texture *spriteTexture = SDL_CreateTextureFromSurface(renderer, spriteSurface);
    Player player = {800, 260, 0, 0, 26*2, 36*2, spriteTexture};
     
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
    SDL_FreeSurface(border2);
    SDL_FreeSurface(bs_l_up4);
    SDL_FreeSurface(bs_l_top4);
    SDL_FreeSurface(top_l_p4);
    SDL_FreeSurface(l_p4);
    SDL_FreeSurface(mid_l_p4);
    SDL_FreeSurface(mid_r_b4);
    SDL_FreeSurface(mid_r_r4);
    SDL_FreeSurface(l_room_b4);
    SDL_FreeSurface(l_room_top4);
    SDL_FreeSurface(l_room_reft4);
    SDL_FreeSurface(right_p4);
    SDL_FreeSurface(l_room_p4);
    SDL_FreeSurface(mid_platform4);
    SDL_FreeSurface(border4);
    SDL_FreeSurface(top_part5);
    SDL_FreeSurface(room_left5);
    SDL_FreeSurface(room_top5);
    SDL_FreeSurface(room_bot5);
    SDL_FreeSurface(mid_up_p5);
    SDL_FreeSurface(left_platform5);
    SDL_FreeSurface(mid_platform5);
    SDL_FreeSurface(border5);
    SDL_FreeSurface(right_platform5);
    SDL_FreeSurface(circular1_s);
    SDL_FreeSurface(circular2_s);
    SDL_FreeSurface(circular3_s);

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
    {0, 184, 128, 96, {255, 255, 255, 255}}, // left plato
    {190, 305, 96, 16, {255,255,255,255}}, // 1
    {380, 400, 192, 16, {255,255,255,255}}, // 2
    {640, 460, 160, 16, {255,255,255,255}}, // 3
    {890, 410, 96, 16, {255,255,255,255}}, 
    {1280-192, 362, 192, 96, {255,255,255,255}},  // right plato
    {0, 0, SCREEN_WIDTH, 16, {255, 255, 255, 255}},
    {0, 0, 16, 90, {255, 255, 255, 255}},
    {0, 184, 16, SCREEN_HEIGHT - 184, {255, 255, 255, 255}}
    //{SCREEN_WIDTH - 16, 0, 16, 720, {255, 255, 255, 255}} //////////////////////??????????????????
    };

    Platform platforms3[] = {
    {20, 181, 32, 25, {255, 255, 255, 255}}, //left box 
    {0, 205, 160, 64, {255, 255, 255, 255}},
    {0, SCREEN_HEIGHT-64, 128, 64, {255, 255, 255, 255}},
    {240, 280, 128, 16, {255, 255, 255, 255}},
    {210, 575, 128, 16, {255, 255, 255, 255}},
    {420, 350, 96, 16, {255, 255, 255, 255}},
    //{530, 405, 64, 16, {255, 255, 255, 255}},
    {655, 490, 128, 16, {255, 255, 255, 255}},
    {440, SCREEN_HEIGHT-160, 160, 160, {255, 255, 255, 255}},
    {835, 410, 64, 16, {255, 255, 255, 255}},
    {940, 350, 64, 16, {255, 255, 255, 255}},
    {1050, 300, 64, 16, {255, 255, 255, 255}},
    {910, 450, 64, 16, {255, 255, 255, 255}},
    {990, 500, 64, 16, {255, 255, 255, 255}},
    {815, 655, 96, 16, {255, 255, 255, 255}},
    {SCREEN_WIDTH-256-33, SCREEN_HEIGHT-128-32, 126, 32, {255, 255, 255, 255}}, ///////////////
    {1156, SCREEN_HEIGHT-128-32, 126, 32, {255, 255, 255, 255}},
    {SCREEN_WIDTH-33, SCREEN_HEIGHT-128-32, 32, 128, {255, 255, 255, 255}},
    {SCREEN_WIDTH-256-33, SCREEN_HEIGHT-32, 256, 32, {255, 255, 255, 255}},
    {SCREEN_WIDTH-128, 280, 128, 64, {255, 255, 255, 255}},
    {0, 0, SCREEN_WIDTH, 16, {255, 255, 255, 255}},
    {0, 0, 16 , SCREEN_HEIGHT - 190, {255, 255, 255, 255}},
    {SCREEN_WIDTH - 16, 0, 16, 190, {255, 255, 255, 255}}, 
    {SCREEN_WIDTH - 16, 280, 16, SCREEN_HEIGHT - 280, {255, 255, 255, 255}}
    };

    Platform platforms4[] = {
    {0, 145, 128, 64, {255, 255, 255, 255}},
    {0, 0, 256, 32, {255, 255, 255, 255}},
    {256, 0, 576, 160, {255, 255, 255, 255}},
    {256, 160, 96, 128, {255, 255, 255, 255}},
    {96, 287, 256, 160, {255, 255, 255, 255}},
    {352, 287, 224, 64, {255, 255, 255, 255}}, 
    {512, 255, 64, 32, {255, 255, 255, 255}},
    {0, 688, 416, 32, {255, 255, 255, 255}},
    {0, 528, 384, 32, {255, 255, 255, 255}},
    {224, 560, 160, 32, {255, 255, 255, 255}},
    {1120, 155, 160, 576, {255, 255, 255, 255}},
    {0, 560, 32, 128, {255, 255, 255, 255}},
    {635, 528, 160, 192, {255, 255, 255, 255}},
    {605, 263, 64, 16, {255, 255, 255, 255}},
    {705, 280, 64, 16, {255, 255, 255, 255}},
    {945, 270, 64, 16, {255, 255, 255, 255}},
    {1040, 217, 64, 16, {255, 255, 255, 255}},
    {450, 640, 64, 16, {255, 255, 255, 255}},
    {530, 585, 64, 16, {255, 255, 255, 255}},
    {705, 382, 96, 16, {255, 255, 255, 255}},
    {816, 328, 96, 16, {255, 255, 255, 255}},
    {113, 663, 32, 25, {255, 255, 255, 255}},
    {416, 262, 32, 25, {255, 255, 255, 255}},
    {0, 0, SCREEN_WIDTH, 16, {255, 255, 255, 255}},
    {0, 0, 16, 62, {255, 255, 255, 255}},
    {0, 145, 16, 575, {255, 255, 255, 255}},
    {1264, 0, 16, 70, {255, 255, 255, 255}}
    };

    Platform platforms5[] = {
    {0, 0, 192, 288, {255, 255, 255, 255}}, //
    {0, 320, 32, 128, {255, 255, 255, 255}}, //
    {0, 288, 608, 32, {255, 255, 255, 255}}, //
    {0, 432, 606, 32, {255, 255, 255, 255}}, //
    {574, 400, 512, 32, {255, 255, 255, 255}},
    {0, 624, 192, 96, {255, 255, 255, 255}},
    {355, 624, 96, 96, {255, 255, 255, 255}},
    {607, 624, 96, 96, {255, 255, 255, 255}},
    {1150, 188, 128, 96, {255, 255, 255, 255}}, //
    {525, 205, 64, 16, {255, 255, 255, 255}},
    {846, 320, 64, 16, {255, 255, 255, 255}},
    {1007, 228, 64, 16, {255, 255, 255, 255}},
    {670, 162, 96, 16, {255, 255, 255, 255}},
    {687, 290, 96, 16, {255, 255, 255, 255}},
    {831, 195, 96, 16, {255, 255, 255, 255}},
    {240, 672, 64, 16, {255, 255, 255, 255}},
    {495, 657, 64, 16, {255, 255, 255, 255}},
    {768, 584, 96, 16, {255, 255, 255, 255}},
    {959, 550, 96, 16, {255, 255, 255, 255}},
    {1152, 535, 128, 16, {255, 255, 255, 255}},
    {1184, 432, 96, 16, {255, 255, 255, 255}},
    {0, 0, 1280, 16, {255, 255, 255, 255}},
    {0, 0, 16, 535, {255, 255, 255, 255}},
    {1280-16, 0, 16, 100, {255, 255, 255, 255}},
    {1280-32, 200, 16, 532, {255, 255, 255, 255}}
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
                    go_left = true;
                }
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    player.dx = 1.5;
                    go_right_animation = true;
                    go_left_for_jump = false;
                    go_left = false;
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
                first_load_player_back = true;
            }
            else if (arena_switch == 3)
            {
                arena_switch = 2;
                first_load_player_back = true;
            }
            else if (arena_switch == 4)
            {
                arena_switch = 3;
                first_load_player_back = true;
            }
            else if (arena_switch == 5)
            {
                arena_switch = 4;
                first_load_player_back = true;
            }
                
        
            player.x = 0;
        }
        if (player.x + player.w > SCREEN_WIDTH + 40) {
            player.x = SCREEN_WIDTH - player.w;
            if (arena_switch == 1)
            {
                first_load_player_front = true;
                arena_switch = 2;
            }
            else if(arena_switch == 2)
            {
                first_load_player_front = true;
                arena_switch = 3;
            }
            else if(arena_switch == 3)
            {
                first_load_player_front = true;
                arena_switch = 4;
            }
            else if(arena_switch == 4)
            {
                first_load_player_front = true;
                arena_switch = 5;
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
        else if (arena_switch == 4) {
            current_p = platforms4;
            size = arr4_size;
        }
        else if (arena_switch == 5) {
            current_p = platforms5;
            size = arr5_size;
        }

        if (player.y >  SCREEN_HEIGHT + 50 && player.x < SCREEN_WIDTH) { //Обработка возраждения
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
            else if (arena_switch == 4)
            {
                player.x = 60;
                player.y = 510;
            }
            else if (arena_switch == 5)
            {
                player.x = 60;
                player.y = 470;
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
                    block_animation_while_jumping = false;
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
        
        /*
         //Player life
        for (unsigned long i = 0; i < size; i++) {
            if (player.x + player.w > current_p[i].x && player.x < current_p[i].x + current_p[i].w &&
                player.y + player.h > current_p[i].y && player.y < current_p[i].y + current_p[i].h) {
                if (player.dy > 0 && player.y + player.h < current_p[i].y + current_p[i].h) {
                    player.y = current_p[i].y - player.h;
                    player.dy = 0;
                    isJumping = false;
                    go_jump = false; // ???
                    go_left_animation = true;
                    go_right_animation = true;
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
        */

        /*
        bool can_move_player_x = true;
        bool can_move_player_y = true;
        int temp_x = player.x + player.dx;
        int temp_y = player.y + player.dy;
        // Player life
        for (unsigned long i = 0; i < size; i++) {
            int block_left = current_p[i].x;
            int block_right = current_p[i].x + current_p[i].w;
            int block_top = current_p[i].y;
            int block_bottom = current_p[i].y + current_p[i].h;

            // Check for collision with block on X-axis
            if (temp_x + player.w > block_left && temp_x < block_right && player.y + player.h > block_top && player.y < block_bottom) {
                can_move_player_x = false;
            }

            // Check for collision with block on Y-axis
            if (temp_y + player.h > block_top && temp_y < block_bottom && player.x + player.w > block_left && player.x < block_right) {
                can_move_player_y = false;
            }
        }

        if (can_move_player_x) {
            player.x = temp_x;
        }

        if (can_move_player_y) {
            player.y = temp_y;
        }


        if (can_move_player_x)
        {
            player.x += player.dx;
        }

        if (can_move_player_y)
        {
            player.y += player.dy;
        }
        */

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        //Uint32 t = SDL_GetTicks();

        if (arena_switch == 1) {
            SDL_RenderCopy(renderer, background_menu[0], NULL, NULL);
            if (first_load_player_back)
            {
                player.x = SCREEN_WIDTH - 50;
                player.y = 550;
                first_load_player_back = false;
            }
            render_arena_1(renderer, platforms1, platforms_t); ///////
            
        }
        else if (arena_switch == 2)
        {
            if (first_load_player_front)
            {
                player.x = 50; // Когда он переходит с сцены 1 на 2
                player.y = 185-player.h;

                first_load_player_front = false;
            }
            else if (first_load_player_back)
            {
                player.x = SCREEN_WIDTH - 40; // Когда он переходит с сцены 3 на 2
                player.y = 360-player.h;

                first_load_player_back = false;
            }
            SDL_RenderCopy(renderer, background_menu[1], NULL, NULL);   
            render_arena_2(renderer, platforms2, platforms_t);
            //arena_switch = 2;        
        }
        else if (arena_switch == 3)
        {
            if (first_load_player_front)
            {
                player.x = 0;
                player.y = SCREEN_HEIGHT-player.h - 66; // Когда он переходит с сцены 2 на 3

                first_load_player_front = false;
            }
            else if (first_load_player_back)
            {
                player.x = SCREEN_WIDTH - 50; // Когда он переходит с сцены 4 на 3
                player.y = 210-player.h;

                first_load_player_back = false;
            }
            SDL_RenderCopy(renderer, background_menu[2], NULL, NULL);
            currentTime_for_circular = SDL_GetTicks();
            currentTime_for_steam = SDL_GetTicks();      
            render_arena_3(renderer, platforms3, platforms_t, circulars_t, steam_t);
            //arena_switch = 2;        
        }
        else if (arena_switch == 4)
        {
            if (first_load_player_front)
            {
                player.x = 200;
                player.y = 200; // Когда он переходит с сцены 3 на 4

                first_load_player_front = false;
            }
            else if (first_load_player_back)
            {
                player.x = SCREEN_WIDTH - 50; // Когда он переходит с сцены 5 на 4
                player.y = 185-player.h;

                first_load_player_back = false;
            }
            SDL_RenderCopy(renderer, background_menu[3], NULL, NULL);   
            render_arena_4(renderer, platforms4, platforms_t); ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //arena_switch = 2;        
        }
        else if (arena_switch == 5)
        {
            if (first_load_player_front)
            {
                player.x = 0;
                player.y = 624-player.h; // Когда он переходит с сцены 4 на 5

                first_load_player_front = false;
            }
            else if (first_load_player_back)
            {
                player.x = SCREEN_WIDTH - 50; // Когда он переходит с сцены 6 на 5 //DEFOLT
                player.y = 185-player.h;    //DEFOLT

                first_load_player_back = false;
            }
            SDL_RenderCopy(renderer, background_menu[4], NULL, NULL);   
            render_arena_5(renderer, platforms5, platforms_t); ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //arena_switch = 2;        
        }
        

        //Animation Player
        Uint32 currentTime = SDL_GetTicks();
        if(go_right_animation && !block_animation_while_jumping)
        {
            if (currentTime - startTime >= ANIMATION_SPEED) {
                currentImageIndex = (currentImageIndex + 1) % animationFrameCount;
                startTime = currentTime;
            }
            playerRect.w = 27*2;
            SDL_RenderCopy(renderer, gPlayerTextures[currentImageIndex], NULL, &playerRect);
        
        }
        else if (go_left_animation && !block_animation_while_jumping)
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
                if(currentImageIndex == 7) currentImageIndex = 7;
                else currentImageIndex = (currentImageIndex + 1) % 7;
                startTime = currentTime;
            }
            playerRect.w = 20*2;
            if(go_left_for_jump || go_left)
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
            if(go_left)
                SDL_RenderCopy(renderer, Oleg_reverse, NULL, &playerRect);
            else
                SDL_RenderCopy(renderer, player.texture, NULL, &playerRect);
            currentImageIndex = 0;
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
