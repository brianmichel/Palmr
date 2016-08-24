#include "palmr.h"

static PalmrDBRef database;

static Boolean PalmrFormEventHandler(EventPtr event);
static Boolean MyOverallEventHandler(EventPtr event);
/**
 * Main entry point for all PalmOS application.
 * @param  launchCode  Where did the launch come from?
 * @param  cmdPBP      //TODO I don't know what this is
 * @param  launchFlags //TODO I don't know what this i
 * @return             0 once the application has halted
 */
UInt32 PilotMain(UInt16 launchCode, MemPtr cmdPBP, UInt16 launchFlags)
{
    EventType event;
    UInt16 error;

    if (launchCode != sysAppLaunchCmdNormalLaunch) {
        return 0;
    }

    printf("I'm launching from PilotMain!");

    FrmGotoForm(PostListForm);

    setup_database();

    do {
        EvtGetEvent(&event, evtWaitForever);

        if (!SysHandleEvent(&event)) {
            if (!MenuHandleEvent(0, &event, &error)) {
                if (!MyOverallEventHandler(&event)) {
                    FrmDispatchEvent(&event);
                }
            }
        }
    } while (event.eType != appStopEvent);

    FrmCloseAllForms();

    if (database != NULL) {
        free(database);
        database = NULL;
    }

    return 0;
}

static Boolean MyOverallEventHandler(EventPtr event)
{
    switch (event->eType) {
    case frmLoadEvent: {
        FormPtr pForm = FrmInitForm(event->data.frmLoad.formID);
        FrmSetActiveForm(pForm);
        FrmSetEventHandler(pForm, PalmrFormEventHandler);
        return true;
    }
    case menuEvent: {
        return true;
    }
    }

    return false;
}

static Boolean PalmrFormEventHandler(EventPtr event)
{
    static FormPtr gpForm;

    switch (event->eType) {
    case frmOpenEvent: {
        FrmDrawForm(gpForm = FrmGetActiveForm());

        return true;
    }
    case frmCloseEvent: {
        FrmEraseForm(gpForm);
        FrmDeleteForm(gpForm);
        return true;
    }
    case ctlSelectEvent: {
        switch (event->data.ctlSelect.controlID) {
        case HelpButton: {
            FrmCustomAlert(TheError,
                "Oops, looks like our connection goofed here, try again?",
                NULL, NULL);
            return true;
        }
        }
    }
    case menuEvent: {
        switch (event->data.menu.itemID) {
        case PalmrMainMenuAbout: {
            FrmAlert(AboutAlert);
            return true;
        }
        }
    }
    default: {
        return false;
    }
    }
}

Boolean setup_database(void)
{
    if (database == NULL) {
        database = malloc(sizeof(PalmrDB));
        if (!initialize_database_reference(database)) {
            return false;
        }
    }

    return true;
}
