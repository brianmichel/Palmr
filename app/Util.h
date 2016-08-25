#ifndef _PALMR_UTIL_H_
#define _PALMR_UTIL_H_

#include "PalmrRsc.h"
#include <System/SystemPublic.h>
#include <UI/UIPublic.h>

#ifndef Assert
#define Assert(EX) ((EX) ? (void)0 : ErrDisplayFileLineMsg(__FILE__, __LINE__, #EX))
#endif

#define AlertPrintf1(a) FrmCustomAlert(PrintfAlert, (a), " ", " ")
#define AlertPrintf2(a, b) FrmCustomAlert(PrintfAlert, (a), (b), " ")
#define AlertPrintf3(a, b, c) FrmCustomAlert(PrintfAlert, (a), (b), (c))

#define AlertPrintf AlertPrintf2

#endif
