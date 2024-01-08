#include "MathHelper.hpp"
#include "Game.hpp"
#include "loadImg.hpp"

const int tableX0 = 29, tableY0 = 285, tableX1 = 829, tableY1 = 493;
const int TOO_BIG = 1, TOO_SMALL= -1, THE_SAME = 0;
const int MAX_NUM = 100;
int markedNumbers[MAX_NUM+1];

void Math::generateRandomNumber()
{
    return rand() % MAX_NUM + 1;
}

int Math::computeNumber(int x, int y)
{
    //int tableX0 = 29, tableY0=285, tableX1=829, tableY1= 493;

    if (x<tableX0 || x>tableX1 || y<tableY0 || y>tableY1)
    {
        return -1;
    }

    int col = (x-tableX0)*20/(tableX1-tableX0), row=(y-tableY0)*5/(tableY1-tableY0);

    return (row*20 + col + 1);
}

void Math::markNumber(int chosenNumber)
{
    //int tableX0 = 29, tableY0=285, tableX1=829, tableY1= 493;

    int cellWidth = (tableX1-tableX0)/20, cellHeight = (tableY1-tableY0)/5;

    int row = (chosenNumber - 1) / 20, col = (chosenNumber - 1) % 20;

    int x = tableX0 + col*cellWidth + 2, y = tableY0 + row*cellHeight + 5;

    /*
    if (markedNumbers[chosenNumber] == TOO_BIG)
    {
        renderTexture(bigMark, renderer, x, y);
    }
    else if (markedNumbers[chosenNumber] == TOO_SMALL)
    {
        renderTexture(smallMark, renderer, x, y);
    }
    else if (markedNumbers[chosenNumber] == THE_SAME)
    {
        renderTexture(sameMark, renderer, x, y);
    }
    */
}
