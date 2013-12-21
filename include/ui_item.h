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

#ifdef DEBUG
#include <iostream>
#endif

#include "sstream"

namespace ui
{

 enum TYPE {
    UI_ERROR,
    UI_USERNAME,
    UI_COMMENT,
    UI_INPUT,
    UI_PARAM,
    UI_DOBJ
 };

 enum FLAGS {
    UI_DEFAULT =        0x0000,
    // items marked UI_OPTIONAL allow the user to not specify an individual
    // item as optional which allows the end user to omit specification of
    // it. NOTE: the ui plays no part in the initialization of non specified
    // items - with the exception of unspecified inputs automatically having
    // an off-connector created for them.
    UI_OPTIONAL =       0x0001,

    // items marked with UI_OPTIONn allow items to be grouped together as
    // multiple choice options. only one option can be selected, but each
    // option can contain several items. Only four options can be catered
    // for but this is unlikely to be too few.

    // the first option should always be UI_OPTION1, and second UI_OPTION2.
    // if an item with UI_OPTION1 is encountered immediately after a
    // UI_OPTION1 + n item, a new multiple-choice selection of options is
    // assumed.

    UI_OPTION1 =        0x0010,
    UI_OPTION2 =        0x0020,
    UI_OPTION3 =        0x0040,
    UI_OPTION4 =        0x0080,

    UI_OPT_OPTIONAL =   0x0100,

    // NOTE: Options were designed more as substitutes for one another than
    //      as choices between entirely different entitites. In particular,
    //      where an output value is the result of modulation between two
    //      fixed values, options allow the user to simply enter one value
    //      instead of two when modulation is not desired.
    //      However, this causes confusion when it comes to duplicating a
    //      module or data object. Which items should be duplicated and which
    //      should not?

    //      The simplest method is to force responsibility onto the module or
    //      data object to specify which items should be duplicated. This then
    //      means it is also down to the module or data object to be sensible
    //      and to ensure that the duplication does actually duplicate what
    //      is necessary.

    //      Items marked with UI_OPT_DUP are items in an option which should
    //      be duplicated when duplication is called for. Only items marked
    //      with UI_OPTION need use this.
    UI_OPT_DUPLICATE =  0x0200,

    // items marked UI_DUMMY are invisible to the end user but exist to
    // trigger some form of setup work. Specifically, they are needed by
    // module inputs which would otherwise exist in one UI_OPTION but not
    // another (see below). Because unspecified inputs must be turned off,
    // the UI_DUMMY input is used as a simple means to do so.
    // (note: see the sampler module for usage example).
    UI_OPT_DUMMY =      0x0400,


    // items can be grouped together by setting one of the UI_GROUPn flags.
    // allowing the whole group of items in the user interface to either
    // all be specified or all absent. additionally, specific items within
    // a group can be be made optional by use of UI_OPTIONAL. Potentially,
    // there can be any number of groups within a container as long as there
    // is at least one non-group item between each group (excluding comment
    // items).  Where distinct groups immediatelly follow each other, then
    // only four distinct but adjaecent groups can be accommodated.

    UI_GROUP1 =         0x1000,
    UI_GROUP2 =         0x2000,
    UI_GROUP3 =         0x4000,
    UI_GROUP4 =         0x8000,

    // items cannot be part of a group and option simultaneously.

    // implementation only:
    UI_OPTION_MASK=     0x00f0,
    UI_GROUP_MASK =     0xf000,
    UI_MATCHED =    0x10000000, // matched
    UI_FORCED_OPT = 0x20000000, // user forced item from unselected option


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
    bool is_optional()          { return !!(flags & UI_OPTIONAL); }
    bool is_opt_optional()      { return !!(flags & UI_OPT_OPTIONAL); }
    bool is_dummy()             { return !!(flags & UI_OPT_DUMMY); }
    bool is_ui_opt_duplicate()  { return !!(flags & UI_OPT_DUPLICATE); }
    bool has_flag(FLAGS f)      { return !!(flags & f); }

    void reset_matched()        { flags &= ~UI_MATCHED; }
    void set_matched()          { flags |= UI_MATCHED; }
    bool is_matched()           { return !!(flags & UI_MATCHED); }

    void set_forced()           { flags |= UI_FORCED_OPT; }
    bool was_forced()           { return !!(flags & UI_FORCED_OPT); }

    bool should_duplicate()     {
        return (flags & UI_OPTION_MASK ? !!(flags & UI_OPT_DUPLICATE)
                                      : true);
    }

