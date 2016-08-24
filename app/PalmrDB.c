#include <System/SystemPublic.h>
#include <stdio.h>

#include "PalmrDB.h"
#include "Util.h"

#define PalmrDBType 'DATA'
#define PalmrDBCreator 'TBLR'

static Err get_database_attributes(LocalID databaseID, UInt16* attributes);
static Err set_database_attributes(LocalID databaseID, UInt16* attributes);

Boolean initialize_database_reference(DmOpenRef* database)
{
    Err error;
    DmOpenRef db;

    LocalID databaseID;
    UInt16 cardNumber = 0;
    UInt16 databaseAttributes;
    UInt16 databaseAttributesWithBackup;

    db = DmOpenDatabaseByTypeCreator(PalmrDBType, PalmrDBCreator, dmModeReadWrite);
    // Database doesn't exist, let's create it
    if (db == NULL) {
        error = DmCreateDatabase(0, "PalmrPosts", PalmrDBCreator, PalmrDBType, false);
        if (error) {
            AlertPrintf1("Unable to create database for Palmr application!");
            return false;
        }

        db = DmOpenDatabaseByTypeCreator(PalmrDBType, PalmrDBCreator, dmModeReadWrite);
    }

    error = DmOpenDatabaseInfo(db, &databaseID, NULL, NULL, &cardNumber, NULL);
    if (error) {
        AlertPrintf1("Unable to get database information from Palmr DB!");
        DmCloseDatabase(db);
        return false;
    }

    if (get_database_attributes(databaseID, &databaseAttributes) != NULL) {
        AlertPrintf1("Unable to get database attributes from Palmr DB!");
        DmCloseDatabase(db);
        return false;
    }

    databaseAttributesWithBackup = databaseAttributes | dmHdrAttrBackup;
    if (set_database_attributes(databaseID, &databaseAttributesWithBackup) != NULL) {
        AlertPrintf1("Unable to set database attributes from Palmr DB!");
        DmCloseDatabase(db);
        return false;
    }

    *database = db;

    AlertPrintf1("There's a database open!");

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

UInt16 number_of_references(DmOpenRef* database)
{
    return DmNumRecords(*database);
}

Boolean destroy_database_reference(DmOpenRef* database)
{
    if (database) {
        Err error;
        error = DmCloseDatabase(*database);
        if (error) {
            printf("Error trying to close database %i\n", error);
            return false;
        }
        return true;
    }

    return false;
}
