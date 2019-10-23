//
//  main.cpp
//  Mastermind
//
//  Created by Damian Charczuk on 11/9/18.
//  Copyright © 2018 Damian Charczuk. All rights reserved.
//
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
using namespace std;

//general class to keep track of individual players' name and points
class Player{
public:
    string name;
    int points;
    void addPoint(){points++;}
};


class Game{
public:
    int maxTurns;               //sets how many tries a player has to win the game
    int numberOfGames;          //determines how many games are played
    int numberOfPointsToWin;    //determines how many points a player needs to win the set of games
    int toGuess;                //player who is guessing (1 = player 1, 2 = player 2)
    int numbersToGuess[4];      //array of hidden numbers that the player must guess
    bool won;                   //determines whether a player won the game (used for awarding points)
    int counterOfGames;         //keeps track of how many games are played
    
};

void displayInstructions(){
    cout << "Welcome to Mastermind" << endl;
    cout << "You will be given 4 numbers either by the computer or by another player" << endl;
    cout << "The goal of the game is to guess the exact order of the numbers." << endl;
    cout << "In One Player mode, you will be given a random order of 4 numbers to guess" << endl;
    cout << "In Two Player mode, one player will set the 4 numbers that the other player will guess" << endl;
    cout << "If you successfully determine the correct order of numbers, you win. Otherwise, you lose" << endl;
    cout << "In Two Player mode, you are awarded with 1 point if you win the game." << endl;
    cout << "In the end, the winner of the set of games is determined by each player's accumulated total points." << endl;
    cout << endl;
}

//prints out contents of sent array
void displayArray(int array[]){
    for(int x = 0; x < 4; x++){
        cout << array[x] << " ";
    }
    cout << endl;
}

//generate a random integer between 1 and 2
int randomNumber2(){
    return (rand() % 2) + 1;
}

void randomSetNumbersToGuess(int numbersToGuess[]){
    for(int x = 0; x < 4; x++){
        numbersToGuess[x] = (rand() % 9) + 1;   //generate a number between 1 and 9
    }
    
}

//setup for one player mode
//sets the max number of turns per game as well as the numbers that need to be guessed
void onePlayerGameSetup(int &maxTurns, int numbersToGuess[]){
    //determine how long each game will go
    cout << "Please determine the max number of turns allowed per game: ";
    cin >> maxTurns;
    
    randomSetNumbersToGuess(numbersToGuess);
}

//set up initial parameters for 2 player game
void twoPlayerGameSetup(Player &player1, Player &player2, int &maxTurns, int &numberOfGames, int &numberOfPointsToWin, int toGuess){
    int input;
    
    //set how many games the players will play
    cout << "Please determine how long the game goes on" << endl;
    cout << "1. Until one player reached a certain number of points" << endl;
    cout << "2. Until a certain number of games have been played" << endl;
    cout << "Option selected: ";
    cin >> input;
    cout << endl;
    
    //determine what to base length of game off of
    if(input == 1){
        cout << "How many points do you want to play until? ";
        cin >> numberOfPointsToWin;
    }
    else{
        cout << "How many games do you want to play? ";
        cin >> numberOfGames;
    }
    
    //determine how long each game will go
    cout << "Please set the max number of turns allowed per game: ";
    cin >> maxTurns;
    
    //set the names of the players for easier recognition of whose turn it is
    string name;
    cout << "Please enter player names separated by a space: ";
    cin >> name;
    player1.name = name;
    cin >> name;
    player2.name = name;
    cout << endl;
    
    //determine which player guess first
    toGuess = randomNumber2();
}

//manually set array values
void setNumbers(int numbersToGuess[], int number, int counter){
    numbersToGuess[counter] = number;
}


//other player sets the numbers the player has to guess
void setNumbersToGuess(string name, int numbersToGuess[]){
    int input;
    
    cout << name << ", enter 4 integers between 0 and 9 (numbers may be repeated): ";
    
    for(int x = 0; x < 4; x++){
        cin >> input;
        setNumbers(numbersToGuess, input, x);
    }
}

void compareArrays(int guess[], int numbersToGuess[], int &inPlace, int &outOfPlace){
    //create copies of guess[] and numbersToGuess[] to alter when comparing them
    int guessCopy[4];
    int numbersToGuessCopy[4];
    
    for(int x = 0; x < 4; x++){
        guessCopy[x] = guess[x];
        numbersToGuessCopy[x] = numbersToGuess[x];
    }
    
    //find which numbers guess and numbersToGuess are not common to both arrays
    //loop through and if a number is not found, set to -1
    //then, don't compare if a number is set to -1
    
    //turn all unshared numbers in guess[] to -1
    for(int g = 0; g < 4; g++){
        bool found = false;
        for(int n = 0; n < 4; n++){
            if(guess[g] == numbersToGuess[n]){
                found = true;
            }
        }
        if(found != true){
            guessCopy[g] = -1;
        }
    }
    
    //turn all unshared numbers in numbersToGuess[] to -1
    for(int n = 0; n < 4; n++){
        bool found = false;
        for(int g = 0; g < 4; g++){
            if(numbersToGuess[n] == guess[g]){
                found = true;
            }
        }
        if(found != true){
            numbersToGuessCopy[n] = -1;
        }
    }
    
    //loop through arrays to find which numbers are in the correct places
    //loop through arrays to find which numbers are shared by both arrays but in different places
    //change matches to -1 to avoid recounting
    inPlace = 0;
    outOfPlace = 0;
    for(int g = 0; g < 4; g++){
        for(int n = 0; n < 4; n++){
            if((guessCopy[g] == numbersToGuessCopy[n]) && (g == n) && (guessCopy[g] != -1)){
                inPlace++;
                guessCopy[g] = -1;
                numbersToGuessCopy[n] = -1;
            }
            else if((guessCopy[g] == numbersToGuessCopy[n]) && (guessCopy[g] != -1)){
                outOfPlace++;
                guessCopy[g] = -1;
                numbersToGuessCopy[n] = -1;
            }
            else{   //guess and number do not match or both numbers are -1
                continue;
            }
        }
    }
}

