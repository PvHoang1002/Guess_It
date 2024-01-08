#pragma once

#include <iostream>
#include <SDL.h>

const int MAX_NUM = 100;

class load
{
public:
    void loadImg();
    void printStartMsg();
private:
    SDL_Texture *gameImg, *startMsg, *computerMsg, *humanMsg;
    SDL_Texture *bigMsg, *smallMsg, *sameMsg;
    SDL_Texture *bigMark, *smallMark, *sameMark;
    SDL_Texture *numMsg[MAX_NUM+1];
    int markedNumbers[MAX_NUM+1];
};
