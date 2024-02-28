#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GRAVITY 0.5
#define JUMP_FORCE -15

int currentImageIndex = 0;
int animationFrameCount = 6; // Number of animation frames
const int ANIMATION_SPEED = 250; // milliseconds per frame
bool go_left_animation = false;
bool go_right_animation = false;
int arena_switch = 1;
bool first_load_player = 0;

int switch_bg = 0;

SDL_Renderer* gRenderer = NULL;
SDL_Texture** gPlayerTextures = NULL;// Dynamically allocated array for player textures
//SDL_Texture **background_menu = NULL;

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

bool isJumping = false;