#define GETRETURNSTATUS(l)       ((int)((l) >> 16))
#define GETRETURNCODE(l)         ((int)(l))

#define ASSIGN_HIWORD(var, val)  (*(((int *) var) + 1) = val)
#define ASSIGN_LOWORD(var, val)  (*(((int *) var)) = val)
 
#define FROM_MOUSE               0x0001

#define TEST 0

#define FLG_SIGNATURE_ENABLE     0x00000001
#define FLG_TIMESTAMP_ENABLED    0x00000002
#define FLG_MSG_RECALL_ENABLED   0x00000004
#define FLG_LENGTH_COUNT         0x00000800

#define OPT_LOG_TRANSACTIONS     0x00000008
#define OPT_LOG_ERRORS           0x00000010

#define PRINTER_PORT_NONE    0
#define PRINTER_PORT_1       1
#define PRINTER_PORT_2       2
#define PRINTER_PORT_3       3

#define HILIGHT               0x0001
#define NO_HILIGHT            0x0002

#define DRAW_ONLY             0x0004
#define PROCESS               0x0008
#define COPY_SELECTION        0x0010

/* Comm Baud Rate indices */
#define CBR_110      0xFF10
#define CBR_300      0xFF11
#define CBR_600      0xFF12
#define CBR_1200     0xFF13
#define CBR_2400     0xFF14
#define CBR_4800     0xFF15
#define CBR_9600     0xFF16
#define CBR_14400    0xFF17
#define CBR_19200    0xFF18
#define CBR_38400    0xFF1B
#define CBR_56000    0xFF1F
#define CBR_128000   0xFF23
#define CBR_256000   0xFF27

#define NOPARITY	    0
#define ODDPARITY	    1
#define EVENPARITY	    2
#define MARKPARITY	    3
#define SPACEPARITY	    4

#define ONESTOPBIT	    0
#define ONE5STOPBITS	    1
#define TWOSTOPBITS	    2

#define DOS_BAUD     1
#define DOS_PARITY   2
#define DOS_STOPBITS 3
#define DOS_DATABITS 4
#define DOS_COMMPORT 5

#define TAB_STRING "!TAB!"
#define ESCAPE_STRING "!ESCAPE!"
#define SHIFT_TAB_STRING "!SHIFT_TAB!"

#define CENTER 0x0001
#define LEFT   0x0002
#define RIGHT  0x0004
#define CLEAR  0x0008

#define SELECTED 2
#define NOT_SELECTED -2

#define ESCAPE     -1
#define ENTER      -3
#define TAB        -4
#define SHIFT_TAB  -5
#define LIST       -6
#define ALT        -7

#define MAX_SUBSCRIBERS 200
#define MAX_SERVICES 200
#define MAX_INSTALLED_MODEMS 100
#define MAX_MODEMS 400
#define MAX_MESSAGES 100
#define MAX_GROUPS 50

#pragma pack(1)
typedef struct tagCntlParam
{
   char pCntlTitle[45];
   int  nAlignment;
   int  nAlignmentOffset;
   int  nCntlRow;
   int  nCntlOffset;
   int  nCntlLength;
   char pCntlBuffer[65];
}  CONTROL_PARAM;
#pragma pack()

typedef CONTROL_PARAM *PCONTROL_PARAM;

#pragma pack(1)
typedef struct tagfile
{
   char filename[13];
}  SPLFILE;
#pragma pack()

#pragma pack(1)
typedef struct tagMultilineCntlParam
{
   char *pCntlTitle;
   int  nAlignment;
   int  nAlignmentOffset;
   int  nCntlRow;
   int  nCntlOffset;
   int  nCntlLength;
   int  nCntlLines;
   char *pMultiBuffer;
}  MULT_CONTROL_PARAM;
#pragma pack()

typedef MULT_CONTROL_PARAM *PMULT_CONTROL_PARAM;

#pragma pack(1)
typedef struct tag_DosBEEP
{
   int     nDaysOld;
   int     nPrinterPort;
   char    szDefaultSignature[WAPI_SIGNATURE_LEN];

}  DosBEEPINFO;
#pragma pack()



