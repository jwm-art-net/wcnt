#ifndef JWM_INIT_H
#define JWM_INIT_H

namespace jwm_init
{

const samp_t default_samplerate          = 44100UL;
const wcint_t min_bpm                    = 10;
const wcint_t max_bpm                    = 1000;
const wcint_t default_beats_per_measure  = 4;
const wcint_t default_beat_value         = 4;
const wcint_t note_array_size            = 10;
const wcint_t note_name_len              = 9;
const wcint_t filter_array_size          = 8192;
const wcint_t wav_buffer_size            = 4096; //16384;
const wcint_t max_anti_clip_size         = wav_buffer_size / 2;
const wcint_t max_anti_clip_samples      = max_anti_clip_size;
const wcint_t sr_div_bi                  = 8;

const char* const ladspa_path_if_env_not_set =
    "/usr/local/lib/ladspa/:/usr/lib/ladspa/";

const wcint_t group_control_stopping_samples = 1;

} /* namespace jwm_init */

#endif
