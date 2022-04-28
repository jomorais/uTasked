CC = gcc
ROOT_DIR = $(PWD)
KERNEL_DIR = $(PWD)/kernel
PORT_DIR = $(PWD)/port
TEST_DIR = $(PWD)/test
CFLAGS = -target arm64-apple-darwin21.4.0

INCLUDES += -I $(ROOT_DIR)
INCLUDES += -I $(KERNEL_DIR)
INCLUDES += -I $(PORT_DIR)
INCLUDES += -I $(TEST_DIR)



include .config
 


all: .config port.o kernel.o test.o
	$(CC)  -o test.elf $(PORT_DIR)/port.o $(KERNEL_DIR)/kernel.o $(TEST_DIR)/test.o

port.o: $(PORT_DIR)/port.c
	$(CC)  -c $(PORT_DIR)/port.c -o $(PORT_DIR)/port.o $(INCLUDES)

kernel.o: $(KERNEL_DIR)/kernel.c $(KERNEL_DIR)/kernel.h $(ROOT_DIR)/config.h $(PORT_DIR)/port.h
	$(CC)  -c $(KERNEL_DIR)/kernel.c -o $(KERNEL_DIR)/kernel.o $(INCLUDES)

test.o: $(TEST_DIR)/test.c $(KERNEL_DIR)/*
	$(CC)  -c $(TEST_DIR)/test.c -o $(TEST_DIR)/test.o $(INCLUDES)

unity_tests.o: $(TEST_DIR)/unity_tests.c $(KERNEL_DIR)/*
	$(CC) $(CFLAGS) -c $(TEST_DIR)/unity_tests.c -o $(TEST_DIR)/unity_tests.o $(INCLUDES)

unity.o: $(TEST_DIR)/unity.c $(TEST_DIR)/unity.h $(TEST_DIR)/unity_internals.h
	$(CC) $(CFLAGS) -c $(TEST_DIR)/unity.c -o $(TEST_DIR)/unity.o $(INCLUDES)	

unity_tests: .config port.o kernel.o unity_tests.o unity.o
	$(CC) $(CFLAGS) -o unity_tests.elf $(PORT_DIR)/port.o $(KERNEL_DIR)/kernel.o $(TEST_DIR)/unity.o $(TEST_DIR)/unity_tests.o

.config:
	@echo "THIS PROJECT IS NOT YET CONFIGURED"
	@echo "TO CREATE THE CONFIG.h FILE, TYPES: make menuconfig"


menuconfig:
	kconfig-mconf $(ROOT_DIR)/KConfig
	python3 config_generator.py

clean:
	rm -rf $(KERNEL_DIR)/*.o
	rm -rf $(PORT_DIR)/*.o 
	rm -rf $(TEST_DIR)/*.o
	rm -rf test.elf