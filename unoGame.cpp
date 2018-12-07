/*=============================================================================
 |   Assignment:  Final Project
 |
 |      Authors:  Jad Aboud Siriani and Ralph Ghannam
 |   To Compile:  Have all three unoGame.cpp, UnoDeck.txt, and player.h files
 |                  in the same directory and simply compile the cpp file
 |
 |        Class:  Object Oriented Programming
 |
 |   Instructor:  Aaron Carpenter
 |     Due Date:  12/7/2018
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  This project
 |      SOLVE.
 |
 |    Algorithm, Input, and Output:  See attached technical video
 |
 |      Sources:  http://www.cplusplus.com/
 |      Website has all the information with examples to applications
 |
 |   Known Bugs:  User input is a little cluncky. Make sure to use the following
 |                format:
 |                - color(character) <space> number(integer) <space> type(string)
 |                - for the color, use S if the card is a wildcard or plusFour
 |                - for the number, use 10 if it's a colored player card
 |                - for the type, enter the type as shown in the hand
 |
 *===========================================================================*/



//Libraries and functions used:

#include <iostream>     //cout; cin;
#include <fstream>      //ifstream;
#include <cmath>        //rand();
#include <list>         //list<card>; iterator; push_back(); erase(); back(); http://www.cplusplus.com/list
#include <vector>       //vector<card>; push_back(); pop_back(); back();      http://www.cplusplus.com/vector
#include <string>       //string
#include <algorithm>    //shuffle()                                           http://www.cplusplus.com/algorithm
#include "player.h"     //Player, User, AI1, AI2, AI3 classes

using namespace std;

