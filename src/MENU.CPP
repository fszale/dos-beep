/* MENU - Module of functions to put menus on the screen and handle keyboard
 * input. To use it, include the MENU.H file in your program. The following
 * functions are public:
 *
 *   Menu       -   Puts a menu on screen and reads input for it
 *   Box        -   Puts a box on screen (fill it yourself)
 *   GetKey     -   Gets ASCII or function key
 *   _outchar   -   Displays character using current text position and color
 *
 * The following structures are defined:
 *
 *   MENU       -   Defines menu colors, box type, and centering
 *   ITEM       -   Defines text of menu item and index of highlight character
 *
 * The global variable "mnuAtrib" has type MENU. Change this variable to
 * change menu appearance.
 */
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <graph.h>
#include <bios.h>
#include <malloc.h>
#include <stdlib.h>
#include <conio.h>
#include "menu.h"
extern "C"
{
#include "cbtree.h"
}
#include "convert.h"
#include "wapi.h"
#include "dbfile.h"
#include "dosbeep.h"

extern char       message[80];
extern BOOL       Keyboard;
extern char far *flags;

extern BOOL MouseDown;
extern BOOL AltKey;
extern BOOL KeyDown;
int sf = 0;
int OldMenu = -9;

/* Prototype for internal function */
void Itemize( short row, short col, int fCur, ITEM itm, int cBlank , int Length, unsigned Flags);
void ItemizeBar( short row, short col, ITEMBAR aItem[], int MenuCount, int iCur, short fgText, long bgFill, unsigned Hilight );

/* Default menu attribute. The default works for color or B&W. You can
 * override the default value by defining your own MENU variable and
 * assigning it to mnuAtrib, or you can modify specific fields at
 * run time. For example, you could use a different attribute for color
 * than for black and white.
 */

int codes[26][2] = 
{
 { 16, 113},
 { 17, 119},
 { 18, 101},
 { 19, 114},
 { 20, 116},
 { 21, 121},
 { 22, 117},
 { 23, 105},
 { 24, 111},
 { 25, 112},
 { 30, 97},
 { 31, 115},
 { 32, 100},
 { 33, 102},
 { 34, 103},
 { 35, 104},
 { 36, 106},
 { 37, 107},
 { 38, 108},
 { 44, 122},
 { 45, 120},
 { 46, 99},
 { 47, 118},
 { 48, 98},
 { 49, 110},
 { 50, 109}
};

MENU mnuAtrib =
{
    _TBLACK, _TBLACK, _TWHITE, _TBRIGHTWHITE, _TBRIGHTWHITE,
    _TWHITE, _TWHITE, _TBLACK, _TWHITE, _TBLACK,
    TRUE,
    'Ú', '¿', 'Ù', 'À', '³', 'Ä'
};

