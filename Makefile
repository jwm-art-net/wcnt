#optimised defs
DEFS=-O2 -fomit-frame-pointer -ffast-math

#debugging defs
#DEFS=-g3 -fno-inline

# the rest:
PROG=wcnt
WARNS=-Wall -ansi -pedantic -D_GNU_SOURCE
CFLAGS=$(DEFS) $(WARNS)
SRC=$(wildcard src/*.cc)
OBJS=$(SRC:.cc=.o)
HEADERS=$(wildcard include/*.h)
EXDIR=examples
EXAMPLES=$(wildcard $(EXDIR)/*.wc)
WAVS=$(EXAMPLES:.wc=.wav)

$(PROG) : $(OBJS)
	@echo Linking $(PROG)
	@$(CXX) $(CFLAGS) $(OBJS) -o $(PROG)
	@echo $(PROG) was compiled with:
	@echo $(CXX) $(CFLAGS)

%.o : %.cc $(HEADERS)
	@echo Compiling $(<F)
	@$(CXX) $(CFLAGS) -c $< -o $@

main.o: main.c $(HEADERS)

examples: $(WAVS)

%.wav : %.wc $(PROG)
	@./$(PROG) $<

clean:
	@rm -vf $(OBJS) $(PROG)

clean_examples:

	@rm -vf $(WAVS)

.PHONEY: examples clean clean_examples
