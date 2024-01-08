#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL.h>

#include "Game.hpp"
#include "MathHelper.hpp"
#include "loadImg.hpp"

using namespace std;

//******************/
const int MAX_NUM = 100;
const int TOO_BIG = 1, TOO_SMALL=-1, THE_SAME = 0;
int generateRandomNumber();
void displayStartGame();
int getPlayerGuess();
void displayPlayerGuess(int number);
void updateAnswer(int &answer, int markedNumbers[], int number, int randomNumber);
void displayAnswer(int answer, int number);

//******************/
const string WINDOW_TITLE = "GuessIt by SDL2";
const int SCREEN_WIDTH = 861;// 800;
const int SCREEN_HEIGHT = 517; //600;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture *gameImg, *startMsg, *computerMsg, *humanMsg;
SDL_Texture *bigMsg, *smallMsg, *sameMsg;
SDL_Texture *bigMark, *smallMark, *sameMark;
SDL_Texture *numMsg[MAX_NUM+1];
int markedNumbers[MAX_NUM+1];

void load_SDL_and_Images();
void unload_SDL_and_Images();
void showGame();
void pause();
void printStartMsg();
void printComputerMsg();
void printComputerComparision(SDL_Texture *tex);
void printHumanMsg();
void printHumanNumber(SDL_Texture *tex);
void markNumber(int i);

//************ SDL functions *************/
void logSDLError(std::ostream& os, const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

/*********************************************/
int main(int argc, char* argv[])
{
    //game = new Game();
	//game->init("GameWindow", 800, 600, false);

    load_SDL_and_Images();

    srand(time(0));
    int randomNumber = generateRandomNumber();
    displayStartGame();

    int number, answer;
    do {
        number = getPlayerGuess();
        displayPlayerGuess(number);
        updateAnswer(answer, markedNumbers, number, randomNumber);
        displayAnswer(answer, number);
    } while (answer != THE_SAME);

    waitUntilKeyPressed();
    unload_SDL_and_Images();
    return 0;
}

int generateRandomNumber()
{
    return rand() % MAX_NUM + 1;
    //return 10;
}
void displayStartGame() {
    showGame();
    pause();
    printStartMsg();
}

int computeNumber(int x, int y)
{
    int tableX0 = 29, tableY0=285, tableX1=829, tableY1= 493;
    if (x<tableX0 || x>tableX1 || y<tableY0 || y>tableY1) return -1;
    int col = (x-tableX0)*20/(tableX1-tableX0), row=(y-tableY0)*5/(tableY1-tableY0);
    return (row*20 + col + 1);
}
int getPlayerGuess()
{
    int number=-1;
    SDL_Event e;
    while (number<=0 || number>MAX_NUM) {
        SDL_Delay(10);
        if ( SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            unload_SDL_and_Images();
            exit(1);
        }

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            number = computeNumber(e.button.x, e.button.y);
        }
    }
    return number;
}

void displayPlayerGuess(int number)
{
    showGame();
    printHumanMsg();
    pause();
    showGame();
    printHumanNumber(numMsg[number]);
    pause();
}

void updateAnswer(int &answer, int markedNumbers[], int number, int randomNumber)
{
    if (number > randomNumber) {
        answer = TOO_BIG;
    } else if (number < randomNumber) {
        answer = TOO_SMALL;
    } else {
        answer =  THE_SAME;
    }
    markedNumbers[number] = answer;
}

void displayAnswer(int answer, int number)
{
    printComputerMsg();
    pause();
    markNumber(number);

    SDL_Texture * tex;
    if (answer == TOO_BIG) {
        tex = bigMsg;
    } else if (answer == TOO_SMALL) {
        tex = smallMsg;
    } else {//THE_SAME
        tex = sameMsg;
    }
    printComputerComparision(tex);
}

//****************************************************
void unload_SDL_and_Images()
{
    SDL_DestroyTexture(gameImg);
    SDL_DestroyTexture(startMsg);
    SDL_DestroyTexture(computerMsg);
    SDL_DestroyTexture(humanMsg);
    SDL_DestroyTexture(bigMsg);
    SDL_DestroyTexture(smallMsg);
    SDL_DestroyTexture(sameMsg);
    SDL_DestroyTexture(bigMark);
    SDL_DestroyTexture(smallMark);
    SDL_DestroyTexture(sameMark);
    for (int i=1; i<=MAX_NUM; i++) {
        SDL_DestroyTexture(numMsg[i]);
    }
    quitSDL(window, renderer);
}

