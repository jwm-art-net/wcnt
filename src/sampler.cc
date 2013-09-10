#ifndef SAMPLER_H
#include "../include/sampler.h"

sampler::sampler(char const* uname) :
 synthmod(synthmodnames::MOD_SAMPLER, sampler_count, uname),
 in_play_trig(0), in_start_pos_mod(0), in_deg_size(0),
 out_left(0), out_right(0), out_l(0), out_r(0), out_loop_trig(OFF),
 play_state(OFF), wavfile(0), play_dir(PLAY_FWD), play_mode(PLAY_STOP),
 jump_mode(JUMP_PLAY_DIR), min_start_pos(0), max_start_pos(0),
 loop_begin(0), loop_end(0), loop_is_offset(OFF), loop_mode(LOOP_OFF),
 loop_bi_offset(0), anti_clip_size(0), ac_each_end(OFF), search_range(0),
 deg_size_amount(0), root_deg_size(1.0), playdir(PLAY_FWD),
 acplaydir(PLAY_FWD), mono_buffer(0), ac_m_buf(0), st_buffer(0),
 ac_st_buf(0), buffer_start_pos(0), buff_pos(0), wavstart(1),
 wavlength(0), wavstbi(0), wavlenbi(0), cur_pos(0), cp_step(0),
 cp_ratio(0), sr_ratio(1), bp_midpoint(0), start_pos(0), 
 startpos_span(0), loopstart(0), loopfinish(0), loop_fits_in_buffer(0),
 loop_loaded(0), do_ac(OFF), ac_cur_pos(0), ac_midpoint(0), 
 ac_buf_pos(0), ac_buf_start_pos(0), ac_step(0), ac_size(0),
 ac_out_left(0), ac_out_right(0), ch(WAV_CH_UNKNOWN)
{
#ifndef BARE_MODULES
    get_outputlist()->add_output(this, outputnames::OUT_LEFT);
    get_outputlist()->add_output(this, outputnames::OUT_RIGHT);
    get_outputlist()->add_output(this, outputnames::OUT_L);
    get_outputlist()->add_output(this, outputnames::OUT_R);
    get_outputlist()->add_output(this, outputnames::OUT_LOOP_TRIG);
    get_outputlist()->add_output(this, outputnames::OUT_PLAY_STATE);
    get_inputlist()->add_input(this, inputnames::IN_PLAY_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_STOP_TRIG);
    get_inputlist()->add_input(this, inputnames::IN_START_POS_MOD);
    get_inputlist()->add_input(this, inputnames::IN_DEG_SIZE);
#endif // BARE_MODULES
    sampler_count++;
#ifndef BARE_MODULES
    create_params();
#endif
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
#ifndef BARE_MODULES
    get_outputlist()->delete_module_outputs(this);
    get_inputlist()->delete_module_inputs(this);
#endif
}

#ifndef BARE_MODULES
void const* sampler::get_out(outputnames::OUT_TYPE ot)
{
    void const* o = 0;
    switch(ot)
    {
    case outputnames::OUT_LEFT:
        o = &out_left;
        break;
    case outputnames::OUT_RIGHT:
        o = &out_right;
        break;
    case outputnames::OUT_L:
        o = &out_l;
        break;
    case outputnames::OUT_R:
        o = &out_r;
        break;
    case outputnames::OUT_LOOP_TRIG:
        o = &out_loop_trig;
        break;
    case outputnames::OUT_PLAY_STATE:
        o = &play_state;
        break;
    default:
        o = 0;
    }
    return o;
}

void const* sampler::set_in(inputnames::IN_TYPE it, void const* o)
{
    void const* i = 0;
    switch(it)
    {
    case inputnames::IN_PLAY_TRIG:
        i = in_play_trig = (STATUS*)o;
        break;
    case inputnames::IN_STOP_TRIG:
        i = in_stop_trig = (STATUS*)o;
        break;
    case inputnames::IN_DEG_SIZE:
        i = in_deg_size = (double*)o;
        break;
    case inputnames::IN_START_POS_MOD:
        i = in_start_pos_mod = (double*)o;
        break;
    default:
        i = 0;
    }
    return i;
}

