#include <iostream>
#include <random>
#include <string>
#include <cstdlib>
#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SDLInit.hpp"
#include "Game.hpp"

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 700;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
TTF_Font* gFont = nullptr;

//Use to tag how the texture was rendered
enum TextureType { from_surface, from_text };

enum class GameStates
{
    MAIN_MENU,
    GAME_LOOP,
    PLAY_AGAIN,
    EXIT,
};

void mainMenu(GameStates& gameState, Game& game);
void gameLoop(GameStates& gameState, Game& game);
void playAgain(GameStates& gameState, Game game);
void init();
void close();

int main(int argc, char* args[])
{
    init();

    GameStates gameState = GameStates::MAIN_MENU;
    Game game;

    while (gameState != GameStates::EXIT)
    {
        switch (gameState)
        {
        case GameStates::MAIN_MENU:
            mainMenu(gameState, game);
            break;
        case GameStates::GAME_LOOP:
            gameLoop(gameState, game);
            break;
        case GameStates::PLAY_AGAIN:
            playAgain(gameState, game);
            break;
        }
    }

    close();

    return 0;
}

class LTexture
{
public:
    LTexture(TextureType type, SDL_Renderer* renderer, std::string const& path);
    LTexture(TextureType type, SDL_Renderer* renderer, std::string const& text, SDL_Color color, int width);

    LTexture(LTexture&& other) noexcept
    {
        swap(*this, other);
    }

    ~LTexture();

    void render(SDL_Renderer* renderer, int x, int y);

    LTexture& operator = (LTexture&& other) noexcept
    {
        swap(*this, other);
        return *this;
    }

    SDL_Texture* mTexture = nullptr;
    int mWidth = 0;
    int mHeight = 0;
    SDL_Rect mButton = {};

    LTexture(LTexture const&) = delete;
    LTexture& operator=(LTexture const&) = delete;

    friend void swap(LTexture& a, LTexture& b) noexcept;
};

LTexture::LTexture(TextureType type, SDL_Renderer* renderer, std::string const& path)
{
    SDL_Surface *surface = IMG_Load(path.c_str());
    if (!surface)
        std::cout << "Failed to create surface";

    mTexture = SDL_CreateTextureFromSurface(gRenderer, surface);
    if (!mTexture)
        std::cout << "Failed to create texture";
    mWidth = surface->w;
    mHeight = surface->h;
}

LTexture::LTexture(TextureType type, SDL_Renderer* renderer, std::string const& text, SDL_Color color, int width)
{
    SDL_Surface* textSurface = TTF_RenderText_Blended_Wrapped(gFont, text.c_str(), color, width);
    if (!textSurface)
        std::cout << "Failed to create surface";

    mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!mTexture)
        std::cout << "Failed to created texture";

    mWidth = textSurface->w;
    mHeight = textSurface->h;

    SDL_FreeSurface(textSurface);
}

void LTexture::render(SDL_Renderer* renderer, int x, int y)
{
    SDL_Rect destRect = { x, y, mWidth, mHeight };
    SDL_RenderCopy(renderer, mTexture, nullptr, &destRect);
    //create a rectangle that coincides with texture to check for button presses
    mButton = { x, y, mWidth, mHeight };
}

void swap(LTexture& a, LTexture& b) noexcept
{
    using std::swap;
    swap(a.mTexture, b.mTexture);
    swap(a.mWidth, b.mWidth);
    swap(a.mHeight, b.mHeight);
    swap(a.mButton, b.mButton);
}

LTexture::~LTexture()
{
    if (mTexture)
        SDL_DestroyTexture(mTexture);
}

//Math helper
int getRandomNumber(int x, int y)
{
    return rand() % y + x;
}
//*************************************************************

//Check button is pressed?
bool button_is_pressed(SDL_Event const& event, SDL_Rect const& button_rect)
{
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        auto const& mouse_button_event = event.button;

        auto const mouse_position = SDL_Point{ mouse_button_event.x, mouse_button_event.y };

        return (mouse_button_event.button == SDL_BUTTON_LEFT) && SDL_PointInRect(&mouse_position, &button_rect);
    }

    return false;
}
//*************************************************************

