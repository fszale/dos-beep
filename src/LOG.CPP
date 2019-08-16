#include <dos.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
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

extern struct videoconfig  vc;
extern WAPIINFO            WapiInfo;
extern char                message[80];
extern int                 sf;
extern BOOL                KeyDown;
extern BOOL                AltKey;
extern ITEM                mnuHelp[3];
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void Log()
{
   BOOL    ProcessOn = FALSE;
   BOOL    DrawOn = FALSE;
   BOOL    smloop = TRUE;
   int     Process = 2;
   long    SubMenu;
   short   loop = TRUE;
   
   ITEM mnuMain[] =
   {
       { 0, "View"             },   
       { 0, "Delete"           },   
       {-1, "---------------"  },   
       { 0, "Return    <Esc>"  },   
       { 0, ""                 } 
   };
   
   ITEMBAR mnuScreen[] =
   {                  
       { 70, 0,"File" },
       {  5, 0,"Help" }
   };

   enum MENUS
   {
       SUB_FILE, HELP
   };
   enum MENUS CurrSelection;

   Coord_List[0].x1 = 8;
   Coord_List[0].y1 = 0;
   Coord_List[0].x2 = 48;
   Coord_List[0].y2 = 0;
   Coord_List[0].hc = mnuScreen[0].achItem[mnuScreen[0].iHilite];

   Coord_List[1].x1 = 568;
   Coord_List[1].y1 = 0;
   Coord_List[1].x2 = 608;
   Coord_List[1].y2 = 0;
   Coord_List[1].hc = mnuScreen[1].achItem[mnuScreen[1].iHilite];
   
   Coord_List[2].x1 = -1;
   Coord_List[10].x1 = -1;

   CurrSelection = SUB_FILE;
   AltKey = FALSE;
   SubMenu = 0;

   while( loop == TRUE )
   {
       DrawLogScreen();
       while(smloop == TRUE)
       {
           switch(Process)
           {
               case 1:
               {
                   int ret;
                   StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);
                   KeyDown = TRUE;
                   sf = 0;
                   ret = (enum MENUS) MenuBar( 0, 0, mnuScreen, 2, -1, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   KeyDown = FALSE;

                   if(ret == -9)
                   {
                       sf = 1;
                       CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 2, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
                   }
                   else
                       CurrSelection = (enum MENUS) ret;

                   if((CurrSelection == ALT) || (CurrSelection == ESCAPE))
                       Process = 2;
                   else
                       smloop = FALSE;
               }
               break;

               case 2:
               {
                   int of = 1;

                   StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);
                   KeyDown = FALSE;
                   sf = 1;
                   MenuBar( 0, 0, mnuScreen, 2, -1, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                   CurrSelection = SUB_FILE;
                   while(AltKey == FALSE)
                   {
                       if(_kbhit())
                           if(getch() == 0x1B)
                               return;

                       mouse(3, 0, 0, 0);

                       if(HandleMouse() != -1)
                       {
                           if(CurrMenu < 10)
                               CurrSelection = (enum MENUS) CurrMenu;
                           AltKey = TRUE;
                           SubMenu = 0;
                           of = 3;
                       }
                   }

                   Process = of;
               }
               break;

               case 3:
                   MenuBar( 0, 0, mnuScreen, 2, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                   StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                   smloop = FALSE;
               break;

               case 4:
                   sf = 0;
                   AltKey = FALSE;
                   CurrSelection = (enum MENUS) MenuBar( 0, 0, mnuScreen, 2, CurrSelection, _TBLACK, _TWHITE, PROCESS, HILIGHT);
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
           case SUB_FILE:
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

	            SubMenu = (enum CHOICES) Menu( 4, 9, mnuMain, SubMenu, 0);
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
                       CurrSelection = HELP;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection= HELP;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                   {
                       struct _find_t  pc_file;
                       ITEM    *pLineList = NULL;
                       int     index = 0, entries, retval = 0;
                       char    szSourceName[256];

                       Process = 3;

                       if((pLineList = (ITEM *) malloc( 61 * sizeof(ITEM) )) != NULL)
                       {
                           if (WapiInfo.szLogDir[strlen(WapiInfo.szLogDir) - 1] != '\\')
      	                        strcat(WapiInfo.szLogDir, "\\");

	                        sprintf(szSourceName, "%s*.LOG", WapiInfo.szLogDir);
	                        if (_dos_findfirst(szSourceName, _A_NORMAL, &pc_file) != 0)
                           {
                               strcpy(message, "ERROR: Log files were not found.");
                               Beep(698, 700);
                               free(pLineList);
                           }
                           else
                           {
	                            do
	                            {
                                   pLineList[index].iHilite = -1;
                                   strcpy((LPSTR) pLineList[index].achItem, (LPCSTR) pc_file.name);
                                   sprintf(szSourceName, "%s%s", WapiInfo.szLogDir, pc_file.name);
                                   index++;
	                            }
	                            while ((_dos_findnext(&pc_file) == 0) && (index < 31));
 
                               if(index > 19)
                                   entries = 19;
                               else
                                   entries = index;
                           
                               MenuBar( 0, 0, mnuScreen, 1, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, HILIGHT);
                               StatusBar("Select log to view.", CENTER, 0, 25, _TBLACK, _TWHITE);

                               if((retval = ListBox( 3, 20, 12, entries, pLineList, index, 0, HILIGHT, NULL)) >= 0)
                               {
                                   char    filename[15];
                                   char    path[256];
                                   char    szBuffer[256];
                                   FILE    *fhLogFile;
                                   int     counter = 0;
                                       
                                   DrawLogScreen();
                                   MenuBar( 0, 0, mnuScreen, 1, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                                   strcpy(filename, pLineList[retval].achItem);
                                   sprintf(message, "Viewing log %s.", filename);
                                   StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
                       
                                   free(pLineList);
                                   pLineList = NULL;

                                   sprintf(path, "%s%s", WapiInfo.szLogDir, filename);
                             
                                   if((fhLogFile = fopen(path, "r")) != NULL)
                                   {
                                       if((pLineList = (ITEM *) malloc( 500 * sizeof(ITEM) )) != NULL)
                                       {
                                           index = 0;
                                           fseek(fhLogFile, 0, SEEK_SET);
                                           while((fgets( szBuffer, 255, fhLogFile) != NULL) && (index < 500))
                                           {
                                               pLineList[index].iHilite = -1;
                                               szBuffer[strlen(szBuffer) - 1] = 0;
                                               memset(pLineList[index].achItem, 0, sizeof(pLineList[index].achItem));
                                               strncpy((LPSTR) pLineList[index].achItem, (LPCSTR) szBuffer, 70);
                                               index++;
                                           }
                                           fclose(fhLogFile);

                                           if(index > 19)
                                               entries = 19;
                                           else
                                               entries = index;
   	             
                                           ListBox( 3, 2, 73, entries, pLineList, index, 0, NO_HILIGHT, NULL);
                                           strcpy(message, "Select menu item.");
                                           free(pLineList);
                                       }
                                       else
                                       {
                                           fclose(fhLogFile);
                                           strcpy(message, "ERROR: Out of memory.");
                                           Beep(698, 700);
                                       }
                                   }
                                   else
                                   {
                                       strcpy(message, "ERROR: Could not open Log file.");
                                       Beep(698, 700);
                                   }
                               }
                               else
                                   free(pLineList);
                           }
                       }
                       else
                       {
                           strcpy(message, "ERROR: Out of memory.");
                           Beep(698, 700);
                       }
                   }
                   break;
                   case 1:
                   {
                       struct _find_t  pc_file;
                       ITEM    *pLineList = NULL;
                       int     index = 0, entries, retval = 0;
                       char    szSourceName[256];
                       
                       Process = 3;

                       if((pLineList = (ITEM *) malloc( 61 * sizeof(ITEM) )) != NULL)
                       {
                           if (WapiInfo.szLogDir[strlen(WapiInfo.szLogDir) - 1] != '\\')
      	                        strcat(WapiInfo.szLogDir, "\\");

	                        sprintf(szSourceName, "%s*.LOG", WapiInfo.szLogDir);
	                        if (_dos_findfirst(szSourceName, _A_NORMAL, &pc_file) != 0)
                           {
                               strcpy(message, "ERROR: Log files were not found.");
                               Beep(698, 700);
                           }
                           else
                           {
	                            do
	                            {
                                   pLineList[index].iHilite = -1;
                                   strcpy((LPSTR) pLineList[index].achItem, (LPCSTR) pc_file.name);
                                   sprintf(szSourceName, "%s%s", WapiInfo.szLogDir, pc_file.name);
                                   index++;
	                            }
	                            while ((_dos_findnext(&pc_file) == 0) && (index < 31));
 
                               if(index > 19)
                                   entries = 19;
                               else
                                   entries = index;
                           
                               MenuBar( 0, 0, mnuScreen, 1, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                               StatusBar("Select log to delete.", CENTER, 0, 25, _TBLACK, _TWHITE);

                               if((retval = ListBox( 4, 20, 12, entries, pLineList, index, 0, HILIGHT, NULL)) >= 0)
                               {
                                   sprintf(szSourceName, "%s%s", WapiInfo.szLogDir, pLineList[retval].achItem);
                                   remove(szSourceName);                           
                                   strcpy(message, "Log deleted.");
                               }
                           }
                               
                           free(pLineList);
                       }
                       else
                       {
                           strcpy(message, "ERROR: Out of memory.");
                           Beep(698, 700);
                       }
                   }
                   break;

                   case 2:
                   case 3:
                       return;
                   break;

                   case ALT:
                       AltKey = FALSE;
                       Process = 2;
                   break;

                   case ESCAPE:
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

               SubMenu = 0;

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
                       CurrSelection = SUB_FILE;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case NEXT:
                       CurrSelection = SUB_FILE;
                       SubMenu = 0;
                       Process = 3;
                   break;
                   case 0:
                       DrawMainScreen();
                       MenuBar( 0, 0, mnuScreen, 2, (int) CurrSelection, _TBLACK, _TWHITE, DRAW_ONLY, NO_HILIGHT);
                       LoadHelpFile();
                       Process = 3;
                       if(CurrMenu >= 0)
                       {
                           AltKey = FALSE;
                           Process = 2;
                       }
                   break;
                   case 1:
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

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawLogScreen()
{
   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, vc.numtextcols - 2, _TWHITE, _TBLUE, BORDER);
   StatusBar("", LEFT, 0, 25, _TWHITE, _TWHITE);

   PrintText(" Log Edit ", CENTER, 0, 2, _TBLUE, _TWHITE);
}

