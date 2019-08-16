/*		   ACK-3D ( Animation Construction Kit 3D )		      */
/* External Reference */
/* Author: Lary Myers */

extern		WALLARRAY	    Walls[];
extern		DOORS		    Door[];
extern		OBJECTS		    ObjList[];
extern		KEYBOARD	    keyBoard;
extern		MOUSE		    mouse;
extern		SPECIALCODE	    SpecialCodes[];
extern			     int    TotalSpecial;
extern			     int    xSecretmPos;
extern			     int    xSecretmPos1;
extern			     int    xSecretColumn;
extern			     int    ySecretmPos;
extern			     int    ySecretmPos1;
extern			     int    ySecretColumn;
extern			     int    TotalSecret;
extern			     int    ErrorCode;
extern			     int    CurrentLevel;
extern	    unsigned	     char   scanCode;
extern			     char   numLockKeyStatus;
extern	    void	 interrupt  (*oldvec)();
extern	    void	 interrupt  myInt();
extern			     long   LastX1;
extern			     long   LastY1;
extern			     long   iLastX;
extern			     long   iLastY;
extern			     int    MaxDistance;
extern			     int    xMapPosn;
extern			     int    yMapPosn;
extern			     int    SkyColor;
extern			     int    FloorColor;
extern			     int    FlashColor;
extern			     int    TopColor;
extern			     int    BottomColor;
extern			     int    StartX;
extern			     int    StartY;
extern			     int    StartAngle;
extern			     int    DoorSpeed;
extern			     int    NonSecretCode;
extern			     int    HaveMouse;	/* 1 if mouse present, else 0 */
extern			     int    NoWalls;
extern			     int    PageNum;	/* 0,1 - current video page  */
extern			     FILE   *dfp;	/* Debug file if DEBUG is 1  */
extern		unsigned     char far *bMaps[]; /* Array of wall bitmaps     */
extern		unsigned     char far *oMaps[]; /* Array of object bitmaps   */
extern			     void  (far *bdfp[])();
extern		unsigned     char   Grid[];	/* Master grid read from disk */
extern		unsigned     char   xGrid[];	/* X walls		     */
extern		unsigned     char   yGrid[];	/* Y walls		     */
extern		unsigned     char   xObjGrid[]; /* X objects		     */
extern		unsigned     char   yObjGrid[]; /* Y objects		     */
extern			     int    PageBegin[]; /* Video offsets	     */
extern			     int    DistanceTable[]; /* Distance vs height   */
extern			     long   AdjustTable[];   /* Used to scale bitmap */
extern			     int    ViewColumn;
extern			     int    CenterRow;
extern			     long far *SinTable;
extern			     long far *CosTable;
extern			     long far *LongTanTable;
extern			     long far *LongInvTanTable;
extern			     long far *InvCosTable;
extern			     long far *InvSinTable;
extern			     long far *LongCosTable;
extern			     long far *ViewCosTable;
extern			     long far *xNextTable;
extern			     long far *yNextTable;
extern		unsigned     char   ObjFlags[];
extern			     int    MaxObjects;
extern			     int    TotalObjects;
extern		unsigned     char   ObjNumber[];
extern			     int    ObjMapPosn[];
extern			     long   ObjRelDist[];
extern			     int    DirDx[];
extern			     int    DirDy[];
extern			     int    DirAngle[];
extern			     char   PalFile[];
extern			     char   GridFile[];
extern			     char   GoalFile[];
extern			     char   GoalPalFile[];
extern		unsigned     char   mask[];
extern		unsigned     char   lowmask[];
extern			     long   txRay;
extern			     long   tyRay;
extern			     long   tDOW;
extern			     long   tFlip;