long MenuBar( short row, short col, ITEMBAR aItem[], int MenuCount, int iCur, short fgText, long bgFill, unsigned Draw, unsigned Hilight )
{
    short CurrColumn = 0;
    int uKey = 0;
    int fl = 1;
    //long cntl;

    _displaycursor( _GCURSOROFF );
    
    _setbkcolor( bgFill );
    _settextcolor( fgText );

    Box( row, col, 1, 78, _TBLACK, _TWHITE, NO_BORDER);

    ItemizeBar( row, col, aItem, MenuCount, iCur, fgText, bgFill, Hilight);

    if(Draw & DRAW_ONLY)
       return ESCAPE;

    CurrMenu = -1;
    //OldMenu = -9;
    while((uKey = GetKey(WAIT, &fl)) != -1)
    {
       if( CurrMenu >= 0 )
          return CurrMenu;

       switch( uKey )
       {
           case ALT:
               return ALT;

           case 0x014d: // ->
               if(iCur == (MenuCount - 1))
                   iCur = 0;
               else
                   iCur++;

               ItemizeBar( row, col, aItem, MenuCount, iCur, fgText, bgFill, Hilight);
           break;

           case 0x014b: // <-
               if(iCur == 0)
                   iCur = (MenuCount - 1);
               else
                   iCur--;

               ItemizeBar( row, col, aItem, MenuCount, iCur, fgText, bgFill, Hilight);
           break;

           case U_UP:
           case U_DN:
           case 0x0d:
               return iCur;

           case 0x1B:
               return ESCAPE;

           case 0x09:
           break;

           default:
           {
               int index, ch, ch2;

               ch = toupper( (int) uKey);

               for(index = 0; index < MenuCount; index++)
               {
                   ch2 = toupper( (int) aItem[index].achItem[aItem[index].iHilite]);

                   if( ch == ch2)
                   {
                       ItemizeBar( row, col, aItem, MenuCount, index, fgText, bgFill, Hilight);
                       return index;
                   }
               }
           }
       }
    }
          
    return -9;
}
/* Menu - Puts menu on screen and reads menu input from keyboard. When a
 * highlighted hot key or ENTER is pressed, returns the index of the
 * selected menu item.
 *
 * Params: row and col - If "fCentered" attribute of "mnuAtrib" is true,
 *           center row and column of menu; otherwise top left of menu
 *         aItem - array of structure containing the text of each item
 *           and the index of the highlighted hot key
 *         iCur - index of the current selection--pass 0 for first item,
 *           or maintain a static value
 *
 * Return: The index of the selected item
 *
 * Uses:   mnuAtrib
 */
