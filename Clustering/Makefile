# Makefile for Image Clustering Project
CPP  = g++.exe
CPPFLAGS = -Wno-deprecated

SRC_DIR = src
INCL_DIR = include
OBJ_DIR = obj
LIB_DIR = ../Base_Classes/lib
LIB_INCL_DIR = ../Base_Classes/include
EXEC_DIR = executable

OBJ  = $(OBJ_DIR)/cluster.o
LIBS = -L$(LIB_DIR) -lImage
EXEC = cluster.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CPP) $(OBJ) $(LIBS) -o $(EXEC_DIR)/$(EXEC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $(SRC_DIR)/$*.cpp -o $(OBJ_DIR)/$*.o -I$(INCL_DIR) -I$(LIB_INCL_DIR)

clean: 
	rm $(OBJ_DIR)/*.o $(EXEC_DIR)/*.exe