//------- defines screens -----------------------------------------------
void   Subscriber();
void   Service();
void   Send_Page();
void   Modem();
void   Log();
void   Settings();
void   Group();
void   Predefined();
//-----------------------------------------------------------------------

//------- handles INI files ---------------------------------------------
void   GetINIValues();
BOOL   GetProfileString(LPSTR szSection, LPSTR szEntry, LPSTR szDefault, 
                       LPSTR szBuffer, int nBufferLen, LPSTR szFileName);
BOOL   WriteProfileString(LPSTR szSection, LPSTR szEntry, LPSTR szString,
                           LPSTR szFileName);
BOOL   FindString(int wFHandle, LPSTR lpszSection, LPSTR lpszEntry, 
                   LPSTR lpszDefault, LPSTR lpszBuffer, int nBufferLen);
BOOL   WriteString(LPSTR szFileName, int *wFHandle, LPSTR lpszSection, LPSTR lpszEntry, LPSTR szString);
//-----------------------------------------------------------------------

//------- defines controls ----------------------------------------------
void   SetSubscriberControls(PCONTROL_PARAM pCntlInfo);
void   SetServiceControls(PCONTROL_PARAM pCntlInfo);
void   SetModemControls(PCONTROL_PARAM pCntlInfo);
void   SetSendControls(PCONTROL_PARAM pCntlInfo, PMULT_CONTROL_PARAM pMult_CntlInfo);
void   SetSettingsControls(PCONTROL_PARAM pCntlInfo);
void   SetPredefControls(PCONTROL_PARAM pCntlInfo, PMULT_CONTROL_PARAM pMult_CntlInfo);
BOOL   SetGroupData(ITEM **Member, ITEM **NonMember, int *MemberCount, int *NonMemberCount);
void   SetGroupControls(PCONTROL_PARAM pCntlInfo);
//-----------------------------------------------------------------------

//-------- handles edit control -----------------------------------------
WORD   ProcessControl(PCONTROL_PARAM pCntlInfo, short fgText, long bgFill, int Flag);
void   ReDrawData(PCONTROL_PARAM pCntlInfo, int index, short fgText, long bgFill);
void   FreeControl(PCONTROL_PARAM pCntlInfo, int nType);
//-----------------------------------------------------------------------

//-------- handles multiline edit control -------------------------------
WORD   ProcessMultiLineControl(PMULT_CONTROL_PARAM pCntlInfo, int LinesNumber, PCONTROL_PARAM pTextLenCntl, short fgText, short bgFill, int count);
void   ReDrawMultilineData(PMULT_CONTROL_PARAM pCntlInfo, int index, int LinesNumber);
void   FreeMultilineControl(PMULT_CONTROL_PARAM pCntlInfo, int nType);
//-----------------------------------------------------------------------

//-------- handles screen drawing ---------------------------------------
void   StatusBar(LPSTR string, int nAlignment, int nAlOffset, int nRow, short fgText, long bgFill);
void   PrintText(LPSTR string, int nAlignment, int nAlOffset, int nRow, short fgText, long bgFill);
void   DrawMultControls(PMULT_CONTROL_PARAM pMult_CntlInfo, int index);
void   DrawControls(PCONTROL_PARAM pCntlInfo, int index);
void   DrawMainScreen();
void   DrawSubscriberBox();
void   DrawServiceBox();
void   DrawModemBox();
void   DrawSendBox();
void   DrawGroupScreen();
void   DrawSettingsScreen();
void   DrawLogScreen();
void   DrawPredefinedScreen();
//-----------------------------------------------------------------------

//-------- handles databases lists --------------------------------------
void   PrintDatabase(int num);
int    ListSubscribers(LPWAPISUBSCRIBERINFO pSubInfo, short row, short col, int maxWidth, short maxEntries);
int    ListServices(LPWAPISERVICEINFO pSerInfo, short row, short col, int maxWidth, short maxEntries);
int    ListModems(LPWAPIMODEMLISTINFO pModemInfo, short row, short col, int maxWidth, short maxEntries);
int    ListInstalledModems(LPWAPIMODEMINFO pModemInfo, WORD wFlag, short row, short col, int maxWidth, short maxEntries);
int    ListPredefMessages(LPWAPIMESSAGEINFO pMessageInfo, short row, short col, int maxWidth, short maxEntries);
int    ListGroups(LPWAPIGROUPINFO pSubInfo, short row, short col, int maxWidth, int maxEntries);
//-----------------------------------------------------------------------

