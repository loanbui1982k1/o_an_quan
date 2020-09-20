#include "SDL_textTexturesFuncs.h"

bool initSDL_ttf()
{
    if (TTF_Init() < 0)
    {
        std::cout << "SDL_ttf could not be initialized. SDL Error: " << TTF_GetError() << "\n";
        return false;
    }

    return true;
}

void closeSDL_ttf()
{
    TTF_Quit();
}

bool loadSDL_ttf(TTF_Font *gFont[], const std::string FONT_NAME[], const int FONT_SIZE[], const int &NUM_OF_FONTS)
{
    for (int i = 0; i < NUM_OF_FONTS; ++ i)
    {
        gFont[i] = NULL;
        gFont[i] = TTF_OpenFont(FONT_NAME[i].c_str(), FONT_SIZE[i]);
        if (gFont[i] == NULL)
        {
            std::cout << "Failed to load font " << i << " texture " << FONT_NAME[i] << "\n";
            return false;
        }
    }

    return true;
}

bool loadSDL_ttf(TTF_Font *&gFont, const std::string FONT_NAME, const int &FONT_SIZE)
{
    gFont = NULL;
    gFont = TTF_OpenFont(FONT_NAME.c_str(), FONT_SIZE);
    if (gFont == NULL)
    {
        std::cout << "Failed to load font texture " << FONT_NAME << "\n";
        return false;
    }
    return true;
}

void Textbox::create(const int &x1, const int &y1, const int &x2, const int &y2,
                     TTF_Font *&_font, const std::string _content = "Null textbox", const SDL_Color &_color = {0, 0, 0})
{
    x = x1;
    y = y1;
    w = x2 - x1;
    h = y2 - y1;
    content = _content;
    font = _font;
    color = _color;
}

bool Textbox::render(SDL_Renderer *&gRenderer)
{
    SDL_Texture* tTexture = NULL;
	SDL_Surface* tSurface = TTF_RenderText_Solid(font, content.c_str(), color);
	if (tSurface == NULL)
    {
        std::cout << "Unable to render text surface. SDL_ttf Error: " << TTF_GetError() << "\n";
        return false;
    }
    tTexture = SDL_CreateTextureFromSurface(gRenderer, tSurface);
    if (tTexture == NULL)
    {
        std::cout << "Unable to create texture from rendered text. SDL Error: " << SDL_GetError() << "\n";
        return false;
    }
    int aw = tSurface->w, ah = tSurface->h; // actual font print size
    int ax = x, ay = y + h - tSurface->h; // actual fonr print coordinate
    SDL_FreeSurface(tSurface);

    SDL_Rect coor = {ax, ay, aw, ah};
    SDL_RenderCopy(gRenderer, tTexture, NULL, &coor);

    SDL_DestroyTexture(tTexture);

	return true;
}