// Main Menu
bool mainMenu_input(GameStates& gameState, Game& game, LTexture& tenButton, LTexture& hundredButton, LTexture& thousandButton)
{
    SDL_Event e;
    srand(time(0));

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            gameState = GameStates::EXIT;
            return false;
        }
        //change game settings based on button
        if (button_is_pressed(e, tenButton.mButton))
        {
            game.max = 10;
            game.guessNumber = getRandomNumber(game.min, game.max);
            gameState = GameStates::GAME_LOOP;
            return false;
        }
        else if (button_is_pressed(e, hundredButton.mButton))
        {
            game.max = 100;
            game.guessNumber = getRandomNumber(game.min, game.max);
            gameState = GameStates::GAME_LOOP;
            return false;
        }
        else if (button_is_pressed(e, thousandButton.mButton))
        {
            game.max = 1000;
            game.guessNumber = getRandomNumber(game.min, game.max);
            gameState = GameStates::GAME_LOOP;
            return false;
        }
    }
    return true;
}

bool mainMenu_update()
{
    return true;
}

bool mainMenu_render(LTexture& announce, LTexture& tenButton, LTexture& hundredButton, LTexture& thousandButton)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    announce.render(gRenderer, 255, 30);
    tenButton.render(gRenderer, (SCREEN_WIDTH / 2) - (tenButton.mWidth / 2), 70);
    hundredButton.render(gRenderer, (SCREEN_WIDTH / 2) - (tenButton.mWidth / 2), 270);
    thousandButton.render(gRenderer, (SCREEN_WIDTH / 2) - (tenButton.mWidth / 2), 470);

    SDL_RenderPresent(gRenderer);

    return true;
}

void mainMenu(GameStates& gameState, Game& game)
{
    SDL_Color const textColor = { 0,0,0 };
    auto const textWidth = 800;

    auto tenButton = LTexture(from_surface, gRenderer, "Resources/Level10.png");
    auto hundredButton = LTexture(from_surface, gRenderer, "Resources/Level100.png");
    auto thousandButton = LTexture(from_surface, gRenderer, "Resources/Level1000.png");
    auto announce = LTexture{ from_text, gRenderer, "Choose level!", textColor, textWidth };

    while (mainMenu_input(gameState, game, tenButton, hundredButton, thousandButton) &&
        mainMenu_update() &&
        mainMenu_render(announce, tenButton, hundredButton, thousandButton))
    {}
}
//*************************************************************

// Game Loop
bool gameLoop_input(GameStates& gameState, Game& game, LTexture const& smallerButton, LTexture const& biggerButton, LTexture const& equalButton, int& guessCount)
{
    SDL_Event e;


    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            gameState = GameStates::EXIT;
            return false;
        }
        if (button_is_pressed(e, smallerButton.mButton))
        {
            game.game_was_won = 0;
            game.min = game.guessNumber + 1;
            if (game.max < game.min)
            {
                gameState = GameStates::PLAY_AGAIN;
                game.game_was_won = -1;
                game.min = 1;
                game.guessNumber = 0;
                return false;
            }
            else
            {
                gameState = GameStates::GAME_LOOP;
            }
            ++guessCount;
            return true;
        }
        if (button_is_pressed(e, biggerButton.mButton))
        {
            game.game_was_won = 0;
            game.max = game.guessNumber - 1;
            if (game.max < game.min)
            {
                gameState = GameStates::PLAY_AGAIN;
                game.game_was_won = -1;
                game.min = 1;
                game.guessNumber = 0;
                return false;
            }
            else
            {
                gameState = GameStates::GAME_LOOP;
            }
            ++guessCount;
            return true;
        }
        if (button_is_pressed(e, equalButton.mButton))
        {
            game.game_was_won = 1;
            gameState = GameStates::PLAY_AGAIN;
            game.min = 1;
            game.guessNumber = 0;
            return false;
        }
    }

    return true;
}

bool gameLoop_update(GameStates& gameState, std::string& dialogue, std::string& answer, int guessCount, Game& game)
{
    srand(time(0));

    if (guessCount == 0)
    {
        dialogue = "I guess your number is... " + std::to_string(game.guessNumber);
    }
    else
    {
        game.guessNumber = (game.min + game.max) / 2;
        dialogue = "I guess your number is... " + std::to_string(game.guessNumber);
    }

    answer = "Give me your answer:";

    return true;
}

