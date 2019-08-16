#include <string.h>
#include <dos.h>
#include <direct.h>
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

int putvid(int left, int top, int right, int bottom, char *buffer);
extern BOOL            AltKey;
extern char            message[80];
extern char            szPrinter[2];
extern struct videoconfig vc;
extern WAPIINFO        WapiInfo;
extern DosBEEPINFO     BeepInfo;
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
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
       x = (short) (((80 / 2) - (strlen(string) / 2)) + nAlOffset);
   else
       if(nAlignment & LEFT)
           x = (short) (nAlOffset);
       else
           if(nAlignment & RIGHT)
               x = (short) (80 - strlen(string) - nAlOffset); 

   if(strlen(string) > 80)
       string[80] = 0;


   _settextposition((short) nRow, x);
   _wrapon(_GWRAPOFF);
   if(strcmp(string, "") == 0)
   {
       mouse(2, 0, 0, 0);
       _outtext(" ");
       mouse(1, 0, 0, 0);
   }
   else
   {
       mouse(2, 0, 0, 0);
       _outtext(string);
       mouse(1, 0, 0, 0);
   }
   _settextposition((short) nRow, x);
   _wrapon(_GWRAPON);
}

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void   DrawMultControls(PMULT_CONTROL_PARAM pCntlInfo, int index)
{
   int counter;

   for(counter = 0; counter < index; counter++)
      PrintText((pCntlInfo + counter)->pCntlTitle, (pCntlInfo + counter)->nAlignment,
                (pCntlInfo + counter)->nAlignmentOffset, (pCntlInfo + counter)->nCntlRow, _TWHITE, _TBLUE);
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void FreeControl(PCONTROL_PARAM pCntlInfo, int index)
{
   int counter;

   for(counter = 0; counter < index; counter++)
   {
      if ( (pCntlInfo + counter)->pCntlBuffer != NULL )
         (pCntlInfo + counter)->pCntlBuffer[0] = 0;
   }
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void   FreeMultilineControl(PMULT_CONTROL_PARAM pCntlInfo, int index)
{
   int counter;

   for(counter = 0; counter < index; counter++)
   {
      if (strcmp((pCntlInfo + counter)->pMultiBuffer, "") != 0 )
      {
         strcpy((pCntlInfo + counter)->pMultiBuffer, "");
      }
   }
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void ConvertParams(PVOID pBuffer, DWORD source, WORD wFlag)
{
   switch(wFlag)
      {
       case DOS_BAUD:
         {
            switch(source)
               {
                case CBR_110:
                  strcpy((LPSTR) pBuffer, "110");
                break;
                case CBR_300:
                  strcpy((LPSTR) pBuffer, "300");
                break;
                case CBR_600:
                  strcpy((LPSTR) pBuffer, "600");
                break;
                case CBR_1200:
                  strcpy((LPSTR) pBuffer, "1200");
                break;
                case CBR_2400:
                  strcpy((LPSTR) pBuffer, "2400");
                break;
                case CBR_4800:
                  strcpy((LPSTR) pBuffer, "4800");
                break;
                case CBR_9600:
                  strcpy((LPSTR) pBuffer, "9600");
                break;
                default:
                  strcpy((LPSTR) pBuffer, "300");
                break;
               }
         }
       break;
       case DOS_PARITY:
         {
            switch(source)
               {
                  case NOPARITY:
                     strcpy((LPSTR) pBuffer, "N");
                  break;
                  case ODDPARITY:
                     strcpy((LPSTR) pBuffer, "O");
                  break;
                  case EVENPARITY:
                     strcpy((LPSTR) pBuffer, "E");
                  break;
                  case MARKPARITY:
                     strcpy((LPSTR) pBuffer, "M");
                  break;
                  case SPACEPARITY:
                     strcpy((LPSTR) pBuffer, "S");
                  break;
                  default:
                     strcpy((LPSTR) pBuffer, "E");
                  break;
               }
         }
       break;
       case DOS_STOPBITS:
         {
            switch(source)
               {
               case ONESTOPBIT:
                    strcpy((LPSTR) pBuffer, "1");
               break;
               case ONE5STOPBITS:
                    strcpy((LPSTR) pBuffer, "1.5");
               break;
               case TWOSTOPBITS:
                    strcpy((LPSTR) pBuffer, "2");
               break;
               default:
                    strcpy((LPSTR) pBuffer, "1");
               break;
               }
         }
      break;
      case DOS_DATABITS:
         {
            switch(source)
            {
               case 8:
                  strcpy((LPSTR) pBuffer, "8");
               break;
               case 7:
                  strcpy((LPSTR) pBuffer, "7");
               break;
               case 6:
                  strcpy((LPSTR) pBuffer, "6");
               break;
               case 5:
                  strcpy((LPSTR) pBuffer, "5");
               break;
               default:
                  strcpy((LPSTR) pBuffer, "7");
               break;
            }
         }
      break;
      case DOS_COMMPORT:
       {
           switch(source)
           {
               case 1:
                  strcpy((LPSTR) pBuffer, "1");
               break;
               case 2:
                  strcpy((LPSTR) pBuffer, "2");
               break;
               case 3:
                  strcpy((LPSTR) pBuffer, "3");
               break;
               case 4:
                  strcpy((LPSTR) pBuffer, "4");
               break;
               default:
                  strcpy((LPSTR) pBuffer, "1");
               break;
            }
       }
      break;
      }
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void NotifyUser(LPSTR message, WORD row, WORD column)
{
   if(!message)
      return;

   if(strlen(message) < 24)
   {
      Box( row - 1, column - 1, 5, 28, _TBLACK, _TWHITE, BORDER);
      Box( row, column, 3, 26, _TBLACK, _TWHITE, BORDER);
   }
   else
   {
      Box( row - 1, column - 1, 5, strlen(message) + 4, _TBLACK, _TWHITE, BORDER);
      Box( row, column, 3, strlen(message) + 2, _TBLACK, _TWHITE, BORDER);
   }
   PrintText(message, LEFT, column, row + 1, _TWHITE, _TBLUE);

   while(_getch() != 0x0D);
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void BackConvertParams(PCONTROL_PARAM pCntlInfo, PVOID dest, WORD wFlag)
{
   switch(wFlag)
      {
       case DOS_BAUD:
         {
            if(strcmp((pCntlInfo->pCntlBuffer), "110") == 0)
            {
               *((short *) dest) = CBR_110;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "300") == 0)
            {
               *((short *) dest) = CBR_300;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "600") == 0)
            {
               *((short *) dest) = CBR_600;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "1200") == 0)
            {
               *((short *) dest) = CBR_1200;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "2400") == 0)
            {
               *((short *) dest) = CBR_2400;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "4800") == 0)
            {
               *((short *) dest) = CBR_4800;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "9600") == 0)
            {
               *((short *) dest) = CBR_9600;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "14400") == 0)
            {
               *((short *) dest) = CBR_14400;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "19200") == 0)
            {
               *((short *) dest) = CBR_19200;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "38400") == 0)
            {
               *((short *) dest) = CBR_38400;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "56000") == 0)
            {
               *((short *) dest) = CBR_56000;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "128000") == 0)
            {
               *((short *) dest) = CBR_128000;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "256000") == 0)
            {
               *((short *) dest) = CBR_256000;
               return;
            }

            *((short *) dest) = CBR_300;
            return;
         }
       break;
       case DOS_PARITY:
         {
            if(strcmp((pCntlInfo->pCntlBuffer), "E") == 0)
            {
               *((BYTE *) dest) = EVENPARITY;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "N") == 0)
            {
               *((BYTE *) dest) = NOPARITY;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "O") == 0)
            {
               *((BYTE *) dest) = ODDPARITY;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "M") == 0)
            {
               *((BYTE *) dest) = MARKPARITY;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "S") == 0)
            {
               *((BYTE *) dest) = SPACEPARITY;
               return;
            }
            
            *((BYTE *) dest) = EVENPARITY;
            return;
         }
       break;
       case DOS_STOPBITS:
         {
            if(strcmp((pCntlInfo->pCntlBuffer), "  1") == 0)
            {
               *((BYTE *) dest) = ONESTOPBIT;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "1.5") == 0)
            {
               *((BYTE *) dest) = ONE5STOPBITS;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "  2") == 0)
            {
               *((BYTE *) dest) = TWOSTOPBITS;
               return;
            }
               
            *((BYTE *) dest) = ONESTOPBIT;
            return;
         }
      break;
         case DOS_DATABITS:
         {
            if(strcmp((pCntlInfo->pCntlBuffer), "8") == 0)
            {
               *((BYTE *) dest) = 8;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "7") == 0)
            {
               *((BYTE *) dest) = 7;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "6") == 0)
            {
               *((BYTE *) dest) = 6;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "5") == 0)
            {
               *((BYTE *) dest) = 5;
               return;
            }

            *((BYTE *) dest) = 7;
            return;
         }
      break;
         case DOS_COMMPORT:
         {
            if(strcmp((pCntlInfo->pCntlBuffer), "8") == 0)
            {
               *((BYTE *) dest) = 8;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "7") == 0)
            {
               *((BYTE *) dest) = 7;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "6") == 0)
            {
               *((BYTE *) dest) = 6;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "5") == 0)
            {
               *((BYTE *) dest) = 5;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "4") == 0)
            {
               *((BYTE *) dest) = 4;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "3") == 0)
            {
               *((BYTE *) dest) = 3;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "2") == 0)
            {
               *((BYTE *) dest) = 2;
               return;
            }
            if(strcmp((pCntlInfo->pCntlBuffer), "1") == 0)
            {
               *((BYTE *) dest) = 1;
               return;
            }

            *((BYTE *) dest) = 2;
            return;
         }
      break;
      }
}
