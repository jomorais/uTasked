CC = gcc
ROOT_DIR = $(PWD)
KERNEL_DIR = $(PWD)/kernel
PORT_DIR = $(PWD)/port
TEST_DIR = $(PWD)/test

INCLUDES += -I $(ROOT_DIR)
INCLUDES += -I $(KERNEL_DIR)
INCLUDES += -I $(PORT_DIR)
INCLUDES += -I $(TEST_DIR)


include .config

all: port.o kernel.o test.o
	$(CC) -o test.elf $(PORT_DIR)/port.o $(KERNEL_DIR)/kernel.o $(TEST_DIR)/test.o

port.o: $(PORT_DIR)/port.c
	$(CC) -c $(PORT_DIR)/port.c -o $(PORT_DIR)/port.o $(INCLUDES)

kernel.o: $(KERNEL_DIR)/kernel.c $(KERNEL_DIR)/kernel.h $(ROOT_DIR)/config.h $(PORT_DIR)/port.h
	$(CC) -c $(KERNEL_DIR)/kernel.c -o $(KERNEL_DIR)/kernel.o $(INCLUDES)

test.o: $(TEST_DIR)/test.c $(KERNEL_DIR)/*
	$(CC) -c $(TEST_DIR)/test.c -o $(TEST_DIR)/test.o $(INCLUDES)

menuconfig:
	kconfig-mconf $(ROOT_DIR)/KConfig
	python3 config_generator.py

clean:
	rm -rf $(KERNEL_DIR)/*.o
	rm -rf $(PORT_DIR)/*.o 
	rm -rf $(TEST_DIR)/*.o
	rm -rf test.elf