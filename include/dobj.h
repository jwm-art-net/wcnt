#ifndef DOBJ_H
#define DOBJ_H

#include <iostream>

#include "stockerrs.h"
#include "dobjnames.h"
#include "paramnames.h"
#include "fxsparamlist.h"

// forward definitions
class dobjlist;      // list of dobj's defined outside a module or dobj
class dobjparamlist; // list of all dparams for all dobj's
class topdobjlist;   // list of dobj types which contain a list

class dobj
{
public:
    dobj(dobjnames::DOBJ_TYPE objtype);
    virtual ~dobj();
    dobjnames::DOBJ_TYPE get_object_type() {
        return ((this) ? object_type : dobjnames::DOBJ_FIRST);
    }
    bool is_named_by_user();
    bool set_username(const char*);
    // some dobj are unamed by user, but contain another string which
    // is helpful when identifying errors, which is why get_username()
    // is now virtual.
    virtual char const* get_username(); 
    bool is_valid(){ return ((this) ? valid : false);}
    virtual stockerrs::ERR_TYPE validate() = 0;
    virtual bool set_param(paramnames::PAR_TYPE, void*);
    virtual void const* get_param(paramnames::PAR_TYPE);
    virtual dobj const* add_dobj(dobj*); // don't be fooled...
    virtual dobj* duplicate_dobj(const char*);
    static void clear_error_msg(){ *err_msg = "";}
    static void register_error_msg(string* e)           {err_msg = e;}
    static void register_path(char* p)                  {path = p;}
    static void register_iocatnames(iocat_names* in)    {iocatnames = in;}
    static void register_dobjnames(dobjnames* dn)       {dobj_names = dn;}
    static void register_dobjlist(dobjlist* dl)         {dobj_list = dl;}
    static void register_dparlist(dobjparamlist* dp)    {dobjpar_list=dp;}
    static void register_paramnames(paramnames* dp)     {par_names = dp;}
    static void register_topdobjlist(topdobjlist* dl)   {top_dobjlist=dl;}
    static void register_fxsparamlist(fxsparamlist* fl) {fxsparlist = fl;}
    static string const*    get_error_msg()     {return err_msg;}
    static char const*      get_path()          {return path;}
    static iocat_names*     get_iocatnames()    {return iocatnames;}
    static dobjnames*       get_dobjnames()     {return dobj_names;}
    static dobjlist*        get_dobjlist()      {return dobj_list;}
    static dobjparamlist*   get_dparlist()      {return dobjpar_list;}
    static paramnames*      get_paramnames()    {return par_names;}
    static topdobjlist*     get_topdobjlist()   {return top_dobjlist;}
    static fxsparamlist*    get_fxsparamlist()  {return fxsparlist;}
    #ifdef SHOW_DOBJ_COUNT
    static long get_created_count(){ return dobjs_created_count;}
    static long get_destroyed_count(){ return dobjs_destroyed_count;}
    static long get_max_count(){ return dobjs_max_count;}
    #endif
protected:
    static string* err_msg;
    virtual void create_params() = 0;
    void invalidate(){ valid = false;}
private:
    dobjnames::DOBJ_TYPE object_type;
    char* username;
    bool valid;
    static char* path; /* to synthfile.wc from cmdline */
    static iocat_names* iocatnames;
    static dobjnames* dobj_names;
    static dobjlist* dobj_list;
    static dobjparamlist* dobjpar_list;
    static paramnames* par_names;
    static topdobjlist* top_dobjlist;
    static fxsparamlist* fxsparlist;
    #ifdef SHOW_DOBJ_COUNT
    static long dobjs_created_count;
    static long dobjs_destroyed_count;
    static long dobjs_count;
    static long dobjs_max_count;
    #endif
};

#endif
