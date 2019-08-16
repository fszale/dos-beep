#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <share.h>
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
#include "state.h"

extern struct videoconfig vc;
extern WAPIINFO            WapiInfo;
extern WAPISPOOLERINFO     CurrentPage;
extern char                message[80];
extern DosBEEPINFO         BeepInfo;
extern int                 StopProcess;
extern int                 sf;
extern BOOL                KeyDown;
extern int                 Redial_Now;
extern int                 FromPrompt;
extern BOOL                AltKey;
extern ITEM mnuHelp[];

extern int                 doPet;
extern MULT_CONTROL_PARAM  Message_CntlInfo;

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ProcessSendEdit(PMULT_CONTROL_PARAM pMult_CntlInfo, PCONTROL_PARAM pCntlInfo)
{
   WAPISUBSCRIBERINFO  SubInfo;
   char szBuffer[256];
   char szBuffer2[256];
   int  loop = 1;
   int  smloop = 1;
   int  next = 0;
   int  retVal;
   int  count = 0;
   int  ret;

   StatusBar("Press <TAB> key to advance to another entry...", CENTER, 0, 25, _TBLACK, _TWHITE);

   _displaycursor( _GCURSOROFF );

   if(WapiInfo.dwOptions & FLG_LENGTH_COUNT)
   {
       if(WapiInfo.dwOptions & FLG_SIGNATURE_ENABLE)
       {
           if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == TRUE)
               GetProfileString("Signature", "DefaultSignature", NULL, szBuffer2, sizeof(szBuffer2), szBuffer);

           count += strlen(szBuffer2) + 1;
       }

       if(WapiInfo.dwOptions & FLG_TIMESTAMP_ENABLED)
           count += 12;
   }
   else
       count = 0;

   while(loop == 1)
   {
       if ( next == 0 )
       {
           while(smloop == 1)
           {
               retVal = ProcessControl(pCntlInfo, _TBRIGHTWHITE, _TBLUE, PROCESS);
           
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 1;
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 2;
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
                   case LIST:
		                if((ret = ListSubscribers(&SubInfo, 3, 45, 30, 8)) >= 0)
		                    strcpy((LPSTR) (pCntlInfo + 0)->pCntlBuffer, (LPCSTR) SubInfo.szSubscriberName);   
                       else
                       if(ret == ALT)
                       {
                           DrawSendBox();
                           DrawControls(pCntlInfo, 3);
                           DrawMultControls(pMult_CntlInfo, 1);
		                    ReDrawData(pCntlInfo, 3, _TBRIGHTWHITE, _TBLUE);
		                    ReDrawMultilineData(pMult_CntlInfo, 1, 12);
                           return ALT;
                       }
                       DrawSendBox();
                       DrawControls(pCntlInfo, 3);
                       DrawMultControls(pMult_CntlInfo, 1);
		                ReDrawData(pCntlInfo, 3, _TBRIGHTWHITE, _TBLUE);
		                ReDrawMultilineData(pMult_CntlInfo, 1, 12);
                   break;
                }
           }
           smloop = 1;
       }

       if ( next == 1 )
       {
           while(smloop == 1)
           {
               retVal = ProcessControl((pCntlInfo + 2), _TBRIGHTWHITE, _TBLUE, PROCESS);
           
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 2;
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 0;
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
                }
           }
           smloop = 1;
       }

       if ( next == 2 )
       {
           while(smloop == 1)
           {
               retVal = ProcessMultiLineControl(pMult_CntlInfo, pMult_CntlInfo->nCntlLines, (pCntlInfo + 1), _TBRIGHTWHITE, _TBLUE, count);
           
               if(CurrMenu >= 0)
                   return CurrMenu;

               switch(retVal)
               {
                   case ALT:
                       return ALT;

                   case ENTER:
                   case TAB:
                       next = 0;
                       smloop = 0;
                   break;
                   case SHIFT_TAB:
                       next = 1;
                       smloop = 0;
                   break;
                   case ESCAPE:
                       return ESCAPE;
                   break;
               }
           }
           smloop = 1;
       }
   }
   return ESCAPE;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawSendBox()
{
   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, vc.numtextcols - 2, _TWHITE, _TBLUE, BORDER);
   StatusBar("", LEFT, 0, 25, _TWHITE, _TWHITE);
   Box( 10, 3, 12, vc.numtextcols - 7, _TWHITE, _TBLUE, BORDER);

   _displaycursor( _GCURSOROFF );
   PrintText(" Send Edit ", CENTER, 0, 2, _TBLUE, _TWHITE);
   PrintText("Message Text:", LEFT, 4, 9, _TWHITE, _TBLUE);
}


