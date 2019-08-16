/* Function prototypes, macros, structure, and global variables for
 * Menu and related functions.
 */

/* Include only once */
#ifndef MENU_H
#define MENU_H

/* Sample key codes for getkey. Additional codes in the same format may
 * be added.
*/

#define MOUSE  -98

#define BORDER    0x0001
#define NO_BORDER 0x0002

#define NEXT      0x0901
#define PREVIOUS  0x0902
#define ANY       0x0903

#define HOME    0x0147
#define END     0x014F

#define PG_UP   0x0149
#define PG_DN   0x0151

#define U_UP    0x0148      /* Unshifted */
#define U_DN    0x0150
#define U_LT    0x014b
#define U_RT    0x014d
#define S_UP    0x0248      /* Shifted */
#define S_DN    0x0250
#define S_LT    0x024b
#define S_RT    0x024d

#define N_PLUS  0x014e      /* PLUS and MINUS on numeric keypad */
#define N_MINUS 0x014a

//#define ENTER   13          /* ASCII */

/* Action codes for getkey */
enum WAITACTION { NO_WAIT, WAIT, CLEAR_WAIT };

/* Text output colors. Note that monochrome can only use _TBLACK,
 * _TWHITE, _TBRIGHTWHITE, and _TUNDERLINE. Graphics black-and-white
 * can only use the first three of these. The first eight colors
 * can be used as background colors (although they may need to be
 * cast to long).
 */
enum TEXTCOLORS
{
    _TBLACK,        _TBLUE,         _TGREEN,        _TCYAN,
    _TRED,          _TMAGENTA,      _TBROWN,        _TWHITE,
    _TGREY,         _TLIGHTBLUE,    _TLIGHTGREEN,   _TLIGHTCYAN,
    _TLIGHTRED,     _TLIGHTMAGENTA, _TLIGHTYELLOW,  _TBRIGHTWHITE,
    _TUNDERLINE = 1
};

/* Structure and global variable for menu attributes */
typedef struct _MENU
{
    int     fgBorder, fgNormal, fgSelect, fgNormHilite, fgSelHilite;
    long    bgBorder, bgNormal, bgSelect, bgNormHilite, bgSelHilite;
    int     fCentered;
    unsigned char   chNW, chNE, chSE, chSW, chNS, chEW;
} MENU;
extern MENU mnuAtrib;

/* Structure and maximum length for menu items */
#define MAXITEM 100
typedef struct _ITEM
{
    int     iHilite;
    char    achItem[81];
} ITEM;

typedef struct _ITEMBAR
{
    int     iMaxLength;
    int     iHilite;
    char    achItem[51];
} ITEMBAR;

typedef struct tagCoordinates
{
   short x1;
   short y1;
   short x2;
   short y2;
   char  hc;
}  COORDS;

typedef COORDS *P_COORDS;

/* Public menu, output, and input functions */
long Menu( short row, short col, ITEM aItem[], int iCur, int BarMenu );
int ListBox( short row, short col, int mxWidth, int maxEntries, ITEM aItem[], int EntriesCount, int iCur, unsigned Hilight, char *retBuffer);
void Box( short row, short col, int rowLast, int colLast, short fgBorder, long bgBorder, short BorderFlag);
int GetKey( int fWait, int *wFlag);
void _outchar( char out );
long MenuBar( short row, short col, ITEMBAR aItem[], int MenuCount, int iCur, short fgText, long bgFill, unsigned Draw, unsigned Hilight );

#endif /* MENU_H */

