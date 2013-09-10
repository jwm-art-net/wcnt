#ifndef JWM_INIT_H
#define JWM_INIT_H

namespace jwm_init
{

const unsigned long default_samplerate          = 44100UL;
const unsigned short default_bpm                 = 100;
const unsigned short default_beats_per_measure   = 4;
const unsigned short default_beat_value          = 4;
const unsigned short note_array_size             = 10;
const unsigned short note_name_len               = 9;
const unsigned short filter_array_size           = 8192;
const unsigned short wav_buffer_size             = 4096;
const unsigned short max_anti_clip_size          = wav_buffer_size / 2;
const unsigned short max_anti_clip_samples       = max_anti_clip_size;
const unsigned short sr_div_bi                   = 8;

} /* namespace jwm_init */

#endif
