#ifndef _PALMRDB_H_
#define _PALMRDB_H_

#include <System/SystemPublic.h>

typedef struct {
    DmOpenRef databaseReference;
    Boolean opened;
} PalmrDB;

typedef PalmrDB* PalmrDBRef;

Boolean initialize_database_reference(PalmrDBRef database);
Boolean destroy_database_reference(PalmrDBRef database);

UInt16 number_of_references(PalmrDBRef database);

#endif
