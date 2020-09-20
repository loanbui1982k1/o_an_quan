#include <iostream>
#include "commonfuntion.h"
#include "BaseObject.h"
#include "game_map.h"
#include "SDL_textTexturesFuncs.h"

struct PopupBox
{
    Textbox note;
    SDL_Rect area;
    void create(int x, int y, int w, int h, std::string content, TTF_Font* &f)
    {
        area.x = x;
        area.y = y;
        area.w = w;
        area.h = h;
        //int centerX = x + w / 2;
        //int centerY = y + h / 2;
        note.create(x, y, x + w, y + h, f, content, {0, 0, 0});
    }
    void render(SDL_Renderer* &renderer)
    {
        //SDL_RenderCopy(renderer, NULL, NULL, &area);
        SDL_SetRenderDrawColor(renderer, 63, 255, 191, 255);
        SDL_RenderFillRect(renderer, &area);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &area);
        note.render(g_screen);
        SDL_RenderPresent(renderer);
    }
};

GameMap game_map_;
BaseObject g_background;
Box box;
Direction img;
Textbox namebox[2];
Textbox scorebox[2];
TTF_Font *mFont = NULL;
PopupBox popupNextTurn;

void waitUntilSpacePressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
            (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
                if (e.key.keysym.sym == SDLK_SPACE)
                    return;
    }
}

bool InitData()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL_Init error: " << SDL_GetError() << "\n";
        return false;
    }

    g_windows = SDL_CreateWindow("Game SDL 2.0", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if ( g_windows == NULL)
    {
        std::cout << "Create window error:   " << SDL_GetError() << "\n";
        return false;
    }

    g_screen = SDL_CreateRenderer(g_windows, -1, SDL_RENDERER_ACCELERATED);
    if (g_screen == NULL)
    {
        std::cout << "Create renderer error:   " << SDL_GetError() << "\n";
        return false;
    }

    if (!initSDL_ttf())
    {
        std::cout << "failed to initialize SDL_ttf:   " << SDL_GetError() << "\n";
        return false;
    }
    if (!loadSDL_ttf(mFont, "font/Roboto-italic.ttf", 32))
    {
        std::cout << "failed to load font with ttf:   " << SDL_GetError() << "\n";
        return false;
    }
    namebox[0].create(60, 40, 220, 80, mFont, PLAYER_NAME[0], {0, 0, 0});
    scorebox[0].create(260, 40, 300, 80, mFont, "00", {0, 0, 0});
    namebox[1].create(500, 400, 680, 440, mFont, PLAYER_NAME[1], {0, 0, 0});
    scorebox[1].create(420, 400, 460, 440, mFont, "00", {0, 0, 0});
    popupNextTurn.create(60, 200, 600, 80, "popupNextTurn's default content", mFont);

    return true;
}

bool LoadBackground()
{
    bool ret = g_background.Load_Image("anhgame//background.png", g_screen);
    if ( !ret) return false;
    return true;
}

void close()
{
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_windows);
    g_windows = NULL;
    IMG_Quit();
    SDL_Quit();
}

bool endGameCondition(int BOARDGAME[], int KING[]){
    return ( !( BOARDGAME[0] + KING[0]) && !(BOARDGAME[6] + KING[1]) );
}

void printGameBoard(int BOARDGAME[],int KING[], int SCORE[]){
    SDL_RenderClear(g_screen);
    g_background.Render(g_screen,NULL);
    game_map_.DrawMap(g_screen,BOARDGAME, KING, SCORE, namebox, scorebox);
    SDL_RenderPresent(g_screen);
}

void swapTurn(int& PLAYER){
    PLAYER = 1 - PLAYER;
}

bool scatterNewCondition( int BOARDGAME[], const int PLAYER){
    for (int i = 1; i<=5; i++){                     // PLAYER 1 : 1   2   3   4   5
        if ( BOARDGAME[i + (PLAYER) *6] )           // PLAYER 2 : 11  10  9   8   7
            return false;
    }
    return true;
}

void scatterNew(int BOARDGAME[], int KING[], const int PLAYER, int SCORE[]){
    if (scatterNewCondition(BOARDGAME, PLAYER)){
        for (int i = 1; i<=5; i++)
            BOARDGAME[i + (PLAYER) *6]++;
        SCORE[PLAYER]-=5;
        printGameBoard(BOARDGAME, KING, SCORE);
    }
    else return;
}

int collect(int BOARDGAME[], movement chosenTile){
    if (chosenTile.position == 0 || chosenTile.position == 6) { // if collecting from king tile, only get 1 stone
        BOARDGAME[chosenTile.position]--;
        return 1;
    }else{
        int stones = BOARDGAME[chosenTile.position];
        BOARDGAME[chosenTile.position] = 0;
        return stones;
    }
}

movement getNext(const movement chosenTile){
    movement nextTile;
    nextTile.position = (chosenTile.position + 12 + chosenTile.direction) % 12;
    nextTile.direction = chosenTile.direction;
    return nextTile;
}

movement scatter(int BOARDGAME[], int KING[], int SCORE[], const int PLAYER, movement chosenTile){
    movement nextTile = chosenTile;
    do
    {
    int stones = collect(BOARDGAME, nextTile);
        printGameBoard(BOARDGAME, KING, SCORE);
        game_map_.WhileScattering(g_screen,BOARDGAME, KING, stones, PLAYER);
        SDL_RenderPresent(g_screen);
        SDL_Delay(DELAY_TIME);
        while(stones){
            nextTile = getNext(nextTile);
            BOARDGAME[nextTile.position]++;
            stones--;
            printGameBoard(BOARDGAME, KING, SCORE);
            game_map_.WhileScattering(g_screen,BOARDGAME, KING, stones, PLAYER);
            SDL_RenderPresent(g_screen);
            SDL_Delay(DELAY_TIME);
        }
        nextTile = getNext(nextTile);
    } while(BOARDGAME[nextTile.position]);
    return nextTile;
}

