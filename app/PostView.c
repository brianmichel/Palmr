#include "PostView.h"
#include "PalmrRsc.h"
#include "Util.h"

void UpdatePostView(FormPtr pForm)
{
  // UInt16 authorLabel = FrmGetObjectIndex(pForm, PostAuthorLabel);
  UInt16 bodyField   = FrmGetObjectIndex(pForm, PostBodyField);
  // UInt16 fldPtr = (FieldType)FldGetTextPtr(bodyField);
  UInt16 fldPtr = FrmGetObjectPtr(pForm, bodyField);
  UInt16 fldTextPtr = FldGetTextPtr(fldPtr);
  char* testAuthor = "Scrungus";
  char* testBody = "test.";

  FrmCopyTitle(pForm, "Bing Bong. A Post!");
  FrmCopyLabel(pForm, PostAuthorLabel, testAuthor);

  // FldSetUsable(bodyField, false);
  FldSetTextPtr(fldTextPtr, testBody);
  FldDrawField(fldPtr);

    // TODO Set up single post view here. Get the active form. Get each object from form. Just like in UpdatePostsTable
}

Boolean PostViewEventHandler(EventPtr event)
{
    static FormPtr gpForm;

    switch (event->eType) {
    case frmOpenEvent: {
        FrmDrawForm(gpForm = FrmGetActiveForm());
        UpdatePostView(gpForm);

        return true;
    }
    case menuEvent: {
        return true;
    }
    default:
        return false;
    }
    // TODO Menu needs a back button event which goes back to the post list view

    return false;
}
