#optimised defs
DEFS=-O2 -fomit-frame-pointer -ffast-math

# development&debugging DEFS:
# add -DSEQ_NOTE_DEBUG and/or -DNOTE_EDIT_DEBUG to DEFS to display
#			note_data info whenever one is created/deleted.
# add -DSHOW_LL_ITEM_COUNT to show counts of ll_item (linkedlist items)
# 						created and destroyed.
# add -DSHOW_DOBJ_COUNT to show counts of dobj (data objects) created
#							and destroyed.
# add -DSHOW_DOBJ_NAMES to show names of data objects created/destroyed.
#					(a lot less verbose than wcnt -v)
# add -DSHOW_NOTE_COUNT to show a count of notes created/destroyed
# add -DDEBUG_STRLIST_PAR to show what synthfileread::read_string_list_param
#						has encountered.
# add -DCRAZY_SAMPLER if you want to see far too much information.

# general debugging defs
# DEFS=-g3 -fno-inline -DSHOW_LL_ITEM_COUNT -DSHOW_DOBJ_COUNT

# the rest:
PROG=wcnt
WARNS=-Wall -ansi -pedantic -D_GNU_SOURCE
CFLAGS=$(DEFS) $(WARNS)
SRC=$(wildcard src/*.cc)
OBJS=$(SRC:.cc=.o)
HEADERS=$(wildcard include/*.h)
EXAMPLES=$(wildcard examples/*.wc)
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

clean:
	@rm -vf $(OBJS) $(PROG)

clean_examples:

	@rm -vf $(WAVS)

.PHONEY: examples clean clean_examples
