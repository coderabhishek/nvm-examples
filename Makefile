TARGET = example
COMPILER = gcc
SOURCE = file1_persistent.c
All:
	$(COMPILER) $(SOURCE) -lpmemobj -o $(TARGET)
CLEAN:
	rm $(TARGET)	
