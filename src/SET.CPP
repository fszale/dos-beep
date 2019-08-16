#include <stdio.h>
#include <dos.h>
#include <ctype.h>
#include <graph.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "menu.h"
extern "C"
{
#include "cbtree.h"
}
#include "convert.h"
#include "wapi.h"
#include "dbfile.h"
#include "dosbeep.h"
#include "comm.h"
#include "state.h"
#include "timerf.h"
#include "com.h"
#include "defs.h"

//----------------------------------------------------------------
//
//  void SetServiceControls(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
void SetServiceControls(PCONTROL_PARAM pCntlInfo)
{
   strcpy((pCntlInfo + 0)->pCntlTitle, "   Service Name :");
   (pCntlInfo + 0)->nAlignment = LEFT;
   (pCntlInfo + 0)->nAlignmentOffset = 22;
   (pCntlInfo + 0)->nCntlRow = 4;
   (pCntlInfo + 0)->nCntlOffset = 18;
   (pCntlInfo + 0)->nCntlLength = 30;
   (pCntlInfo + 0)->pCntlBuffer[0] = 0;
   
   strcpy((pCntlInfo + 1)->pCntlTitle, "          Phone :");
   (pCntlInfo + 1)->nAlignment = LEFT;
   (pCntlInfo + 1)->nAlignmentOffset = 22;
   (pCntlInfo + 1)->nCntlRow = 6;
   (pCntlInfo + 1)->nCntlOffset = 18;
   (pCntlInfo + 1)->nCntlLength = 20;
   (pCntlInfo + 1)->pCntlBuffer[0] = 0;
   
   strcpy((pCntlInfo + 2)->pCntlTitle,  "       Password :");
   (pCntlInfo + 2)->nAlignment = LEFT;
   (pCntlInfo + 2)->nAlignmentOffset = 22;
   (pCntlInfo + 2)->nCntlRow = 8;
   (pCntlInfo + 2)->nCntlOffset = 18;
   (pCntlInfo + 2)->nCntlLength = 10;
   (pCntlInfo + 2)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 3)->pCntlTitle,  " Message Length :");
   (pCntlInfo + 3)->nAlignment = LEFT;
   (pCntlInfo + 3)->nAlignmentOffset = 22;
   (pCntlInfo + 3)->nCntlRow = 9;
   (pCntlInfo + 3)->nCntlOffset = 18;
   (pCntlInfo + 3)->nCntlLength = 3;
   (pCntlInfo + 3)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 4)->pCntlTitle,  "      Baud Rate :");
   (pCntlInfo + 4)->nAlignment = LEFT;
   (pCntlInfo + 4)->nAlignmentOffset = 22;
   (pCntlInfo + 4)->nCntlRow = 11;
   (pCntlInfo + 4)->nCntlOffset = 18;
   (pCntlInfo + 4)->nCntlLength = 6;
   (pCntlInfo + 4)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 5)->pCntlTitle,  "         Parity :");
   (pCntlInfo + 5)->nAlignment = LEFT;
   (pCntlInfo + 5)->nAlignmentOffset = 22;
   (pCntlInfo + 5)->nCntlRow = 12;
   (pCntlInfo + 5)->nCntlOffset = 18;
   (pCntlInfo + 5)->nCntlLength = 1;
   (pCntlInfo + 5)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 6)->pCntlTitle,  "      Data Bits :");
   (pCntlInfo + 6)->nAlignment = LEFT;
   (pCntlInfo + 6)->nAlignmentOffset = 22;
   (pCntlInfo + 6)->nCntlRow = 13;
   (pCntlInfo + 6)->nCntlOffset = 18;
   (pCntlInfo + 6)->nCntlLength = 5;
   (pCntlInfo + 6)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 7)->pCntlTitle,  "      Stop Bits :");
   (pCntlInfo + 7)->nAlignment = LEFT;
   (pCntlInfo + 7)->nAlignmentOffset = 22;
   (pCntlInfo + 7)->nCntlRow = 14;
   (pCntlInfo + 7)->nCntlOffset = 18;
   (pCntlInfo + 7)->nCntlLength = 3;
   (pCntlInfo + 7)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 8)->pCntlTitle,  "Installed Modem :");
   (pCntlInfo + 8)->nAlignment = LEFT;
   (pCntlInfo + 8)->nAlignmentOffset = 6;
   (pCntlInfo + 8)->nCntlRow = 16;
   (pCntlInfo + 8)->nCntlOffset = 18;
   (pCntlInfo + 8)->nCntlLength = 50;
   (pCntlInfo + 8)->pCntlBuffer[0] = 0;
}
//----------------------------------------------------------------
//
//  void SetSubscriberControls(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
void SetSubscriberControls(PCONTROL_PARAM pCntlInfo)
{
                                 
   strcpy((pCntlInfo + 0)->pCntlTitle,  "Subscriber Name :");
   (pCntlInfo + 0)->nAlignment = LEFT;
   (pCntlInfo + 0)->nAlignmentOffset = 23;
   (pCntlInfo + 0)->nCntlRow = 8;
   (pCntlInfo + 0)->nCntlOffset = 18;
   (pCntlInfo + 0)->nCntlLength = WAPI_SUBSCRIBER_NAME_LEN;
   (pCntlInfo + 0)->pCntlBuffer[0] = 0;
   
   strcpy((pCntlInfo + 1)->pCntlTitle,  "     Pin Number :");
   (pCntlInfo + 1)->nAlignment = LEFT;
   (pCntlInfo + 1)->nAlignmentOffset = 23;
   (pCntlInfo + 1)->nCntlRow = 10;
   (pCntlInfo + 1)->nCntlOffset = 18;
   (pCntlInfo + 1)->nCntlLength = WAPI_PIN_LEN;
   (pCntlInfo + 1)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 2)->pCntlTitle,  "   Service Name :");
   (pCntlInfo + 2)->nAlignment = LEFT;
   (pCntlInfo + 2)->nAlignmentOffset = 23;
   (pCntlInfo + 2)->nCntlRow = 13;
   (pCntlInfo + 2)->nCntlOffset = 18;
   (pCntlInfo + 2)->nCntlLength = WAPI_SERVICE_NAME_LEN;
   (pCntlInfo + 2)->pCntlBuffer[0] = 0;
 
}

