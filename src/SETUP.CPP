#include <malloc.h>
#include <errno.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <share.h>
#include <process.h>
#include <direct.h>
#include <bios.h>
#include <graph.h>
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
#include "dosbeep.h"

MENU mnuAtrib =
{
    _TBLACK, _TBLACK, _TWHITE, _TBRIGHTWHITE, _TBRIGHTWHITE,
    _TWHITE, _TWHITE, _TBLACK, _TWHITE, _TBLACK,
    TRUE,
    'Ú', '¿', 'Ù', 'À', '³', 'Ä'
};

void Delay(WORD seconds);
BOOL CheckForWinbeep(void);
int copyfile( char *source, char *target );
int makedir(char *dir);
unsigned GetKey( int fWait);
char szDrive[20] = "";

void main()
{
   CONTROL_PARAM CntlInfo;
   int retVal;
   char ch;
   BOOL Edit = FALSE;

   _setvideomode(_DEFAULTMODE);
   _setbkcolor( _TBLACK );
   _clearscreen( _GCLEARSCREEN );
   _displaycursor( _GCURSOROFF );

   strcpy(CntlInfo.pCntlTitle, "Specify source drive : ");
   CntlInfo.nAlignment = LEFT;
   CntlInfo.nAlignmentOffset = 5;
   CntlInfo.nCntlRow = 4;
   CntlInfo.nCntlOffset = 28;
   CntlInfo.nCntlLength = 43;
   strcpy(CntlInfo.pCntlBuffer, "a:\\");

   Box( 2, 0, 3, 78, _TBRIGHTWHITE, _TBLUE, BORDER);

   _settextcolor( _TBRIGHTWHITE );
   _setbkcolor( _TBLUE );
   DrawControls(&CntlInfo, 1);
   ReDrawData(&CntlInfo, 1, _TBRIGHTWHITE, _TBLUE);
   StatusBar("Specify the drive to install DosBEEP from.", CENTER, 0, 7, _TBLACK, _TWHITE);
   
   if(access("C:\\CONFIG.SYS", 0) == 0)
      WriteProfileString("", "FILES", "60", "C:\\CONFIG.SYS");

   while(1)
   {
       retVal = ProcessControl(&CntlInfo, _TBRIGHTWHITE, _TBLUE, PROCESS);
       if(retVal == ESCAPE)
       {
           _settextcolor( _TWHITE );
           _setbkcolor( _TBLACK );
           _clearscreen( _GCLEARSCREEN );
           _displaycursor( _GCURSORON );
           exit(0);
       }
       else
       {
           char szBuffer[128];

           if ( CntlInfo.pCntlBuffer[strlen(CntlInfo.pCntlBuffer) - 1] != '\\')
               strcat(CntlInfo.pCntlBuffer, "\\");
       
           sprintf(szBuffer, "%sdosbeep.exe", CntlInfo.pCntlBuffer);
           if(_access(szBuffer, 0) != 0)
           {
               StatusBar("Incorrect directory. DOSBEEP.EXE was not found.", CENTER, 0, 7, _TBLACK, _TWHITE);
               Beep(698, 700);
               Delay(1);
           }
           else
           {
               strcpy(szDrive, CntlInfo.pCntlBuffer);
               if(CheckForWinbeep())
               {
                   Delay(3);
                   _settextcolor( _TWHITE );
                   _setbkcolor( _TBLACK );
                   _clearscreen( _GCLEARSCREEN );
                   _displaycursor( _GCURSORON );
                   exit(0);
               }
               else
                   break;
           }
       }
   }

   strcpy(CntlInfo.pCntlTitle ,"DosBEEP install directory :");
   strcpy(CntlInfo.pCntlBuffer, "C:\\DOSBEEP\\");

   Box( 2, 0, 3, 78, _TBRIGHTWHITE, _TBLUE, BORDER);
   _settextcolor( _TBRIGHTWHITE );
   _setbkcolor( _TBLUE );
   DrawControls(&CntlInfo, 1);
   ReDrawData(&CntlInfo, 1, _TBRIGHTWHITE, _TBLUE);
 
   while(1)
   {

       if ( CntlInfo.pCntlBuffer[strlen(CntlInfo.pCntlBuffer) - 1] != '\\')
           strcat(CntlInfo.pCntlBuffer, "\\");

       StatusBar("Specify the directory to install DosBEEP at.", CENTER, 0, 7, _TBLACK, _TWHITE);
       retVal = ProcessControl(&CntlInfo, _TBRIGHTWHITE, _TBLUE, PROCESS);
       switch(retVal)
       {
           case ENTER:
           {   // install
               char command[256];
               char command2[256];

               if(strcmp(CntlInfo.pCntlBuffer, "") == 0)
               {
                   StatusBar("Specify directory first.", CENTER, 0, 7, _TBLACK, _TWHITE);
                   Beep(698, 700);
                   break;
               }
               else
               { 
                   struct _find_t c_dir;

                   if ( CntlInfo.pCntlBuffer[strlen(CntlInfo.pCntlBuffer) - 1] == '\\')
                       CntlInfo.pCntlBuffer[strlen(CntlInfo.pCntlBuffer) - 1] = 0;

                   if(_dos_findfirst(CntlInfo.pCntlBuffer, _A_SUBDIR, &c_dir) == 0)
                   {
                       int status = 0;

                       if ( CntlInfo.pCntlBuffer[strlen(CntlInfo.pCntlBuffer) - 1] != '\\')
                           strcat(CntlInfo.pCntlBuffer, "\\");

                       sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "dosbeep.exe");
                       StatusBar("Copying DOSBEEP.EXE...", CENTER, 0, 7, _TBLACK, _TWHITE);
                       sprintf(command2, "%s%s", szDrive, "dosbeep.exe");
                       if(copyfile(command2, command) != 0)
                       {
                           StatusBar("Could not copy a DOSBEEP.EXE.", CENTER, 0, 7, _TBLACK, _TWHITE);
                           Beep(698, 700);
                           Delay(2);
                           status += 1;
                       }

                       sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "dosbeep.hlp");
                       StatusBar("Copying DOSBEEP.HLP...", CENTER, 0, 7, _TBLACK, _TWHITE);
                       sprintf(command2, "%s%s", szDrive, "dosbeep.hlp");
                       if(copyfile(command2, command) != 0)
                       {
                           StatusBar("Could not copy a DOSBEEP.HLP.", CENTER, 0, 7, _TBLACK, _TWHITE);
                           Beep(698, 700);
                           Delay(2);
                           status += 1;
                       }

                       sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "dbtsr.exe");
                       StatusBar("Copying DBTSR.EXE...", CENTER, 0, 7, _TBLACK, _TWHITE);
                       sprintf(command2, "%s%s", szDrive, "dbtsr.exe");
                       if(copyfile(command2, command) != 0)
                       {
                           StatusBar("Could not copy a DBTSR.EXE.", CENTER, 0, 7, _TBLACK, _TWHITE);
                           Beep(698, 700);
                           Delay(2);
                           status += 1;
                       }

                       sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "modemlst.idx");
                       StatusBar("Copying MODEMLST.IDX...", CENTER, 0, 7, _TBLACK, _TWHITE);
                       sprintf(command2, "%s%s", szDrive, "modemlst.idx");
                       if(copyfile(command2, command) != 0)
                       {
                           StatusBar("Could not copy a MODEMLST.IDX.", CENTER, 0, 7, _TBLACK, _TWHITE);
                           Beep(698, 700);
                           Delay(2);
                           status += 1;
                       }

                       sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "modemlst.dat");
                       StatusBar("Copying MODEMLST.DAT...", CENTER, 0, 7, _TBLACK, _TWHITE);
                       sprintf(command2, "%s%s", szDrive, "modemlst.dat");
                       if(copyfile(command2, command) != 0)
                       {
                           StatusBar("Could not copy a MODEMLST.DAT.", CENTER, 0, 7, _TBLACK, _TWHITE);
                           Beep(698, 700);
                           Delay(2);
                           status += 1;
                       }

                       if(status == 0)
                       {
                           StatusBar("DosBEEP was installed successfully. Hit <Esc> to exit.", CENTER, 0, 7, _TBLACK, _TWHITE);
                           while(getch() != 0x1B);
                       }
                       else
                       {
                           char message[80];

                           sprintf(message, "Setup could not copy %d file(s). Hit <Esc> to exit.", status);
                           StatusBar(message, CENTER, 0, 7, _TBLACK, _TWHITE);
                           while(getch() != 0x1B);
                       }
                   }
                   else
                   {
                       StatusBar("Directory does not exist. Do you want to create it ? (y/n)", CENTER, 0, 7, _TBLACK, _TWHITE);
                       while((ch = getch()) != 0)
                       {
                           if(tolower(ch) == 'y')
                           {
                               strcpy(command, CntlInfo.pCntlBuffer);

                               if ( command[strlen(command) - 1] == '\\')
                                   command[strlen(command) - 1] = 0;

                               StatusBar("Creating directory...", CENTER, 0, 7, _TBLACK, _TWHITE);
                               if(makedir(command) == 0)
                               {
                                   int status = 0;

                                   if ( CntlInfo.pCntlBuffer[strlen(CntlInfo.pCntlBuffer) - 1] != '\\')
                                       strcat(CntlInfo.pCntlBuffer, "\\");

                                   sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "dosbeep.exe");
                                   StatusBar("Copying DOSBEEP.EXE...", CENTER, 0, 7, _TBLACK, _TWHITE);
                                   sprintf(command2, "%s%s", szDrive, "dosbeep.exe");
                                   if(copyfile(command2, command) != 0)
                                   {
                                       StatusBar("Could not copy a DOSBEEP.EXE.", CENTER, 0, 7, _TBLACK, _TWHITE);
                                       Beep(698, 700);
                                       Delay(2);
                                       status += 1;
                                   }

                                   sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "dosbeep.hlp");
                                   StatusBar("Copying DOSBEEP.HLP...", CENTER, 0, 7, _TBLACK, _TWHITE);
                                   sprintf(command2, "%s%s", szDrive, "dosbeep.hlp");
                                   if(copyfile(command2, command) != 0)
                                   {
                                       StatusBar("Could not copy a DOSBEEP.HLP.", CENTER, 0, 7, _TBLACK, _TWHITE);
                                       Beep(698, 700);
                                       Delay(2);
                                       status += 1;
                                   }

                                   sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "dbtsr.exe");
                                   StatusBar("Copying DBTSR.EXE...", CENTER, 0, 7, _TBLACK, _TWHITE);
                                   sprintf(command2, "%s%s", szDrive, "dbtsr.exe");
                                   if(copyfile(command2, command) != 0)
                                   {
                                         StatusBar("Could not copy a DBTSR.EXE.", CENTER, 0, 7, _TBLACK, _TWHITE);
                                         Beep(698, 700);
                                         Delay(2);
                                         status += 1;
                                   }

                                   sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "modemlst.idx");
                                   StatusBar("Copying MODEMLST.IDX...", CENTER, 0, 7, _TBLACK, _TWHITE);
                                   sprintf(command2, "%s%s", szDrive, "modemlst.idx");
                                   if(copyfile(command2, command) != 0)
                                   {
                                       StatusBar("Could not copy a MODEMLST.IDX.", CENTER, 0, 7, _TBLACK, _TWHITE);
                                       Beep(698, 700);
                                       Delay(2);
                                       status += 1;
                                   }

                                   sprintf(command, "%s%s", CntlInfo.pCntlBuffer, "modemlst.dat");
                                   StatusBar("Copying MODEMLST.DAT...", CENTER, 0, 7, _TBLACK, _TWHITE);
                                   sprintf(command2, "%s%s", szDrive, "modemlst.dat");
                                   if(copyfile(command2, command) != 0)
                                   {
                                       StatusBar("Could not copy a MODEMLST.DAT.", CENTER, 0, 7, _TBLACK, _TWHITE);
                                       Beep(698, 700);
                                       Delay(2);
                                       status += 1;
                                   }

                                   if(status == 0)
                                   {
                                       StatusBar("DosBEEP was installed successfully. Hit <Esc> to exit.", CENTER, 0, 7, _TBLACK, _TWHITE);
                                       while(getch() != 0x1B);
                                       break;
                                   }
                                   else
                                   {
                                       char message[80];

                                       sprintf(message, "Setup could not copy %d file(s). Hit <Esc> to exit.", status);
                                       StatusBar(message, CENTER, 0, 7, _TBLACK, _TWHITE);
                                       while(getch() != 0x1B);
                                       break;
                                   }
                               }
                               else
                               {
                                   StatusBar("Could not create this directory.", CENTER, 0, 7, _TBLACK, _TWHITE);
                                   Beep(698, 700);
                                   Delay(2);
                                   break;
                               }
                           }

                           if(tolower(ch) == 'n')
                           {
                               Edit = TRUE;
                               break;
                           }
                       }

                       if(Edit == TRUE)
                       {
                           Edit = FALSE;
                           break;
                       }
                   }
               }
           }
           case ESCAPE:
           {
               _settextcolor( _TWHITE );
               _setbkcolor( _TBLACK );
               _clearscreen( _GCLEARSCREEN );
               _displaycursor( _GCURSORON );
               exit(0);
           }
           break;
       }
   }
}


