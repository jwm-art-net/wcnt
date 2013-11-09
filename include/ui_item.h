#ifndef UI_ITEM_H
#define UI_ITEM_H

#include "checkvalue.h"
#include "dobjnames.h"
#include "inputnames.h"
#include "paramnames.h"
#include "stockerrs.h"
#include "synthmodnames.h"
#include "linkedlist.h"

// a mechanism for registering the UI components of a module or data object.
// descriptions can be added to items to supplant a predefined description
// (ie where a type such as an output is so generic the description is
// meaningless).

// items may also be arranged in groups if they are optional, or arranged
// as multiple-choices.


#include <iostream>

namespace ui
{

 enum TYPE {
    UI_ERROR,
    UI_COMMENT,
    UI_INPUT,
    UI_PARAM,
    UI_DOBJ
 };

 enum FLAGS {
    UI_DEFAULT =    0x0000,
    // items marked optional allow the user to not specify an item nor
    // be demanded to do so by the user interface.note that the ui plays
    // no part in the initialization of non specified items - with one
    // exception: an off-connector is added for non specified inputs.
    UI_OPTIONAL =   0x0001,

    // items marked as as choice means the user must choose one and only one
    // of several choices. one choice might be one item, or one choice
    // might be several items such that:
    //      choice 1: (item1 AND item2 AND item3)
    //  OR
    //      choice 2: (item4 AND item5)
    // additionally, if the first item in the first choice is also specified
    // as UI_OPTIONAL, then the user may ommit specifying any choice at all.
    // the first choice should always be UI_CHOICE1, and second UI_CHOICE2.
    // if an item with UI_CHOICE1 is encountered immediately after a
    // UI_CHOICE1 + n item, a new selection of choices is assumed.

    UI_CHOICE1 =    0x0010,
    UI_CHOICE2 =    0x0020,
    UI_CHOICE3 =    0x0040,
    UI_CHOICE4 =    0x0080,

    // items can be grouped together by setting one of the UI_GROUPn flags.
    // allowing the whole group of items in the user interface to either
    // all be specified or all absent. additionally, specific items within
    // a group can be be made optional by use of UI_OPTIONAL. Potentially,
    // there can be any number of groups within a container as long as there
    // is at least one non-group item between each group (excluding comment
    // items).  Where distinct groups immediatelly follow each other, then
    // only four distinct but adjaecent groups can be accommodated.

    UI_GROUP1 =     0x0100,
    UI_GROUP2 =     0x0200,
    UI_GROUP3 =     0x0400,
    UI_GROUP4 =     0x0800,

    // items cannot be part of a group and choice simultaneously.

    // implementation only:
    UI_CHOICE_MASK= 0x00f0,
    UI_GROUP_MASK = 0x0f00,
    UI_MATCHED =    0x8000,
 };

 enum {
    ITEM_CHOICE_UNEXPECTED = -5,// first item chosen, and so was this.
    ITEM_CHOICE_EXPECTED = -4,  // first item chosen but this wasnt.
    ITEM_GROUP_UNEXPECTED = -3,   // first item skipped but this wasnt.
    ITEM_GROUP_EXPECTED = -2,     // first item specified but this wasnt.
    ITEM_UNEXPECTED = -1,       // item did not match
    ITEM_MATCH = 0,             // item did match
    ITEM_SKIPPED                // item was optional and skipped
 };

 template <class T>
 class base
 {
  public:
    base(TYPE it);
    base(TYPE it, const char* descr_literal);

    virtual ~base() {};
    virtual bool validate(T, errors::TYPE) = 0;

    TYPE    get_item_type() const       { return itemtype; }
    bool operator==(TYPE it) const      { return (itemtype == it); }

    virtual const char* get_name()      { return 0; }

    base<T>* add_descr(const char* literal) { descr = literal; return this; }
    virtual const char* get_descr()     { return descr; }

    bool is_name_match(const char* str) { return name_match(str); }

    base<T>* set_flags(int f);
    bool is_optional()      { return !!(flags & UI_OPTIONAL); }
    bool has_flag(FLAGS f)  { return !!(flags & f); }

    void reset_matched()    { flags &= ~UI_MATCHED; }
    void set_matched()      { flags |= UI_MATCHED; }
    bool is_matched()       { return !!(flags & UI_MATCHED); }

    FLAGS get_choice_id() {
        return static_cast<FLAGS>(flags & UI_CHOICE_MASK);
    }
    FLAGS get_group_id() {
        return static_cast<FLAGS>(flags & UI_GROUP_MASK);
    }

    #ifdef DEBUG
    virtual void dump() {};
    #endif
  protected:
    virtual bool name_match(const char*) = 0;

  private:
    int         flags;
    TYPE        itemtype;
    const char* descr;
    base() {};
    base(const base&) {};
 };

 template <class T>
 base<T>::base(TYPE it)
  : flags(0), itemtype(it), descr(0)
 {
 }

 template <class T>
 base<T>::base(TYPE it, const char* str)
  : flags(0), itemtype(it), descr(str)
 {
 }

