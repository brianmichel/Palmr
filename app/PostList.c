#include "PostList.h"
#include "PostView.h"
#include "PalmrDB.h"
#include "Util.h"

// static Boolean PostListTableHandler(EventPtr event);

void UpdatePostsTable(FormPtr pForm, UInt16 numberOfRows)
{
  UInt16 rowsToDraw;
  UInt16		obj = FrmGetObjectIndex(pForm, PostListTable);
  TablePtr	table = (TablePtr)FrmGetObjectPtr(pForm, obj);
  char    *icon[PostListMaxRows] = {"*", "*", "*", "*", "*", "*", "*", "*", "*", "*"};
  char    *userName[PostListMaxRows] = { "Chungo", "Scrungus", "Meeper", "Sreegs", "Brian", "Ken", "Ello", "I'm", "Oli", "Test"};
  char    *postPreview[PostListMaxRows] = {  "Cut my life into pieces",
                                          "This is my last resort",
                                          "Wake me up inside",
                                          "I can't wake up",
                                          "Today I ate a piece of toast",
                                          "The discourse...",
                                          "Top ten things I love about squart",
                                          "This is just sample data",
                                          "If you're reading this the app is broke",
                                          "Testing 1 2 3"};
  register	i;

  if (numberOfRows > PostListMaxRows) {
    AlertPrintf3(
        "Number of posts exceeded the limit. Reducing to 10.",
        NULL, NULL);
    rowsToDraw = PostListMaxRows;
  } else {
    rowsToDraw = numberOfRows;
  }

  for (i = 0; i < rowsToDraw; i++)
  {
    TblSetItemStyle (table, i, 0, labelTableItem);
    TblSetItemStyle (table, i, 1, labelTableItem);
    TblSetItemStyle (table, i, 2, labelTableItem);
    TblSetItemPtr(table, i, 0, icon[i]);
    TblSetItemPtr(table, i, 1, userName[i]);
    TblSetItemPtr(table, i, 2, postPreview[i]);
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
    UInt16 postCount;

    switch (event->eType) {
    case frmOpenEvent: {
        postCount = 10; // If you exceed 10 you pop a warning and it gets trimmed.
        setup_database();
        FrmDrawForm(gpForm = FrmGetActiveForm());
        UpdatePostsTable(gpForm, postCount);

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
    case tblSelectEvent: {
      int row = event->data.tblSelect.row;
      int column = event->data.tblSelect.column;
      // char strRow[15];
      // char strColumn[15];
      //
      // sprintf(strRow, "%i", row);
      // sprintf(strColumn, "%i", column);

      // AlertPrintf3(
      //     "You selected:",
      //     strRow, strColumn);

      FrmGotoForm(PostViewForm);

      return true;
    }
    default: {
        return false;
    }
    }
}
