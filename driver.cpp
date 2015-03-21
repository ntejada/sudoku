#include "board.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

int main(){
     int init[9][9] = {
        {0,0,3,0,2,0,6,0,0} ,
        {9,0,0,3,0,5,0,0,1} ,
        {0,0,1,8,0,6,4,0,0} ,
        {0,0,8,1,0,2,9,0,0} ,
        {7,0,0,0,0,0,0,0,8} ,
        {0,0,6,7,0,8,2,0,0} ,
        {0,0,2,6,0,9,5,0,0} ,
        {8,0,0,2,0,3,0,0,9} ,
        {0,0,5,0,1,0,3,0,0} ,
    };
    int init2[9][9] = {
        {0,1,0,0,0,4,0,0,0} ,
        {0,0,6,8,0,5,0,0,1} ,
        {5,0,3,7,0,1,9,0,0} ,
        {8,0,4,0,0,7,0,0,0} ,
        {0,0,0,0,0,0,0,0,0} ,
        {0,0,0,3,0,0,6,0,9} ,
        {0,0,1,5,0,8,2,0,4} ,
        {6,0,0,4,0,3,1,0,0} ,
        {0,0,0,2,0,0,0,5,0} ,
    };
    
    board testBoard2, testBoard;
    testBoard.init(init);
    testBoard2.init(init2);
    board* searchedBoard2 = testBoard2.search();
    
    cout<< "One easy (that was solved only through constraint propagation) and one difficult board" <<endl;
    testBoard.print();
    cout<<endl;
    testBoard2.print();
    cout<<endl;
    searchedBoard2->print();

    cout<<endl;
    int t = 15;
    srand(time(NULL));
    cout<<"15 randomly generated sudoku boards with their solutions" << endl;
    cout<<endl;
    while(t--){
        board randBoard = rand_board(9);
        randBoard.print();
        board* searchedRandBoard = randBoard.search();
        cout<<endl;
        searchedRandBoard->print();
        cout <<endl;
    }
}

