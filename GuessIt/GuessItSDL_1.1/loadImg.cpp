#include <loadImg.hpp>
#include <Game.hpp>

void load::loadImg()
{
    gameImg = loadTexture("images/game.bmp", renderer);
    startMsg = loadTexture("images/start.bmp", renderer);
    computerMsg = loadTexture("images/computer.bmp", renderer);
    humanMsg = loadTexture("images/human.bmp", renderer);
    bigMsg = loadTexture("images/big.bmp", renderer);
    smallMsg = loadTexture("images/small.bmp", renderer);
    sameMsg = loadTexture("images/same.bmp", renderer);
    bigMark = loadTexture("images/bigmark.bmp", renderer);
    smallMark = loadTexture("images/smallmark.bmp", renderer);
    sameMark = loadTexture("images/samemark.bmp", renderer);

    bool is_load_num_failed = false;
    for (int i=1; i<=MAX_NUM; i++)
    {
        markedNumbers[i] = MAX_NUM;
        numMsg[i] = loadTexture("images/" +to_string(i) + ".bmp", renderer);
        if (numMsg[i] == nullptr)
        {
            is_load_num_failed = true;
            break;
        }
    }
}

void printStartMsg()
{
    renderTexture(startMsg, renderer, 555, 132);
    render();
}
