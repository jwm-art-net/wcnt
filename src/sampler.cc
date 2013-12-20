#include "../include/sampler.h"
#include "../include/wavfilein.h"
#include "../include/globals.h"

#ifdef CRAZY_SAMPLER
#include <iostream>
#endif


/* YEAH GOOD LUCK DEBUGGING THIS SUCKER MWHAAA HA HA HA ! */

sampler::sampler(const char* uname) :
 synthmod::base(synthmod::SAMPLER, uname, SM_HAS_STEREO_OUTPUT),
 in_play_trig(0), in_stop_trig(0), in_start_pos_mod(0), in_phase_step(0),
 out_left(0), out_right(0),
 out_loop_trig(OFF), play_state(OFF),
 wavfile(0),
 play_dir(PLAY_FWD), play_mode(PLAY_STOP), jump_mode(JUMP_PLAY_DIR),
 min_start_pos(0), max_start_pos(0),
 loop_begin(0), loop_end(1), loop_is_offset(OFF), loop_mode(LOOP_OFF),
 loop_bi_offset(0),
 xfade_samples(0), xfade_each_end(OFF), search_range(0),
 phase_step_amount(1.0), root_phase_step(1.0),
 playdir(PLAY_FWD), acplaydir(PLAY_FWD), loop_yet(false),
 mono_buffer(0), xf_m_buf(0), st_buffer(0), xf_st_buf(0),
 buffer_start_pos(0), buff_pos(0),
 wavstart(1),  wavlength(0), wavstbi(0), wavlenbi(0),
 cur_pos(0), xf_cpstep(0), oldcpstep(0), cp_step(0), cp_ratio(0),
 sr_ratio(1), bp_midpoint(0), start_pos(0), startpos_span(0),
 loopstart(0), loopfinish(0), loop_fits_in_buffer(0),
 loop_loaded(0),
 do_ac(OFF), xf_cur_pos(0), xf_midpoint(0),
 xf_buf_pos(0), xf_buf_start_pos(0), xf_step(0), xf_size(0),
 xf_out_left(0), xf_out_right(0),
 ch(WAV_CH_UNKNOWN)
{
    register_output(output::OUT_LEFT);
    register_output(output::OUT_RIGHT);
    register_output(output::OUT_LOOP_TRIG);
    register_output(output::OUT_PLAY_STATE);
    sampletot = 0;
}

void sampler::register_ui()
{
    register_param(param::WAVFILEIN);
    register_param(param::PLAY_DIR, "fwd|rev")
                                            ->set_flags(ui::UI_OPTIONAL);
    register_input(input::IN_PLAY_TRIG);
    register_input(input::IN_STOP_TRIG)     ->set_flags(ui::UI_OPTIONAL);
    register_input(input::IN_PHASE_STEP);
    register_param(param::PHASE_STEP_AMOUNT)->set_flags(ui::UI_OPTIONAL);

    register_param(param::PLAY_MODE, "stop|wrap|bounce|jump")
                                            ->set_flags(ui::UI_GROUP1);
    register_param(param::JUMP_MODE, "play|loop")
                                            ->set_flags(ui::UI_GROUP1);

    //register_comment("[[ Static start position:");
    register_param(param::START_POS)        ->set_flags(ui::UI_OPTION1 | ui::UI_OPT_OPTIONAL);
    register_input(input::IN_START_POS_MOD) ->set_flags(ui::UI_OPTION1 | ui::UI_OPT_DUMMY);
    //register_comment("|| or modulated start position:");
    register_input(input::IN_START_POS_MOD) ->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    register_param(param::START_POS_MIN)    ->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    register_param(param::START_POS_MAX)    ->set_flags(ui::UI_OPTION2 | ui::UI_OPT_DUPLICATE);
    //register_comment("]]");

    //register_comment("[[ Optional loop:");
    register_param(param::LOOP_MODE, "off|fwd|rev|bi")
                                            ->set_flags(ui::UI_GROUP2);
    register_param(param::LOOP_BEGIN)       ->set_flags(ui::UI_GROUP2);
    register_param(param::LOOP_END)         ->set_flags(ui::UI_GROUP2);
    register_param(param::LOOP_IS_OFFSET)   ->set_flags(ui::UI_GROUP2 | ui::UI_OPTIONAL);
    register_param(param::LOOP_BI_OFFSET)   ->set_flags(ui::UI_GROUP2 | ui::UI_OPTIONAL);
    //register_comment("]]");

    //register_comment("[[ Optional xfades:");
    register_param(param::XFADE_SAMPLES)    ->set_flags(ui::UI_GROUP3);
    register_param(param::XFADE_EACH_END)   ->set_flags(ui::UI_GROUP3);
    register_param(param::ZERO_SEARCH_RANGE)->set_flags(ui::UI_GROUP3 | ui::UI_OPTIONAL);
    //register_comment("]]");
}

