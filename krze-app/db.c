/*****************************************************************************\
*                                                                             *
* Project       : Mobile development tutorial                                 *
* Application   : Demonstration of database development                       *
* Component     : Main                                                        *
* Author        : Eric Poncet (mobile.eric-poncet.com)                        *
* Creation      : 07/10/2000                                                  *
*                                                                             *
*******************************************************************************
*                                                                             *
* History       :                                                             *
*                                                                             *
\*****************************************************************************/


#include "db.h"
#include <PalmTypes.h>
#include <PalmCompatibility.h>
#include <System/SystemPublic.h>
#include <UI/UIPublic.h>


#define CreatorID 'TuDb'
#define ROM_VERSION_REQUIRED	0x02000000		// This application requires PalmOS 2.0 or later
#define DBName "TutorialDatabaseDB"
#define DBType 'DATA'

typedef struct DBRecordType
{
	char	name[21];
	Int16	quantity;
} DBRecordType;
typedef DBRecordType*	DBRecordTypePtr;


static DmOpenRef		db;
static UInt16			dbCard;
static LocalID		dbID;
static Int16			nbRec = 0, curRec = 0;
static DBRecordType		dbRecord;
static UInt32			totalBytes, dataBytes;


static Char* GetField(UInt16 formID, UInt16 fieldID)
{
	FormPtr 	frm;
	FieldPtr	fld;
	UInt16		obj;
	Char		*p;

	frm = FrmGetFormPtr(formID);
	obj = FrmGetObjectIndex(frm, fieldID);
	fld = (FieldPtr)FrmGetObjectPtr(frm, obj);
	p = FldGetTextPtr(fld);
	return (p ? p : "");
}


static FieldPtr SetField(UInt16 formID, UInt16 fieldID, MemPtr str)
{
	FormPtr 	frm;
	UInt16		obj;
	FieldPtr	fld;
	CharPtr		p;
	VoidHand	h;

	frm = FrmGetFormPtr(formID);
	obj = FrmGetObjectIndex(frm, fieldID);
	fld = (FieldPtr)FrmGetObjectPtr(frm, obj);
	h = (VoidHand)FldGetTextHandle(fld);
	if (h == NULL)
	{
		h = MemHandleNew (FldGetMaxChars(fld)+1);
		ErrFatalDisplayIf(!h, "No Memory");
	}

	p = (CharPtr)MemHandleLock(h);
	StrCopy(p, str);
	MemHandleUnlock(h);

	FldSetTextHandle(fld, (Handle)h);
}


static Int16 GetControl(UInt16 formID, UInt16 controlID)
{
	FormPtr 	frm;
	UInt16		obj;
	ControlPtr	ctl;

	frm = FrmGetFormPtr(formID);
	obj = FrmGetObjectIndex(frm, controlID);
	ctl = (ControlPtr)FrmGetObjectPtr(frm, obj);
	return CtlGetValue(ctl);
}


static void SetControl(UInt16 formID, UInt16 controlID, Int16 value)
{
	FormPtr 	frm;
	UInt16		obj;
	ControlPtr ctl;

	frm = FrmGetFormPtr(formID);
	obj = FrmGetObjectIndex(frm, controlID);
	ctl = (ControlPtr)FrmGetObjectPtr(frm, obj);
	CtlSetValue (ctl, value);
}


static FieldPtr GetFocusObjectPtr(void)
{
	FormPtr frm;
	UInt16 focus;
	FormObjectKind objType;

	frm = FrmGetActiveForm();
	focus = FrmGetFocus(frm);
	if (focus == noFocus)
		return(NULL);

	objType = FrmGetObjectType(frm, focus);

	if (objType == frmFieldObj)
		return(FrmGetObjectPtr(frm, focus));

	else if (objType == frmTableObj)
		return(TblGetCurrentField(FrmGetObjectPtr(frm, focus)));

	return (NULL);
}