//----------------------------------------------------------------
//
//  void SetModemControls(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
void SetModemControls(PCONTROL_PARAM pCntlInfo)
{
                                 
   strcpy((pCntlInfo + 0)->pCntlTitle,  "        Modem Name :");
   (pCntlInfo + 0)->nAlignment = LEFT;
   (pCntlInfo + 0)->nAlignmentOffset = 8;
   (pCntlInfo + 0)->nCntlRow = 6;
   (pCntlInfo + 0)->nCntlOffset = 21;
   (pCntlInfo + 0)->nCntlLength = 40;
   (pCntlInfo + 0)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 1)->pCntlTitle,  "       Init String :");
   (pCntlInfo + 1)->nAlignment = LEFT;
   (pCntlInfo + 1)->nAlignmentOffset = 20;
   (pCntlInfo + 1)->nCntlRow = 8;
   (pCntlInfo + 1)->nCntlOffset = 21;
   (pCntlInfo + 1)->nCntlLength = 30;
   (pCntlInfo + 1)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 2)->pCntlTitle,  "     Hangup String :");
   (pCntlInfo + 2)->nAlignment = LEFT;
   (pCntlInfo + 2)->nAlignmentOffset = 20;
   (pCntlInfo + 2)->nCntlRow = 9;
   (pCntlInfo + 2)->nCntlOffset = 21;
   (pCntlInfo + 2)->nCntlLength = 11;
   (pCntlInfo + 2)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 3)->pCntlTitle,  "Dial Prefix String :");
   (pCntlInfo + 3)->nAlignment = LEFT;
   (pCntlInfo + 3)->nAlignmentOffset = 20;
   (pCntlInfo + 3)->nCntlRow = 10;
   (pCntlInfo + 3)->nCntlOffset = 21;
   (pCntlInfo + 3)->nCntlLength = 11;
   (pCntlInfo + 3)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 4)->pCntlTitle,  "Dial Suffix String :");
   (pCntlInfo + 4)->nAlignment = LEFT;
   (pCntlInfo + 4)->nAlignmentOffset = 20;
   (pCntlInfo + 4)->nCntlRow = 11;
   (pCntlInfo + 4)->nCntlOffset = 21;
   (pCntlInfo + 4)->nCntlLength = 11;
   (pCntlInfo + 4)->pCntlBuffer[0] = 0;
   
   strcpy((pCntlInfo + 5)->pCntlTitle,  "      Reset String :");
   (pCntlInfo + 5)->nAlignment = LEFT;
   (pCntlInfo + 5)->nAlignmentOffset = 20;
   (pCntlInfo + 5)->nCntlRow = 12;
   (pCntlInfo + 5)->nCntlOffset = 21;
   (pCntlInfo + 5)->nCntlLength = 11;
   (pCntlInfo + 5)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 6)->pCntlTitle,  "     Escape String :");
   (pCntlInfo + 6)->nAlignment = LEFT;
   (pCntlInfo + 6)->nAlignmentOffset = 20;
   (pCntlInfo + 6)->nCntlRow = 13;
   (pCntlInfo + 6)->nCntlOffset = 21;
   (pCntlInfo + 6)->nCntlLength = 11;
   (pCntlInfo + 6)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 7)->pCntlTitle,  " Number of Redials :");
   (pCntlInfo + 7)->nAlignment = LEFT;
   (pCntlInfo + 7)->nAlignmentOffset = 20;
   (pCntlInfo + 7)->nCntlRow = 15;
   (pCntlInfo + 7)->nCntlOffset = 21;
   (pCntlInfo + 7)->nCntlLength = 2;
   (pCntlInfo + 7)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 8)->pCntlTitle,  "Communication Port :");
   (pCntlInfo + 8)->nAlignment = LEFT;
   (pCntlInfo + 8)->nAlignmentOffset = 20;
   (pCntlInfo + 8)->nCntlRow = 17;
   (pCntlInfo + 8)->nCntlOffset = 21;
   (pCntlInfo + 8)->nCntlLength = 1;
   (pCntlInfo + 8)->pCntlBuffer[0] = 0;
 
}

