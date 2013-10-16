#ifndef UI_ITEM_H
#define UI_ITEM_H

#include "dobjnames.h"
#include "inputnames.h"
#include "paramnames.h"
#include "stockerrs.h"
#include "synthmodnames.h"
#include "linkedlist.h"

// a mechanism for registering the UI components of a data object.
// see ui_moditem.h for related information about how this differs
// from old method...

namespace ui
{ // where T is type* and DT is enum { type1, type2... }

 enum TYPE {
    UI_ERR,
    UI_INPUT,
    UI_PARAM,
    UI_DOBJ
 };

 template <class T, typename DT>
 class base
 {
  public:
    base(TYPE it, DT dt) : itemtype(it), datatype(dt) {};
    virtual ~base() {};
    virtual bool validate(T, errors::TYPE) = 0;

    TYPE    get_item_type() const   { return itemtype; }
    bool operator==(TYPE it) const  { return (itemtype == it); }

    DT      get_data_type() const   { return datatype; }
    bool operator()(DT & dt) const  { return (datatype == dt); }
    bool operator==(DT dt) const    { return (datatype == dt); }

    #ifdef DEBUG
    virtual void dump() {};
    #endif

  private:
    TYPE    itemtype;
    DT      datatype;
    base(){};
    base(const base&){};
 };

 template <class T, typename DT>
 class param_item : public base<T, DT>
 {
  public:
    param_item(DT dt, param::TYPE pt)
     : base<T, DT>(UI_PARAM, dt), partype(pt) {};

    ~param_item() {};

    param::TYPE get_param_type() const      { return partype; }
    bool operator()(param::TYPE & pt) const { return (partype == pt); }
    bool operator==(param::TYPE rhs) const  { return (partype == rhs); }

    bool validate(T, errors::TYPE);

    #ifdef DEBUG
    void dump()
        { std::cout << "param: " << param::names::get(partype) << std::endl; }
    #endif

  private:
    param::TYPE partype;
 };

 template<class T, typename DT>
 bool param_item<T, DT>::validate(T obj, errors::TYPE et)
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


 template <class T, typename DT>
 class input_item : public base<T, DT>
 {
  public:
    input_item(DT dt, input::TYPE it)
     : base<T, DT>(UI_INPUT, dt), intype(it) {};

    ~input_item() {};

    input::TYPE get_input_type() const      { return intype; }
    bool operator()(input::TYPE & it) const { return (intype == it); }
    bool operator==(input::TYPE rhs) const  { return (intype == rhs); }

    bool validate(T, errors::TYPE) { return true; }

    #ifdef DEBUG
    void dump()
        { std::cout << "input: " << input::names::get(intype) << std::endl; }
    #endif

  private:
    input::TYPE intype;
};

 template <class T, typename DT>
 class dobj_item : public base<T, DT>
 {
  public:
    dobj_item(DT dt, dobj::TYPE p, dobj::TYPE c)
     : base<T, DT>(UI_DOBJ, dt), parent(p), child(c) {};

    ~dobj_item() {};

    dobj::TYPE get_dobj_parent() const     { return parent; }
    dobj::TYPE get_dobj_child() const      { return child; }

    /*  these operators may conflict with base class if base class
        uses dobj::base
    bool operator()(dobj::TYPE & it) const { return (parent == it); }
    bool operator==(dobj::TYPE rhs) const  { return (parent == rhs); }
    */
    bool validate(T, errors::TYPE) { return true; }

    #ifdef DEBUG
    void dump()
        { std::cout << "dobj: parent: " << dobj::names::get(parent);
          std::cout << "child: " << dobj::names::get(child) << std::endl; }
    #endif

  private:
    dobj::TYPE parent;
    dobj::TYPE child;
 };
};

#endif
