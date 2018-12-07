

/****************************************************************/
#ifndef PLAYER
#define PLAYER

#include <iostream>
#include <fstream>
#include <cmath>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

struct card {
 char color;
 int number;
 string type;
};

class player{
protected:
 list<card> hand;
 list<card> allowedCards;
 string name;
public:

 player(){cout << "Player created" << endl;}
 ~player(){cout << "Player destroyed" << endl;}

 void setName(string n){
	 name = n;
 }

 string getName(){
	 return name;
 }

 void addCard(card c){ hand.push_back(c);}

 void getHand(){
	 cout << "Your hand is:" << endl;
	 for(list<card>::iterator it = hand.begin(); it != hand.end(); it++){
		 if(it -> type == "regular"){ //regular card with color
			 cout << it -> color << it -> number<< endl;
		 }
		 else if(it -> type == "plusTwo" || it -> type == "reverse"){//plus two special
			 cout << it -> type << it -> color << endl;
		 }
		 else {//plus four special
			 cout << it -> type << endl;
		 }
	 } //end for
 }

 int getHandSize(){
	 return hand.size();
 }

 virtual char chooseColor(){
	 int radom = rand() % 4;
	 switch(radom){
		 case 0:
				 cout << "Chosen color: Red\n";
				 return 'R';
				 break;
			 case 1:
				 cout << "Chosen color: Blue\n";
				 return 'B';
				 break;
			 case 2:
				 cout << "Chosen color: Yellow\n";
				 return 'Y';
				 break;
			 default:
				 cout << "Chosen color: Green\n";
				 return 'G';
				 break;
	 }
 }

 void remove(card c){
	 for(list<card>::iterator i=hand.begin(); i != hand.end(); ++i){
		 if(i->color == c.color){
				 if( (i->type == c.type) && (i->number == c.number)) i = hand.erase(i);
			 }
	 }
	 return;
 }

 card setAllowedCards(card onTable){
	 allowedCards.clear();
	 for(list<card>::iterator it = hand.begin(); it != hand.end(); it++){
		 if  (it->number == 10) {
				 if((it->type == "plusFour") || (it->type == "wildCard") ||
						 ( (it->color == onTable.color) || (it->type == onTable.type)))

						 allowedCards.push_back(*it);
			 } else {
				 if( (it->number == onTable.number ) || (it->color == onTable.color)) {
							allowedCards.push_back(*it);
					 }
			 }

	 }

 }
 void getAllowedCards(){ //print cards allowed
	 for(list<card>::iterator it = allowedCards.begin(); it != hand.end(); it++){
		 cout << it->color << " " << it->number << " " << it->type << endl;
			 return;
	 }
 }




 virtual card strategy(card) = 0;
};

/****************************************************************/
/* AI1
Tuesday December 4
12:00
AI1 is the first derived class from Player
AI1 has no strategy (returns first card allowed)
*/


class AI1 : public player {

public:
	AI1(){cout << "AI1 created" << endl; setName("AI1");}
 ~AI1(){cout << "AI1 destroyed" << endl;}
 //available functions
 //addCard(card)
 //getAllowedCards()
 //getHand()

 card strategy(card onTable){
	/*No strategy*/
 if(allowedCards.size()){
	 	card temp = allowedCards.front();
		 cout << getName() << ": card played: "<<temp.color << temp.number << " "<< temp.type <<endl;
	 		remove(temp);
			return temp;


 } else { //return default "no options" card
	 card temp;
	 temp.type = "NAN";
			 cout << getName() << " has no cards to play... Card added!" << endl;
	 return temp;
 }
 }
};
/***************************************************************************/
/* AI2
Tuesday December 4
10:46
AI2 is a derived class from Player
AI2 has unique strategy: Plays first possible Number
*/


class AI2 : public player {

public:
	AI2(){cout << "AI2 created" << endl; setName("AI2");}
 ~AI2(){cout << "AI2 destroyed" << endl;}
 //available functions
 //addCard(card)
 //getAllowedCards()
 //getHand()


