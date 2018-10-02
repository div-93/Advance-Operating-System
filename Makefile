.SILENT:

MODULE := NODE

CC := gcc
AR := ar
INC := ./inc/

CFLAGS := -g -c -I $(INC)
EFLAGS := -fno-stack-protector -lpthread -fno-common

BASE_DIR = .

SRC_DIR = ./src

OBJ_DIR = ./obj

OBJS = NodeMain.o\
       NodeCommon.o\
       NodeServ.o\
       NodeClnt.o\
       NodeCli.o\
       NodeApi.o\
       Node.o

all : $(OBJ_DIR)/NodeMain.o\
      $(OBJ_DIR)/NodeCommon.o\
      $(OBJ_DIR)/NodeServ.o\
      $(OBJ_DIR)/NodeClnt.o\
      $(OBJ_DIR)/NodeCli.o\
      $(OBJ_DIR)/NodeApi.o\
      $(BASE_DIR)/Node.o

$(OBJ_DIR)/NodeMain.o : $(SRC_DIR)/NodeMain.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(EFLAGS)

$(OBJ_DIR)/NodeCommon.o : $(SRC_DIR)/NodeCommon.c
	$(CC) $(CFLAGS) $< -o $@ $(EFLAGS)

$(OBJ_DIR)/NodeServ.o : $(SRC_DIR)/NodeServ.c
	$(CC) $(CFLAGS) $< -o $@ $(EFLAGS)

$(OBJ_DIR)/NodeClnt.o : $(SRC_DIR)/NodeClnt.c
	$(CC) $(CFLAGS) $< -o $@ $(EFLAGS)

$(OBJ_DIR)/NodeCli.o : $(SRC_DIR)/NodeCli.c
	$(CC) $(CFLAGS) $< -o $@ $(EFLAGS)

$(OBJ_DIR)/NodeApi.o : $(SRC_DIR)/NodeApi.c
	$(CC) $(CFLAGS) $< -o $@ $(EFLAGS)


$(BASE_DIR)/Node.o :
	$(CC) -g\
        $(OBJ_DIR)/NodeMain.o\
        $(OBJ_DIR)/NodeServ.o\
        $(OBJ_DIR)/NodeClnt.o\
        $(OBJ_DIR)/NodeCommon.o\
        $(OBJ_DIR)/NodeCli.o\
        $(OBJ_DIR)/NodeApi.o\
        -o $(BASE_DIR)/Node.o $(EFLAGS)

clean :
	rm -rf $(OBJ_DIR)
	rm $(BASE_DIR)/Node.o