long Menu( short row, short col, ITEM aItem[], int iCur, int wFlag)
{
    int cItem, cchItem = 2; /* Counts of items and chars per item       */
    int i, iPrev;           /* Indexes - temporary and previous         */
    int acchItem[MAXITEM];  /* Array of counts of character in items    */
    char *pchT;             /* Temporary character pointer              */
    char achHilite[36];     /* Array for highlight characters           */
    int uKey;          /* Unsigned key code                        */
    long bgColor;           /* Screen color, position, and cursor       */
    short fgColor;
    struct rccoord rc;
    unsigned fCursor;
    int fl = 0;
    //long cntl = 0;
 
    /* Save screen information. */
    fCursor = _displaycursor( _GCURSOROFF );
    bgColor = _getbkcolor();
    fgColor = _gettextcolor();
    rc = _gettextposition();
    
   /* Count items, find longest, and put count of each in array. Also,
     * put the highlighted character from each in a string.
     */
    for( cItem = 0; aItem[cItem].achItem[0]; cItem++ )
    {
        acchItem[cItem] = strlen( aItem[cItem].achItem );
        cchItem = (acchItem[cItem] > cchItem) ? acchItem[cItem] : cchItem;
        i = aItem[cItem].iHilite;
        achHilite[cItem] = aItem[cItem].achItem[i];
    }
    cchItem += 2;
    achHilite[cItem] = 0;          /* Null-terminate and lowercase string  */
    strlwr( achHilite );

    /* Adjust if centered, and draw menu box. */
    if( mnuAtrib.fCentered )
    {
        row -= (short) (cItem / 2);
        col -= (short) (cchItem / 2);
    }
    Box( row++, col++, cItem, cchItem, _TBLACK, _TWHITE, BORDER);

    iPrev = 0;

    /* Put items on menu. */
    for( i = 0; i < cItem; i++ )
    {
        if( i == iCur )
        {
            if(wFlag & NO_HILIGHT)
               Itemize( (short) (row + i), col, TRUE, aItem[i], cchItem - acchItem[i], 0, NO_HILIGHT);
            else
               Itemize( (short) (row + i), col, TRUE, aItem[i], cchItem - acchItem[i], 0, HILIGHT);
            if(iCur != 0)
               iPrev = iCur - 1;
        }
        else
        {
            if(wFlag & NO_HILIGHT)
               Itemize( (short) (row + i), col, FALSE, aItem[i], cchItem - acchItem[i], 0, NO_HILIGHT);
            else
               Itemize( (short) (row + i), col, FALSE, aItem[i], cchItem - acchItem[i], 0, HILIGHT);
        }
    }
    //sf = 1;
    CurrMenu = -1;

    if(wFlag & DRAW_ONLY)
       return ESCAPE;


    //OldMenu = -9;
    while((uKey = GetKey( WAIT, &fl)) != -1)
    {
        if( CurrMenu >= 0 )
           return CurrMenu;

        switch( uKey )
        {
           case ALT:
               return ALT;

           case U_LT:
               return PREVIOUS;
           break;

           case U_RT:
               return NEXT;
           break;

            case U_UP:                      /* Up key       */
                iPrev = iCur;
                  if(aItem[iCur - 1].achItem[0] == '-')
                     {
                        iCur = (iCur > 0) ? ((iCur-=2) % cItem) : cItem - 1;
                     }
                  else
                     {
                        iCur = (iCur > 0) ? (--iCur % cItem) : cItem - 1;
                     }
                break;
            case U_DN:                      /* Down key     */
                iPrev = iCur;
                  if(aItem[iCur + 1].achItem[0] == '-')
                     {
                        iCur = (iCur < cItem) ? ((iCur+=2) % cItem) : 0;
                     }
                  else
                     {
                        iCur = (iCur < cItem) ? (++iCur % cItem) : 0;
                     }
                break;

            case 0x1B:
              _setbkcolor( bgColor );
              _settextcolor( fgColor );
              _settextposition( rc.row, rc.col );
              _displaycursor( (short) fCursor );
              return ESCAPE;
            break;

            case 0x09:
              //_setbkcolor( bgColor );
              //_settextcolor( fgColor );
              //_settextposition( rc.row, rc.col );
              //_displaycursor( (short) fCursor );
              //return TAB;
            break;

            default:
                if( uKey > 256 )            /* Ignore unknown function key  */
                    continue;
                pchT = strchr( achHilite, (char)tolower( uKey ) );
                if( pchT != NULL )         
                {
                   int track;

                   for(track = 0; track < cItem; track++ )
                       Itemize( (short) (row + track), col, FALSE, aItem[track], cchItem - acchItem[track], 0, NO_HILIGHT);

                   //Itemize( (short) (row + iCur), col,
                   //        FALSE, aItem[iCur], cchItem - acchItem[iCur], 0, NO_HILIGHT);
                   iCur = pchT - achHilite;
                   Itemize( (short) (row + iCur), col,
                           TRUE, aItem[iCur], cchItem - acchItem[iCur], 0, NO_HILIGHT);
                }
                else
                    continue;               /* Ignore unknown ASCII key     */

            case 0x0d:
            {
                int track;

                for(track = 0; track < cItem; track++ )
                    Itemize( (short) (row + track), col, FALSE, aItem[track], cchItem - acchItem[track], 0, NO_HILIGHT);
 
                Itemize( (short) (row + iCur), col,
                   TRUE, aItem[iCur], cchItem - acchItem[iCur], 0, NO_HILIGHT);
 
                _setbkcolor( bgColor );
                _settextcolor( fgColor );
                _settextposition( rc.row, rc.col );
                _displaycursor((short) fCursor );
                return iCur;
            }
            break;
        }

        if(uKey != 0x09)
        {
           /* Redisplay current and previous. */
           Itemize( (short) (row + iCur), col,
                   TRUE, aItem[iCur], cchItem - acchItem[iCur], 0, HILIGHT);
           Itemize( (short) (row + iPrev), col,
                   FALSE, aItem[iPrev], cchItem - acchItem[iPrev], 0, HILIGHT);
        }
    }
          
    return ESCAPE;
}
//------------------------------------------------------------
//
//          ListBox function
//
//                                      
//------------------------------------------------------------
int ListBox( short row, short col, int maxWidth, int maxEntries, ITEM aItem[], int EntriesCount, int iCur, unsigned Hilight, char *retBuffer)
{
    int cItem, cchItem = 2;
    int i;
    int acchItem[MAXITEM]; 
    int uKey;         
    long bgColor;          
    short fgColor;
    struct rccoord rc;
    short fCursor;
    int First = 0, Last;
    int off = 0;
    int Highlight = 0;
    int fl = 0;
    //long cntl;

    /* Save screen information. */
    fCursor = _displaycursor( _GCURSOROFF );
    bgColor = _getbkcolor();
    fgColor = _gettextcolor();
    rc = _gettextposition();
    
    cItem = maxEntries;

    Box( row++, col++, maxEntries, maxWidth + 2, _TBLACK, _TWHITE, BORDER);

    if ( maxEntries > EntriesCount )
       if(EntriesCount == 0)
           maxEntries = 1;
       else
           maxEntries = EntriesCount;

    if(iCur > maxEntries)
    {
       if(iCur > (EntriesCount - maxEntries))
       {
           First = (EntriesCount - maxEntries);
           Last = EntriesCount;
       }
       else
       {
           First = (iCur - (iCur%maxEntries));
           Last = (First + maxEntries);
       }
    }
    else
    {
       First = 0;
       Last = maxEntries;
    }

    /* Put items on menu. */
    for( i = 0; i < maxEntries; i++ )
    {
        if(( (i + First) == iCur ) && (Hilight & HILIGHT ))
        {
            //Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth );
            Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem, maxWidth , HILIGHT);
            off = i;
            //off = iCur;
        }
        else
        {
            //Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth );
            Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem, maxWidth , HILIGHT);
        }
    }

    if(Hilight & DRAW_ONLY)
       return ESCAPE;

    while((uKey = GetKey( WAIT, &fl)) != -9)
    {
       if( CurrMenu >= 0 )
           return CurrMenu;

       switch( uKey )
       {
           case ALT:
               return ALT;

           case 0x1B:
               _setbkcolor( bgColor );
               _settextcolor( fgColor );
               _settextposition( rc.row, rc.col );
               _displaycursor((short) fCursor );
               return ESCAPE;
           break;

           case 0x09:
               _setbkcolor( bgColor );
               _settextcolor( fgColor );
               _settextposition( rc.row, rc.col );
               _displaycursor((short) fCursor );
               return TAB;
           break;

           case 0x020f:   //shift & tab
               _setbkcolor( bgColor );
               _settextcolor( fgColor );
               _settextposition( rc.row, rc.col );
               _displaycursor( fCursor );
               return SHIFT_TAB;
           break;

           case HOME:
                 First= 0;
                 Last= maxEntries;
                 iCur= 0;
                 off = 0;

                 for( i = 0; i < maxEntries; i++ )
                 {
                    if(( i == 0 )  && (Hilight & HILIGHT))
                    {
                       Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                    }
                    else
                       Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                 }
           break;

           case END:
                 First= (EntriesCount - maxEntries);
                 Last= EntriesCount;
                 iCur= EntriesCount - 1;
                 off = maxEntries - 1;

                 for( i = 0; i < maxEntries; i++ )
                 {
                    if(( i == (maxEntries - 1) )  && (Hilight & HILIGHT))
                    {
                       Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                    }
                    else
                       Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                 }
           break;

           case PG_UP:
              if ( First > maxEntries )
              {
                 First-= maxEntries;
                 Last-= maxEntries;
                 iCur= (iCur - maxEntries) - off;
                 off = 0;

                 for( i = 0; i < maxEntries; i++ )
                 {
                    if(( i == 0 )  && (Hilight & HILIGHT))
                    {
                       Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                    }
                    else
                       Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                 }
              }
              else
              {
                 First= 0;
                 Last= maxEntries;
                 iCur= 0;
                 off = 0;

                 for( i = 0; i < maxEntries; i++ )
                 {
                    if(( i == 0 )  && (Hilight & HILIGHT))
                    {
                       Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                    }
                    else
                       Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                 }
              }
           break;
           
           case U_UP:                      /* Up key       */

               if ( off >= 1 )
               {
                  Itemize( (short) (row + off), col, FALSE, aItem[iCur], cchItem - acchItem[i] , maxWidth , HILIGHT);
                  off--;
                  if((Hilight & HILIGHT))
                       Itemize( (short) (row + off), col, TRUE, aItem[--iCur], cchItem - acchItem[i] , maxWidth , HILIGHT);
                  else
                       Itemize( (short) (row + off), col, FALSE, aItem[--iCur], cchItem - acchItem[i] , maxWidth , HILIGHT);
               }
               else
                 if(First > 0)
                 {
                    First--;
                    Last--;
                    iCur--;

                    for( i = 0; i < maxEntries; i++ )
                    {
                       if(( i == 0 ) && (Hilight & HILIGHT))
                       {
                          Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                       }
                       else
                          Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                    }
                 }
           break;

           case PG_DN:
              if ( Last < (EntriesCount - maxEntries) )
              {
                 First+= maxEntries;
                 Last+= maxEntries;
                 iCur+= maxEntries - off;
                 off = 0;
                 
                 for( i = 0; i < maxEntries; i++ )
                 {
                    if(( i == 0 )  && (Hilight & HILIGHT))
                    {
                       Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                    }
                    else
                       Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                 }
              }
              else
              {
                 First= (EntriesCount - maxEntries);
                 Last= EntriesCount;
                 iCur= (EntriesCount - maxEntries);
                 off = 0;
                 
                 for( i = 0; i < maxEntries; i++ )
                 {
                    if(( i == 0 ) && (Hilight & HILIGHT)) 
                    {
                       Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                    }
                    else
                       Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                 }
              }
           break;

           case U_DN:  

                 if(off < maxEntries - 1)
                 {
                    Itemize( (short) (row + off), col, FALSE, aItem[iCur], cchItem - acchItem[i] , maxWidth , HILIGHT);
                    off++;
                    if((Hilight & HILIGHT))
                       Itemize( (short) (row + off), col, TRUE, aItem[++iCur], cchItem - acchItem[i] , maxWidth , HILIGHT);
                    else
                       Itemize( (short) (row + off), col, FALSE, aItem[++iCur], cchItem - acchItem[i] , maxWidth , HILIGHT);
                 }
                 else
                    if( Last < EntriesCount )
                    {
                       First++;
                       Last++;
                       iCur++;
                    
                       for( i = 0; i < maxEntries; i++ )
                       {
                          if(( i == ( maxEntries - 1 )  ) && (Hilight & HILIGHT))  /* -1 and == for the last entry */
                          {
                             Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                          }
                          else
                             Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                       }
                    }
           break;
           
           case 0x0d:
               _setbkcolor( bgColor );
               _settextcolor( fgColor );
               _settextposition( rc.row, rc.col );
               _displaycursor((short) fCursor );

               if(Hilight & COPY_SELECTION)
                   strcpy(retBuffer, aItem[iCur].achItem);

                return iCur;
           break;

           default:
           {
              char ch = (char) uKey;
              char lch;
              int index, i;

              ch = (char) toupper((int) ch);

              for ( index = 0; index < EntriesCount; index++ )
              {
                  lch = (char) toupper ((int) aItem[index].achItem[0]);
                  if( lch == ch )
                  {
                      First= index;
                      Last= First + maxEntries;
                      iCur= index;
                      off = 0;
                      Highlight = 0;

                      if ( First < maxEntries )
                      {
                          First= 0;
                          Last= maxEntries;
                          off = Highlight = index;
                      }
                      else
                      if ( index > EntriesCount - (maxEntries * 2) )
                      {
                          if ( First < (EntriesCount - maxEntries) )
                          {
                              First= index;
                              off = Highlight = 0;
                          }
                          else
                          {
                              First= (EntriesCount - maxEntries);
                              off = Highlight = maxEntries - (EntriesCount - index);
                          }
                      }
                          

                      for( i = 0; i < maxEntries; i++ )
                      {
                          if (( i == Highlight )  && (Hilight & HILIGHT))
                          {
                              Itemize( (short) (row + i), col, TRUE, aItem[i + First], cchItem - acchItem[i] , maxWidth , HILIGHT);
                          }
                          else
                              Itemize( (short) (row + i), col, FALSE, aItem[i + First], cchItem - acchItem[i], maxWidth , HILIGHT);
                      }

                      break;
                  }
              }
           }
           break;
       }
    }
    return ESCAPE;
}

