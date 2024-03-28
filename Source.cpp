#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<stdlib.h>
#include <SDL_mixer.h>
//Them may nut tren man hinh: chon nhac (nhu game phuc), play again?
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;

//Start up SDL and create window
bool Init();

//the window we will use
SDL_Window* window = NULL;

SDL_Renderer* ren = NULL;

SDL_Texture* texture = NULL;

SDL_Texture* image = NULL; 

SDL_Texture* imageX;
SDL_Texture* imageO;
SDL_Texture* imagePlayAgain;
SDL_Texture* imageY;
SDL_Texture* imageN;

//Declare the music
Mix_Music* gMusic = NULL;       //the music that will be played
    //The sound effects that will be used (hieu ung am thanh)
Mix_Chunk* gScratch = NULL;


//Toa do nut bam chuot
int x, y;
int player = 0;

int table[12][12] = { 0 };  //max:11
bool result = 0;   //draw

//To mau nen trang -> Ve len do 1 cai bang 
void Draw_background()
{
    SDL_SetRenderTarget(ren, texture);      //chi dinh cai but ren de ve len texture
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255); 
    SDL_RenderFillRect(ren, NULL);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);  //ve duong mau den
    
    for (int i = 50; i <= 550; i += 50)
    {
        SDL_RenderDrawLine(ren, i, 0, i, 600);
        SDL_RenderDrawLine(ren, 0, i, 600, i);
    }

    SDL_SetRenderTarget(ren, NULL);

    Mix_PlayChannel(-1, gScratch, 1);
}

void Draw_PlayAgain()
{
    //Tao Play Again?
    SDL_Rect x; //x: la 1 hinh chu nhat
    x.x = 150;
    x.y = 75;
    x.w = 300;
    x.h = 100;

    image = imagePlayAgain;
    SDL_RenderCopy(ren, image, NULL, &x);

    //Tao Yes
    x.x = 50;
    x.y = 300;
    x.w = 100;
    x.h = 100;
    image = imageY;
    SDL_RenderCopy(ren, image, NULL, &x);

    //Tao No
    x.x = 450;
    x.y = 300;
    x.w = 100;
    x.h = 100;
    image = imageN;
    SDL_RenderCopy(ren, image, NULL, &x);
}

//Khoi tao cua so, hinh anh
bool Init()
{
    //Initialization flag: maybe like bool check
    bool success = true;

    //Create window
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        cout << "fail to initialize the window" << endl;
        cout << "SDL_Error: " << SDL_GetError() << endl;
    }
    else
    {
        window = SDL_CreateWindow("X_O Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        
        ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

        texture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    IMG_Init(IMG_INIT_PNG);
    imageX = IMG_LoadTexture(ren, "image/x.png");
    imageO = IMG_LoadTexture(ren, "image/O.png");
    imagePlayAgain = IMG_LoadTexture(ren, "image/play-again.png");
    imageY = IMG_LoadTexture(ren, "image/Yes.png");
    imageN = IMG_LoadTexture(ren, "image/x.png");

    //Load music
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    //gMusic = Mix_LoadMUS("sound/Music.mp3");
    
    gScratch = Mix_LoadWAV("sound/Music.mp3");        

    return success;
}

void Reset()
{
    Draw_background();
    SDL_RenderCopy(ren, texture, NULL, NULL); //dan texture len voi do dai, do rong: NULL, NULL: full dai, full rong
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 12; j++)         table[i][j] = 0;
    }
    result = 0;
    player = 0;
}

//Three void below are the back-end that have function play the game
void win()
{
    result = true;
    Draw_PlayAgain();

    if (player % 2 == 1)         cout << "Player 1 win" << endl;
    else                         cout << "Player 2 win" << endl;
}
//Neu win -> In ra xem thang nao thang

void process()
{
    int count1 = 1, count2 = 1, count3 = 1, count4 = 1;             //ngang - doc -  cheo1  -  cheo2

    //ngang
    bool left = true, right = true;
    for (int i = 1; i <= 4 && (left || right); i++)
    {
        if (left)
        {
            if (table[x - i][y] == table[x][y])        count1 += 1;
            else          left = false;
        }
        if (right)
        {
            if (table[x + i][y] == table[x][y])        count1 += 1;
            else          right = false;
        }
    }
    if (count1 == 5)     win();

    left = true, right = true;
    for (int i = 1; i <= 4 && (left || right); i++)
    {
        if (left)
        {
            if (table[x][y-i] == table[x][y])        count2 += 1;
            else          left = false;
        }
        if (right)
        {
            if (table[x][y+i] == table[x][y])        count2 += 1;
            else          right = false;
        }
    }
    if (count2 == 5)     win();

    left = true, right = true;
    for (int i = 1; i <= 4 && (left || right); i++)
    {
        if (left)
        {
            if (table[x-i][y-i] == table[x][y])        count3 += 1;
            else          left = false;
        }
        if (right)
        {
            if (table[x+i][y + i] == table[x][y])        count3 += 1;
            else          right = false;
        }
    }
    if (count3 == 5)     win();

    left = true, right = true;
    for (int i = 1; i <= 4 && (left || right); i++)
    {
        if (left)
        {
            if (table[x - i][y + i] == table[x][y])        count4 += 1;
            else          left = false;
        }
        if (right)
        {
            if (table[x + i][y - i] == table[x][y])        count4 += 1;
            else          right = false;
        }
    }
    if (count4 == 5)     win();
}
//Kiem tra xem da thang hay chhua (voi vi tri ma nguoi choi moi chon)

void play()
{
    //Qua trinh nhan tin hieu tu chuot -> hien thi (NOT PLAY)
    SDL_GetMouseState(&x, &y);
    SDL_Rect a;
    //Tao a.x
    a.x = (x / 50) * 50;
    x = a.x / 50;    //De su dung a.x vao ham play() ma ko can a.x la bien toan cuc
    //Tao a.y
    a.y = (y / 50) * 50;
    y = a.y / 50;

    a.w = 50;
    a.h = 50;

    if (table[x][y] != 0)        return;

    player += 1;
    if (player % 2 == 1)
    {
        image = imageX;
        table[x][y] = -1;
    }
    else
    {
        image = imageO;
        table[x][y] = 1;
    }
    SDL_RenderCopy(ren, image, NULL, &a);

    process();
}     
//Nhap chuot -> Doc ky tu tu chuot -> Danh dau o trong

bool new_game = true;

int main(int argc, char* args[])
{
    //reset or initialize everything
    Init();

    Reset();    //set up lai tu dau

    bool quit = false;
    while (!quit)
    {
        SDL_Event e;
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            //User requests quit == dau x thoat chuong trinh
            if (e.type == SDL_QUIT)     
            {
                quit = true;
            }
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (result == false)
                {
                    play();
                }
                else
                {
                    SDL_GetMouseState(&x, &y);
                    if (x >= 50 && x <= 150 && y >= 300 && y <= 400)
                    {
                        result = false;        //Choi tiep
                        Reset();
                    }
                    else if (x >= 450 && x <= 550 && y >= 300 && y <= 400)
                    {
                        quit = true;                  
                    }
                }
            }
            SDL_RenderPresent(ren);
            gScratch = NULL;
        }
    }
    return 0;
}