ui::moditem_list* sampler::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

sampler::~sampler()
{
    if (mono_buffer)
        delete [] mono_buffer;
    if (st_buffer)
        delete [] st_buffer;
    if (xf_m_buf)
        delete [] xf_m_buf;
    if (xf_st_buf)
        delete [] xf_st_buf;
}

const void* sampler::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_LEFT:         return &out_left;
        case output::OUT_RIGHT:        return &out_right;
        case output::OUT_LOOP_TRIG:    return &out_loop_trig;
        case output::OUT_PLAY_STATE:   return &play_state;
        default: return 0;
    }
}

const void* sampler::set_in(input::TYPE it, const void* o)
{
    switch(it)
    {
        case input::IN_PLAY_TRIG:
            return in_play_trig = (STATUS*)o;
        case input::IN_STOP_TRIG:
            return in_stop_trig = (STATUS*)o;
        case input::IN_PHASE_STEP:
            return in_phase_step = (double*)o;
        case input::IN_START_POS_MOD:
            return in_start_pos_mod = (double*)o;
        default:
            return 0;
    }
}

const void* sampler::get_in(input::TYPE it) const
{
    switch(it)
    {
        case input::IN_PLAY_TRIG:      return in_play_trig;
        case input::IN_STOP_TRIG:      return in_stop_trig;
        case input::IN_PHASE_STEP:     return in_phase_step;
        case input::IN_START_POS_MOD:  return in_start_pos_mod;
        default: return 0;
    }
}

bool sampler::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
    case param::WAVFILEIN:
        if (((dobj::base*)data)->get_object_type() != dobj::DEF_WAVFILEIN)
        {
            sm_err("%s is not a wavfilein.",
                    *((dobj::base*)data)->get_username());
            return false;
        }
        wavfile = (wavfilein*)data; // note: pointer.
        return true;
    case param::PLAY_DIR:
        play_dir = (PLAY_DIR)(*(int*)data);
        return true;
    case param::PLAY_MODE:
        play_mode = (PLAY_MODE)(*(int*)data);
        return true;
    case param::JUMP_MODE:
        jump_mode = (JUMP_DIR)(*(int*)data);
        return true;
    case param::START_POS:
        min_start_pos = *(samp_t*)data;
        max_start_pos = min_start_pos + 1;
        return true;
    case param::START_POS_MIN:
        min_start_pos = *(samp_t*)data;
        return true;
    case param::START_POS_MAX:
        max_start_pos = *(samp_t*)data;
        return true;
    case param::LOOP_BEGIN:
        loop_begin = *(samp_t*)data;
        return true;
    case param::LOOP_END:
        loop_end = *(samp_t*)data;
        return true;
    case param::LOOP_MODE:
        loop_mode = (LOOP_MODE)(*(int*)data);
        return true;
    case param::LOOP_IS_OFFSET:
        loop_is_offset = *(STATUS*)data;
        return true;
    case param::LOOP_BI_OFFSET:
        loop_bi_offset = *(wcint_t*)data;
        return true;
    case param::XFADE_SAMPLES:
        xfade_samples = *(wcint_t*)data;
        return true;
    case param::XFADE_EACH_END:
        xfade_each_end = *(STATUS*)data;
        return true;
    case param::ZERO_SEARCH_RANGE:
        search_range = *(wcint_t*)data;
        if(search_range == 1) search_range++;
        return true;
    case param::PHASE_STEP_AMOUNT:
        phase_step_amount = *(double*)data;
        return true;
    default:
        return false;
    }
}

const void* sampler::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::WAVFILEIN:         return wavfile;
        case param::PLAY_DIR:          return &play_dir;
        case param::PLAY_MODE:         return &play_mode;
        case param::JUMP_MODE:         return &jump_mode;
        case param::START_POS:         return &min_start_pos;
        case param::START_POS_MIN:     return &min_start_pos;
        case param::START_POS_MAX:     return &max_start_pos;
        case param::LOOP_BEGIN:        return &loop_begin;
        case param::LOOP_END:          return &loop_end;
        case param::LOOP_MODE:         return &loop_mode;
        case param::LOOP_IS_OFFSET:    return &loop_is_offset;
        case param::LOOP_BI_OFFSET:    return &loop_bi_offset;
        case param::XFADE_SAMPLES:     return &xfade_samples;
        case param::XFADE_EACH_END:    return &xfade_each_end;
        case param::ZERO_SEARCH_RANGE: return &search_range;
        case param::PHASE_STEP_AMOUNT: return &phase_step_amount;
        default: return 0;
    }
}

