#define MAXNUMBERofSTATES 20
#define MAXSTATECHARS 25
#define WM_RECSTATUSCHNG      WM_USER + 1989
#define WM_TRAN_CONNECT       WM_USER + 1990
#define WM_TRAN_DISCONNECT    WM_USER + 1991
                                              
#define MSK_STATUS_UPDATE				0x0001
#define MSK_LOG_UPDATE					0x0002
#define MSK_ERROR_UPDATE				0x0004

#pragma pack(1)
typedef struct tagActionState
{
	BYTE            count;
	int             (*StateProc) ();
	BYTE            Identification_String[MAXSTATECHARS];
}               ActionState;
#pragma pack()

int Initialize_State_Table();
VOID State_Verification(LPBYTE ch);
                                              
int PET_Dial(VOID);
int PET_Connect(VOID);
int PET_Logon(VOID);
int PET_SendMessage(VOID);
int PET_Accepted(VOID);
int PET_Disconnect(VOID);
int PET_Not_Accepted(VOID);
int PET_Retry(VOID);
void Pet_DeleteMessage(VOID);
