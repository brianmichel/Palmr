#include "PalmOS.h"
#include "resource.h" // definition of HelloForm

static Boolean ApplicationEventHandelr(EventPtr event);
static Boolean HelloFormEventHandler(EventPtr event);

UInt32 PilotMain(UInt16 launchCode, MemPtr cmdPBP, UInt16 launchFlags) {
  EventType event;
  UInt16 error;

  if (launchCode != sysAppLaunchCmdNormalLaunch) {
    return 0
  }

  FrmGotoForm(HelloForm);

  do {
    EvtGetEvent(&event, evtWaitForever);

    if (!SysHandlEvent(&event)) {
      if (!MenuHandleEvent(0, &event, &error)) {
        if (!MyOverallEventHandler(&event)) {
          FrmDispatchEvent(&event);
        }
      }
    }
  } while (event.eType != appStopEvent);

  FrmCloseAllForms();

  return 0;
}

static Boolean MyOverallEventHandler(EventPtr event) {
  switch (event->eType) {
  case frmLoadEvent: {
    FormPtr pForm = FrmInitForm(event->data.frmLoad.formID);
    FrmSetActiveForm(pForm);
    FrmSetEventHandler(pForm, HelloFormEventHandler);
    return true;
  }
  case menuEvent: {
    switch (event->data.menu.itemID) {
    case HelpMenuAbout: {
      FrmAlert(AboutAlert);
      break;
    }
      return true;

    default: { return false; }
    }
  }
  }
}

static Boolean HelloFormEventHandler(EventPtr event) {
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
  default: { return false; }
  }
}
