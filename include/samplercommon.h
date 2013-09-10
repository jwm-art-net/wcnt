#ifndef SAMPLERCOMMON_H
#define SAMPLERCOMMON_H

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
