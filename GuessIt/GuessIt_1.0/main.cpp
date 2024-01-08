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
    MAIN_LOOP,
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
        case GameStates::MAIN_LOOP:
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

    LTexture& operator=(LTexture&& other) noexcept
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
int compare(int randomNumber, int guess)
{
    if (randomNumber < guess)
    {
        return -1;
    }
    if (randomNumber > guess)
    {
        return 1;
    }

    return 0;
}

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
            game.numberofGuesses = 5;
            game.max = 10;
            game.randomNumber = getRandomNumber(game.min, game.max);
            gameState = GameStates::MAIN_LOOP;
            //used to make sure game works properly
            std::cout << game.randomNumber << std::endl;
            return false;
        }
        else if (button_is_pressed(e, hundredButton.mButton))
        {
            game.numberofGuesses = 7;
            game.max = 100;
            game.randomNumber = getRandomNumber(game.min, game.max);
            gameState = GameStates::MAIN_LOOP;
            std::cout << game.randomNumber << std::endl;
            return false;
        }
        else if (button_is_pressed(e, thousandButton.mButton))
        {
            game.numberofGuesses = 9;
            game.max = 1000;
            game.randomNumber = getRandomNumber(game.min, game.max);
            gameState = GameStates::MAIN_LOOP;
            std::cout << game.randomNumber << std::endl;
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
bool gameLoop_input(GameStates& gameState, std::string& guessInput, int& guess, int& guessCount)
{
    SDL_Event e;

    SDL_StartTextInput();

    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            gameState = GameStates::EXIT;
            return false;
        }
        if (e.type == SDL_TEXTINPUT)
        {
            //if input is a numeric value, add to string.
            if (e.text.text[0] == '0' || e.text.text[0] == '1' || e.text.text[0] == '2' || e.text.text[0] == '3' || e.text.text[0] == '4' ||
                e.text.text[0] == '5' || e.text.text[0] == '6' || e.text.text[0] == '7' || e.text.text[0] == '8' || e.text.text[0] == '9')
            {
                guessInput += e.text.text;
            }
        }
        if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_KP_ENTER)
            {
                //if input is not empty
                if (guessInput != " ")
                {
                    //convert guess to int
                    guess = stoi(guessInput);
                    //reset string
                    guessInput = " ";
                    ++guessCount;
                }
            }
            else if (e.key.keysym.sym == SDLK_BACKSPACE && guessInput.length() > 0)
            {
                guessInput.pop_back();
            }
        }
    }

    SDL_StopTextInput();
    return true;
}

bool gameLoop_update(GameStates& gameState, std::string& dialogue, int guess, int guessCount, Game& game)
{
    if (guessCount == 0)
    {
        dialogue = "I'm thinking of a number between " + std::to_string(game.min) + " and " + std::to_string(game.max) + ". You have " + std::to_string(game.numberofGuesses) + " guesses.";
    }

    // if player ran out of moves
    else if (guessCount == game.numberofGuesses)
    {
        game.game_was_won = false;
        gameState = GameStates::PLAY_AGAIN;
        return false;
    }
    else
    {
        int cmp = compare(game.randomNumber, guess);

        //change dialogue based on comparison
        if (cmp > 0)
        {
            dialogue = "Your guess was too low.";
        }
        else if (cmp < 0)
        {
            dialogue = "Your guess was too high.";
        }
        else
        {
            game.game_was_won = true;
            gameState = GameStates::PLAY_AGAIN;
            return false;
        }
    }

    return true;
}

bool gameLoop_render(LTexture& computerTexture, LTexture& guessPrompt, std::string guessInput, std::string dialogue, SDL_Color textColor, int textWidth, int guessCount, Game game)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    computerTexture.render(gRenderer, 50, 40);
    guessPrompt.render(gRenderer, 250, 300);

    auto bubbleText = LTexture(from_text, gRenderer, dialogue, textColor, textWidth);
    bubbleText.render(gRenderer, 335, 70);

    auto guessCounter = LTexture(from_text, gRenderer, "Guesses remaining: " + std::to_string(game.numberofGuesses - guessCount), textColor, textWidth);
    guessCounter.render(gRenderer, 350, 200);

    auto inputTexture = LTexture(from_text, gRenderer, guessInput, textColor, textWidth);
    if (guessInput == "")
    {
        inputTexture = LTexture(from_text, gRenderer, " ", textColor, textWidth);
    }
    inputTexture.render(gRenderer, 250 + guessPrompt.mWidth, 300);

    SDL_RenderPresent(gRenderer);

    return true;
}

void gameLoop(GameStates& gameState, Game& game)
{
    std::string guessInput = " ";
    std::string dialogue = " ";

    SDL_Color const textColor = { 0,0,0 };
    auto const textWidth = 250;

    auto computerTexture = LTexture(from_surface, gRenderer, "Resources/Computer.png");
    auto guessPrompt = LTexture(from_text, gRenderer, "Enter a number:", textColor, textWidth);

    int guessCount = 0;
    int guess = 0;
    while (gameLoop_input(gameState, guessInput, guess, guessCount) &&
        gameLoop_update(gameState, dialogue, guess, guessCount, game) &&
        gameLoop_render(computerTexture, guessPrompt, guessInput, dialogue, textColor, textWidth, guessCount, game))
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

bool playAgain_render(LTexture& winLoseTexture, LTexture& winlose, LTexture& playAgain, LTexture& yesButton, LTexture& noButton)
{
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer);

    winlose.render(gRenderer, 150, 70);
    winLoseTexture.render(gRenderer, SCREEN_WIDTH / 2 - 105, 140);
    playAgain.render(gRenderer, SCREEN_WIDTH / 2 - 45, 400);
    yesButton.render(gRenderer, 80, 450);
    noButton.render(gRenderer, 80 + yesButton.mWidth + 10, 450);

    SDL_RenderPresent(gRenderer);

    return true;
}

void playAgain(GameStates& gameState, Game game)
{
    SDL_Color const textColor = { 0,0,0 };
    auto const textWidth = 460;

    auto playAgain = LTexture(from_text, gRenderer, "Play Again?", textColor, textWidth);
    auto yesButton = LTexture{ from_surface, gRenderer, "Resources/YesButton.png" };
    auto noButton = LTexture{ from_surface, gRenderer, "Resources/NoButton.png" };
    auto winLoseTexture = LTexture(from_surface, gRenderer, "Resources/Win.png");

    std::string dialogue;
    //change dialogue based on if player won or not
    if (game.game_was_won)
    {
        dialogue = "YOU WON!!! The correct answer was " + std::to_string(game.randomNumber) + ".";
    }
    else
    {
        dialogue = "YOU LOSE!!! The correct answer was " + std::to_string(game.randomNumber) + ".";
        winLoseTexture = LTexture(from_surface, gRenderer, "Resources/Lose.png");
    }

    auto winlose = LTexture{ from_text, gRenderer, dialogue, textColor, textWidth };

    while (playAgain_input(gameState, yesButton, noButton) && playAgain_update() && playAgain_render(winLoseTexture, winlose, playAgain, yesButton, noButton))
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





