int do_menu(SDL_Renderer *renderer,
        TTF_Font *font,
        SDL_Texture *logo,
        int width, int height,
        int *score, int hscore
        );

SDL_Texture *string_to_texture(SDL_Renderer *renderer,
        TTF_Font *font, char str[], SDL_Color textColor);