errors::TYPE sampler::validate()
{
    WAV_STATUS wavstatus = wavfile->open_wav();
    if (wavstatus == WAV_STATUS_NOT_FOUND) {

        sm_err("%s using wavfilein %s file %s not found.",
                param::names::get(param::FILENAME),
                wavfile->get_username(), wavfile->get_filename());
        invalidate();
        return errors::ERROR;
    }
    if (wavstatus == WAV_STATUS_WAVERR) {
        sm_err("%s using wavfilein %s file %s is not a WAV.",
                param::names::get(param::FILENAME),
                wavfile->get_username(), wavfile->get_filename());
        invalidate();
        return errors::ERROR;
    }
    if (wavstatus != WAV_STATUS_OPEN) {
        sm_err("%s using wavfilein %s file %s failed to open.",
                param::names::get(param::FILENAME),
                wavfile->get_username(), wavfile->get_filename());
        invalidate();
        return errors::ERROR;
    }

    if (!validate_param(param::START_POS_MIN, errors::RANGE_SAMPLE))
        return errors::RANGE_SAMPLE;

    if (!validate_param(param::START_POS_MAX, errors::RANGE_SAMPLE))
        return errors::RANGE_SAMPLE;

    if (max_start_pos < min_start_pos) {
        sm_err("%s must not be less than %s.",
                param::names::get(param::START_POS_MAX),
                param::names::get(param::START_POS_MIN));
        invalidate();
        return errors::ERROR;
    }
    if (loop_is_offset == OFF) {
        if (!validate_param(param::LOOP_BEGIN, errors::RANGE_SAMPLE))
            return errors::RANGE_SAMPLE;

        if (!validate_param(param::LOOP_END, errors::RANGE_SAMPLE))
            return errors::RANGE_SAMPLE;
    }
    if (loop_end <= loop_begin) {
        sm_err("%s must be more than %s.",
                param::names::get(param::LOOP_END),
                param::names::get(param::LOOP_BEGIN));
        invalidate();
        return errors::ERROR;
    }
    if (xfade_samples < 0
        || xfade_samples > wcnt::max_xfade_samples)
    {
        sm_err("%s out of range 0 ~ %d.",
                param::names::get(param::XFADE_SAMPLES),
                wcnt::max_xfade_samples);
        invalidate();
        return errors::ERROR;
    }
    if (!validate_param(param::ZERO_SEARCH_RANGE, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    if (!validate_param(param::LOOP_BI_OFFSET, errors::RANGE_COUNT))
        return errors::RANGE_COUNT;

    return errors::NO_ERROR;
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
        sr_ratio = (double)wavfile->get_sample_rate() / wcnt::jwm.samplerate();
        if (ch == WAV_CH_MONO) {
            mono_buffer = new double[wcnt::wav_buffer_size];
            if (!mono_buffer) {
                invalidate();
                return;
            }
            for (int i = 0; i < wcnt::wav_buffer_size; i++)
                mono_buffer[i] = 0;
            xf_m_buf = new double[wcnt::max_xfade_samples];
            if (!xf_m_buf) {
                invalidate();
                return;
            }
            for (int i = 0; i < wcnt::max_xfade_samples; i++)
                xf_m_buf[i] = 0;
        }
        else {
            st_buffer = new st_data[wcnt::wav_buffer_size];
            if (!st_buffer) {
                invalidate();
                return;
            }
            for (int i = 0; i < wcnt::wav_buffer_size; i++)
            {
                st_buffer[i].left = 0;
                st_buffer[i].right = 0;
            }
            xf_st_buf = new st_data[wcnt::max_xfade_samples];
            if (!xf_st_buf) {
                invalidate();
                return;
            }
            for (int i = 0; i < wcnt::max_xfade_samples; i++) {
                xf_st_buf[i].left = 0;
                xf_st_buf[i].right = 0;
            }
        }
    }
    else {
        sm_err("sampler %s has not got an open wav. Expect sigsegv\n",
                                                        get_username());
    }
    root_phase_step = wavfile->get_root_phase_step();
 // do a whole load of checks on the users input.
    int halfsr = search_range / 2;
    if (xfade_each_end == ON) {
        wavstart = zero_search(xfade_samples + halfsr, search_range);
        wavlength
         = zero_search(wavlength-xfade_samples-halfsr,search_range);
    } else {
        wavstart = zero_search(halfsr, search_range);
        wavlength = zero_search(wavlength - halfsr, search_range);
    }
    int bisr = search_range / wcnt::sr_div_bi;
    if (search_range > 0 && bisr < 2) bisr = 2;
    wavstbi = zero_search(wavstart + loop_bi_offset, bisr);
    wavlenbi = zero_search(wavlength - loop_bi_offset, bisr);
    if (min_start_pos - xfade_samples < 0)
        min_start_pos += xfade_samples;
    if (max_start_pos > wavlength)  max_start_pos = wavlength;
    startpos_span = max_start_pos - min_start_pos;
    start_pos = 4294967295ul;
    if (loop_mode != LOOP_OFF) {
        if (xfade_samples > 0) {
            if (loop_is_offset == OFF) {
                if (loop_begin < wavstart) loop_begin += xfade_samples;
                if (loop_end > wavlength) loop_end -= xfade_samples;
            }
        }
        if (loop_is_offset == OFF) {
            loop_begin = zero_search(loop_begin, search_range);
            loop_end = zero_search(loop_end, search_range);
        }
        int loopsize = ((loop_end >= 0) ? loop_end : -loop_end) -
                       ((loop_begin >= 0) ? loop_begin : -loop_begin);
        if (loopsize < wcnt::wav_buffer_size - 1)
            loop_fits_in_buffer = 1;
        else loop_fits_in_buffer = 0;
        loop_loaded = 0;
        if (loopsize != 0 && loopsize < xfade_samples)
            xfade_samples=(wcint_t)(loopsize / 2.1);
    }
    if (xfade_samples > 0)  xf_step = 1.00 / (double)xfade_samples;
    do_ac = OFF;
}

// read data from wav file into buffer
inline void sampler::fill_buffer(samp_t pos)
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
        buff_pos = (int)cur_pos - buffer_start_pos;
        #ifdef CRAZY_SAMPLER
        if (buff_pos < 0 || buff_pos >= wcnt::wav_buffer_size) {
            std::cout << "\n" << get_username() << "::run buff_pos = cur_pos(";
            std::cout << (double)cur_pos << ") - ";
            std::cout << "buffer_start_pos(" << buffer_start_pos << ")";
            std::cout << " = " << buff_pos;
        }
        #endif
        if (playdir == PLAY_FWD) {
            if (cur_pos >= wavlength) {
                pos_wavlen();
                buff_pos = (int)cur_pos - buffer_start_pos;
                #ifdef CRAZY_SAMPLER
                if (buff_pos < 0 || buff_pos >= wcnt::wav_buffer_size)
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
                buff_pos = (int)cur_pos - buffer_start_pos;
                #ifdef CRAZY_SAMPLER
                std::cout << "\n" << get_username() << "::run ";
                std::cout << "(playdir == PLAY_FWD && loopyet && cur_pos";
                std::cout << " > loopfinish >";
                std::cout << "buff_pos = cur_pos(" << cur_pos << ") - ";
                std::cout << "buffer_start_pos(" << buffer_start_pos << ")";
                std::cout << " = " << buff_pos;
                #endif
            }
            if (buff_pos >= wcnt::wav_buffer_size - 1) {
                buffer_start_pos = (samp_t)cur_pos;
                fill_buffer(buffer_start_pos);
                buff_pos = 0;
            }
        } else { // playdir == PLAY_REV
            if (cur_pos <= wavstart) {
                pos_wavstart();
                buff_pos = (int)cur_pos - buffer_start_pos;
            }
            else if (loop_yet && cur_pos < loopstart) {
                pos_loopbegin();
                buff_pos = (int)cur_pos - buffer_start_pos;
            }
            if (buff_pos <= 0) {
                if (cur_pos - (wcnt::wav_buffer_size - 2) >= 0) {
                    buffer_start_pos = (samp_t)
                        (cur_pos - (wcnt::wav_buffer_size -2));
                    fill_buffer(buffer_start_pos);
                    buff_pos = wcnt::wav_buffer_size - 2;
                } else {
                    fill_buffer(0);
                    buffer_start_pos = 0;
                    buff_pos = (int)cur_pos;
                }
            }
        }
        bp_midpoint = (double)((cur_pos - buffer_start_pos) - buff_pos);
        if (do_ac == ON) {
            //xf_cpstep = oldcpstep * (1 - xf_size) + cp_step * xf_size;
            if (xf_size > 1) do_ac = OFF;
            xf_midpoint = (double)((xf_cur_pos - xf_buf_start_pos)
                                                        - xf_buf_pos);
            #ifdef CRAZY_SAMPLER
            if ((xf_buf_pos > wcnt::max_xfade_samples - 1
              || xf_buf_pos < 0) && do_ac == ON)
            {
                std::cout << "\n" << get_username() << "::run ";
                std::cout << "xfade out of bounds of xfade buffer ";
                std::cout << xf_buf_pos;
                std::cout << "\n(samp_t)-1 = " << (samp_t)-1;
                std::cout << "\n(samp_t)xf_buf_pos = ";
                std::cout << (samp_t)xf_buf_pos;
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
            if (buff_pos + 1 >= wcnt::wav_buffer_size || buff_pos < 0)
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
                xf_out_left = calc_midpoint(xf_m_buf[xf_buf_pos],
                                            xf_m_buf[xf_buf_pos + 1],
                                            xf_midpoint);
                out_left = out_left * xf_size + xf_out_left * (1 - xf_size);
            }
            out_right = out_left;
        }
        else { // ch == WAV_CH_STEREO
            out_left = calc_midpoint(st_buffer[buff_pos].left,
             st_buffer[buff_pos + 1].left, bp_midpoint);
            out_right = calc_midpoint(st_buffer[buff_pos].right,
             st_buffer[buff_pos + 1].right, bp_midpoint);
            if (do_ac == ON) {
                xf_out_left = calc_midpoint(xf_st_buf[xf_buf_pos].left,
                                            xf_st_buf[xf_buf_pos + 1].left,
                                            xf_midpoint);
                xf_out_right= calc_midpoint(xf_st_buf[xf_buf_pos].right,
                                            xf_st_buf[xf_buf_pos + 1].right,
                                            xf_midpoint);
                out_left = out_left * xf_size + xf_out_left * (1 - xf_size);
                out_right= out_right * xf_size + xf_out_right * (1-xf_size);
            }
        } // endif ch == MONO/STEREO
        if (do_ac == ON) {
            if (acplaydir == PLAY_FWD) {
                xf_cur_pos += xf_cpstep;
                double xf_buf_posf = xf_cur_pos - xf_buf_start_pos;
                xf_buf_pos = (int)xf_buf_posf;
                xf_size = xf_buf_posf / xfade_samples;
            }
            else {
                xf_cur_pos -= xf_cpstep;
                double xf_buf_posf = xf_cur_pos - xf_buf_start_pos;
                xf_buf_pos = (int)xf_buf_posf;
                xf_size = (double)(xfade_samples - xf_buf_posf)
                                            / xfade_samples;
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
    if (play_state == ON && xfade_samples > 0) {
        if (playdir == PLAY_FWD) xfade_fwd();
        else xfade_rev();
    }
    playdir = play_dir;
    double spm = *in_start_pos_mod; // make input positive only
    spm = (spm > 0) ? spm : -spm;
    if (playdir == PLAY_FWD) {
        start_pos=(samp_t)(min_start_pos + startpos_span * spm);
        if (start_pos < wavstart) start_pos = wavstart;
        else if (start_pos > wavlength) start_pos = wavlength;
        cur_pos = start_pos = zero_search(start_pos, search_range);
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(PLAY_FWD) cur_pos = " << cur_pos;
        #endif
        buffer_start_pos = (samp_t)cur_pos;
    } else {
        start_pos=(samp_t)(max_start_pos - startpos_span * spm);
        if (start_pos < wavstart) start_pos = wavstart;
        else if (start_pos > wavlength) start_pos = wavlength;
        cur_pos = start_pos = zero_search(start_pos, search_range);
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(PLAY_REV) cur_pos = " << cur_pos;
        #endif
        if (cur_pos - (wcnt::wav_buffer_size - 2) < 0)
            buffer_start_pos = 0;
        else buffer_start_pos
          = (samp_t)(cur_pos - (wcnt::wav_buffer_size -2));
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
        if (loopfinish - loopstart < wcnt::wav_buffer_size - 2)
            loop_fits_in_buffer = 1;
        else loop_fits_in_buffer = 0;
        loop_loaded = 0;
    }
    play_state = ON;
}

samp_t sampler::zero_search(samp_t pos, wcint_t range)
{
    if (range <= 0) return pos;
    double smallest1 = 10;
    double smallest2 = 10;
    samp_t smallest_pos1 = 0;
    samp_t smallest_pos2 = 0;
    int halfsr = range / 2;
    samp_t buf_st_pos = pos - halfsr;
    samp_t buf_end_pos = buf_st_pos + range;
    if (ch == WAV_CH_MONO) {
        double* sm_buf = new double[wcnt::wav_buffer_size];
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
        st_data* sst_buf = new st_data[wcnt::wav_buffer_size];
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
    if (xfade_each_end == ON && xfade_samples > 0)
        xfade_fwd();
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
        if (play_mode == PLAY_WRAP) {
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
        buffer_start_pos = (samp_t)cur_pos;
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
    if (cur_pos - (wcnt::wav_buffer_size - 2) < 0)
        buffer_start_pos = 0;
    else buffer_start_pos
     = (samp_t)(cur_pos - (wcnt::wav_buffer_size -2));
    fill_buffer(buffer_start_pos);
}

void sampler::pos_loopend()
{// playdir = PLAY_FWD
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username() << "::pos_loopend";
    #endif
    out_loop_trig = ON;
    if (xfade_samples > 0)
        xfade_fwd();
    if (loop_mode == LOOP_FWD) {
        cur_pos = loopstart;
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(LOOP_FWD)cur_pos = " << cur_pos;
        #endif
        if (loop_fits_in_buffer && loop_loaded) return;
        else loop_loaded = 1;
        buffer_start_pos = (samp_t)cur_pos;
        fill_buffer(buffer_start_pos);
        return;
    }
    playdir = PLAY_REV;
    int bisr = search_range / wcnt::sr_div_bi;
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
        cur_pos = zero_search((samp_t)cur_pos, bisr);
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(cur_pos != wavstart)cur_pos = " << cur_pos;
        #endif
    }
    if (loop_fits_in_buffer && loop_loaded) return;
    else loop_loaded = 1;
    if (cur_pos - (wcnt::wav_buffer_size -2) < 0)
        buffer_start_pos = 0;
    else buffer_start_pos = (samp_t)
        (cur_pos - (wcnt::wav_buffer_size -2));
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
    if (xfade_each_end == ON && xfade_samples > 0)
        xfade_rev();
    loop_yet = 1;
    PLAY_DIR jumpdir;
    if (jump_mode == JUMP_PLAY_DIR)
        jumpdir = play_dir;
    else if (loop_mode == LOOP_FWD) jumpdir = PLAY_FWD;
    else jumpdir = PLAY_REV;
    if (play_mode == PLAY_WRAP ||
            (play_mode == PLAY_JUMP && jumpdir == PLAY_REV) ||
            (loopstart == 0 && loop_mode == LOOP_REV))
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
        if (cur_pos - (wcnt::wav_buffer_size - 2) < 0)
            buffer_start_pos = 0;
        else buffer_start_pos = (samp_t)
             (cur_pos - (wcnt::wav_buffer_size -2));
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
    buffer_start_pos = (samp_t)cur_pos;
    fill_buffer(buffer_start_pos);
}

void sampler::pos_loopbegin()
{// playdir == PLAY_REV
    #ifdef CRAZY_SAMPLER
    std::cout << "\n" << get_username() << "::pos_loopbegin";
    #endif
    out_loop_trig = ON;
    if (xfade_samples > 0)
        xfade_rev();
    if (loop_mode == LOOP_REV) {
        cur_pos = loopfinish;
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(LOOP_REV)cur_pos = " << cur_pos;
        #endif
        if (loop_fits_in_buffer && loop_loaded) return;
        else loop_loaded = 1;
        if (cur_pos - (wcnt::wav_buffer_size - 2) < 0)
            buffer_start_pos = 0;
        else buffer_start_pos = (samp_t)
            (cur_pos - (wcnt::wav_buffer_size -2));
        fill_buffer(buffer_start_pos);
        return;
    }
    playdir = PLAY_FWD;
    int bisr = search_range / wcnt::sr_div_bi;
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
        cur_pos = zero_search((samp_t)cur_pos, bisr);
        #ifdef CRAZY_SAMPLER
        std::cout << "\n" << get_username();
        std::cout << "(cur_pos != wavlength)cur_pos = " << cur_pos;
        #endif
    }
    if (loop_fits_in_buffer && loop_loaded) return;
    else loop_loaded = 1;
    buffer_start_pos = (samp_t)cur_pos;
    fill_buffer(buffer_start_pos);
}

void sampler::xfade_fwd()
{
    double* xf_m_tmp = 0;
    st_data* xf_st_tmp = 0;
    if (do_ac == ON) { // xfade already active
        if (ch == WAV_CH_MONO) {
            xf_m_tmp = new double[wcnt::max_xfade_samples];
            xf_copy_fwd_mono(xf_m_tmp);
        } else {
            xf_st_tmp = new st_data[wcnt::max_xfade_samples];
            xf_copy_fwd_stereo(xf_st_tmp);
        }
    } // could use buff_pos, but recalculating gives better result:
    int abp = (samp_t)(cur_pos - buffer_start_pos);
    if (abp + xfade_samples > wcnt::wav_buffer_size - 1) {
        if (ch == WAV_CH_MONO)
            wavfile->read_wav_chunk(
             xf_m_buf, (samp_t)cur_pos, xfade_samples + 1);
        else
            wavfile->read_wav_chunk(
             xf_st_buf,(samp_t) cur_pos, xfade_samples + 1);
    }
    else {   // get from wav buffer
        if (ch == WAV_CH_MONO) {
            for (int i = 0; i <= xfade_samples; i++)
                xf_m_buf[i] = mono_buffer[abp + i];
        }
        else {
            for (int i = 0; i <= xfade_samples; i++) {
                xf_st_buf[i].left = st_buffer[abp + i].left;
                xf_st_buf[i].right = st_buffer[abp + i].right;
            }
        }
    }
    if (do_ac == ON) { // now mix with new
        if (ch == WAV_CH_MONO) {
            xf_mix_fwd_mono(xf_m_tmp);
            delete [] xf_m_tmp;
        } else {
            xf_mix_fwd_stereo(xf_st_tmp);
            delete [] xf_st_tmp;
        }
    }
    acplaydir = PLAY_FWD;
    xf_cur_pos = cur_pos;
    xf_buf_pos = 0;
    xf_buf_start_pos = (samp_t)cur_pos;
    xf_out_left = xf_out_right = 0;
    xf_size = 0;
    do_ac = ON;
    xf_cpstep = oldcpstep;
}

void sampler::xfade_rev()
{
    double* xf_m_tmp = 0;
    st_data* xf_st_tmp = 0;
    if (do_ac == ON) { // xfade  already active
        if (ch == WAV_CH_MONO) {
            xf_m_tmp = new double[wcnt::max_xfade_samples];
            xf_copy_rev_mono(xf_m_tmp);
        } else {
            xf_st_tmp = new st_data[wcnt::max_xfade_samples];
            xf_copy_rev_stereo(xf_st_tmp);
        }
    }
    if (cur_pos - (xfade_samples - 1) < 0) xf_buf_start_pos = 0;
    else xf_buf_start_pos=(samp_t)(cur_pos - (xfade_samples - 1));
    xf_cur_pos = cur_pos;
    int acbp = (int)(cur_pos - buffer_start_pos) - (xfade_samples - 1);
    if (acbp <= 0) {
        if (ch == WAV_CH_MONO)
            wavfile->read_wav_chunk(xf_m_buf,
                                    xf_buf_start_pos, xfade_samples + 1);
        else
            wavfile->read_wav_chunk(xf_st_buf,
                                    xf_buf_start_pos, xfade_samples + 1);
    }
    else {
        if (ch == WAV_CH_MONO)
            for (int i = 0; i <= xfade_samples; i++)
                xf_m_buf[i] = mono_buffer[acbp + i];
        else
            for (int i = 0; i <= xfade_samples; i++) {
                xf_st_buf[i].left = st_buffer[acbp + i].left;
                xf_st_buf[i].right = st_buffer[acbp + i].right;
            }
    }
    if (do_ac == ON) { // now mix with new
        if (ch == WAV_CH_MONO) {
            xf_mix_rev_mono(xf_m_tmp);
            delete [] xf_m_tmp;
        } else {
            xf_mix_rev_stereo(xf_st_tmp);
            delete [] xf_st_tmp;
        }
    }
    acplaydir = PLAY_REV;
    xf_out_left = xf_out_right = 0;
    xf_step = 1.00 / (double)xfade_samples;
    xf_size = 0;
    xf_buf_pos = (int)(xf_cur_pos - xf_buf_start_pos);
    do_ac = ON;
    xf_cpstep = oldcpstep;
}

// xf_copy... methods are used when xfade is activated while already
// active, there are four of them.  xf_copy_fwd... is called by
// xfade_fwd
// while xf_copy_rev... is called by xfade_rev, the relevant mono or
// stereo versions are called dependant on the wavfile being read.
void sampler::xf_copy_fwd_mono(double* xf_m_tmp)
{
    double tmp_acsz = xf_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= xfade_samples; i++) {
            if (xf_buf_pos + i <= xfade_samples && tmp_acsz <= 1) {
                xf_m_tmp[i] = xf_m_buf[xf_buf_pos + i] * (1 - tmp_acsz);
                tmp_acsz += xf_step;
            }
            else xf_m_tmp[i] = 0;
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= xfade_samples; i++) {
            if (xf_buf_pos - i >= 0 && tmp_acsz <= 1.0) {
                xf_m_tmp[i] = xf_m_buf[xf_buf_pos - i] * (1 - tmp_acsz);
                tmp_acsz += xf_step;
            }
            else xf_m_tmp[i] = 0;
        }
    }
}