static void SelectRecord(UInt16 index)
{
	MemHandle			recH;
	DBRecordTypePtr		recP;
	UInt16				attr;
	UInt32				uniqueID;
	char				buf[20];

	DmDatabaseSize(dbCard, dbID, NULL, &totalBytes, &dataBytes);
	SetField(MainForm, MainDataBytes, StrIToA(buf, dataBytes));
	SetField(MainForm, MainTotalBytes, StrIToA(buf, totalBytes));

	if (recH = (MemHandle)DmQueryRecord(db, index))
	{
		if (DmRecordInfo(db, index, &attr, &uniqueID, NULL))
		{
			FrmCustomAlert(ErrorAlert, "Unable to get record info", "", "");
			return;
		}

		recP = (DBRecordTypePtr)MemHandleLock(recH);

		StrPrintF(buf, "%4lx", uniqueID);
		SetField(MainForm, MainUniqueID  , buf);
		SetField(MainForm, MainName      , recP->name);
		StrIToA(buf, recP->quantity);
		SetField(MainForm, MainQuantity  , buf);

		MemHandleLock(recH);

		SetControl(MainForm, MainAttrDelete, (attr & dmRecAttrDelete) ? 1 : 0);
		SetControl(MainForm, MainAttrDirty , (attr & dmRecAttrDirty ) ? 1 : 0);
		SetControl(MainForm, MainAttrBusy  , (attr & dmRecAttrBusy  ) ? 1 : 0);
		SetControl(MainForm, MainAttrSecret, (attr & dmRecAttrSecret) ? 1 : 0);
	}
	else
	{
		SetField(MainForm, MainUniqueID  , "");
		SetField(MainForm, MainName      , "");
		SetField(MainForm, MainQuantity  , "");
		SetControl(MainForm, MainAttrDelete, 0);
		SetControl(MainForm, MainAttrDirty , 0);
		SetControl(MainForm, MainAttrBusy  , 0);
		SetControl(MainForm, MainAttrSecret, 0);
		curRec = -1;
	}

	if (nbRec > 0)
		StrPrintF(buf, "%2d/%-2d", curRec+1, nbRec);
		else StrCopy(buf, "");
	SetField(MainForm, MainCursor, buf);

	FrmDrawForm(FrmGetFormPtr(MainForm));
}


static void UpdateRecord()
{
	UInt16				index = curRec;
	MemHandle			recH;
	MemPtr				recP;
	UInt16				attr;
	char				buf[20];

	if (DmRecordInfo(db, index, &attr, NULL, NULL))
	{
		FrmCustomAlert(ErrorAlert, "Unable to get record info", "", "");
		return;
	}

	if ((attr & dmRecAttrBusy) != 0)
	{
		FrmCustomAlert(ErrorAlert, "Record is locked", "", "");
		return;
	}

	if (recH = (MemHandle)DmGetRecord(db, index))
	{
		if (GetControl(MainForm, MainAttrSecret))
			attr |= dmRecAttrSecret;
			else attr &= ~dmRecAttrSecret;
		if (DmSetRecordInfo(db, index, &attr, NULL))
		{
			FrmCustomAlert(ErrorAlert, "Error setting record info", "", "");
			return;
		}

		StrCopy(dbRecord.name, GetField(MainForm, MainName));
		dbRecord.quantity = StrAToI(GetField(MainForm, MainQuantity));

		recP = MemHandleLock(recH);
		DmWrite(recP, 0, &dbRecord, sizeof(DBRecordType));
		MemPtrUnlock(recP);

		DmReleaseRecord(db, index, true);
	}
	else FrmCustomAlert(ErrorAlert, "Unable to get record", "", "");
}


static void DeleteRecord()
{
	UInt16		attr;
	Err			error;

	if (DmRecordInfo(db, curRec, &attr, NULL, NULL))
	{
		FrmCustomAlert(ErrorAlert, "Unable to get record info", "", "");
		return;
	}

	if ((attr & dmRecAttrDelete) == 0)
		if ((error = DmRemoveRecord(db,curRec)) == 0)	// delete OK: we need to find a valid "current" rec, forwards OR backwards
			{
				UInt16	seek = curRec;
				while (seek < nbRec)						// fast forward :)
					if (DmQueryRecord(db, seek) == NULL)
						seek++;
						else break;							// found one!
				if (seek >= nbRec)							// found none :o(
				{
					seek = curRec;
					while (--seek > 0)						// rewind :)
						if (DmQueryRecord(db, seek) != NULL)
							break;							// found one!
				}

				curRec = seek;								// got the new "current"
				nbRec--;									// decrement rec count
				SelectRecord(curRec);
			}
			else ErrAlert(error);
		else FrmCustomAlert(ErrorAlert, "Record already deleted", "", "");
}


static void LockRecord()
{
	UInt16		attr;
	MemHandle	recH;

	if (DmRecordInfo(db, curRec, &attr, NULL, NULL))
	{
		FrmCustomAlert(ErrorAlert, "Unable to get record info", "", "");
		return;
	}

	if ((attr & dmRecAttrBusy) == 0)
		if (recH = (MemHandle)DmGetRecord(db, curRec))
			SelectRecord(curRec);
			else ErrAlert(DmGetLastErr());
		else FrmCustomAlert(ErrorAlert, "Record already locked", "", "");
}


