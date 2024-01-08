#pragma once

struct Game
{
    int numberofGuesses = 0;
    const int min = 1;
    int max = 0;
    int randomNumber = 0;
    bool game_was_won = false;
};
