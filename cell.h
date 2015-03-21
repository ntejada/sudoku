/*
 * cell.h
 *
 *  Created on: Mar 2, 2015
 *      Author: EmilyBaer, NicTejada
 */
#ifndef CELL_H
#define CELL_H
#include <vector>
#include <iostream>
class board;
using namespace std;

class cell{
	bool * possVals;
    int possSize;
	int solved;
    int x;
    int y;
    board* b;
    public:
	cell(){};
    cell& operator=(cell oldCell);
    ~cell(){if(possVals!=0){delete[] possVals;}};
    cell(const cell& oldCell, board* newBoard);
    cell(int x, int y, board* b){possVals = new bool [9]; solved = 0; fill_n(possVals, 9, true); possSize = 9; this->x=x; this->y=y; this->b=b;};
	bool solved_cell(){return solved>0;};
	int getPossSize(){return possSize;};
    bool* get_possVals(){return possVals;};
	bool is_poss(int x){return possVals[x-1];};
    int updatePoss(int x){if(possVals[x-1]){possSize--;} possVals[x-1]=false; return possSize;};
    int get_solved(){return solved;};
    bool eliminate(int d);
    bool assign(int d);
    int getX(){return x;};
    int getY(){return y;};
};
#endif
