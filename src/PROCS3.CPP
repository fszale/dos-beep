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

extern struct videoconfig vc;
extern WAPIINFO WapiInfo;
extern int          sf;



//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
WORD ProcessMultiLineControl(PMULT_CONTROL_PARAM pCntlInfo, int LinesNumber, PCONTROL_PARAM pTextLenCntl, short fgText, short bgFill, int count)
{
   int         uKey;
   int         CurrentLine = 0;
   int         TextLength = 0;
   int         CursorPosition = 0;
   char        ch[2];
   int         LinePosition = 0;
   char        *TextBuffer;
   short       prevCurs = 0x0707;
   BOOL        Overwrite = FALSE;
   struct _rccoord prevPos;
   int         fl = 0;

   if((TextBuffer = (char *) calloc((pCntlInfo->nCntlLength * pCntlInfo->nCntlLines) + 1, sizeof(char))) == NULL)
       return FALSE;
   
   if(strcmp(pCntlInfo->pMultiBuffer, "") != 0)
   {
       strncpy(TextBuffer, pCntlInfo->pMultiBuffer, pCntlInfo->nCntlLength * pCntlInfo->nCntlLines);
       TextLength = strlen(TextBuffer);
   }

   _settextcolor( (short) fgText );
   _setbkcolor( (long) bgFill );

   itoa(TextLength + count, pTextLenCntl->pCntlBuffer, 10);
   _settextposition( (short) pTextLenCntl->nCntlRow, (short) 
       (pTextLenCntl->nCntlOffset + pTextLenCntl->nAlignmentOffset + 2));
   _outtext(pTextLenCntl->pCntlBuffer);
   _outtext(" ] ");

   ch[1] = 0;

   _displaycursor( _GCURSOROFF );
   _settextposition((short) (pCntlInfo->nCntlRow + CurrentLine), (short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
   _displaycursor( _GCURSORON );


   sf = 1;
   while((uKey = GetKey( WAIT, &fl)) != -9)
   {
       if( CurrMenu >= 0 )
       {
          memset(pCntlInfo->pMultiBuffer, 0, pCntlInfo->nCntlLength * pCntlInfo->nCntlLines);
          strcpy(pCntlInfo->pMultiBuffer, TextBuffer);
          free(TextBuffer);
          return CurrMenu;
       }

       switch(uKey)
       {
           case ALT:
           {
               memset(pCntlInfo->pMultiBuffer, 0, pCntlInfo->nCntlLength * pCntlInfo->nCntlLines);
               strcpy(pCntlInfo->pMultiBuffer, TextBuffer);
               free(TextBuffer);
               return ALT;
           }
           break;

           case 0x1B:  // escape key
           {
               memset(pCntlInfo->pMultiBuffer, 0, pCntlInfo->nCntlLength * pCntlInfo->nCntlLines);
               strcpy(pCntlInfo->pMultiBuffer, TextBuffer);
               free(TextBuffer);
               return ESCAPE;
           }
           break;
           case 0x0D:  // enter key
           {
               memset(pCntlInfo->pMultiBuffer, 0, pCntlInfo->nCntlLength * pCntlInfo->nCntlLines);
               strcpy(pCntlInfo->pMultiBuffer, TextBuffer);
               free(TextBuffer);
               return ENTER;
           }
           break;
           case 0x09:  // tab key 
           {
               memset(pCntlInfo->pMultiBuffer, 0, pCntlInfo->nCntlLength * pCntlInfo->nCntlLines);
               strcpy(pCntlInfo->pMultiBuffer, TextBuffer);
               free(TextBuffer);
               return TAB;
           }
           break;
           case 0x020f:// Shift-Tab 
           {
               memset(pCntlInfo->pMultiBuffer, 0, pCntlInfo->nCntlLength * pCntlInfo->nCntlLines);
               strcpy(pCntlInfo->pMultiBuffer, TextBuffer);
               free(TextBuffer);
               return SHIFT_TAB;
           }
           break;

           case 0x014d:// ->
           {
               if(CursorPosition <= TextLength)
               {
                   if(LinePosition == pCntlInfo->nCntlLength)
                   {
                       if((CurrentLine + 1) < LinesNumber)
                       {
                           CurrentLine++;
                           LinePosition = 0;
                           _settextposition((short) (pCntlInfo->nCntlRow + CurrentLine),(short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                       }
                   }
                   else
                   {
                       LinePosition++;
                       CursorPosition++;
                       _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                   }
               }
           }
           break;

           case 0x014b:// <-
           {
               if(CursorPosition > 0)
               {
                   if(LinePosition == 0)
                   {
                       if((CurrentLine - 1) >= 0)
                       {
                           CurrentLine--;
                           LinePosition = pCntlInfo->nCntlLength;
                           _settextposition((short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                       }
                   }
                   else
                   {
                       LinePosition--;
                       CursorPosition--;
                       _settextposition((short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                   }
               }
           }
           break;

           case 0x0153:// delete key
           {
               if((TextLength > 0) && (CursorPosition < TextLength))
               {
                   {
                       ch[0] = ' ';
                       ch[1] = 0;
                       //LinePosition--;
                   
                       _displaycursor( _GCURSOROFF );
                       _settextposition((short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                       _outtext(ch);
                       TextLength--;

                       prevPos = _gettextposition();

                       itoa(TextLength + count, pTextLenCntl->pCntlBuffer, 10);
                       _settextposition( (short) pTextLenCntl->nCntlRow, (short) 
                           (pTextLenCntl->nCntlOffset + pTextLenCntl->nAlignmentOffset + 2));
                       _outtext(pTextLenCntl->pCntlBuffer);
                       _outtext(" ] ");

                       _settextposition(prevPos.row, prevPos.col); 

                       if(CursorPosition < TextLength)
                       {
                           int     offset;
                           int     index;
                           int     length;
                           int     line = CurrentLine;
                           char    szBuffer[74];

                           TextBuffer[CursorPosition] = ' ';
                           for(index = CursorPosition; index < TextLength; index++)
                           {
                               TextBuffer[index] = TextBuffer[index + 1];
                           }

                           TextBuffer[TextLength] = 0;

                           offset = CursorPosition;
                           length = pCntlInfo->nCntlLength - LinePosition;
                           _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));

                           while(offset < TextLength)
                           {
                               memset(szBuffer, 0, sizeof(szBuffer));
                               strncpy(szBuffer, &TextBuffer[offset], length);
                               _outtext((LPSTR) szBuffer);
                               _outtext((LPSTR) " ");
                               length = pCntlInfo->nCntlLength;
                               offset += strlen(szBuffer);
                               if(++line < LinesNumber)
                                   _settextposition( (short) (pCntlInfo->nCntlRow + line), (short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                               if(offset == TextLength)
                                   _outtext((LPSTR) " ");
                           }
                       }
                       else
                           TextBuffer[TextLength] = 0;

                       _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                       _displaycursor( _GCURSORON );
                   }
               }
           }
           break;

           case 0x0152:// insert
           {
               if ( Overwrite == FALSE )
               {
                   prevCurs = _gettextcursor();
                   _settextcursor(0x0007);
                   Overwrite = TRUE;
               }
               else
               {
                   _settextcursor(prevCurs);
                   Overwrite = FALSE;
               }
           }
           break;

           case 0x0147:// home
           {
               if(CursorPosition > 0)
               {
                   LinePosition = 0;
                   CurrentLine = 0;
                   _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                   CursorPosition = 0;
               }
           }
           break;

           case 0x014F:// end
           {
               if(CursorPosition < TextLength)
               {
                   if((LinePosition = (TextLength%pCntlInfo->nCntlLength)) == 0)
                       LinePosition = pCntlInfo->nCntlLength;
                   if((CurrentLine = (WORD) (TextLength / pCntlInfo->nCntlLength)) == LinesNumber)
                       CurrentLine--;
                   _settextposition((short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                   CursorPosition = TextLength;
               }
           }
           break;

           case 0x0148:
           case 0x0149:
           case 0x0150:
           case 0x0151:
           break;

           case 0x08:  // backspace
           {
               if(TextLength > 0)
               {
                   if(LinePosition == 0)
                   {
                       if((CurrentLine - 1) >= 0)
                       {
                           CurrentLine--;
                           LinePosition = pCntlInfo->nCntlLength;
                           _settextposition((short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                       }
                   }
                   else
                   {
                       ch[0] = ' ';
                       ch[1] = 0;
                       LinePosition--;
                   
                       _displaycursor( _GCURSOROFF );
                       _settextposition((short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                       _outtext(ch);
                       TextLength--;

                       prevPos = _gettextposition();

                       itoa(TextLength + count, pTextLenCntl->pCntlBuffer, 10);
                       _settextposition( (short) pTextLenCntl->nCntlRow, (short) 
                           (pTextLenCntl->nCntlOffset + pTextLenCntl->nAlignmentOffset + 2));
                       _outtext(pTextLenCntl->pCntlBuffer);
                       _outtext(" ] ");

                       _settextposition(prevPos.row, prevPos.col); 

                       CursorPosition--;
                       if(CursorPosition < TextLength)
                       {
                           int     offset;
                           int     index;
                           int     length;
                           int     line = CurrentLine;
                           char    szBuffer[74];

                           TextBuffer[CursorPosition] = ' ';
                           for(index = CursorPosition + 1; index <= TextLength; index++)
                           {
                               TextBuffer[index - 1] = TextBuffer[index];
                           }

                           TextBuffer[TextLength] = 0;

                           offset = CursorPosition;
                           length = pCntlInfo->nCntlLength - LinePosition;
                           _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));

                           while(offset < TextLength)
                           {
                               memset(szBuffer, 0, sizeof(szBuffer));
                               strncpy(szBuffer, &TextBuffer[offset], length);
                               _outtext((LPSTR) szBuffer);
                               _outtext((LPSTR) " ");
                               length = pCntlInfo->nCntlLength;
                               offset += strlen(szBuffer);
                               if(++line < LinesNumber)
                                   _settextposition( (short) (pCntlInfo->nCntlRow + line), (short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                               if(offset == TextLength)
                                   _outtext((LPSTR) " ");
                           }
                       }
                       else
                           TextBuffer[TextLength] = 0;

                       _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                       _displaycursor( _GCURSORON );
                   }
               }
           }
           break;

           default:  
           {
           if((uKey >= 32) && (uKey <= 126 ))
           {
               if((TextLength < (pCntlInfo->nCntlLength * pCntlInfo->nCntlLines)) || (Overwrite == TRUE))
               {
                   if(LinePosition == pCntlInfo->nCntlLength)
                   {
                       if((CurrentLine + 1) < LinesNumber)
                       {
                           ch[0] = (char) uKey;
                           ch[1] = 0;
                           CurrentLine++;
                           LinePosition = 0;
                           _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                         
                           if(Overwrite == TRUE)
                           {
                               TextBuffer[CursorPosition] = ch[0];
                           }
                           else
                           {
                               TextBuffer[TextLength] = ch[0];
                               TextLength++;
                           }
                           CursorPosition++;
                           LinePosition++;
                           _outtext(ch);
                       }
                   }
                   else
                   {
                       ch[0] = (char) uKey;
                       ch[1] = 0;
                       LinePosition++;

                       _outtext(ch);

                       if(Overwrite == TRUE)
                       {
                           TextBuffer[CursorPosition] = ch[0];
                       }
                       else
                       {
                           int     offset;
                           int     index;
                           int     length;
                           int     line = CurrentLine;
                           char    szBuffer[74];
                           
                           TextLength++;

                           prevPos = _gettextposition();

                           itoa(TextLength + count, pTextLenCntl->pCntlBuffer, 10);
                           _settextposition( (short) pTextLenCntl->nCntlRow, (short) 
                               (pTextLenCntl->nCntlOffset + pTextLenCntl->nAlignmentOffset + 2));
                           _outtext(pTextLenCntl->pCntlBuffer);
                           _outtext(" ] ");

                           _settextposition(prevPos.row, prevPos.col); 

                           if((CursorPosition + 1) < TextLength)
                           {
                               for(index = TextLength; index >= CursorPosition; index--)
                               {
                                   TextBuffer[index] = TextBuffer[index - 1];
                               }
                                  
                               TextBuffer[CursorPosition] = ch[0];
                               TextBuffer[TextLength] = 0;

                               _displaycursor( _GCURSOROFF );
                               offset = CursorPosition;
                               length = pCntlInfo->nCntlLength - LinePosition + 1;
                               _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2 - 1));

                               while(offset < TextLength)
                               {
                                   memset(szBuffer, 0, sizeof(szBuffer));
                                   strncpy(szBuffer, &TextBuffer[offset], length);
                                   _outtext((LPSTR) szBuffer);
                                   length = pCntlInfo->nCntlLength;
                                   offset += strlen(szBuffer);
                                   if((offset) < TextLength)
                                       _settextposition( (short) (pCntlInfo->nCntlRow + (++line)), (short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                               }
                               _displaycursor( _GCURSORON );
                               _settextposition( (short) (pCntlInfo->nCntlRow + CurrentLine), (short) (LinePosition + pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
                           }
                           else
                               TextBuffer[CursorPosition] = ch[0];
                       }

                       CursorPosition++;
                   }
               }
           }
           }
           break;
       }
   }
   
   return TRUE;

}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void ReDrawMultilineData(PMULT_CONTROL_PARAM pCntlInfo, int index, int LinesNumber)
{
   int     counter;
   char    Buffer[74];
   int     length = 0;
   int     index2 = 0;
   int     offset = 0;

   for(counter = 0; counter < index; counter++)
   {
      length = (pCntlInfo + counter)->nCntlLength * (pCntlInfo + counter)->nCntlLines;
      for( index2 = 0; index2 < LinesNumber; index2++)
      {
         _settextposition( (short) ((pCntlInfo + counter)->nCntlRow + index2), (short) 
            ((pCntlInfo + counter)->nCntlOffset + (pCntlInfo + counter)->nAlignmentOffset + 2));

         if(offset <= length)
         {
            memset(Buffer, 0, sizeof(Buffer));
            if(strcmp((pCntlInfo + counter)->pMultiBuffer, "") != 0)
            {
               int tracker;

               for(tracker = 0; tracker < pCntlInfo->nCntlLength; tracker++)
                   Buffer[tracker] = ' ';
               Buffer[tracker] = 0;
               _outtext((LPSTR) Buffer);

               _settextposition( (short) ((pCntlInfo + counter)->nCntlRow + index2), (short) 
                   ((pCntlInfo + counter)->nCntlOffset + (pCntlInfo + counter)->nAlignmentOffset + 2));

               memset(Buffer, 0, sizeof(Buffer));
               strncpy(Buffer, (((pCntlInfo + counter)->pMultiBuffer) + offset), pCntlInfo->nCntlLength);
               _outtext((LPSTR) Buffer);
            }
            offset += pCntlInfo->nCntlLength;
         }
      }
   }
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void ReDrawData(PCONTROL_PARAM pCntlInfo, int index, short fgText, long bgFill)
{
   int counter;
   //char szCntlBuffer[65];

   _displaycursor( _GCURSOROFF );
   _settextcolor(fgText);
   _setbkcolor(bgFill);

   for(counter = 0; counter < index; counter++)
   {
      _settextposition( (short) ((pCntlInfo + counter)->nCntlRow), (short) 
         ((pCntlInfo + counter)->nCntlOffset + (pCntlInfo + counter)->nAlignmentOffset));
  
      _outtext("[ ");

      _settextposition( (short) ((pCntlInfo + counter)->nCntlRow), (short) 
         ((pCntlInfo + counter)->nCntlOffset + (pCntlInfo + counter)->nAlignmentOffset + 2));

      //memset(szCntlBuffer, 0, sizeof(szCntlBuffer));
      //strncpy( szCntlBuffer, (pCntlInfo + counter)->pCntlBuffer, (pCntlInfo + counter)->nCntlLength);
      if(strcmp((pCntlInfo + counter)->pCntlBuffer, "") != 0)
           _outtext((LPSTR) (pCntlInfo + counter)->pCntlBuffer);
      _outtext(" ]");
   }
}


//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void ReDrawCheckData(PCONTROL_PARAM pCntlInfo, int index, short fgText, long bgFill)
{
   int counter;
   //char szCntlBuffer[65];

   _settextcolor(fgText);
   _setbkcolor(bgFill);

   for(counter = 0; counter < index; counter++)
   {
      _settextposition( (short) ((pCntlInfo + counter)->nCntlRow), (short) 
         ((pCntlInfo + counter)->nCntlOffset + (pCntlInfo + counter)->nAlignmentOffset));
  
      _outtext("[");

      _settextposition( (short) ((pCntlInfo + counter)->nCntlRow), (short) 
         ((pCntlInfo + counter)->nCntlOffset + (pCntlInfo + counter)->nAlignmentOffset + 1));

      //memset(szCntlBuffer, 0, sizeof(szCntlBuffer));
      //strncpy( szCntlBuffer, (pCntlInfo + counter)->pCntlBuffer, (pCntlInfo + counter)->nCntlLength);
      if(strcmp((pCntlInfo + counter)->pCntlBuffer, "") != 0)
           _outtext((LPSTR) (pCntlInfo + counter)->pCntlBuffer);
      _outtext("]");
   }
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ListComOptions(LPWAPIMODEMINFO pModemInfo, short row, short col, int maxWidth, short maxEntries) 
{
   ITEM SubList[5];
   int index, offset = NOT_SELECTED;
   int Overflow = 1;
   char *ConnType[5] = {"NONE", "COM 1", "COM 2", "COM 3", "COM 4" };
   char message[30];
   CONTROL_PARAM CntlInfo;
   char szBuffer[4];

   for(index = 0; index < 5; index++)
   {
      SubList[index].iHilite = -1;
      strcpy((LPSTR) SubList[index].achItem, ConnType[index]);
   }
   

   StatusBar("Select connection type.", CENTER, 0, 25, _TBLACK, _TWHITE);
   offset = ListBox( row, col, maxWidth, maxEntries, SubList, index, 0, HILIGHT, NULL);

   if(offset >= 0)
   {
       if( offset == 0 )
       {
           pModemInfo->byCOMMType = COMM_NONE;
           pModemInfo->byCOMMPort = 0;
   
           StatusBar("NONE selected.", CENTER, 0, 25, _TBLACK, _TWHITE);

           return offset;
       }
       else
       {
           pModemInfo->byCOMMType = COMM_LOCAL;
       
           pModemInfo->byCOMMPort = (BYTE) offset;
           itoa(offset, szBuffer, 10);
           strcpy(CntlInfo.pCntlBuffer, szBuffer);
           sprintf((LPSTR) message, "COM %d selected.", (WORD) pModemInfo->byCOMMPort); 
           StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);

           return offset;
       }
   }
   return ESCAPE;
}


//----------------------------------------------------------------
//
//       WORD CheckControl(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
WORD   CheckControl(PCONTROL_PARAM pCntlInfo, short fgText, long bgFill, int )
{
   int         Offset = 0;
   char        szBuffer[65];
   BOOL        swit = TRUE;
   short       MenuOn = FALSE;
   short       prevCurs = 0x0607;
   int         uKey;
   int         TextLen = 0;
   BOOL        bInsert = FALSE;
   short       loop = TRUE;
   BOOL        Selected = FALSE;
   int fl = 0;

   memset(szBuffer, 0, 65);
   strncpy(szBuffer, pCntlInfo->pCntlBuffer, pCntlInfo->nCntlLength);
   TextLen = strlen(szBuffer);

   // Highlight text when control becomes active

   _setbkcolor(bgFill);
   _settextcolor(fgText);
   _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
   if(strcmp(szBuffer, "") == 0)
   {
       _outtext("[ ]");
       strcpy(szBuffer, " ");
       Selected = FALSE;
   }
   else
   if(strcmp(szBuffer, "X") == 0)
   {
       _outtext("[");
       _outtext(szBuffer);
       _outtext("]");
       Selected = TRUE;
   }
   _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 1));

   _displaycursor( _GCURSORON );
   
   while((uKey = GetKey( WAIT, &fl)) != -9)
   {
       if( CurrMenu >= 0 )
       {
          strcpy(pCntlInfo->pCntlBuffer, szBuffer);
          return CurrMenu;
       }

       switch(uKey)
       {
         case ALT:
           strcpy(pCntlInfo->pCntlBuffer, szBuffer);
           return ALT;
         break;
         
         case 0x0D: // enter key
            {
               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               return ENTER;
            }
         break;

         case 0x1B:  // escape key 
            {
               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               return ESCAPE;
            }
         break;
         case 0x09:  // tab key
            {
               _displaycursor( _GCURSOROFF );

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               return TAB;
            }
         break;

         case 0x020f:  // Shift-Tab
            {
               _displaycursor( _GCURSOROFF );
               
               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               return SHIFT_TAB;
            }
         break;

         case U_UP:
         case U_DN:
         case U_LT:
         case U_RT:
         case 0x20:
         {
           if(Selected == TRUE) 
           {
               Selected = FALSE;
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               _outtext("[ ]");
               strcpy(szBuffer, " ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 1));
           }
           else
           {
               Selected = TRUE;
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               _outtext("[X]");
               strcpy(szBuffer, "X");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 1));
           }
         }
         break;
      }
   }
   return ESCAPE;

}