bool sampler::set_param(paramnames::PAR_TYPE pt, void const* data)
{
    bool retv = false;
    switch(pt)
    {
    case paramnames::PAR_WAVFILEIN:
        if (((dobj*)data)->get_object_type() != dobjnames::SDEF_WAVFILEIN)
        {
            *err_msg = "\n" + *((dobj*)data)->get_username();
            *err_msg += " is not a wavfilein";
            retv = false;
        }
        else {
            set_wavfilein((wavfilein*)data); //note: pass pointer
            retv = true;
        }
        break;
    case paramnames::PAR_PLAY_DIR:
        set_play_dir(*(PLAY_DIR*)data);
        retv = true;
        break;
    case paramnames::PAR_PLAY_MODE:
        set_play_mode(*(PLAY_MODE*)data);
        retv = true;
        break;
    case paramnames::PAR_JUMP_MODE:
        set_jump_mode(*(JUMP_DIR*)data);
        retv = true;
        break;
    case paramnames::PAR_START_POS_MIN:
        set_start_pos_min(*(unsigned long*)data);
        retv = true;
        break;
    case paramnames::PAR_START_POS_MAX:
        set_start_pos_max(*(unsigned long*)data);
        retv = true;
        break;
    case paramnames::PAR_LOOP_BEGIN:
        set_loop_begin(*(unsigned long*)data);
        retv = true;
        break;
    case paramnames::PAR_LOOP_END:
        set_loop_end(*(unsigned long*)data);
        retv = true;
        break;
    case paramnames::PAR_LOOP_MODE:
        set_loop_mode(*(LOOP_MODE*)data);
        retv = true;
        break;
    case paramnames::PAR_LOOP_IS_OFFSET:
        set_loop_is_offset(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_LOOP_BI_OFFSET:
        set_loop_bi_offset(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_ANTI_CLIP:
        set_anti_clip_samples(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_AC_EACH_END:
        set_anti_clip_each_end(*(STATUS*)data);
        retv = true;
        break;
    case paramnames::PAR_ZERO_SEARCH_RANGE:
        set_zero_search_range(*(short*)data);
        retv = true;
        break;
    case paramnames::PAR_DEGSIZE_AMOUNT:
        set_degsize_amount(*(double*)data);
        retv = true;
        break;
    default:
        retv = false;
        break;
    }
    return retv;
}

void const* sampler::get_param(paramnames::PAR_TYPE pt)
{
    switch(pt)
    {
    case paramnames::PAR_WAVFILEIN:
        return wavfile;
    case paramnames::PAR_PLAY_DIR:
        return &play_dir;
    case paramnames::PAR_PLAY_MODE:
        return &play_mode;
    case paramnames::PAR_JUMP_MODE:
        return &jump_mode;
    case paramnames::PAR_START_POS_MIN:
        return &min_start_pos;
    case paramnames::PAR_START_POS_MAX:
        return &max_start_pos;
    case paramnames::PAR_LOOP_BEGIN:
        return &loop_begin;
    case paramnames::PAR_LOOP_END:
        return &loop_end;
    case paramnames::PAR_LOOP_MODE:
        return &loop_mode;
    case paramnames::PAR_LOOP_IS_OFFSET:
        return &loop_is_offset;
    case paramnames::PAR_LOOP_BI_OFFSET:
        return &loop_bi_offset;
    case paramnames::PAR_ANTI_CLIP:
        return &anti_clip_size;
    case paramnames::PAR_AC_EACH_END:
        return &ac_each_end;
    case paramnames::PAR_ZERO_SEARCH_RANGE:
        return &search_range;
    case paramnames::PAR_DEGSIZE_AMOUNT:
        return &deg_size_amount;
    default:
        return 0;
    }
}

#endif // BARE_MODULES

void sampler::set_wavfilein(wavfilein * wi)
{
    if (wi->get_status() == WAV_STATUS_OPEN)
    {
        wavfile = wi;
        ch = wavfile->get_channel_status();
        wavlength = wavfile->get_length() - 2;
        sr_ratio = (double)wavfile->get_sample_rate() / audio_samplerate;
        if (ch == WAV_CH_MONO) {
            mono_buffer = new short[WAV_BUFFER_SIZE];
            for (int i = 0; i < WAV_BUFFER_SIZE; i++)
                mono_buffer[i] = 0;
            ac_m_buf = new short[MAX_ANTI_CLIP_SIZE];
            for (int i = 0; i < MAX_ANTI_CLIP_SIZE; i++)
                ac_m_buf[i] = 0;
        }
        else {
            st_buffer = new stereodata[WAV_BUFFER_SIZE];
            for (int i = 0; i < WAV_BUFFER_SIZE; i++) {
                st_buffer[i].left = 0;
                st_buffer[i].right = 0;
            }
            ac_st_buf = new stereodata[MAX_ANTI_CLIP_SIZE];
            for (int i = 0; i < MAX_ANTI_CLIP_SIZE; i++) {
                ac_st_buf[i].left = 0;
                ac_st_buf[i].right = 0;
            }
        }
    }
    root_deg_size = wavfile->get_root_deg_size();
}

void sampler::set_anti_clip_samples(short acs)
{
    anti_clip_size = acs;
}

stockerrs::ERR_TYPE sampler::validate()
{
    modparamlist* pl = get_paramlist();
    if (!pl->validate(this, paramnames::PAR_START_POS_MIN,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_START_POS_MIN);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (!pl->validate(this, paramnames::PAR_START_POS_MAX,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_START_POS_MAX);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    if (max_start_pos < min_start_pos) {
        *err_msg +=
         get_paramnames()->get_name(paramnames::PAR_START_POS_MAX);
        *err_msg += " must not be less than ";
        *err_msg +=
         get_paramnames()->get_name(paramnames::PAR_START_POS_MIN);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (loop_is_offset == OFF) {
        if (!pl->validate(this, paramnames::PAR_LOOP_BEGIN,
                stockerrs::ERR_NEGATIVE))
        {
            *err_msg =
             get_paramnames()->get_name(paramnames::PAR_LOOP_BEGIN);
            *err_msg += " of absolute value";
            invalidate();
            return stockerrs::ERR_NEGATIVE;
        }
        if (!pl->validate(this, paramnames::PAR_LOOP_END,
                stockerrs::ERR_NEGATIVE))
        {
            *err_msg =
             get_paramnames()->get_name(paramnames::PAR_LOOP_END);
            *err_msg += " of absolute value";
            invalidate();
            return stockerrs::ERR_NEGATIVE;
        }
    }
    if (loop_end <= loop_begin) {
        *err_msg += get_paramnames()->get_name(paramnames::PAR_LOOP_END);
        *err_msg += " must be more than ";
        *err_msg +=
         get_paramnames()->get_name(paramnames::PAR_LOOP_BEGIN);
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (anti_clip_size < 0 || anti_clip_size > MAX_ANTI_CLIP_SAMPLES) {
        *err_msg += get_paramnames()->get_name(paramnames::PAR_ANTI_CLIP);
        *err_msg += " out of range 0 ~ 2048";
        invalidate();
        return stockerrs::ERR_ERROR;
    }
    if (!pl->validate(this, paramnames::PAR_ZERO_SEARCH_RANGE,
            stockerrs::ERR_NEGATIVE))
    {
        *err_msg =
         get_paramnames()->get_name(paramnames::PAR_ZERO_SEARCH_RANGE);
        invalidate();
        return stockerrs::ERR_NEGATIVE;
    }
    return stockerrs::ERR_NO_ERROR;
}

void sampler::init()
{ // do a whole load of checks on the users input.
    int halfsr = search_range / 2;
    if (ac_each_end == ON) {
        wavstart = zero_search(anti_clip_size + halfsr, search_range);
        wavlength
         = zero_search(wavlength-anti_clip_size-halfsr,search_range);
    } else {
        wavstart = zero_search(halfsr, search_range);
        wavlength = zero_search(wavlength - halfsr, search_range);
    }
    int bisr = search_range / SR_DIV_BI;
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
        if (loopsize < WAV_BUFFER_SIZE - 1)
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
inline short sampler::calc_midpoint(short a, short b, double r) {
    return (short)(a + (b - a) * r);
}

void sampler::run()
{
    if (*in_stop_trig == ON) {
        out_left = out_right = 0;
        out_l = out_r = 0;
        play_state = OFF;
        do_ac = OFF;
    }
    if (*in_play_trig == ON)
        trigger_playback();
    if (play_state == ON) {
        if (do_ac == OFF) oldcpstp = cp_step;
        if (out_loop_trig == ON) out_loop_trig = OFF;
        cp_ratio = *in_deg_size / root_deg_size;
        //sample rate ratio is sr_ratio
        cp_step = (1 + (cp_ratio - 1) * deg_size_amount) * sr_ratio;
        buff_pos = (unsigned int)cur_pos - buffer_start_pos;
        if (playdir == PLAY_FWD) {
            if (cur_pos >= wavlength) {
                pos_wavlen();
                buff_pos = (unsigned int)cur_pos - buffer_start_pos;
            }
            else if (loop_yet && cur_pos > loopfinish) {
                pos_loopend();
                buff_pos = (unsigned int)cur_pos - buffer_start_pos;
            }
            if (buff_pos >= WAV_BUFFER_SIZE - 1) {
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
                if (cur_pos - (WAV_BUFFER_SIZE - 2) >= 0) {
                    buffer_start_pos 
                     = (unsigned long)(cur_pos - (WAV_BUFFER_SIZE -2));
                    fill_buffer(buffer_start_pos);
                    buff_pos = WAV_BUFFER_SIZE - 2;
                } else {
                    fill_buffer(0);
                    buffer_start_pos = 0;
                    buff_pos = (int)cur_pos;
                }
            }
        }
        bp_midpoint = (double)((cur_pos - buffer_start_pos) - buff_pos);
        if (do_ac == ON) {
            //ac_cpstep = oldcpstp * (1 - ac_size) + cp_step * ac_size;
            if (ac_size > 1) do_ac = OFF;
            ac_midpoint 
             = (double)((ac_cur_pos-ac_buf_start_pos)-ac_buf_pos);
            if ((ac_buf_pos > MAX_ANTI_CLIP_SIZE - 1 
                || ac_buf_pos < 0) && do_ac == ON)
            {
                cout << "\nanti_clipping out of bounds of AC buffer ";
                cout << ac_buf_pos;
                cout << "\nmodule responsible " << *get_username();
                cout << "\nPlease report this to james@jwm-art.net";
                cout << " specifying";
                cout << "\nlength, format, samplerate of wavfile, or";
                cout << "the sample";
                cout << "\nitself, and include the wc file responsible.";
                cout << "Thanks.";
                do_ac = OFF;
            }
        }
        if (ch == WAV_CH_MONO) {
            out_left = calc_midpoint(mono_buffer[buff_pos],
             mono_buffer[buff_pos + 1], bp_midpoint);
            if (do_ac == ON)  {
                ac_out_left = calc_midpoint(ac_m_buf[ac_buf_pos],
                 ac_m_buf[ac_buf_pos + 1], ac_midpoint);
                out_left = (short)(out_left * ac_size 
                 + ac_out_left * (1 - ac_size));
            }
            out_right = out_left;
            out_l = out_r = (double)out_left / 0x7fff;
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
                out_left = (short)(out_left * ac_size +
                                   ac_out_left * (1 - ac_size));
                out_right = (short)(out_right * ac_size +
                                    ac_out_right * (1 - ac_size));
            }
            out_l = (double)out_left / 0x7fff;
            out_r = (double)out_right / 0x7fff;
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
        if (playdir == PLAY_FWD) cur_pos += cp_step;
        else cur_pos -= cp_step;
    }
    sampletot ++;
}

void sampler::trigger_playback()
{
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
        buffer_start_pos = (unsigned long)cur_pos;
    } else {
        start_pos=(unsigned long)(max_start_pos - startpos_span * spm);
        if (start_pos < wavstart) start_pos = wavstart;
        else if (start_pos > wavlength) start_pos = wavlength;
        cur_pos = start_pos = zero_search(start_pos, search_range);
        if (cur_pos - (WAV_BUFFER_SIZE - 2) < 0) buffer_start_pos = 0;
        else buffer_start_pos 
          = (unsigned long)(cur_pos - (WAV_BUFFER_SIZE -2));
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
        if (loopfinish - loopstart < WAV_BUFFER_SIZE - 2)
            loop_fits_in_buffer = 1;
        else loop_fits_in_buffer = 0;
        loop_loaded = 0;
    }
    play_state = ON;
}

unsigned long sampler::zero_search(unsigned long pos, short range)
{
    if (range <= 0) return pos;
    long smallest1 = 65536;
    long smallest2 = 65536;
    unsigned long smallest_pos1 = 0;
    unsigned long smallest_pos2 = 0;
    int halfsr = range / 2;
    unsigned long buf_st_pos = pos - halfsr;
    unsigned long buf_end_pos = buf_st_pos + range;
    if (ch == WAV_CH_MONO) {
        short * sm_buf = new short[WAV_BUFFER_SIZE];
        wavfile->read_wav_at(sm_buf, buf_st_pos);
        for (int i = 0; i < halfsr; i++) {
            int si = range - i;
            int s1 = ((sm_buf[i] > 0) ? sm_buf[i] : -sm_buf[i]);
            int s2 = ((sm_buf[si] > 0) ? sm_buf[si] : -sm_buf[si]);
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
        stereodata* sst_buf = new stereodata[WAV_BUFFER_SIZE];
        wavfile->read_wav_at(sst_buf, buf_st_pos);
        for (int i = 0; i < halfsr; i++) {
            int si = range - i;
            int s1 
             = ((sst_buf[i].left>0)?sst_buf[i].left:-sst_buf[i].left);
            s1 +=
             ((sst_buf[i].right>0)?sst_buf[i].right:-sst_buf[i].right);
            int s2 
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
    if (play_mode == PLAY_STOP || loop_mode == LOOP_OFF) {
        out_left = out_right = 0;
        out_l = out_r = 0;
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
        if (play_mode == PLAY_WRAP)	cur_pos = wavstart;
        else {
            out_loop_trig = ON;
            cur_pos = loopstart;
            if (loop_fits_in_buffer) loop_loaded = 1;
        }
        buffer_start_pos = (unsigned long)cur_pos;
        fill_buffer(buffer_start_pos);
        return;
    } // play_bounce/loop_rev/loop_bi/(play_jump && jump_loop_dir):
    playdir = PLAY_REV;
    if (play_mode == PLAY_BOUNCE)
        cur_pos= wavlenbi;
    else {
        out_loop_trig = ON;
        cur_pos = loopstart;
        if (loop_fits_in_buffer) loop_loaded = 1;
    }
    if (cur_pos - (WAV_BUFFER_SIZE - 2) < 0) buffer_start_pos = 0;
    else buffer_start_pos 
     = (unsigned long)(cur_pos - (WAV_BUFFER_SIZE -2));
    fill_buffer(buffer_start_pos);
}

void sampler::pos_loopend()
{// playdir = PLAY_FWD
    out_loop_trig = ON;
    if (anti_clip_size > 0)
        anti_clip_fwd();
    if (loop_mode == LOOP_FWD) {
        cur_pos = loopstart;
        if (loop_fits_in_buffer && loop_loaded) return;
        else loop_loaded = 1;
        buffer_start_pos = (unsigned long)cur_pos;
        fill_buffer(buffer_start_pos);
        return;
    }
    playdir = PLAY_REV;
    int bisr = search_range / SR_DIV_BI;
    if (search_range > 0 && bisr < 2) bisr = 2;
    cur_pos = loopfinish - loop_bi_offset;
    if (cur_pos < wavstart) cur_pos = wavstart;
    else if (cur_pos < loopstart) {
        loop_yet = 0; // suspend looping for the mo.
        if (cur_pos < wavstart) cur_pos = wavstart;
        loop_loaded = 0; // no longer in loop
    }
    if (cur_pos != wavstart) 
        cur_pos = zero_search((unsigned long)cur_pos, bisr);
    if (loop_fits_in_buffer && loop_loaded)	return;
    else loop_loaded = 1;
    if (cur_pos - (WAV_BUFFER_SIZE -2) < 0) buffer_start_pos = 0;
    else buffer_start_pos 
         = (unsigned long)(cur_pos - (WAV_BUFFER_SIZE -2));
    fill_buffer(buffer_start_pos);
    return;
}

void sampler::pos_wavstart()
{// playdir == PLAY_REV
    if (play_mode == PLAY_STOP || loop_mode == LOOP_OFF) {
        out_left = out_right = 0;
        out_l = out_r = 0;
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
    ;
    if (play_mode == PLAY_WRAP ||
            (play_mode == PLAY_JUMP && jumpdir == PLAY_REV) ||
            (loopstart = 0 && loop_mode == LOOP_REV))
    {
        if (play_mode == PLAY_WRAP) cur_pos = wavlength;
        else {
            cur_pos = loopfinish;
            out_loop_trig = ON;
            if (loop_fits_in_buffer) loop_loaded = 1;
        }
        if (cur_pos - (WAV_BUFFER_SIZE - 2) < 0) buffer_start_pos = 0;
        else buffer_start_pos
             = (unsigned long)(cur_pos - (WAV_BUFFER_SIZE -2));
        fill_buffer(buffer_start_pos);
        return;
    }
    playdir = PLAY_FWD;
    if (play_mode == PLAY_BOUNCE)
        cur_pos = wavstbi;
    else { // must be PLAY_JUMP then.
        cur_pos = loopstart;
        out_loop_trig = ON;
        if (loop_fits_in_buffer) loop_loaded = 1;
    }
    buffer_start_pos = (unsigned long)cur_pos;
    fill_buffer(buffer_start_pos);
}

void sampler::pos_loopbegin()
{// playdir == PLAY_REV
    out_loop_trig = ON;
    if (anti_clip_size > 0)
        anti_clip_rev();
    if (loop_mode == LOOP_REV) {
        cur_pos = loopfinish;
        if (loop_fits_in_buffer && loop_loaded) return;
        else loop_loaded = 1;
        if (cur_pos - (WAV_BUFFER_SIZE - 2) < 0) buffer_start_pos = 0;
        else buffer_start_pos
             = (unsigned long)(cur_pos - (WAV_BUFFER_SIZE -2));
        fill_buffer(buffer_start_pos);
        return;
    }
    playdir = PLAY_FWD;
    int bisr = search_range / SR_DIV_BI;
    if (search_range > 0 && bisr < 2) bisr = 2;
    cur_pos = loopstart + loop_bi_offset;
    if (cur_pos < wavstart) cur_pos = wavstart;
    else if (cur_pos > loopfinish) {
        loop_yet = 0; // suspend looping until back inside loop.
        if (cur_pos > wavlength) cur_pos = wavlength;
        loop_loaded = 0; // no longer in loop.
    }
    if (cur_pos != wavlength)
        cur_pos = zero_search((unsigned long)cur_pos, bisr);
    if (loop_fits_in_buffer && loop_loaded) return;
    else loop_loaded = 1;
    buffer_start_pos = (unsigned long)cur_pos;
    fill_buffer(buffer_start_pos);
}

void sampler::anti_clip_fwd()
{
    short* ac_m_tmp = 0;
    stereodata* ac_st_tmp = 0;
    if (do_ac == ON) { // anti clip already active
        if (ch == WAV_CH_MONO) {
            ac_m_tmp = new short[MAX_ANTI_CLIP_SIZE];
            ac_copy_fwd_mono(ac_m_tmp);
        } else {
            ac_st_tmp = new stereodata[MAX_ANTI_CLIP_SIZE];
            ac_copy_fwd_stereo(ac_st_tmp);
        }
    } // could use buff_pos, but recalculating gives better result:
    int abp = (unsigned long)(cur_pos - buffer_start_pos);
    if (abp + anti_clip_size > WAV_BUFFER_SIZE - 1) {
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
    out_l = out_r = 0;
    do_ac = ON;
    ac_cpstep = oldcpstp;
}

void sampler::anti_clip_rev()
{
    short* ac_m_tmp = 0;
    stereodata* ac_st_tmp = 0;
    if (do_ac == ON) { // anti clip already active
        if (ch == WAV_CH_MONO) {
            ac_m_tmp = new short[MAX_ANTI_CLIP_SIZE];
            ac_copy_rev_mono(ac_m_tmp);
        } else {
            ac_st_tmp = new stereodata[MAX_ANTI_CLIP_SIZE];
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
    out_l = out_r = 0;
    do_ac = ON;
    ac_cpstep = oldcpstp;
}

// ac_copy... methods are used when anticlip is activated while already
// active, there are four of them.  ac_copy_fwd... is called by
// anti_clip_fwd
// while ac_copy_rev... is called by anti_clip_rev, the relevant mono or
// stereo versions are called dependant on the wavfile being read.
void sampler::ac_copy_fwd_mono(short* ac_m_tmp)
{
    double tmp_acsz = ac_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos + i <= anti_clip_size && tmp_acsz <= 1) {
                ac_m_tmp[i]
                 = (short)(ac_m_buf[ac_buf_pos + i]*(1 - tmp_acsz));
                tmp_acsz += ac_step;
            }
            else ac_m_tmp[i] = 0;
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos - i >= 0 && tmp_acsz <= 1.0) {
                ac_m_tmp[i]
                 = (short)(ac_m_buf[ac_buf_pos - i]*(1 - tmp_acsz));
                tmp_acsz += ac_step;
            }
            else ac_m_tmp[i] = 0;
        }
    }
}

void sampler::ac_copy_fwd_stereo(stereodata* ac_st_tmp)
{
    double tmp_acsz = ac_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos + i <= anti_clip_size && tmp_acsz <= 1) {
                ac_st_tmp[i].left=(short)(ac_st_buf[ac_buf_pos + i].left
                 * (1 - tmp_acsz));
                ac_st_tmp[i].right=(short)(ac_st_buf[ac_buf_pos + i].right
                 * (1 - tmp_acsz));
                tmp_acsz += ac_step;
            } else {
                ac_st_tmp[i].left = 0;
                ac_st_tmp[i].right = 0;
            }
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos - i >= 0 && tmp_acsz <= 1) {
                ac_st_tmp[i].left =(short)(ac_st_buf[ac_buf_pos - i].left
                 * (1 - tmp_acsz));
                ac_st_tmp[i].right =(short)(ac_st_buf[ac_buf_pos
                 - i].right * (1 - tmp_acsz));
                tmp_acsz += ac_step;
            } else {
                ac_st_tmp[i].left = 0;
                ac_st_tmp[i].right = 0;
            }
        }
    }
}

void sampler::ac_mix_fwd_mono(short* ac_m_tmp)
{
    int i = 0;
    while (ac_size <= 1 && i <= anti_clip_size) {
        ac_m_buf[i] = (short)(ac_m_buf[i] * ac_size	+ ac_m_tmp[i]);
        ac_size += ac_step;
        i++;
    }
}

void sampler::ac_mix_fwd_stereo(stereodata* ac_st_tmp)
{
    int i = 0;
    while (ac_size < 1) {
        ac_st_buf[i].left = (short)(ac_st_buf[i].left * ac_size
         + ac_st_tmp[i].left);
        ac_st_buf[i].right = (short)(ac_st_buf[i].right * ac_size
         + ac_st_tmp[i].right);
        ac_size += ac_step;
        i++;
    }
}

void sampler::ac_copy_rev_mono(short* ac_m_tmp)
{
    double tmp_acsz = ac_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos + i <= anti_clip_size && tmp_acsz <= 1) {
                ac_m_tmp[anti_clip_size - i] =
                    (short)(ac_m_buf[ac_buf_pos + i]*(1 - tmp_acsz));
                tmp_acsz += ac_step;
            }
            else ac_m_tmp[anti_clip_size - i] = 0;
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos - i >= 0 && tmp_acsz <= 1.0) {
                ac_m_tmp[anti_clip_size - i] =
                    (short)(ac_m_buf[ac_buf_pos-i] * (1 - tmp_acsz));
                tmp_acsz += ac_step;
            }
            else ac_m_tmp[anti_clip_size - i] = 0;
        }
    }
}

void sampler::ac_copy_rev_stereo(stereodata* ac_st_tmp)
{
    double tmp_acsz = ac_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= anti_clip_size; i++) {
            if (ac_buf_pos + i <= anti_clip_size && tmp_acsz <= 1) {
                ac_st_tmp[anti_clip_size - i].left
                 = (short)(ac_st_buf[ac_buf_pos + i].left 
                 * (1 - tmp_acsz));
                ac_st_tmp[anti_clip_size - i].right
                 = (short)(ac_st_buf[ac_buf_pos + i].right 
                 * (1 - tmp_acsz));
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
                 = (short)(ac_st_buf[ac_buf_pos - i].left
                 * (1 - tmp_acsz));
                ac_st_tmp[anti_clip_size - i].right
                 = (short)(ac_st_buf[ac_buf_pos + i].right
                 * (1 - tmp_acsz));
                tmp_acsz += ac_step;
            } else {
                ac_st_tmp[anti_clip_size - i].left = 0;
                ac_st_tmp[anti_clip_size - i].right = 0;
            }
        }
    }
}

