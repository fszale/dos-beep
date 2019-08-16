/*========[ The TesSeRact(TM) Ram Resident Development System ]=========*
 | Copyright (c) 1987-1992, Innovative Data Concepts, Inc.
 | All Rights Reserved.
 |
 | This Library is part of IDC's TesSeRact Development Tools product
 | line. For information about other IDC products, call 1-215-443-9705.
 *-V--------------------------------------------------------------------*
 | $Header:   ttsr.h     200.0   14 Aug 1992  3:46:46  $
 |
 | $Log:   ttsr.h    $
 *-D--------------------------------------------------------------------*
 |
 *-N-----------------------[ Notes and Caveats ]------------------------*
 |
 *======================================================================*/

    /*
     * Stores interrupt vector information for TesSeRact use
     */
struct TsrIntTable {
    void far * OldVector;               /* Old Interrupt Vector            */
    unsigned char IntNumber;            /* Interrupt Number                */
    void near * NewVector;              /* offset of new vector            */
    };

    /*
     * User structure for defining additional hotkeys
     */
struct ExtraHot {
    unsigned char Hotkey;               /* hotkey to check for             */
    unsigned char ShiftState;           /* shift state for this hot key    */
    unsigned char FlagByte;             /* flag value to use               */
                                        /*  MAY NOT BE ZERO!!!             */
    };

    /*
     * TesSeRact internal data
     */
struct TsrData {
    unsigned char RevLvl;               /* Revision Level of TESS Lib      */
    unsigned char PopupType;            /* Type of popup in effect         */
    unsigned char WasInt8;              /* An Interrupt 08h occurred       */
    unsigned char WasInt13;             /* An Interrupt 13h occurred       */
    unsigned char IntFlags;             /* Which interrupts are active     */
    unsigned char SoftFlags;            /* Which soft ints are active      */
    unsigned char DosVersion;           /* current major revision of DOS   */
    unsigned char waitcount;            /* Count to wait before popping up */
    void far * InDosFlag;               /* Pointer to DOS INDOS flag       */
    void far * DosCritErr;              /* Pointer to DOS Critical Error   */
    void far * UserInt24;               /* pointer to use INT 24 handler   */
    unsigned char SwapType;             /* type of swapping in use         */
    unsigned char DiskFn;               /* internal disk function variable */

    struct TsrIntTable Int8;            /* Interrupt vector tables         */
    struct TsrIntTable Int9;
    struct TsrIntTable Int10;
    struct TsrIntTable Int13;
    struct TsrIntTable Int16;
    struct TsrIntTable Int1C;
    struct TsrIntTable Int2A;
    struct TsrIntTable Int28;
    struct TsrIntTable Int2F;
    struct TsrIntTable Int1B;
    struct TsrIntTable Int23;
    struct TsrIntTable Int24;

    unsigned short UserSS;              /* Stack segment of user program   */
    unsigned short UserSP;              /* Stack pointer of user program   */
    unsigned short User28SS;            /* Stack segment of user program   */
    unsigned short User28SP;            /* Stack pointer of user program   */
    unsigned short SavSiz;              /* Size of DOS save area           */
    unsigned short MemSeg;    /* special memory segment          */
    unsigned short PopStk;    /* offset of stack during popup    */
    unsigned short BakStk;    /* offset of stack during backgroun*/
    unsigned short IntBuf;    /* offset of interrupt save buffer */
    unsigned short ExtBuf;    /* offset of extra hotkey buffer   */
    unsigned short KeyBuf;    /* offset of keyboard paste buffer */
    unsigned short MouBuf;    /* offset of mouse save buffer     */
    unsigned short MouSiz;             /* size of mouse save buffer       */
    unsigned short Paras;     /* number of paragraphs to swap    */
    unsigned short Start;     /* start segment of swap area      */
    unsigned short SegAddr;      /* EMM Segment base                */
    unsigned short EmmHan;       /* EMM handle in use               */
    unsigned short XmmHan;       /* XMS handle in use               */
    void far * XmsCall;          /* address of XMS entry point      */
    };

    /*
     * TesSeRact data for individual TSR
     */