void load_SDL_and_Images()
{
    initSDL(window, renderer);

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
    for (int i=1; i<=MAX_NUM; i++) {
        markedNumbers[i] = MAX_NUM;
        numMsg[i] = loadTexture("images/" +to_string(i) + ".bmp", renderer);
        if (numMsg[i] == nullptr) {
            is_load_num_failed = true;
            break;
        }
    }

    if (gameImg == nullptr  || startMsg == nullptr || computerMsg == nullptr
        || humanMsg == nullptr  || bigMsg == nullptr || smallMsg == nullptr
        || sameMsg == nullptr || is_load_num_failed){
        unload_SDL_and_Images();
        exit(1);
    }
}

void markNumber(int i)
{
    int tableX0 = 29, tableY0=285, tableX1=829, tableY1= 493;
    int cellWidth = (tableX1-tableX0)/20, cellHeight = (tableY1-tableY0)/5;
    int row = (i-1)/20, col=(i-1)%20;
    int x = tableX0 + col*cellWidth + 2, y = tableY0 + row*cellHeight + 5;
    if (markedNumbers[i]==TOO_BIG){
        renderTexture(bigMark, renderer, x, y);
    } else if (markedNumbers[i]==TOO_SMALL){
        renderTexture(smallMark, renderer, x, y);
    } else if (markedNumbers[i]==THE_SAME){
        renderTexture(sameMark, renderer, x, y);
    }
}

void showGame()
{
    SDL_RenderClear(renderer);
    renderTexture(gameImg, renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    for (int i=1; i<=MAX_NUM; i++) {
       markNumber(i);
    }
    SDL_RenderPresent(renderer);
}

void pause()
{
    SDL_Delay(1500);
    //SDL_Delay(100);
}

void printStartMsg()
{
    renderTexture(startMsg, renderer, 555, 132);
    SDL_RenderPresent(renderer);
}

void printComputerMsg()
{
    renderTexture(computerMsg, renderer, 552, 130);
    SDL_RenderPresent(renderer);
}


void printComputerComparision(SDL_Texture *tex)
{
    renderTexture(tex, renderer, 550, 147);
    SDL_RenderPresent(renderer);
}

void printHumanMsg()
{
    renderTexture(humanMsg, renderer, 296, 107);
    SDL_RenderPresent(renderer);
}

void printHumanNumber(SDL_Texture *tex)
{
    renderTexture(tex, renderer, 315, 102);
    SDL_RenderPresent(renderer);
}

//*****************************************************
// Các hàm chung về khởi tạo và huỷ SDL
void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
    //   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);


    //Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

/**
* Nạp một ảnh bitmap (BMP) vào một texture trên thiết bị hiện thị (rendering device)
* @param file: đường dẫn và tên file ảnh BMP
* @param ren: renderer để nạp texture lên
* @return trả về texture đã nạp, hoặc nullptr nếu có lỗi.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
	//Khởi tạo là nullptr để tránh lỗi 'dangling pointer'
	SDL_Texture *texture = nullptr;
	//Nạp ảnh từ tên file (với đường dẫn)
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//Nếu không có lỗi, chuyển đổi về dạng texture and và trả về
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(ren, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Đảm bảo việc chuyển đổi không có lỗi
		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

/**
* Vẽ một SDL_Texture lên một SDL_Renderer tại toạ độ (x, y), trong khi
* giữ nguyên chiều rộng và cao của ảnh
* @param tex: texture nguồn chúng ta muốn vẽ ra
* @param ren: thiết bị renderer chúng ta muốn vẽ vào
* @param x: hoành độ
* @param y: tung độ
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Truy vẫn texture để lấy chiều rộng và cao (vào chiều rộng và cao tương ứng của hình chữ nhật đích)
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Vẽ một SDL_Texture lên một SDL_Renderer tại toạ độ (x, y), với
* chiều rộng và cao mới
* @param tex: texture nguồn chúng ta muốn vẽ ra
* @param ren: thiết bị renderer chúng ta muốn vẽ vào
* @param x: hoành độ
* @param y: tung độ
* @param w: chiều rộng (mới)
* @param h: độ cao (mới)
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
	//Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
    dst.w = w;
    dst.h = h;
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
    //(ảnh sẽ co dãn cho khớp với kích cỡ mới)
	SDL_RenderCopy(ren, tex, NULL, &dst);
}
//**************************************************************

