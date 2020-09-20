#ifndef SDL_TEXTTEXTURESFUNCS__H
#define SDL_TEXTTEXTURESFUNCS__H

#include <iostream>
#include <SDL_ttf.h>

struct Textbox
{
public:
    void create(const int &x1, const int &y1, const int &x2, const int &y2,
                TTF_Font *&_font, const std::string _content, const SDL_Color &_color);
    bool render(SDL_Renderer *&gRenderer);
    std::string content;
private:
    int x, y, w, h;
    TTF_Font *font;
    SDL_Color color;
};

bool initSDL_ttf();
void closeSDL_ttf();
bool loadSDL_ttf(TTF_Font *gFont[], const std::string FONT_NAME[], const int FONT_SIZE[], const int &NUM_OF_FONTS);
bool loadSDL_ttf(TTF_Font *&gFont, const std::string FONT_NAME, const int &FONT_SIZE);

#endif // SDL_TEXTTEXTURESFUNCS__H