void sampler::xf_copy_fwd_stereo(st_data* xf_st_tmp)
{
    double tmp_acsz = xf_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= xfade_samples; i++) {
            if (xf_buf_pos + i <= xfade_samples && tmp_acsz <= 1) {
                xf_st_tmp[i].left = xf_st_buf[xf_buf_pos + i].left *
                                    (1 - tmp_acsz);
                xf_st_tmp[i].right= xf_st_buf[xf_buf_pos + i].right *
                                    (1 - tmp_acsz);
                tmp_acsz += xf_step;
            } else {
                xf_st_tmp[i].left = 0;
                xf_st_tmp[i].right = 0;
            }
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= xfade_samples; i++) {
            if (xf_buf_pos - i >= 0 && tmp_acsz <= 1) {
                xf_st_tmp[i].left = xf_st_buf[xf_buf_pos - i].left *
                                    (1 - tmp_acsz);
                xf_st_tmp[i].right =xf_st_buf[xf_buf_pos - i].right *
                                    (1 - tmp_acsz);
                tmp_acsz += xf_step;
            } else {
                xf_st_tmp[i].left = 0;
                xf_st_tmp[i].right = 0;
            }
        }
    }
}

void sampler::xf_mix_fwd_mono(double* xf_m_tmp)
{
    int i = 0;
    while (xf_size <= 1 && i <= xfade_samples) {
        xf_m_buf[i] = xf_m_buf[i] * xf_size + xf_m_tmp[i];
        xf_size += xf_step;
        i++;
    }
}

