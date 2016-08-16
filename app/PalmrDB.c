#include <System/SystemPublic.h>

#include "PalmrDB.h"

#define PalmrDBType 'DATA'
#define PalmrDBCreator 'TBLR'

static Err get_database_attributes(LocalID databaseID, UInt16* attributes);
static Err set_database_attributes(LocalID databaseID, UInt16* attributes);

Boolean initialize_database_reference(PalmrDBRef database)
{
    Err error;
    DmOpenRef db;

    LocalID databaseID;
    UInt16 cardNumber;
    UInt16 databaseAttributes;
    UInt16 databaseAttributesWithBackup;

    db = DmOpenDatabaseByTypeCreator(PalmrDBType, PalmrDBCreator, dmModeReadWrite);
    // Database doesn't exist, let's create it
    if (!db) {
        error = DmCreateDatabase(0, "Palmr", PalmrDBCreator, PalmrDBType, false);
        if (error) {
            return false;
        }

        db = DmOpenDatabaseByTypeCreator(PalmrDBType, PalmrDBCreator, dmModeReadWrite);
    }

    error = DmOpenDatabaseInfo(db, &databaseID, NULL, NULL, &cardNumber, NULL);
    if (error) {
        DmCloseDatabase(db);
        return false;
    }

    if (get_database_attributes(databaseID, &databaseAttributes) != NULL) {
        DmCloseDatabase(db);
        return false;
    }

    databaseAttributesWithBackup = databaseAttributes | dmHdrAttrBackup;
    if (set_database_attributes(databaseID, &databaseAttributesWithBackup) != NULL) {
        DmCloseDatabase(db);
        return false;
    }

    database->databaseReference = db;
    database->opened = true;

    return true;
}

static Err get_database_attributes(LocalID databaseID, UInt16* attributes)
{
    Err error;
    error = DmDatabaseInfo(0,
        databaseID,
        NULL,
        attributes,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);

    return error;
}

static Err set_database_attributes(LocalID databaseID, UInt16* attributes)
{
    Err error;
    error = DmSetDatabaseInfo(0,
        databaseID,
        NULL,
        attributes,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL);

    return error;
}

UInt16 number_of_references(PalmrDBRef database)
{
    return DmNumRecords(database->databaseReference);
}

Boolean destroy_database_reference(PalmrDBRef database)
{
    if (database && database->opened) {
        Err error;
        error = DmCloseDatabase(database->databaseReference);
        if (error) {
            return false;
        }
        return true;
    }

    return false;
}
