#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>

#define DESIRED_WIDTH 20
#define DESIRED_HEIGHT 20


SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture** gPlayerTextures = NULL; // Dynamically allocated array for player textures

int currentImageIndex = 0;
int animationFrameCount = 6; // Number of animation frames
const int ANIMATION_SPEED = 200; // milliseconds per frame

SDL_Surface* resizeSurface(SDL_Surface* surface, int newWidth, int newHeight) {
    // Create a new surface with the desired dimensions
    SDL_Surface* newSurface = SDL_CreateRGBSurface(0, newWidth, newHeight, 32,0,0,0,0);
    if (!newSurface) {
        SDL_Log("Failed to create surface: %s", SDL_GetError());
        return NULL;
    }
    
    // Use SDL's built-in scaling function to resize the surface
    if (SDL_BlitScaled(surface, NULL, newSurface, NULL) != 0) {
        SDL_Log("Failed to resize surface: %s", SDL_GetError());
        SDL_FreeSurface(newSurface);
        return NULL;
    }
    
    return newSurface;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_SHOWN);
    if (gWindow == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

bool loadMedia(char** fileNames, int count) {    
    gPlayerTextures = malloc(sizeof(SDL_Texture*) * count);
    if (gPlayerTextures == NULL) {
        printf("Memory allocation failed for textures array!\n");
        return false;
    }
    for (int i = 0; i < count; ++i) {
        SDL_Surface* surface = IMG_Load(fileNames[i]);
        if (surface == NULL) {
            printf("Unable to load image %s! SDL_image Error: %s\n", fileNames[i], IMG_GetError());
            return false;
        }

        gPlayerTextures[i] = SDL_CreateTextureFromSurface(gRenderer, surface);
        SDL_FreeSurface(surface);
        if (gPlayerTextures[i] == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", fileNames[i], SDL_GetError());
            return false;
        }
    }
    printf("Media loaded successfully.\n");
    return true;
}


void closeSDL() {
    if (gPlayerTextures != NULL) {
        for (int i = 0; i < animationFrameCount; ++i) {
            SDL_DestroyTexture(gPlayerTextures[i]);
        }
        free(gPlayerTextures);
    }
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[]) {
    char* player_animation[] = {"1.png", "2.png", "3.png","4.png", "5.png", "6.png"}; // Example animation frames
    int animationFrameCount = sizeof(player_animation) / sizeof(player_animation[0]);

    if (!initSDL()) {
        printf("Failed to initialize!\n");
        return -1;
    }
    if (!loadMedia(player_animation, animationFrameCount)) {
        printf("Failed to load media!\n");
        return -1;
    }
    bool quit = false;
    SDL_Event e;
    Uint32 startTime = SDL_GetTicks();
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        // Calculate frame time
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - startTime >= ANIMATION_SPEED) {
            // Render current frame
            SDL_RenderClear(gRenderer);
            SDL_RenderCopy(gRenderer, gPlayerTextures[currentImageIndex], NULL, NULL);
            SDL_RenderPresent(gRenderer);
            // Move to next frame
            currentImageIndex = (currentImageIndex + 1) % animationFrameCount;
            startTime = currentTime;
        }
    }
    closeSDL();
    return 0;
}
