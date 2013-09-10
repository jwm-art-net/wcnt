#ifndef PARAMEDITOR_H
#define PARAMEDITOR_H

#include "paramedit.h"

// forward definition
class paramedit;

class parameditor : public dobj
{
public:
    parameditor();
    ~parameditor();
    paramedit* add_paramedit(paramedit*);
    paramedit* goto_first_paramedit() {
        return par_edit = (paramedit*)
            (edit_item = par_edit_list->goto_first())->get_data();
    }
    paramedit* goto_last_paramedit() {
        return par_edit = (paramedit*)
            (edit_item = par_edit_list->goto_last())->get_data();
    }
    paramedit* goto_prev_paramedit() {
        return par_edit = (paramedit*)
            (edit_item = par_edit_list->goto_prev())->get_data();
    }
    paramedit* goto_next_paramedit() {
        return par_edit = (paramedit*)
            (edit_item = par_edit_list->goto_next())->get_data();
    }
    bool do_param_edits();
    // virtuals from dobj
    virtual stockerrs::ERR_TYPE validate(){
        return stockerrs::ERR_NO_ERROR;
    }
    dobj const* add_dobj(dobj* dbj);
    void set_verbose(){ verbose = true; }

private:
    linkedlist* par_edit_list;
    ll_item* edit_item;
    paramedit* par_edit;
    void create_params();
    static bool done_params;
    bool verbose;
};

#endif