void sampler::xf_mix_fwd_stereo(st_data* xf_st_tmp)
{
    int i = 0;
    while (xf_size < 1) {
        xf_st_buf[i].left = xf_st_buf[i].left * xf_size +
                            xf_st_tmp[i].left;
        xf_st_buf[i].right = xf_st_buf[i].right * xf_size +
                             xf_st_tmp[i].right;
        xf_size += xf_step;
        i++;
    }
}

void sampler::xf_copy_rev_mono(double* xf_m_tmp)
{
    double tmp_acsz = xf_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= xfade_samples; i++) {
            if (xf_buf_pos + i <= xfade_samples && tmp_acsz <= 1) {
                xf_m_tmp[xfade_samples - i] =
                    xf_m_buf[xf_buf_pos + i]*(1 - tmp_acsz);
                tmp_acsz += xf_step;
            }
            else xf_m_tmp[xfade_samples - i] = 0;
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= xfade_samples; i++) {
            if (xf_buf_pos - i >= 0 && tmp_acsz <= 1.0) {
                xf_m_tmp[xfade_samples - i] =
                    xf_m_buf[xf_buf_pos-i] * (1 - tmp_acsz);
                tmp_acsz += xf_step;
            }
            else xf_m_tmp[xfade_samples - i] = 0;
        }
    }
}

