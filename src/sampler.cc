#include "../include/sampler.h"
#include "../include/wavfilein.h"
#include "../include/jwm_globals.h"
#include "../include/modoutputlist.h"
#include "../include/modinputlist.h"
#include "../include/modparamlist.h"
#include "../include/fxsparamlist.h"
#include "../include/jwm_init.h"

#include <iostream>

sampler::sampler(char const* uname) :
 synthmod(synthmodnames::SAMPLER, uname, SM_HAS_STEREO_OUTPUT),
 in_play_trig(0), in_stop_trig(0), in_start_pos_mod(0), in_phase_step(0),
 out_left(0), out_right(0),
 out_loop_trig(OFF), play_state(OFF),
 wavfile(0),
 play_dir(PLAY_FWD), play_mode(PLAY_STOP), jump_mode(JUMP_PLAY_DIR),
 min_start_pos(0), max_start_pos(0),
 loop_begin(0), loop_end(0), loop_is_offset(OFF), loop_mode(LOOP_OFF),
 loop_bi_offset(0),
 anti_clip_size(0), ac_each_end(OFF), search_range(0),
 phase_step_amount(0), root_phase_step(1.0),
 playdir(PLAY_FWD), acplaydir(PLAY_FWD), loop_yet(false),
 mono_buffer(0), ac_m_buf(0), st_buffer(0), ac_st_buf(0),
 buffer_start_pos(0), buff_pos(0),
 wavstart(1),  wavlength(0), wavstbi(0), wavlenbi(0),
 cur_pos(0), ac_cpstep(0), oldcpstep(0), cp_step(0), cp_ratio(0),
 sr_ratio(1), bp_midpoint(0), start_pos(0), startpos_span(0),
 loopstart(0), loopfinish(0), loop_fits_in_buffer(0),
 loop_loaded(0),
 do_ac(OFF), ac_cur_pos(0), ac_midpoint(0), 
 ac_buf_pos(0), ac_buf_start_pos(0), ac_step(0), ac_size(0),
 ac_out_left(0), ac_out_right(0),
 ch(WAV_CH_UNKNOWN)
{
    add_output(outputnames::OUT_LEFT);
    add_output(outputnames::OUT_RIGHT);
    add_output(outputnames::OUT_LOOP_TRIG);
    add_output(outputnames::OUT_PLAY_STATE);
    add_input(inputnames::IN_PLAY_TRIG);
    add_input(inputnames::IN_STOP_TRIG);
    add_input(inputnames::IN_START_POS_MOD);
    add_input(inputnames::IN_PHASE_STEP);
    create_params();
    sampletot = 0;
}

sampler::~sampler()
{
    if (mono_buffer)
        delete [] mono_buffer;
    if (st_buffer)
        delete [] st_buffer;
    if (ac_m_buf)
        delete [] ac_m_buf;
    if (ac_st_buf)
        delete [] ac_st_buf;
}

