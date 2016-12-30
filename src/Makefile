COMPLILER=gcc
CFLAGS=-Wall -g

LIB_LUA=-llua
LIB_MATH=-lm
LIB_SDL2=-lSDL2
LIB_SDL2IMG=-lSDL2_image
LIB_SDL2TTF=-lSDL2_ttf

LIBS=$(LIB_LUA) $(LIB_MATH) $(LIB_SDL2) $(LIB_SDL2IMG) $(LIB_SDL2TTF)

OPTIONS=$(CFLAGS) $(LIBS)

C_FILES=$(sort $(wildcard *.c))
O_FILES=$(sort $(patsubst %.c,%.o,$(C_FILES)))

all: ainur

%.o: %.c
	@printf "Compiling %s..." $<
	@$(COMPLILER) -c -o $@ $< $(OPTIONS)
	@printf "\t\t...Done\n"

ainur: $(O_FILES)
	@printf "Compiling %s..." $@
	@$(COMPLILER) -o $@ $^ $(OPTIONS)
	@printf "\t\t...Done\n"

clean:
	rm -f *.o ainur