void Box( short row, short col, int rowLast, int colLast, short fgBorder, long bgBorder, short BorderFlag)
{
    short i;
    char achT[MAXITEM + 2];         /* Temporary array of characters */

    /* Set color and position. */
    _settextposition( row, col );
    _settextcolor( (short) fgBorder );
    _setbkcolor( (long) bgBorder );

    /* Draw box top. */
    if(BorderFlag & BORDER)
    {
       achT[0] = mnuAtrib.chNW;
       memset( achT + 1, mnuAtrib.chEW, colLast );
       achT[colLast + 1] = mnuAtrib.chNE;
       achT[colLast + 2] = 0;
       _outtext( achT );
    }

    /* Draw box sides and center. */
    if(BorderFlag & NO_BORDER)
    {
       memset(achT, 0, sizeof(achT));
       memset( achT, ' ', colLast + 2);

       for( i = 0; i < rowLast; ++i )
       {
           _settextposition( row + i, col );
           _outtext( achT );
       }
    }
    else
    {
       achT[0] = mnuAtrib.chNS;
       memset( achT + 1, ' ', colLast );
       achT[colLast + 1] = mnuAtrib.chNS;
       achT[colLast + 2] = 0;
       for( i = 1; i <= rowLast; ++i )
       {
           _settextposition( row + i, col );
           _outtext( achT );
       }
    }

    /* Draw box bottom. */
    if(BorderFlag & BORDER)
    {
       _settextposition( (short) (row + rowLast + 1), col );
       achT[0] = mnuAtrib.chSW;
       memset( achT + 1, mnuAtrib.chEW, colLast );
       achT[colLast + 1] = mnuAtrib.chSE;
       achT[colLast + 2] = 0;
       _outtext( achT );
    }
}

