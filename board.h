/*
 * board.h
 *
 *  Created on: Mar 2, 2015
 *      Author: EmilyBaer, Nic Tejada
 */

#ifndef BOARD_H
#define BOARD_H
#include "cell.h"
#include <iostream>

class board{
    private:
    cell** values;
    int totSolved; 
    bool uniqueDigits[9];
    public:
    board();
    ~board(){for(int i = 0; i<9; i++){if(values[i]!=0){delete[] values[i];}} if(values!=0){delete[] values;}};
    board& operator=(board oldBoard);
    void init(int initVals[9][9]);
    board(const board& oldBoard);
    cell* getCell(int x, int y){return &values[x][y];};
    void print();
    void printPossBoard();
    void printPossVals();
    cell* lowCell();
    board* search();
    bool isSolved();
    void incTotSolved(){totSolved++;};
    void updateUnique(int x){if(x>-1&&x<9){uniqueDigits[x]=true;}};
    bool rand_board_check(int n);
};

board rand_board(int n);
#endif