//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
BOOL InsertMessage(LPSTR szSubscriber, PCONTROL_PARAM pFileInfo, LPSTR LastFile, HWAPIPAGEID wapiID)
{
   int pr = 0;
   long filelength = 0;
   int  fhAttach;
   char *BeepMessage;

   memset(&CurrentPage, 0, sizeof(WAPISPOOLERINFO));

   strcpy(CurrentPage.SubscriberInfo.szSubscriberName, szSubscriber);
   CurrentPage.wapiPageID = wapiID;

   if ( Spooler_Fill(&WapiInfo, 0, &CurrentPage) == CALL_SUCCESS )
   {
       strcpy(CurrentPage.szFileName, LastFile);

       if(strcmp(pFileInfo->pCntlBuffer, "") != 0)
       {
           if((fhAttach = _open(pFileInfo->pCntlBuffer, _O_RDONLY)) != -1)
           {
               filelength = _filelength(fhAttach);
               if(filelength > 5000)
                   filelength = 5000;
               else
               if(filelength == 0)
                   close(fhAttach);
           }
           else
           {
               int ret = errno;

               if ( FromPrompt == FALSE )
                   StatusBar("ERROR : Could not access attached file.", CENTER, 0, 25, _TBLACK, _TWHITE);
               filelength = 0;
           }
       }
       else
           filelength = 0;

       if((BeepMessage = (char *) malloc( (int) ((strlen(Message_CntlInfo.pMultiBuffer)) + 256 + filelength))) != NULL)
       {
           char szBuffer[128];

           strcpy(BeepMessage, "");

           if(WapiInfo.dwOptions & FLG_TIMESTAMP_ENABLED)
           {            
               char szTime[15];
               int hours = 0;

               _strtime(szTime);
               strncpy(szBuffer, szTime, 2);
               if((hours = atoi(szBuffer)) > 12)
               {   
                   hours -=12;
                   sprintf(szBuffer, "%-02.2d%-6.6s PM ", hours, &szTime[2]);
               }
               else 
                   sprintf(szBuffer, "%-8s AM ", szTime);

               strcat(BeepMessage, szBuffer);
           }

           if(WapiInfo.dwOptions & FLG_SIGNATURE_ENABLE)
           {
               strcat(BeepMessage, BeepInfo.szDefaultSignature);
               strcat(BeepMessage, " ");
           }

           strcat(BeepMessage, Message_CntlInfo.pMultiBuffer);
           
           // attach file read
           
           if(filelength > 0)
           {
               char    *buff;
               long length = 0;

               strcat(BeepMessage, " [File Text Attached]  ");
               if((buff = (char *) malloc( (int) ((filelength + 50) * sizeof(char)))) != NULL)
               {
                   length = _read(fhAttach, buff, (int) filelength);
                   if(length >= 0)
                       buff[length] = 0;
                   strcat( BeepMessage, buff);
                   free(buff);
               }

               close(fhAttach);
           }   
           // end of attach file processing

           strcpy(CurrentPage.szWAPILogon, "DosBEEP 2.04.00");

           //if ( FromPrompt == FALSE )
               //StatusBar("Inserting a message", CENTER, 0, 25, _TBLACK, _TWHITE);

           if ( (pr = Spooler_InsertRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage, BeepMessage)) == CALL_SUCCESS)
           {
               if ( CurrentPage.ModemInfo.byCOMMType == COMM_LOCAL )
                   doPet = TRUE;
           }
           else
           {
               strcpy(message, "ERROR: Could not add this page.");
               Beep(698, 700);
           }

           free(BeepMessage);
       }
       else
       {
           if(filelength > 0)
               close(fhAttach);

           strcpy(message, "ERROR: Out of memory.");
           Beep(698, 700);
           return FALSE;
       }

       return TRUE;
   }

   return FALSE;
}

