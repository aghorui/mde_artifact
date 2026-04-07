#include "ZDDWrapper.h"

DdManager* ZDD::global_mgr = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS,CUDD_CACHE_SLOTS, 0);