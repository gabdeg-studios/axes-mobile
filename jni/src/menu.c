#include "SDL.h"
#include "SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>

struct Button {
    int  x1;
    int  y1;
    int  x2;
    int  y2;
    bool pd;
};


struct Button pB;
struct Button qB;

SDL_Texture *string_to_texture(SDL_Renderer *renderer,
        TTF_Font *font, char str[], SDL_Color textColor) {
    SDL_Surface *textSurface;
    SDL_Texture *texture;

    textSurface = TTF_RenderText_Solid(font, str, textColor);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);
    return texture;
}

void render_menu(SDL_Renderer *renderer,
        TTF_Font *font,
        SDL_Texture *logo,
        int width,
        int height,
        int *score,
        int hscore
        ) {
    int txWidth, txHeight;
    int touchX = 0;
    int touchY = 0;
    char scoreStr[16];
    char hscoreStr[16];

    SDL_Rect    dstRect;
    SDL_Color   textColor = {255, 255, 255};
    SDL_Texture *textTexture;

    textTexture = string_to_texture(renderer, font, "PLAY", textColor);
    SDL_QueryTexture(textTexture, NULL, NULL, &txWidth, &txHeight);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    dstRect.x = (width  - txWidth) / 2;
    dstRect.y = (height / 2) - txHeight;
    dstRect.w = txWidth;
    dstRect.h = txHeight;

    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);
    textTexture = string_to_texture(renderer, font, "QUIT", textColor);

    dstRect.y = (height / 2) + txHeight;
    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    sprintf(scoreStr,  "Score:      %d", *score );
    sprintf(hscoreStr, "High Score: %d", hscore);

    textTexture = string_to_texture(renderer, font, hscoreStr, textColor);
    SDL_QueryTexture(textTexture, NULL, NULL, &txWidth, &txHeight);

    dstRect.x = (width - txWidth) / 2;
    dstRect.y = (height * 3) / 4;
    dstRect.w = txWidth;
    dstRect.h = txHeight;

    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);
    textTexture = string_to_texture(renderer, font, scoreStr, textColor);
    SDL_QueryTexture(textTexture, NULL, NULL, &txWidth, &txHeight);

    dstRect.y = (height * 3) / 4 + txHeight;
    dstRect.w = txWidth;
    dstRect.h = txHeight;

    SDL_RenderCopy(renderer, textTexture, NULL, &dstRect);

    dstRect.x = width / 4;
    dstRect.y = (height / 2) - txHeight;
    dstRect.w = width / 2;
    dstRect.h = txHeight;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &dstRect);
    dstRect.x = width / 4;
    dstRect.y = (height / 2) + txHeight;
    SDL_RenderDrawRect(renderer, &dstRect);

    SDL_QueryTexture(logo, NULL, NULL, &txWidth, &txHeight);
    txHeight = height / 10;
    txWidth  = (int) ((float) txHeight * 3.3984375);

    dstRect.x = (width - txWidth) / 2;
    dstRect.y = (height / 4) - (txHeight / 2);
    dstRect.w = txWidth;
    dstRect.h = txHeight;
    SDL_RenderCopy(renderer, logo, NULL, &dstRect);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(textTexture);
}

int do_menu(SDL_Renderer *renderer,
        TTF_Font *font,
        SDL_Texture *logo,
        int width, int height,
        int *score, int hscore
        ) {
    static bool hasRendered = false;

    int touchX;
    int touchY;
    struct Button pB;
    struct Button qB;
    if (!hasRendered) {
        render_menu(renderer, font, logo, width, height, score, hscore);
        hasRendered = true;
    }
    pB.x1 = width / 4;
    pB.y1 = (height / 2) - (height / 20);
    pB.x2 = pB.x1 + width / 2;
    pB.y2 = pB.y1 + (height / 20);
    pB.pd = false;

    qB.x1 = width / 4;
    qB.y1 = (height / 2) + (height / 20);
    qB.x2 = qB.x1 + width / 2;
    qB.y2 = qB.y1 + (height / 20);
    qB.pd = false;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 0;
        } else if (event.type == SDL_FINGERDOWN) {
            SDL_TouchFingerEvent tevent;
            tevent = event.tfinger;
            touchX = (int) (tevent.x * (float) width );
            touchY = (int) (tevent.y * (float) height);
            if ((touchX > pB.x1 && touchX < pB.x2) &&
                    (touchY > pB.y1 && touchY < pB.y2)) {
                pB.pd = true;
            } else if ((touchX > qB.x1 && touchX < qB.x2) &&
                    (touchY > qB.y1 && touchY < qB.y2)) {
                qB.pd = true;
            }
        }
    }
    if (pB.pd) {
        *score = 0;
        hasRendered = false;
        return 2;
    } else if (qB.pd) {
        *score = 0;
        hasRendered = false;
        return 0;
    }
    return 1;
}



