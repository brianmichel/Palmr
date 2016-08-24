#include "PostList.h"
#include "PalmrDB.h"

// static Boolean PostListTableHandler(EventPtr event);

void UpdatePostsTable(FormPtr pForm)
{
  UInt16		obj = FrmGetObjectIndex(pForm, PostListTable);
  TablePtr	table = (TablePtr)FrmGetObjectPtr(pForm, obj);
  char		*strings[PostListRows] = {"Uno", "Dos", "Tres", "Cuatro", "Cinco", "Seis", "Siete", "Ocho", "Fuck", "Piss"};
  register	i;

  for (i = 0; i < PostListRows; i++)
  {
    TblSetItemStyle (table, i, 0, numericTableItem);
    TblSetItemStyle (table, i, 1, labelTableItem);
    TblSetItemInt(table, i, 0, i+1);
    TblSetItemPtr(table, i, 1, strings[i]);
    TblSetRowUsable(table, i, true);
  //OS3.5:		TblSetRowMasked(table, i, ((i%2) == 0) ? true : false);
  //		TblSetRowSelectable(table, i, ((i%2) == 0) ? false : true);
    TblMarkRowInvalid(table, i);
  }

  for (i = 0; i < PostListColumns; i++)
  {
    TblSetColumnUsable(table, i, true);
  }
  //	TblSetRowUsable(table, 0, false);
  //	TblRedrawTable(table);
  //	FrmUpdateForm (Table, 0);
  TblRedrawTable(table);
}

Boolean PostListFormEventHandler(EventPtr event)
{
    static FormPtr gpForm;

    switch (event->eType) {
    case frmOpenEvent: {
        setup_database();
        FrmDrawForm(gpForm = FrmGetActiveForm());
        UpdatePostsTable(gpForm);

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
