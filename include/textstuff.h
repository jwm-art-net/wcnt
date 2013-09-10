#ifndef TEXTSTUFF_H
#define TEXTSTUFF_H

#include <string>

/*
// if collumnize is called with sort set to true, then it will sort
// the array of c-strings alphanumerically within each collumn such that
// top of the 2nd collumn follows on from the bottom of the 1st.
//
// it should be noted that when sort is used, the width of the collumns
// should be greater than the longest c-string in the array - otherwise it
// causes a great deal of nonsense of logic which must be avoided at all
// costs.
//
// if collumnize is called with sort set to false, any c-string whose
// width is greater than the collumn width and whose length would take
// it into a collumn greater than collumn count, then the row will be
// abandonded and the c-string will begin on the next row.

// delete the returned string when done with.
*/

std::string*
collumnize(const char** textarr,    unsigned int arrsize,
           unsigned int colwidth,   unsigned int colcount,  bool sort);

#endif