/* Box - Draw menu box, filling interior with blanks of the border color.
 *
 * Params: row and col - upper left of box
 *         rowLast and colLast - height and width
 *
 * Return: None
 *
 * Uses:   mnuAtrib
 */
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
       mouse(2, 0, 0, 0);
       _outtext( achT );
       mouse(1, 0, 0, 0);
    }

    /* Draw box sides and center. */
    if(BorderFlag & NO_BORDER)
    {
       memset(achT, 0, sizeof(achT));
       memset( achT, ' ', colLast + 2);

       for( i = 0; i < rowLast; ++i )
       {
           _settextposition( row + i, col );
           mouse(2, 0, 0, 0);
           _outtext( achT );
           mouse(1, 0, 0, 0);
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
           mouse(2, 0, 0, 0);
           _outtext( achT );
           mouse(1, 0, 0, 0);
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
       mouse(2, 0, 0, 0);
       _outtext( achT );
       mouse(1, 0, 0, 0);
    }
}

/* Itemize - Display one selection (item) of a menu. This function
 * is normally only used internally by Menu.
 *
 * Params: row and col - top left of menu
 *         fCur - flag set if item is current selection
 *         itm - structure containing item text and index of highlight
 *         cBlank - count of blanks to fill
 *
 * Return: none
 *
 * Uses:   mnuAtrib
 */
