#ifndef SAMPLERCOMMON_H
#define SAMPLERCOMMON_H

#include "wavfileheader.h" // to get WAV_BUFFER_SIZE
#include "wavfilein.h"

// max size of anti clip buffer
#define MAX_ANTI_CLIP_SIZE (WAV_BUFFER_SIZE / 2)
// max anti_clip samples specifiable by user. (depreciated)
#define MAX_ANTI_CLIP_SAMPLES MAX_ANTI_CLIP_SIZE
// max search range specifiable by user. (unimplimentated)
#define MAX_SEARCH_RANGE (WAV_BUFFER_SIZE / 2)
#define SR_DIV_BI 8
// SR_DIV_BI - divide search range by n.  tells the sampler how many
// samples to search for a match closest to zero when calculating the
// bi-directional offset from the current position when changing
// direction. result should be pretty small because the user specifies 
// the offset from the loop point.  The target is to jump half a phase 
// in the waveform so that continuity of alternating positive and 
// negative waveforms is not lost.

// play forward or reverse
enum PLAY_DIR{ PLAY_FWD, PLAY_REV};

// when loop mode is on, in some circumstances the start position
// and direction will lead to the end or the begining of the wav
// before getting to the loop points, what to do?
enum PLAY_MODE{ PLAY_STOP, PLAY_WRAP, PLAY_BOUNCE, PLAY_JUMP };
// stop - stop playing.
// wrap - to start or end of wav.
// bounce - change direction.
// jump - jump to loop.

// how to jump?
enum JUMP_DIR{ JUMP_PLAY_DIR, JUMP_LOOP_DIR };
// play_dir - start loop with initial play dir before adoptiong loop mode
// loop_dir - start loop with loop mode if loop_bi 
// continue with current dir.

// loop modes:
enum LOOP_MODE{ LOOP_OFF, LOOP_FWD, LOOP_REV, LOOP_BI };

#endif
