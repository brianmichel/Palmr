#ifndef _POSTLIST_H_
#define _POSTLIST_H_
#include <System/SystemPublic.h>
#include <UI/UIPublic.h>

#include "PalmrRsc.h"

void UpdatePostsTable(FormPtr pForm, UInt16 numberOfRows);
Boolean PostListFormEventHandler(EventPtr event);

#endif
