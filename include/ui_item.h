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
#include <iostream>

namespace ui
{ // where T is type* and DT is enum { type1, type2... }

 enum TYPE {
    UI_ERROR,
    UI_COMMENT,
    UI_INPUT,
    UI_PARAM,
    UI_DOBJ
 };

 enum FLAGS {
    UI_DEFAULT =    0x0000,
    UI_OPTIONAL =   0x0001,

    UI_CHOICE1 =    0x0100,
    UI_CHOICE2 =    0x0200,
    UI_CHOICE3 =    0x0400,
    UI_CHOICE4 =    0x0800,
    UI_CHOICE_MASK= 0x0f00,

    UI_SET1 =       0x1000,
    UI_SET2 =       0x2000,
    UI_SET3 =       0x4000,
    UI_SET4 =       0x8000,
    UI_SET_MASK =   0xf000
 };

 enum {
    ITEM_CHOICE_UNEXPECTED = -5,// first item chosen, and so was this.
    ITEM_CHOICE_EXPECTED = -4,  // first item chosen but this wasnt.
    ITEM_SET_UNEXPECTED = -3,   // first item skipped but this wasnt.
    ITEM_SET_EXPECTED = -2,     // first item specified but this wasnt.
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

    TYPE    get_item_type() const   { return itemtype; }
    bool operator==(TYPE it) const  { return (itemtype == it); }

    virtual const char* get_name() { return 0; }

    void add_descr(const char* literal)   { descr = literal; }
    virtual const char* get_descr()       { return descr; }

    int is_match(const char* str, FLAGS skip_id, FLAGS match_id);

    void set_flags(int f);
    bool is_optional()      { return (flags & UI_OPTIONAL); }
    bool has_flag(FLAGS f)  { return !!(flags & f); }

    FLAGS get_choice_id() {
        return static_cast<FLAGS>(flags & UI_CHOICE_MASK);
    }
    FLAGS get_set_id() {
        return static_cast<FLAGS>(flags & UI_SET_MASK);
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
 void base<T>::set_flags(int f)
 {
    flags = f;

    #ifdef DEBUG
    if (f < 0) {
        std::cout << "invalid ui item flags: " << f << std::endl;
        return;
    }
    int set = 0;
    set += !!(f & UI_SET1);
    set += !!(f & UI_SET2);
    set += !!(f & UI_SET3);
    set += !!(f & UI_SET4);
    if (set > 1) {
        std::cout << "invalid ui item flags: " << f
                  << "item has more than one set-id bit set." << std::endl;
    }

    int choice = 0;
    choice += !!(f & UI_CHOICE1);
    choice += !!(f & UI_CHOICE2);
    choice += !!(f & UI_CHOICE3);
    choice += !!(f & UI_CHOICE4);
    if (choice > 1) {
        std::cout << "invalid ui item flags: " << f
                  << "item has more than one choice-id bit set."
                  << std::endl;
    }

    if (set && choice) {
        std::cout << "invalid ui item flags: " << f
                  << "item has both set-id and choice-id bits set."
                  << std::endl;
    }
    #endif
 }

 template <class T>
 int base<T>::is_match(const char* str, FLAGS skip_id, FLAGS match_id)
 {
    if (name_match(str)) {
        std::cout << "::: item name '" << str << "' matched.. " << std::endl;
        if (skip_id == UI_CHOICE_MASK) {
            std::cout << "::: processing choice " << match_id << std::endl;
            if (match_id == get_choice_id()) {
                std::cout << "::: choice match." << std::endl;
                return ITEM_MATCH;
            }
            else {
                std::cout << "::: choice mismatch." << std::endl;
                return ITEM_CHOICE_UNEXPECTED;
            }
        }
        if (skip_id != UI_DEFAULT && skip_id == get_set_id())
            return ITEM_SET_UNEXPECTED;
        return ITEM_MATCH;
    }
    else {
        std::cout << "::: item name '" << str << "' unmatched.. " << std::endl;
        if (skip_id == UI_CHOICE_MASK) {
            std::cout << "::: processing choice " << match_id << std::endl;
            if (match_id == get_choice_id()) {
                std::cout << "::: choice mismatch " << std::endl;
                return ITEM_CHOICE_EXPECTED;
            }
            else {
                std::cout << "::: choice skipped" << std::endl;
                return ITEM_SKIPPED;
            }
        }
        if (get_choice_id() != UI_DEFAULT)
            return ITEM_SKIPPED;
        if (is_optional())
            return ITEM_SKIPPED;
        if ((skip_id != UI_DEFAULT && skip_id == get_set_id()))
            return ITEM_SKIPPED;
        if (match_id != UI_DEFAULT && match_id == get_set_id())
            return ITEM_SET_EXPECTED;
        return ITEM_UNEXPECTED;
    }
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
        std::cout << "comment: " << base<T>::get_descr() << std::endl;
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
        std::cout << "param: " << param::names::get(partype) << std::endl;
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
      case iocat::DOUBLE:   return check_value(*(const double*)data, et);
      case iocat::WCINT_T:  return check_value(*(const wcint_t*)data, et);
      case iocat::SAMP_T:   return check_value(*(const samp_t*)data, et);
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
        return (strcmp(str, input::names::get(intype)) == 0);
    }

    #ifdef DEBUG
    void dump() {
        std::cout << "input: " << input::names::get(intype) << std::endl;
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
        return (strcmp(str, dobj::names::get(parent)) == 0);
    }

    #ifdef DEBUG
    void dump() {
        std::cout << "dobj: parent: " << dobj::names::get(parent);
        std::cout << "child: " << dobj::names::get(child) << std::endl;
    }
    #endif

  private:
    dobj::TYPE parent;
    dobj::TYPE child;
 };
};

#endif
