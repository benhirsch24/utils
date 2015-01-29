BIN = ./bin
UTILS = planetoppm

bin:
	mkdir -p ./bin

planetoppm: bin ./imgutils/planetoppm.c
	gcc -O3 ./imgutils/planetoppm.c -o $(BIN)/planetoppm

clean:
	rm $(BIN)/*

all : bin $(UTILS)

install: all
	cp $(BIN)/* ~/bin
