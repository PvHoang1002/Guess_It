#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int generateRandomNumber(int level);
int chooseLevel();
char getHostAnswer();
int selectNumber(int low, int high);
void playGuessIt();
int playAgain();

int main()
{
    srand((unsigned) time(0));

    while(true)
    {
        playGuessIt();
        int isPlayAgain = playAgain();
        if(isPlayAgain == 0)
        {
            break;
        }
        else
        {
            cout << "\nCung choi lai nao!!!" << endl << endl;
        }
    }

    cout << "Tam biet!" << endl;

    return 0;
}

int generateRandomNumber(int level)
{
    int randomNumber = (rand() + 1) % level;
    return randomNumber;
}

int chooseLevel()
{
    int level;
    cout << "So cua ban nam trong khoang nao?" << endl;
    cout << "10 --- 100 --- 1000: ";
    cin >> level;
    cout << endl;
    return level;
}

char getHostAnswer()
{
    cout << "So toi doan co chinh xac khong? Hay cho toi goi y(<, >, =): ";

    char answer;
    do
    {
        cin >> answer;
        if(answer != '<' && answer != '>' && answer != '=')
        {
            cout << "Xin loi toi khong hieu goi y cua ban!" << endl;
            cout << "Hay cho toi goi y(<, >, =): ";
        }
    } while (answer != '<' && answer != '>' && answer != '=');

    cout << endl;

    return answer;
}

int selectNumber(int low, int high)
{
    return (low + high) / 2;
}

void playGuessIt()
{
    int level = chooseLevel();
    int guessNumber = generateRandomNumber(level);

    int low = 1;
    int high = level;

    cout << "Toi da san sang!!! Ban hay doan mot con so!" << endl << endl;
    cout << "So cua ban co phai.... " << guessNumber << endl;
    char hostAnswer = getHostAnswer();
    if(hostAnswer == '=')
    {
        cout << "Nhan pham cua toi that tuyet!!!" << endl;
    }
    else
    {
        if (hostAnswer == '>') high = guessNumber - 1;
        if (hostAnswer == '<') low = guessNumber + 1;
        do {
            guessNumber = selectNumber(low, high);
            cout << "Toi doan so.... " << guessNumber << endl;
            hostAnswer = getHostAnswer();
            if (hostAnswer == '>') high = guessNumber - 1;
            if (hostAnswer == '<') low = guessNumber + 1;
        } while (hostAnswer != '=');
    }
}

int playAgain()
{
    cout << "\nVui qua! Ban co muon choi lai khong?" << endl;

    int answer;
    do
    {
        cout << "Nhan 1 de tiep tuc / Nhan 0 de thoat" << endl;
        cin >> answer;
        if(answer != 0 && answer != 1)
        {
            cout << "\nXin loi! Toi khong hieu yeu cau cua ban!" << endl;
        }
    } while (answer != 0 && answer != 1);

    return answer;
}