//----------------------------------------------------------------
//
// void SetSendControls(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
void SetSendControls(PCONTROL_PARAM pCntlInfo, PMULT_CONTROL_PARAM pMult_CntlInfo)
{
   strcpy((pCntlInfo + 0)->pCntlTitle,  "       Send to :");
   (pCntlInfo + 0)->nAlignment = LEFT;
   (pCntlInfo + 0)->nAlignmentOffset = 2;
   (pCntlInfo + 0)->nCntlRow = 4;
   (pCntlInfo + 0)->nCntlOffset = 18;
   (pCntlInfo + 0)->nCntlLength = 30;
   (pCntlInfo + 0)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 1)->pCntlTitle,  "Text Length :");
   (pCntlInfo + 1)->nAlignment = RIGHT;
   (pCntlInfo + 1)->nAlignmentOffset = 11;
   (pCntlInfo + 1)->nCntlRow = 9;
   (pCntlInfo + 1)->nCntlOffset = 60;
   (pCntlInfo + 1)->nCntlLength = 3;
   (pCntlInfo + 1)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 2)->pCntlTitle,  "   Attach File :");
   (pCntlInfo + 2)->nAlignment = LEFT;
   (pCntlInfo + 2)->nAlignmentOffset = 2;
   (pCntlInfo + 2)->nCntlRow = 7;
   (pCntlInfo + 2)->nCntlOffset = 18;
   (pCntlInfo + 2)->nCntlLength = 50;
   (pCntlInfo + 2)->pCntlBuffer[0] = 0;

   pMult_CntlInfo->pCntlTitle = "";
   pMult_CntlInfo->nAlignment = LEFT;
   pMult_CntlInfo->nAlignmentOffset = 2;
   pMult_CntlInfo->nCntlRow = 11;
   pMult_CntlInfo->nCntlOffset = 0;
   pMult_CntlInfo->nCntlLength = 72;
   pMult_CntlInfo->nCntlLines = 12;
   pMult_CntlInfo->pMultiBuffer = (char *) calloc(((pMult_CntlInfo)->nCntlLength * (pMult_CntlInfo)->nCntlLines) + 1, sizeof(char));
}
//----------------------------------------------------------------
//
//  void SetSettingsControls(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
void SetSettingsControls(PCONTROL_PARAM pCntlInfo)
{
                                 
   strcpy((pCntlInfo + 0)->pCntlTitle,  "   Spooler directory :");
   (pCntlInfo + 0)->nAlignment = LEFT;
   (pCntlInfo + 0)->nAlignmentOffset = 5;
   (pCntlInfo + 0)->nCntlRow = 4;
   (pCntlInfo + 0)->nCntlOffset = 23;
   (pCntlInfo + 0)->nCntlLength = 50;
   (pCntlInfo + 0)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 1)->pCntlTitle,  "Phone Book directory :");
   (pCntlInfo + 1)->nAlignment = LEFT;
   (pCntlInfo + 1)->nAlignmentOffset = 5;
   (pCntlInfo + 1)->nCntlRow = 5;
   (pCntlInfo + 1)->nCntlOffset = 23;
   (pCntlInfo + 1)->nCntlLength = 50;
   (pCntlInfo + 1)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 2)->pCntlTitle,  "Data Files directory :");
   (pCntlInfo + 2)->nAlignment = LEFT;
   (pCntlInfo + 2)->nAlignmentOffset = 5;
   (pCntlInfo + 2)->nCntlRow = 6;
   (pCntlInfo + 2)->nCntlOffset = 23;
   (pCntlInfo + 2)->nCntlLength = 50;
   (pCntlInfo + 2)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 3)->pCntlTitle,  "Predefined directory :");
   (pCntlInfo + 3)->nAlignment = LEFT;
   (pCntlInfo + 3)->nAlignmentOffset = 5;
   (pCntlInfo + 3)->nCntlRow = 7;
   (pCntlInfo + 3)->nCntlOffset = 23;
   (pCntlInfo + 3)->nCntlLength = 50;
   (pCntlInfo + 3)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 4)->pCntlTitle,  "       Log directory :");
   (pCntlInfo + 4)->nAlignment = LEFT;
   (pCntlInfo + 4)->nAlignmentOffset = 5;
   (pCntlInfo + 4)->nCntlRow = 8;
   (pCntlInfo + 4)->nCntlOffset = 23;
   (pCntlInfo + 4)->nCntlLength = 50;
   (pCntlInfo + 4)->pCntlBuffer[0] = 0;
   
   strcpy((pCntlInfo + 5)->pCntlTitle,  " Default Modem :");
   (pCntlInfo + 5)->nAlignment = LEFT;
   (pCntlInfo + 5)->nAlignmentOffset = 5;
   (pCntlInfo + 5)->nCntlRow = 10;
   (pCntlInfo + 5)->nCntlOffset = 17;
   (pCntlInfo + 5)->nCntlLength = 51;
   (pCntlInfo + 5)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 6)->pCntlTitle,  "                Signature :");
   (pCntlInfo + 6)->nAlignment = LEFT;
   (pCntlInfo + 6)->nAlignmentOffset = 5;
   (pCntlInfo + 6)->nCntlRow = 12;
   (pCntlInfo + 6)->nCntlOffset = 27;
   (pCntlInfo + 6)->nCntlLength = WAPI_SIGNATURE_LEN;
   (pCntlInfo + 6)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 7)->pCntlTitle,  "         Enable Signature :");
   (pCntlInfo + 7)->nAlignment = LEFT;
   (pCntlInfo + 7)->nAlignmentOffset = 5;
   (pCntlInfo + 7)->nCntlRow = 13;
   (pCntlInfo + 7)->nCntlOffset = 27;
   (pCntlInfo + 7)->nCntlLength = 1;
   strcpy((pCntlInfo + 7)->pCntlBuffer, " ");

   strcpy((pCntlInfo + 8)->pCntlTitle,  "       Message Time Stamp :");
   (pCntlInfo + 8)->nAlignment = LEFT;
   (pCntlInfo + 8)->nAlignmentOffset = 5;
   (pCntlInfo + 8)->nCntlRow = 14;
   (pCntlInfo + 8)->nCntlOffset = 27;
   (pCntlInfo + 8)->nCntlLength = 1;
   strcpy((pCntlInfo + 8)->pCntlBuffer, " ");

   strcpy((pCntlInfo + 9)->pCntlTitle,  "        Save Last Message :");
   (pCntlInfo + 9)->nAlignment = LEFT;
   (pCntlInfo + 9)->nAlignmentOffset = 5;
   (pCntlInfo + 9)->nCntlRow = 16;
   (pCntlInfo + 9)->nCntlOffset = 27;
   (pCntlInfo + 9)->nCntlLength = 1;
   strcpy((pCntlInfo + 9)->pCntlBuffer, " ");

   strcpy((pCntlInfo + 10)->pCntlTitle,  "          Log Message(s) :");
   (pCntlInfo + 10)->nAlignment = LEFT;
   (pCntlInfo + 10)->nAlignmentOffset = 5;
   (pCntlInfo + 10)->nCntlRow = 17;
   (pCntlInfo + 10)->nCntlOffset = 27;
   (pCntlInfo + 10)->nCntlLength = 1;
   strcpy((pCntlInfo + 10)->pCntlBuffer, " ");

   strcpy((pCntlInfo + 11)->pCntlTitle,  "        Purge After Days :");
   (pCntlInfo + 11)->nAlignment = LEFT;
   (pCntlInfo + 11)->nAlignmentOffset = 5;
   (pCntlInfo + 11)->nCntlRow = 18;
   (pCntlInfo + 11)->nCntlOffset = 27;
   (pCntlInfo + 11)->nCntlLength = 2;
   (pCntlInfo + 11)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 12)->pCntlTitle,  "signature and time stamp :");
   (pCntlInfo + 12)->nAlignment = LEFT;
   (pCntlInfo + 12)->nAlignmentOffset = 5;
   (pCntlInfo + 12)->nCntlRow = 21;
   (pCntlInfo + 12)->nCntlOffset = 27;
   (pCntlInfo + 12)->nCntlLength = 1;
   strcpy((pCntlInfo + 12)->pCntlBuffer, " ");
}
//----------------------------------------------------------------
//
// void SetPredefControls(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
void SetPredefControls(PCONTROL_PARAM pCntlInfo, PMULT_CONTROL_PARAM pMult_CntlInfo)
{
                                 
   strcpy((pCntlInfo + 0)->pCntlTitle,  "   Message Name :");
   (pCntlInfo + 0)->nAlignment = LEFT;
   (pCntlInfo + 0)->nAlignmentOffset = 20;
   (pCntlInfo + 0)->nCntlRow = 5;
   (pCntlInfo + 0)->nCntlOffset = 18;
   (pCntlInfo + 0)->nCntlLength = 30;
   (pCntlInfo + 0)->pCntlBuffer[0] = 0;

   strcpy((pCntlInfo + 1)->pCntlTitle,  "Text Length :");
   (pCntlInfo + 1)->nAlignment = RIGHT;
   (pCntlInfo + 1)->nAlignmentOffset = 11;
   (pCntlInfo + 1)->nCntlRow = 7;
   (pCntlInfo + 1)->nCntlOffset = 60;
   (pCntlInfo + 1)->nCntlLength = 3;
   (pCntlInfo + 1)->pCntlBuffer[0] = 0;

   pMult_CntlInfo->pCntlTitle = "";
   pMult_CntlInfo->nAlignment = LEFT;
   pMult_CntlInfo->nAlignmentOffset = 2;
   pMult_CntlInfo->nCntlRow = 9;
   pMult_CntlInfo->nCntlOffset = 0;
   pMult_CntlInfo->nCntlLength = 72;
   pMult_CntlInfo->nCntlLines = 12;
   pMult_CntlInfo->pMultiBuffer = (char *) calloc(((pMult_CntlInfo)->nCntlLength * (pMult_CntlInfo)->nCntlLines) + 1, sizeof(char));
 
}
//----------------------------------------------------------------
//
// void SetGroupControls(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
void SetGroupControls(PCONTROL_PARAM pCntlInfo)
{
                                 
   strcpy((pCntlInfo + 0)->pCntlTitle, "   Group Title :");
   (pCntlInfo + 0)->nAlignment = LEFT;
   (pCntlInfo + 0)->nAlignmentOffset = 20;
   (pCntlInfo + 0)->nCntlRow = 5;
   (pCntlInfo + 0)->nCntlOffset = 18;
   (pCntlInfo + 0)->nCntlLength = 30;
   (pCntlInfo + 0)->pCntlBuffer[0] = 0;
 
}