void   StatusBar(LPSTR string, int nAlignment, int nAlOffset, int nRow, short fgText, long bgFill)
{
   char emptyline[81];

   memset(emptyline, ' ', 80);
   emptyline[80] = 0;
   PrintText(emptyline, LEFT, 0, nRow, fgText, bgFill);
   PrintText(string, nAlignment, nAlOffset, nRow, fgText, bgFill);
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void PrintText(LPSTR string, int nAlignment, int nAlOffset, int nRow, short fgText, long bgFill)
{
   short x = 0;

   _displaycursor( _GCURSOROFF );
   _settextcolor( (short) fgText );
   _setbkcolor( (long) bgFill );

   if(nAlignment & CENTER)
       x = (short) (((80 / 2) - (strlen( (LPCSTR) string) / 2)) + nAlOffset);
   else
       if(nAlignment & LEFT)
           x = (short) (nAlOffset);
       else
           if(nAlignment & RIGHT)
               x = (short) (80 - strlen( (LPCSTR) string) - nAlOffset); 

   _settextposition((short) nRow, x);
   _wrapon(_GWRAPOFF);
   _outtext((LPCSTR) string );
   _settextposition((short) nRow, x);
   _wrapon(_GWRAPON);
}
//---------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------
WORD   ProcessControl(PCONTROL_PARAM pCntlInfo, short fgText, long bgFill, int Flag)
{
   char        ch[2];
   int         Offset = 0;
   char        szBuffer[65];
   BOOL        swit = TRUE;
   short       MenuOn = FALSE;
   short       prevBg;
   short       prevCurs = 0x0607;
   unsigned    uKey;
   int         TextLen = 0;
   BOOL        bInsert = FALSE;
   short       loop = TRUE;

   memset(szBuffer, 0, 65);
   strncpy(szBuffer, pCntlInfo->pCntlBuffer, pCntlInfo->nCntlLength);
   TextLen = strlen(szBuffer);

   // Highlight text when control becomes active

   _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
   _setbkcolor(bgFill);
   prevBg = (short) _setbkcolor(fgText);
   _settextcolor(prevBg);
   _outtext(szBuffer);
   _setbkcolor(prevBg);
   _settextcolor(fgText);
   _outtext(" ]");
   _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));

   if(Flag == HILIGHT)
       return ESCAPE;

   _displaycursor( _GCURSORON );
   
   while((uKey = GetKey( WAIT )) != 0)
   {
      switch(uKey)
      {
         case ALT:
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");

               return ALT;
            }
         break;

         case 0x1B:  /* escape key */
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");

               return ESCAPE;
            }
         break;
         case 0x0D: /* enter key */
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");

               return ENTER;
            }
         break;
         case 0x09:  /* tab key */
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");

               return TAB;
            }
         break;

         case U_DN:
         case U_UP:
         break;

         case 0x020f:  /* Shift-Tab */
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);
               
               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");

               return SHIFT_TAB;
            }
         break;

         case 0x014d: // ->
         {
            char    str[2] = "";
            int     counter;

            _displaycursor( _GCURSOROFF );

            if ( (Offset < (pCntlInfo->nCntlLength)) && (Offset < TextLen) )
            {
                if ( swit == TRUE )
                {
                    _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                    _outtext(szBuffer);
                    _outtext(" ]");
                    _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                }
                swit = FALSE;

                Offset++;
                
                _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                
                for (counter = 0; counter < TextLen; counter++)
                {
                    str[0] = szBuffer[counter];

                    if ( counter == Offset )
                    {
                        prevBg = (short) _setbkcolor(fgText);
                        _settextcolor(prevBg);
                        _outtext(str);
                        _setbkcolor(prevBg);
                        _settextcolor(fgText);
                    }
                    else
                        _outtext(str);
                }
                
                _outtext(" ]");
                _settextposition((short) pCntlInfo->nCntlRow, (short) (Offset + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
            }
            
            _displaycursor( _GCURSORON );
         }
         break;

         case 0x014b: // <-
         {
            if ( Offset > 0 )
            {
                char    str[2] = "";
                int     counter;

                _displaycursor( _GCURSOROFF );

                Offset--;
                
                _settextposition((short) pCntlInfo->nCntlRow, (short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                
                for (counter = 0; counter < TextLen; counter++)
                {
                    str[0] = szBuffer[counter];

                    if ( counter == Offset )
                    {
                        prevBg = (short) _setbkcolor(fgText);
                        _settextcolor(prevBg);
                        _outtext(str);
                        _setbkcolor(prevBg);
                        _settextcolor(fgText);
                    }
                    else
                        _outtext(str);
                }
                
                _outtext(" ]");
                _settextposition((short) pCntlInfo->nCntlRow, (short) (Offset + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
            
                _displaycursor( _GCURSORON );
            }
         }
         break;

         case 0x0153: // delete key
         {
            if ( (Offset == 0) && (swit == TRUE))
            {
                int counter;

                swit = FALSE;

                _displaycursor( _GCURSOROFF );
                
                memset(szBuffer, 0, sizeof(szBuffer));
                for ( counter = 0; counter < TextLen; counter++)
                    szBuffer[counter] = ' ';
                      
                _settextposition( (short) (pCntlInfo)->nCntlRow, (short) 
                     ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset));
                _outtext("[ ");
                _settextposition((short) (pCntlInfo)->nCntlRow,(short) 
                     ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
                _outtext(szBuffer);
                _outtext("  ");
                _settextposition((short) (pCntlInfo)->nCntlRow, (short) 
                     ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
                _outtext(" ]");
                memset(szBuffer, 0, sizeof(szBuffer));

                TextLen = 0;
                _settextposition( (short) (pCntlInfo)->nCntlRow, (short) 
                     ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
                _displaycursor( _GCURSORON );
            }
            else
                if ( (Offset >= 0) && (TextLen > 0) && (Offset < TextLen))
                {
                    char    szBuffer2[65];
                    int     counter;
                    char    str[2] = "";

                    _displaycursor( _GCURSOROFF );

                    memset(szBuffer2, 0, sizeof(szBuffer2));

                    for ( counter = 0; counter < Offset; counter++)
                        szBuffer2[counter] = szBuffer[counter];
                    
                    for ( counter++; counter < TextLen; counter++)
                        szBuffer2[counter - 1] = szBuffer[counter];
                        
                    _settextposition((short) (pCntlInfo)->nCntlRow, (short) 
                        ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset));
                    _outtext("[ ");

                    memset(szBuffer, 0, sizeof(szBuffer));
                    for ( counter = 0; counter < TextLen; counter++)
                        szBuffer[counter] = ' ';

                    _settextposition((short) (pCntlInfo)->nCntlRow, (short) 
                        ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
                    _outtext(szBuffer);
                    _outtext("  ");
                    memset(szBuffer, 0, sizeof(szBuffer));

                    TextLen--;

                    _settextposition((short) (pCntlInfo)->nCntlRow, (short) 
                        ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
                    for (counter = 0; counter < TextLen; counter++)
                    {
                        str[0] = szBuffer2[counter];

                        if ( counter == Offset )
                        {
                            prevBg = (short) _setbkcolor(fgText);
                            _settextcolor(prevBg);
                            _outtext(str);
                            _setbkcolor(prevBg);
                            _settextcolor(fgText);
                        }
                        else
                            _outtext(str);
                    }
                    _outtext(" ]");
                    _settextposition( (short) pCntlInfo->nCntlRow, (short) (Offset + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));

                    if ( TextLen > 0 )
                        strcpy(szBuffer, szBuffer2);

                    _displaycursor( _GCURSORON );
                }
         }
         break;

         case 0x0152: // insert
         {
            if ( bInsert == FALSE )
            {
               prevCurs = _gettextcursor();
               _settextcursor(0x0007);
               bInsert = TRUE;
            }
            else
            {
                _settextcursor(prevCurs);
                bInsert = FALSE;
            }
         }
         break;

         case 0x0147: // home
         {
            int     counter;
            char    str[2];

            str[1] = 0;

            _displaycursor( _GCURSOROFF );
            _settextposition((short) (pCntlInfo)->nCntlRow, (short)
                    ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
            Offset = 0;
            
            for (counter = 0; counter < TextLen; counter++)
            {
                str[0] = szBuffer[counter];

                if ( counter == Offset )
                {
                    prevBg = (short) _setbkcolor(fgText);
                    _settextcolor(prevBg);
                    _outtext(str);
                    _setbkcolor(prevBg);
                    _settextcolor(fgText);
                }
                else
                    _outtext(str);
            }
            _settextposition( (short) (pCntlInfo)->nCntlRow, (short)
                    ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));

            _displaycursor( _GCURSORON );
         }
         break;

         case 0x014F: // end
         {
            int     counter;
            char    str[2];

            str[1] = 0;

            _displaycursor( _GCURSOROFF );

            _settextposition( (short) (pCntlInfo)->nCntlRow, (short)
                    ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
            Offset = TextLen;
            
            for (counter = 0; counter < TextLen; counter++)
            {
                str[0] = szBuffer[counter];

                if ( counter == Offset )
                {
                    prevBg = (short) _setbkcolor(fgText);
                    _settextcolor(prevBg);
                    _outtext(str);
                    _setbkcolor(prevBg);
                    _settextcolor(fgText);
                }
                else
                    _outtext(str);
            }
            _settextposition( (short) (pCntlInfo)->nCntlRow, (short)
                    ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + TextLen + 2));

            swit = FALSE;

            _displaycursor( _GCURSORON );
         }
         break;

         case 0x0149:
         case 0x0151:
         break;

         case 0x08:   /* backspace */
         {
            if(Offset > 0)
            {
                char    szBuffer2[65];
                int     counter;
                char    str[2] = "";

                _displaycursor( _GCURSOROFF );

                memset(szBuffer2, 0, sizeof(szBuffer2));
                
                Offset--;

                for ( counter = 0; counter < Offset; counter++)
                    szBuffer2[counter] = szBuffer[counter];
                
                for ( counter++; counter < TextLen; counter++)
                    szBuffer2[counter - 1] = szBuffer[counter];
                    
                _settextposition( (short) (pCntlInfo)->nCntlRow, (short)
                    ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset));
                _outtext("[ ");

                memset(szBuffer, 0, sizeof(szBuffer));
                for ( counter = 0; counter < TextLen; counter++)
                    szBuffer[counter] = ' ';

                _settextposition( (short) (pCntlInfo)->nCntlRow, (short)
                    ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
                _outtext(szBuffer);
                _outtext("  ");

                TextLen--;

                _settextposition( (short) (pCntlInfo)->nCntlRow,  (short)
                    ((pCntlInfo)->nCntlOffset + (pCntlInfo)->nAlignmentOffset + 2));
                for (counter = 0; counter < TextLen; counter++)
                {
                    str[0] = szBuffer2[counter];

                    if ( counter == Offset )
                    {
                        prevBg = (short) _setbkcolor(fgText);
                        _settextcolor(prevBg);
                        _outtext(str);
                        _setbkcolor(prevBg);
                        _settextcolor(fgText);
                    }
                    else
                        _outtext(str);
                }
                _outtext(" ]");
                _settextposition( (short) pCntlInfo->nCntlRow,  (short) (Offset + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                strcpy(szBuffer, szBuffer2);
                
                _displaycursor( _GCURSORON );
            }
         }
         break;
         default:  /* other characters */
         {
            if((TextLen < pCntlInfo->nCntlLength) || (bInsert == TRUE))
            {
                if((Offset < pCntlInfo->nCntlLength))
                {
                    char    szBuffer2[65];
                    char    str[2] = "";
                    int     counter;
                
                    ch[0] = (char) uKey;
                    ch[1] = 0;

                    _displaycursor( _GCURSOROFF );

                    if ( (swit == TRUE) && (Offset == 0) && (bInsert != TRUE))
                    {
                        _settextposition( (short) pCntlInfo->nCntlRow,  (short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                        memset(szBuffer, 0, sizeof(szBuffer));
                    
                        for ( counter = 0; counter < TextLen; counter++)
                            szBuffer[counter] = ' ';

                        _outtext(szBuffer);
                        _outtext("  ");

                        memset(szBuffer, 0, sizeof(szBuffer));
                        TextLen = 0;
                    
                        _settextposition( (short) pCntlInfo->nCntlRow, (short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                        swit = FALSE;
                    }

                    if ( szBuffer[Offset] == 0 )
                    {
                        szBuffer[Offset] = ch[0];
                        szBuffer[Offset + 1] = 0;
                    }
                    else
                    {
                        memset(szBuffer2, 0, sizeof(szBuffer2));
                    
                        for ( counter = 0; counter < Offset; counter++ )
                            szBuffer2[counter] = szBuffer[counter];


                        szBuffer2[counter] = ch[0];

                        if ( bInsert == TRUE )
                        {
                            for ( counter++; counter < TextLen; counter++ )
                                szBuffer2[counter] = szBuffer[counter];
                        }
                        else
                            for ( ; counter < TextLen; counter++ )
                                szBuffer2[counter + 1] = szBuffer[counter];
                    
                        memset(szBuffer, 0, sizeof(szBuffer));
                        strcpy(szBuffer, szBuffer2);
                    }
                
                    _settextposition( (short) pCntlInfo->nCntlRow,  (short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                
                    if ( (bInsert == TRUE) && (Offset < TextLen) && (TextLen > 0))
                        TextLen--;
                
                    TextLen++;

                    Offset++;
                
                    for (counter = 0; counter < TextLen; counter++)
                    {
                        str[0] = szBuffer[counter];

                        if ( counter == Offset )
                        {
                            prevBg = (short) _setbkcolor(fgText);
                            _settextcolor(prevBg);
                            _outtext(str);
                            _setbkcolor(prevBg);
                            _settextcolor(fgText);
                        }
                        else
                            _outtext(str);
                    }
                
                    _outtext(" ]");
                    _settextposition( (short) pCntlInfo->nCntlRow,  (short) (Offset + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                    _displaycursor( _GCURSORON );
                }
            }
         }
         break;
      }
   }
   
   return ESCAPE;

}
void DrawControls(PCONTROL_PARAM pCntlInfo, int index)
{
   int counter;

   for(counter = 0; counter < index; counter++)
       PrintText((pCntlInfo + counter)->pCntlTitle, (pCntlInfo + counter)->nAlignment,
           (pCntlInfo + counter)->nAlignmentOffset, (pCntlInfo + counter)->nCntlRow, _TBRIGHTWHITE, _TBLUE);
}

unsigned GetKey( int fWait)
{
    unsigned uKey = 0;
    unsigned uShift;
    int loop = TRUE;

    /* If CLEAR_WAIT, drain the keyboard buffer. */
    if( fWait == CLEAR_WAIT )
        while( _bios_keybrd( _KEYBRD_READY ) )
            _bios_keybrd( _KEYBRD_READ );

    /* If NO_WAIT, return 0 if there is no key ready. */
    //if( !fWait && !_bios_keybrd( _KEYBRD_READY ) )
    //    return FALSE;

    while(loop == TRUE)
    {
       if(_bios_keybrd( _KEYBRD_READY ))
       {
           uKey = _bios_keybrd( _KEYBRD_READ );

           /* If low byte is not zero, it's an ASCII key. Check scan code to see
           * if it's on the numeric keypad. If not, clear high byte and return.
           */
           if( uKey & 0x00ff )
               if( (uKey >> 8) < 69 )
                   return( uKey & 0x00ff );

           /* For function keys and numeric keypad, put scan code in low byte
           * and shift state codes in high byte.
           */
           uKey >>= 8;
           uShift = _bios_keybrd( _KEYBRD_SHIFTSTATUS ) & 0x000f;
           switch( uShift )
           {
               case 0:
                   return( 0x0100 | uKey );  /* None (1)    */
               case 1:
               case 2:
               case 3:
                   return( 0x0200 | uKey );  /* Shift (2)   */
               case 4:
                   return( 0x0300 | uKey );  /* Control (3) */
               case 8:
                   return( 0x0400 | uKey );  /* Alt (4)     */
               default:
                   return FALSE;
           }
       }
    }

   return uKey;
}
void ReDrawData(PCONTROL_PARAM pCntlInfo, int index, short fgText, long bgFill)
{
   int counter;
   char szCntlBuffer[65];

   _settextcolor(fgText);
   _setbkcolor(bgFill);

   for(counter = 0; counter < index; counter++)
   {
      _settextposition( (short) ((pCntlInfo + counter)->nCntlRow), (short) 
         ((pCntlInfo + counter)->nCntlOffset + (pCntlInfo + counter)->nAlignmentOffset));
  
      _outtext("[ ");

      _settextposition( (short) ((pCntlInfo + counter)->nCntlRow), (short) 
         ((pCntlInfo + counter)->nCntlOffset + (pCntlInfo + counter)->nAlignmentOffset + 2));

      memset(szCntlBuffer, 0, sizeof(szCntlBuffer));
      strncpy( szCntlBuffer, (pCntlInfo + counter)->pCntlBuffer, (pCntlInfo + counter)->nCntlLength);
      _outtext((LPSTR) szCntlBuffer);
      _outtext(" ]");
   }
}

//--------------------------------------------------------------------
//
// BOOL CheckForWinbeep()
//
//--------------------------------------------------------------------
BOOL CheckForWinbeep()
{
   char szBuffer[512];
   char szBuffer2[512];

   StatusBar("Searching for WAPI.INI...", CENTER, 0, 7, _TBLACK, _TWHITE);
   if(GetProfileString("WAPI", "INI", NULL, szBuffer2, sizeof(szBuffer2), "WIN.INI") == TRUE)
   {
      if(GetProfileString("Directories", "BinFiles", NULL, szBuffer, sizeof(szBuffer), szBuffer2) == TRUE)
      {
           char command[256];
           char command2[256];
           int status = 0;

           if ( szBuffer[strlen(szBuffer) - 1] != '\\')
               strcat(szBuffer, "\\");

           StatusBar("Copying DOSBEEP.EXE...", CENTER, 0, 7, _TBLACK, _TWHITE);
           sprintf(command, "%s%s", szBuffer, "dosbeep.exe");
           sprintf(command2, "%s%s", szDrive, "dosbeep.exe");
           if(copyfile(command2, command) != 0)
               status += 1;

           StatusBar("Copying DOSBEEP.HLP...", CENTER, 0, 7, _TBLACK, _TWHITE);
           sprintf(command, "%s%s", szBuffer, "dosbeep.hlp");
           sprintf(command2, "%s%s", szDrive, "dosbeep.hlp");
           if(copyfile(command2, command) != 0)
               status += 1;

           StatusBar("Copying DBTSR.EXE...", CENTER, 0, 7, _TBLACK, _TWHITE);
           sprintf(command, "%s%s", szBuffer, "dbtsr.exe");
           sprintf(command2, "%s%s", szDrive, "dbtsr.exe");
           if(copyfile(command2, command) != 0)
               status += 1;

           if(GetProfileString("Directories", "DataFiles", NULL, szBuffer, sizeof(szBuffer), szBuffer2) == TRUE)
           {
               BOOL copyOK = TRUE;

               if ( szBuffer[strlen(szBuffer) - 1] != '\\')
                   strcat(szBuffer, "\\");

               sprintf(command, "%s%s", szBuffer, "modemlst.idx");
               sprintf(command2, "%s%s", szDrive, "modemlst.idx");
               if(access(command, 0) == 0)
               {              
                   struct _find_t fileinfo;
                   struct _find_t fileinfo2;

                   if(_dos_findfirst(command, _A_RDONLY, &fileinfo) == 0)
                   {
                       if(_dos_findfirst(command2, _A_RDONLY, &fileinfo2) == 0)
                       {
                           if( fileinfo2.wr_date < fileinfo.wr_date )
                           {
                               char ch;

                               StatusBar("Do you want to replace the MODEMLST.IDX file ? (Y/N)", CENTER, 0, 7, _TBLACK, _TWHITE);
                               while((ch = getch()) != 0x1B)
                               {
                                   if(tolower(ch) == 'n')
                                   {
                                       copyOK = FALSE;
                                       break;
                                   }
                                   if(tolower(ch) == 'y')
                                       break;
                               }

                               if(ch == 0x1B)
                                   return TRUE;
                           }
                       }
                   }


               }

               if(copyOK)
               {
                   StatusBar("Copying MODEMLST.IDX...", CENTER, 0, 7, _TBLACK, _TWHITE);
                   if(copyfile(command2, command) != 0)
                       status += 1;

                   sprintf(command, "%s%s", szBuffer, "modemlst.dat");
                   sprintf(command2, "%s%s", szDrive, "modemlst.dat");
                   StatusBar("Copying MODEMLST.DAT...", CENTER, 0, 7, _TBLACK, _TWHITE);
                   if(copyfile(command2, command) != 0)
                       status += 1;
               }
           }

           if(status == 0)
           {
               StatusBar("DosBEEP was succesfully installed.", CENTER, 0, 7, _TBLACK, _TWHITE);
               return TRUE;
           }
      }
   }
   return FALSE;
}

//--------------------------------------------------------------------
//
// GetProfileString
//
//--------------------------------------------------------------------
BOOL GetProfileString(LPSTR lpszSection, LPSTR lpszEntry, LPSTR lpszDefault, 
                 LPSTR lpszBuffer, int nBufferLen, LPSTR lpszFileName)
{
   int  wFHandle;
   LPSTR spPath;
   LPSTR ppath;
   char  szFile[128];
   char  szBuffer[256];
   char  szEnvPath[512];

   if((wFHandle = _sopen(lpszFileName, _O_RDONLY, _SH_DENYNO)) != -1)
   {
                
       if(FindString(wFHandle, lpszSection, lpszEntry, 
                  lpszDefault, lpszBuffer, nBufferLen) == TRUE)
       {
           close(wFHandle);
           return TRUE;
       }
       else
       {
           close(wFHandle);
           strncpy(lpszBuffer, lpszDefault, nBufferLen);
           return FALSE;
       }
   }
   else
       if((ppath = getenv("PATH")) != NULL)
       {
           
           strcpy(szEnvPath, ppath);
           spPath = strtok(szEnvPath, ";");

           do
           {
               memset(szBuffer, 0, sizeof(szBuffer));     
               strcpy(szBuffer, spPath);
               spPath = NULL;

               if (szBuffer[strlen(szBuffer) - 1] != '\\')
      	            strcat(szBuffer, "\\");

               sprintf(szFile, "%s%s", szBuffer, lpszFileName);
               if((wFHandle = _sopen(szFile, _O_RDONLY, _SH_DENYNO)) != -1)
               {

                   if(FindString(wFHandle, lpszSection, lpszEntry, 
                       lpszDefault, lpszBuffer, nBufferLen) == TRUE)
                   {
                       close(wFHandle);
                       return TRUE;
                   }
                   else
                   {
                       close(wFHandle);
                       strncpy(lpszBuffer, lpszDefault, nBufferLen);
                       return FALSE;
                   }
               }
           }
           while((spPath = strtok(NULL, ";")) != NULL);
       }

   return FALSE;
}

//--------------------------------------------------------------------
//
// FindString
//
//--------------------------------------------------------------------
BOOL FindString(int wFHandle, LPSTR lpszSection, LPSTR lpszEntry, 
                LPSTR lpszDefault, LPSTR lpszBuffer, int nBufferLen)
{
   FILE    *fp;
   char    szBuffer[512];
   char    *temp;
   char    *filebuff;
   long    filelength;
   BOOL    DoLine = FALSE;
   char    *curroffset;
   BOOL    ret = FALSE;

   if((fp = _fdopen(wFHandle, "r")) != NULL)
   {
       filelength = _filelength(wFHandle);

       if(filelength < 10000)
       {
           if((filebuff = (char *) malloc((filelength + 50) * sizeof(char))) != NULL)
           {
               // preset to default
               strncpy(lpszBuffer, lpszDefault, nBufferLen - 1);

               fread(filebuff, sizeof(char), filelength, fp);
               sprintf(szBuffer, "[%s]", lpszSection);
               if((curroffset = strstr(filebuff, szBuffer)) != NULL)
               {
                   sprintf(szBuffer, "%s=", lpszEntry);
                   if((curroffset = strstr(curroffset, szBuffer)) != NULL)
                       if(strtok(curroffset, "=") != NULL)
                       {
                           if((curroffset = strtok(NULL, "\n")) != NULL)
                           {
                               strncpy(lpszBuffer, curroffset, nBufferLen - 1);
                               ret = TRUE;
                           }
                       }
               }

               free(filebuff);
               fclose(fp);
               return ret;
           }
           else
               DoLine = TRUE;
       }
       else
           DoLine = TRUE;

       if(DoLine == TRUE)
       {
           while(fgets(szBuffer, sizeof(szBuffer), fp) != NULL)
           {
               if(strstr(szBuffer, lpszSection) != NULL)
               {
                   memset(szBuffer, 0, sizeof(szBuffer));
               
                   while(fgets(szBuffer, sizeof(szBuffer), fp) != NULL)
                   {
                       if(strstr(szBuffer, lpszEntry) != NULL)
                       {
                           temp = strtok(szBuffer, "=");
                           temp = strtok(NULL, "=\n");
                           strncpy(lpszBuffer, temp, nBufferLen - 1);
                           fclose(fp);
                           return TRUE;
                       }
                   }
               }

           }
       }
   }

   strncpy(lpszBuffer, lpszDefault, nBufferLen - 1);
                       
   fclose(fp);
   return FALSE;
}

int copyfile( char *source, char *target )
{
    char *buf;
    int hsource, htarget, ch;
    unsigned count = 0xff00;

    /* Open source file and create target, overwriting if necessary. */
    if( (hsource = _open( source, _O_BINARY | _O_RDONLY )) == - 1 )
        return errno;
    htarget = _open( target, _O_BINARY | _O_WRONLY | _O_CREAT | _O_EXCL,
                             _S_IREAD | _S_IWRITE );
    if( errno == EEXIST )
    {
        htarget = _open( target, _O_BINARY | _O_WRONLY | _O_CREAT |
                                     _O_TRUNC, _S_IREAD | _S_IWRITE );
    }
    if( htarget == -1 )
        return errno;

    if( (unsigned)_filelength( hsource ) < count )
        count = (int)_filelength( hsource );

    /* Dynamically allocate a large file buffer. If there's not enough
     * memory for it, find the largest amount available on the near heap
     * and allocate that. This can't fail, no matter what the memory model.
     */
    if( (buf = (char *)malloc( (size_t)count )) == NULL )
    {
        count = _memmax();
        if( (buf = (char *)malloc( (size_t)count )) == NULL )
            return ENOMEM;
    }

    /* Read-write until there's nothing left. */
    while( !_eof( hsource ) )
    {
        /* Read and write input. */
        if( (count = _read( hsource, buf, count )) == -1 )
            return errno;
        if( (count = _write( htarget, buf, count )) == - 1 )
            return errno;
    }

    /* Close files and release memory. */
    _close( hsource );
    _close( htarget );
    free( buf );
    return 0;
}
int makedir(char *dir)
{
   char path[256];
   char *curr;

   curr = strtok(dir, "\\ ");
   strcpy(path, curr);
   while((curr = strtok(NULL, "\\ ")) != NULL)
   {
       strcat(path, "\\");
       strcat(path, curr);
       if(_mkdir(path) == -1)
           if(errno != EACCES)
               return -1;
   }

   return 0;
}


//--------------------------------------------------------------------
//
// WriteString
//
//--------------------------------------------------------------------
BOOL WriteString(LPSTR szFileName, int *wFHandle, LPSTR lpszSection, LPSTR lpszEntry, LPSTR szString)
{
   char *pszBuffer;
   char *pStrOff;
   char *pStrEnd;
   char szNewEntry[512];
   int  size;
   char temp[255];

   size = (int) (_filelength(*wFHandle) + 10);

   if((pszBuffer = (char *) malloc(size * (sizeof(char)) ) ) != NULL)
   {
       memset(pszBuffer, 0, size);

       size = _read(*wFHandle, (void *) pszBuffer, size );
       close(*wFHandle);
   
       if(((*wFHandle) = _sopen(szFileName, _O_TRUNC|_O_WRONLY, _SH_DENYNO)) != -1)
       {
           if(strcmp(lpszSection, "") != 0)
           {
               sprintf(temp, "[%s]", lpszSection);
               if((pStrOff = strstr(pszBuffer, temp)) != NULL)
               {
                     pStrOff = strstr(pStrOff, "]");
                     if((pStrOff = strstr(pStrOff, lpszEntry)) != NULL)
                     {
                        pStrEnd = strchr(pStrOff, '\n') + 1;    

                        _write(*wFHandle, (void *) pszBuffer, (pStrOff - pszBuffer));
                        sprintf(szNewEntry, "%s=%s\n", lpszEntry, szString);
                        _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
                        _write(*wFHandle, (void *) (pszBuffer + (pStrEnd - pszBuffer)), (size - (pStrEnd - pszBuffer)));

                        return TRUE;
                     }
                     else
                     {
                        sprintf(temp, "[%s]", lpszSection);
                        if((pStrOff = strstr(pszBuffer, temp)) != NULL)
                        {
                           pStrEnd = strchr(pStrOff, '\n') + 1;    

                           _write(*wFHandle, (void *) pszBuffer, (pStrEnd - pszBuffer));
                           sprintf(szNewEntry, "%s=%s\n", lpszEntry, szString);
                           _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
                           _write(*wFHandle, (void *) (pszBuffer + (pStrEnd - pszBuffer)), (size - (pStrEnd - pszBuffer)));
               
                           return TRUE;
                        }
                     }
               }
               else
               {
                     _write(*wFHandle, (void *) pszBuffer, size);
                     sprintf(szNewEntry, "\n[%s]\n", lpszSection);
                     _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
                     sprintf(szNewEntry, "%s=%s\n", lpszEntry, szString);
                     _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
       
                     return TRUE;
               }
           }
           else
           {
               if((pStrOff = strstr(pszBuffer, lpszEntry)) != NULL)
               {
                  pStrEnd = strchr(pStrOff, '\n') + 1;    

                  _write(*wFHandle, (void *) pszBuffer, (pStrOff - pszBuffer));
                  sprintf(szNewEntry, "%s=%s\n", lpszEntry, szString);
                  _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
                  _write(*wFHandle, (void *) (pszBuffer + (pStrEnd - pszBuffer)), (size - (pStrEnd - pszBuffer)));

                  return TRUE;
               }
               else
               {
                  _write(*wFHandle, (void *) pszBuffer, size);
                  sprintf(szNewEntry, "%s=%s\n", lpszEntry, szString);
                  _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));

               }
           }
       }
   }
   
   return FALSE;
}

//--------------------------------------------------------------------
//
// WriteProfileString
//
//--------------------------------------------------------------------
BOOL WriteProfileString(LPSTR szSection, LPSTR szEntry, LPSTR szString,
                        LPSTR szFileName)
{
   int  wFHandle;
   LPSTR spPath, ppath;
   char  szFile[128];
   char  szBuffer[256];
   char  szEnvPath[512];

   if((wFHandle = _sopen(szFileName, _O_CREAT|_O_RDWR, _SH_DENYNO, _S_IREAD|_S_IWRITE)) != -1)
   {
       if(WriteString(szFileName, &wFHandle, szSection, szEntry, szString) == TRUE)
       {
           close(wFHandle);
           return TRUE;
       }
       else
       {
           close(wFHandle);
           return FALSE;
       }
   }
   else
       if((ppath = getenv("PATH")) != NULL)
       {
           
           strcpy(szEnvPath, ppath);
           spPath = strtok(szEnvPath, ";");

           do
           {
               memset(szBuffer, 0, sizeof(szBuffer));     
               strcpy(szBuffer, spPath);
               spPath = NULL;

               if (szBuffer[strlen(szBuffer) - 1] != '\\')
      	            strcat(szBuffer, "\\");

               sprintf(szFile, "%s%s", szBuffer, szFileName);
               if((wFHandle = _sopen(szFile, _O_CREAT|_O_RDWR, _SH_DENYNO, _S_IREAD|_S_IWRITE)) != -1)
               {

                   if(WriteString(szFileName, &wFHandle, szSection, szEntry, szString) == TRUE)
                   {
                       close(wFHandle);
                       return TRUE;
                   }
                   else
                   {
                       close(wFHandle);
                       return FALSE;
                   }
               }

           }
           while((spPath = strtok(NULL, ";")) != NULL);
       }

   return FALSE;
}                        