void const* sampler::get_out(outputnames::OUT_TYPE ot) const
{
    switch(ot)
    {
        case outputnames::OUT_LEFT:         return &out_left;
        case outputnames::OUT_RIGHT:        return &out_right;
        case outputnames::OUT_LOOP_TRIG:    return &out_loop_trig;
        case outputnames::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

void const* sampler::set_in(inputnames::IN_TYPE it, void const* o)
{
    switch(it)
    {
        case inputnames::IN_PLAY_TRIG:
            return in_play_trig = (STATUS*)o;
        case inputnames::IN_STOP_TRIG:
            return in_stop_trig = (STATUS*)o;
        case inputnames::IN_PHASE_STEP:
            return in_phase_step = (double*)o;
        case inputnames::IN_START_POS_MOD:
            return in_start_pos_mod = (double*)o;
        default:
            return 0;
    }
}

void const* sampler::get_in(inputnames::IN_TYPE it) const
{
    switch(it)
    {
        case inputnames::IN_PLAY_TRIG:      return in_play_trig;
        case inputnames::IN_STOP_TRIG:      return in_stop_trig;
        case inputnames::IN_PHASE_STEP:     return in_phase_step;
        case inputnames::IN_START_POS_MOD:  return in_start_pos_mod;
        default: return 0;
    }
}

bool sampler::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    switch(pt)
    {
    case paramnames::WAVFILEIN:
        if (((dobj*)data)->get_object_type() != dobjnames::DEF_WAVFILEIN)
        {
            *err_msg = "\n" + *((dobj*)data)->get_username();
            *err_msg += " is not a wavfilein";
            return false;
        }
        wavfile = (wavfilein*)data; // note: pointer.
        return true;
    case paramnames::PLAY_DIR:
        play_dir = (PLAY_DIR)(*(int*)data);
        return true;
    case paramnames::PLAY_MODE:
        play_mode = (PLAY_MODE)(*(int*)data);
        return true;
    case paramnames::JUMP_MODE:
        jump_mode = (JUMP_DIR)(*(int*)data);
        return true;
    case paramnames::START_POS_MIN:
        min_start_pos = *(unsigned long*)data;
        return true;
    case paramnames::START_POS_MAX:
        max_start_pos = *(unsigned long*)data;
        return true;
    case paramnames::LOOP_BEGIN:
        loop_begin = *(unsigned long*)data;
        return true;
    case paramnames::LOOP_END:
        loop_end = *(unsigned long*)data;
        return true;
    case paramnames::LOOP_MODE:
        loop_mode = (LOOP_MODE)(*(int*)data);
        return true;
    case paramnames::LOOP_IS_OFFSET:
        loop_is_offset = *(STATUS*)data;
        return true;
    case paramnames::LOOP_BI_OFFSET:
        loop_bi_offset = *(short*)data;
        return true;
    case paramnames::ANTI_CLIP:
        anti_clip_size = *(short*)data;
        return true;
    case paramnames::AC_EACH_END:
        ac_each_end = *(STATUS*)data;
        return true;
    case paramnames::ZERO_SEARCH_RANGE:
        search_range = *(short*)data;
        if(search_range == 1) search_range++;
        return true;
    case paramnames::PHASE_STEP_AMOUNT:
        phase_step_amount = *(double*)data;
        return true;
    default:
        return false;
    }
}

void const* sampler::get_param(paramnames::PAR_TYPE pt) const
{
    switch(pt)
    {
        case paramnames::WAVFILEIN:         return wavfile;
        case paramnames::PLAY_DIR:          return &play_dir;
        case paramnames::PLAY_MODE:         return &play_mode;
        case paramnames::JUMP_MODE:         return &jump_mode;
        case paramnames::START_POS_MIN:     return &min_start_pos;
        case paramnames::START_POS_MAX:     return &max_start_pos;
        case paramnames::LOOP_BEGIN:        return &loop_begin;
        case paramnames::LOOP_END:          return &loop_end;
        case paramnames::LOOP_MODE:         return &loop_mode;
        case paramnames::LOOP_IS_OFFSET:    return &loop_is_offset;
        case paramnames::LOOP_BI_OFFSET:    return &loop_bi_offset;
        case paramnames::ANTI_CLIP:         return &anti_clip_size;
        case paramnames::AC_EACH_END:       return &ac_each_end;
        case paramnames::ZERO_SEARCH_RANGE: return &search_range;
        case paramnames::PHASE_STEP_AMOUNT: return &phase_step_amount;
        default: return 0;
    }
}

stockerrs::ERR_TYPE sampler::validate()
{
    WAV_STATUS wavstatus = wavfile->open_wav();
    if (wavstatus == WAV_STATUS_NOT_FOUND) {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::FILENAME);
        *err_msg += ", using wavfilein ";
        *err_msg += wavfile->get_username();
        *err_msg += " the file ";
        *err_msg += wavfile->get_filename();
        *err_msg += " was not found.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (wavstatus == WAV_STATUS_WAVERR) {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::FILENAME);
        *err_msg += ", using wavfilein ";
        *err_msg += wavfile->get_username();
        *err_msg += " the file ";
        *err_msg += wavfile->get_filename();
        *err_msg += " is not a wav file.";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (wavstatus != WAV_STATUS_OPEN) {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::FILENAME);
        *err_msg += ", using wavfilein ";
        *err_msg += wavfile->get_username();
        *err_msg = ", an unspecified error occurred trying to open ";
        *err_msg += wavfile->get_filename();
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    modparamlist* pl = jwm.get_paramlist();
    if (!pl->validate(this, paramnames::START_POS_MIN,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::START_POS_MIN);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::START_POS_MAX,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::START_POS_MAX);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (max_start_pos < min_start_pos) {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::START_POS_MAX);
        *err_msg += " must not be less than ";
        *err_msg +=
         jwm.get_paramnames()->get_name(paramnames::START_POS_MIN);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (loop_is_offset == OFF) {
        if (!pl->validate(this, paramnames::LOOP_BEGIN,
                stockerrs::ERR_NEGATIVE))
        {
            *err_msg =
             jwm.get_paramnames()->get_name(paramnames::LOOP_BEGIN);
            *err_msg += " of absolute value";
            invalidate();
            return stockerrs::ERR_NEGATIVE;
        }
        if (!pl->validate(this, paramnames::LOOP_END,
                stockerrs::ERR_NEGATIVE))
        {
            *err_msg =
             jwm.get_paramnames()->get_name(paramnames::LOOP_END);
            *err_msg += " of absolute value";
            invalidate();
            return stockerrs::ERR_NEGATIVE;
        }
    }
    if (loop_end <= loop_begin) {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::LOOP_END);
        *err_msg += " must be more than ";
        *err_msg +=
            jwm.get_paramnames()->get_name(paramnames::LOOP_BEGIN);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (anti_clip_size < 0
        || anti_clip_size > jwm_init::max_anti_clip_samples)
    {
        *err_msg = jwm.get_paramnames()->get_name(paramnames::ANTI_CLIP);
        *err_msg += " out of range 0 ~ 2048";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!pl->validate(this, paramnames::ZERO_SEARCH_RANGE,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         jwm.get_paramnames()->get_name(paramnames::ZERO_SEARCH_RANGE);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void sampler::init()
{
    if (wavfile->get_status() == WAV_STATUS_OPEN)
    {
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username() << "::init() WAV_STATUS_OPEN";
        #endif
        ch = wavfile->get_channel_status();
        wavlength = wavfile->get_length() - 2;
        sr_ratio = (double)wavfile->get_sample_rate() / jwm.samplerate();
        if (ch == WAV_CH_MONO) {
            mono_buffer = new double[jwm_init::wav_buffer_size];
            if (!mono_buffer) {
                invalidate();
                return;
            }
            for (int i = 0; i < jwm_init::wav_buffer_size; i++)
                mono_buffer[i] = 0;
            ac_m_buf = new double[jwm_init::max_anti_clip_size];
            if (!ac_m_buf) {
                invalidate();
                return;
            }
            for (int i = 0; i < jwm_init::max_anti_clip_size; i++)
                ac_m_buf[i] = 0;
        }
        else {
            st_buffer = new st_data[jwm_init::wav_buffer_size];
            if (!st_buffer) {
                invalidate();
                return;
            }
            for (int i = 0; i < jwm_init::wav_buffer_size; i++)
            {
                st_buffer[i].left = 0;
                st_buffer[i].right = 0;
            }
            ac_st_buf = new st_data[jwm_init::max_anti_clip_size];
            if (!ac_st_buf) {
                invalidate();
                return;
            }
            for (int i = 0; i < jwm_init::max_anti_clip_size; i++) {
                ac_st_buf[i].left = 0;
                ac_st_buf[i].right = 0;
            }
        }
    } 
    else {
        std::cout << "\nsampler "<< get_username();
        std::cout << " has not got an open wav. expect a sigsegv!";
    }
    root_phase_step = wavfile->get_root_phase_step();
 // do a whole load of checks on the users input.
    int halfsr = search_range / 2;
    if (ac_each_end == ON) {
        wavstart = zero_search(anti_clip_size + halfsr, search_range);
        wavlength
         = zero_search(wavlength-anti_clip_size-halfsr,search_range);
    } else {
        wavstart = zero_search(halfsr, search_range);
        wavlength = zero_search(wavlength - halfsr, search_range);
    }
    int bisr = search_range / jwm_init::sr_div_bi;
    if (search_range > 0 && bisr < 2) bisr = 2;
    wavstbi = zero_search(wavstart + loop_bi_offset, bisr);
    wavlenbi = zero_search(wavlength - loop_bi_offset, bisr);
    if (min_start_pos - anti_clip_size < 0)
        min_start_pos += anti_clip_size;
    if (max_start_pos > wavlength)  max_start_pos = wavlength;
    startpos_span = max_start_pos - min_start_pos;
    start_pos = 4294967295ul;
    if (loop_mode != LOOP_OFF) {
        if (anti_clip_size > 0) {
            if (loop_is_offset == OFF) {
                if (loop_begin < wavstart) loop_begin += anti_clip_size;
                if (loop_end > wavlength) loop_end -= anti_clip_size;
            }
        }
        if (loop_is_offset == OFF) {
            loop_begin = zero_search(loop_begin, search_range);
            loop_end = zero_search(loop_end, search_range);
        }
        int loopsize = ((loop_end >= 0) ? loop_end : -loop_end) -
                       ((loop_begin >= 0) ? loop_begin : -loop_begin);
        if (loopsize < jwm_init::wav_buffer_size - 1)
            loop_fits_in_buffer = 1;
        else loop_fits_in_buffer = 0;
        loop_loaded = 0;
        if (loopsize != 0 && loopsize < anti_clip_size)
            anti_clip_size=(short)(loopsize / 2.1);
    }
    if (anti_clip_size > 0)	ac_step = 1.00 / (double)anti_clip_size;
    do_ac = OFF;
}

// read data from wav file into buffer
inline void sampler::fill_buffer(unsigned long pos)
{
    if (ch == WAV_CH_MONO)
        wavfile->read_wav_at(mono_buffer, pos);
    else
        wavfile->read_wav_at(st_buffer, pos);
}

// calculate value of point r (0~1) between sample A and sample B
inline double sampler::calc_midpoint(double a, double b, double r) {
    return a + (b - a) * r;
}

void sampler::run()
{
    if (*in_stop_trig == ON) {
        out_left = out_right = 0;
        play_state = OFF;
        do_ac = OFF;
    }
    if (*in_play_trig == ON) {
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username() << "::run *in_play_trig == ON";
        #endif
        trigger_playback();
    }
    if (play_state == ON) {
        if (do_ac == OFF) oldcpstep = cp_step;
        if (out_loop_trig == ON) out_loop_trig = OFF;
        cp_ratio = *in_phase_step / root_phase_step;
        //sample rate ratio is sr_ratio
        cp_step = (1 + (cp_ratio - 1) * phase_step_amount) * sr_ratio;
        buff_pos = (unsigned int)cur_pos - buffer_start_pos;
        #ifdef CRAZY_SAMPLER
        if (buff_pos < 0 || buff_pos >= jwm_init::wav_buffer_size) {
            std::cout << "\n" << get_username() << "::run buff_pos = cur_pos(";
            std::cout << (double)cur_pos << ") - ";
            std::cout << "buffer_start_pos(" << buffer_start_pos << ")";
            std::cout << " = " << buff_pos;
        }
        #endif
        if (playdir == PLAY_FWD) {
            if (cur_pos >= wavlength) {
                pos_wavlen();
                buff_pos = (unsigned int)cur_pos - buffer_start_pos;
                #ifdef CRAZY_SAMPLER
                if (buff_pos < 0 || buff_pos >= jwm_init::wav_buffer_size)
                {
                    std::cout << "\n" << get_username() << "::run ";
                    std::cout << "(playdir==PLAY_FWD && cur_pos>=wavlength";
                    std::cout << " buff_pos = cur_pos(" << cur_pos << ") - ";
                    std::cout << "buffer_start_pos(" << buffer_start_pos;
                    std::cout << ")";
                    std::cout << " = " << buff_pos;
                }
                #endif
            }
            else if (loop_yet && cur_pos > loopfinish) {
                pos_loopend();
                buff_pos = (unsigned int)cur_pos - buffer_start_pos;
                #ifdef CRAZY_SAMPLER
                std::cout << "\n" << get_username() << "::run ";
                std::cout << "(playdir == PLAY_FWD && loopyet && cur_pos";
                std::cout << " > loopfinish >";
                std::cout << "buff_pos = cur_pos(" << cur_pos << ") - ";
                std::cout << "buffer_start_pos(" << buffer_start_pos << ")";
                std::cout << " = " << buff_pos;
                #endif
            }
            if (buff_pos >= jwm_init::wav_buffer_size - 1) {
                buffer_start_pos = (unsigned long)cur_pos;
                fill_buffer(buffer_start_pos);
                buff_pos = 0;
            }
        } else { // playdir == PLAY_REV
            if (cur_pos <= wavstart) {
                pos_wavstart();
                buff_pos = (unsigned int)cur_pos - buffer_start_pos;
            }
            else if (loop_yet && cur_pos < loopstart) {
                pos_loopbegin();
                buff_pos = (unsigned int)cur_pos - buffer_start_pos;
            }
            if (buff_pos <= 0) {
                if (cur_pos - (jwm_init::wav_buffer_size - 2) >= 0) {
                    buffer_start_pos = (unsigned long)
                        (cur_pos - (jwm_init::wav_buffer_size -2));
                    fill_buffer(buffer_start_pos);
                    buff_pos = jwm_init::wav_buffer_size - 2;
                } else {
                    fill_buffer(0);
                    buffer_start_pos = 0;
                    buff_pos = (int)cur_pos;
                }
            }
        }
        bp_midpoint = (double)((cur_pos - buffer_start_pos) - buff_pos);
        if (do_ac == ON) {
            //ac_cpstep = oldcpstep * (1 - ac_size) + cp_step * ac_size;
            if (ac_size > 1) do_ac = OFF;
            ac_midpoint 
             = (double)((ac_cur_pos-ac_buf_start_pos)-ac_buf_pos);
            #ifdef CRAZY_SAMPLER
            if ((ac_buf_pos > jwm_init::max_anti_clip_size - 1 
                || ac_buf_pos < 0) && do_ac == ON)
            {
                std::cout << "\n" << get_username() << "::run ";
                std::cout << "anti_clipping out of bounds of AC buffer ";
                std::cout << ac_buf_pos;
                std::cout << "\n(unsigned long)-1 = " << (unsigned long)-1;
                std::cout << "\n(unsigned long)ac_buf_pos = ";
                std::cout << (unsigned long)ac_buf_pos;
                std::cout << "\nmodule responsible " << get_username();
                std::cout << "\nPlease report this to james@jwm-art.net";
                std::cout << " specifying";
                std::cout << "\nlength, format, samplerate of wavfile, or";
                std::cout << "the sample";
                std::cout << "\nitself, and include the wc file responsible.";
                std::cout << "Thanks.";
                do_ac = OFF;
            }
            #endif
        }
        if (ch == WAV_CH_MONO) {
            #ifdef CRAZY_SAMPLER
            if (buff_pos + 1 >= jwm_init::wav_buffer_size || buff_pos < 0)
            {
                std::cout << "\n" << get_username() << "::run ";
                std::cout << " detected invalid read in mono_buffer of ";
                std::cout << "index " << buff_pos << ".";
                buff_pos = 0;
            }
            #endif
            out_left = calc_midpoint(mono_buffer[buff_pos],
                            mono_buffer[buff_pos + 1], bp_midpoint);
            if (do_ac == ON)  {
                ac_out_left = calc_midpoint(ac_m_buf[ac_buf_pos],
                 ac_m_buf[ac_buf_pos + 1], ac_midpoint);
                out_left = out_left * ac_size + ac_out_left *
                           (1 - ac_size);
            }
            out_right = out_left;
        }
        else { // ch == WAV_CH_STEREO
            out_left = calc_midpoint(st_buffer[buff_pos].left,
             st_buffer[buff_pos + 1].left, bp_midpoint);
            out_right = calc_midpoint(st_buffer[buff_pos].right,
             st_buffer[buff_pos + 1].right, bp_midpoint);
            if (do_ac == ON) {
                ac_out_left = calc_midpoint(ac_st_buf[ac_buf_pos].left,
                        ac_st_buf[ac_buf_pos + 1].left, ac_midpoint);
                ac_out_right = calc_midpoint(ac_st_buf[ac_buf_pos].right,
                        ac_st_buf[ac_buf_pos + 1].right, ac_midpoint);
                out_left = out_left * ac_size +
                           ac_out_left * (1 - ac_size);
                out_right = out_right * ac_size +
                            ac_out_right * (1 - ac_size);
            }
        } // endif ch == MONO/STEREO
        if (do_ac == ON) {
            if (acplaydir == PLAY_FWD) {
                ac_cur_pos += ac_cpstep;
                double ac_buf_posf = ac_cur_pos - ac_buf_start_pos;
                ac_buf_pos = (int)ac_buf_posf;
                ac_size = ac_buf_posf / anti_clip_size;
            }
            else {
                ac_cur_pos -= ac_cpstep;
                double ac_buf_posf = ac_cur_pos - ac_buf_start_pos;
                ac_buf_pos = (int)ac_buf_posf;
                ac_size = (double)(anti_clip_size - ac_buf_posf)
                          / anti_clip_size;
            }
        }
        if (playdir == PLAY_FWD) {
            cur_pos += cp_step;
        }
        else {
            cur_pos -= cp_step;
        }
    }
    sampletot ++;
}

void sampler::trigger_playback()
{
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username() << "::trigger_playback()";
    #endif
    if (play_state == ON && anti_clip_size > 0) {
        if (playdir == PLAY_FWD) anti_clip_fwd();
        else anti_clip_rev();
    }
    playdir = play_dir;
    double spm = *in_start_pos_mod; // make input positive only
    spm = (spm > 0) ? spm : -spm;
    if (playdir == PLAY_FWD) {
        start_pos=(unsigned long)(min_start_pos + startpos_span * spm);
        if (start_pos < wavstart) start_pos = wavstart;
        else if (start_pos > wavlength) start_pos = wavlength;
        cur_pos = start_pos = zero_search(start_pos, search_range);
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(PLAY_FWD) cur_pos = " << cur_pos;
        #endif
        buffer_start_pos = (unsigned long)cur_pos;
    } else {
        start_pos=(unsigned long)(max_start_pos - startpos_span * spm);
        if (start_pos < wavstart) start_pos = wavstart;
        else if (start_pos > wavlength) start_pos = wavlength;
        cur_pos = start_pos = zero_search(start_pos, search_range);
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(PLAY_REV) cur_pos = " << cur_pos;
        #endif
        if (cur_pos - (jwm_init::wav_buffer_size - 2) < 0)
            buffer_start_pos = 0;
        else buffer_start_pos 
          = (unsigned long)(cur_pos - (jwm_init::wav_buffer_size -2));
    }
    fill_buffer(buffer_start_pos);
    if (loop_mode == LOOP_OFF)
        loop_yet = 0;
    else {
        if (loop_is_offset == ON) {
            loopstart = start_pos + loop_begin;
            loopfinish = start_pos + loop_end;
            if (loopstart < wavstart || loopstart > wavlength)
                loopstart = wavstart;
            if (loopfinish < wavstart || loopfinish > wavlength)
                loopfinish = wavlength;
            loopstart = zero_search(loopstart, search_range);
            loopfinish = zero_search(loopfinish, search_range);
        }
        else {
            loopstart = loop_begin;
            loopfinish = loop_end;
        } // is play direction travelling away from loop?
        if ((loopstart > start_pos && playdir == PLAY_REV) ||
                (loopfinish < start_pos && playdir == PLAY_FWD))
            loop_yet = 0; else loop_yet = 1;
        if (loopfinish - loopstart < jwm_init::wav_buffer_size - 2)
            loop_fits_in_buffer = 1;
        else loop_fits_in_buffer = 0;
        loop_loaded = 0;
    }
    play_state = ON;
}

unsigned long sampler::zero_search(unsigned long pos, short range)
{
    if (range <= 0) return pos;
    double smallest1 = 10;
    double smallest2 = 10;
    unsigned long smallest_pos1 = 0;
    unsigned long smallest_pos2 = 0;
    int halfsr = range / 2;
    unsigned long buf_st_pos = pos - halfsr;
    unsigned long buf_end_pos = buf_st_pos + range;
    if (ch == WAV_CH_MONO) {
        double* sm_buf = new double[jwm_init::wav_buffer_size];
        wavfile->read_wav_at(sm_buf, buf_st_pos);
        for (int i = 0; i < halfsr; i++) {
            int si = range - i;
            double s1 = ((sm_buf[i] > 0) ? sm_buf[i] : -sm_buf[i]);
            double s2 = ((sm_buf[si] > 0) ? sm_buf[si] : -sm_buf[si]);
            if ( smallest1 > s1) {
                smallest1 = s1;
                smallest_pos1 = buf_st_pos + i;
            }
            if (smallest2 > s2) {
                smallest2 = s2;
                smallest_pos2 = buf_end_pos - i;
            }
        }
        delete [] sm_buf;
    }
    else {
        st_data* sst_buf = new st_data[jwm_init::wav_buffer_size];
        wavfile->read_wav_at(sst_buf, buf_st_pos);
        for (int i = 0; i < halfsr; i++) {
            int si = range - i;
            double s1 
             = ((sst_buf[i].left>0)?sst_buf[i].left:-sst_buf[i].left);
            s1 +=
             ((sst_buf[i].right>0)?sst_buf[i].right:-sst_buf[i].right);
            double s2 
             = ((sst_buf[si].left>0)?sst_buf[si].left:-sst_buf[si].left);
            s2 +=
             ((sst_buf[si].right>0)?sst_buf[si].right:-sst_buf[si].right);
            if (smallest1 > s1) {
                smallest1 = s1;
                smallest_pos1 = buf_st_pos + i;
            }
            if (smallest2 > s2) {
                smallest2 = s2;
                smallest_pos2 = buf_end_pos - i;
            }
        }
        delete [] sst_buf;
    }
    if (smallest1 < smallest2)
        return smallest_pos1;
    else
        return smallest_pos2;
}

void sampler::pos_wavlen()
{// playdir = PLAY_FWD
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username() << "::pos_wavlen";
    #endif
    if (play_mode == PLAY_STOP || loop_mode == LOOP_OFF) {
        out_left = out_right = 0;
        play_state = OFF;
        return;
    }
    if (ac_each_end == ON && anti_clip_size > 0)
        anti_clip_fwd();
    loop_yet = 1;
    PLAY_DIR jumpdir;
    if (jump_mode == JUMP_PLAY_DIR)
        jumpdir = play_dir;
    else if (loop_mode == LOOP_FWD) jumpdir = PLAY_FWD;
    else jumpdir = PLAY_REV;
    ;
    if (play_mode == PLAY_WRAP ||
            (play_mode == PLAY_JUMP && jumpdir == PLAY_FWD)
            || (loopfinish == wavlength && loop_mode == LOOP_FWD))
    {
        if (play_mode == PLAY_WRAP)	{
            cur_pos = wavstart;
            #ifdef CRAZY_SAMPLER
            std::cout << "\n" << get_username();
            std::cout << "(PLAY_WRAP) cur_pos = " << cur_pos;
            #endif
        }
        else {
            out_loop_trig = ON;
            cur_pos = loopstart;
            #ifdef CRAZY_SAMPLER
            std::cout << "\n" << get_username();
            std::cout << "(!PLAY_JUMP && jdPLAY_FWD) cur_pos = " << cur_pos;
            #endif
            if (loop_fits_in_buffer) loop_loaded = 1;
        }
        buffer_start_pos = (unsigned long)cur_pos;
        fill_buffer(buffer_start_pos);
        return;
    } // play_bounce/loop_rev/loop_bi/(play_jump && jump_loop_dir):
    playdir = PLAY_REV;
    if (play_mode == PLAY_BOUNCE) {
        cur_pos= wavlenbi;
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(PLAY_BOUNCE) cur_pos = " << cur_pos;
        #endif
    }
    else {
        out_loop_trig = ON;
        cur_pos = loopstart;
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(!PLAY_BOUNCE) cur_pos = " << cur_pos;
        #endif
        if (loop_fits_in_buffer) loop_loaded = 1;
    }
    if (cur_pos - (jwm_init::wav_buffer_size - 2) < 0)
        buffer_start_pos = 0;
    else buffer_start_pos 
     = (unsigned long)(cur_pos - (jwm_init::wav_buffer_size -2));
    fill_buffer(buffer_start_pos);
}

void sampler::pos_loopend()
{// playdir = PLAY_FWD
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username() << "::pos_loopend";
    #endif
    out_loop_trig = ON;
    if (anti_clip_size > 0)
        anti_clip_fwd();
    if (loop_mode == LOOP_FWD) {
        cur_pos = loopstart;
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(LOOP_FWD)cur_pos = " << cur_pos;
        #endif
        if (loop_fits_in_buffer && loop_loaded) return;
        else loop_loaded = 1;
        buffer_start_pos = (unsigned long)cur_pos;
        fill_buffer(buffer_start_pos);
        return;
    }
    playdir = PLAY_REV;
    int bisr = search_range / jwm_init::sr_div_bi;
    if (search_range > 0 && bisr < 2) bisr = 2;
    cur_pos = loopfinish - loop_bi_offset;
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username();
    std::cout << "(PLAY_REV)cur_pos = " << cur_pos;
    #endif
    if (cur_pos < wavstart) cur_pos = wavstart;
    else if (cur_pos < loopstart) {
        loop_yet = 0; // suspend looping for the mo.
        if (cur_pos < wavstart) {
            cur_pos = wavstart;
            #ifdef CRAZY_SAMPLER
            std::cout << "\n" << get_username();
            std::cout << "(cur_pos < wavstart)cur_pos = " << cur_pos;
            #endif
        }
        loop_loaded = 0; // no longer in loop
    }
    if (cur_pos != wavstart) {
        cur_pos = zero_search((unsigned long)cur_pos, bisr);
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(cur_pos != wavstart)cur_pos = " << cur_pos;
        #endif
    }
    if (loop_fits_in_buffer && loop_loaded)	return;
    else loop_loaded = 1;
    if (cur_pos - (jwm_init::wav_buffer_size -2) < 0)
        buffer_start_pos = 0;
    else buffer_start_pos = (unsigned long)
        (cur_pos - (jwm_init::wav_buffer_size -2));
    fill_buffer(buffer_start_pos);
    return;
}

void sampler::pos_wavstart()
{// playdir == PLAY_REV
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username() << "::pos_wavstart";
    #endif
    if (play_mode == PLAY_STOP || loop_mode == LOOP_OFF) {
        out_left = out_right = 0;
        play_state = OFF;
        return;
    }
    if (ac_each_end == ON && anti_clip_size > 0)
        anti_clip_rev();
    loop_yet = 1;
    PLAY_DIR jumpdir;
    if (jump_mode == JUMP_PLAY_DIR)
        jumpdir = play_dir;
    else if (loop_mode == LOOP_FWD) jumpdir = PLAY_FWD;
    else jumpdir = PLAY_REV;
    if (play_mode == PLAY_WRAP ||
            (play_mode == PLAY_JUMP && jumpdir == PLAY_REV) ||
            (loopstart = 0 && loop_mode == LOOP_REV))
    {
        if (play_mode == PLAY_WRAP) {
            cur_pos = wavlength;
            #ifdef CRAZY_SAMPLER
            std::cout << "\n" << get_username();
            std::cout << "(PLAY_WRAP)cur_pos = " << cur_pos;
            #endif
        }
        else {
            cur_pos = loopfinish;
            #ifdef CRAZY_SAMPLER
            std::cout << "\n" << get_username();
            std::cout << "(!PLAY_WRAP)cur_pos = " << cur_pos;
            #endif
            out_loop_trig = ON;
            if (loop_fits_in_buffer) loop_loaded = 1;
        }
        if (cur_pos - (jwm_init::wav_buffer_size - 2) < 0)
            buffer_start_pos = 0;
        else buffer_start_pos = (unsigned long)
             (cur_pos - (jwm_init::wav_buffer_size -2));
        fill_buffer(buffer_start_pos);
        return;
    }
    playdir = PLAY_FWD;
    if (play_mode == PLAY_BOUNCE) {
        cur_pos = wavstbi;
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(PLAY_BOUNCE)cur_pos = " << cur_pos;
        #endif
    }
    else { // must be PLAY_JUMP then.
        cur_pos = loopstart;
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(PLAY_JUMP)cur_pos = " << cur_pos;
        #endif
        out_loop_trig = ON;
        if (loop_fits_in_buffer) loop_loaded = 1;
    }
    buffer_start_pos = (unsigned long)cur_pos;
    fill_buffer(buffer_start_pos);
}

void sampler::pos_loopbegin()
{// playdir == PLAY_REV
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username() << "::pos_loopbegin";
    #endif
    out_loop_trig = ON;
    if (anti_clip_size > 0)
        anti_clip_rev();
    if (loop_mode == LOOP_REV) {
        cur_pos = loopfinish;
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(LOOP_REV)cur_pos = " << cur_pos;
        #endif
        if (loop_fits_in_buffer && loop_loaded) return;
        else loop_loaded = 1;
        if (cur_pos - (jwm_init::wav_buffer_size - 2) < 0) 
            buffer_start_pos = 0;
        else buffer_start_pos = (unsigned long)
            (cur_pos - (jwm_init::wav_buffer_size -2));
        fill_buffer(buffer_start_pos);
        return;
    }
    playdir = PLAY_FWD;
    int bisr = search_range / jwm_init::sr_div_bi;
    if (search_range > 0 && bisr < 2) bisr = 2;
    cur_pos = loopstart + loop_bi_offset;
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username();
    std::cout << "(PLAY_FWD)cur_pos = " << cur_pos;
    #endif
    if (cur_pos < wavstart) cur_pos = wavstart;
    else if (cur_pos > loopfinish) {
        loop_yet = 0; // suspend looping until back inside loop.
        if (cur_pos > wavlength) {
            cur_pos = wavlength;
            #ifdef CRAZY_SAMPLER
            std::cout << "\n" << get_username();
            std::cout << "(cur_pos > wavlength)cur_pos = " << cur_pos;
            #endif
        }
        loop_loaded = 0; // no longer in loop.
    }
    if (cur_pos != wavlength) {
        cur_pos = zero_search((unsigned long)cur_pos, bisr);
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(cur_pos != wavlength)cur_pos = " << cur_pos;
        #endif
    }
    if (loop_fits_in_buffer && loop_loaded) return;
    else loop_loaded = 1;
    buffer_start_pos = (unsigned long)cur_pos;
    fill_buffer(buffer_start_pos);
}

void sampler::anti_clip_fwd()
{
    double* ac_m_tmp = 0;
    st_data* ac_st_tmp = 0;
    if (do_ac == ON) { // anti clip already active
        if (ch == WAV_CH_MONO) {
            ac_m_tmp = new double[jwm_init::max_anti_clip_size];
            ac_copy_fwd_mono(ac_m_tmp);
        } else {
            ac_st_tmp = new st_data[jwm_init::max_anti_clip_size];
            ac_copy_fwd_stereo(ac_st_tmp);
        }
    } // could use buff_pos, but recalculating gives better result:
    int abp = (unsigned long)(cur_pos - buffer_start_pos);
    if (abp + anti_clip_size > jwm_init::wav_buffer_size - 1) {
        if (ch == WAV_CH_MONO)
            wavfile->read_wav_chunk(
             ac_m_buf, (unsigned long)cur_pos, anti_clip_size + 1);
        else
            wavfile->read_wav_chunk(
             ac_st_buf,(unsigned long) cur_pos, anti_clip_size + 1);
    }
    else {   // get from wav buffer
        if (ch == WAV_CH_MONO) {
            for (int i = 0; i <= anti_clip_size; i++)
                ac_m_buf[i] = mono_buffer[abp + i];
        }
        else {
            for (int i = 0; i <= anti_clip_size; i++) {
                ac_st_buf[i].left = st_buffer[abp + i].left;
                ac_st_buf[i].right = st_buffer[abp + i].right;
            }
        }
    }
    if (do_ac == ON) { // now mix with new
        if (ch == WAV_CH_MONO) {
            ac_mix_fwd_mono(ac_m_tmp);
            delete [] ac_m_tmp;
        } else {
            ac_mix_fwd_stereo(ac_st_tmp);
            delete [] ac_st_tmp;
        }
    }
    acplaydir = PLAY_FWD;
    ac_cur_pos = cur_pos;
    ac_buf_pos = 0;
    ac_buf_start_pos = (unsigned long)cur_pos;
    ac_out_left = ac_out_right = 0;
    ac_size = 0;
    do_ac = ON;
    ac_cpstep = oldcpstep;
}

void sampler::anti_clip_rev()
{
    double* ac_m_tmp = 0;
    st_data* ac_st_tmp = 0;
    if (do_ac == ON) { // anti clip already active
        if (ch == WAV_CH_MONO) {
            ac_m_tmp = new double[jwm_init::max_anti_clip_size];
            ac_copy_rev_mono(ac_m_tmp);
        } else {
            ac_st_tmp = new st_data[jwm_init::max_anti_clip_size];
            ac_copy_rev_stereo(ac_st_tmp);
        }
    }
    if (cur_pos - (anti_clip_size - 1) < 0) ac_buf_start_pos = 0;
    else ac_buf_start_pos=(unsigned long)(cur_pos - (anti_clip_size - 1));
    ac_cur_pos = cur_pos;
    int acbp = (int)(cur_pos - buffer_start_pos) - (anti_clip_size - 1);
    if (acbp <= 0) {
        if (ch == WAV_CH_MONO)
            wavfile->read_wav_chunk(ac_m_buf,
                                    ac_buf_start_pos, anti_clip_size + 1);
        else
            wavfile->read_wav_chunk(ac_st_buf,
                                    ac_buf_start_pos, anti_clip_size + 1);
    }
    else {
        if (ch == WAV_CH_MONO)
            for (int i = 0; i <= anti_clip_size; i++)
                ac_m_buf[i] = mono_buffer[acbp + i];
        else
            for (int i = 0; i <= anti_clip_size; i++) {
                ac_st_buf[i].left = st_buffer[acbp + i].left;
                ac_st_buf[i].right = st_buffer[acbp + i].right;
            }
    }
    if (do_ac == ON) { // now mix with new
        if (ch == WAV_CH_MONO) {
            ac_mix_rev_mono(ac_m_tmp);
            delete [] ac_m_tmp;
        } else {
            ac_mix_rev_stereo(ac_st_tmp);
            delete [] ac_st_tmp;
        }
    }
    acplaydir = PLAY_REV;
    ac_out_left = ac_out_right = 0;
    ac_step = 1.00 / (double)anti_clip_size;
    ac_size = 0;
    ac_buf_pos = (int)(ac_cur_pos - ac_buf_start_pos);
    do_ac = ON;
    ac_cpstep = oldcpstep;
}

// ac_copy... methods are used when anticlip is activated while already
// active, there are four of them.  ac_copy_fwd... is called by
// anti_clip_fwd
// while ac_copy_rev... is called by anti_clip_rev, the relevant mono or
// stereo versions are called dependant on the wavfile being read.
void sampler::ac_copy_fwd_mono(double* ac_m_tmp)
{
    double tmp_acsz = ac_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos + i <= anti_clip_size && tmp_acsz <= 1) {
                ac_m_tmp[i] = ac_m_buf[ac_buf_pos + i] * (1 - tmp_acsz);
                tmp_acsz += ac_step;
            }
            else ac_m_tmp[i] = 0;
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos - i >= 0 && tmp_acsz <= 1.0) {
                ac_m_tmp[i] = ac_m_buf[ac_buf_pos - i] * (1 - tmp_acsz);
                tmp_acsz += ac_step;
            }
            else ac_m_tmp[i] = 0;
        }
    }
}

