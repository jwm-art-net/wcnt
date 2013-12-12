#include "../include/stereomixer.h"
#include "../include/dobjmod.h"
#include "../include/dobjlist.h"

stereomixer::stereomixer(const char* uname) :
 synthmod::base(synthmod::STEREOMIXER, uname, SM_HAS_STEREO_OUTPUT),
 linkedlist(MULTIREF_ON, PRESERVE_DATA),
 out_left(0), out_right(0), master_level(0.75),
 chans_left(0), chans_right(0)
{
    register_output(output::OUT_LEFT);
    register_output(output::OUT_RIGHT);
}

void stereomixer::register_ui()
{
    register_dobj(dobj::LST_MIX, dobj::DOBJ_SYNTHMOD);
    register_param(param::MASTER_LEVEL);
}

ui::moditem_list* stereomixer::get_ui_items()
{
    static ui::moditem_list items;
    return &items;
}

stereomixer::~stereomixer()
{
    if (chans_left)
        delete [] chans_left;
    if (chans_right)
        delete [] chans_right;
}

const void* stereomixer::get_out(output::TYPE ot) const
{
    switch(ot)
    {
        case output::OUT_LEFT:  return &out_left;
        case output::OUT_RIGHT: return &out_right;
        default: return 0;
    }
}

bool stereomixer::set_param(param::TYPE pt, const void* data)
{
    switch(pt)
    {
        case param::MASTER_LEVEL:
            master_level = *(double*)data;
            return true;
        default:
            return false;
    }
}

const void* stereomixer::get_param(param::TYPE pt) const
{
    switch(pt)
    {
        case param::MASTER_LEVEL: return &master_level;
        default: return 0;
    }
}

synthmod::base*
stereomixer::duplicate_module(const char* uname, DUP_IO dupio)
{
    return duplicate_list_module(this, goto_first(), uname, dupio);
}

errors::TYPE stereomixer::validate()
{
    if (master_level == 0) {
        // FIXME: should probably allow zero amplitude...
        sm_err("%s is zero, all will be very quiet!",
                param::names::get(param::MASTER_LEVEL));
        invalidate();
        return errors::ERROR;
    }
    return errors::NO_ERROR;
}

dobj::base* stereomixer::add_dobj(dobj::base* dbj)
{
    if (dbj->get_object_type() == dobj::DOBJ_SYNTHMOD) {
        synthmod::base* sm = ((dobjmod*)dbj)->get_synthmod();
        if (!sm) {
            sm_err("%s Attempt made to add NULL module to mixer",
                errors::stock::major);
            return 0;
        }
        if (sm->get_module_type() != synthmod::STEREOCHANNEL
            && !sm->get_out(output::OUT_LEFT))
        {
            sm_err("%s will not accept the module %s because modules of "
                    "type %s do not have the %s or %s output types.",
                    get_username(), sm->get_username(),
                    synthmod::names::get(sm->get_module_type()),
                    output::names::get(output::OUT_LEFT),
                    output::names::get(
                                                output::OUT_RIGHT));
            return 0;
        }
        if (!add_at_tail(sm)) {
            sm_err("Could not insert %s into mixer.", sm->get_username());
            return 0;
        }
        // add the dobj synthmod wrapper to the dobjlist
        // so it gets deleted in the end.
        wcnt::get_dobjlist()->add_dobj(dbj);
        return dbj;
    }
    sm_err("%s %s to %s", errors::stock::major, errors::stock::bad_add,
                                                    get_username());
    return 0;
}

void stereomixer::init()
{
    chans_left  = new double const*[get_count() + 1];
    chans_right = new double const*[get_count()];
    synthmod::base* sm = goto_first();
    long ix = 0;
    while(sm) {
        chans_left[ix] =
            (double const*)sm->get_out(output::OUT_LEFT);
        chans_right[ix] =
            (double const*)sm->get_out(output::OUT_RIGHT);
        ix++;
        sm = goto_next();
    }
    chans_left[ix] = 0;
    empty_list();
}

void stereomixer::run()
{
    out_left = out_right = 0;
    for (long ix = 0; chans_left[ix]; ix++){
        out_left  += *chans_left[ix];
        out_right += *chans_right[ix];
    }
    out_left  *= master_level;
    out_right *= master_level;
}
