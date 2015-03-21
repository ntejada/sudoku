CXX=g++
RM=rm -f

SRCS=driver.cpp board.cpp cell.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: sudoku

sudoku: $(OBJS)
	$(CXX) -o sudoku $(OBJS)

driver.o: driver.cpp 

cell.o: cell.cpp

board.o: board.cpp

clean:
	$(RM) $(OBJS) sudoku
