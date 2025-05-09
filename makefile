CC = clang++

 FLAGS =-O3 -mavx2 #-D _DEBUG -ggdb3 -Wall -Wextra  -Waggressive-loop-optimizations \
 	   	#-Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
 		-Wconversion -Wempty-body -Wfloat-equal \
 		-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline \
 		-Wlogical-op -Wopenmp-simd -Wpacked \
 		-Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion \
 		-Wstrict-overflow=2 -Wsuggest-attribute=noreturn \
 		-Wsuggest-final-methods -Wsuggest-final-types -Wswitch-default \
 		-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused \
 		-Wvariadic-macros -Wno-missing-field-initializers -Wno-narrowing \
 		-Wno-varargs -Wstack-protector \
 		-fstack-protector -fstrict-overflow -flto -fno-omit-frame-pointer \
 		-pie -fPIE -Werror=vla \

CFLAGS = -c $(FLAGS)
LDFLAGS = $(FLAGS) -lm

SOURCES_LIST = main.cpp graphics.cpp mandelbrot_alg.cpp

SOURCES = $(SOURCES_LIST:%=src/%)
OBJECTS = $(SOURCES_LIST:%.cpp=build/%.o)
DEPS = $(OBJECTS:%.o=%.d)
EXECUTABLE = build/test

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(LDFLAGS) $^ -o $@ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

-include $(DEPS)

build/%.o: src/%.cpp
	@$(CC) -I./include $(CFLAGS) -MMD -MP $< -o $@

clean:
	rm -f build/*.o $(EXECUTABLE)
