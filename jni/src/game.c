#include "SDL.h"
#include <stdbool.h>

int PlayGrid[18][30];

void draw_grid(SDL_Renderer *renderer, int bW, int height) {
    int x, y;
    for (x = 0; x < 18; x++) {
        for (y = 0; y < 30; y++) {
            if (PlayGrid[x][y] > 0) {
                switch (PlayGrid[x][y]) {
                    case 1:
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        break;
                    case 2:
                        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                        break;
                }
                SDL_Rect targetRect;
                targetRect.x = x * bW;
                targetRect.y = (29 - y) * bW;
                targetRect.w = bW;
                targetRect.h = bW;
                SDL_RenderFillRect(renderer, &targetRect);
            }
        }
    }
}

bool move_down() {
    int x;
    int y;
    for (x = 0; x < 18; x++) {
        if (PlayGrid[x][0] > 0) {
            return false;
        }
    }
    for (x = 0; x < 18; x++) {
        for (y = 0; y < 30; y++) {
            if (y > 0) {
                PlayGrid[x][y-1] = PlayGrid[x][y];
                PlayGrid[x][y] = 0;
            }
        }
    }
    return true;
}

void add_targets(int score) {
    int x, val;
    x = rand() % 18;
    if (score >= 15) {
        val = (rand() % 2) + 1;
    } else {
        val = 1;
    }
    PlayGrid[x][29] = val;
}

int play_game(SDL_Renderer *renderer, int width, int height, int *pscore) {
    static bool triggered = false;
    static bool hasShotX  = false;
    static bool hasShotY  = false;

    int step = width / 80;
    int wdBw = width / 18;
    int hdBw = height / 30;
    int bW   = 0;
    if (wdBw > hdBw) {
        bW = hdBw;
    } else {
        bW = wdBw;
    }

    static int xC       = 0;
    static int yC       = 0;
    static int trigDir  = 0;
    static int trigPos  = 0;
    static int xTrigPos = 0;

    int score = *pscore;

    static unsigned int lastMove = 0;

    SDL_Event event;
    if (trigDir == 0) {
        trigDir = step;
    }
    if (lastMove == 0) {
        lastMove = SDL_GetTicks();
    }
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 0;
        } else if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_AC_BACK) {
                int x, y;
                for (x = 0; x < 18; x++) {
                    for (y = 0; y < 30; y++) {
                        PlayGrid[x][y] = 0;
                    }
                }
                hasShotX  = false;
                hasShotY  = false;
                triggered = false;
                lastMove = 0;
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
        hasShotX = false;
        hasShotY = false;
        if (PlayGrid[xC / bW][(yC - bW) / bW] > 0) {
            PlayGrid[xC / bW][(yC - bW) / bW]--;
            score++;
        }
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);

        SDL_RenderDrawLine(renderer, 0, bW * 30, width, bW * 30);
        SDL_RenderDrawLine(renderer, bW * 18, 0, bW * 18, height);

        draw_grid(renderer, bW, height);
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
                if (trigPos < 2) {
                    trigPos = 2;
                }
                trigPos = trigPos - (height - (bW * 30));
                yC = trigPos + (bW - (trigPos % bW));
                hasShotY = true;
                trigPos = 0 - 2 * step;
                trigDir = step;
                triggered = false;
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
    *pscore = score;
    if (SDL_GetTicks() - lastMove > 2000) {
        bool run;
        run = move_down();
        add_targets(score);
        lastMove = SDL_GetTicks();
        if (!run) {
            int x, y;
            for (x = 0; x < 18; x++) {
                for (y = 0; y < 30; y++) {
                    PlayGrid[x][y] = 0;
                }
            }
            hasShotX  = false;
            hasShotY  = false;
            triggered = false;
            lastMove = 0;
            trigPos  = 0;
            xTrigPos = 0;
            trigDir  = 0;
            return 1;
        }
    }
    trigPos += trigDir;
    return 2;
}

