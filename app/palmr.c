#include "palmr.h"
#include "Util.h"

static DmOpenRef PalmrDatabase;

// static Boolean PalmrFormEventHandler(EventPtr event);
static Boolean MyOverallEventHandler(EventPtr event);

Boolean setup_database(void);
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

    FrmGotoForm(PostListForm);

    // setup_database();

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

    if (PalmrDatabase != NULL) {
        destroy_database_reference(&PalmrDatabase);
        PalmrDatabase = NULL;
    }

    return 0;
}

static Boolean MyOverallEventHandler(EventPtr event)
{
    FormPtr form;
    UInt16 formId;

    switch (event->eType) {
    case frmLoadEvent: {
        formId = event->data.frmLoad.formID;
        form = FrmInitForm(formId);
        FrmSetActiveForm(form);

        switch (formId) {
        case PostListForm:
            FrmSetEventHandler(form, PostListFormEventHandler);
            return true;
        case PostViewForm:
            FrmSetEventHandler(form, PostViewEventHandler);
            return true;
        }
    }
    case menuEvent: {
        return handle_menu_event(event);
    }
    default:
        return false;
    }

    return false;
}

// static Boolean PalmrFormEventHandler(EventPtr event)
// {
//     static FormPtr gpForm;
//
//     switch (event->eType) {
//     case frmOpenEvent: {
//         FrmDrawForm(gpForm = FrmGetActiveForm());
//         UpdatePostsTable(gpForm);
//
//         return true;
//     }
//     case frmCloseEvent: {
//         FrmEraseForm(gpForm);
//         FrmDeleteForm(gpForm);
//         return true;
//     }
//     case ctlSelectEvent: {
//         switch (event->data.ctlSelect.controlID) {
//         case HelpButton: {
//             FrmCustomAlert(TheError,
//                 "Oops, looks like our connection goofed here, try again?",
//                 NULL, NULL);
//             return true;
//         }
//         case PalmrMainMenuAbout: {
//             FrmAlert(AboutAlert);
//             return true;
//         }
//         }
//     }
//     default: {
//         return false;
//     }
//     }
// }

Boolean setup_database(void)
{
    if (PalmrDatabase == NULL) {
        if (!initialize_database_reference(&PalmrDatabase)) {
            FrmCustomAlert(TheError,
                "Unable to setup the database :(",
                NULL, NULL);
            return false;
        }
    }

    return true;
}

Boolean handle_menu_event(EventPtr event)
{
    Boolean handled = false;

    switch (event->data.menu.itemID) {
    case PalmrMainMenuHelp:
        FrmCustomAlert(TheError, "There is no help to be offered to you.", NULL, NULL);
        handled = true;
        break;
    case PalmrMainMenuAbout:
        FrmCustomAlert(AboutAlert, NULL, NULL, NULL);
        handled = true;
        break;
    }
    return handled;
}