struct TsrParms {
    char IdCode[8];                     /* Unique TSR Identification String*/
                                        /* NOTE -- NOT NULL-TERMINATED     */
    unsigned short IdNum;               /* Unique TSR Identification Number*/
    unsigned long FuncFlags;            /* Bit map of supported functions  */
    unsigned char HotKey;               /* Hotkey used by TSR for popup    */
    unsigned char ShiftState;           /* ShiftState used by this TSR pop */
    unsigned char HotKeyFlag;           /* which hotkey is in use          */
    unsigned char ExtraHotCount;        /* Number of Extra Hotkeys to use  */
    unsigned short TsrStatus;           /* Current TSR Status Flags        */
    unsigned short TsrPSP;              /* TSR's PSP Segment               */
    void far * TsrDTA;                  /* Pointer to TSR's DTA region     */
    unsigned short TsrDSeg;             /* TSR's Default Data Segment      */
    void far * PopupStack;              /* Pointer to Popup Stack Area     */
    void far * BackStack;               /* Pointer to Background Stack     */
    };

/*-------------------------------------------------------------------------*/

        /************************************************************
        *   Other EQUates                                           *
        *********************************************************CR*/

    /*
     * #defines for HotKey
     */
#define TSRHOT_A        0x1e
#define TSRHOT_B        0x30
#define TSRHOT_C        0x2e
#define TSRHOT_D        0x20
#define TSRHOT_E        0x12
#define TSRHOT_F        0x21
#define TSRHOT_G        0x22
#define TSRHOT_H        0x23
#define TSRHOT_I        0x17
#define TSRHOT_J        0x24
#define TSRHOT_K        0x25
#define TSRHOT_L        0x26
#define TSRHOT_M        0x32
#define TSRHOT_N        0x31
#define TSRHOT_O        0x18
#define TSRHOT_P        0x19
#define TSRHOT_Q        0x10
#define TSRHOT_R        0x13
#define TSRHOT_S        0x1f
#define TSRHOT_T        0x14
#define TSRHOT_U        0x16
#define TSRHOT_V        0x2f
#define TSRHOT_W        0x11
#define TSRHOT_X        0x2d
#define TSRHOT_Y        0x15
#define TSRHOT_Z        0x2c
#define TSRHOT_0        0x0b
#define TSRHOT_1        0x02
#define TSRHOT_2        0x03
#define TSRHOT_3        0x04
#define TSRHOT_4        0x05
#define TSRHOT_5        0x06
#define TSRHOT_6        0x07
#define TSRHOT_7        0x08
#define TSRHOT_8        0x09
#define TSRHOT_9        0x0a
#define TSRHOT_F1       0x3b
#define TSRHOT_F2       0x3c
#define TSRHOT_F3       0x3d
#define TSRHOT_F4       0x3e
#define TSRHOT_F5       0x3f
#define TSRHOT_F6       0x40
#define TSRHOT_F7       0x41
#define TSRHOT_F8       0x42
#define TSRHOT_F9       0x43
#define TSRHOT_F10      0x44

    /*
     * Enhanced Keyboards only
     *
     * May not work with all computers, keyboards
     *
     */
#define TSRHOT_F11      0x57
#define TSRHOT_F12      0x58

    /*
     * #defines for ShiftState
     */
#define TSRPOPRSHIFT    0x01
#define TSRPOPLSHIFT    0x02
#define TSRPOPCTRL      0x04
#define TSRPOPALT       0x08

    /*
     * Obsolete equates maintained for compatibility
     */
#define NOPOPGRAPH      0x0000          /* Don't pop over graphics         */
#define NOPOPCOMMAND    0x0000          /* Don't pop over command-line     */
#define SHIFTSON        0x0000          /* shift states match              */
#define EXTRAHOTSET     0x0000          /* extra hot keys installed        */

    /*
     * #defines for TSRFLAGS
     */
#define TSRUSEPOPUP     0x0010          /* Use TsrMain()                   */
#define TSRUSEBACK      0x0020          /* Use TsrBackProc()               */
#define TSRUSETIMER     0x0040          /* Use TsrTimer()                  */
#define TSRUSEUSER      0x0100          /* Use TsrUserProc()               */
#define TSRNOHMA        0x1000          /* Do not use HMA for data         */
#define TSRNOHIGH       0x2000          /* Do not load TSR high            */
    /*
     * #defines for TsrStatus flags
     */

#define HOTKEYON        0x0001          /* hot key pressed                 */
#define TSRACTIVE       0x0004          /* tsr is running in foreground    */
#define INT28ACTIVE     0x0008          /* INT28 routine is running        */

#define POPUPSET        0x0010          /* popup resident routine installed*/
#define BACKSET         0x0020          /* background routine installed    */
#define TIMERSET        0x0040          /* timer procedure installed       */

