#include "SDL.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "game.h"
#include "menu.h"
#include "tutr.h"

//unsigned int LastTicks;
int width;
int height;

/*
void frame_limit(int limit) {
    float minTicks, diffTicks;
    unsigned int sinceTicks;

    sinceTicks = SDL_GetTicks() - LastTicks;
    minTicks = 1000.0 / (float) limit;
    diffTicks = floor(minTicks - (float) sinceTicks + 0.5);

    if (diffTicks > 0) {
        SDL_Delay((int) diffTicks);
    }
    LastTicks = SDL_GetTicks();
}
*/


int main(int argc, char *argv[]) {
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();

    TTF_Font     *font;
    SDL_Window   *window;
    SDL_Renderer *renderer;
    SDL_RWops    *file;
    SDL_Surface  *logoSrfc;
    SDL_Texture  *logoTxtr;
    SDL_Texture  *gabdeg;

    window = SDL_CreateWindow(
            "Axes",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            0,
            0,
            SDL_WINDOW_SHOWN
            );
    renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
            );
    SDL_GetWindowSize(window, &width, &height);
    font = TTF_OpenFont("font.ttf", height / 20);

    file = SDL_RWFromFile("score.dat", "r");

    logoSrfc = SDL_LoadBMP("logo.bmp");
    logoTxtr = SDL_CreateTextureFromSurface(renderer, logoSrfc);
    SDL_FreeSurface(logoSrfc);
    logoSrfc = SDL_LoadBMP("gabdeg.bmp");
    gabdeg   = SDL_CreateTextureFromSurface(renderer, logoSrfc);
    SDL_FreeSurface(logoSrfc);

    while (!font) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }
    if (font) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect dstRect;
        dstRect.x = 0;
        dstRect.y = (height - (int)((float)width * 0.1368)) / 2;
        dstRect.w = width;
        dstRect.h = (int)((float)width * 0.1368);
        SDL_RenderCopy(renderer, gabdeg, NULL, &dstRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(1500);

        SDL_DestroyTexture(gabdeg);
    }

    //0 = quit
    //1 = menu
    //2 = game
    //3 = tutr
    int state = 1;

    int score  = 0;
    int hscore = 0;

    char hsBuf[4];
    SDL_RWread(file, hsBuf, sizeof(hsBuf), 4);
    SDL_RWclose(file);
    hscore = atoi(hsBuf);

    //LastTicks = SDL_GetTicks();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {}

    while (state > 0) {
        if (state == 2) {
            state = play_game(renderer, width, height, &score);
        } else if (state == 1) {
            if (score > hscore) {
                hscore = score;
                file = SDL_RWFromFile("score.dat", "w");
                sprintf(hsBuf, "%d", hscore);
                size_t hsBufLen = SDL_strlen(hsBuf);
                SDL_RWwrite(file, hsBuf, sizeof(hsBuf), hsBufLen);
                SDL_RWclose(file);
            }
            state = do_menu(renderer, font, logoTxtr, width, height, &score, hscore);
            SDL_Delay(30);
        } else if (state == 3) {
            state = play_tutr(renderer, font, width, height);
        }
    }
    exit(0);
}
