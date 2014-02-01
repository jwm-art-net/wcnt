#ifndef UI_ITEM_LIST_H
#define UI_ITEM_LIST_H

#include "connectorlist.h"
#include "fxsparamlist.h"
#include "textstuff.h"
#include "types.h"
#include "ui_item.h"

#include <string>
#include <sstream>

// currently ui_item.h contains all the information about how this works.


class connector;

namespace wcnt {
    fxsparamlist* get_fxsparamlist();
};

namespace ui
{
 #ifdef DEBUG
 #define ui_err(fmt, ... )                              \
 {                                                       \
    cfmt(item_list<T>::err_msg, STRBUFLEN, "%40s:%5d %-35s\n" fmt, \
                    __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);   \
 }
 #else
 #define ui_err(fmt, ... ) \
    cfmt(item_list<T>::err_msg, STRBUFLEN, fmt, __VA_ARGS__)
 #endif

 // T will be a pointer to synthmod::base or dobj::base


 template <class T>
 class item_list : public linked_list< base<T> >
 {
  public:
    item_list();
    ~item_list() {};

    typedef ll_item< base<T> > llitem;
    typedef linked_list< base<T> > linkedlist;

    input_item<T>*      add_input_item(input::TYPE);
    param_item<T>*      add_param_item(param::TYPE);
    param_item<T>*      add_param_item(param::TYPE, const char* fixed_str);
    dobj_item<T>*       add_dobj_item(dobj::TYPE parent, dobj::TYPE child);
    comment_item<T>*    add_comment_item(const char* comment);

    // specialist methods, used by synthmod::base, dobj::base impl is empty.
    connector* add_connector_off(T, input::TYPE);
    connector* add_connector_self(T, input::TYPE, output::TYPE);
    connector* add_connector_as(T, input::TYPE, input::TYPE itas);

    //  while the linkedlist provides methods for stepping through the list,
    //  these do the same but additionally maintain information about item
    //  groups and choices.
    base<T>* first_item();
    base<T>* next_item();

    bool item_is_choice_head()          { return (choice.head != 0); }
    bool item_is_choice_option_head()   { return (choice.opt0 != 0); }
    bool item_choice_ended()            { return track.choice_ended; }
    bool item_is_group_head()           { return (track.group_head != 0); }
    bool item_group_ended()             { return track.group_ended; }

    std::string get_item_header();

    // call match_begin at the start of each new module/data object
    // definition being read from a file. match_edit at the start
    // of editing via param_editor etc.
    void     match_begin(T t) {
        def_username = 0;
        match_begin_proper(t);
    }

    void     match_begin(T t, const char* username) {
        def_username = username;
        match_begin_proper(t);
    }

    void     match_edit(T);

    base<T>* match_item(const char*);

    // confirm all non-optional items have been matched etc,
    // returns 0 on success or error<T>::err();
    base<T>* validate_matches();

    // validates data for parameters, (T is the container)
    bool data_validate(T, param::TYPE, errors::TYPE);


  private:
    T       subject;

    FLAGS    skip_id;
    FLAGS    match_id;
    base<T>* item;
    base<T>* prev;
    base<T>* last;
    bool     editing;

    struct track_item
    {
        bool    choice_ended;

        llitem* group_head;
        FLAGS   group_id;
        bool    group_ended;

        #if DEBUG
        bool    choice_specified_dup;
        #endif
        void reset() {
            group_head = 0;
            group_id = UI_DEFAULT;
            #if DEBUG
            choice_specified_dup = false;
            #endif
        }
    } track;

    struct multiplechoice
    {
        llitem* head;       // first item of first option
        llitem* opt0;       // first item of current option
        llitem* chosen0;    // first item of chosen option
        base<T>* last;      // full circle (but not necessarily from start)
        FLAGS opt0id;
        FLAGS chosen0id;

        void reset() {
            head = opt0 = chosen0 = 0;
            last = 0;
            opt0id = chosen0id = UI_DEFAULT;
        }

        #if DEBUG
        void dump() {
            debug("option:\thead %p ", head);
            if (head)
                debug(" ('%s')", head->get_data()->get_name());
            debug("\topt0 %p", opt0);
            if (opt0)
                debug(" ('%s')", opt0->get_data()->get_name());
            debug("\tchosen0 %p", chosen0);
            if (chosen0)
                debug(" ('%s')", chosen0->get_data()->get_name());
            debug("\tlast %p", last);
            if (last)
                debug(" ('%s')", last->get_name());
            debug("\n");
        }
        #endif

    } choice;

