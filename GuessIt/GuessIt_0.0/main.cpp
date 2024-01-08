#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int generateRandomNumber();
int getPlayerNumber();
string printResult(int randomNumber, int guessNumber);
void playGuessIt();
int askPlayerToContinue();

int main() {
    srand((unsigned) time(0));

    while(true) {
        playGuessIt();
        int playerChoice = askPlayerToContinue();
        if(playerChoice == 0) {
            break;
        }
        cout << endl;
    }
    return 0;
}

int generateRandomNumber(int level) {
    int randomNumber = (rand() + 1) % level;
    return randomNumber;
}

int getPlayerNumber() {
    int playerNumber;
    cout << "Nhap so ban doan: ";
    cin >> playerNumber;
    return playerNumber;
}

string printResult(int randomNumber, int guessNumber) {
    string result;
    if(guessNumber < randomNumber) {
        result = "So cua ban qua nho! Thu lai nhe!\n\n";
    }else if(guessNumber > randomNumber) {
        result = "So cua ban qua lon! Thu lai nhe!\n\n";
    }else if(guessNumber == randomNumber) {
        result = "Chuc mung ban da doan dung!\n\n";
    }
    return result;
}

int chooseLevel()
{
    int level;
    cout << "Ban muon doan trong khoang nao?" << endl;
    cout << "10 --- 100 --- 1000: ";
    cin >> level;
    cout << endl;
    return level;
}

void playGuessIt() {
    int count = 0;
    int level = chooseLevel();
    int randomNumber = generateRandomNumber(level);

    while (true) {
        int guessNumber = getPlayerNumber();
        cout << printResult(randomNumber, guessNumber);
        count++;
        if(guessNumber == randomNumber) {
            break;
        }
    }
    cout << "\nBan da doan " << count << " lan!" << endl;
    cout << "Ket thuc tro choi!" << endl;
}

int askPlayerToContinue() {
    int choice;

    cout << "\nBan co muon choi lai khong? " << endl;

    do
    {
        cout << "Nhan 1 de tiep tuc / Nhan 0 de thoat" << endl;
        cin >> choice;
        if(choice != 0 && choice != 1)
        {
            cout << "\nXin loi! Toi khong hieu yeu cau cua ban!" << endl;
        }
    } while(choice != 0 && choice != 1);

    return choice;
}