void sampler::ac_copy_fwd_stereo(st_data* ac_st_tmp)
{
    double tmp_acsz = ac_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos + i <= anti_clip_size && tmp_acsz <= 1) {
                ac_st_tmp[i].left = ac_st_buf[ac_buf_pos + i].left *
                                    (1 - tmp_acsz);
                ac_st_tmp[i].right= ac_st_buf[ac_buf_pos + i].right *
                                    (1 - tmp_acsz);
                tmp_acsz += ac_step;
            } else {
                ac_st_tmp[i].left = 0;
                ac_st_tmp[i].right = 0;
            }
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos - i >= 0 && tmp_acsz <= 1) {
                ac_st_tmp[i].left = ac_st_buf[ac_buf_pos - i].left *
                                    (1 - tmp_acsz);
                ac_st_tmp[i].right =ac_st_buf[ac_buf_pos - i].right *
                                    (1 - tmp_acsz);
                tmp_acsz += ac_step;
            } else {
                ac_st_tmp[i].left = 0;
                ac_st_tmp[i].right = 0;
            }
        }
    }
}

void sampler::ac_mix_fwd_mono(double* ac_m_tmp)
{
    int i = 0;
    while (ac_size <= 1 && i <= anti_clip_size) {
        ac_m_buf[i] = ac_m_buf[i] * ac_size	+ ac_m_tmp[i];
        ac_size += ac_step;
        i++;
    }
}