 card strategy(card onTable){
 /*Order of game play:
 1) Matching number if possible
 2) Specialty card if available
 3) First matching color
 */
 if(allowedCards.size()){
	 for(list<card>::iterator i=allowedCards.begin(); i != allowedCards.end(); ++i){
		 if(i->number == onTable.number){
						cout << getName() << ": card played: "<<i->color << " "<< i->number << " "<< i->type <<endl;
						remove(*i);
			 return *i;
		 }
	 }
	 //if none of possible moves are the same number return specialty card
	 for(list<card>::iterator i=allowedCards.begin(); i != allowedCards.end(); ++i){
		 if( (i->type == "plusFour") || (i->type == "wildCard") ||
		 				((i->color == onTable.color) && (i->type == "plusTwo")) ||
						 ((i->color == onTable.color) && (i->type == "stop")) ||
							 ((i->color == onTable.color) && (i->type == "reverse"))){
							 cout << getName() << ": card played: "<<i->color << " "<< i->number << " "<< i->type <<endl;
							 remove(*i);
							 return *i;
						 }
	 }
	 card temp = allowedCards.front();
			 cout << getName() << ": card played: "<<temp.color << " "<< temp.number << " "<< temp.type <<endl;

			 remove(temp);
	 return temp;

 } else { //return default "no options" card
	 card temp;
	 temp.type = "NAN";
			 cout << getName() << " has no cards to play... Card added!" << endl;
	 return temp;
 }
 }
};
/***************************************************************************/
/* AI3
Tuesday December 4
11:18
AI3 is a derived class from Player
AI3 has unique strategy: Plays first possible color
*/


class AI3 : public player {

public:
	AI3(){cout << "AI3 created" << endl; setName("AI3");}
 ~AI3(){cout << "AI3 destroyed" << endl;}
 /*available functions:
	 addCard(card)
	 getAllowedCards()
	 getHand()
 */


 card strategy(card onTable){
 /*Oder of game play:
 1) First matching color
 2) Specialty card if available
 3) Matching number if possible
 */
 if(allowedCards.size()){ //if there are cards in this list
	 for(list<card>::iterator i = allowedCards.begin(); i != allowedCards.end(); ++i){
		 if(i->color == onTable.color){
						 	cout << getName() << ": card played: "<<i->color << " "<< i->number << " "<< i->type <<endl;
							remove(*i);
			 return *i;
		 }
	 }
	 //if none of possible moves are the same number return specialty card
	 for(list<card>::iterator i=allowedCards.begin(); i != allowedCards.end(); ++i){
		 if( (i->type == "plusFour") || (i->type == "plusTwo") ){
						 	cout << getName() << ": card played: "<<i->color << " "<< i->number << " "<< i->type <<endl;
							remove(*i);
			 return *i;
		 }
	 }
	 		card temp = allowedCards.front();
			 cout << getName() << ": card played: "<<temp.color << " "<< temp.number << " "<< temp.type <<endl;
			 remove(temp);
	 return temp; //if neither color nor specialty possible
 } else { //return default "no options" card
	 card temp;
	 temp.type = "NAN";
			 cout << getName() << " has no cards to play... Card added!" << endl;

	 return temp;
 }
 }
};
/***************************************************************************/

/* User Player
Tuesday December 6
12:46
User is a derived class from Player
User takes input to put down a card
*/



class User : public player {
public:
	User(){string n; cout << "User created" << endl << "Enter your name! " << endl; cin >> n; setName(n); cout << "Welcome " << getName() << endl;}
 ~User(){cout << "User destroyed" << endl;}
 //available functions
 //addCard(card)
 //getAllowedCards()
 //getHand()

 char chooseColor(){
	 cout << "Enter a number between 1-4\n";
	 int radom;
	 cin >> radom;
	 switch(radom){
		 case 1:
				 cout << "Chosen color: Red\n";
				 return 'R';
				 break;
			 case 2:
				 cout << "Chosen color: Blue\n";
				 return 'B';
				 break;
			 case 3:
				 cout << "Chosen color: Yellow\n";
				 return 'Y';
				 break;
			 default:
				 cout << "Chosen color: Green\n";
				 return 'G';
				 break;
	 }
 }

 card strategy(card onTable){
 if(allowedCards.size()){ //returns the first card available
	 getHand();
	 while(1){
		 cout << "Pick a Card (color, number, type) from your hand" << endl;
		 char c; int n; string t;
		 cin >> c >> n >> t;
		 card temp;
		 temp.color = c; temp.number = n; temp.type = t;
				 for(list<card>::iterator i = allowedCards.begin(); i != allowedCards.end(); ++i){
					 if(temp.color == i->color && temp.number == i->number && temp.type == i->type){
						 cout << getName() << " card played: "<< i->color << " "<< i->number << " "<< i->type <<endl;
						 remove(temp); //removes the card from the hand before playing it
						 return temp;
					 }
	 		 	}
				cout << "The card does not exist, or cannot be played. ¯_(ツ)_/¯" << endl;
	 	}
	}
	else{
		card temp;
		temp.type = "NAN";
				cout << getName() << ": has no cards to play... Card added!" << endl;

		return temp;
	}
 }
};
/***************************************************************************/

#endif