//---------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------
void ViewQueue(void)
{
   ITEM    *pSubList = NULL;
   int     index = 0, Overflow = 0;
   int     Height;

   DrawSendBox();

   if((pSubList = (ITEM *) malloc( 400 * sizeof(ITEM) )) == NULL)
      return;
 
   if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
	{
       char buffer[50];

       if ( Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) != CALL_SUCCESS)
       {
           strcpy(message, "No messages were found.");
           StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);

           free(pSubList);
           Close_DBFile(&WapiInfo, SPOOLER_DB);
           return;
       }

       StatusBar("Loading queued messages...", CENTER, 0, 25, _TBLACK, _TWHITE);
       if ( Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS)
       {
           do
           {
               pSubList[index].iHilite = -1;
               memset(pSubList[index].achItem, 0, sizeof(pSubList[index].achItem));

               sprintf(pSubList[index].achItem, "%-20.20s %-12.12s %-24.24s %-14.14s",
                           CurrentPage.SubscriberInfo.szSubscriberName, 
                           CurrentPage.SubscriberInfo.szPIN,
                           CurrentPage.ServiceInfo.szServiceName,
                           CurrentPage.ServiceInfo.szPhoneNumber);

               index++;
               Overflow++;
           }
           while((Spooler_GetnextRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS) && (Overflow < 400));
       }                                 
      
       StatusBar("Hit Enter <CR> to return to menu.", LEFT, 35, 25, _TBLACK, _TWHITE);

       if( index > 17 )
           Height = 17;
       else
           Height = index;
         
       Close_DBFile(&WapiInfo, SPOOLER_DB);
         
       Box( 3, 2, 2, vc.numtextcols - 4, _TBLACK, _TWHITE, BORDER);

       PrintText(" Subscriber            ", LEFT, 3, 4, _TBRIGHTWHITE, _TBLACK);
       PrintText("Pin                    ", LEFT, 25, 4, _TBRIGHTWHITE, _TBLACK);
       PrintText("Service                ", LEFT, 38, 4, _TBRIGHTWHITE, _TBLACK);
       PrintText("Phone Number      ", LEFT, 61, 4, _TBRIGHTWHITE, _TBLACK);

       sprintf(buffer, " Total queued pages : %d ", index);
       PrintText(buffer, LEFT, 1, 25, _TBRIGHTWHITE, _TBLACK);

       ListBox( 5, 2, 74, Height, pSubList, index, 0, NO_HILIGHT, NULL);
       StatusBar("", LEFT, 0, 25, _TBLACK, _TWHITE);
   }
 
   free(pSubList);
}

