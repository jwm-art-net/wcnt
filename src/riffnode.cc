#ifndef RIFFNODE_H
#include "../include/riffnode.h"

riff_node::riff_node(riffdata* rd, short barpos) 
: start_bar(barpos), riff_source(rd)
{
}

riff_node::~riff_node() 
{
}

#endif