 template <class T>
 base<T>* base<T>::set_flags(int f)
 {
    flags = f;

    #ifdef DEBUG
    if (f < 0) {
        this->dump();
        std::cout << "invalid ui item flags: " << f << std::endl;
        return 0;
    }

    int group = 0;
    group += !!(f & UI_GROUP1);
    group += !!(f & UI_GROUP2);
    group += !!(f & UI_GROUP3);
    group += !!(f & UI_GROUP4);

    if (group > 1) {
        this->dump();
        std::cout << "invalid ui item flags: " << f
                  << "item has more than one set-id bit set." << std::endl;
        return 0;
    }

    int choice = 0;
    choice += !!(f & UI_CHOICE1);
    choice += !!(f & UI_CHOICE2);
    choice += !!(f & UI_CHOICE3);
    choice += !!(f & UI_CHOICE4);

    if (choice > 1) {
        this->dump();
        std::cout << "invalid ui item flags: " << f
                  << "item has more than one choice-id bit set."
                  << std::endl;
        return 0;
    }

    if (group && choice) {
        this->dump();
        std::cout << "invalid ui item flags: " << f
                  << "item has both set-id and choice-id bits set."
                  << std::endl;
        return 0;
    }
    #endif
    return this;
 }


 template <class T>
 class error_item : public base<T>
 {
  public:

    static error_item<T>* err(const char* err_buf = 0)
    {
        static error_item<T> err;
        if (err_buf)
            err.err_msg = err_buf;
        std::cout << "err.err_msg: " << (void*)err.err_msg << std::endl;
        return &err;
    }

    bool validate(T, errors::TYPE)  { return false; }
    bool name_match(const char*)    { return false; }
    const char* get_descr()         { return err_msg; }

  private:
    error_item() : base<T>(UI_ERROR) {};
    ~error_item() {}
    static const char* err_msg;
 };

 template <class T>
 class comment_item : public base<T>
 {
  public:
    comment_item(const char* literal)
     : base<T>(UI_COMMENT, literal) {};

    ~comment_item() {};

    bool validate(T, errors::TYPE) { return true; }
    bool name_match(const char*) { return false; }

    #ifdef DEBUG
    void dump() {
        std::cout << "comment: " << base<T>::get_descr() << "(0x"
                  << (void*)this << ")" << std::endl;
    }
    #endif
 };


 template <class T>
 class param_item : public base<T>
 {
  public:
    param_item(param::TYPE pt)
     : base<T>(UI_PARAM), partype(pt) {};

    ~param_item() {};

    const char* get_name() { return param::names::get(partype); }

    param::TYPE get_param_type() const      { return partype; }
    bool operator()(param::TYPE & pt) const { return (partype == pt); }
    bool operator==(param::TYPE rhs) const  { return (partype == rhs); }

    bool validate(T, errors::TYPE);

    bool name_match(const char* str) {
        std::cout << "checking \"" << str << "\" against "
                  << param::names::get(partype) << std::endl;
        if (partype == param::STR_UNNAMED || partype == param::STR_LIST)
            return true;
        return (strcmp(str, param::names::get(partype)) == 0);
    }

    #ifdef DEBUG
    void dump() {
        std::cout << "param: " << param::names::get(partype) << "(0x"
                  << (void*)this << ")" << std::endl;
    }
    #endif

  private:
    param::TYPE partype;
 };

 template<class T>
 bool param_item<T>::validate(T obj, errors::TYPE et)
 {
    const void* data = obj->get_param(partype);
    if (!data)
        return false;
    switch(param::names::category(partype)) {
      case iocat::DOUBLE:
        return check_value(*static_cast<const double*>(data), et);
      case iocat::WCINT_T:
        return check_value(*static_cast<const wcint_t*>(data), et);
      case iocat::SAMP_T:
        return check_value(*static_cast<const samp_t*>(data), et);
      default:
        return false;
    }
 }


 template <class T>
 class input_item : public base<T>
 {
  public:
    input_item(input::TYPE it)
     : base<T>(UI_INPUT), intype(it) {};

    ~input_item() {};

    const char* get_name() { return input::names::get(intype); }

    input::TYPE get_input_type() const      { return intype; }
    bool operator()(input::TYPE & it) const { return (intype == it); }
    bool operator==(input::TYPE rhs) const  { return (intype == rhs); }

    bool validate(T, errors::TYPE) { return true; }

    bool name_match(const char* str) {
        std::cout << "checking \"" << str << "\" against "
                  << input::names::get(intype) << std::endl;
        return (strcmp(str, input::names::get(intype)) == 0);
    }

    #ifdef DEBUG
    void dump() {
        std::cout << "input: " << input::names::get(intype) << "(0x"
                  << (void*)this << ")" << std::endl;
    }
    #endif

  private:
    input::TYPE intype;
};

 template <class T>
 class dobj_item : public base<T>
 {
  public:
    dobj_item(dobj::TYPE p, dobj::TYPE c)
     : base<T>(UI_DOBJ), parent(p), child(c) {};

    ~dobj_item() {};

    const char* get_name() { return dobj::names::get(parent); }

    dobj::TYPE get_dobj_parent() const     { return parent; }
    dobj::TYPE get_dobj_child() const      { return child; }

    bool validate(T, errors::TYPE) { return true; }

    bool name_match(const char* str) {
        std::cout << "checking \"" << str << "\" against "
                  << dobj::names::get(parent) << std::endl;
        return (strcmp(str, dobj::names::get(parent)) == 0);
    }

    #ifdef DEBUG
    void dump() {
        std::cout << "dobj: parent: " << dobj::names::get(parent) << "(0x"
                  << (void*)this << ")" ;
        std::cout << "child: " << dobj::names::get(child) << std::endl;
    }
    #endif

  private:
    dobj::TYPE parent;
    dobj::TYPE child;
 };
};

#endif
