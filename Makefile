CC?=gcc

SRC=src
INC=include
LIB=lib
OBJS=$(SRC)/*.o

INSTALLINC=/usr/include
INSTALLLIB=/lib
INSTALLBIN=/usr/bin

librwelf:
	$(CC) -fPIC -g -c -Wall -pedantic -I$(INC)/ -o$(SRC)/elf.o $(SRC)/elf.c
	$(CC) -fPIC -g -c -Wall -pedantic -I$(INC)/ -o$(SRC)/ehdr.o $(SRC)/ehdr.c
	$(CC) -fPIC -g -c -Wall -pedantic -I$(INC)/ -o$(SRC)/shdr.o $(SRC)/shdr.c
	$(CC) -fPIC -g -c -Wall -pedantic -I$(INC)/ -o$(SRC)/sym.o $(SRC)/sym.c
	$(CC) -fPIC -g -c -Wall -pedantic -I$(INC)/ -o$(SRC)/phdr.o $(SRC)/phdr.c
	$(CC) -fPIC -g -c -Wall -pedantic -I$(INC)/ -o$(SRC)/dyn.o $(SRC)/dyn.c
	$(CC) -fPIC -g -c -Wall -pedantic -I$(INC)/ -o$(SRC)/rela.o $(SRC)/rela.c
	$(CC) -shared -Wl,-soname,$(LIB)/librwelf.so.0 -o$(LIB)/librwelf.so.0.1.0 $(OBJS)
	ln -sf librwelf.so.0.1.0 $(LIB)/librwelf.so.0
	ln -sf librwelf.so.0.1.0 $(LIB)/librwelf.so

	$(CC) -orwelf -I$(INC)/ $(SRC)/rwelf/main.c -L$(LIB)/ -lrwelf

clean:
	rm -rf rwelf $(LIB)/librwelf.so* $(SRC)/*.o

install:
	cp $(LIB)/* $(INSTALLLIB)
	ln -sf librwelf.so.0.1.0 $(INSTALLLIB)/librwelf.so.0
	ln -sf librwelf.so.0.1.0 $(INSTALLLIB)/librwelf.so
	cp $(INC)/* $(INSTALLINC)

	$(CC) -o$(INSTALLBIN)/rwelf -I$(INSTALLINC)/ $(SRC)/rwelf/main.c -L$(INSTALLLIB)/ -lrwelf