void sampler::ac_mix_fwd_stereo(st_data* ac_st_tmp)
{
    int i = 0;
    while (ac_size < 1) {
        ac_st_buf[i].left = ac_st_buf[i].left * ac_size +
                            ac_st_tmp[i].left;
        ac_st_buf[i].right = ac_st_buf[i].right * ac_size +
                             ac_st_tmp[i].right;
        ac_size += ac_step;
        i++;
    }
}

void sampler::ac_copy_rev_mono(double* ac_m_tmp)
{
    double tmp_acsz = ac_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos + i <= anti_clip_size && tmp_acsz <= 1) {
                ac_m_tmp[anti_clip_size - i] =
                    ac_m_buf[ac_buf_pos + i]*(1 - tmp_acsz);
                tmp_acsz += ac_step;
            }
            else ac_m_tmp[anti_clip_size - i] = 0;
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos - i >= 0 && tmp_acsz <= 1.0) {
                ac_m_tmp[anti_clip_size - i] =
                    ac_m_buf[ac_buf_pos-i] * (1 - tmp_acsz);
                tmp_acsz += ac_step;
            }
            else ac_m_tmp[anti_clip_size - i] = 0;
        }
    }
}

void sampler::ac_copy_rev_stereo(st_data* ac_st_tmp)
{
    double tmp_acsz = ac_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos + i <= anti_clip_size && tmp_acsz <= 1) {
                ac_st_tmp[anti_clip_size - i].left
                 = ac_st_buf[ac_buf_pos + i].left * (1 - tmp_acsz);
                ac_st_tmp[anti_clip_size - i].right
                 = ac_st_buf[ac_buf_pos + i].right* (1 - tmp_acsz);
                tmp_acsz += ac_step;
            } else {
                ac_st_tmp[anti_clip_size - i].left = 0;
                ac_st_tmp[anti_clip_size - i].right = 0;
            }
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos - i >= 0 && tmp_acsz <= 1) {
                ac_st_tmp[anti_clip_size - i].left
                 = ac_st_buf[ac_buf_pos - i].left * (1 - tmp_acsz);
                ac_st_tmp[anti_clip_size - i].right
                 = ac_st_buf[ac_buf_pos + i].right* (1 - tmp_acsz);
                tmp_acsz += ac_step;
            } else {
                ac_st_tmp[anti_clip_size - i].left = 0;
                ac_st_tmp[anti_clip_size - i].right = 0;
            }
        }
    }
}