void sampler::xf_copy_rev_stereo(st_data* xf_st_tmp)
{
    double tmp_acsz = xf_size;// <- preserve for mix
    if (acplaydir == PLAY_FWD) {
        for (int i = 0; i <= xfade_samples; i++) {
            if (xf_buf_pos + i <= xfade_samples && tmp_acsz <= 1) {
                xf_st_tmp[xfade_samples - i].left
                 = xf_st_buf[xf_buf_pos + i].left * (1 - tmp_acsz);
                xf_st_tmp[xfade_samples - i].right
                 = xf_st_buf[xf_buf_pos + i].right* (1 - tmp_acsz);
                tmp_acsz += xf_step;
            } else {
                xf_st_tmp[xfade_samples - i].left = 0;
                xf_st_tmp[xfade_samples - i].right = 0;
            }
        }
    } else { // acplaydir == PLAY_REV
        for (int i = 0; i <= xfade_samples; i++) {
            if (xf_buf_pos - i >= 0 && tmp_acsz <= 1) {
                xf_st_tmp[xfade_samples - i].left
                 = xf_st_buf[xf_buf_pos - i].left * (1 - tmp_acsz);
                xf_st_tmp[xfade_samples - i].right
                 = xf_st_buf[xf_buf_pos + i].right* (1 - tmp_acsz);
                tmp_acsz += xf_step;
            } else {
                xf_st_tmp[xfade_samples - i].left = 0;
                xf_st_tmp[xfade_samples - i].right = 0;
            }
        }
    }
}

void sampler::xf_mix_rev_mono(double* xf_m_tmp)
{
    int i = xfade_samples;
    while (xf_size <= 1 && i >= 0) {
        xf_m_buf[i] = xf_m_buf[i] * xf_size + xf_m_tmp[i];
        xf_size += xf_step;
        i--;
    }
}

void sampler::xf_mix_rev_stereo(st_data* xf_st_tmp)
{
    int i = xfade_samples;
    while (xf_size <= 1 && i > 0) {
        xf_st_buf[i].left = xf_st_buf[i].left * xf_size +
                            xf_st_tmp[i].left;
        xf_st_buf[i].right = xf_st_buf[i].right * xf_size +
                             xf_st_tmp[i].right;
        xf_size += xf_step;
        i--;
    }
}