//-------- handles predefined options lists -----------------------------
WORD   ListBaudRate(LPSTR szBuffer);
WORD   ListBaudRate(LPSTR szBuffer);
WORD   ListDataBits(LPSTR szBuffer);
WORD   ListParity(LPSTR szBuffer);
WORD   ListStopBits(LPSTR szBuffer);
int    ListComOptions(LPWAPIMODEMINFO pModemInfo, short row, short col, int maxWidth, short maxEntries);
//-----------------------------------------------------------------------

//-------- handles screen contorls --------------------------------------
int    ProcessSettingsEdit(PCONTROL_PARAM pCntlInfo);
int    ProcessServiceEdit(PCONTROL_PARAM pCntlInfo);
int    ProcessSubscriberEdit(PCONTROL_PARAM pCntlInfo);
int    ProcessModemEdit(PCONTROL_PARAM pCntlInfo, short cntlNum);
int    ProcessSendEdit(PMULT_CONTROL_PARAM pMult_CntlInfo, PCONTROL_PARAM pCntlInfo);
int    ProcessPredefEdit(PMULT_CONTROL_PARAM pMult_CntlInfo, PCONTROL_PARAM pCntlInfo);
int    ProcessGroupEdit(PCONTROL_PARAM pCntlInfo, ITEM *NonMember, ITEM *Member, int *MemberCount, int *NonMemberCount);
//-----------------------------------------------------------------------

//-------- handles data conversion --------------------------------------
void   ConvertParams(PVOID pBuffer, DWORD source, WORD wFlag);
void   BackConvertParams(PCONTROL_PARAM pCntlInfo, PVOID dest, WORD wFlag);
//-----------------------------------------------------------------------

//-------- handles user notification ------------------------------------
void   NotifyUser(LPSTR message, WORD row, WORD column);
//-----------------------------------------------------------------------

//-------- handles paging -----------------------------------------------
void   Do_PET(LPWAPISPOOLERINFO);
//-----------------------------------------------------------------------

void   DeleteItem(ITEM *list, int *Count, LPSTR retBuffer);
void   AddItem(ITEM *NonMember, int *NonMemberCount, LPSTR retBuffer);
WORD   CheckControl(PCONTROL_PARAM pCntlInfo, short fgText, long bgFill, int Flag);
BOOL   PurgeLogs(int nDaysOld);
BOOL   WriteToLogFile(LPSTR szLogText);
BOOL   InsertMessage(LPSTR szSubscriber, PCONTROL_PARAM pFileInfo, LPSTR LastFile, HWAPIPAGEID wapiID);
void   ViewQueue(void);
void   AddToQueue(PCONTROL_PARAM pCntlInfo);
void   RemoveFromQueue(void);
void   ClearQueue(void);
void   Beep(unsigned duration, unsigned frequency);
void   ReDrawCheckData(PCONTROL_PARAM pCntlInfo, int index, short fgText, long bgFill);
void   FillGroupListBox(ITEM *list, int *Count, LPSTR Group, ITEM *NonMember, int *NonMemberCount);
int    ListEntries(ITEM *list, int Count, LPSTR member, short row, short col, int maxWidth, int maxEntries);
void   LoadHelpFile();
void   AboutBox(void);

BOOL   InstallModem(LPWAPIMODEMINFO pModemInfo, PCONTROL_PARAM pCntlInfo);
BOOL   KeyAlt();
void   mouse(int none,int ntwo,int nthree,int nfour);
int    HandleMouse();

extern union REGS Mouse_Regs;
extern int     one, two, three, four;
extern BOOL    MouseExists;
extern COORDS  Coord_List[30];
extern int     CurrMenu;
extern int     OldMenu;