static void UnlockRecord()
{
	UInt16		attr;

	if (DmRecordInfo(db, curRec, &attr, NULL, NULL))
	{
		FrmCustomAlert(ErrorAlert, "Unable to get record info", "", "");
		return;
	}

	if ((attr & dmRecAttrBusy) != 0)
		if (DmReleaseRecord(db, curRec, true) == errNone)
			SelectRecord(curRec);
			else FrmCustomAlert(ErrorAlert, "Unable to unlock record", "", "");
		else FrmCustomAlert(ErrorAlert, "Record not locked", "", "");
}


static void InsertRecord()
{
	UInt16		index;
	MemHandle 	recH;
	MemPtr		recP;

	// Add a new record at the end of the database.
	index = DmNumRecords(db);
	if (!(recH = DmNewRecord(db, &index, sizeof(DBRecordType))))
	{
		FrmCustomAlert(ErrorAlert, "Unable to create record", "", "");
		return;
	}

	StrCopy(dbRecord.name, GetField(InsertForm, InsertName));
	dbRecord.quantity = StrAToI(GetField(InsertForm, InsertQuantity));

	recP = MemHandleLock(recH);
	DmWrite(recP, 0, &dbRecord, sizeof(DBRecordType));
	MemPtrUnlock(recP);

	DmReleaseRecord(db, index, true);

	curRec = index;
	nbRec++;
}


static Boolean MainFormHandleEvent(EventPtr event)
{
    Boolean		handled = false;
    FieldPtr	fld;

    switch (event->eType)
    {
	    case frmOpenEvent:
			SelectRecord(curRec);
			handled = true;
			break;

	    case menuEvent:
			MenuEraseStatus(NULL);
			switch (event->data.menu.itemID)
			{
				case MainEditClearCmd:
					if (fld = GetFocusObjectPtr())
						FldDelete(fld, 0, FldGetTextLength(fld));
				    handled = true;
				    break;

				case MainEditCopyCmd:
					if (fld = GetFocusObjectPtr())
						FldCopy(fld);
				    handled = true;
				    break;

				case MainEditCutCmd:
					if (fld = GetFocusObjectPtr())
						FldCut(fld);
				    handled = true;
				    break;

				case MainEditPasteCmd:
					if (fld = GetFocusObjectPtr())
						FldPaste(fld);
				    handled = true;
				    break;

				case MainEditSelectAllCmd:
					if (fld = GetFocusObjectPtr())
						FldSetSelection(fld, 0, FldGetTextLength(fld));
				    handled = true;
				    break;

				case MainOptionsHelpCmd:
				    (void)FrmAlert(HelpAlert);
				    handled = true;
				    break;

				case MainOptionsAboutCmd:
				    (void)FrmAlert(AboutAlert);
				    handled = true;
				    break;

				default:
					break;
			}
			break;

	    case ctlSelectEvent:
			switch (event->data.ctlSelect.controlID)
			{
				case MainUpdate:
DoUpdate:
					if (curRec >= 0)
						UpdateRecord();
				    handled = true;
				    break;

				case MainDelete:
DoDelete:
					if (curRec >= 0)
						DeleteRecord();
				    handled = true;
				    break;

				case MainLock:
DoLock:
					if (curRec >= 0)
						LockRecord();
				    handled = true;
				    break;

				case MainUnlock:
DoUnlock:
					if (curRec >= 0)
						UnlockRecord();
				    handled = true;
				    break;

				case MainFirstRec:
					SelectRecord(curRec = 0);
				    handled = true;
				    break;

				case MainPrevRec:
					if (curRec > 0)
						SelectRecord(--curRec);
				    handled = true;
				    break;

				case MainNextRec:
					if (curRec < nbRec-1)
						SelectRecord(++curRec);
				    handled = true;
				    break;

				case MainLastRec:
					SelectRecord(curRec = nbRec-1);
				    handled = true;
				    break;

				case MainInsert:
					FrmPopupForm(InsertForm);
				    handled = true;
				    break;

				default:
					break;
			}
			break;

	    case keyDownEvent:
			switch (event->data.keyDown.chr)
			{
				case vchrHard1:
					goto DoUpdate;

				case vchrHard2:
					goto DoDelete;

				case vchrHard3:
					goto DoLock;

				case vchrHard4:
					goto DoUnlock;

				case vchrPageDown:
					if (curRec > 0)
						SelectRecord(--curRec);
				    handled = true;
					break;

				case vchrPageUp:
					if (curRec < nbRec-1)
						SelectRecord(++curRec);
				    handled = true;
				    break;

				default:
					break;
			}
			break;

	    case penDownEvent:
			break;

	    case penUpEvent:
			break;

	    case frmCloseEvent:
			break;

	    case nilEvent:
			break;

	    default:
			break;
    }

    return handled;
}


