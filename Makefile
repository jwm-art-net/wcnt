#---------------------------------------------------
# optimised #DEFINES for general use:
#---------------------------------------------------
DEFS=-O2 -fomit-frame-pointer -DWITH_LADSPA

# Previously the optimised #DEFINES had a -ffast-math but by using this 
# further calculation quality is sacrificed so i don't use it.

# remove -DWITH_LADSPA from the DEFS= line if you wish to remove the (meagre)
#                      LADSPA plugin support. but you'd be a fool.


#---------------------------------------------------
# development/debugging #DEFINES 
#---------------------------------------------------
# DEBUG		= show various information supposedly to aid debugging.
# DATA_STATS    = show counts of the construction/destruction of select
# 		  classes.
# CRAZY_SAMPLER = to see far too much infomation concerning the sampler.
#
# NOTE_EDIT_DEBUG=display note_data debugging info.
# SANITY_CHECKS = Some sort of half assed check for program sanity.


#DEFS=-ggdb -fno-inline -DWITH_LADSPA -DDEBUG -DSTR_DEBUG #-DSANITY_CHECKS

# nothing to be modified further:
PROG=wcnt
WARNS=-Wall -Wextra
CFLAGS=$(DEFS) $(WARNS) `pkg-config sndfile --cflags`
LIBS=`pkg-config sndfile --libs`
SRC=$(wildcard src/*.cc)
OBJS=$(SRC:.cc=.o)
HEADERS=$(wildcard include/*.h)
EXAMPLES=$(wildcard examples/*.wc)
WAVS=$(EXAMPLES:.wc=.wav)

$(PROG) : $(OBJS)
	@echo Linking $(PROG)
	@$(CXX) $(CFLAGS) $(OBJS) $(LIBS) -ldl -o $(PROG)
# uncomment line below and comment out line above to use ccmalloc 
# memory profiler in the linking process... but i prefer to use valgrind
#	@ccmalloc $(CXX) $(CFLAGS) $(OBJS) $(LIBS) -ldl -o $(PROG)
	@echo $(PROG) was compiled with:
	@echo $(CXX) $(CFLAGS)

%.o : %.cc $(HEADERS)
	@echo Compiling $(<F)
	@$(CXX) $(CFLAGS) -c $< -o $@

main.o: main.c $(HEADERS)

examples: $(WAVS)

%.wav :  %.wc $(PROG)
	@./$(PROG) --no-title $<

clean:
	@rm -vf $(OBJS) $(PROG)

clean_examples:

	@rm -vf $(WAVS)

.PHONEY: examples clean clean_examples
