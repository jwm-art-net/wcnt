#ifndef UI_ITEM_H
#define UI_ITEM_H

#include "dobjnames.h"
#include "inputnames.h"
#include "paramnames.h"
#include "stockerrs.h"
#include "synthmodnames.h"
#include "linkedlist.h"

// a mechanism for registering the UI components of a module or data object.
// comments can be added to items to supply further information to end user.
// whereas the comment_item is used as an item itself such that it might
// be used for example as as a UI sub-heading.


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

 template <class T>
 class base
 {
  public:
    base(TYPE it);
    base(TYPE it, const char* comment_literal);

    virtual ~base() {};
    virtual bool validate(T, errors::TYPE) = 0;

    TYPE    get_item_type() const   { return itemtype; }
    bool operator==(TYPE it) const  { return (itemtype == it); }

    void add_comment(const char* literal)   { item_comment = literal; }
    virtual const char* get_comment()       { return item_comment; }

    int is_match(const char* str, FLAGS skip_id, FLAGS match_id);

    void set_flags(int f);
    bool is_optional()      { return (flags & UI_OPTIONAL); }
    bool has_flag(FLAGS f)  { return !!(flags & f); }

    FLAGS get_choice_id()   { return (flags & UI_CHOICE_MASK); }
    FLAGS get_set_id()      { return (flags & UI_SET_MASK); }

    #ifdef DEBUG
    virtual void dump() {};
    #endif
  protected:
    virtual bool name_match(const char*) = 0;

  private:
    int         flags;
    TYPE        itemtype;
    const char* item_comment;
    base(){};
    base(const base&){};
 };

 template <class T>
 base<T>::base(TYPE it)
  : flags(0), itemtype(it), item_comment(0)
 {
 }

 template <class T>
 base<T>::base(TYPE it, const char* comment)
  : flags(0), itemtype(it), item_comment(comment)
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

/*

 template <class T>
 int base<T>::is_match(const char* str, int skip, int match)
 {
    FLAGS skip_id = (skip & UI_SET_MASK);

    if (skip_id) {
        if (get_set_id() == skip_id && name_match(str)) {
            // warning, previous items in set not specified
            return 1;
        }
    }

    if (name_match(str)) {
        if (get_set_id()) {
            if (!(running_flags & UI_SET_MASK)) {
                // previous items in set were skipped, why start now?
                return -1;
            }
            if ((running_flags & UI_SET_MASK) != get_set_id()) {
                // set id mismatch (????)
                return -1;
            }
            return 0;
        }
        if (get_choice_id()) {
            if (!(running_flags & UI_CHOICE_MASK)) {
                // skipped a choice item, bugger.
                return -1;
            }
            if ((running_flags & UI_CHOICE_MASK) != get_choice_id()) {
                // only one choice may be chosen!
                return -1;
            }
            return 0;
        }
        return 0; // got there eventually
    }
    else { // str does not match name of expected type
        if (get_set_id()) {
            if (!(running_flags & UI_SET_MASK)) {
                
                return -1;
            }
        }
    }
    return 0;
 }
*/


 template <class T>
 class error_item : public base<T>
 {
  public:

    static error_item<T>* err(char* msg)
    {
        error_item<T> err;
        err->set_msg(msg);
        return &err;
    }

    bool validate(T, errors::TYPE)  { return false; }
    bool name_match(const char*)    { return false; }
    const char* get_comment()       { return err_msg; }

  private:
    error_item() : base<T>(UI_ERROR), err_msg(0) {};

    ~error_item() {
        if (err_msg) delete [] err_msg;
    }

    void set_msg(char* msg) {
        if (err_msg) delete [] err_msg;
        err_msg = msg;
    }
    char* err_msg;
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
        std::cout << "comment: " << base<T>::get_comment() << std::endl;
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

    param::TYPE get_param_type() const      { return partype; }
    bool operator()(param::TYPE & pt) const { return (partype == pt); }
    bool operator==(param::TYPE rhs) const  { return (partype == rhs); }

    bool validate(T, errors::TYPE);

    bool name_match(const char* str) {
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
