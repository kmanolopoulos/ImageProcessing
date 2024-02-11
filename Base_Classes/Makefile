# Makefile for Image Clustering Project
CPP  = g++.exe
CPPFLAGS = -Wno-deprecated
AR = ar
ARFLAGS = -rcs

SRC_DIR = src
INCL_DIR = include
OBJ_DIR = obj
LIB_DIR = lib

OBJ  = $(OBJ_DIR)/BwImage.o $(OBJ_DIR)/GrayImage.o $(OBJ_DIR)/ColorImage.o $(OBJ_DIR)/Filter.o $(OBJ_DIR)/Fourier.o $(OBJ_DIR)/Complex.o
LIB = libImage.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) $(ARFLAGS) $(LIB_DIR)/$(LIB) $(OBJ)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c $(SRC_DIR)/$*.cpp -o $(OBJ_DIR)/$*.o -I$(INCL_DIR)

clean: 
	rm $(OBJ_DIR)/*.o $(LIB_DIR)/*.a
