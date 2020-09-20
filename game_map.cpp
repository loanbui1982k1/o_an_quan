
#include "game_map.h"
#include<stdio.h>
#include<conio.h>


void GameMap:: LoadTiles ( SDL_Renderer* screen)
{
    char file_img[30];
    FILE* fp = NULL;

    for ( int i = 0; i< MAX_TILE; i++)
    {
        sprintf(file_img,"map/map%d.png",i);
        fp = fopen(file_img,"rb");
        if ( fp == NULL)
        {
            continue;
        }
        fclose(fp);
        tile_map[i].Load_Image(file_img, screen);
    }
}
void GameMap:: DrawMap ( SDL_Renderer* screen, int BOARDGAME[], int KING[], int SCORE[], Textbox namebox[], Textbox scorebox[])
{

    game_map.tile[1][0] = BOARDGAME[0] ;
    game_map.tile[0][6] = BOARDGAME[6] ;
    if (KING[0] != 0 ) game_map.tile[0][0] = KING[0] +39;
    else game_map.tile[0][0] = 0;
    if (KING[1] != 0 ) game_map.tile[1][6] = KING[1] + 40;
    else game_map.tile[1][6] = 0;
    for ( int j = 1; j<= 5; j++)
    {
        game_map.tile[0][j] = BOARDGAME[j];
        game_map.tile[1][j] = BOARDGAME[12 - j];
    }
    for ( int i = 160; i<= 240; i+= 80)
    {
        for ( int j = 80; j<= 560; j += 80)
        {
            int val = game_map.tile[i/80 -2][j/80-1];
            if ( val >0 && val < 50)
            {
                tile_map[val].SetRect(j,i);
                tile_map[val].Render(screen);
            }
        }
    }
    scorebox[0].content = std::to_string(SCORE[0]);
    scorebox[1].content = std::to_string(SCORE[1]);
    namebox[0].render(screen);
    namebox[1].render(screen);
    scorebox[0].render(screen);
    scorebox[1].render(screen);
}
void GameMap:: WhileScattering(SDL_Renderer* screen,int BOARDGAME[], int KING[],  int stones, int PLAYER)
{
    if ( stones > 0 && stones < 30)
    {
        if ( PLAYER == 1)
        {
            tile_map[stones].SetRect(260, 360);
            tile_map[stones].Render(screen);
        }
        else if ( PLAYER == 0)
        {
            tile_map[stones].SetRect(420, 40);
            tile_map[stones].Render(screen);
        }
    }
}