void sampler::ac_mix_rev_mono(double* ac_m_tmp)
{
    int i = anti_clip_size;
    while (ac_size <= 1 && i >= 0) {
        ac_m_buf[i] = ac_m_buf[i] * ac_size + ac_m_tmp[i];
        ac_size += ac_step;
        i--;
    }
}

void sampler::ac_mix_rev_stereo(st_data* ac_st_tmp)
{
    int i = anti_clip_size;
    while (ac_size <= 1 && i > 0) {
        ac_st_buf[i].left = ac_st_buf[i].left * ac_size +
                            ac_st_tmp[i].left;
        ac_st_buf[i].right = ac_st_buf[i].right * ac_size +
                             ac_st_tmp[i].right;
        ac_size += ac_step;
        i--;
    }
}

void sampler::create_params()
{
    if (done_params())
        return;
    relate_param(paramnames::WAVFILEIN);
    relate_param(paramnames::PLAY_DIR,     "fwd/rev");
    relate_param(paramnames::PLAY_MODE,    "stop/wrap/bounce/jump");
    relate_param(paramnames::JUMP_MODE,    "play/loop");
    relate_param(paramnames::START_POS_MIN);
    relate_param(paramnames::START_POS_MAX);
    relate_param(paramnames::LOOP_MODE,    "off/fwd/rev/bi");
    relate_param(paramnames::LOOP_BEGIN);
    relate_param(paramnames::LOOP_END);
    relate_param(paramnames::LOOP_IS_OFFSET);
    relate_param(paramnames::LOOP_BI_OFFSET);
    relate_param(paramnames::ANTI_CLIP);
    relate_param(paramnames::AC_EACH_END);
    relate_param(paramnames::ZERO_SEARCH_RANGE);
    relate_param(paramnames::PHASE_STEP_AMOUNT);
}