void sampler::ac_mix_rev_mono(short* ac_m_tmp)
{
    int i = anti_clip_size;
    while (ac_size <= 1 && i >= 0) {
        ac_m_buf[i] = (short)(ac_m_buf[i] * ac_size + ac_m_tmp[i]);
        ac_size += ac_step;
        i--;
    }
}

void sampler::ac_mix_rev_stereo(stereodata* ac_st_tmp)
{
    int i = anti_clip_size;
    while (ac_size <= 1 && i > 0) {
        ac_st_buf[i].left = (short)(ac_st_buf[i].left * ac_size
                                    + ac_st_tmp[i].left);
        ac_st_buf[i].right = (short)(ac_st_buf[i].right * ac_size
                                     + ac_st_tmp[i].right);
        ac_size += ac_step;
        i--;
    }
}

int sampler::sampler_count = 0;

#ifndef BARE_MODULES
bool sampler::done_params = false;

void sampler::create_params()
{
    if (done_params == true)
        return;
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_WAVFILEIN);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_PLAY_DIR);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_PLAY_MODE);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_JUMP_MODE);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_START_POS_MIN);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_START_POS_MAX);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_LOOP_MODE);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_LOOP_BEGIN);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_LOOP_END);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_LOOP_IS_OFFSET);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_LOOP_BI_OFFSET);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_ANTI_CLIP);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_AC_EACH_END);
    get_paramlist()->add_param(
     synthmodnames::MOD_SAMPLER, paramnames::PAR_ZERO_SEARCH_RANGE);
    get_paramlist()->
    add_param(synthmodnames::MOD_SAMPLER, paramnames::PAR_DEGSIZE_AMOUNT);
    done_params = true;
}
#endif
#endif
