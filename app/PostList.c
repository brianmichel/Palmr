#include "PostList.h"

// static Boolean PostListTableHandler(EventPtr event);

void UpdatePostsTable(FormPtr pForm)
{
  FormPtr		frm = FrmGetFormPtr(pForm);
  UInt16		obj = FrmGetObjectIndex(frm, Table);
  TablePtr	table = (TablePtr)FrmGetObjectPtr(frm, obj);
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
};
