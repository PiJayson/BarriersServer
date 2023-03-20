BIN=bin
SRC=src
OUTPUT=output


all: $(BIN)/init_barrier.x $(BIN)/destroy_barrier.x $(BIN)/wait.x $(BIN)/series.x $(BIN)/errno.x $(BIN)/sigusr.x


$(BIN)/init_barrier.x: $(SRC)/init_barrier.c
	cc $? -o $@

$(BIN)/destroy_barrier.x: $(SRC)/destroy_barrier.c
	cc $? -o $@

$(BIN)/wait.x: $(SRC)/wait.c
	cc $? -o $@

$(BIN)/series.x: $(SRC)/series.c
	cc $? -o $@

$(BIN)/errno.x: $(SRC)/errno.c
	cc $? -o $@

$(BIN)/sigusr.x: $(SRC)/sigusr.c
	cc $? -o $@

clean:
	rm $(BIN)/* $(OUTPUT)/*
