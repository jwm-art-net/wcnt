#ifndef INPUTEDITOR_H
#define INPUTEDITOR_H

#include "inputedit.h"
#include "linkedlist.h"

class inputeditor : public dobj
{
public:
    inputeditor();
    ~inputeditor();
    inputedit* add_inputedit(inputedit*);
    inputedit* goto_first_inputedit() {
        return input_edit = (inputedit*)
            (input_item = input_edit_list->goto_first())->get_data();
    }
    inputedit* goto_last_inputedit() {
        return input_edit = (inputedit*)
            (input_item = input_edit_list->goto_last())->get_data();
    }
    inputedit* goto_prev_inputedit() {
        return input_edit = (inputedit*)
            (input_item = input_edit_list->goto_prev())->get_data();
    }
    inputedit* goto_next_inputedit() {
        return input_edit = (inputedit*)
            (input_item = input_edit_list->goto_next())->get_data();
    }
    bool create_connectors();
    // virtuals from dobj
    virtual stockerrs::ERR_TYPE validate(){
        return stockerrs::ERR_NO_ERROR;
    }
    dobj const* add_dobj(dobj* dbj);

private:
    linkedlist* input_edit_list;
    ll_item* input_item;
    inputedit* input_edit;
    void create_params();
    static bool done_params;
};

#endif
