# Makefile for Image Multiplexing Project
CPP  = g++.exe
CPPFLAGS = -Wno-deprecated

SRC_DIR = src
INCL_DIR = include
OBJ_DIR = obj
LIB_DIR = ../Base_Classes/lib
LIB_INCL_DIR = ../Base_Classes/include
EXEC_DIR = executable

OBJ1 = $(OBJ_DIR)/mult.o
OBJ2 = $(OBJ_DIR)/demult.o
LIBS = -L$(LIB_DIR) -lImage
EXEC1 = mult.exe
EXEC2 = demult.exe

all: $(EXEC1) $(EXEC2)

$(EXEC1): $(OBJ1)
	$(CPP) $(OBJ1) $(LIBS) -o $(EXEC_DIR)/$(EXEC1)
	
$(EXEC2): $(OBJ2)
	$(CPP) $(OBJ2) $(LIBS) -o $(EXEC_DIR)/$(EXEC2)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $(SRC_DIR)/$*.cpp -o $(OBJ_DIR)/$*.o -I$(INCL_DIR) -I$(LIB_INCL_DIR)

clean: 
	rm $(OBJ_DIR)/*.o $(EXEC_DIR)/*.exe
