/*
 * board.cpp
 *
 *  Created on: Mar 2, 2015
 *      Author: EmilyBaer, NicTejada
 */

#include "board.h"
#include "cell.h"
#include <math.h>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <time.h>
using namespace std;

//default constructor
board::board(){
    values = new cell*[9];
    for(int i=0; i<9; i++){
		values[i]= new cell[9];
        for(int j=0; j<9; j++){
			values[i][j] = cell(i, j, this);
		}
	}
    fill_n(uniqueDigits, 9, false);
    totSolved = 0;
}

//initializer with 2d array initVals
//uses constraint propagation (through assign) to see if any cells can be solved without guessing (search)
void board::init(int initVals[9][9]){
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            if(initVals[i][j]>0){
                if(!this->values[i][j].assign(initVals[i][j])){
                    throw logic_error("Unsolvable puzzle!");
                }
            }
        }
    }
}

//copy constructor
board::board(const board& oldBoard){
    if(oldBoard.values==0){
        values=0;
    }
    else{
        values = new cell*[9];
        for(int i=0; i<9; i++){
	    	values[i]= new cell[9];
            for(int j=0; j<9; j++){
		        values[i][j] = cell(oldBoard.values[i][j], this);
		    }
	    }
    }
}

//assignment operator
board& board::operator=(board oldBoard){
    if(oldBoard.values==0){
        values=0;
    }
    else{
        values = new cell*[9];
        for(int i=0; i<9; i++){
	    	values[i]= new cell[9];
            for(int j=0; j<9; j++){
		        values[i][j] = cell(oldBoard.values[i][j], this);
		    }
	    }
    }
}

//depth-first search of the board for the solution
board* board::search(){
    if(this->isSolved()){
        return this;
    }
    else{
        cell* low = this->lowCell();
        for(int k = 0; k<9; k++){
            if(low->is_poss(k+1)){
                board* copyBoard = new board(*this);
                board* newBoardP=0;
                if(copyBoard->values[low->getX()][low->getY()].assign(k+1)){
                    newBoardP=copyBoard->search();
                    if(newBoardP!=0){
                        return newBoardP;
                    }
                }
                delete copyBoard;
            }
        }
        return 0;
    }
}


//returns a pointer to the cell with the least number of possible values that is not solved
cell* board::lowCell(){
    cell* retCell=0;
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            if(values[i][j].getPossSize()>1){
                if(values[i][j].getPossSize()==2){
                    return &values[i][j];
                }
                else if(retCell==0){
                    retCell=&values[i][j];
                }
                else if(retCell->getPossSize()>values[i][j].getPossSize()){
                    retCell = &values[i][j];
                }
            }
        }
    }
    return retCell;
}

//returns true if the board is solved already
bool board::isSolved(){
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            if(!values[i][j].solved_cell()){
                return false;
            }
        }
    }
    return true;
}


void board::print(){
    for(int i = 0; i<9; i++){
        if(i==3 || i==6){
            cout << "-------+-------+-------" << endl;
        }
        for(int j = 0; j<9; j++){
            if(j==2 || j == 5){
                cout<< values[i][j].get_solved();
                cout << "|";
            }
            else{
            cout<< values[i][j].get_solved() << "  ";
            }
        }
        cout << endl;
    }
}

void board::printPossBoard(){
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            for(int k = 0; k<9; k++){
                if(values[i][j].is_poss(k+1))
                        cout << k+1;
            }
            cout << " ";
        }
        cout << endl;
    }
}

void board::printPossVals(){
    for(int i = 0; i<9; i++){
        for(int j = 0; j<9; j++){
            cell curr = values[i][j];
            cout<<"("<<i<<","<<j<<"): ";
            if(curr.solved_cell()){cout<<"solved " << values[i][j].get_solved()<<endl;}
            else{
                bool * currPossVals= curr.get_possVals();
                for(int k = 0; k<9; k++){
                    if(currPossVals[k]==true){
                        cout << k+1 << " ";
                    }
                }
                cout<<endl;
            }
        }
    }
}

//Random board generation:
//generates a random solvable board with at least n squares filled
board rand_board(int n){
    board *randBoard;
    while(1){
        randBoard = new board();
        
        while(1){
            //choose a random cell on the board
            int x = rand() % 9;
            int y = rand() % 9;
            cell* randCell = randBoard->getCell(x, y);
            //if it is already solved, choose another cell
            if(randCell->solved_cell()){
                continue;
            }
            //counter of how many values tried in the cell in question
            int possTried = 0;
            //attempt to put possSize values in randCell 
            while(possTried<randCell->getPossSize()){
                int i = rand()%9;
                //if i is possible attempt to assign to randCell
                if(randCell->is_poss(i+1)){
                    possTried++;
                    //since assign() changes the board we have to make a copy
                    board* copyBoard = new board(*randBoard);
                    cell* copyCell = copyBoard->getCell(x, y);
                    //if the assign is impossible then delete the copyBoard
                    if(!copyCell->assign(i+1)){
                        delete copyBoard;
                    }
                    //else use the newly assigned board
                    else{
                        delete randBoard;
                        randBoard = copyBoard;
                        break;
                    }
                }
            }
            //check if the randBoard is ready to return
            if(randBoard->rand_board_check(n)){
                //return the board if the board is solvable
                if(randBoard->search()!=0){
                    return *randBoard;
                }
                //else try again
                else{
                    delete randBoard;
                    break;
                }
            }
        }
    }
}

bool board::rand_board_check(int n){
    int uniqueTot=0;
    for(int i = 0; i<9; i++){
        if(uniqueDigits[i])
            uniqueTot++;
    }
    return uniqueTot>7 && totSolved>=n;
}
