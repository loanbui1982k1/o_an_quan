
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "commonfuntion.h"
#include "BaseObject.h"
#include "SDL_textTexturesFuncs.h"

#define MAX_TILE 50

class TileMat: public BaseObject
{
public:
    TileMat(){;}
    ~TileMat(){;}
};
class Direction: public BaseObject
{
public:
    Direction(){;}
    ~Direction(){;}
};
class GameMap
{
public:
    GameMap() {;}
    ~GameMap() {;}
    void LoadTiles( SDL_Renderer* screen);
    void DrawMap ( SDL_Renderer* screen, int BOARDGAME[], int KING[], int SCORE[], Textbox namebox[], Textbox scorebox[]);
    void WhileScattering(SDL_Renderer* screen, int BOARDGAME[], int KING[], int stones, int PLAYER);

private:
    Map game_map;
    TileMat tile_map[MAX_TILE];
};
#endif // GAME_MAP_H