void populateDeck(vector<card> &); //function that will populate the game's deck
void shuffleDeck(vector<card> &);  //fucntion that will shuffle the game's deck
void printIntroMenu();             //function that vill print the initial game menu game menu
void restockDeck(vector<card> &, vector<card> &); //function that restocks the deck using the graveyard
int myrandom (int);                //function used to shuffle
int playerIndex(int, int, int);    //function that changes the player index accordingly
int main(){

  //Gameplay Variables
  vector<card> deck;              //the game's deck
  vector<card> graveyard;         //the game's graveyard
  int turn = 0;                   //variable that controls the turn
  int turnIncrement = 1;          //variable that controls the turn's rotation (always 1 or -1)
  int turnIsOver = 0;             //variable that determines if the turn is turn
  card returnedCard;              //card returned by a player during the turn
  card onTable;                   //card that is currently on the table
  int isWinner = 0;               //variable that will determine if a player won the game

  //Game Setup Variables

  int numUsers = 1;     //variable that determines the number of human players
  //game setup is hardcoded for this version

  /****************************************************************************/
  //Game's intro menu

  printIntroMenu();

  //Game Setup

  populateDeck(deck);             //populate the game's deck
  shuffleDeck(deck);              //shuffle the game's deck

  cout << "Populated and Shuffled" << endl;

  //for this version, all unique players are included
  User user; AI1 ai1; AI2 ai2; AI3 ai3;
  player *players[4] = {&user, &ai1, &ai2, &ai3};

  /****************************************************************************/
  //Start of the game's main loop

  onTable = deck.back();             //puts the first card on the table
  graveyard.push_back(onTable);       //puts the card immediately in the graveyard, so the card's original version is preserved
  deck.pop_back();                   //removes the a card from the deck

  //deal the first set of cards
  for(int i = 0; i < 7; i++) {
    for(int j = 0; j < 4; j++) {
      players[j] -> addCard(deck.back());
      deck.pop_back();
    }
  }

  cout << "-------------------------------------" << "Let the Game Begin!" << "-------------------------------------" << endl;

  while(!isWinner){

    cout << players[turn] -> getName() << "'s turn!" << endl;
    cout << "The card on the table is " << onTable.color << onTable.number << ' ' << onTable.type << endl;
    //Player turn:
    cout << "-------------------------------------" << endl;
    while(!turnIsOver){
      players[turn] -> setAllowedCards(onTable); //the player takes in the card currently on the table and generates a list of allowed cards.
      returnedCard = players[turn] -> strategy(onTable); //the player executes his strategy
      if(returnedCard.type == "NAN"){  //if the player has a to play
        if(deck.empty()){             //if the deck is empty, refill it
          restockDeck(deck, graveyard);
        }
        players[turn] -> addCard(deck.back());
        deck.pop_back();
      }
      else{
        graveyard.push_back(returnedCard);
        onTable = returnedCard;
        turnIsOver++;
      }
    }
    turnIsOver = 0;                                  //reset the variable for the next

    //Determining how the next turn will go:
    if(onTable.color == 'S'){                         //if the card has no color
      onTable.color = players[turn] -> chooseColor(); //the player choses a color for the next player to play
      onTable.number = 11;                           //sets the number to 11, avoiding a bug where the next player can place cards that are special but have different colors
      if(onTable.type == "plusFour"){                 //if the card is a plus four
        cout << players[playerIndex(turn, turnIncrement, 0)] -> getName() << " Just received 4 extra cards and lost his turn!" << endl;
        for(int i = 0; i < 4; i++){                  //loop that adds 4 cards to the next player
          if(deck.empty()){                          //if the deck is empty, refill it
            restockDeck(deck, graveyard);
          }
          players[playerIndex(turn, turnIncrement, 0)] -> addCard(deck.back()); //give the next player 4 cards
          deck.pop_back();
        }
        turn = playerIndex(turn, turnIncrement, 1);        //skip one person
      }
      else if(onTable.type == "wildCard"){
        cout << players[turn] -> getName() << " Just changed the color!" << endl;
        turn = playerIndex(turn, turnIncrement, 0);         //go to the next person
      }
    }
    else if(onTable.type == "plusTwo"){             //if the card is a plus two
      cout << players[playerIndex(turn, turnIncrement, 0)] -> getName() << " Just received 2 extra cards and lost his turn!" << endl;
      for(int i = 0; i < 2; i++){                  //loop that adds 2 cards to the next player
        if(deck.empty()){                          //if the deck is empty, refill it
          restockDeck(deck, graveyard);
        }
        players[playerIndex(turn, turnIncrement, 0)] -> addCard(deck.back()); //give the next player 2 cards
        deck.pop_back();
      }
      turn = playerIndex(turn, turnIncrement, 1);         //skip one person
    }
    else if(onTable.type == "reverse"){            //if the card is a reverse card
      cout << players[turn] -> getName() << " Just reversed the turn order!" << endl;
      turnIncrement = - turnIncrement;            //reverse the order of the turns
      turn = playerIndex(turn, turnIncrement, 0);          //go to the next person
    }
    else if(onTable.type == "stop"){            //if the card is a stop card
      cout << players[playerIndex(turn, turnIncrement, 0)] -> getName() << " Just got skipped!" << endl;
      turn = playerIndex(turn, turnIncrement, 1);          //skip one person
    }
    else{                                         //if a regular card was placed
      turn = playerIndex(turn, turnIncrement, 0);          //go to the next person
    }



    //Check for winners:
    for(int i = 0; i < 4; i++){
      if(!(players[i] -> getHandSize())){
        isWinner++;
        cout << "-------------------------------------" << players[i] -> getName() << " has won the game!" << "-------------------------------------" << endl;
        cout << "Thank you for playing!" << endl;

      }
    }
}
  return 0;
}


void printIntroMenu(){
  cout << "Welcome to Uno version 0.9" << endl;
  cout << "-------------------------------------" << endl;
  cout << "Enter the number of players:" << endl;
  cout << "(Set to 1 by default for testing)" << endl;
}

void populateDeck(vector<card> &deck){
  ifstream infile("UnoDeck.txt"); //text file containing the cards
  card temp;                      //variable that will temporarily hold the values from the text file
  for(int stop = 0; stop <= 71; ++stop){
    infile >> temp.color >> temp.number >> temp.type;
    deck.push_back(temp);
  }

}

void shuffleDeck(vector<card> &deck){
  int seed = rand() % 20; //gets a random seed
  random_shuffle (deck.begin(), deck.end(), myrandom); //shuffles the deck using the seed
}

int myrandom (int i) { return std::rand()%i;}

void restockDeck(vector<card> &deck, vector<card> &graveyard){
  for(vector<card>::iterator it = graveyard.begin(); it != graveyard.end(); it++){ //loops around graveyard and places is into deck
    deck.push_back(*it);
  }
  graveyard.clear();
  shuffleDeck(deck);
}

int playerIndex(int turn, int turnIncrement, int nextOrSkip){ //addOrNext can have 2 values: 0 to simply going next, 1 to skip a player and go to the next
  int ans;
  if(nextOrSkip == 0)
    ans = (turn + turnIncrement) % 4;
  if(nextOrSkip == 1)
    ans = (turn + 2*turnIncrement) % 4;
  if(ans < 0){
    ans = ans + 4;
  }
  return ans;
}
