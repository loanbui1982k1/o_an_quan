#ifndef COMMON_FUNTION_H_
#define COMMON_FUNTION_H_

#include<windows.h>
#include<SDL.h>
#include<SDL_image.h>
#include<string>
#include<SDL_ttf.h>

static SDL_Window* g_windows = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;
//Screen
const int SCREEN_WIDTH = 720;
const int SCREEN_HEIGHT = 480;
const int SPEED_MAIN = 8;
const int SCREEN_BPP = 32;
const int COLOR_KEY_B = 255;
const int COLOR_KEY_G = 255;
const int COLOR_KEY_R = 255;
const int RENDER_DRAW_COLOR = 255;
const int DELAY_TIME = 150;
const std::string PLAYER_NAME[2] = {"Player 1", "Player 2"};

#define TILE_SIZE 80

#define MAX_MAP_X 9
#define MAX_MAP_Y 7
struct Map
{
    int start_x;
    int start_y;

    int max_x;
    int max_y;

    int tile[2][7];
};
struct Box
{
    int x, y;
    int size = 80;
    void render( SDL_Renderer* renderer)
    {
        SDL_Rect filled_rect;
        for (int i = 0; i <= 2; ++ i)
        {
            filled_rect.x = x + i;
            filled_rect.y = y + i;
            filled_rect.w = size + 1 - 2 * i;
            filled_rect.h = size + 1 - 2 * i;
            SDL_SetRenderDrawColor(renderer, 255, 128, 128, 0);
            SDL_RenderDrawRect( renderer, &filled_rect);
        }
    }
    void turnright()
    {
        if (x < 480) x += 80;
    }
    void turnleft()
    {
        if (x > 160) x -= 80;
    }
};
struct movement{
    int position;
    int direction;
    // 1 = clockwise; -1 = counter-clockwise;
};

SDL_Surface* Load_Image(std:: string fie_path);

#endif // COMMON_FUNTION_H_
