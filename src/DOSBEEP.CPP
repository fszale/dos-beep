//--------------------------------------------------------------------
//
//     Very Important !!!
//
//     In order for DosBeep tp work properly, compile dosbeep.cpp
//     module without any optimization, in other word no /Ox, etc options.
//
//
//
//--------------------------------------------------------------------
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



void setcntl();


#define BITSET(x, n) ( (((unsigned) x >> n) & 0x0001) == 1 ? 1 : 0)

//--------------------------------------------------------------
   //  Local function declarations
//--------------------------------------------------------------

void __cdecl __interrupt __far new_int(void);
void (__interrupt __far * old_int) ();
void RestoreInterrupts(void);
long PrinterSetup(void);
void RunFromDosPrompt(int argc, char **argv);
void UpdateModem(void);

//--------------------------------------------------------------
   //  External variables declarations
//--------------------------------------------------------------

extern WAPISPOOLERINFO CurrentPage;
extern PORT            *cport;
extern int             StopProcess = FALSE;
extern PORT            *cport;
extern LPSTR           lpSendingMessage;

extern int sf;
extern fNetwork;
//--------------------------------------------------------------
   //  Global variables declarations
//--------------------------------------------------------------

int                     Redial_Now = TRUE;
int                     FromPrompt = FALSE;
int                     Debuger = FALSE;
int                     TimeOut;
int                     OnceFlag = 0;

extern long             TimerT;
extern long             TimerD;
extern long             TimerC;
extern long             TimerL;
extern long             TimerS;
extern long             TimerH;

extern WORD             TimerTalk;
extern WORD             TimerDial;
extern WORD             TimerConnect;
extern WORD             TimerLogon;
extern WORD             TimerSend;
extern WORD             TimerHangUp;

void mouse(int one,int two,int three,int four);
void (interrupt far *OldInt09)(void); 
void interrupt far Int09(void);
char far *flags;
char far *kbdBuffer;
extern BOOL               AltKey;
BOOL               MouseDown = FALSE;
BOOL               KeyDown = FALSE;
BOOL               EscapeKey = FALSE;
char               message[80];
int                CurrMenu = -1;
int                SubMenu = 0;
BOOL               Immediate = FALSE;

struct videoconfig vc;
extern WAPIINFO           WapiInfo;
extern DosBEEPINFO        BeepInfo;

COORDS                    Coord_List[30];
union REGS Mouse_Regs;
int one, two, three, four;
BOOL MouseExists = TRUE;

ITEM mnuHelp[3] =
{               
    { 0, "Help"     },   
    { 0, "About"    },   
    { 0, ""         } 
};

