#include <stdio.h>
#include <string.h>
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

#define BITSET(x, n) ( (((unsigned) x >> n) & 0x0001) == 1 ? 1 : 0)
#define peek( addr )        (*(unsigned char __far *)addr)

//--------------------------------------------------------------
   //  Local function declarations
//--------------------------------------------------------------

extern void __cdecl __interrupt __far new_int(void);
extern void (__interrupt __far * old_int) ();
void RestoreInterrupts(void);
long PrinterSetup(void);
void RunFromDosPrompt(int argc, char **argv);
void UpdateModem(void);

//--------------------------------------------------------------
   //  External variables declarations
//--------------------------------------------------------------

extern WAPISPOOLERINFO CurrentPage;
extern PORT            *cport;
extern int             StopProcess;
extern PORT            *cport;
extern LPSTR           lpSendingMessage;

extern int sf;
extern fNetwork;
//--------------------------------------------------------------
   //  Global variables declarations
//--------------------------------------------------------------

extern int                 Redial_Now;
extern int                 FromPrompt;
extern int                 Debuger;
extern int                 TimeOut;
extern int                 OnceFlag;

extern long                TimerT;
extern long                TimerD;
extern long                TimerC;
extern long                TimerL;
extern long                TimerS;
extern long                TimerH;

extern WORD                TimerTalk;
extern WORD                TimerDial;
extern WORD                TimerConnect;
extern WORD                TimerLogon;
extern WORD                TimerSend;
extern WORD                TimerHangUp;

void mouse(int one,int two,int three,int four);
void interrupt far Int09(void);
extern void (interrupt far *OldInt09)(void); 
extern char far *flags;
extern char far *kbdBuffer;

extern BOOL                AltKey;
extern BOOL                MouseDown;
extern BOOL                KeyDown;
extern BOOL                EscapeKey;
extern char                message[80];

extern struct videoconfig  vc;
extern WAPIINFO            WapiInfo;
extern DosBEEPINFO         BeepInfo;
extern MULT_CONTROL_PARAM  Message_CntlInfo;

BOOL Keyboard = TRUE;