void guessing(int numbersToGuess[], int maxTurns, bool &won){
    won = false;
    cout << endl;
    cout << "Turns         In Place   Out of Place" << endl;
    int turnCounter = 1;
    int guess[4] = {0,0,0,0};
    
    //take in a guess
    while(turnCounter < (maxTurns + 1)){
        cout << turnCounter << ". ";
        int input;
        int counter = 0;
        for(int x = 0; x < 4; x++){
            cin >> input;
            guess[counter] = input;
            counter++;
        }
        
        //compare guess[] and numbersToCompare[] to determine how many numbers are in place or out of place
        int inPlace = 0;
        int outOfPlace = 0;
        compareArrays(guess, numbersToGuess, inPlace, outOfPlace);
        
        cout << "                 " << inPlace << "             " << outOfPlace << endl;
        
        if(inPlace == 4){
            won = true;
            break;
        }
        turnCounter++;
    }// end while loop
}

//executes each game
void game(Player &player1, Player &player2, int maxTurns, int numberOfGames, int numberOfPointsToWin, int toGuess, int numbersToGuess[], bool won){
    //display which player guesses and which player set the numbers
    if(toGuess == 1){
        cout << "The player who will guess will be " << player1.name << endl;
        setNumbersToGuess(player2.name, numbersToGuess);
    }
    else{
        cout << "The player who will guess will be " << player2.name << endl;
        setNumbersToGuess(player1.name, numbersToGuess);
    }
    
    //creates 20 blank lines to hide the numbers that need to be guessed
    for(int x = 0; x < 20; x++){
        cout << endl;
    }
    
    //guessing process
    guessing(numbersToGuess, maxTurns, won);
    
    if(won){
        if(toGuess == 1){
            player1.addPoint();
            cout << "Congratulations " << player1.name << "! You now have " << player1.points << " point(s)" << endl;
            cout << player2.name << " has " << player2.points << " points" << endl;
        }
        else{
            player2.addPoint();
            cout << "Congratulations " << player2.name << "! You now have " << player2.points << " point(s)" << endl;
            cout << player1.name << " has " << player1.points << " points" << endl;
        }
    }
    
}

void playSetOfGames(Player &player1, Player &player2, int maxTurns, int numberOfGames, int numberOfPointsToWin, int toGuess, int numbersToGuess[], bool won, int counterOfGames){
    //playing based on number of games
    if(numberOfGames != -1){
        while(counterOfGames != numberOfGames){
            cout << "\nGame " << (counterOfGames + 1) << endl;
            game(player1, player2, maxTurns, numberOfGames, numberOfPointsToWin, toGuess, numbersToGuess, won);
            counterOfGames++;
            
            //changes which player guesses
            if(toGuess == 1){
                toGuess = 2;
            }
            else{
                toGuess = 1;
            }
        }
    }
    //playing based on total points
    else{
        int counterOfGames = 0;
        //continue to play until one player acquires the number of points needed to win
        while((player1.points != numberOfPointsToWin) && (player2.points) != numberOfPointsToWin){
            cout << "\nGame " << (counterOfGames + 1) << endl;
            game(player1, player2, maxTurns, numberOfGames, numberOfPointsToWin, toGuess, numbersToGuess, won);
            counterOfGames++;
            
            //changes which player guesses
            if(toGuess == 1){
                toGuess = 2;
            }
            else{
                toGuess = 1;
            }
        }
    }
    
    string input;
    cout << "Would you like to keep playing? (yes or no) ";
    cin >> input;
    if(input == "yes"){
        if(numberOfGames > -1){
            numberOfGames++;
        }
        else{
            numberOfPointsToWin++;
        }
        playSetOfGames(player1, player2, maxTurns, numberOfGames, numberOfPointsToWin, toGuess, numbersToGuess, won, counterOfGames);
    }
    
}//end playSetOfGames()

int main(){
    srand(1);
    int numbersToGuess[] = {-1,-1,-1,-1};
    int input;
    
    int maxTurns;
    int numberOfGames = -1;
    int numberOfPointsToWin = -1;
    
    bool won = false;
    
    displayInstructions();
    cout << "Please choose a mode by entering \'1\' or \'2\'" << endl;
    cout << "1. One player" << endl;
    cout << "2. Two player" << endl;
    cout << "Option selected: ";
    cin >> input;
    cout << endl;
    
    switch(input){
            //one player game
        case 1:
            onePlayerGameSetup(maxTurns, numbersToGuess);
            guessing(numbersToGuess, maxTurns, won);
            break;
            
            //two player game
        case 2:
            //create two players
            Player player1;
            Player player2;
            
            player1.points = 0;
            player2.points = 0;
            
            int toGuess = 0;
            int counterOfGames = 0;
            
            //set up game parameters
            twoPlayerGameSetup(player1, player2, maxTurns, numberOfGames, numberOfPointsToWin, toGuess);
            //play game until all games are played
            playSetOfGames(player1, player2, maxTurns, numberOfGames, numberOfPointsToWin, toGuess, numbersToGuess, won, counterOfGames);
            
            //compare player points after all games have been played
            if(player1.points == player2.points){
                cout << "It's a tie! Both players have " << player1.points << endl;
            }
            else if(player1.points > player2.points){
                cout << player1.name << " wins!" << endl;
            }
            else{
                cout << player2.name << " wins!" << endl;
            }
            break;
    }
    
    cout << "Exiting program...";
    exit(0);
    return 0;
}