    void    match_begin_proper(T);

    base<T>* match_item_choice(const char*);
    bool     match_item_choice_is_valid();
    base<T>* match_item_chosen(const char*);
    bool     match_item_choice_all_present(const char*);
    void     match_item_chosen_invalid_error();

    void goto_choice_head() {
        item = this->goto_item(choice.head);
        choice.opt0 = choice.head;
        choice.opt0id = item->get_option_id();
    }

    void skip_choice();

    bool validate_matches_choice();
    bool validate_matches_group();

    enum { LIST_STD = 0xff, LIST_EDIT = 0xfff };
    bool goodlist(int t) {
        if (listtype == 0)
            listtype = t;
        return (listtype == t);
    }

    int listtype;
    const char* def_username;
    static char err_msg[STRBUFLEN];
 };


 template <class T>
 item_list<T>::item_list() :
  linkedlist(MULTIREF_OFF),
  subject(0),
  skip_id(UI_DEFAULT), match_id(UI_DEFAULT),
  item(0), prev(0), last(0), editing(false),
  listtype(0), def_username(0)
 {
    debug("\n______________________________________________________"
          "\n^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*^&*\n");
    debug("item_list<T>::item_list<T>()\n");
    debug(" + err_msg: %p\n", err_msg);
    error_item<T>::err(err_msg);
 }