static Boolean InsertFormHandleEvent(EventPtr event)
{
    Boolean		handled = false;
    FieldPtr	fld;

    switch (event->eType)
    {
	    case frmOpenEvent:
			FrmDrawForm(FrmGetFormPtr(InsertForm));
			handled = true;
			break;

	    case ctlSelectEvent:
			switch (event->data.ctlSelect.controlID)
			{
				case InsertOK:
					if (GetField(InsertForm, InsertName) != "")
					{
						InsertRecord();
						FrmReturnToForm(0);
						SelectRecord(curRec);
					}
					else FrmCustomAlert(ErrorAlert, "Please complete form", "Enter name", "");
				    handled = true;
				    break;

				case InsertCancel:
					FrmReturnToForm(0);
				    handled = true;
				    break;

				default:
					break;
			}
			break;

	    default:
			break;
    }

    return handled;
}


static Boolean AppHandleEvent(EventPtr event)
{
    FormPtr	frm;
    Int		formId;
    Boolean	handled;

    handled = false;

    if (event->eType == frmLoadEvent)
    {
		formId = event->data.frmLoad.formID;
		frm = FrmInitForm(formId);
		FrmSetActiveForm(frm);
		switch (formId)
		{
			case MainForm:
				FrmSetEventHandler(frm, MainFormHandleEvent);
				handled = true;
				break;

			case InsertForm:
				FrmSetEventHandler(frm, InsertFormHandleEvent);
				handled = true;
				break;

			default:
				break;
		}
    }

    return handled;
}


static Err AppStart()
{
	UInt16	attributes;
	UInt32	locks;
	Err		retcode = 0;

	if (db = DmOpenDatabaseByTypeCreator(DBType, CreatorID, dmModeReadWrite | dmModeShowSecret))
		DmOpenDatabaseInfo(db, &dbID, NULL, NULL, &dbCard, NULL);
		else
		{
			if (retcode = DmCreateDatabase(0, DBName, CreatorID, DBType, false))
				return retcode;

			if (!(db = DmOpenDatabaseByTypeCreator(DBType, CreatorID, dmModeReadWrite | dmModeShowSecret)))
				return dmErrCantOpen;

			DmOpenDatabaseInfo(db, &dbID, NULL, NULL, &dbCard, NULL);
			DmDatabaseInfo(dbCard, dbID, NULL, &attributes, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
			attributes |= dmHdrAttrBackup;
			DmSetDatabaseInfo(dbCard, dbID, NULL, &attributes, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		}

    DmGetDatabaseLockState(db, NULL, NULL, &locks);
    DmDatabaseProtect(dbCard, dbID, (locks > 0) ? false : true);			// unprotect/protect DB accordingly, to balance AppClose similar call

	nbRec = DmNumRecords(db);

	FrmGotoForm(MainForm);

	return retcode;
}


static void AppEventLoop(void)
{
	EventType	event;
	short		error;

    do
    {
		EvtGetEvent(&event, 100);

		if	(										// intercept hard key
				(event.eType == keyDownEvent) &&
				(TxtCharIsHardKey(event.data.keyDown.modifiers, event.data.keyDown.chr)) &&
				(event.data.keyDown.chr >= vchrHard1) &&
				(event.data.keyDown.chr <= vchrHard4) &&
				(!(event.data.keyDown.modifiers & poweredOnKeyMask))
			)
			goto AppEvent;

		if (SysHandleEvent(&event))
			continue;

		if (MenuHandleEvent((void *)0, &event, &error))
			continue;

AppEvent:
		if (AppHandleEvent(&event))
			continue;

		FrmDispatchEvent(&event);
    }
    while (event.eType != appStopEvent);
}


static Err AppStop()
{
	UInt32	locks;
	Boolean	protect = false;
	Err		retcode;

    FrmCloseAllForms();

    DmGetDatabaseLockState(db, NULL, NULL, &locks);
    if (protect = (locks > 0))								// any record locked?
    {
    	char	buf[40];

    	StrPrintF(buf, "%ld record(s) still locked.", locks);
    	FrmCustomAlert(ErrorAlert, buf, "Database will be closed protected.", "If you don't unlock every record, you WON'T be allowed to delete the database.");
    }
    DmDatabaseProtect(dbCard, dbID, protect);				// protect/unprotect DB accordingly

		DmCloseDatabase(db);

    return retcode;
}


UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	UInt32 romVersion;

	FtrGet(sysFtrCreator, sysFtrNumROMVersion, &romVersion);	// Minimal ROM version requirement
	if (romVersion < ROM_VERSION_REQUIRED)
	{
		FrmAlert(RomIncompatibleAlert);
		return(sysErrRomIncompatible);
	}

	if (cmd == sysAppLaunchCmdNormalLaunch)						// Normal launch
	{
		Err retcode;

		if ((retcode = AppStart()) != 0)
			return(retcode);
		AppEventLoop();
		AppStop();
	}

	return 0;
}
