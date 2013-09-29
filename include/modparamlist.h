#ifndef MODPARAMLIST_H
#define MODPARAMLIST_H

#include "linkedlist.h"
#include "modparam.h"

/* modparamlist is slightly different to modinputlist or modoutputlist
instead of taking a synthmod* arguement it takes 
module::TYPE.  The list also differs in the way it is
called, ie the params should be added only once for each module type,
not for every module created.  (possibly by a static member func called
by the module constructor) also, there is no need to delete the params
once corresponding module has been deleted, can leave it till the end
when the list is destroyed.  

Plus the fact I've removed the remove_module_... member funcs cos there's
no need for them.  futher, the list_of_category.... functions in
modinputlist etc have been removed as there is no need to use them.

All of this is because the params are used in a completely different way
to the inputs and outputs.

    added bool validate(..) func.  called by synthmod::validate().
     works out modparam from PAR_TYPE and calls its bool validate(..) */

class modparamlist : public linked_list<modparam>
{
public:
    modparamlist();
    modparamlist(DESTRUCTION);

    modparam* add_param(module::TYPE, param::TYPE);

    bool validate(synthmod*, param::TYPE, stockerrs::ERR_TYPE);

private:
    bool delete_param(modparam*);
};

#endif
