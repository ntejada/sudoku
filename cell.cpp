#include "cell.h"
#include "board.h"
#include <math.h>
#include <iostream>
using namespace std;

//copy constructor
cell::cell(const cell& oldCell, board* newBoard){
    possSize=oldCell.possSize;
    solved=oldCell.solved;
    x=oldCell.x;
    y=oldCell.y;
    b=newBoard;
    possVals = new bool[9];
    for(int i = 0; i<9; i++){
        possVals[i]=oldCell.possVals[i];
    }
}

//assignment operator overload
cell& cell::operator=(cell oldCell){
    possSize=oldCell.possSize;
    solved=oldCell.solved;
    x=oldCell.x;
    y=oldCell.y;
    b=oldCell.b;
    possVals = new bool[9];
    for(int i = 0; i<9; i++){
        possVals[i]=oldCell.possVals[i];
    }
}

//tries to assign d to this
//returns false if the assignment is illegal
//this can be if:
//1. one of the cell's peers is already assigned d
//2. the assignment leads to an impossible situation (some cell has no possible values)
bool cell::assign(int d){
    board* b = this->b;    
    //check if one of the cell's peers already assigned d
    for(int k=0; k<9; k++){
	    if(k!=this->y){
            //column
            cell* c1 = b->getCell(this->x, k);
            if(c1->get_solved()==d){
                return false;
            }
        }
        if(k!=this->x){
            //row
            cell* c2 = b->getCell(k, this->y);
            if(c2->get_solved()==d){
                return false;
            }
        }
    }
	//quadrant
    int m,n,endx,endy;
	m=floor(this->x/3)*3;
    endx=m+3;
	n=floor(this->y/3)*3;
    endy=n+3;
	for(m=floor(this->x/3)*3; m<endx; m++){
		for(n=floor(this->y/3)*3; n<endy; n++){
            if (m!=this->x && n!=this->y){
			    cell* c3 =  b->getCell(m, n);
                if(c3->get_solved()==d){
                    return false;    
                }
            }
	    }
    }

    //constraint propagation on the cell's peers (what is the effect of assigning d to this?)
    this->solved = d;
    this->b->incTotSolved();
    this->b->updateUnique(d-1);
    for(int k=0; k<9; k++){
	    if(k!=this->y){
            cell* c1 = b->getCell(this->x, k);
            if((*c1).eliminate(d)==false){
                return false;
            }
        }
        if(k!=this->x){
            cell* c2 = b->getCell(k, this->y);
            if((*c2).eliminate(d)==false){
                return false;
            }
        }
    }
	m=floor(this->x/3)*3;
    endx=m+3;
	n=floor(this->y/3)*3;
    endy=n+3;
	for(m=floor(this->x/3)*3; m<endx; m++){
		for(n=floor(this->y/3)*3; n<endy; n++){
            if (m!=this->x && n!=this->y){
			    cell* c3 =  b->getCell(m, n);
                if((*c3).eliminate(d)==false){
                    return false;    
                }
            }
	    }
    }
    //update the cell
    this->possSize=1;
    for(int i = 0; i<9; i++){
        this->possVals[i]=false;
    }
    this->possVals[d-1]=true;
    return true;
}

//eliminate is called on each peer when a cell is assigned a value
//it eliminates d from this->possVals and checks if that causes any other assignments to be apparent
//these checks are:
//1) check if this now has only one possible value (that value should now be assigned to this)
//2) check if this's units (its row, column, and quadrant) now have only one place for d (d should then be assigne to that cell)
//eliminate returns false if any cells are reduced to having 0 possible values 
bool cell::eliminate(int d){

    //not possible already
   if(this->possVals[d-1]==false){
       return true;
   }
 
   if(this->solved>0){
       return true;
   }

   int poss = this->updatePoss(d);
   
   if(poss==0){
       return false; //not possible
   }

    //SQUARE
   if(poss==1){
       for(int i = 0; i<9; i++){
           if(this->possVals[i])
               if(!this->assign(i+1)){
                   return false;
               }
       }
   }

    //UNITS
   int rowSum=0;
   int colSum=0;
   cell* rowCell = 0;
   cell* colCell = 0;
   for(int k=0; k<9; k++){
	    cell* c1 = b->getCell(this->x, k);
        if(c1->is_poss(d)){
            rowCell=c1;
            rowSum++;
        }
        cell* c2 = b->getCell(k, this->y);
        if(c2->is_poss(d)){
            colCell=c2;
            colSum++;
        }
    }
    
    int m,n,endx,endy;
    m=floor(this->x/3)*3;
    n=floor(this->y/3)*3;
    endx=m+3;
    endy=n+3;
    int quadSum = 0;
    cell* quadCell = 0;
	for(m=floor(this->x/3)*3; m<endx; m++){
		for(n=floor(this->y/3)*3; n<endy; n++){
			cell* c3 =  b->getCell(m, n);
            if(c3->is_poss(d)){
                quadCell = c3;
                quadSum++;
            }
        }
    }
    
    if(rowSum==0||colSum==0||quadSum==0){
        return false;
    }

    if(rowSum==1 && rowCell->get_solved()==0){
        if(!rowCell->assign(d))
            return false;
    }
    if(colSum==1 && colCell->get_solved()==0){
        if(!colCell->assign(d))
            return false;
    }
    if(quadSum==1 && quadCell->get_solved()==0){
        if(!quadCell->assign(d))
            return false;
    }
    return true;
}
