#define TRUE      1
#define FALSE     0
#define FAR   // __far   
#define NEAR  // __near   
#define PASCAL    _pascal
#define WINAPI
#define HINSTANCE unsigned short
#define HWND      unsigned short
#define LONG      long
#define VOID      void
#define PVOID     void *
#define FARPROC   PVOID 
#define UINT      unsigned int
//#define NPSTR     char *
//#define NPCSTR    const char *
#define HFILE     int
#define HFILE_ERROR -1 

typedef struct tagDATAFILEINFO
{
	struct	btcommo bt;
	struct	btcommo bt2;
	int      fhData;
	char     szData_File[128];
   	int      fhIndex;
	char     szIndex_File[128];
}           DATAFILEINFO;

typedef struct tagPACKETINFO
{
	DATAFILEINFO Subscriber;
	DATAFILEINFO Service;
	DATAFILEINFO Modem;
	DATAFILEINFO ModemList;
	DATAFILEINFO Spooler;
	DATAFILEINFO Message;
	DATAFILEINFO Group;
}           PACKETINFO;

typedef PACKETINFO *PPACKETINFO;
typedef PACKETINFO __far *LPPACKETINFO;
