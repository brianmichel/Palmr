#ifndef _PALMRDB_H_
#define _PALMRDB_H_

#include <System/SystemPublic.h>

Boolean initialize_database_reference(DmOpenRef* database);
Boolean destroy_database_reference(DmOpenRef* database);

UInt16 number_of_references(DmOpenRef* database);

#endif
