#include "PalmrRsc.h"
#include <System/SystemPublic.h>
#include <UI/UIPublic.h>

static Boolean PalmrFormEventHandler(EventPtr event);
static Boolean MyOverallEventHandler(EventPtr event);

UInt32 PilotMain(UInt16 launchCode, MemPtr cmdPBP, UInt16 launchFlags) {
  EventType event;
  UInt16 error;

  if (launchCode != sysAppLaunchCmdNormalLaunch) {
    return 0;
  }

  FrmGotoForm(PalmrForm);

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

  return 0;
}

static Boolean MyOverallEventHandler(EventPtr event) {
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

static Boolean PalmrFormEventHandler(EventPtr event) {
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
      FrmAlert(AboutAlert);
      return true;
    }
    }
  }
  default: { return false; }
  }
}
