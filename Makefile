MAIN := main
OBJECTS := main.o Aluno.o GerenciadorArquivo.o Benchmark.o
CXXFLAGS := -Wall -Wextra -std=c++17 -pedantic-errors -O2
DEBUG := -g
MATH := -lm
CXX := g++

ifeq ($(OS), Windows_NT)
    DEL := del /Q /F
    OUTPUTMAIN := $(MAIN).exe
else
    DEL := rm -f
    OUTPUTMAIN := $(MAIN).out
endif

# Alvos falsos
.PHONY: all clean run

all: $(OUTPUTMAIN)

$(OUTPUTMAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(DEBUG) $^ -o $@ $(MATH)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@


clean:
	@$(DEL) $(OBJECTS) $(OUTPUTMAIN) *.dat

run: all
	./$(OUTPUTMAIN)
