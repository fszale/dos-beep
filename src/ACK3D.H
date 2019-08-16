/* Header file for xEngine.c "Currently called xtest.c" */

#define DEBUG	0

#define CLOCK_PTR   (*(long far *)((long)0x46C))

#define ULPTR	unsigned long

#define INT_ANGLE_1	5
#define INT_ANGLE_2	10
#define INT_ANGLE_4	20
#define INT_ANGLE_6	30
#define INT_ANGLE_30	160
#define INT_ANGLE_45	240
#define INT_ANGLE_90	480
#define INT_ANGLE_135	720
#define INT_ANGLE_180	960
#define INT_ANGLE_225	1200
#define INT_ANGLE_270	1440
#define INT_ANGLE_315	1680
#define INT_ANGLE_360	1920

#define FP_SHIFT	16
#define FP_MULT		65536
#define FP_HALF		32768

#define VIEW_WIDTH	320
#define MAX_DISTANCE	2048

#define SKY_COLOR	195
#define SKY_FLASH	15
#define FLOOR_COLOR	20

#define TYPE_WALL	0
#define TYPE_OBJECT	1
#define TYPE_PALETTE	2

#define BITMAP_WIDTH	64
#define BITMAP_HEIGHT	64
#define BITMAP_SIZE	(BITMAP_WIDTH * BITMAP_HEIGHT)

#define GRID_SIZE	64
#define GRID_WIDTH	64
#define GRID_HEIGHT	64
#define GRID_MAX	GRID_WIDTH * GRID_HEIGHT
#define GRID_XMAX	GRID_WIDTH * GRID_SIZE
#define GRID_YMAX	GRID_HEIGHT * GRID_SIZE

#define GRID_XMAXLONG	(GRID_XMAX * FP_MULT)
#define GRID_YMAXLONG	(GRID_YMAX * FP_MULT)

typedef struct {		/* Keyboard structure. Each key that can */
    int rightArrow;		/* be pressed is represented here.	 */
    int leftArrow;
    int upArrow;
    int downArrow;
    int home;
    int pgup;
    int pgdn;
    int end;
    int control;
    int alt;
    int plus;
    int minus;
    int escape;
    int letA;
    int letS;
} KEYBOARD;

#define KEYBD  0x9    /* INTERRUPT 9 */

/* Keyboard press and release codes for INT 9 handler */

#define RIGHT_ARROW_PRESSED   77
#define RIGHT_ARROW_RELEASED  205
#define UP_ARROW_PRESSED      72
#define UP_ARROW_RELEASED     200
#define LEFT_ARROW_PRESSED    75
#define LEFT_ARROW_RELEASED   203
#define DOWN_ARROW_PRESSED    80
#define DOWN_ARROW_RELEASED   208
#define CONTROL_PRESSED	      29
#define CONTROL_RELEASED      157
#define ESCAPE		      1
#define HOME_PRESSED	      71
#define HOME_RELEASED	      199
#define PGUP_PRESSED	      73
#define PGUP_RELEASED	      201
#define PGDN_PRESSED	      81
#define PGDN_RELEASED	      209
#define END_PRESSED	      79
#define END_RELEASED	      207
#define PLUS_PRESSED	      78
#define PLUS_RELEASED	      206
#define MINUS_PRESSED	      74
#define MINUS_RELEASED	      202
#define LETTER_A_PRESSED      30
#define LETTER_A_RELEASED     158
#define LETTER_S_PRESSED      31
#define LETTER_S_RELEASED     159

typedef struct {
	    int	    BitmapNumber;	/* Number in map  */
	    int	    BitmapType;		/* Wall or object */
	    char    *BitmapFile;	/* Filename	  */
} BMFILE;

typedef struct {
	int	    Number;
	int	    Column;
	int	    Distance;
	int	    filler;	/* Use to allow shifts vs multiplies */
    } WALLARRAY;

#define MAX_VIEWS	    49
#define MAX_OBJECTS	    100

#define MAX_HEIGHT	    1024
#define MIN_HEIGHT	    16


#define DOOR_SECRETCODE	    58
#define DOOR_XCODE	    60
#define DOOR_SIDECODE	    61
#define DOOR_YCODE	    62