#define USERPROCON      0x0100          /* User-defined procedure installed*/
#define TSRENABLED      0x0200          /* TSR currently enabled           */
#define TSRRELEASED     0x0400          /* TSR has been released           */
#define EXTRAINT24      0x0800          /* User installed replacement INT24*/


    /*
     * EQUates for IntFlags and SoftFlags
     */

#define ININT13         0x04            /* Interrupt 13 is currently active*/
#define ININT21         0x08            /* Interrupt 21 is currently active*/
#define ININT16         0x10            /* Interrupt 16 is currently active*/
#define ININT28         0x20            /* Interrupt 28 is currently active*/
#define ININT08         0x40            /* Interrupt 08 is currently active*/
#define ININT1C         0x80            /* Interrupt 1C is currently active*/

    /*
     *  size of internal keyboard pasting buffer.  Increasing this number
     *    will increase the resident size of the program.
     */

#define KEY_SIZ         256

    /*
     * number of extra hotkey structures allocated for resident program.
     *   Increasing this number will increase the resident size of the program;
     *   the ExtraHot structure is 3 bytes per key, and the default is for 8
     *   keys.  This must be set at a MINIMUM of 1
     */

#define EXTRA_SIZ       8


/*-------------------------------------------------------------------------*/

        /************************************************************
        *   Prototypes for routines in TESS.LIB                     *
        *********************************************************CR*/

extern unsigned far pascal TsDoInit(
    unsigned char HotKey,
    unsigned char ShiftState,
    unsigned short TSRFlags,
    unsigned short MemoryTop
    );


extern void far pascal TsSetStack(
    void far *PopUpStack,
    void far *BackGroundStack
    );

extern void far pascal TsSetStack(
    void far *PopUpStack,
    void far *BackGroundStack
    );

extern void far pascal TsSetDefStk(
    unsigned int StkSiz
    );

extern void far pascal TessBeep(
    void
    );

/*-------------------------------------------------------------------------*/
        /************************************************************
        *  TesSeRact Multiplex Interrupt (2Fh) functions            *
        *    These functions are provided for your convenience in   *
        *    TESS.LIB                                               *
        *********************************************************CR*/

extern unsigned far pascal TsCheckResident(
    char far *IDStr,
    unsigned far *IDNum
    );

extern struct TsrParms far * far pascal TsGetParms(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsCheckHotkey(
    unsigned char HotKey
    );

extern unsigned far pascal TsSetUser24(
    unsigned short TsrIdNum,
    void (far *UserCritErrProc) (void)
    );

extern struct TsrData far * far pascal TsGetData(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsSetExtraHot(
    unsigned short TsrIdNum,
    unsigned char count,
    struct ExtraHot far *ExtraHotKeys
    );

extern unsigned far pascal TsEnable(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsDisable(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsRelease(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsRestart(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsGetStat(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsSetStat(
    unsigned short TsrIdNum,
    unsigned short NewStatus
    );

extern unsigned far pascal TsGetPopType(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsCallUserProc(
    unsigned short TsrIdNum,
    void far *UserPtr
    );

extern unsigned far pascal TsStuffKeyboard(
    unsigned short TsrIdNum,
    void far *KbdChars,
    unsigned short CharCount,
    unsigned short Speed
    );

extern unsigned far pascal TsTriggerPop(
    unsigned short TsrIdNum
    );

extern unsigned far pascal TsTriggerBack(
    unsigned short TsrIdNum
    );

void far pascal TsSwapType(unsigned char type);

void far pascal TsSwapPath(char far *PathName);


/*-------------------------------------------------------------------------*/
        /************************************************************
        *   USER-DEFINED routines                                   *
        *     If no entry point for these routines is defined in    *
        *     your code, a dummy procedure from TESS.LIB will be    *
        *     used to satisfy the external references.              *
        *       These are the prototypes for the functions.         *
        *     Note that beginning with Version 0.74, it is no       *
        *       longer necessary for all these procedures to be     *
        *       defined -- TESS.LIB has dummy procedures to resolve *
        *       the external references.                            *
        *********************************************************CR*/

void far pascal TsrMain(
    void
    );

unsigned near pascal TsrBackCheck(
    void
    );

void far pascal TsrBackProc(
    void
    );

void far pascal TsrUserProc(
    void far *UserPtr
    );

void far pascal TsrCleanUp(
    unsigned InitOrShutdown
    );

void near pascal TsrBackNear(
    void
    );

void near pascal TsrTimerProc(
    void
    );