void Itemize( short row, short col, int fCur, ITEM itm, int cBlank , int Length, unsigned Flags)
{
    short i;
    char achT[MAXITEM];             /* Temporary array of characters */

    /* Set text position and color. */
    _settextposition( row, col );
    if( fCur )
    {
        _settextcolor( (short) mnuAtrib.fgSelect );
        _setbkcolor( (long) mnuAtrib.bgSelect );
    }
    else
    {
        _settextcolor( (short) mnuAtrib.fgNormal );
        _setbkcolor( (long) mnuAtrib.bgNormal );
    }

    /* Display item and fill blanks. */
    if( Length > 0 )
    {
      strcpy( achT, " " );
      strncat( achT, itm.achItem, Length );
      strncat(achT, "                                                                          ", Length - strlen(itm.achItem));
      mouse(2, 0, 0, 0);
      _outtext( achT );
      mouse(1, 0, 0, 0);
    }
    else
    {
      strcat( strcpy( achT, " " ), itm.achItem );
      mouse(2, 0, 0, 0);
      _outtext( achT );
      mouse(1, 0, 0, 0);

      memset( achT, ' ', cBlank-- );
      achT[cBlank] = 0;
      mouse(2, 0, 0, 0);
      _outtext( achT );
      mouse(1, 0, 0, 0);

      /* Set position and color of highlight character, then display it. */
       if(Flags & HILIGHT)
       {
           i = (short) itm.iHilite;
           _settextposition( row, (short) (col + i + 1) );
           if( fCur )
           {
               _settextcolor( (short) mnuAtrib.fgSelHilite );
               _setbkcolor( (long) mnuAtrib.bgSelHilite );
           }
           else
           {
               _settextcolor( (short) mnuAtrib.fgNormHilite );
               _setbkcolor( (long) mnuAtrib.bgNormHilite );
           }
           _outchar( itm.achItem[i] );
       }
    }      
}
//---------------------------------------
//
//
//
//---------------------------------------
void ItemizeBar( short row, short col, ITEMBAR aItem[], int MenuCount, int iCur, short fgText, long bgFill, unsigned Hilight )
{
   int index;
   int CurrColumn;
   char str[20];    
   

   for( index = 0; index < MenuCount; index++ )
   {
      if(index == 0)
      {
          CurrColumn = col + 2;
      }
      else
      {
          CurrColumn += aItem[index - 1].iMaxLength;
      }

      _settextposition( (short) (row + 1), (short) CurrColumn );

      if(index == iCur)
      {
           _setbkcolor( (long) fgText );
           _settextcolor( (short) bgFill );
  
           mouse(2, 0, 0, 0);
           _outtext(" ");
           _outtext(aItem[index].achItem);
           _outtext(" ");
           mouse(1, 0, 0, 0);
   
           if( Hilight & HILIGHT)
           {
               _settextposition( (short) (row + 1), (short) (CurrColumn + aItem[index].iHilite + 1));
               str[0] = aItem[index].achItem[aItem[index].iHilite];
               str[1] = 0;
               _settextcolor( _TBRIGHTWHITE );
               mouse(2, 0, 0, 0);
               _outtext(str);
               mouse(1, 0, 0, 0);
           }

           _setbkcolor( bgFill );
           _settextcolor( fgText );
      }
      else
      {
           mouse(2, 0, 0, 0);
           _outtext(" ");
           _outtext(aItem[index].achItem);
           _outtext(" ");
           mouse(1, 0, 0, 0);

           if( (Hilight & HILIGHT) )
           {
               _settextposition( (short) (row + 1), (short) (CurrColumn + aItem[index].iHilite + 1));
               str[0] = aItem[index].achItem[aItem[index].iHilite];
               str[1] = 0;
               _settextcolor( _TBRIGHTWHITE );
               mouse(2, 0, 0, 0);
               _outtext(str);
               mouse(1, 0, 0, 0);
               _settextcolor( (short) fgText );
           }
      }
   }
}

