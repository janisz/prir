CC := color-gcc
CFLAGS := -Wall -pedantic -std=gnu99 
LFLAGS := -lpthread

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

#default value
ifeq ($(strip $(RUN_ARGS)),)
  RUN_ARGS := 9
endif

.PHONY: run compile all clean

all: compile

run: program
	./program $(RUN_ARGS)

compile: program

program: program.o jlib.o
	$(CC) -o program program.o jlib.o $(LFLAGS) 

program.o: program.c jlib.h
	$(CC) -c $(CFLAGS)  program.c -o program.o

jlib.o: jlib.c jlib.h
	$(CC) -c $(CFLAGS)  jlib.c -o jlib.o

style: clean
	astyle -A8 -t *.c *.h

zip: clean
	tar -jcvf ${USER}.tar.bz2 *.c *.h Makefile

clean:
	rm -f jlib.o program.o program $(USER).tar.bz2
