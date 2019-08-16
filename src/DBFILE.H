// this mask will go into wClass member
// of the WAPISPOOLERINFO struct.
//
// it allows DosBEEP to recognize when
// to delete the *.spl file for the last
// group member
#define LAST_TO_ACCESS  0x0001



#define ERR                      -1
#define CR(btloc) (btloc + sizeof(WORD))
#define MAX_FILE_PATH 128
#define BYTE_MAX 5
#define MAX_KEY_LEN                128

#define MODEM_DTR                  0x01
#define MODEM_CELLULAR             0x02

#define SPOOLER_DB                 1
#define SUBSCRIBER_DB              2
#define SERVICE_DB                 3
#define MODEM_DB                   4
#define MODEMLIST_DB               5
#define GROUP_DB                   6
#define MESSAGE_DB                 7

#define SPOOLER_DATA         "SPOOLER.DAT"
#define SPOOLER_INDEX        "SPOOLER.IDX"
#define SERVICE_DATA         "SERVICE.DAT"
#define SERVICE_INDEX        "SERVICE.IDX"
#define SUBSCRIBER_DATA      "SUBSCRIB.DAT"
#define SUBSCRIBER_INDEX     "SUBSCRIB.IDX"
#define MODEM_DATA           "MODEM.DAT"
#define MODEM_INDEX          "MODEM.IDX"
#define MODEMLIST_DATA       "MODEMLST.DAT"
#define MODEMLIST_INDEX      "MODEMLST.IDX"
#define GROUP_DATA           "GROUP.DAT"
#define GROUP_INDEX          "GROUP.IDX"
#define MESSAGE_DATA         "MESSAGE.DAT"
#define MESSAGE_INDEX        "MESSAGE.IDX"

#define DEFAULT_MODEM_CONNECTION "Use Default Connection"

#define INVALID_DB_TYPE      1
#define CALL_SUCCESS         2
#define CALL_FAILURE         3
#define BTOPEN_ERROR         4
#define BTCINIT_ERROR        5
#define BTSEEKV_ERROR        6
#define CBTREE_ERROR         7
#define BUILD_KEY_ERROR      8
#define BTREAD_ERROR         9
#define BTWRITE_ERROR		 10
#define INVALID_PARAM		 11

#define SPL "SPL"
#define PDM "PDM"

WORD Open_DBFile(LPWAPIINFO lpInfo, WORD wFlag, LPSTR szPath);
WORD Close_DBFile(LPWAPIINFO lpInfo, WORD wFlag);
WORD BuildInstalledModemKey(LPSTR lpBuf, WORD nType, LPVOID lpModem);

WORD InitTree(LPWAPIINFO lpInfo, WORD wFlag);

WORD Spooler_GetfirstRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Spooler_GetnextRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Spooler_GetanyRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Spooler_InsertRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer, LPSTR lpMessage);
WORD Spooler_DeleteRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Spooler_Fill(LPWAPIINFO lpInfo, WORD wFlag, LPWAPISPOOLERINFO lpSpool);
WORD Spooler_IsFileName(LPWAPIINFO lpInfo, LPSTR lpszFileName, WORD);

WORD Subscriber_GetfirstRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Subscriber_GetnextRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Subscriber_GetanyRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Subscriber_InsertRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Subscriber_DeleteRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);

WORD Service_GetfirstRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Service_GetnextRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Service_GetanyRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Service_InsertRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Service_DeleteRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);

WORD Modem_GetfirstRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Modem_GetnextRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Modem_GetanyRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Modem_InsertRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Modem_DeleteRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);

WORD ModemList_GetfirstRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD ModemList_GetnextRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD ModemList_GetanyRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD ModemList_InsertRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD ModemList_DeleteRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
                        
WORD Message_GetfirstRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Message_GetnextRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Message_GetanyRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Message_InsertRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer, LPSTR lpMessage);
WORD Message_DeleteRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);

WORD Group_GetfirstRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Group_GetnextRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Group_GetanyRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Group_InsertRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);
WORD Group_DeleteRec(LPWAPIINFO lpInfo, WORD nType, WORD wFlag, PVOID buffer);

LPWAPIINFO allocate_Wwapiinfo(void);
BOOL free_wapiinfo(LPWAPIINFO lpWapiInfo);

void AnsiUpper(LPSTR lp); 
HWAPIPAGEID CreateUniqueFileName(LPSTR lpString, LPSTR lpDir, LPSTR lpExt);
VOID RemoveCtlChars(LPSTR lpStr);
VOID RemoveTrailingBlanks(LPSTR lpStr);
BOOL FileExist(LPSTR lpFullPathName);


