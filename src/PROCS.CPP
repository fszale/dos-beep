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
DosBEEPINFO        BeepInfo;

//----------------------------------------------------------------
//
//       WORD ProcessControl(PCONTROL_PARAM pCntlInfo)
//
//----------------------------------------------------------------
WORD   ProcessControl(PCONTROL_PARAM pCntlInfo, short fgText, long bgFill, int Flag)
{
   char        ch[2];
   int         Offset = 0;
   char        szBuffer[65];
   BOOL        swit = TRUE;
   short       MenuOn = FALSE;
   short       prevBg;
   short       prevCurs = 0x0607;
   int         uKey;
   int         TextLen = 0;
   BOOL        bInsert = FALSE;
   short       loop = TRUE;
   int         fl = 0;

   memset(szBuffer, 0, 65);
   strncpy(szBuffer, pCntlInfo->pCntlBuffer, pCntlInfo->nCntlLength);
   TextLen = strlen(szBuffer);

   // Highlight text when control becomes active

   _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
   _setbkcolor(bgFill);
   prevBg = (short) _setbkcolor(fgText);
   _settextcolor(prevBg);
   mouse(2, 0, 0, 0);
   _outtext(szBuffer);
   mouse(1, 0, 0, 0);
   _setbkcolor(prevBg);
   _settextcolor(fgText);
   mouse(2, 0, 0, 0);
   _outtext(" ]");
   mouse(1, 0, 0, 0);
   _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));

   if(Flag == HILIGHT)
       return ESCAPE;

   _displaycursor( _GCURSORON );
   
   sf = 1;
   while((uKey = GetKey( WAIT, &fl)) != -9)
   {
      if( CurrMenu >= 0 )
      {
         _displaycursor( _GCURSOROFF );
          _settextcursor(prevCurs);

         strcpy(pCntlInfo->pCntlBuffer, szBuffer);
         _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
         mouse(2, 0, 0, 0);
         _outtext("[ ");
         _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
         _outtext(szBuffer);
         _outtext(" ]");
         mouse(1, 0, 0, 0);

         return CurrMenu;
      }

      switch(uKey)
      {
         case ALT:
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               mouse(2, 0, 0, 0);
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");
               mouse(1, 0, 0, 0);

               return ALT;
            }
         break;

         case 0x1B:  /* escape key */
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               mouse(2, 0, 0, 0);
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");
               mouse(1, 0, 0, 0);

               return ESCAPE;
            }
         break;
         case 0x0D: /* enter key */
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               mouse(2, 0, 0, 0);
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");
               mouse(1, 0, 0, 0);

               return ENTER;
            }
         break;
         case 0x09:  /* tab key */
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               mouse(2, 0, 0, 0);
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");
               mouse(1, 0, 0, 0);

               return TAB;
            }
         break;

         case U_DN:
         case U_UP:
           {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);

               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               mouse(2, 0, 0, 0);
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");
               mouse(1, 0, 0, 0);
           
               return LIST;
           }
         break;

         case 0x020f:  /* Shift-Tab */
            {
               _displaycursor( _GCURSOROFF );
                _settextcursor(prevCurs);
               
               strcpy(pCntlInfo->pCntlBuffer, szBuffer);
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset));
               mouse(2, 0, 0, 0);
               _outtext("[ ");
               _settextposition((short) pCntlInfo->nCntlRow,(short) (pCntlInfo->nCntlOffset + pCntlInfo->nAlignmentOffset + 2));
               _outtext(szBuffer);
               _outtext(" ]");
               mouse(1, 0, 0, 0);

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
                     mouse(2, 0, 0, 0);
                    _outtext(szBuffer);
                    _outtext(" ]");
                     mouse(1, 0, 0, 0);
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
                        mouse(2, 0, 0, 0);
                        _outtext(str);
                        mouse(1, 0, 0, 0);
                        _setbkcolor(prevBg);
                        _settextcolor(fgText);
                    }
                    else
                    {
                        mouse(2, 0, 0, 0);
                        _outtext(str);
                        mouse(1, 0, 0, 0);
                    }
                }
                
                mouse(2, 0, 0, 0);
                _outtext(" ]");
                mouse(1, 0, 0, 0);

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
           if((uKey >= 32) && (uKey <= 126 ))
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
         }
         break;
      }
   }
   
   return ESCAPE;

}
