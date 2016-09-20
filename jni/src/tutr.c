#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include "menu.h"

/*
 * Yes, I may have just copy-pasted the play_game function
 * from game.c and modified it a bit. But it works so...
 */

int play_tutr(SDL_Renderer *renderer, TTF_Font *font, int width, int height) {
    static bool triggered = false;
    static bool hasShotX  = false;
    static bool hasShotY  = false;

    int wdBw = width / 18;
    int hdBw = height / 30;
    int bW   = 0;
    if (wdBw > hdBw) {
        bW = hdBw;
    } else {
        bW = wdBw;
    }
    int step = (bW * 18) / 90;

    static int xC       = 0;
    static int yC       = 0;
    static int trigDir  = 0;
    static int trigPos  = 0;
    static int xTrigPos = 0;

    static SDL_Texture *inst1;
    static SDL_Texture *inst2;
    SDL_Color textColor = {255, 255, 255};
    if (!inst1) {
        inst1 = string_to_texture(renderer, font, "Tap when the line is", textColor);
    }
    int txWidth, txHeight;
    SDL_QueryTexture(inst1, NULL, NULL, &txWidth, &txHeight);
    if (!inst2) {
        inst2 = string_to_texture(renderer, font, "over the red square ", textColor);
    }

    SDL_Event event;
    if (trigDir == 0) {
        trigDir = step;
    }
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 0;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_AC_BACK) {
                hasShotX  = false;
                hasShotY  = false;
                triggered = false;
                trigPos  = 0;
                xTrigPos = 0;
                trigDir  = 0;
                return 1;
            } else {
                triggered = true;
            }
        } else if (event.type == SDL_FINGERDOWN) {
            triggered = true;
        }
    }

    if (hasShotX && hasShotY) {
        hasShotX  = false;
        hasShotY  = false;
        triggered = false;
        trigPos  = 0;
        xTrigPos = 0;
        trigDir  = 0;
        return 1;
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

        SDL_RenderDrawLine(renderer, 0, bW * 30, width, bW * 30);
        SDL_RenderDrawLine(renderer, bW * 18, 0, bW * 18, height);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect targetRect;
        targetRect.x = 8 * bW;
        targetRect.y = (29 - 14) * bW;
        targetRect.w = bW;
        targetRect.h = bW;
        SDL_RenderFillRect(renderer, &targetRect);

        SDL_Rect dstRect;
        dstRect.x = 0;
        dstRect.y = (height * 3) / 4;
        dstRect.w = width;
        dstRect.h = (int) (((float)width / (float)txWidth) * txHeight);
        SDL_RenderCopy(renderer, inst1, NULL, &dstRect);

        dstRect.y = dstRect.y + txHeight;
        SDL_RenderCopy(renderer, inst2, NULL, &dstRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (!hasShotX) {
            SDL_Rect markRect;
            markRect.x = trigPos;
            markRect.y = 0;
            markRect.w = 2;
            markRect.h = height;
            if (triggered) {
                if (trigPos > (bW * 18) - 2) {
                    trigPos = (bW * 18) - 2;
                }
                xTrigPos = trigPos;
                xC = trigPos - (trigPos % bW);
                hasShotX = true;
                trigPos = height - (bW * 30) - step;
                trigDir = step;
                triggered = false;
                if (xC / bW != 8) {
                    hasShotX = false;
                    trigPos = 0;
                }
            }
            if (trigPos <= 0) {
                trigDir = step;
            } else if (trigPos >= bW * 18) {
                trigDir = -step;
            }
            SDL_RenderFillRect(renderer, &markRect);
        } else if (!hasShotY) {
            SDL_Rect markRect;
            markRect.x = 0;
            markRect.y = height - trigPos;
            markRect.w = width;
            markRect.h = 2;
            if (triggered) {
                if (trigPos < height - (bW * 30)) {
                    trigPos = height - (bW * 30);
                }
                trigPos -= height - (bW * 30);
                yC = trigPos + (bW - (trigPos % bW));
                hasShotY = true;
                trigPos = 0 - 2 * step;
                trigDir = step;
                triggered = false;
                if ((yC - bW) / bW != 14) {
                    hasShotY = false;
                    trigPos = height - (bW * 30) - step;
                }
            }
            if (trigPos <= height - (bW * 30)) {
                trigDir = step;
            } else if (trigPos >= height) {
                trigDir = -step;
            }
            SDL_RenderFillRect(renderer, &markRect);
            markRect.x = xTrigPos;
            markRect.y = 0;
            markRect.w = 2;
            markRect.h = height;
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &markRect);
        }
        SDL_RenderPresent(renderer);
    }
    trigPos += trigDir;
    return 3;
}