extern MULT_CONTROL_PARAM  Message_CntlInfo;
//------------------------------------------------------------------------
//
//     DosBEEP function:   void main(int argc, char **argv)
//
//     decsription:
//
//------------------------------------------------------------------------
void main(int argc, char **argv)
{
   long iMainCur;
   BOOL ProcessOn = FALSE;
   BOOL DrawOn = FALSE;
   short loop = TRUE;
   int Process = 2;
   BOOL smloop = TRUE;
   int fFirstTime = TRUE;
   int MenuJump = 1;

   ITEM mnuFile[] =
   {
       { 0, "Print..."             },   
       { 1, "Printer Setup..."     },   
       {-1, "----------------"     },   
       { 1, "Exit"                 },   
       { 0, ""                     } 
   };

   ITEM mnuTools[] =
   {
       { 0, "Port Setup"           },   
       { 0, "Options"              },   
       { 0, ""                     } 
   };

   ITEM mnuWindows[] =
   {
       { 0, "Send"                 },   
       { 0, "Configuration..."     },   
       { 0, "Miscellaneous..."     },   
       { 0, ""                     } 
   };

   ITEM mnuConfig[] =
   {
       { 0, "Subscriber"           },   
       { 1, "Service"              },   
       { 0, "Group"                },   
       { 0, ""                     } 
   };


   ITEM mnuMisc[] =
   {
       { 0, "Log Viewer"           },   
       { 0, "Predefined Messages"  },   
       { 0, ""                     } 
   };


   ITEMBAR mnuScreen[] =
   {         
       { 7,  0, "File"     },   
       { 8,  0, "Tools"     },   
       { 55, 0, "Window"  },   
       { 5,  0, "Help"     } 
   };

   enum MENUS
   {
       FILE, TOOLS, WINDOW, HELP
   };
   enum MENUS CurrSelection;
         
   _getvideoconfig( &vc );

   memset(&WapiInfo, 0, sizeof(WAPIINFO));
   //WapiInfo.dwOptions = 1;

   //----------------------------------------------------------------
       // Get initial values loaded
   //----------------------------------------------------------------
   GetINIValues();
   if((WapiInfo.dwOptions & OPT_LOG_TRANSACTIONS) || (WapiInfo.dwOptions & OPT_LOG_ERRORS))
       WriteToLogFile("DosBEEP 2.0 Started");

   //UpdateModem();
   //----------------------------------------------------------------
       // if specified, purge old log files
   //----------------------------------------------------------------
   if(BeepInfo.nDaysOld > 0)
       PurgeLogs(BeepInfo.nDaysOld);

   //----------------------------------------------------------------
       // if there are any parameters on the command prompt, do
       // prompt version of DosBEEP
   //----------------------------------------------------------------
   if(argc > 1)
       RunFromDosPrompt(argc, argv);

   //----------------------------------------------------------------
       // Otherwise start screen version
   //----------------------------------------------------------------

   _setvideomode(_DEFAULTMODE);
   _clearscreen( _GCLEARSCREEN );

   mouse(0, 0, 0, 0);
   if(one != -1)
      MouseExists = FALSE;
   else
   {
      mouse(7, 0, 0, 639);
      mouse(8, 0, 0, 199);
      mouse(1, 0, 0, 0);
   }

   memset(&Coord_List, 0, sizeof(Coord_List));

   setcntl();

   DrawMainScreen();
   AboutBox();
   // ----------------- main loop ------------------------------

   flags = ((char far *) 0x00000417);
   //kbdBuffer = (char far *) 0x0000041E;
   OldInt09 = _dos_getvect( 0x0009 );
   _dos_setvect( 0x0009, Int09 );
   AltKey = FALSE;
   CurrSelection = FILE;
   SubMenu = 0;
       
   while(loop == TRUE)
   {
   	DrawMainScreen();

       while(smloop == TRUE)
       {
           switch(Process)
           {
               case 1:
               {
                   long ret;

                   StatusBar("Welcome to DosBEEP v. 2.00.07", CENTER, 0, 25, _TBLACK, _TWHITE);
                   sf = 0;
                   KeyDown = TRUE;
                   ret = MenuBar( 0, 0, mnuScreen, 4, -1, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   KeyDown = FALSE;
                   if(ret == -9) // Alt Key up
                   {
                       sf = 1;
                       CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   }
                   else
                       CurrSelection = (enum MENUS) ret;
                   
                   if((CurrSelection == ALT) || (CurrSelection == ESCAPE))
                   {
                       Process = 2;
                       AltKey = FALSE;
                   }
                   else
                       smloop = FALSE;
               }
               break;

               case 2:
               {
                   short of;
                   long trck = 0;

                   StatusBar("Welcome to DosBEEP v. 2.00.07", CENTER, 0, 25, _TBLACK, _TWHITE);
                   KeyDown = FALSE;
                   sf = 1;
                   of = 1;
                   MenuBar( 0, 0, mnuScreen, 4, -1, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                   CurrSelection = FILE;
                   while(AltKey == FALSE)
                   {
                       if(_kbhit())
                           getch();

                       mouse(3, 0, 0, 0);

                       if(HandleMouse() != -1)
                       {
                           if(CurrMenu < 10)
                               CurrSelection = (enum MENUS) CurrMenu;
                           AltKey = TRUE;
                           of = 3;
                       }
                   }
                   Process = of;
               }
               break;

               case 3:
                   MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                   StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                   smloop = FALSE;
               break;

               case 4:
                   sf = 0;
                   AltKey = FALSE;
                   CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   if((CurrSelection == ALT) || (CurrSelection == ESCAPE))
                   {
                       Process = 2;
                       AltKey = FALSE;
                   }
                   else
                       smloop = FALSE;
               break;
           }
       }

       smloop = TRUE;
       strcpy(message, "Select menu item.");

       switch(CurrSelection)
       {
           case FILE:
           {
               Coord_List[10].x1 = 8;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 144;
               Coord_List[10].y2 = 16;
   
               Coord_List[11].x1 = 8;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 144;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = 8;
               Coord_List[12].y1 = 40;
               Coord_List[12].x2 = 144;
               Coord_List[12].y2 = 40;

               Coord_List[13].x1 = -1;

               //MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
               if(!Immediate)
               {
                   SubMenu = Menu( 4, 10, mnuFile, SubMenu, FILE);
                   if(CurrMenu == -1)
                   {
                       Process = 2;
                       AltKey = FALSE;
                       SubMenu = 0;
                       break;
                   }

                   if(CurrMenu >= 0)
                   {
                       if(CurrMenu < 10)
                       {
                           Process = 3;
                           CurrSelection = (enum MENUS) CurrMenu;
                           SubMenu = 0;
                           break;
                       }    
                       else
                       {
                           CurrMenu -= 10;
                           SubMenu = CurrMenu;
                           Menu( 4, 10, mnuFile, SubMenu, DRAW_ONLY|NO_HILIGHT);
                       }
                   }            
               }
               else
                   Menu( 4, 10, mnuFile, SubMenu, DRAW_ONLY|NO_HILIGHT);


               Immediate = FALSE;

               switch(SubMenu)
               {
                   case PREVIOUS:
                       CurrSelection = HELP;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection = TOOLS;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0: //  print databases
                   {
                       int offset = 0;

                       ITEM    SubList[] = 
                       {
                           {   6,  "Print Services"     },
                           {   8,  "Print Subscribers"  },
                           {   6,  "Print Groups"       },
                           {   6,  "Print Log"          },
                           {   0,  ""                   }
                       };

                       Coord_List[13].x1 = 168;
                       Coord_List[13].y1 = 24;
                       Coord_List[13].x2 = 312;
                       Coord_List[13].y2 = 24;

                       Coord_List[14].x1 = 168;
                       Coord_List[14].y1 = 32;
                       Coord_List[14].x2 = 312;
                       Coord_List[14].y2 = 32;

                       Coord_List[15].x1 = 168;
                       Coord_List[15].y1 = 40;
                       Coord_List[15].x2 = 312;
                       Coord_List[15].y2 = 40;

                       Coord_List[16].x1 = 168;
                       Coord_List[16].y1 = 48;
                       Coord_List[16].x2 = 312;
                       Coord_List[16].y2 = 48;

                       Coord_List[17].x1 = -1;

                       while(loop == TRUE)
                       {
                           offset = Menu( 5, 30, SubList, offset, 0);
                           if(CurrMenu != 10)
                           {
                               if(CurrMenu == -1)
                               {
                                   Process = 2;
                                   AltKey = FALSE;
                                   Immediate = FALSE;
                                   SubMenu = 0;
                                   break;
                               }

                               if(CurrMenu >= 0)
                               {
                                   if(CurrMenu < 13)
                                   {
                                       if(CurrMenu >= 10)
                                       {
                                           Immediate = TRUE;
                                           CurrSelection = FILE;
                                           SubMenu = (CurrMenu - 10);
                                       }
                                       else
                                       {
                                           CurrSelection = (enum MENUS) CurrMenu;
                                           Immediate = FALSE;
                                           SubMenu = 0;
                                       }
                                       Process = 3;
                                       break;
                                   }    
                                   else
                                   {
                                       CurrMenu -= 13;
                                       offset = CurrMenu;
                                       Menu( 4, 10, mnuFile, SubMenu, DRAW_ONLY|NO_HILIGHT);
                                   }
                               }

                               switch(offset)
                               {
                                   case ALT:
                                       Process = 2;
                                       AltKey = FALSE;
                                   break;

                                   case 0:
                                       PrintDatabase(offset);
                                       Process = 2;
                                       offset = ALT;
                                   break;

                                   case 1:
                                       PrintDatabase(offset);
                                       Process = 2;
                                       offset = ALT;
                                   break;

                                   case 2:
                                       PrintDatabase(offset);
                                       Process = 2;
                                       offset = ALT;
                                   break;

                                   case 3:
                                       PrintDatabase(offset);
                                       Process = 2;
                                       offset = ALT;
                                   break;

                                   case ESCAPE:
                                       Process = 3;
                                       SubMenu = 0;
                                   break;

                                   default:
                                       Process = 2;
                                       SubMenu = 0;
                                   break;
                               }

                               if (offset == ESCAPE)
                               {
                                   Process = 3;
                                   break;
                               }

                               if (offset == ALT)
                               {
                                   AltKey = FALSE;
                                   break;
                               }
                           }
                           else
                               offset = 0;
                       }
                   }
                   break;
                   case 1: //  printer setup
                   {
                       int offset = 0;
                       BOOL Write = FALSE;

                       ITEM    SubList2[]=
                       { 
                           {   0, "NONE"       },
                           {   7, "DEV LPT1"   },
                           {   7, "DEV LPT2"   },
                           {   7, "DEV LPT3"   },
                           {   0, ""           }
                       };

                       Coord_List[13].x1 = 168;
                       Coord_List[13].y1 = 32;
                       Coord_List[13].x2 = 312;
                       Coord_List[13].y2 = 32;

                       Coord_List[14].x1 = 168;
                       Coord_List[14].y1 = 40;
                       Coord_List[14].x2 = 312;
                       Coord_List[14].y2 = 40;

                       Coord_List[15].x1 = 168;
                       Coord_List[15].y1 = 48;
                       Coord_List[15].x2 = 312;
                       Coord_List[15].y2 = 48;

                       Coord_List[16].x1 = 168;
                       Coord_List[16].y1 = 56;
                       Coord_List[16].x2 = 312;
                       Coord_List[16].y2 = 56;

                       Coord_List[17].x1 = -1;

                       while(loop == TRUE)
                       {
                           switch(BeepInfo.nPrinterPort)
                           {
                               case PRINTER_PORT_1:
                                   offset = 1;
                                   strcpy(message, "Default Printer is on LPT1.");
                               break;
                               case PRINTER_PORT_2:
                                   offset = 2;
                                   strcpy(message, "Default Printer is on LPT2.");
                               break;
                               case PRINTER_PORT_3:
                                   offset = 3;
                                   strcpy(message, "Default Printer is on LPT3.");
                               break;
                               case PRINTER_PORT_NONE:
                               default:
                                   BeepInfo.nPrinterPort = 0;
                                   offset = 0;
                                   strcpy(message, "Default Printer is not available.");
                               break;
                           }

                           StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
    
                           offset = Menu( 6, 26, SubList2, offset, 0);
                           if(CurrMenu != 11)
                           {
                               if(CurrMenu == -1)
                               {
                                   Process = 2;
                                   AltKey = FALSE;
                                   Immediate = FALSE;
                                   SubMenu = 0;
                                   break;
                               }

                               if(CurrMenu >= 0)
                               {
                                   if(CurrMenu < 13)
                                   {
                                       if(CurrMenu >= 10)
                                       {
                                           Immediate = TRUE;
                                           CurrSelection = FILE;
                                           SubMenu = (CurrMenu - 10);
                                       }
                                       else
                                       {
                                           CurrSelection = (enum MENUS) CurrMenu;
                                           Immediate = FALSE;
                                           SubMenu = 0;
                                       }
                                       Process = 3;
                                       break;
                                   }    
                                   else
                                   {
                                       CurrMenu -= 13;
                                       offset = CurrMenu;
                                       Menu( 4, 10, mnuFile, SubMenu, DRAW_ONLY|NO_HILIGHT);
                                   }
                               }
   
                               Write = FALSE;

                               switch(offset)
                               {
                                   case ALT:
                                       Process = 2;
                                       AltKey = FALSE;
                                   break;

                                   case ESCAPE:
                                       Process = 3;
                                       SubMenu = 0;
                                   break;

                                   case 1:
                                       BeepInfo.nPrinterPort = PRINTER_PORT_1;
                                       Write = TRUE;
                                   break;

                                   case 2:
                                       BeepInfo.nPrinterPort = PRINTER_PORT_2;
                                       Write = TRUE;
                                   break;

                                   case 3:
                                       BeepInfo.nPrinterPort = PRINTER_PORT_3;
                                       Write = TRUE;
                                   break;

                                   case 0:
                                       BeepInfo.nPrinterPort = PRINTER_PORT_NONE;
                                       Write = TRUE;
                                   break;
                               }

                               if (offset == ESCAPE)
                               {
                                   Process = 3;
                                   break;
                               }

                               if (offset == ALT)
                               {
                                   AltKey = FALSE;
                                   break;
                               }

                               if(Write == TRUE)
                               {
                                   char    szBuffer[256];

                                   sprintf(message, "%d", offset);

                                   if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
                                       strcpy(szBuffer, "WAPI.INI");

                                   WriteProfileString("DosBEEP_Setup", "PrinterDev", message, szBuffer);

                                   switch(BeepInfo.nPrinterPort)
                                   {
                                       case PRINTER_PORT_1:
                                           offset = 1;
                                           strcpy(message, "Default Printer is on LPT1.");
                                       break;
                                       case PRINTER_PORT_2:
                                           offset = 2;
                                           strcpy(message, "Default Printer is on LPT2.");
                                       break;
                                       case PRINTER_PORT_3:
                                           offset = 3;
                                           strcpy(message, "Default Printer is on LPT3.");
                                       break;
                                       case PRINTER_PORT_NONE:
                                       default:
                                           BeepInfo.nPrinterPort = 0;
                                           offset = 0;
                                           strcpy(message, "Default Printer is not available.");
                                       break;
                                   }

                                   StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                                   break;
                               }
                           }
                           else
                               offset = 0;
                       }

                   }
                   break;
                   case 2: //  exit - mouse
                   case 3: //  exit - keys
                       _dos_setvect( 0x0009, OldInt09 );
                       if((WapiInfo.dwOptions & OPT_LOG_TRANSACTIONS) || (WapiInfo.dwOptions & OPT_LOG_ERRORS))
                           WriteToLogFile("DosBEEP 2.0 Shutdown");
	                    _setvideomode( _DEFAULTMODE );
                       return;
                   break;

                   case ALT:
                       AltKey = FALSE;
                       SubMenu = 0;
                       Process = 2;
                   break;

                   case ESCAPE:
                       SubMenu = 0;
                       Process = 4;
                   break;
               }
           }
           break;

           case TOOLS:
           {
               Coord_List[10].x1 = 64;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 152;
               Coord_List[10].y2 = 16;
   
               Coord_List[11].x1 = 64;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 152;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = -1;

               //MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
	            SubMenu = Menu( 3, 14, mnuTools, SubMenu, 0);
               if(CurrMenu == -1)
               {
                   Process = 2;
                   AltKey = FALSE;
                   SubMenu = 0;
                   break;
               }

               if(CurrMenu >= 0)
               {
                   if(CurrMenu < 10)
                   {
                       Process = 3;
                       CurrSelection = (enum MENUS) CurrMenu;
                       SubMenu = 0;
                       break;
                   }    
                   else
                   {
                       CurrMenu -= 10;
                       SubMenu = CurrMenu;
                   }
               }

               switch(SubMenu)
               {
                   case PREVIOUS:
                       CurrSelection = FILE;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection = WINDOW;
                       SubMenu = 0;
                       Process = 3;
                   break;

                   case 0: // port setup
                       Modem();
                       setcntl();
                       SubMenu = 0;
                       Process = 2;
                   break;

                   case 1: // settings
                       Settings();
                       setcntl();
                       SubMenu = 0;
                       Process = 2;
                   break;
               
                   case ESCAPE:
                       SubMenu = 0;
                       Process = 4;
                   break;

                   case ALT:
                       AltKey = FALSE;
                       SubMenu = 0;
                       Process = 2;
                   break;

               }
           }
           break;

           case WINDOW:
           {
               Coord_List[10].x1 = 120;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 256;
               Coord_List[10].y2 = 16;
   
               Coord_List[11].x1 = 120;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 256;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = 120;
               Coord_List[12].y1 = 32;
               Coord_List[12].x2 = 256;
               Coord_List[12].y2 = 32;

               Coord_List[13].x1 = -1;

               //MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
               if(!Immediate)
               {
	                SubMenu = Menu( 3, 25, mnuWindows, SubMenu, 0);
                   if(CurrMenu == -1)
                   {
                       Process = 2;
                       AltKey = FALSE;
                       SubMenu = 0;
                       break;
                   }

                   if(CurrMenu >= 0)
                   {
                       if(CurrMenu < 10)
                       {
                           Process = 3;
                           CurrSelection = (enum MENUS) CurrMenu;
                           SubMenu = 0;
                           break;
                       }    
                       else
                       {
                           CurrMenu -= 10;
                           SubMenu = CurrMenu;
                           Menu( 3, 25, mnuWindows, SubMenu, DRAW_ONLY|NO_HILIGHT);
                       }
                   }
               }
               else
                   Menu( 3, 25, mnuWindows, SubMenu, DRAW_ONLY|NO_HILIGHT);
                                           
               Immediate = FALSE;

               switch(SubMenu)
               {
                   case PREVIOUS:
                       CurrSelection = TOOLS;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection = HELP;
                       SubMenu = 0;
                       Process = 3;
                   break;

                   case 0: // send
                       Send_Page();
                       setcntl();
                       SubMenu = 0;
                       Process = 2;
                   break;

                   case 1:
                   {
                       long offset = 0;
                       BOOL loop = TRUE;

                       Coord_List[13].x1 = 280;
                       Coord_List[13].y1 = 32;
                       Coord_List[13].x2 = 368;
                       Coord_List[13].y2 = 32;
                       
                       Coord_List[14].x1 = 280;
                       Coord_List[14].y1 = 40;
                       Coord_List[14].x2 = 368;
                       Coord_List[14].y2 = 40;

                       Coord_List[15].x1 = 280;
                       Coord_List[15].y1 = 48;
                       Coord_List[15].x2 = 368;
                       Coord_List[15].y2 = 48;

                       Coord_List[16].x1 = -1;
                       
                       while(loop == TRUE)
                       {
                           offset = Menu( 5, 42, mnuConfig, offset, 0);
                           if(CurrMenu != 11)
                           {
                               if(CurrMenu == -1)
                               {
                                   Process = 2;
                                   AltKey = FALSE;
                                   Immediate = FALSE;
                                   SubMenu = 0;
                                   break;
                               }

                               if(CurrMenu >= 0)
                               {
                                   if(CurrMenu < 13)
                                   {
                                       if(CurrMenu >= 10)
                                       {
                                           Immediate = TRUE;
                                           CurrSelection = WINDOW;
                                           SubMenu = (CurrMenu - 10);
                                       }
                                       else
                                       {
                                           CurrSelection = (enum MENUS) CurrMenu;
                                           Immediate = FALSE;
                                           SubMenu = 0;
                                       }
                                       Process = 3;
                                       break;
                                   }    
                                   else
                                   {
                                       CurrMenu -= 13;
                                       offset = CurrMenu;
                                       Menu( 3, 25, mnuWindows, SubMenu, DRAW_ONLY|NO_HILIGHT);
                                   }
                               }

                               switch(offset)
                               {
                                   case ALT:
                                       Process = 2;
                                       SubMenu = 0;
                                       AltKey = FALSE;
                                   break;

                                   case 0: // Subscriber
                                       Subscriber();
                                       setcntl();
                                       Process = 2;
                                       SubMenu = 0;
                                       offset = ALT;
                                   break;

                                   case 1: // Service
                                       Service();
                                       setcntl();
                                       Process = 2;
                                       SubMenu = 0;
                                       offset = ALT;
                                   break;

                                   case 2: // Group
                                       Group();
                                       setcntl();
                                       Process = 2;
                                       SubMenu = 0;
                                       offset = ALT;
                                   break;

                                   case ESCAPE:
                                       Process = 3;
                                       SubMenu = 0;
                                   break;

                                   default:
                                       Process = 2;
                                       SubMenu = 0;
                                   break;
                               }

                               if (offset == ESCAPE)
                               {
                                   Process = 3;
                                   break;
                               }

                               if (offset == ALT)
                               {
                                   AltKey = FALSE;
                                   break;
                               }
                           }
                           else
                               offset = 0;
                       }
                   }
                   break;

                   case 2:
                   {
                       long offset = 0;
                       BOOL loop = TRUE;

                       Coord_List[13].x1 = 280;
                       Coord_List[13].y1 = 40;
                       Coord_List[13].x2 = 440;
                       Coord_List[13].y2 = 40;

                       Coord_List[14].x1 = 280;
                       Coord_List[14].y1 = 48;
                       Coord_List[14].x2 = 440;
                       Coord_List[14].y2 = 48;

                       Coord_List[15].x1 = -1;

                       while(loop == TRUE)
                       {
                           offset = Menu( 6, 46, mnuMisc, offset, 0);
                           if(CurrMenu != 12)
                           {
                               if(CurrMenu == -1)
                               {
                                   Process = 2;
                                   AltKey = FALSE;
                                   Immediate = FALSE;
                                   SubMenu = 0;
                                   break;
                               }

                               if(CurrMenu >= 0)
                               {
                                   if(CurrMenu < 13)
                                   {
                                       if(CurrMenu >= 10)
                                       {
                                           Immediate = TRUE;
                                           CurrSelection = WINDOW;
                                           SubMenu = (CurrMenu - 10);
                                       }
                                       else
                                       {
                                           CurrSelection = (enum MENUS) CurrMenu;
                                           Immediate = FALSE;
                                           SubMenu = 0;
                                       }
                                       Process = 3;
                                       break;
                                   }    
                                   else
                                   {
                                       CurrMenu -= 13;
                                       offset = CurrMenu;
                                       Menu( 3, 25, mnuWindows, SubMenu, DRAW_ONLY|NO_HILIGHT);
                                   }
                               }

                               switch(offset)
                               {
                                   case ALT:
                                       Process = 2;
                                       SubMenu = 0;
                                       AltKey = FALSE;
                                   break;

                                   case 0: // Subscriber
                                       Log();
                                       setcntl();
                                       Process = 2;
                                       SubMenu = 0;
                                       offset = ALT;
                                   break;

                                   case 1: // Service
                                       Predefined();
                                       setcntl();
                                       Process = 2;
                                       SubMenu = 0;
                                       offset = ALT;
                                   break;

                                   case ESCAPE:
                                       Process = 3;
                                       SubMenu = 0;
                                   break;

                                   default:
                                       Process = 2;
                                       SubMenu = 0;
                                   break;
                               }

                               if (offset == ESCAPE)
                               {
                                   Process = 3;
                                   break;
                               }

                               if (offset == ALT)
                               {
                                   AltKey = FALSE;
                                   break;
                               }
                           }
                           else
                               offset = 0;
                       }
                   }
                   break;
                   
                   case ALT:
                       AltKey = FALSE;
                       SubMenu = 0;
                       Process = 2;
                   break;

                   case ESCAPE:
                       SubMenu = 0;
                       Process = 4;
                   break;
               }
           }
           break;

           case HELP:
           {
               Coord_List[10].x1 = 568;
               Coord_List[10].y1 = 16;
               Coord_List[10].x2 = 616;
               Coord_List[10].y2 = 16;
   
               Coord_List[11].x1 = 568;
               Coord_List[11].y1 = 24;
               Coord_List[11].x2 = 616;
               Coord_List[11].y2 = 24;

               Coord_List[12].x1 = -1;

               //MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
	            SubMenu = Menu( 3, 74, mnuHelp, SubMenu, 0);
               if(CurrMenu == -1)
               {
                  Process = 2;
                  AltKey = FALSE;
                  SubMenu = 0;
                  break;
               }

               if(CurrMenu >= 0)
               {
                   if(CurrMenu < 10)
                   {
                       Process = 3;
                       CurrSelection = (enum MENUS) CurrMenu;
                       SubMenu = 0;
                       break;
                   }    
                   else
                   {
                       CurrMenu -= 10;
                       SubMenu = CurrMenu;
                   }
               }

               switch(SubMenu)
               {
                   case PREVIOUS:
                       CurrSelection = WINDOW;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection = FILE;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case MNU_HELP:
                       DrawMainScreen();
                       MenuBar( 0, 0, mnuScreen, 4, CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                       LoadHelpFile();
                       Process = 3;
                       if(CurrMenu >= 0)
                       {
                           AltKey = FALSE;
                           Process = 2;
                       }
                   break;
                   case MNU_ABOUT:
                       AboutBox();
                       Process = 3;
                       if(CurrMenu >= 0)
                       {
                           AltKey = FALSE;
                           Process = 2;
                       }
                   break;
                   
                   case ALT:
                       AltKey = FALSE;
                       SubMenu = 0;
                       Process = 2;
                   break;

                   case ESCAPE:
                       SubMenu = 0;
                       Process = 4;
                   break;
               }
           }
           break;
       }
   }
}

void setcntl()
{
   ITEMBAR mnuScreen[] =
   {         
       { 7,  0, "File"     },   
       { 8,  0, "Tools"     },   
       { 55, 0, "Window"  },   
       { 5,  0, "Help"     } 
   };

   Coord_List[0].x1 = 8;
   Coord_List[0].y1 = 0;
   Coord_List[0].x2 = 48;
   Coord_List[0].y2 = 0;
   Coord_List[0].hc = mnuScreen[0].achItem[mnuScreen[0].iHilite];
   
   Coord_List[1].x1 = 64;
   Coord_List[1].y1 = 0;
   Coord_List[1].x2 = 112;
   Coord_List[1].y2 = 0;
   Coord_List[1].hc = mnuScreen[1].achItem[mnuScreen[1].iHilite];

   Coord_List[2].x1 = 128;
   Coord_List[2].y1 = 0;
   Coord_List[2].x2 = 184;
   Coord_List[2].y2 = 0;
   Coord_List[2].hc = mnuScreen[2].achItem[mnuScreen[2].iHilite];

   Coord_List[3].x1 = 568;
   Coord_List[3].y1 = 0;
   Coord_List[3].x2 = 608;
   Coord_List[3].y2 = 0;
   Coord_List[3].hc = mnuScreen[3].achItem[mnuScreen[3].iHilite];

   Coord_List[4].x1 = -1;
   Coord_List[10].x1 = -1;
}
