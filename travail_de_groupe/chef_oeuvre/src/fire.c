#include "fire.h"

listchainfire_t initFire (void)
{
    return NULL;
}


booleen_t emptyListFire(listchainfire_t listFire)
{
    booleen_t result = false;

    if (listFire==NULL)
    {
        result=true;
    }
    return (result);
}

