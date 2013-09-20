#---------------------------------------------------
# optimised #DEFINES for general use:
#---------------------------------------------------
DEFS=-O2 -fomit-frame-pointer -DWITH_LADSPA

# Previously the optimised #DEFINES had a -ffast-math but by using this 
# further calculation quality is sacrificed so i don't use it.

# remove -DWITH_LADSPA from the DEFS= line if you wish to remove the (meagre)
#                      LADSPA plugin support. but you'd be a fool.


#---------------------------------------------------
# development/debugging #DEFINES  (pretty ugly)
#---------------------------------------------------
# add -DSEQ_NOTE_DEBUG and/or -DNOTE_EDIT_DEBUG to DEFS to display
#   (might be broken) note_data info whenever one is created/deleted.
#
# add -DDEBUG_STRLIST_PAR to show what synthfileread::read_string_list_param
#						has encountered.
# add -DCRAZY_SAMPLER if you want to see far too much information.

#---------------------------------------------------
# new 1.26 development/debugging/statistics #DEFINES
#---------------------------------------------------
# DATA_STATS    = show counts of various data classes used

# NAME_CHECK    = checks that the inputnames/outputnames etc are created
#                 properly - that they're not created in the wrong order.

# CMDLINE_DBG   = displays random information while processing commandline.

# SANITY_CHECKS = checks that when inputs and params are set, the same values
#                 are returned by the get methods. Also checks that all the
#                 outputs created for a module can be accessed.

# DEBUG_STRLIST_PAR = displays strings read from file while processing the
#                     special parameter type used by the input_editor and
#                     param_editor data objects.

DEFS=-ggdb -fno-inline -DWITH_LADSPA -DNAME_CHECK -DDEBUG_MSG # -DDATA_STATS

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