#define DEFAULT_DOOR_SPEED  2
#define MAX_DOORS	    10	    /* Max opening or closing at one time  */
#define MAX_UPDOWN	    30	    /* Max up or down spots for each level */
#define MAX_SECRET_DOORS    30	    /* Max secret doors in one level	   */

#define MAP_STARTCODE	    0xFC    /* Force player to this square  */
#define MAP_UPCODE	    0xFD    /* Go up to previous level	    */
#define MAP_DOWNCODE	    0xFE    /* Go down to next level	    */
#define MAP_GOALCODE	    0xFF    /* Finish line!		    */


#define ERR_BADFILE	    100
#define ERR_BADCOMMAND	    101
#define ERR_BADOBJNUMBER    102
#define ERR_BADSYNTAX	    103
#define ERR_LOADINGBITMAP   104
#define ERR_BADDIRECTION    105
#define ERR_BADSTARTX	    106
#define ERR_BADSTARTY	    107
#define ERR_BADANGLE	    108
#define ERR_BADMAPFILE	    109
#define ERR_READINGMAP	    110
#define ERR_BADPICNAME	    111
#define ERR_INVALIDFORM	    112
#define ERR_NOPBM	    113
#define ERR_BADPICFILE	    114
#define ERR_NOMEMORY	    115
#define ERR_BADPALFILE	    116

typedef struct {
	    int	    mPos;
unsigned    char    mCode;
} SPECIALCODE;


typedef struct {
	    int	    mPos;
	    int	    mPos1;
unsigned    char    mCode;
unsigned    char    mCode1;
unsigned    char    ColOffset;
	    char    Speed;
	    char    Type;
} DOORS;


typedef struct {
	    char    Active;
unsigned    char    bmNum[MAX_VIEWS];
	    int	    Sides;
	    char    Dir;
unsigned    char    CurNum;
unsigned    char    MaxNum;
	    char    Speed;
	    int	    VidRow;
	    int	    x;
	    int	    y;
	    int	    mPos;
} OBJECTS;

typedef struct {
    int	    mdx;
    int	    mdy;
    int	    mButtons;
} MOUSE;

/****************************************************************************
**  P R O T O T Y P E S							   **
****************************************************************************/
void SetPalette(unsigned char far *PalBuf);
int mouse_installed(void);
void mouse_read_cursor(int *button,int *row,int *col);
void mouse_set_cursor(int row,int col);
void usepage(int);
void flippage(void);
void graphinit(void);
void textmode(int);
void DrawObject(int ObjNum,int ObjCol,int ObjDist,int VidCol,int PageNum);
void DrawOneObject(int ObjNum,int ObjCol,int ObjDist,int VidCol,int PageNum);
void DrawWalls(void);
unsigned char far * Readiff(char *picname);
char *GetExtent(char *s);
int LoadBitmap(int BitmapNumber,char *BitmapName,int BitmapType);
int LoadPalette(char *PalName);
char *StripEndOfLine(char *s);
char *SkipSpaces(char *s);
char *AddExtent(char *s,char *ext);
char *CopyToComma(char *dest,char *src);
long long_sqrt(long value);
char xRay(int x,int y,int angle);
char yRay(int x,int y,int angle);
void DrawView(int xPlayer,int yPlayer,int PlayerAngle);
void FindObject(int xPlayer,int yPlayer,int PlayerAngle);
int GetTables(void);
void keyCheck(void);
void keyBoardInit(void);
long CheckObjects(int xPlayer,int yPlayer,int PlayerAngle);
int CheckHit(int xPlayer,int yPlayer,int ViewAngle);
void CheckMouse(MOUSE *m);
void MoveObject(int Index,int dx,int dy);
void CheckObjectMovement(void);
int FindDoor(int MapPosn);
int FindDoorSlot(int MapPosn);
int BuildTables(void);
void InitializeMouse(void);
void BuildXYgrid(void);
void CheckDoors(int xPlayer,int yPlayer);
int ReadMasterFile(char *fname);
int ReadMapFile(void);
unsigned char CheckSpecialCodes(int MapPosn);
int ReadNewLevel(void);
void WrapUp(void);
void DoGoalScreen(void);
void SetVGAmode(void);
int FindSecretDoor(int MapPosn);
int GetSecretColumn(int MapPosn);