/* GetKey - Gets a key from the keyboard. This routine distinguishes
 * between ASCII keys and function or control keys with different shift
 * states. It also accepts a flag to return immediately if no key is
 * available.
 *
 * Params: fWait - Code to indicate how to handle keyboard buffer:
 *   NO_WAIT     Return 0 if no key in buffer, else return key
 *   WAIT        Return first key if available, else wait for key
 *   CLEAR_WAIT  Throw away any key in buffer and wait for new key
 *
 * Return: One of the following:
 *
 *   Keytype                                High Byte    Low Byte
 *   -------                                ---------    --------
 *   No key available (only with NO_WAIT)       0           0
 *   ASCII value                                0        ASCII code
 *   Unshifted function or keypad               1        scan code
 *   Shifted function or keypad                 2        scan code
 *   CTRL function or keypad                    3        scan code
 *   ALT function or keypad                     4        scan code
 *
 * Note:   getkey cannot return codes for keys not recognized by BIOS
 *         int 16, such as the CTRL-UP or the 5 key on the numeric keypad.
 */
int GetKey( int fWait, int *wFlag)
{
    int uKey = -1;
    unsigned uShift;
    int loop = TRUE;
    int index;
    int first = 0;

    /* If CLEAR_WAIT, drain the keyboard buffer. */
    if( fWait == CLEAR_WAIT )
        while( _bios_keybrd( _KEYBRD_READY ) )
            _bios_keybrd( _KEYBRD_READ );

    /* If NO_WAIT, return 0 if there is no key ready. */
    //if( !fWait && !_bios_keybrd( _KEYBRD_READY ) )
    //    return FALSE;
    
    //mouse(1, 0, 0, 0);

    while(loop == TRUE)
    {
       if(_bios_keybrd( _KEYBRD_READY ))
       {
           CurrMenu = -2;

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

       mouse(3,0,0,0);

       //sprintf(message, "-- %03d %03d %03d --", two, three, four);
       //_settextposition((short) 25, 20);
       //_outtext(message);

       if(two == 1)
       {
           while(two == 1)
           {
               if((uKey = HandleMouse()) != -1)
               {
                   if(first == 0)
                   {
                       if(OldMenu != CurrMenu)
                       {
                           OldMenu = CurrMenu;
                           return uKey;
                       }
                   }
                   else
                   {
                       OldMenu = CurrMenu;
                       return uKey;
                   }
               }
               else
                   if(uKey == -1)
                   {
                       OldMenu = CurrMenu;
                       return uKey;   
                   }     
            
               mouse(3,0,0,0);
           }

           first = 1;
       }

       if(KeyAlt())
       {
           if(*wFlag == 1)
           {
               while(KeyAlt())
               {
                   if(KeyDown)
                   {
                       while(KeyDown && KeyAlt())
                       {
                           mouse(3,0,0,0);
                           if((uKey = HandleMouse()) != -1)
                               return uKey;

                           if(_kbhit())
                           {
                               if((uKey = getch()) == 0)
                               {
                                   CurrMenu = -1;
                                   return uKey;
                               }

                               KeyDown = FALSE;

                               for(index = 0; index < 26; index++)
                               {
                                   if((codes[index][0]) == uKey)
                                   {
                                       *wFlag = 0;
                                       CurrMenu = -1;
                                       return codes[index][1];
                                   }
                               }
                           }
                       }

                       *wFlag = 0;
                       KeyDown = FALSE;
                       CurrMenu = -1;
                       return -1;
                   }

                   mouse(3,0,0,0);
                   if((uKey = HandleMouse()) != -1)
                       return uKey;
               }

               return ALT;
           }

           if(*wFlag == 0)
           {
               if(sf == 1)
               {
                   CurrMenu = -1;
                   return ALT;
               }

               while(KeyAlt());
               //{
               //    if((uKey = HandleMouse()) != -1)
               //        return uKey;
               //}
               sf = 1;
           }
       }
       
    }

   CurrMenu = -1;
   return uKey;
}

/* _outchar - Display a character. This is the character equivalent of
 * _outtext. It is affected by _settextposition, _settextcolor, and
 * _setbkcolor. It should not be used in loops. Build strings and then
 * _outtext to show multiple characters.
 *
 * Params: out - character to be displayed
 *
 * Return: none
 */
void _outchar( char out )
{
    static char achT[2] = " ";      /* Temporary array of characters */

    achT[0] = out;
    mouse(2, 0, 0, 0);
    _outtext( achT );
    mouse(1, 0, 0, 0);
}

int HandleMouse()
{
    //sprintf(message, "-- %03d %03d %03d --", two, three, four);
    //_settextposition((short) 25, 20);
    //_outtext(message);

    if(two == 1)
    {
       int index = 0;

       while(Coord_List[index].x1 != -1)
       {
           if(
                ( (three >= Coord_List[index].x1) && 
                (three <= Coord_List[index].x2) ) 
                && 
                ( (four >= Coord_List[index].y1) && 
                (four <= Coord_List[index].y2) )
             )
           {
               two = three = four = 0;
               CurrMenu = index;
               return Coord_List[index].hc;
           }

           index++;
       }

       index = 10;

       while(Coord_List[index].x1 != -1)
       {
           if(
                ( (three >= Coord_List[index].x1) && 
                (three <= Coord_List[index].x2) ) 
                && 
                ( (four >= Coord_List[index].y1) && 
                (four <= Coord_List[index].y2) )
             )
           {
               two = three = four = 0;
               CurrMenu = index;
               return CurrMenu;
               //return Coord_List[index].hc;
           }

           index++;
       }

       two = three = four = 0;
    }

    CurrMenu = -1;
    return -1;
}













