movement select(int BOARDGAME[], int KING[], int SCORE[], const int PLAYER){
    movement chosenTile;
    if ( PLAYER == 1) box.y = 240;
    else if ( PLAYER == 0) box.y = 160;
    bool is_quit = false;

    popupNextTurn.note.content = PLAYER_NAME[PLAYER] + "'s turn. Press SPACE to continue.";
    popupNextTurn.render(g_screen);
    waitUntilSpacePressed();

    while ( ! is_quit)
    {
        while ( SDL_PollEvent( &g_event) != 0)
        {
           if( g_event.type == SDL_QUIT)
           {
               is_quit = true;
           }
        }
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        g_background.Render(g_screen,NULL);
        game_map_.DrawMap(g_screen, BOARDGAME, KING, SCORE, namebox, scorebox);
        bool kt = false;
        SDL_Event e;
        while ( kt == false )
        {
            while ( e.key.keysym.sym != SDLK_RETURN)
            {
                box.render(g_screen);
                SDL_RenderPresent(g_screen);
                if ( SDL_WaitEvent(&e) == 0) continue;
                if ( e.type == SDL_KEYDOWN)
                   {
                       switch (e.key.keysym.sym)
                       {
                           case SDLK_LEFT : box.turnleft(); break;
                           case SDLK_RIGHT: box.turnright(); break;
                           default: break;
                       }
                   }
                SDL_RenderClear(g_screen);
                g_background.Render(g_screen,NULL);
                game_map_.DrawMap(g_screen,BOARDGAME, KING, SCORE, namebox, scorebox);
            }
            chosenTile.position = box.x/80 - 1;
            if (PLAYER) chosenTile.position = 12 - chosenTile.position;
            if ( BOARDGAME[chosenTile.position] != 0) kt = true;
            else if ( SDL_WaitEvent(&e) == 0) continue;
        }
        chosenTile.position = box.x/80 - 1;
        if (PLAYER) chosenTile.position = 12 - chosenTile.position;
        img.Load_Image("anhgame//direction.png", g_screen);
        img.SetRect(box.x,box.y);
        img.Render(g_screen);
        SDL_RenderPresent(g_screen);

        bool checkDir = false;
        while ( !checkDir)
        {
            if ( SDL_WaitEvent(&e) == 0) continue;
            if ( e.type == SDL_QUIT) break;
            if ( e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_LEFT:
                    {
                        if (PLAYER) chosenTile.direction = 1;
                        else chosenTile.direction = -1;
                        is_quit = true;
                        checkDir = true;
                        break;
                    }
                    case SDLK_RIGHT:
                        {
                            if ( PLAYER) chosenTile.direction = -1;
                            else chosenTile.direction = 1;
                            is_quit = true;
                            checkDir = true;
                            break;
                        }
                    default: break;
                }
            }
        }

    SDL_RenderPresent(g_screen);
    }
    return chosenTile;
}

int sum(int BOARDGAME[], int KING[], movement lastTile){ // second next Tile after the last tile of scattering
    int point = 0;
    movement nextTile = lastTile;
    while (true){
        nextTile = getNext(nextTile);
        if (nextTile.position == 0 && !KING[0] && BOARDGAME[0] == 0) break;
		if (nextTile.position == 6 && !KING[1] && BOARDGAME[6] == 0) break;
		if (nextTile.position != 0 && nextTile.position != 6 && BOARDGAME[nextTile.position] == 0) break;
		if (nextTile.position == 0 && KING[0])
		{
			point += 10;
			KING[0] = 0;
		}
		if (nextTile.position == 6 && KING[1])
		{
			point += 10;
			KING[1] = 0;
		}
		point += BOARDGAME[nextTile.position];
		BOARDGAME[nextTile.position] = 0;
		nextTile = getNext(nextTile);
		if (BOARDGAME[nextTile.position] != 0) break;
	}
    return point;
}

int main(int argc, char* argv[])
{
    if ( !InitData()) return -1;
    if ( !LoadBackground()) return -1;
    int BOARDGAME[12], KING[2];
    for (int i = 0; i<12; i++) BOARDGAME[i] = 5;
    BOARDGAME[0] = 0;
    BOARDGAME[6] = 0;
    int SCORE[2] = {};
    KING[0] = 1; KING[1] = 1;
    game_map_.LoadTiles(g_screen);
    box.x = 160;
    box.y = 160;
    int PLAYER = 0;

    SDL_RenderClear(g_screen);
    g_background.Render(g_screen,NULL);
    game_map_.DrawMap(g_screen, BOARDGAME, KING, SCORE, namebox, scorebox);
    while(!endGameCondition(BOARDGAME, KING))
    {
        scatterNew(BOARDGAME, KING, PLAYER, SCORE);
        movement chosenTile = select(BOARDGAME, KING, SCORE, PLAYER);
        movement nextTile = scatter(BOARDGAME, KING, SCORE, PLAYER, chosenTile);
        SDL_Delay(DELAY_TIME);
        SCORE[PLAYER] += sum(BOARDGAME, KING, nextTile);
        swapTurn(PLAYER);
        printGameBoard(BOARDGAME, KING, SCORE);
        // printScore(SCORE);
    }
    SDL_RenderPresent(g_screen);
    closeSDL_ttf();
    close();
    return 0;
}
