#optimised defs
DEFS=-O2 -fomit-frame-pointer -ffast-math

#debugging defs
# add -DSEQ_NOTE_DEBUG to DEFS to display note_data
# info whenever one is created/deleted.
# DEFS=-g3 -fno-inline

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
# uncomment line below and comment out line above to use ccmalloc 
# memory profiler in the linking process...
#	@ccmalloc $(CXX) $(CFLAGS) $(OBJS) -o $(PROG)
	@echo $(PROG) was compiled with:
	@echo $(CXX) $(CFLAGS)

%.o : %.cc $(HEADERS)
	@echo Compiling $(<F)
	@$(CXX) $(CFLAGS) -c $< -o $@

main.o: main.c $(HEADERS)

examples: $(WAVS)

%.wav :  %.wc $(PROG)
	@./$(PROG) $<

# uncommenting this causes make examples to use valgrind too.
# (be warned, it takes hours...)
# valgrind: $(WAVS)
#
# %.wav :  %.wc $(PROG)
#	valgrind --leak-check=yes --log-file=wcnt-valgrind $(PROG) $<
#

clean:
	@rm -vf $(OBJS) $(PROG)

clean_examples:

	@rm -vf $(WAVS)

.PHONEY: examples clean clean_examples
