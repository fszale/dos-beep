/*
 * ========================== W A P I   API's =============================
 * 
 * SOURCE: WAPI.H
 * 
 * WRITTEN BY: Fourth Wave Software, Inc.  Copyright (C) 1992
 * 
 * Who							What										When
 * ---                     ----                             ----
 * 
 * Louis Jackson           Creation                         9/17/92
 * 
 * ========================================================================
 */
#ifndef _INC_WAPI
#define _INC_WAPI

#ifdef __cplusplus
extern          "C"
{
#endif

#ifdef WIN32

#undef  _loadds
#define _loadds
#undef  _far
#define _far
#undef  _near
#define _near
#undef  _pascal
#define _pascal
#undef  _import
#define _import
#undef  __import
#define __import
#undef  _export
#define _export
#undef  __export
#define __export

#define DllImport __declspec( dllimport )
#define DllExport __declspec( dllexport )
#else

#undef  DllImport
#undef  DllExport

#define DllImport
#define DllExport

#endif

typedef int     BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned long HWAPI;
typedef unsigned short HWAPIPAGEID;
typedef unsigned short HWAPIPAGE;

typedef void    (FAR PASCAL * WAPIPROC) (HWND, WORD, WORD, LONG);

typedef char FAR *LPSTR;
typedef const char FAR *LPCSTR;
typedef BYTE FAR *LPBYTE;
typedef int FAR *LPINT;
typedef WORD FAR *LPWORD;
typedef long FAR *LPLONG;
typedef DWORD FAR *LPDWORD;
typedef void FAR *LPVOID;

#ifndef MAKEWORD
#define MAKEWORD(low, high) ((WORD)(((BYTE)(low)) | (((WORD)((BYTE)(high))) << 8)))
#endif

#define WAPI_ID                     31
#define WAPI_LOGON_LEN              31
#define WAPI_SUBSCRIBER_NAME_LEN 	31
#define WAPI_SERVICE_NAME_LEN 		31
#define WAPI_PIN_LEN 					21
#define WAPI_MAPI_ID_LEN 				21
#define WAPI_VIM_ID_LEN 				21
#define WAPI_MHS_ID_LEN 				21
#define WAPI_X400_ID_LEN 				21
#define WAPI_SMTP_ID_LEN 				21
#define WAPI_MCI_ID_LEN 				21
#define WAPI_PROFS_ID_LEN 				21
#define WAPI_MAIL_ID_LEN 				21
#define WAPI_INSTALLED_MODEM_LEN    90
#define WAPI_MODEM_NAME_LEN         41
#define WAPI_MODEM_INIT_LEN 			61
#define WAPI_MODEM_HANGUP_LEN       11
#define WAPI_MODEM_PREFIX_LEN       31
#define WAPI_MODEM_SUFFIX_LEN       21
#define WAPI_MODEM_RESET_LEN        11
#define WAPI_MODEM_ESCAPE_LEN       11
#define WAPI_MODEM_OK_LEN           16
#define WAPI_MODEM_BUSY_LEN         16
#define WAPI_MODEM_CONNECT_LEN      16
#define WAPI_MODEM_NOCARRIER_LEN    16
#define WAPI_MODEM_NODIALTONE_LEN	16
#define WAPI_MODEM_NOANSWER_LEN		16
#define WAPI_PHONE_NO_LEN 				31
#define WAPI_GROUP_NAME_LEN 			31
#define WAPI_TIMESTAMP_LEN 			31
#define WAPI_FILENAME_LEN 				13
#define WAPI_NETWORK_NAME_LEN 		31
#define WAPI_DIR_PATH_LEN				128
#define WAPI_SIGNATURE_LEN				31
#define WAPI_NASI_SERVER_NAME_LEN	9
#define WAPI_NASI_GENERAL_NAME_LEN	9
#define WAPI_NASI_SPECIFIC_NAME_LEN	15
#define WAPI_WFW_STATION_NAME_LEN   16
#define WAPI_EOL_CHARS					80
#define WAPI_STATUS_TEXT            128
#define WAPI_OTHER_BYTES            33
#define WAPI_SUBJECT                65
#define WAPI_FORMATTED_TIME         15

	/* Database File identification */
#define SUBSCRIBER_DATABASE         1
#define SERVICE_DATABASE            2
#define GROUP_DATABASE              3
#define MESSAGES_DATABASE           4
#define SCHEDULE_DATABASE           5
#define SPOOLER_DATABASE            6
#define MODEM_DATABASE              7
#define MODEMLIST_DATABASE          8
#define INBOUND_DATABASE            9


#define MAIL_NONE       0x00000000
#define MAIL_VIM        0x00000001
#define MAIL_MAPI       0x00000002
#define MAIL_MHS        0x00000004
#define MAIL_X400       0x00000008
#define MAIL_SMTP       0x00000010
#define MAIL_MCI        0x00000020
#define MAIL_PROFS      0x00000040
#define MAIL_OTHER      0x00000080
#define MAIL_PAGER      0x00000100
#define MAIL_PAGERONLY  0x00000200

#define WBDB_MSG_TITLE_LEN      31
#define MESSAGE_TYPE1                  1

#pragma pack(1)
typedef struct tagWAPIMESSAGEINFO
{
	char            szTitle[WBDB_MSG_TITLE_LEN];
	char            szFileName[WAPI_FILENAME_LEN];
}               WAPIMESSAGEINFO;
#pragma pack()

typedef WAPIMESSAGEINFO FAR  *LPWAPIMESSAGEINFO;
typedef WAPIMESSAGEINFO NEAR *NPWAPIMESSAGEINFO;


#define SUB_TYPE1                  1

#pragma pack(1)
typedef struct tagWAPISUBSCRIBERINFO
{
	char            szSubscriberName[WAPI_SUBSCRIBER_NAME_LEN];
	char            szServiceName[WAPI_SERVICE_NAME_LEN];
	char            szPIN[WAPI_PIN_LEN];

	DWORD           dwMail;		   /* Selected */

	char            szMAPI_ID[WAPI_MAPI_ID_LEN];
	char            szVIM_ID[WAPI_VIM_ID_LEN];
	char            szMHS_ID[WAPI_MHS_ID_LEN];
	char            szX400_ID[WAPI_X400_ID_LEN];
	char            szSMTP_ID[WAPI_SMTP_ID_LEN];
	char            szMCI_ID[WAPI_MCI_ID_LEN];
	char            szPROFS_ID[WAPI_PROFS_ID_LEN];

	union
	{
		char            szOther_ID[WAPI_MAIL_ID_LEN];
		BYTE            byOther[WAPI_OTHER_BYTES];
	}  uExtra;
}               WAPISUBSCRIBERINFO;
#pragma pack()
typedef WAPISUBSCRIBERINFO FAR *LPWAPISUBSCRIBERINFO;
typedef WAPISUBSCRIBERINFO NEAR *NPWAPISUBSCRIBERINFO;

#define PET_PASSWORD_LEN 				11
#define PET_ID_REQUEST_LEN          11
#define PET_LOGON_LEN               11
#define PET_LOGOFF_LEN              11
#define PET_CONTINUE_RESPONSE_LEN   11
#define PET_ID_RESPONSE_LEN         11
#define PET_ACK_RESPONSE_LEN        11
#define PET_NAK_RESPONSE_LEN        11
#define PET_RS_RESPONSE_LEN         11
#define PET_DISCONNECT_RESPONSE_LEN	11

#define PET_MASKED_CHARS_LEN		   255

#pragma pack(1)
typedef struct tagWAPIPETINFO
{
	char            szPassword[PET_PASSWORD_LEN];
	WORD            wMaxMsgLength;
	char            szID_Request[PET_ID_REQUEST_LEN];
	BYTE            byID_Request_Max;
	BYTE            byID_Response_Delay;
	char            szLogon[PET_LOGON_LEN];
	BYTE            byLogon_NAK_Max;
	BYTE            byLogon_Delay;
	BYTE            byGo_Ahead_Delay;
	BYTE            byMessage_NAK_Max;
	BYTE            byMessage_Delay;
	char            szLogoff[PET_LOGOFF_LEN];
	BYTE            byLogoff_Retries;
	BYTE            byLogoff_Delay;
	BYTE            byHostRetry_Max;
	BYTE            byRetry_Max;
	BYTE            byMultipleTransmit;
	char            szMasked_Chars[PET_MASKED_CHARS_LEN];
	char            szID_Response[PET_ID_RESPONSE_LEN];
	char            szNAK_Response[PET_NAK_RESPONSE_LEN];
	char            szACK_Response[PET_ACK_RESPONSE_LEN];
	char            szRS_Response[PET_RS_RESPONSE_LEN];
	char            szDisconnect_Response[PET_DISCONNECT_RESPONSE_LEN];
	char            szContinue_Response[PET_CONTINUE_RESPONSE_LEN];
}               WAPIPETINFO;
#pragma pack()
typedef WAPIPETINFO FAR *LPWAPIPETINFO;
typedef WAPIPETINFO NEAR *NPWAPIPETINFO;

#pragma pack(1)
typedef struct tagWAPINONEINFO
{
	BYTE            fActivePort;
}               WAPINONEINFO;
#pragma pack()
typedef WAPINONEINFO FAR *LPWAPINONEINFO;
typedef WAPINONEINFO NEAR *NPWAPINONEINFO;


/* Outbound protocols */
#define OUTBOUND_PET                    1
#define OUTBOUND_TAP                    1
#define OUTBOUND_IXO                    1
#define OUTBOUND_BEYOND                 2
#define OUTBOUND_DTMF                   3		/* Numeric paging */
#define OUTBOUND_MOBIPAGE               4

	/* Inbound protocols */
#define INBOUND_NONE                    10
#define INBOUND_PET                     11
#define INBOUND_TAP                     11
#define INBOUND_IXO                     11
#define INBOUND_NEWSSTREAM					 12
#define INBOUND_MOBIPAGE                13

#define SERVICE_TYPE1                  1
#define SERVICE_OUTBOUND               1

#define SERVICE_TYPE2                  2
#define SERVICE_INBOUND                2

#pragma pack(1)
typedef struct tagWAPISERVICEINFO
{
	char            szServiceName[WAPI_SERVICE_NAME_LEN];
	char            szInstalledModem[WAPI_INSTALLED_MODEM_LEN];
	char            szPhoneNumber[WAPI_PHONE_NO_LEN];
	BYTE            byDataBits;
	BYTE            byParity;
	BYTE            byStopBits;
	DWORD           dwBaud;
	BYTE            fDuplex;
	BYTE            fXON_XOFF;
	BYTE            fCTS_RTS;
	char            cEOLChar[WAPI_EOL_CHARS];
	BYTE            byEOLLength;
	BYTE            byEOLType;
	BYTE            byProtocol;	   /* Outbound protocols - PET = 1, ... */

	union
	{
		WAPIPETINFO     Pet;
		WAPINONEINFO    None;
	}               uProtocol;
}               WAPISERVICEINFO;
#pragma pack()
typedef WAPISERVICEINFO FAR *LPWAPISERVICEINFO;
typedef WAPISERVICEINFO NEAR *NPWAPISERVICEINFO;


#define GROUP_TYPE1                 0x01
#define GROUP_TYPE2                 0xFE
#define GROUP_TYPE3                 0xFF

#pragma pack(1)
typedef struct tagWAPIGROUPINFO
{
	char            szGroupName[WAPI_GROUP_NAME_LEN];
	char            szSubscriberName[WAPI_SUBSCRIBER_NAME_LEN];
}               WAPIGROUPINFO;
#pragma pack()
typedef WAPIGROUPINFO FAR *LPWAPIGROUPINFO;
typedef WAPIGROUPINFO NEAR *NPWAPIGROUPINFO;

#define MODEM_DTR                  0x01
#define MODEM_CELLULAR             0x02

#define MODEMLIST_TYPE1                  1
#pragma pack(1)
typedef struct tagWAPIMODEMLISTINFO
{
	char            szModemName[WAPI_MODEM_NAME_LEN];
	char            szInitString[WAPI_MODEM_INIT_LEN];
	char            szHangupString[WAPI_MODEM_HANGUP_LEN];
	char            szDialPrefixString[WAPI_MODEM_PREFIX_LEN];
	char            szDialSuffixString[WAPI_MODEM_SUFFIX_LEN];
	char            szResetString[WAPI_MODEM_RESET_LEN];
	char            szEscapeString[WAPI_MODEM_ESCAPE_LEN];
	char            szOK_Response[WAPI_MODEM_OK_LEN];		/* "OK" */
	char            szBusy_Response[WAPI_MODEM_BUSY_LEN];	/* BUSY */
	char            szConnect_Response[WAPI_MODEM_CONNECT_LEN];		/* CONNECT */
	char            szNoCarrier_Response[WAPI_MODEM_NOCARRIER_LEN];	/* NO CARRIER */
	char            szNoDialtone_Response[WAPI_MODEM_NODIALTONE_LEN];		/* NO DIALTONE */
	char            szNoAnswer_Response[WAPI_MODEM_NOANSWER_LEN];	/* NO ANSWER */
	BYTE            byReset_Delay;
	BYTE            byHangUp_Delay;
	BYTE            byEscape_Delay;
	BYTE            byInit_Delay;
	BYTE            byDial_Delay;
	BYTE            byConnect_Delay;
	BYTE            byRedial_Delay;
	BYTE            byReDial_Count;
	BYTE            fOptions;
}               WAPIMODEMLISTINFO;
#pragma pack()
typedef WAPIMODEMLISTINFO FAR *LPWAPIMODEMLISTINFO;
typedef WAPIMODEMLISTINFO NEAR *NPWAPIMODEMLISTINFO;


#define COMM_NONE          0x01		   /* Just place in the queue, don't send */
#define COMM_LOCAL         0x02
#define COMM_INT14         0x04
#define COMM_NASI          0x08
#define COMM_WRKGRP        0x10
#define COMM_LOCAL_DIRECT  0x12
#define COMM_MOBIDEM       0x14


#define MODEM_TYPE1                  1
#pragma pack(1)
typedef struct tagWAPIMODEMINFO
{
	char            szInstalledName[WAPI_INSTALLED_MODEM_LEN];
	WAPIMODEMLISTINFO ModemInfo;
	BYTE            byCOMMType;
	BYTE            byCOMMPort;

	/*
	 * -------------------------------------------------------
	 */
	/*
	 * ONLY USED when the destination is NOT the final ServiceInfo
	 * Destination. (i.e. MOBIDEM connections...)
	 */
	BYTE            byDataBits;
	BYTE            byParity;
	BYTE            byStopBits;
	DWORD           dwBaud;
	/*
	 * -------------------------------------------------------
	 */

	WORD            wCommThreadPriority;	/* For multi-processing OS's (i.e. NT) */

	char            szNASIServer[WAPI_NASI_SERVER_NAME_LEN];
	char            szNASIGeneral[WAPI_NASI_GENERAL_NAME_LEN];
	char            szNASISpecific[WAPI_NASI_SPECIFIC_NAME_LEN];

	char            szWFWStation[WAPI_WFW_STATION_NAME_LEN];
}               WAPIMODEMINFO;
#pragma pack()
typedef WAPIMODEMINFO FAR *LPWAPIMODEMINFO;
typedef WAPIMODEMINFO NEAR *NPWAPIMODEMINFO;

#define PRIORITY_HIGH                  0x0001
#define PRIORITY_NORMAL                0x0002
#define PRIORITY_LOW                   0x0003
#define PRIORITY_INCREASE              0xFFFD
#define PRIORITY_DECREASE              0xFFFE
#define PRIORITY_PAUSE                 0xFFFF


#define CLASS_DONT_FORWARD             0x8000

#define SPOOLER_TYPE1                  1
#pragma pack(1)
typedef struct tagWAPISPOOLERINFO
{
	HWAPI           wapiInst;	   /* Used to identify Callback funtion */
	char            szWAPILogon[WAPI_LOGON_LEN];

	HWAPIPAGEID     wapiPageID;	   /* Unique ID for message
								    * identification */
	HWAPIPAGE       wapiPage;	   /* Unique ID for message record
								    * identification */

	WAPISUBSCRIBERINFO SubscriberInfo;
	WAPISERVICEINFO ServiceInfo;
	WAPIMODEMINFO   ModemInfo;

	WORD            wPriority;
	WORD            wClass;

	char            szTimeStamp[WAPI_TIMESTAMP_LEN];
	char            szFileName[WAPI_FILENAME_LEN];

	/*
	 * Used for DDE Connections all other connection info is passed to
	 * the callback
	 */
	char            szNetworkName[WAPI_NETWORK_NAME_LEN];
	DWORD           dwDDEId;
}               WAPISPOOLERINFO;
#pragma pack()
typedef WAPISPOOLERINFO FAR *LPWAPISPOOLERINFO;
typedef WAPISPOOLERINFO NEAR *NPWAPISPOOLERINFO;


#define INBOUND_TYPE1                  1

#pragma pack(1)
typedef struct tagWAPIINBOUNDINFO
{
	char            szWAPILogon[WAPI_LOGON_LEN];
	HWAPIPAGEID     wapiPageID;
	HWAPIPAGE       wapiPage;
	char            szTimeStamp[WAPI_TIMESTAMP_LEN];
	char            szFileName[WAPI_FILENAME_LEN];
	DWORD           dwFileType;
   char            szSubject[WAPI_SUBJECT];
   char            szDevTimeRcvd[WAPI_FORMATTED_TIME];
   BYTE            szDevTimeRcvdFormat;
   char            szDevTimeSent[WAPI_FORMATTED_TIME];
   BYTE            szDevTimeSentFormat;
   DWORD           dwICN;
   DWORD           dwAddress;
}               WAPIINBOUNDINFO;
#pragma pack()
typedef WAPIINBOUNDINFO FAR  *LPWAPIINBOUNDINFO;
typedef WAPIINBOUNDINFO NEAR *NPWAPIINBOUNDINFO;


#define NET_NONE             0x00000000
#define NET_INT14            0x00000001
#define NET_NASI             0x00000002
#define NET_WFW              0x00000004
#define NET_MOBIDEM			  0x00000008

#pragma pack(1)
typedef struct tagWAPIINFO
{
	HWAPI           hWapiInstance;
	char            szWAPILogon[WAPI_LOGON_LEN];

	DWORD           dwFlags;	   /* Internal Use Only  */
	DWORD           dwOptions;	   /* Internal Use Only  */

	char            szDefaultModemName[WAPI_INSTALLED_MODEM_LEN];

	DWORD           dwMail;		   /* Available */
	DWORD           dwMailSelection;		/* Selected */

	BYTE            fNetWork;	   /* Is this a network app */
	DWORD           dwNetWork;	   /* Availible Network Features */

	char            szActiveSpooler[WAPI_DIR_PATH_LEN];

	char            szWAPI_INI[WAPI_DIR_PATH_LEN];
	char            szPhoneBookDir[WAPI_DIR_PATH_LEN];
	char            szSpoolDir[WAPI_DIR_PATH_LEN];
	char            szPredefinedMsgs[WAPI_DIR_PATH_LEN];
	char            szLogDir[WAPI_DIR_PATH_LEN];
	char            szDataDir[WAPI_DIR_PATH_LEN];
   PACKETINFO      PacketInfo;
}               WAPIINFO;
#pragma pack()
typedef WAPIINFO FAR *LPWAPIINFO;
typedef WAPIINFO NEAR *NPWAPIINFO;

#define WAPI_STAT_SUCCESS   0
#define WAPI_STAT_INIT      1
#define WAPI_STAT_DIALING   2
#define WAPI_STAT_CONNECT   3
#define WAPI_STAT_LOGON     4
#define WAPI_STAT_SENDING   5
#define WAPI_STAT_ABORTING  6
#define WAPI_STAT_RETRY     7
#define WAPI_STAT_DISCON    8
#define WAPI_STAT_REDIAL    9
#define WAPI_STAT_TIMEOUT   10

	/* WAPI Callback Messages */
#define MSK_NONE            0x0000
#define WM_WAPI_FIRST	    0x0001

#define WM_LOGON				 0x0001
#define WM_LOGOFF				 0x0002
#define WM_SUBSCRIBERCHANGE 0x0004
#define WM_SERVICECHANGE 	 0x0008
#define WM_GROUPCHANGE 	    0x0010
#define WM_SPOOLERCHANGE 	 0x0020
#define WM_MODEMCHANGE 	    0x0040
#define WM_WAPIINICHANGE	 0x0080
#define WM_DEFMODEMCHANGE	 0x0100
#define WM_STATUSCHANGE	    0x0200
#define WM_ERRORREPORT      0x0400
#define WM_SCHEDULECHANGE   0x0800
#define WM_MESSAGECHANGE    0x1000
#define WM_INBOUNDCHANGE 	 0x2000

#define MSK_INDEXCHECK      0x8000
#define MSK_ALL	          0xFFFF
#define WM_WAPI_LAST	       0x2000

#pragma pack(1)
typedef struct tagWAPISTATUSINFO
{
	HWAPI           wapiInst;
	HWAPIPAGEID     wapiPageID;
	WORD            wStatus;
	char            szStatusText[WAPI_STATUS_TEXT];
}               WAPISTATUSINFO;
#pragma pack()
typedef WAPISTATUSINFO FAR *LPWAPISTATUSINFO;
typedef WAPISTATUSINFO NEAR *NPWAPISTATUSINFO;


	/* WAPI function Return Codes */
#define WAPI_SUCCESS       0
#define WAPI_RC_BTERROR   -1
#define WAPI_RC_BTNOTFND  -2
#define WAPI_RC_BTCOMMO   -3
#define WAPI_RC_NOMEM     -4
#define WAPI_RC_BTCPYBRO  -5
#define WAPI_RC_BTLOCK    -6
#define WAPI_RC_BTUNLOCK  -7
#define WAPI_RC_BTABORT   -9
#define WAPI_RC_BTCKERROR -10
#define WAPI_RC_BUSY      -11

#define WAPI_NOT_LOGGED_ON -100
#define WAPI_UNKNOWN_TYPE  -101
#define WAPI_INVALID_PARAM -102


	/* WAPI function Error Codes */
#define WAPI_EC_CK_NOMEM  1
#define WAPI_EC_CK_BTRIN  2
#define WAPI_EC_CK_OPEN   3
#define WAPI_EC_CK_SEEK   4
#define WAPI_EC_CK_READ   5
#define WAPI_EC_CK_WRIT   6
#define WAPI_EC_CK_CBTR   7
#define WAPI_EC_CK_STAK   8
#define WAPI_EC_CK_PUSH   9
#define WAPI_EC_CK_INDX  10

/*
 * - - - - - - - - - - - - - WAPI PROTOTYPES - - - - - - - - - - - - - -
 */

DllExport int WINAPI __export WAPILogon(HINSTANCE hInst, HWND hwnd, LPSTR lpLogon, LPWAPIINFO lpInfo, WAPIPROC lpProc, DWORD dwFlags);
DllExport BOOL WINAPI __export WAPILogoff(LPWAPIINFO lpInfo, DWORD dwFlags);

DllExport WORD WINAPI __export WAPIVersion(DWORD dwFlags, LPDWORD lpdwWapi, LPDWORD lpdwBook, LPDWORD lpdwSpooler);

DllExport int WINAPI __export WAPIUpdateDirectories(LPWAPIINFO lpInfo, DWORD dwFlags);
DllExport int WINAPI __export WAPIGetInfo(LPWAPIINFO lpInfo, DWORD dwFlags);
DllExport int WINAPI __export WAPISetInfo(LPWAPIINFO lpInfo, DWORD dwFlags);

DllExport int WINAPI __export WAPISetStatusInfo(HWAPI hInst, HWAPIPAGEID hID, WORD wStat, LPSTR lpText, DWORD dwFlags);

DllExport BOOL WINAPI __export WAPI_FileExist(LPSTR pFullPathName);
DllExport VOID WINAPI __export WAPIRemoveTrailingBlanks(LPSTR lpStr);
DllExport VOID WINAPI __export WAPIRemoveCtlChars(LPSTR lpStr);
DllExport BOOL WINAPI __export WAPIWritePrivateProfileInt(LPSTR lpApp, LPSTR lpKey, int nValue, LPSTR lpFile);

DllExport BOOL WINAPI __export WAPIEnumAddressBooks(LPWAPIINFO lpInfo,  FARPROC fnCallback, DWORD dwFlags);
DllExport BOOL WINAPI __export WAPIEnumSpoolers(LPWAPIINFO lpInfo,  FARPROC fnCallback, DWORD dwFlags);

DllExport int WINAPI __export WAPIRegisterNotification(LPWAPIINFO lpInfo, WAPIPROC lpProc, DWORD dwMask, LPDWORD lpdwFlags);
DllExport int WINAPI __export WAPISetMaskNotification(LPWAPIINFO lpInfo, DWORD dwMask, LPDWORD lpdwFlags);
DllExport int WINAPI __export WAPIGetMaskNotification(LPWAPIINFO lpInfo, LPDWORD lpdwMask, DWORD dwFlags);

DllExport int WINAPI __export WAPIGetStatus(LPWAPIINFO lpInfo, HWAPIPAGEID hwapiPage, DWORD dwFlags, LPSTR lpStat, UINT nLen);

/* - - - - - - - - - - WAPI SUBSCRIBER PROTOTYPES - - - - - - - - - - - -  */

DllExport int WINAPI __export WAPISubscriberGetFirst(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);
DllExport int WINAPI __export WAPISubscriberGetNext(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);
DllExport int WINAPI __export WAPISubscriberGetPrevious(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);
DllExport int WINAPI __export WAPISubscriberGetLast(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);

DllExport int WINAPI __export WAPISubscriberGetPartial(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);
DllExport int WINAPI __export WAPISubscriberGetExact(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);

DllExport int WINAPI __export WAPISubscriberInsert(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);
DllExport int WINAPI __export WAPISubscriberDelete(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);
DllExport int WINAPI __export WAPISubscriberUpdate(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);

DllExport LONG WINAPI __export WAPISubscriberGetCount(LPWAPIINFO lpInfo, WORD nType, DWORD dwFlags);
DllExport BOOL WINAPI __export WAPIIsSubscriber(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSub, DWORD dwFlags);

DllExport int WINAPI __export WAPIIsServiceUsed(LPWAPIINFO lpInfo, LPSTR lpService, DWORD dwFlags);

DllExport BOOL WINAPI __export WAPIFreeSubscriber(LPWAPIINFO lpInfo, DWORD dwFlags);

/*
 * - - - - - - - - - - - - WAPI GROUP PROTOTYPES - - - - - - - - - - - -
 */

DllExport int WINAPI __export WAPIGroupGetFirst(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport int WINAPI __export WAPIGroupGetNext(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport int WINAPI __export WAPIGroupGetPrevious(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport int WINAPI __export WAPIGroupGetLast(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);

DllExport int WINAPI __export WAPIGroupGetPartial(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport int WINAPI __export WAPIGroupGetExact(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);

DllExport int WINAPI __export WAPIGroupDelete(LPWAPIINFO lpInfo, WORD nType, LPSTR lpGroup, DWORD dwFlags);

DllExport LONG WINAPI __export WAPIGroupGetCount(LPWAPIINFO lpInfo, WORD nType, DWORD dwFlags);
DllExport BOOL WINAPI __export WAPIIsGroup(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);

DllExport int WINAPI __export WAPIGroupInsertMember(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport int WINAPI __export WAPIGroupDeleteMember(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport int WINAPI __export WAPIGroupUpdateMember(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);

DllExport int WINAPI __export WAPIGroupDeleteSubscriber(LPWAPIINFO lpInfo, WORD nType, LPSTR lpSubscriber, DWORD dwFlags);

DllExport int WINAPI __export WAPIGroupGetFirstMember(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport int WINAPI __export WAPIGroupGetNextMember(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport int WINAPI __export WAPIGroupGetPrevMember(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);

DllExport LONG WINAPI __export WAPIGroupGetCountMember(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);
DllExport BOOL WINAPI __export WAPIIsGroupMember(LPWAPIINFO lpInfo, WORD nType, LPVOID lpGroup, DWORD dwFlags);

DllExport BOOL WINAPI __export WAPIFreeGroup(LPWAPIINFO lpInfo, DWORD dwFlags);

/*
 * - - - - - - - - - - - - WAPI SERVICE PROTOTYPES - - - - - - - - - - -
 */

DllExport int WINAPI __export WAPIServiceGetFirst(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);
DllExport int WINAPI __export WAPIServiceGetNext(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);
DllExport int WINAPI __export WAPIServiceGetPrevious(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);
DllExport int WINAPI __export WAPIServiceGetLast(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);

DllExport int WINAPI __export WAPIServiceGetPartial(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);
DllExport int WINAPI __export WAPIServiceGetExact(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);

DllExport int WINAPI __export WAPIServiceInsert(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);
DllExport int WINAPI __export WAPIServiceDelete(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);
DllExport int WINAPI __export WAPIServiceUpdate(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);

DllExport LONG WINAPI __export WAPIServiceGetCount(LPWAPIINFO lpInfo, WORD nType, DWORD dwFlags);
DllExport BOOL WINAPI __export WAPIIsService(LPWAPIINFO lpInfo, WORD nType, LPVOID lpService, DWORD dwFlags);

DllExport BOOL WINAPI __export WAPIFreeService(LPWAPIINFO lpInfo, DWORD dwFlags);

DllExport int WINAPI __export WAPIIsInstalledModemUsed(LPWAPIINFO lpInfo, LPSTR lpModem, DWORD dwFlags);

/*
 * - - - - - - - - - - - - WAPI SPOOLER PROTOTYPES - - - - - - - - - - -
 */

DllExport int WINAPI __export WAPIStartSpooler(LPWAPIINFO lpInfo, DWORD dwFlags);
DllExport int WINAPI __export WAPIStopSpooler(LPWAPIINFO lpInfo, DWORD dwFlags);

DllExport int WINAPI __export WAPISpoolerGetFirst(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);
DllExport int WINAPI __export WAPISpoolerGetNext(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);
DllExport int WINAPI __export WAPISpoolerGetPrevious(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);
DllExport int WINAPI __export WAPISpoolerGetLast(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);

DllExport HWAPIPAGEID WINAPI __export WAPISpoolerInsert(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, LPSTR lpMessage, BOOL fNewMessage, DWORD dwFlags);
DllExport HWAPIPAGEID WINAPI __export WAPISpoolerQuickInsert(LPWAPIINFO lpInfo, LPSTR lpPIN, LPSTR lpService, LPSTR lpMessage, DWORD dwFlags);

DllExport int WINAPI __export WAPISpoolerDeleteFirst(LPWAPIINFO lpInfo, DWORD dwFlags);
DllExport LONG WINAPI __export WAPISpoolerDelete(LPWAPIINFO lpInfo, HWAPIPAGEID wapiPage, DWORD dwFlags);
DllExport LONG WINAPI __export WAPISpoolerDeleteSpecific(LPWAPIINFO lpInfo, LPWAPISPOOLERINFO lpSpool, DWORD dwFlags);
DllExport HWAPIPAGEID WINAPI __export WAPISpoolerTransfer(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);

DllExport LONG WINAPI __export WAPISpoolerChangePriority(LPWAPIINFO lpInfo, LPWAPISPOOLERINFO lpSpool, WORD wNewPriority, DWORD dwFlags);

DllExport LONG WINAPI __export WAPISpoolerGetCount(LPWAPIINFO lpInfo, WORD nType, DWORD dwFlags);
DllExport int WINAPI __export WAPIIsFileNameUsed(LPWAPIINFO lpInfo, LPSTR lpFileName, DWORD dwFlags);
DllExport int WINAPI __export WAPISpoolerGetPage(LPWAPIINFO lpInfo, HWAPIPAGEID wapiPage, LPWAPISPOOLERINFO lpSpool, DWORD dwFlags);

DllExport BOOL WINAPI __export WAPIFreeSpooler(LPWAPIINFO lpInfo, DWORD dwFlags);

DllExport int WINAPI __export WAPISpoolFill(LPWAPIINFO lpInfo, LPWAPISPOOLERINFO lpSpool, DWORD dwFlags);

/*
 * - - - - - - - - - - - - WAPI SIMPLE PROTOTYPES - - - - - - - - - - -
 */
DllExport int WINAPI __export WAPISimpleModemInstall(HWND hwnd, LPWAPIINFO lpInfo, DWORD dwFlags);
DllExport int WINAPI __export WAPISimpleSender(HWND hwnd, LPWAPIINFO lpInfo, DWORD dwFlags);
DllExport int WINAPI __export WAPISimpleSave(HWND hwnd, LPWAPIINFO lpInfo, DWORD dwFlags);

DllExport int WINAPI __export WAPISimpleSubscriber(HWND hwnd, LPWAPIINFO lpInfo, DWORD dwFlags);
DllExport int WINAPI __export WAPISimpleService(HWND hwnd, LPWAPIINFO lpInfo, DWORD dwFlags);

DllExport int WINAPI __export WAPISimpleSend(LPSTR lpDestination, LPSTR lpMessage);

/*
 * - - - - - - - - - - WAPI PREDEFINED MODEM LIST PROTOTYPES - - - - - -
 */
DllExport int WINAPI __export WAPIModemListGetFirst(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemListGetNext(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemListGetPrevious(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemListGetLast(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);

DllExport int WINAPI __export WAPIModemListGetPartial(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemListGetExact(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);

DllExport int WINAPI __export WAPIModemListInsert(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemListDelete(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemListUpdate(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);

DllExport LONG WINAPI __export WAPIModemListGetCount(LPWAPIINFO lpInfo, WORD nType, DWORD dwFlags);
DllExport BOOL WINAPI __export WAPIIsModemList(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);

DllExport BOOL WINAPI __export WAPIFreeModemList(LPWAPIINFO lpInfo, DWORD dwFlags);


/*
 * - - - - - - - - - - WAPI INSTALLED MODEM PROTOTYPES - - - - - - - - -
 * - - -
 */
DllExport int WINAPI __export WAPIModemGetFirst(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemGetNext(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemGetPrevious(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemGetLast(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);

DllExport int WINAPI __export WAPIModemGetPartial(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemGetExact(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);

DllExport int WINAPI __export WAPIModemInsert(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemDelete(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);
DllExport int WINAPI __export WAPIModemUpdate(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);

DllExport LONG WINAPI __export WAPIModemGetCount(LPWAPIINFO lpInfo, WORD nType, DWORD dwFlags);
DllExport BOOL WINAPI __export WAPIIsModem(LPWAPIINFO lpInfo, WORD nType, LPVOID lpModem, DWORD dwFlags);

DllExport BOOL WINAPI __export WAPIFreeModem(LPWAPIINFO lpInfo, DWORD dwFlags);

DllExport int WINAPI __export WAPI_BuildInstalledModemKey(LPSTR lpBuf, WORD nType, LPWAPIMODEMINFO lpModem, DWORD dwFlags);

/* - - - - - - - - - - WAPI TABLE PROTOTYPES - - - - - - - - - - - -  */
DllExport LPSTR WINAPI __export GetSubscriberTable(LPWAPIINFO lpInfo, LPLONG lpLong, LPWORD lpRecSize, DWORD dwFlags);
DllExport VOID WINAPI __export SubscriberTableFill(LPWAPIINFO lpInfo, HWND hwnd, BYTE fListBox, DWORD dwFlags);

DllExport LPSTR WINAPI __export GetServiceTable(LPWAPIINFO lpInfo, LPLONG lpLong, LPWORD lpRecSize, DWORD dwFlags);
DllExport VOID WINAPI __export ServiceTableFill(LPWAPIINFO lpInfo, HWND hwnd, BYTE fListBox, DWORD dwFlags);

DllExport LPSTR WINAPI __export GetGroupTable(LPWAPIINFO lpInfo, LPLONG lpLong, LPWORD lpRecSize, DWORD dwFlags);
DllExport VOID WINAPI __export GroupTableFill(LPWAPIINFO lpInfo, HWND hwnd, BYTE fListBox, DWORD dwFlags);

DllExport LPSTR WINAPI __export GetModemListTable(LPWAPIINFO lpInfo, LPLONG lpLong, LPWORD lpRecSize, DWORD dwFlags);
DllExport VOID WINAPI __export ModemListTableFill(LPWAPIINFO lpInfo, HWND hwnd, BYTE fListBox, DWORD dwFlags);

DllExport LPSTR WINAPI __export GetModemTable(LPWAPIINFO lpInfo, LPLONG lpLong, LPWORD lpRecSize, DWORD dwFlags);
DllExport VOID WINAPI __export ModemTableFill(LPWAPIINFO lpInfo, HWND hwnd, BYTE fListBox, DWORD dwFlags);

/*
 * - - - - - - - - - - - - WAPI INBOUND PROTOTYPES - - - - - - - - - - -
 */

DllExport int WINAPI __export WAPIInboundGetFirst(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);
DllExport int WINAPI __export WAPIInboundGetNext(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);
DllExport int WINAPI __export WAPIInboundGetPrevious(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);
DllExport int WINAPI __export WAPIInboundGetLast(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, DWORD dwFlags);

DllExport int WINAPI __export WAPIInboundGetExact(LPWAPIINFO lpInfo, WORD nType, LPVOID lpInbound, DWORD dwFlags);

DllExport HWAPIPAGEID WINAPI __export WAPIInboundInsert(LPWAPIINFO lpInfo, WORD nType, LPVOID lpSpool, LPSTR lpMessage, BOOL fNewMessage, DWORD dwFlags);

DllExport int WINAPI __export WAPIInboundDeleteFirst(LPWAPIINFO lpInfo, DWORD dwFlags);
DllExport LONG WINAPI __export WAPIInboundDeleteSpecific(LPWAPIINFO lpInfo, LPWAPIINBOUNDINFO lpSpool, DWORD dwFlags);

DllExport LONG WINAPI __export WAPIInboundGetCount(LPWAPIINFO lpInfo, WORD nType, DWORD dwFlags);

DllExport BOOL WINAPI __export WAPIFreeInbound(LPWAPIINFO lpInfo, DWORD dwFlags);

/*
 * - - - - - - - - - - - - - WAPI PROTOTYPES - - - - - - - - - - - - - -
 */


#ifdef __cplusplus
}
#endif

#endif