//------------------------------------------------------------------------
//
//     DosBEEP function:   void Do_PET(LPWAPISPOOLERINFO lpSpoolInfo)
//
//     decsription:
//
//------------------------------------------------------------------------
void Do_PET(LPWAPISPOOLERINFO lpSpoolInfo)
{
   BOOL            loop = TRUE;
   int             comchar=0;
   char            szBuffer[150];
   int             len = 0;
   short           index = 1;
   short           line = 0;
   int             PrintMessage = FALSE;
   BOOL            ModemExists = TRUE;
   struct _rccoord pos;
   
   pos = _gettextposition();
   line = (short) (pos.row + 1);
   old_int = _dos_getvect( 0x1C );
   _dos_setvect( 0x1C, new_int );

   cport = NULL;

   if ( FromPrompt == FALSE )
   {
       Box( 14, 18, 3, 40, _TBLACK, _TWHITE, BORDER);
       PrintText("Press <ESC> key to cancel the message." , LEFT, 20, 16, _TBLACK, _TWHITE);
   }

   if ( FromPrompt == FALSE )
       StatusBar("INFO: Opening COMM Port.", CENTER, 0, 25, _TBLACK, _TWHITE);

   if ( OpenPort(ProcessComPort(lpSpoolInfo->ModemInfo.byCOMMPort),
        ProcessBaudRate(lpSpoolInfo->ServiceInfo.dwBaud), 
        ProcessParity(lpSpoolInfo->ServiceInfo.byParity), 
        ProcessStopBits(lpSpoolInfo->ServiceInfo.byStopBits),
        lpSpoolInfo->ServiceInfo.byDataBits) != NULL)
   {

       TimerT = 0;
       TimerTalk = TIMER_ON;
       Frame_OnTimer(THREAD_LIFE_TIMER);

       while(loop == TRUE)
       {
           if ( TimerTalk == TIMER_ON )
           {
               if ( TimerT >= 1080 )
               {
                   ModemExists = FALSE;
                   StopProcess = TRUE;
                   TimerT = 0;
                   
                   if ( FromPrompt == FALSE )
                       StatusBar("ERROR: Check the modem.", CENTER, 0, 25, _TBLACK, _TWHITE);
               }
           }
       
           if ( TimerDial == TIMER_ON )
           {
               if ( TimerD >= 1080 )
               {
                   Frame_OnTimer(TIMER_REDIAL);
                   TimerD = 0;
               }

           }

           if ( TimerConnect == TIMER_ON )
           {
               if ( TimerC >= 72 )
               {
                   Frame_OnTimer(TIMER_SERVICE_PET);
                   TimerC = 0;
               }
           }

           if ( TimerLogon == TIMER_ON )
           {
               if ( TimerL >= 1080 )
               {
                   Frame_OnTimer(TIMER_REDIAL);
                   TimerL = 0;
               }
           }

           if ( TimerSend == TIMER_ON )
           {
               if ( TimerS >= 1080 )
               {
                   Frame_OnTimer(TIMER_REDIAL);
                   TimerS = 0;
               }
           }

           if ( StopProcess == TRUE )
               break;

           if ( _kbhit() )
           {
               if ( _getch() == 0x1B )
               {
                   Redial_Now = FALSE;
                   StopProcess = TRUE;

                   if ( FromPrompt == FALSE )
                   {
                       BOOL loop = TRUE;

                       strcpy(message, "Message was canceled.");     
                       Box( 14, 18, 3, 40, _TBLACK, _TWHITE, BORDER);
                       PrintText("Do you want to delete the message ?." , LEFT, 22, 16, _TBLACK, _TWHITE);
                       PrintText("Y" , LEFT, 35, 17, _TWHITE, _TBLACK);
                       PrintText("es or " , LEFT, 36, 17, _TBLACK, _TWHITE);
                       PrintText("N" , LEFT, 42, 17, _TWHITE, _TBLACK);
                       PrintText("o" , LEFT, 43, 17, _TBLACK, _TWHITE);
                       while(loop == TRUE)
                       {
                           if(_kbhit())
                           {
                               int ch;

                               ch = getch();
                               if((char) tolower(ch) == 'n')
                                   break;
                               if((char) tolower(ch) == 'y')
                               {
                                   if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
                                   {
                                       if(Spooler_DeleteRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS)
                                           StatusBar("Canceled message was deleted.", CENTER, 0, 25, _TBLACK, _TWHITE);

		                                Close_DBFile(&WapiInfo, SPOOLER_DB);
                                   }
			        
                                   break;
                               }
                           }
                       }
                   }

                   strcpy(szBuffer, "Abandoned : ");
                   strncat(szBuffer, CurrentPage.SubscriberInfo.szSubscriberName, 15);
                   strcat(szBuffer, " : ");
                   strncat(szBuffer, CurrentPage.SubscriberInfo.szPIN, 10);
                   strcat(szBuffer, " : ");
                   if((len = strlen(lpSendingMessage)) < 100)
                       len -= 4;
                   strncat(szBuffer, lpSendingMessage, len);

                   Pet_DeleteMessage();
                   lpSendingMessage = NULL;

                   if((WapiInfo.dwOptions & OPT_LOG_TRANSACTIONS) || (WapiInfo.dwOptions & OPT_LOG_ERRORS))
				        WriteToLogFile(szBuffer);
               }
           }

           if((comchar = ReadCommChar()) > 0)
           {
               if ( Debuger == TRUE )
               {
                   if ( (comchar < 32) || (comchar > 127) )
                   {
                       comchar = (char) toupper((int) comchar);
                       sprintf(message, " <%02X> ", (int) comchar);
                       _settextposition(line, index);
                       _outtext((LPCSTR) message);
                       index += 5;
                   }
                   else
                   {
                       sprintf(message, "%c", comchar);
                       _settextposition(line, index);
                       _outtext((LPCSTR) message);
                   }
 
                   if(index >= vc.numtextcols)
                   {
                       line++;
                       index = 1;
                   }
                   else
                       index++;
               }
   
               // ----------------------------
               //  if the connection is to be terminated, do it before the 
               //  machine could somehow hang up.

                   if ( StopProcess == TRUE )
                       break;
        
                   if ( _kbhit() )
                   {
                       if ( _getch() == 0x1B ) 
                       {                                 
                           Redial_Now = FALSE;
                           StopProcess = TRUE;

                           if ( FromPrompt == FALSE )
                           {
                               BOOL loop = TRUE;

                               strcpy(message, "Message was canceled.");     
                               Box( 14, 18, 3, 40, _TBLACK, _TWHITE, BORDER);
                               PrintText("Do you want to delete the message ?." , LEFT, 22, 16, _TBLACK, _TWHITE);
                               PrintText("Y" , LEFT, 35, 17, _TWHITE, _TBLACK);
                               PrintText("es or " , LEFT, 36, 17, _TBLACK, _TWHITE);
                               PrintText("N" , LEFT, 42, 17, _TWHITE, _TBLACK);
                               PrintText("o" , LEFT, 43, 17, _TBLACK, _TWHITE);
                               while(loop == TRUE)
                               {
                                   if(_kbhit())
                                   {
                                       int ch;

                                       ch = getch();
                                       if((char) tolower(ch) == 'n')
                                           break;
                                       if((char) tolower(ch) == 'y')
                                       {
                                           if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
                                           {
                                               if(Spooler_DeleteRec(&WapiInfo, SPOOLER_TYPE1, 0, (PVOID) &CurrentPage) == CALL_SUCCESS)
                                                   StatusBar("Canceled message was deleted.", CENTER, 0, 25, _TBLACK, _TWHITE);

		                                        Close_DBFile(&WapiInfo, SPOOLER_DB);
                                           }
			        
                                           break;
                                       }
                                   }
                               }
                           }

                           strcpy(szBuffer, "Abandoned : ");
                           strncat(szBuffer, CurrentPage.SubscriberInfo.szSubscriberName, 15);
                           strcat(szBuffer, " : ");
                           strncat(szBuffer, CurrentPage.SubscriberInfo.szPIN, 10);
                           strcat(szBuffer, " : ");
                           if((len = strlen(lpSendingMessage)) < 100)
                               len -= 4;
                           strncat(szBuffer, lpSendingMessage, len);

                           Pet_DeleteMessage();
                           lpSendingMessage = NULL;

                           if((WapiInfo.dwOptions & OPT_LOG_TRANSACTIONS) || (WapiInfo.dwOptions & OPT_LOG_ERRORS))
				                WriteToLogFile(szBuffer);
                       }
                   }

               //-----------------------------

               State_Verification((LPBYTE) &comchar);
           }
       }

       // reset all the timers, so that they will not accidentally kick in

       TimerTalk = TIMER_OFF;
       TimerDial = TIMER_OFF;
       TimerConnect = TIMER_OFF;
       TimerLogon = TIMER_OFF;
       TimerSend = TIMER_OFF;
       TimerHangUp = TIMER_OFF;
       StopProcess = FALSE;
 
       // if modem is not pluged in, do not talk to it. Saves hangup time.

       if ( ModemExists == TRUE )
       {
            //if ( FromPrompt == FALSE )
            //   StatusBar("Hanging Up...", CENTER, 0, 25, _TBLACK, _TWHITE);

            TimerT = 0;  
            while(( TimerT < 20 ) && ( TimerConnect == TIMER_OFF ));
                WriteCommString("~~~", NOT_A_MESSAGE);
            TimerT = 0;  
            while( TimerT < 20 );
                WriteCommString("+++", NOT_A_MESSAGE);
            TimerT = 0;  
            while( TimerT < 40 );
                WriteCommString("ATH0\r", NOT_A_MESSAGE);
            TimerT = 0;  
            while( TimerT < 20 );
       }

       ClosePort();
       TimerT = 0;
       while( TimerT < 40 );

       //if ( FromPrompt == FALSE )
       //    StatusBar("INFO: Comm port closed.", CENTER, 0, 25, _TBLACK, _TWHITE);
   }
   else
       if ( FromPrompt == FALSE )
           StatusBar("ERROR: Could not open the comm port.", CENTER, 0, 25, _TBLACK, _TWHITE);

   _dos_setvect( 0x1C, old_int );

   _displaycursor( _GCURSOROFF );
}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void __cdecl __interrupt __far new_int(void)
//
//     description:        
//
//------------------------------------------------------------------------
void __cdecl __interrupt __far new_int(void)
{
   TimerT++;
   TimerC++;
   TimerD++;
   TimerL++;
   TimerS++;
   TimerH++;
   
   _chain_intr( old_int );
}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void RestoreInterrupts(void)
//
//     description:        
//
//------------------------------------------------------------------------
void RestoreInterrupts(void)
{
   _dos_setvect( 0x1C, old_int );
}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void AboutBox(void)
//
//     description:        
//
//------------------------------------------------------------------------
void AboutBox(void)
{
   int loop = 1;
   int fl = 0;
   _displaycursor( _GCURSOROFF );
 	DrawMainScreen();
   Box( 5, 5, 12, vc.numtextcols - 11, _TBLACK, _TWHITE, BORDER);

   if(fNetwork)
       PrintText("Welcome to DosBEEP v. 2.00.07 Network", CENTER, 0, 6, _TBLACK, _TWHITE);
   else
       PrintText("Welcome to DosBEEP v. 2.00.07 Standalone", CENTER, 0, 6, _TBLACK, _TWHITE);

   PrintText("Copyright (c) 1994", CENTER, 0, 8, _TBLACK, _TWHITE);
   PrintText("Created by", CENTER, 0, 9, _TBLACK, _TWHITE);
   PrintText("Fourth Wave Technologies, Inc.", CENTER, 0, 10, _TBLACK, _TWHITE);
   PrintText("560 Kirts Blvd. Suite #105", CENTER, 0, 11, _TBLACK, _TWHITE);
   PrintText("Troy, MI 48084", CENTER, 0, 12, _TBLACK, _TWHITE);
   PrintText("For technical support call", CENTER, 0, 14, _TBLACK, _TWHITE);
   PrintText("(810)-362-2288", CENTER, 0, 15, _TBLACK, _TWHITE);
   PrintText("(810)-362-2295 fax only", CENTER, 0, 16, _TBLACK, _TWHITE);

   StatusBar("Press any key to continue...", CENTER, 0, 25, _TBLACK, _TWHITE);
   
   mouse(3,0,0,0);
   while(two != 0)
      mouse(3,0,0,0);

   while(loop == 1)
   {
       if(_kbhit())
       {
           getch();
           break;
       }

       mouse(3,0,0,0);
       if(two != 0)
         break;
   }
}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void PrinterSetup(void)
//
//     description:        
//
//------------------------------------------------------------------------
long PrinterSetup(void)
{
    long     offset = NOT_SELECTED;
    char    szBuffer[256];
    int     iCur = 0;
    ITEM    SubList2[]=
    { 
       {   0, "NONE"       },
       {   7, "DEV LPT1"   },
       {   7, "DEV LPT2"   },
       {   7, "DEV LPT3"   },
       {   0, ""           }
    };

    switch(BeepInfo.nPrinterPort)
    {
       case PRINTER_PORT_1:
           iCur = 1;
           strcpy(message, "Default Printer is on LPT1.");
       break;
       case PRINTER_PORT_2:
           iCur = 2;
           strcpy(message, "Default Printer is on LPT2.");
       break;
       case PRINTER_PORT_3:
           iCur = 3;
           strcpy(message, "Default Printer is on LPT3.");
       break;
       case PRINTER_PORT_NONE:
       default:
           BeepInfo.nPrinterPort = 0;
           iCur = 0;
           strcpy(message, "Default Printer is not available.");
       break;
    }

    StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
    
    offset = Menu( 5, 26, SubList2, iCur, 11);
    
    if(offset == ALT)
       return ALT;
    
    if ( offset >= 0 )
    {
       switch(offset)
       {
          case 1:
              BeepInfo.nPrinterPort = PRINTER_PORT_1;
          break;
          case 2:
              BeepInfo.nPrinterPort = PRINTER_PORT_2;
          break;
          case 3:
              BeepInfo.nPrinterPort = PRINTER_PORT_3;
          break;
          case 0:
          default:
              BeepInfo.nPrinterPort = PRINTER_PORT_NONE;
          break;
       }

       sprintf(message, "%d", offset);

       if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
           strcpy(szBuffer, "WAPI.INI");

       WriteProfileString("DosBEEP_Setup", "PrinterDev", message, szBuffer);

       switch(BeepInfo.nPrinterPort)
       {
           case PRINTER_PORT_1:
               iCur = 1;
               strcpy(message, "Default Printer is on LPT1.");
           break;
           case PRINTER_PORT_2:
               iCur = 2;
               strcpy(message, "Default Printer is on LPT2.");
           break;
           case PRINTER_PORT_3:
               iCur = 3;
               strcpy(message, "Default Printer is on LPT3.");
           break;
           case PRINTER_PORT_NONE:
           default:
               BeepInfo.nPrinterPort = 0;
               iCur = 0;
               strcpy(message, "Default Printer is not available.");
           break;
       }

       StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
    }
    return offset;
}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void interrupt far Int09(void)
//
//     description:        keyboard replacement for Alt key
//
//------------------------------------------------------------------------
void interrupt far Int09(void)
{
   _disable();

   OldInt09();

   if(Keyboard)
   {
       if( peek( 0x00400096 ) & 0x10 )
       {
           if( ((unsigned) *flags >> 3) & 0x0001 )
               AltKey = TRUE;
           else
               AltKey = FALSE;
       }
       else
           if( ( ((unsigned) *flags >> 3) & 0x0001 ) || ( ((unsigned) *flags >> 1) & 0x0001 ) )
               AltKey = TRUE;
           else
               AltKey = FALSE;
   }

}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void DrawMainScreen(void)
//
//     description:        
//
//------------------------------------------------------------------------
void DrawMainScreen(void)
{
   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, vc.numtextcols - 2, _TWHITE, _TBLUE, BORDER);
   PrintText(" DosBEEP ", CENTER, 0, 2, _TBLUE, _TWHITE);
   StatusBar("", LEFT, 0, 25, _TWHITE, _TWHITE);
}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void RunFromDosPrompt(int argc, char **argv)
//
//     description:        Runs Dosbeep from the prompt
//
//------------------------------------------------------------------------
void RunFromDosPrompt(int argc, char **argv)
{
   struct _rccoord textpos;

   if ( (argc > 1) && ((argc == 2) || (argc == 3) || (argc == 4)))
   {
       if ( argc == 2 )
       {
           textpos = _gettextposition();

           if(fNetwork)
               printf("[DosBEEP v. 2.00.07 Network]\n");
           else
               printf("[DosBEEP v. 2.00.07 Standalone]\n");

           printf("[Copyright (c) 1994 by Fourth Wave Technologies, Inc.]\n");
           printf("Usage:\n");
           printf("   dosbeep /[d,?] [\"subscriber\" \"message\"]\n\n");
           printf("Description:\n");
           printf(" dosbeep - Loads the DosBEEP Screen.\n");
           printf(" /? - Help on DosBEEP.\n");
           printf(" /d - Trace connection when paging from DOS prompt.\n");
           printf(" \"subscriber\" - Any valid subscriber name.\n");
           printf(" \"message\" - Any combination of characters.\n");

           _settextposition((short) (textpos.row + 11), 0);
           _displaycursor( _GCURSORON );
           exit(0);
       }


       if ( argc == 4 )
       {
           if( (strcmp(*(argv + 1), "/d") == 0) ||
               (strcmp(*(argv + 1), "/D") == 0) )
           {
               Debuger = TRUE;
           }
           else
           {
               printf("Invalid option specified.\n");
               
               _settextposition((short) (textpos.row + 10), 0);
               _displaycursor( _GCURSORON );
               exit(0);
           }
       }

       if( Open_DBFile(&WapiInfo, SPOOLER_DB, WapiInfo.szSpoolDir) == CALL_SUCCESS )
       {
           CONTROL_PARAM CntlInfo;
           int     doPet = FALSE;
           char    szLastFile[128];
           char    name[50];
           HWAPIPAGEID wapiID;

           FromPrompt = TRUE;

		    Open_DBFile(&WapiInfo, SUBSCRIBER_DB, WapiInfo.szPhoneBookDir);
		    Open_DBFile(&WapiInfo, SERVICE_DB, WapiInfo.szPhoneBookDir);
		    Open_DBFile(&WapiInfo, MODEM_DB, WapiInfo.szDataDir);


           if ( argc == 4 )
		        strcpy(name, *(argv + 2));
           else
		        strcpy(name, *(argv + 1));

           wapiID = CreateUniqueFileName(CurrentPage.szFileName, WapiInfo.szSpoolDir, SPL);
           strcpy(szLastFile, CurrentPage.szFileName);
           memset(&CntlInfo, 0, sizeof(CntlInfo));

           if ( argc == 4 )
               strcpy(Message_CntlInfo.pMultiBuffer, *(argv + 3));
           else
               strcpy(Message_CntlInfo.pMultiBuffer, *(argv + 2));

           if(InsertMessage(name, &CntlInfo, szLastFile, wapiID) == FALSE)
               printf("Subscriber \"%s\" was not found.\n", CurrentPage.SubscriberInfo.szSubscriberName);
           else
               doPet = TRUE;

		    Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
		    Close_DBFile(&WapiInfo, SERVICE_DB);
		    Close_DBFile(&WapiInfo, MODEM_DB);
		    Close_DBFile(&WapiInfo, SPOOLER_DB);
		 

		  if ( doPet == TRUE )
         {
            short Redials;
        
            Initialize_State_Table();
            Redial_Now = TRUE;
            StopProcess = FALSE;

            for(Redials = 0; (Redial_Now == TRUE) && (Redials < CurrentPage.ModemInfo.ModemInfo.byReDial_Count); Redials++)
               Do_PET(&CurrentPage);
         }
    
         _displaycursor( _GCURSORON );
         if((WapiInfo.dwOptions & OPT_LOG_TRANSACTIONS) || (WapiInfo.dwOptions & OPT_LOG_ERRORS))
             WriteToLogFile("DosBEEP 2.0 Shutdown");
         exit(0);
      }
   }
}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void LoadHelpFile(ITEM *, int)
//
//     description:        
//
//------------------------------------------------------------------------
void LoadHelpFile()
{
   FILE    *fhHelpFile;
   ITEM    *pLineList = NULL;
   int     index = 0;
   char    szBuffer[256];
   int     entries = 0;
   int     counter = 0;
   char    szBuffer2[256];

   // 50 is a temporary value

   if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
       strcpy(szBuffer, "WAPI.INI");

   if(GetProfileString("Directories", "BinFiles", NULL, szBuffer2, sizeof(szBuffer2), szBuffer) == FALSE)
       strcpy(szBuffer, "DOSBEEP.HLP");
   else
       sprintf(szBuffer, "%s%s", szBuffer2, "DOSBEEP.HLP");

   if((fhHelpFile = fopen(szBuffer, "r")) != NULL)
   {
       counter = 500;

       if(((pLineList = (ITEM *) malloc( counter * sizeof(ITEM) )) != NULL) && (fhHelpFile != NULL))
       {
           fseek(fhHelpFile, 0, SEEK_SET);
           while((fgets( szBuffer, 80, fhHelpFile) != NULL) && (index <= counter))
           {
               pLineList[index].iHilite = -1;
               szBuffer[strlen(szBuffer) - 1] = 0;
               strncpy((LPSTR) pLineList[index].achItem, (LPCSTR) szBuffer, 80);
               index++;
           }

           fclose(fhHelpFile);

           if(index > 19)
               entries = 19;
           else
               entries = index;
       
           StatusBar("Hit Enter <CR> or Esc key to return to menu.", CENTER, 0, 25, _TBLACK, _TWHITE);
           _setbkcolor( _TWHITE );
           _settextcolor( _TBLACK );
           ListBox( 3, 2, 73, entries, pLineList, index, 0, NO_HILIGHT, 0);

           free(pLineList);
       }
       else
           fclose(fhHelpFile);
   }
}
BOOL KeyAlt()
{
   if( peek( 0x00400096 ) & 0x10 )
   {
       if( ((unsigned) *flags >> 3) & 0x0001 )
           return TRUE;
   }
   else
       if( ( ((unsigned) *flags >> 3) & 0x0001 ) || ( ((unsigned) *flags >> 1) & 0x0001 ) )
           return TRUE;

   return FALSE;
}