bool gameLoop_render(LTexture& computerTexture, LTexture& smallerButtonTexture, LTexture& biggerButtonTexture, LTexture& equalButtonTexture,
                     std::string dialogue, std::string answer, SDL_Color textColor, int textWidth, Game game)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    computerTexture.render(gRenderer, 70, 40);

    auto bubbleText = LTexture(from_text, gRenderer, dialogue, textColor, textWidth + 50);
    bubbleText.render(gRenderer, 320, 70);

    auto answerText = LTexture(from_text, gRenderer, answer, textColor, textWidth + 50);
    answerText.render(gRenderer, 235, 400);

    smallerButtonTexture.render(gRenderer, 100, 500);
    biggerButtonTexture.render(gRenderer, 100 + smallerButtonTexture.mWidth + 30, 500);
    equalButtonTexture.render(gRenderer, 100 + smallerButtonTexture.mWidth + biggerButtonTexture.mWidth + 60, 500);

    SDL_RenderPresent(gRenderer);

    return true;
}

void gameLoop(GameStates& gameState, Game& game)
{
    std::string dialogue = " ";
    std::string answer = " ";

    SDL_Color const textColor = { 0,0,0 };
    auto const textWidth = 250;

    auto computerTexture = LTexture(from_surface, gRenderer, "Resources/Computer.png");
    auto smallerButton = LTexture(from_surface, gRenderer, "Resources/SmallerButton.png");
    auto biggerButton = LTexture(from_surface, gRenderer, "Resources/BiggerButton.png");
    auto equalButton = LTexture(from_surface, gRenderer, "Resources/EqualButton.png");

    int guessCount = 0;

    while (gameLoop_input(gameState, game, smallerButton, biggerButton, equalButton, guessCount) &&
        gameLoop_update(gameState, dialogue, answer, guessCount, game) &&
        gameLoop_render(computerTexture, smallerButton, biggerButton, equalButton, dialogue, answer, textColor, textWidth, game))
    {}
}
//*************************************************************

//Play Again
bool playAgain_input(GameStates& gameState, LTexture const& yesButton, LTexture const& noButton)
{
    SDL_Event e;

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT || button_is_pressed(e, noButton.mButton))
        {
            gameState = GameStates::EXIT;
            return false;
        }

        if (button_is_pressed(e, yesButton.mButton))
        {
            gameState = GameStates::MAIN_MENU;
            return false;
        }
    }

    return true;
}

bool playAgain_update()
{
    return true;
}

bool playAgain_render(LTexture& winLoseTexture, LTexture& winlose, LTexture& yesButton, LTexture& noButton)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    winlose.render(gRenderer, 180, 70);
    winLoseTexture.render(gRenderer, SCREEN_WIDTH / 2 - 130, 140);
    yesButton.render(gRenderer, 80, 450);
    noButton.render(gRenderer, 80 + yesButton.mWidth + 10, 450);

    SDL_RenderPresent(gRenderer);

    return true;
}

void playAgain(GameStates& gameState, Game game)
{
    SDL_Color const textColor = { 0,0,0 };
    auto const textWidth = 460;

    auto yesButton = LTexture{ from_surface, gRenderer, "Resources/YesButton.png" };
    auto noButton = LTexture{ from_surface, gRenderer, "Resources/NoButton.png" };
    auto winLoseTexture = LTexture(from_surface, gRenderer, "Resources/Smile.png");

    std::string dialogue;
    //change dialogue based on if player won or not
    if (game.game_was_won == 1)
    {
        dialogue = "That was fun! Wanna play again?";
    }
    else if (game.game_was_won == -1)
    {
        dialogue = "You are cheating! Wanna play again?";
        winLoseTexture = LTexture(from_surface, gRenderer, "Resources/Angry.png");
    }

    auto winlose = LTexture{ from_text, gRenderer, dialogue, textColor, textWidth };

    while (playAgain_input(gameState, yesButton, noButton) && playAgain_update() && playAgain_render(winLoseTexture, winlose, yesButton, noButton))
    {}
}
//*************************************************************

// Init and Close
void init()
{
    auto sdlinit = sdl{};
    auto img = sdl_img{};
    auto ttf = sdl_ttf{};

    gWindow = SDL_CreateWindow("Guess It", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr)
    {
        std::cout << "Failed to create window";
    }
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == nullptr)
    {
        std::cout << "Failed to create renderer";
    }
    gFont = TTF_OpenFont("Resources/opensans.ttf", 20);
    if (gFont == nullptr)
    {
        std::cout << "Failed to open font";
    }
}

void close()
{
    if (gFont)
    {
        TTF_CloseFont(gFont);
    }
    if (gWindow)
    {
        SDL_DestroyWindow(gWindow);
    }
    if (gRenderer)
    {
        SDL_DestroyRenderer(gRenderer);
    }
}
//*************************************************************

