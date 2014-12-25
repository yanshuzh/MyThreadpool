INC=
LIB=-lpthread 


CXX=g++
CC_FLAG=-g -Wall -std=c++11 

TARGET=test
OBJ=Task.o MThread.o MThreadPool.o ThreadManage.o test.o
$(TARGET):$(OBJ)
	$(CXX) -o $@ $(OBJ) $(INC) $(LIB)
.SUFFIXES: .c .o .cpp
.cpp.o:
	$(CXX) -c $*.cpp -o $*.o $(CC_FLAG) $(INC)

.PRONY:clean
clean:
	@echo "Removing linked and compiled files...."
	rm -f $(OBJ) $(TARGET)
