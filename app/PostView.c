#include "PostView.h"
#include "PalmrRsc.h"
#include "Util.h"

void UpdatePostView(FormPtr pForm)
{
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
    }
    // TODO Menu needs a back button event which goes back to the post list view
    
    return false;
}
