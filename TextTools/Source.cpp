#include <Windows.h>
#include "TextDraw.h"
#include "MyMouse.h"
#include <array>
#include <iostream>
#include<random>
#include <string>
#include "MySound.h"

using namespace std;

//creating a class for Memorycard Game
class MemoryGame {

public:
    int clicknumber;
    int numberoffirstcardclicked;
    int numberofsecondcardclicked;

    std::array<int, 12> cardValues;
    std::array<int, 12> deadCards;

 //functions part of the class
public:

    MemoryGame();

	void LoadCard(string filename, BoardData card[50][50]);

	void DisplayCard(int x, int y, int rows, int columns, BoardData card[50][50]);

	void DisplaySpecificCard(int cardnumber, BoardData oneCard[50][50]);

	int GetCardNumber(int x, int y);
};

 
//memorygame constructor for arrays
MemoryGame::MemoryGame() {

    cardValues = { 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6 };

    deadCards = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    //shuffle funciton to randomize the numbers every game
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cardValues.begin(), cardValues.end(), g);
}

//defining the functions from our class to load our cards
void MemoryGame::LoadCard(string filename, BoardData card[50][50])
{
    int rows, columns;
    ifstream input(filename);
    if (!input)
        return;
    input >> rows >> columns;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
            input >> card[i][j].symbol >> card[i][j].color >> card[i][j].bgcolor;
    }
    input.close();
}
//function definition that is part of our class, to display our cards 
void MemoryGame::DisplayCard(int x, int y, int rows, int columns, BoardData card[50][50])
{
    Coordinates C;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            C.gotoxy(j + x, i + y);
            C.setForeGroundAndBackGroundColor(card[i][j].color, card[i][j].bgcolor);
            cout << char(card[i][j].symbol);
        }
    }
}
//function definition part of our class, display the specific card clicked on
void MemoryGame::DisplaySpecificCard(int cardnumber, BoardData oneCard[50][50])
{
    int x, y;
    if (cardnumber == 1){
        x = 15; y = 1;
    }
    else if (cardnumber == 2){
        x = 24; y = 1;
    }
    else if (cardnumber == 3){
        x = 15; y = 11;
    }
    else if (cardnumber == 4){
        x = 24; y = 11;
    }
    else if (cardnumber == 5){
        x = 33; y = 1;
    }
    else if (cardnumber == 6){
        x = 42; y = 1;
    }
    else if (cardnumber == 7){
        x = 33; y = 11;
    }
    else if (cardnumber == 8){
        x = 42; y = 11;
    }
    else if (cardnumber == 9) {
        x = 15; y = 21;
    }
    else if (cardnumber == 10) {
        x = 24; y = 21;
    }
    else if (cardnumber == 11) {
        x = 33; y = 21;
    }
    else if (cardnumber == 12) {
        x = 42; y = 21;
    }


    DisplayCard(x, y, 10, 9, oneCard);
}
//function definition part of our class, return card number
int MemoryGame:: GetCardNumber(int x, int y)
{
    if ((x >= 15 && x <= 22) && (y >= 1 && y <= 9))
        return 1;
    else if ((x >= 24 && x <= 31) && (y >= 1 && y <= 9))
        return 2;
    else if ((x >= 15 && x <= 22) && (y >= 11 && y <= 19))
        return 3;
    else if ((x >= 24 && x <= 31) && (y >= 11 && y <= 19))
        return 4;
    else if ((x >= 33 && x <= 40) && (y >= 1 && y <= 9))
        return 5;
    else if ((x >= 42 && x <= 49) && (y >= 1 && y <= 9))
        return 6;
    else if ((x >= 33 && x <= 40) && (y >= 11 && y <= 19))
        return 7;
    else if ((x >= 42 && x <= 49) && (y >= 11 && y <= 19))
        return 8;
    else if ((x >= 15 && x <= 22) && (y >= 21 && y <= 29))
        return 9;
    else if ((x >= 24 && x <= 31) && (y >= 21 && y <= 29))
        return 10;
    else if ((x >= 33 && x <= 40) && (y >= 21 && y <= 29))
        return 11;
    else if ((x >= 42 && x <= 49) && (y >= 21 && y <= 29))
        return 12;
    else
        return 0;
} 

void main() {

    BoardData card[7][50][50];
    BoardData empty[50][50];

    MySound mySong;
    MySound ring;

    system("cls");
    //creating an object from class MyMouse
    MyMouse M;
    //object from out MemoryGame class
    MemoryGame game;
    
    //assigning a value to clicknumber
    game.clicknumber = 0;
    int counter = 0;

    mySong.Play(L"files/FREE.wav", NULL,SND_ASYNC);
   
    game.LoadCard("files/0.txt", card[0]);
    game.LoadCard("files/card_2.txt", card[1]);
    game.LoadCard("files/card_3.txt", card[2]);
    game.LoadCard("files/card_4.txt", card[3]);
    game.LoadCard("files/cardA.txt", card[4]);
    game.LoadCard("files/cardK.txt", card[5]);
    game.LoadCard("files/cardQ.txt", card[6]);

    game.LoadCard("files/empty.txt", empty);

    Sleep(1000);
    //display the back of our cards to start the game
    for (int j = 1; j <= 12; j++)
        game.DisplaySpecificCard(j, card[0]);
    
    while (1)
    {

        M.ReadMouseInput();

        switch (M.InputRecord.EventType)
        {
            //song to play in the background
            mySong.Play(L"files/FREE.wav", NULL, SND_ASYNC);
        case MOUSE_EVENT: // mouse input 
            if (M.InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                int x = M.InputRecord.Event.MouseEvent.dwMousePosition.X;
                int y = M.InputRecord.Event.MouseEvent.dwMousePosition.Y;

                int cardClicked = game.GetCardNumber(x, y);
                if (cardClicked == 0) {
                    break;
                }
                if (game.deadCards[cardClicked - 1] == 0)
                {
                        game.DisplaySpecificCard(cardClicked, card[game.cardValues[cardClicked - 1]]);

                    if (game.clicknumber == 0)
                    {
                        game.numberoffirstcardclicked = cardClicked;
                        game.clicknumber = 1;
                    }
                    else
                    {
                        game.clicknumber = 0;
                        game.numberofsecondcardclicked = cardClicked;
                        if ((game.cardValues[game.numberoffirstcardclicked - 1] ==
                            game.cardValues[game.numberofsecondcardclicked - 1]))
                        {
                            Sleep(500);
                            game.DisplaySpecificCard(game.numberoffirstcardclicked, empty);
                            game.DisplaySpecificCard(game.numberofsecondcardclicked, empty);
                            game.deadCards[game.numberoffirstcardclicked - 1] = 1;
                            game.deadCards[game.numberofsecondcardclicked - 1] = 1;
                        }
                        else
                        {
                            Sleep(500);
                            game.DisplaySpecificCard(game.numberoffirstcardclicked, card[0]);
                            game.DisplaySpecificCard(game.numberofsecondcardclicked, card[0]);
                        }
                    }
                }
            }
            break;
        }
    } //cout< "Number of tries" << counter;
};