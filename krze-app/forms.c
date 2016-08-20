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


/* include the .h */
#include <PalmTypes.h>
#include <PalmCompatibility.h>
#include <System/SystemPublic.h>
#include <UI/UIPublic.h>


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

	/* display counter */
}


void UpdateTable()
{
	FormPtr		frm = FrmGetFormPtr(Form4);
	UInt16		obj = FrmGetObjectIndex(frm, Table);
	TablePtr	table = (TablePtr)FrmGetObjectPtr(frm, obj);
	char		*strings[TableRows] = {"Uno", "Dos", "Tres", "Cuatro", "Cinco", "Seis", "Siete", "Ocho"};
	register	i;

	for (i = 0; i < TableRows; i++)
	{
		TblSetItemStyle (table, i, 0, numericTableItem);
		TblSetItemStyle (table, i, 1, labelTableItem);
		TblSetItemInt(table, i, 0, i+1);
		TblSetItemPtr(table, i, 1, strings[i]);
		/* TblSetRowUsable */
//OS3.5:		TblSetRowMasked(table, i, ((i%2) == 0) ? true : false);
//		TblSetRowSelectable(table, i, ((i%2) == 0) ? false : true);
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
						/* handle Next */
						else
						if (e.data.ctlSelect.controlID == Exit)
							goto _quit;
						goto _default;
						
					case ctlRepeatEvent:
						if (e.data.ctlRepeat.controlID == Form3Minus)
						{
							if (cnt > 0)
								UpdateCount(-1);
						}
						else
						/* handle "+" button */
						goto _default;
				
					case frmLoadEvent:
					    /* set active form */
						break;
			
					case frmOpenEvent:
						pfrm = FrmGetActiveForm();
						/* draw the form */
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
    	/* close them open formz :) */
	}

	return 0;
}