//--------------------------------------------------------------
//
//
//
//--------------------------------------------------------------
void AddToQueue(PCONTROL_PARAM pCntlInfo)
{
   BOOL Group = FALSE;
   WAPIGROUPINFO       GroupInfo;
   WAPISUBSCRIBERINFO  SubInfo;
   WAPIMESSAGEINFO     MessageInfo;
   
	if(strcmp(pCntlInfo->pCntlBuffer, "") != 0)
	{
	    if((strcmp(Message_CntlInfo.pMultiBuffer, "") != 0) || (strcmp((pCntlInfo + 2)->pCntlBuffer, "") != 0))
	    {
	        if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
	        {
	            Open_DBFile(&WapiInfo, GROUP_DB, WapiInfo.szPhoneBookDir);
	            Open_DBFile(&WapiInfo, SUBSCRIBER_DB, WapiInfo.szPhoneBookDir);
	            Open_DBFile(&WapiInfo, SERVICE_DB, WapiInfo.szPhoneBookDir);
	            Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir);
                  
               strcpy(SubInfo.szSubscriberName, pCntlInfo->pCntlBuffer);
               
               if(WapiInfo.dwOptions & FLG_MSG_RECALL_ENABLED)
               {
                   if( Open_DBFile(&WapiInfo, MESSAGE_DB, WapiInfo.szPredefinedMsgs) == CALL_SUCCESS )
                   {
                       strcpy(MessageInfo.szTitle, "*LAST MESSAGE");

                       if(Message_GetanyRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo) != CALL_SUCCESS)
                           Message_InsertRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo, Message_CntlInfo.pMultiBuffer);
                       else
                           if(Message_DeleteRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo) == CALL_SUCCESS)
                               Message_InsertRec(&WapiInfo, MESSAGE_TYPE1, 0, (PVOID) &MessageInfo, Message_CntlInfo.pMultiBuffer);

                       Close_DBFile(&WapiInfo, MESSAGE_DB);
                   }
               }
              
               if(Subscriber_GetanyRec(&WapiInfo, SUB_TYPE1, 0, (PVOID) &SubInfo) == CALL_SUCCESS)
               {
                   char    szLastFile[128];
                   HWAPIPAGEID wapiID;

                   StatusBar("Adding a message to the queue.", CENTER, 0, 25, _TBLACK, _TWHITE);
                   wapiID = CreateUniqueFileName(CurrentPage.szFileName, WapiInfo.szSpoolDir, SPL);
                   strcpy(szLastFile, CurrentPage.szFileName);
                   if(InsertMessage((pCntlInfo + 0)->pCntlBuffer, (pCntlInfo + 2), szLastFile, wapiID) == TRUE)
                       strcpy(message, "1 message was added to the queue.");   
               }
               else
               {
                   Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo);
                   do
                   {
                       if(strcmp(GroupInfo.szGroupName, (pCntlInfo + 0)->pCntlBuffer) == 0)
                       {
                           char    szLastFile[128];
                           HWAPIPAGEID wapiID;
                           int     index = 1;

                           StatusBar("Adding messages to the queue.", CENTER, 0, 25, _TBLACK, _TWHITE);
                           wapiID = CreateUniqueFileName(CurrentPage.szFileName, WapiInfo.szSpoolDir, SPL);
                           strcpy(szLastFile, CurrentPage.szFileName);
                           InsertMessage(GroupInfo.szSubscriberName, (pCntlInfo + 2), szLastFile, wapiID);

                           while((Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS) &&
                               (strcmp((pCntlInfo + 0)->pCntlBuffer, GroupInfo.szGroupName) == 0))
                           {
                               if(InsertMessage(GroupInfo.szSubscriberName, (pCntlInfo + 2), szLastFile, CreateUniqueFileName(CurrentPage.szFileName, WapiInfo.szSpoolDir, SPL)) == TRUE)
                                   index++;
                           }

                           sprintf(message, "%d messages, were added to the queue.", index);
                           break;
                       }
                   }
                   while(Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS);
               }
                            
              
	            Close_DBFile(&WapiInfo, MODEM_DB);
	            Close_DBFile(&WapiInfo, SERVICE_DB);
	            Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
	            Close_DBFile(&WapiInfo, GROUP_DB);
	            Close_DBFile(&WapiInfo, SPOOLER_DB);
	        }
       }
       else
       {
           strcpy(message, "ERROR: Message text is missing.");   
           Beep(698, 700);
       }
	}
	else
   {
       strcpy(message, "ERROR: Subscriber name is missing.");   
       Beep(698, 700);
   }
}

