IMG = ./imgutils
BIN = ./bin
UTILS = planetoppm colorconv ppmheader

bin:
	mkdir -p ./bin

planetoppm: bin $(IMG)/planetoppm.c
	gcc -O3 $(IMG)/planetoppm.c -o $(BIN)/planetoppm

colorconv: bin $(IMG)/colorconv.c
	gcc -O3 $(IMG)/colorconv.c -o $(BIN)/colorconv

ppmheader: bin $(IMG)/ppmheader.c
	gcc -O3 $(IMG)/ppmheader.c -o $(BIN)/ppmheader

clean:
	rm $(BIN)/*

all : bin $(UTILS)

install: all
	cp $(BIN)/* ~/bin