    FLAGS get_option_id() {
        return static_cast<FLAGS>(flags & UI_OPTION_MASK);
    }
    FLAGS get_group_id() {
        return static_cast<FLAGS>(flags & UI_GROUP_MASK);
    }

    int get_option_no() {
        switch (get_option_id()) {
          case UI_OPTION1: return 1;
          case UI_OPTION2: return 2;
          case UI_OPTION3: return 3;
          case UI_OPTION4: return 4;
          default:         return 0;
        }
    }

    int get_group_no() {
        switch (get_group_id()) {
          case UI_GROUP1: return 1;
          case UI_GROUP2: return 2;
          case UI_GROUP3: return 3;
          case UI_GROUP4: return 4;
          default:        return 0;
        }
    }

    bool get_item_flags(char* buf, size_t sz) {
        int i = 0;
        for (; i < sz - 1; ++i)
            buf[i] = ' ';
        buf[i] = '\0';
        std::ostringstream s;
        int n = get_option_no();
        s << "(";
        if (n)
            s << "C" << n;
        else if ((n = get_group_no()))
            s << "G" << n;
        if (is_optional())
            s << "o";
        else
            s << "*";
        s << ")";
        strncpy(buf, s.str().c_str(), sz);
        return true;
    }

    #ifdef DEBUG
    virtual void dump() {
        std::cout << "M" << (is_matched() ? "y" : "n");
        std::cout << "  O" << (is_optional() ? "y" : "n");
        int n = get_option_no();
        if (n)
            std::cout << "  C" << n;
        else
            std::cout << "  C*";
        if ((n = get_group_id()))
            std::cout << "  G" << n;
        else
            std::cout << "  G*";
        std::cout << "\t";
    };
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
                  << "item has more than one group-id bit set." << std::endl;
        return 0;
    }

    int option = 0;
    option += !!(f & UI_OPTION1);
    option += !!(f & UI_OPTION2);
    option += !!(f & UI_OPTION3);
    option += !!(f & UI_OPTION4);

    if (option > 1) {
        this->dump();
        std::cout << "invalid ui item flags: " << f
                  << "item has more than one option-id bit set."
                  << std::endl;
        return 0;
    }

    if (group && option) {
        this->dump();
        std::cout << "invalid ui item flags: " << f
                  << "item has both group-id and option-id bits set."
                  << std::endl;
        return 0;
    }

    if (!option) {
        if (f & UI_OPT_OPTIONAL
         || f & UI_OPT_DUPLICATE
         || f & UI_OPT_DUMMY)
        {
            this->dump();
            std::cout << "invalid ui item flags: " << f
                      << "item has UI_OPT_xxx flags set without UI_OPTIONx set."
                      << std::endl;
            return 0;
        }
    }

    #endif
    return this;
 }

 template <class T>
 class username_item : public base<T>
 {
  public:
    static username_item<T>* username()
    {
        static username_item<T> uname;
        return &uname;
    }
    bool validate(T, errors::TYPE)  { return true; }
  private:
    username_item() : base<T>(UI_USERNAME) {};
    bool name_match(const char*) { return false; }
 };


 template <class T>
 class error_item : public base<T>
 {
  public:

    static error_item<T>* err(const char* err_buf = 0)
    {
        static error_item<T> err;
        if (err_buf)
            err.err_msg = err_buf;
        #ifdef DEBUG
        std::cout << "err.err_msg: " << (void*)err.err_msg << std::endl;
        #endif
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
        #ifdef DEBUG
        std::cout << "checking \"" << str << "\" against "
                  << param::names::get(partype) << std::endl;
        #endif
        if (partype == param::STR_UNNAMED || partype == param::STR_LIST)
            return true;
        return (strcmp(str, param::names::get(partype)) == 0);
    }

    #ifdef DEBUG
    void dump() {
        base<T>::dump();
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
        #ifdef DEBUG
        std::cout << "checking \"" << str << "\" against "
                  << input::names::get(intype) << std::endl;
        #endif
        return (strcmp(str, input::names::get(intype)) == 0);
    }

    #ifdef DEBUG
    void dump() {
        base<T>::dump();
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
        #ifdef DEBUG
        std::cout << "checking \"" << str << "\" against "
                  << dobj::names::get(parent) << std::endl;
        #endif
        return (strcmp(str, dobj::names::get(parent)) == 0);
    }

    #ifdef DEBUG
    void dump() {
        base<T>::dump();
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