 template <class T>
 param_item<T>* item_list<T>::add_param_item(param::TYPE pt)
 {
    if (pt == param::STR_UNNAMED || pt == param::STR_LIST) {
        if (!goodlist(LIST_EDIT)) {
            return 0;
        }
    }
    else if (!goodlist(LIST_STD))
        return 0;

    param_item<T>* i = new param_item<T>(pt);
    if (i) {
        if (!this->add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    #ifdef DEBUG
    i->dump();
    #endif
    return i;
 }

 template <class T>
 param_item<T>* item_list<T>::add_param_item(param::TYPE pt, const char* fixstr)
 {
    if (!goodlist(LIST_STD))
        return 0;

    if (!fixstr)
        return 0;

    param_item<T>* i = new param_item<T>(pt);
    if (i) {
        llitem * li = this->add_at_tail(i);
        if (li) {
            if (!wcnt::get_fxsparamlist()->add_param(fixstr, pt)) {
                delete this->unlink_item(li);
                delete i;
                i = 0;
            }
        }
        else {
            delete i;
            i = 0;
        }
    }
    #ifdef DEBUG
    i->dump();
    #endif
    return i;
 }

 template <class T>
 dobj_item<T>* item_list<T>::add_dobj_item(dobj::TYPE parent, dobj::TYPE child)
 {
    if (!goodlist(LIST_STD))
        return 0;
    dobj_item<T>* i = new dobj_item<T>(parent, child);
    if (i) {
        if (!this->add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    #ifdef DEBUG
    i->dump();
    #endif
    return i;
 }

 template <class T>
 comment_item<T>* item_list<T>::add_comment_item(const char* comment)
 {
    comment_item<T>* i = new comment_item<T>(comment);
    if (i) {
        if (!this->add_at_tail(i)) {
            delete i;
            i = 0;
        }
    }
    #ifdef DEBUG
    i->dump();
    #endif
    return i;
 }


 template <class T>
 base<T>* item_list<T>::first_item()
 {
    this->track.reset();
    choice.reset();
    item = this->goto_first();

    if (!item)
        return 0;

    FLAGS id = item->get_option_id();

    if (id & UI_OPTION_MASK) {
        choice.head = choice.opt0 = this->sneak_current();
        choice.opt0id = id;
        #ifdef DEBUG
        if (item->should_duplicate())
            track.choice_specified_dup = true;
        #endif
    }
    else {
        id = item->get_group_id();
        if (id & UI_GROUP_MASK) {
            track.group_head = this->sneak_current();
            track.group_id = id;
        }
    }
    return item;
 }

 template <class T>
 base<T>* item_list<T>::next_item()
 {
    item = this->goto_next();
    //  the tracking used here is different to that used in match_item.
    //  the members pointing to the head of a group or choice are only
    //  set when pointing to the head of a group or choice. While this
    //  aids detection of head items, it means tracking state is moved
    //  to the corresponding members which store item IDs.

    if (!item)
        return 0;

    track.choice_ended = false;
    track.group_ended = false;
    choice.head = choice.opt0 = 0;
    track.group_head = 0;

    FLAGS id = item->get_option_id();

    if (choice.opt0id & UI_OPTION_MASK) {
        if (id & UI_OPTION_MASK) {
            if (id == UI_OPTION1 && choice.opt0id > UI_OPTION1) {
                #ifdef DEBUG
                if (!track.choice_specified_dup) {
                    debug("***** WARNING *****\n\tno items in multiple choice"
                          " marked to duplicate.\n");
                }
                #endif
                choice.reset();
                track.reset();
                track.choice_ended = true;
                choice.head = choice.opt0 = this->sneak_current();
                choice.opt0id = id;
            }
            else if (id > choice.opt0id) {
                choice.opt0 = this->sneak_current();
                choice.opt0id = id;
            }
            #ifdef DEBUG
            if (item->is_ui_opt_duplicate())
                track.choice_specified_dup = true;
            if (!this->sneak_next()
             && !track.choice_specified_dup) {
                debug("***** WARNING *****\n\tno items in multiple choice "
                      "marked to duplicate.\n");
            }
            #endif
        }
        else {
            #ifdef DEBUG
            if (!track.choice_specified_dup) {
                debug("***** WARNING *****\n\tno items in multiple choice "
                      "marked to duplicate.\n");
            }
            #endif
            choice.reset();
            track.reset();
            track.choice_ended = true;
        }
    }
    // allow fall from above..
    if (!(choice.opt0id & UI_OPTION_MASK)) {
        if (id & UI_OPTION_MASK) {
            choice.head = choice.opt0 = this->sneak_current();
            choice.opt0id = id;
            #ifdef DEBUG
            if (item->is_ui_opt_duplicate())
                track.choice_specified_dup = true;
            #endif
        }

        id = item->get_group_id();

        if (track.group_id & UI_GROUP_MASK) {
            if (id & UI_GROUP_MASK) {
                if (id != track.group_id) {
                    track.group_head = this->sneak_current();
                    track.group_id = id;
                }
            }
            else {
                choice.reset();
                track.reset();
                track.group_ended = true;
            }
        }
        else if (id & UI_GROUP_MASK) {
            track.group_head = this->sneak_current();
            track.group_id = id;
        }
    }

    return item;
 }


 template <class T>
 std::string item_list<T>::get_item_header()
 {
    std::string tmp = "";

    if (this->item_is_choice_option_head()) {
        if (this->item_is_choice_head()) {
            if (item->is_opt_optional())
                tmp = "//--{ multi-choice (optional):";
            else
                tmp = "//--{ multi-choice:";
        }
        else {
            std::ostringstream s;
            s << "//--{ or option " << item->get_option_no() << ":";
            tmp = s.str();
        }
    }
    else if (this->item_is_group_head()) {
        tmp = "//--{ group (optional):";
    }
    else if (this->item_choice_ended()) {
        tmp = "// multi-choice end }--";
    }
    else if (this->item_group_ended()) {
        tmp = "// group end }--";
    }

    return tmp;
 }


 template <class T>
 void item_list<T>::match_begin_proper(T t)
 {
    #ifdef DEBUG
    int n = 0;
    debug("\n----------------------------------------------- match_begin()\n");
    #endif

    editing = false;
    subject = t;

    if (!(item = this->goto_first()))
        return;

    do {
        item->reset_matched();
        #ifdef DEBUG
        llitem* li = this->sneak_current();
        debug("ll_item: %p\titem: %d\n", li, ++n);
        item->dump();
        #endif
    } while ((item = this->goto_next()) != 0);

    choice.reset();

    prev = last = 0;
    item = this->goto_first();
 }


 template <class T>
 void item_list<T>::match_edit(T t)
 {
    editing = true;
    subject = t;
    choice.reset();
    prev = last = 0;
    item = this->goto_first();
 }

 template <class T>
 base<T>* item_list<T>::match_item(const char* str)
 {
    if (!listtype || !str || this->is_empty())
        return 0;

    debug("match_item(%s)\n", str);

    base<T>* failchoice = 0;

restart:
    if (!item)
        item = this->goto_first();
    #ifdef DEBUG
    debug("passed restart\n");
    debug("item dump:\n");
    item->dump();
    debug("last dump:\n");
    if (last)
        last->dump();
    else
        debug("NULL\n");
    #endif

    if (last == item) {
        if (!(item = this->goto_next()))
            item = this->goto_first();
    }

    if (listtype == LIST_EDIT) {
        last = item;
        return item;
    }

    if (choice.opt0) {
        debug("\t(CHOICE.CUR0)\n");
        base<T>* i = match_item_choice(str);
        if (i)
            return i;
        failchoice = choice.head->get_data();
        #ifdef DEBUG
        debug("\t*+*+*+*+*+*+*+ \tlast:\n"); last->dump();
        debug("\t*+*+*+*+*+*+*+ \tchoice.last:\n"); choice.last->dump();
        debug("\t*+*+*+*+*+*+*+ \tfailchoice:\n"); failchoice->dump();
        #endif
        skip_choice();
    }

    do {
        if (item->get_item_type() != UI_COMMENT) {

            #ifdef DEBUG
            debug("match_item loop on:\n");
            item->dump();
            #endif

            FLAGS id = item->get_option_id();

            if (id & UI_OPTION_MASK) {
                if (item == failchoice) {
                    debug("\tFAILCHOICE BREAK 1\n");
                    break;
                }
                choice.head = choice.opt0 = this->sneak_current();
                choice.opt0id = id;
                choice.chosen0 = 0;
                choice.chosen0id = UI_DEFAULT;
                choice.last = 0;
                debug("/////////// choice.head llitem: %p\n", choice.head);
                base<T>* ic = match_item_choice(str);
                if (ic)
                    return ic;
                if (!failchoice) {
                    failchoice = choice.head->get_data();
                    #ifdef DEBUG
                    debug("\t*+*+*+*+*+*+*+ \tfailchoice: ");
                    failchoice->dump();
                    #endif
                }
                debug("resetting choice before restart...\n");
                skip_choice();
                if (item == failchoice) {
                    debug("\tFAILCHOICE BREAK 2\n");
                    break;
                }
                goto restart;
            }

            if (item->is_name_match(str)) {
                if (!editing && item->is_matched()) {
                    if (def_username && strcmp(def_username, str) == 0)
                        return username_item<T>::username();
                    ui_err("Duplicate item: %s.", item->get_name());
                    return error_item<T>::err();
                }
                debug("item matched, not choice/group\n");
                item->set_matched();
                last = item;
                return item;
            }
        }
        #ifdef DEBUG
        else {
            debug("skipping comment '%s'\n", item->get_descr());
        }
        #endif

        item = this->goto_next();
        if (last && !item)
            item = this->goto_first();

    } while (item != last);

    if (def_username && strcmp(def_username, str) == 0)
        return username_item<T>::username();

    ui_err("No item found matching '%s'.", str);
    return error_item<T>::err();
 }

// match_item_choice, called by match_item, when choice0 is non-zero
// choice0 is only non-zero once a string has been matched with a choice-item
//
 template <class T>
 base<T>* item_list<T>::match_item_choice(const char* str)
 {
    char buf[80];
    size_t l = strlen(str);

    if (l > 80) {
        strncpy(buf, str, 80);
        buf[79] = '\0';
        l = 79;
    }
    else
        strcpy(buf, str);

    bool forced = false;

    if (buf[l - 1] == '!') {
        buf[l - 1] = '\0';
        forced = true;
    }

    str = buf;

    #ifdef DEBUG
    debug("match_item_choice(str:'%s', item: '%s')\n", str, item->get_name());
    debug("-------------------- choice.head llitem: %p\n", choice.head);
    #endif

    if (!choice.last)
        choice.last = item;
    else if (choice.last == item) {
        if (!(item = this->goto_next()))
            this->goto_choice_head();
        //choice.last = item;
    }
    #ifdef DEBUG
    debug("\t!^!^!^!^!^!^!^ \tchoice.last:\n");
    if (choice.last) choice.last->dump(); else debug("NULL\n");
    #endif

    do {
        if (item->get_item_type() != UI_COMMENT) {
            #ifdef DEBUG
            debug("   match_item_choice\n");
            item->dump();
            #endif

            FLAGS id = item->get_option_id();

            if (id & UI_OPTION_MASK) {
                if (choice.opt0id > UI_OPTION1 && id == UI_OPTION1) {
                    debug("\t*1*CHOICETAIL == ITEM (%p)\n", item);
                    this->goto_choice_head();
                    continue;
                }
                if (id > choice.opt0id) {
                    choice.opt0 = this->sneak_current();
                    choice.opt0id = id;
                }

                if (!item->is_dummy()) {
                    if (!choice.chosen0) {
                        if (item->is_matched() && !item->was_forced()) {
                            if (def_username && strcmp(def_username, str) == 0)
                                return username_item<T>::username();
                            debug("detected previously matched choice item\n");
                            choice.chosen0 = choice.opt0;
                            choice.chosen0id = choice.opt0id;
                        }
                    }

                    if (item->is_name_match(str)) {
                        if (!editing && item->is_matched()) {
                            if (def_username && strcmp(def_username, str) == 0)
                                return username_item<T>::username();
                            ui_err("Item '%s' already specified.", item->get_name());
                            return error_item<T>::err();
                        }
                        if (forced)
                            item->set_forced();
                        else {
                            if (!choice.chosen0) {
                                choice.chosen0 = choice.opt0;
                                choice.chosen0id = choice.opt0id;
                            }
                            else if (id != choice.chosen0id) {
                                debug("validity failed\n");
                                match_item_chosen_invalid_error();
                                return error_item<T>::err();
                            }
                        }
                        item->set_matched();
                        choice.last = item;
                        last = choice.head->get_data();
                        return item;
                    }
                }
            }
            else {
                debug("\t*2*CHOICETAIL == ITEM (%p)\n", item);
                this->goto_choice_head();
                continue;
            }
        }
        #ifdef DEBUG
        else {
            debug("skipping comment '%s'\n", item->get_descr());
        }
        #endif

        item = this->goto_next();

        if (choice.last && !item) {
            debug("out of items. goto choice.head\n");
            this->goto_choice_head();
        }
    } while(item != choice.last);

    // only one reason we are here:
    //      * there was no item matching str *in the choice*
    // but several reasons why that might be so,
    // so delay judgement for now...
   debug("returning from choice with nothing!\n");

    return 0;
 }


 template <class T>
 void item_list<T>::match_item_chosen_invalid_error()
 {
    std::string errstr = "";
    int items = 0;

    llitem* li = choice.chosen0;

    do {
        base<T>* it = li->get_data();
        if (item->get_item_type() != UI_COMMENT && !item->is_dummy()) {
            if (it->get_option_id() != choice.chosen0id)
                break;
            if (it->is_matched()) {
                debug("adding error item:'%s'\n", it->get_name());
                errstr += (items == 0 ? "'" : ", '");
                errstr += it->get_name();
                errstr += "'";
                ++items;
            }
        }
        li = li->get_next();
    } while (li);

    ui_err("Specifying item '%s' is invalidated by previous selection of %s %s.",
            item->get_name(), (items > 1 ? "items" : "item"), errstr.c_str());
 }


 template <class T>
 base<T>* item_list<T>::validate_matches()
 {
    if (listtype == LIST_EDIT)
        return 0;

    item = this->goto_first();
    debug("\n(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)\nvalidate_matches()\n");

    while (item) {
            #ifdef DEBUG
            debug("validate_matches:\n");
            item->dump();
            #endif
        if (item->get_item_type() == UI_COMMENT)
            item = this->goto_next();
        else {

            FLAGS id = item->get_option_id();
            if (id & UI_OPTION_MASK) {
                choice.reset();
                if (!validate_matches_choice())
                    return error_item<T>::err();
                item = this->sneak_current()->get_data();
            }
            else if ((id = item->get_group_id()) & UI_GROUP_MASK) {
                if (!validate_matches_group())
                    return error_item<T>::err();
                item = this->sneak_current()->get_data();
            }
            else {
                if (!item->is_matched()) {
                    if (item->is_optional()) {
                        if (*item == UI_INPUT) {
                            input_item<T>* in = static_cast<input_item<T>*>(item);
                            output::TYPE ot = in->get_self_connect();
                            if (ot != output::ERR_TYPE)
                                add_connector_self(subject, in->get_input_type(), ot);
                            else {
                                input::TYPE it = in->get_connect_as();
                                if (it != input::ERR_TYPE)
                                    add_connector_as(subject, in->get_input_type(), it);
                                else
                                    add_connector_off(subject, in->get_input_type());
                            }
                        }
                    }
                    else {
                        ui_err("Item '%s' is non-optional.", item->get_name());
                        return error_item<T>::err();
                    }
                }
                item = this->goto_next();
            }
        }
    }
    return 0;
 }


 template <class T>
 bool item_list<T>::validate_matches_choice()
 {
    // this does not need to test for invalid choice selection (ie
    // items matched from *both* choice 1 *and* choice 2) as that is
    // handled by the initial match_item process.


    // FIXME:   when one of the options in a choice contains an input,
    //          detect when developer has not added a dummy option to
    //          any choices which allow the input to not be specified.
    //          (nb, otherwise its a bit of a bugger to debug!)
    // *****    and, not forgetting UI_OPT_DUPLICATE

    debug("\n(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)\nvalidate_matches_choice()\n");


    bool choice_optional = item->is_optional();
    choice.head = choice.opt0 = this->sneak_current();
    choice.opt0id = item->get_option_id();

    // initial pass of choice selection, detects head
    // and chosen... and the no-choice-selected scenario
    // where selection of a choice is non-optional.

    while (item) {
        if (item->get_item_type() != UI_COMMENT) {
            #ifdef DEBUG
            debug("validate_matches_choice:\n");
            item->dump();
            #endif

            FLAGS id = item->get_option_id();
            if ((choice.opt0id > UI_OPTION1 && id == UI_OPTION1)
             || id == UI_DEFAULT)
                break;
            if (id > choice.opt0id) {
                choice.opt0 = this->sneak_current();
                choice.opt0id = id;
            }
            if (item->is_matched() && !choice.chosen0) {
                choice.chosen0 = choice.opt0;
                choice.chosen0id = choice.opt0id;
                break;
            }
        }
        item = this->goto_next();
    }

    if (choice_optional && !choice.chosen0) {
        debug("choice_optional && !choice.chosen0\n");
        // selection of the choice was optional and this fact
        // has been taken advantage of.  all that needs to be
        // done is to add off-connectors for any inputs in the
        // choice.
        item = this->goto_item(choice.head);
        while (item) {
            if (item->get_item_type() != UI_COMMENT) {
                #ifdef DEBUG
                debug("validate_matches_choice(2):\n");
                item->dump();
                #endif
                FLAGS id = item->get_option_id();
                if ((choice.opt0id > UI_OPTION1 && id == UI_OPTION1)
                 || id == UI_DEFAULT)
                    break;
                if (*item == UI_INPUT) {
                    // add_connector_off is specialized.
                    input_item<T>* in = static_cast<input_item<T>*>(item);
                    output::TYPE ot = in->get_self_connect();
                    if (ot != output::ERR_TYPE)
                        add_connector_self(subject, in->get_input_type(), ot);
                    else {
                        input::TYPE it = in->get_connect_as();
                        if (it != input::ERR_TYPE)
                            add_connector_as(subject, in->get_input_type(), it);
                        else
                            add_connector_off(subject, in->get_input_type());
                    }
                }
                item = this->goto_next();
            }
        }
        return true;
    }

    // second pass
    debug("second pass... \n");
    std::string errstr = "";
    std::string goodstr = "";
    int errors = 0;
    int goods = 0;

    item = this->goto_item(choice.chosen0);

    while (item) {
        #ifdef DEBUG
        debug("validate\n");
        item->dump();
        #endif
        if (item->get_item_type() != UI_COMMENT) {
            #ifdef DEBUG
            debug("validate_matches_choice(3):\n");
            item->dump();
            #endif
            FLAGS id = item->get_option_id();
            if (id != choice.chosen0id) {
                debug("breakout, no longer in chosen\n");
                break;
            }
            debug("processing...\n");
            if (item->is_matched()) {
                goodstr += (goods == 0 ? "'" : ", '");
                goodstr += item->get_name();
                goodstr += "'";
                ++goods;
            }
            else {
                if (!item->is_optional() && !item->is_dummy()) {
                    errstr += (errors == 0 ? "'" : ", '");
                    errstr += item->get_name();
                    errstr += "'";
                    ++errors;
                }
                else if (*item == UI_INPUT) {
                    input_item<T>* in = static_cast<input_item<T>*>(item);
                    output::TYPE ot = in->get_self_connect();
                    if (ot != output::ERR_TYPE)
                        add_connector_self(subject, in->get_input_type(), ot);
                    else {
                        input::TYPE it = in->get_connect_as();
                        if (it != input::ERR_TYPE)
                            add_connector_as(subject, in->get_input_type(), it);
                        else
                            add_connector_off(subject, in->get_input_type());
                    }
                }
            }
        }
        item = this->goto_next();
    }

    if (errors) {
        ui_err("The following %s %s should have been specified "
               "alongside %s %s",
                (errors > 1 ? "items" : "item"), errstr.c_str(),
                (goods > 1 ? "items" : "item"), goodstr.c_str());
        return false;
    }

    return true;
 }


template <class T>
 bool item_list<T>::validate_matches_group()
 {
    bool selected = false;
    bool unmatched = false;
    llitem* group0 = this->sneak_current();
    FLAGS group0id = item->get_group_id();

    debug("\n(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)(*)\nvalidate_matches_group()\n");

    while (item) {
        if (item->get_item_type() != UI_COMMENT) {
            #ifdef DEBUG
            debug("validate_matches_group:\t");
            item->dump();
            #endif
            if (item->get_group_id() != group0id)
                break;
            if (item->is_matched()) {
                selected = true;
                debug("selected\n");
            }
            else {
                if (!item->is_optional()) {
                    unmatched = true;
                    debug("unmatched\n");
                }
                if (*item == UI_INPUT) {
                    debug("adding off connector...\n");
                    // optimized for non-error situation
                    input_item<T>* in = static_cast<input_item<T>*>(item);
                    output::TYPE ot = in->get_self_connect();
                    if (ot != output::ERR_TYPE)
                        add_connector_self(subject, in->get_input_type(), ot);
                    else {
                        input::TYPE it = in->get_connect_as();
                        if (it != input::ERR_TYPE)
                            add_connector_as(subject, in->get_input_type(), it);
                        else
                            add_connector_off(subject, in->get_input_type());
                    }
                }
            }
        }
        item = this->goto_next();
    }

    if (!selected || (selected && !unmatched))
        return true;


    std::string errstr = "";
    std::string goodstr = "";
    int errors = 0;
    int goods = 0;

    item = this->goto_item(group0);

    while (item) {
        if (item->get_item_type() != UI_COMMENT) {
            #ifdef DEBUG
            debug("validate_matches_group(2):\t");
            item->dump();
            #endif
            if (item->get_group_id() != group0id)
                break;
            if (item->is_matched()) {
                goodstr += (goods == 0 ? "'" : ", '");
                goodstr += item->get_name();
                goodstr += "'";
                ++goods;
            }
            else if (!item->is_optional()) {
                errstr += (errors == 0 ? "'" : ", '");
                errstr += item->get_name();
                errstr += "'";
                ++errors;
            }
        }
        item = this->goto_next();
    }

    ui_err("The following %s %s should have been specified alongside %s %s",
            (errors > 1 ? "items" : "item"), errstr.c_str(),
            (goods > 1 ? "items" : "item"), goodstr.c_str());
    return false;
 }


 template <class T>
 bool item_list<T>::data_validate(T t, param::TYPE pt, errors::TYPE et)
 {
    base<T>* i = this->goto_first();

    while(i) {
        if (*i == ui::UI_PARAM) {
            param_item<T>* p = static_cast<param_item<T>*>(i);
            if (*p == pt)
                return i->validate(t, et);
        }
        i = this->goto_next();
    }
    // failed to validate due to subject lacking parameter type
    return false;
 }

 template <class T>
 void item_list<T>::skip_choice()
 {
    FLAGS cid = item->get_option_id();
    bool wrap = false;
    debug("\t\t\tSKIPPING CHOICE (\n");
    do {
        if (item->get_item_type() != UI_COMMENT) {
            if (wrap && this->sneak_current() == choice.head)
                break;
            FLAGS id = item->get_option_id();
            if ((id == UI_OPTION1 && cid > UI_OPTION1)
              || id == UI_DEFAULT)
                break;
            cid = id;
            debug("'%s'\n", item->get_name());
        }
        if (!(item = this->goto_next())) {
            item = this->goto_first();
            wrap = true;
            break;
        }
    } while (true);

    debug("\t\t\t) SKIPPED CHOICE\n");

    choice.reset();
 }


};

#endif
