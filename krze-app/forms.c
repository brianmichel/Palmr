/*****************************************************************************\
*                                                                             *
* Project       : Mobile development tutorial                                 *
* Application   : Demonstration of forms development                          *
* Component     : Main                                                        *
* Author        : Eric Poncet (mobile.eric-poncet.com)                        *
* Creation      : 06/24/2000                                                  *
*                                                                             *
*******************************************************************************
*                                                                             *
* History       :                                                             *
*                                                                             *
\*****************************************************************************/


#include "forms.h"
#include <PalmTypes.h>
#include <PalmCompatibility.h>
#include <System/SystemPublic.h>
#include <UI/UIPublic.h>

#define MIN_COUNT 0
#define	MAX_COUNT	9999

int	cnt = 1000;

void UpdateCount(UInt16 val)
{
	FormPtr 	frm;
	FieldPtr	fld;
	UInt16		obj;
	CharPtr		p;
	VoidHand	h;

	cnt += val;

	frm = FrmGetActiveForm();
	obj = FrmGetObjectIndex(frm, Form3Count);
	fld = (FieldPtr)FrmGetObjectPtr(FrmGetActiveForm(), obj);
	h = (VoidHand)FldGetTextHandle(fld);

	if (h == NULL)
	{
		h = MemHandleNew(FldGetMaxChars(fld)+10);
		ErrFatalDisplayIf(!h, "No Memory");
	}

	p = (CharPtr)MemHandleLock(h);
	StrIToA(p, cnt);
	MemHandleUnlock(h);

	FldSetTextHandle(fld, (Handle)h);
	FldDrawField(fld);
}


void UpdateTable()
{
	FormPtr		frm = FrmGetFormPtr(Form4);
	UInt16		obj = FrmGetObjectIndex(frm, Table);
	TablePtr	table = (TablePtr)FrmGetObjectPtr(frm, obj);
	char		*strings[TableRows] = {"Uno", "Dos", "Tres", "Cuatro", "Cinco", "Seis", "Siete", "Ocho"};
	register int	i;

	for (i = 0; i < TableRows; i++)
	{
		TblSetItemStyle (table, i, 0, numericTableItem);
		TblSetItemStyle (table, i, 1, labelTableItem);
		TblSetItemInt(table, i, 0, i+1);
		TblSetItemPtr(table, i, 1, strings[i]);
		TblSetRowUsable(table, i, true);
// OS3.5:		TblSetRowMasked(table, i, ((i%2) == 0) ? true : false);
		// TblSetRowSelectable(table, i, ((i%2) == 0) ? false : true);
		TblMarkRowInvalid(table, i);
	}

	for (i = 0; i < TableCols; i++)
	{
		TblSetColumnUsable(table, i, true);
	}
//	TblSetRowUsable(table, 0, false);
//	TblRedrawTable(table);
//	FrmUpdateForm (Table, 0);
	TblRedrawTable(table);
}


UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	short err;
	EventType e;
	FormType *pfrm;
	short f = FormFirst;		// Start with 1st form 1st :)

	if (cmd == sysAppLaunchCmdNormalLaunch)			// Make sure only react to NormalLaunch, not Reset, Beam, Find, GoTo...
	{
		FrmGotoForm(f);
		while(1)
		{
			EvtGetEvent(&e, 100);
			if (SysHandleEvent(&e))
				continue;
			if (MenuHandleEvent((void *)0, &e, &err))
				continue;

			switch (e.eType)
			{
					case ctlSelectEvent:
						if (e.data.ctlSelect.controlID == Prev)
						{
							if (f > FormFirst)
								FrmGotoForm(--f);
						}
						else
						if (e.data.ctlSelect.controlID == Next)
						{
							if (f < FormLast)
								FrmGotoForm(++f);
						}
						else
						if (e.data.ctlSelect.controlID == Exit)
							goto _quit;
						goto _default;

					case ctlRepeatEvent:
						if (e.data.ctlRepeat.controlID == Form3Minus)
						{
							if (cnt > MIN_COUNT)
								UpdateCount(-1);
						}
						else
						if (e.data.ctlRepeat.controlID == Form3Plus)
						{
							if (cnt < MAX_COUNT)
								UpdateCount(+1);
						}
						goto _default;

					case frmLoadEvent:
					    FrmSetActiveForm(FrmInitForm(e.data.frmLoad.formID));
						break;

					case frmOpenEvent:
						pfrm = FrmGetActiveForm();
						FrmDrawForm(pfrm);
						if (e.data.frmLoad.formID == Form3)
							UpdateCount(0);
							else
						if (e.data.frmLoad.formID == Form4)
							UpdateTable();

						break;

					case menuEvent:
						break;

					case appStopEvent:
						goto _quit;

					default:
_default:
						if (FrmGetActiveForm())
							FrmHandleEvent(FrmGetActiveForm(), &e);
			}
		}

_quit:
    	FrmCloseAllForms();
	}

	return 0;
}