//--------------------------------------------------------------
//
//
//
//--------------------------------------------------------------
void RemoveFromQueue(void)
{
   ITEM    *pSubList = NULL;
   int     index = 0;
   int     Height, ret;
   SPLFILE *filename;
   BOOL    loop = TRUE;

   DrawSendBox();

   if((pSubList = (ITEM *) malloc( 200 * sizeof(ITEM) )) == NULL)
      return;

   if((filename = (SPLFILE *) malloc( 200 * sizeof(SPLFILE) )) == NULL)
       return;

   if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
	{
       if ( Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) != CALL_SUCCESS)
       {
           strcpy(message, "No messages were found.");
           StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);

           free(pSubList);
           free(filename);
           Close_DBFile(&WapiInfo, SPOOLER_DB);
           return;
       }

       while(loop == TRUE)
       {
           DrawSendBox();
           Box( 3, 2, 2, vc.numtextcols - 4, _TBLACK, _TWHITE, BORDER);

           PrintText(" Subscriber            ", LEFT, 3, 4, _TBRIGHTWHITE, _TBLACK);
           PrintText("Pin                    ", LEFT, 25, 4, _TBRIGHTWHITE, _TBLACK);
           PrintText("Service                ", LEFT, 38, 4, _TBRIGHTWHITE, _TBLACK);
           PrintText("Phone Number      ", LEFT, 61, 4, _TBRIGHTWHITE, _TBLACK);

           StatusBar("Loading queued messages...", CENTER, 0, 25, _TBLACK, _TWHITE);
           if ( Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS)
           {
               do
               {
                   pSubList[index].iHilite = -1;
                   memset(pSubList[index].achItem, 0, sizeof(pSubList[index].achItem));

                   sprintf(pSubList[index].achItem, "%-20.20s %-12.12s %-24.24s %-14.14s",
                           CurrentPage.SubscriberInfo.szSubscriberName, 
                           CurrentPage.SubscriberInfo.szPIN,
                           CurrentPage.ServiceInfo.szServiceName,
                           CurrentPage.ServiceInfo.szPhoneNumber);

                   strcpy(filename[index].filename, CurrentPage.szFileName);
                   index++;
               }
               while( Spooler_GetnextRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS);
           }                                       
       
           if( index > 17 )
               Height = 17;
           else
               Height = index;
       
           StatusBar("Hit Escape key to return to menu.", CENTER, 0, 25, _TBLACK, _TWHITE);
         
           if((ret = ListBox( 5, 2, 74, Height, pSubList, index, 0, HILIGHT, NULL)) >= 0)
           {
               char CurrentFileName[15];

               if(ret == ALT)
                   break;

               strcpy(CurrentFileName, filename[ret].filename);

               StatusBar("Deleting message from queue...", CENTER, 0, 25, _TBLACK, _TWHITE);
               if ( Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS)
               {
                   do
                   {
                       if(strcmp(CurrentPage.szFileName, CurrentFileName) == 0)
                       {
                           if(Spooler_DeleteRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS)
                               strcpy(message, "Message successfully deleted.");
                           else
                           {
                               strcpy(message, "ERROR : Message could not be deleted.");
                               Beep(698, 700);
                           }

                           break;
                       }
                   }
                   while( Spooler_GetnextRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS);
               }

               index = 0;
           }
           else
               break;
       }

       Close_DBFile(&WapiInfo, SPOOLER_DB);
   }
 
   free(pSubList);
   free(filename);
}

//--------------------------------------------------------------
//
//
//
//--------------------------------------------------------------
void ClearQueue(void)
{
   StatusBar("Clearing queue...", CENTER, 0, 25, _TBLACK, _TWHITE);
   if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
	{
       if ( Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) != CALL_SUCCESS)
       {
           strcpy(message, "No messages were found.");
           StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);

           Close_DBFile(&WapiInfo, SPOOLER_DB);
           return;
       }

       while ( Spooler_GetfirstRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS)
               Spooler_DeleteRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage);
   
       strcpy(message, "Queue cleared. All messages have been deleted");    
       Close_DBFile(&WapiInfo, SPOOLER_DB);
   }

}

