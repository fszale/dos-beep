#include <string.h>
#include <dos.h>
#include <direct.h>
#include <fcntl.h>
#include <io.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <share.h>
#include <graph.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "menu.h"
extern "C"
{
#include "cbtree.h"
}
#include "convert.h"
#include "wapi.h"
#include "dbfile.h"
#include "dosbeep.h"

extern BOOL            AltKey;
extern char            message[80];
extern char            szPrinter[2];
extern struct videoconfig vc;
extern WAPIINFO        WapiInfo;
extern DosBEEPINFO     BeepInfo;
//--------------------------------------------------------------------
//
// GetINIValues()
//
//--------------------------------------------------------------------
void GetINIValues()
{
   char szBuffer[512];
   char szBuffer2[512];

   WapiInfo.dwOptions = 0;

   if(GetProfileString("WAPI", "INI", "", szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
       strcpy(szBuffer2, "WAPI.INI");
   else
       strcpy(szBuffer2, szBuffer);

   if(FileExist(szBuffer2) == TRUE)
   {
       GetProfileString("Options", "DefModem", "", szBuffer, sizeof(szBuffer), szBuffer2);
       strcpy(WapiInfo.szDefaultModemName, szBuffer);
       
       GetProfileString("Directories", "DataFiles", "", szBuffer, sizeof(szBuffer), szBuffer2);
       strcpy(WapiInfo.szDataDir, szBuffer);
       
       GetProfileString("Directories", "Phonebook", "", szBuffer, sizeof(szBuffer), szBuffer2);
       strcpy(WapiInfo.szPhoneBookDir, szBuffer);
       
       GetProfileString("Directories", "Spool", "", szBuffer, sizeof(szBuffer), szBuffer2);
       strcpy(WapiInfo.szSpoolDir, szBuffer);

       GetProfileString("Directories", "Predefined", "", szBuffer, sizeof(szBuffer), szBuffer2);
       strcpy(WapiInfo.szPredefinedMsgs, szBuffer);

       GetProfileString("Directories", "LogDirectory", "", szBuffer, sizeof(szBuffer), szBuffer2);
       strcpy(WapiInfo.szLogDir, szBuffer);

       GetProfileString("DosBEEP_Setup", "PrinterDev", "0", szBuffer, sizeof(szBuffer), szBuffer2);
       switch(atoi(szBuffer))
       {
          case 1:
              BeepInfo.nPrinterPort = PRINTER_PORT_1;
          break;
          case 2:
              BeepInfo.nPrinterPort = PRINTER_PORT_2;
          break;
          case 3:
              BeepInfo.nPrinterPort = PRINTER_PORT_3;
          break;
          case 0:
          default:
              BeepInfo.nPrinterPort = PRINTER_PORT_NONE;
          break;
       }

       GetProfileString("Options", "Options", "0", szBuffer, sizeof(szBuffer), szBuffer2);
       WapiInfo.dwOptions = (DWORD) atol(szBuffer);

       GetProfileString("Options", "Flags", "0", szBuffer, sizeof(szBuffer), szBuffer2);
       WapiInfo.dwFlags = (DWORD) atol(szBuffer);

       GetProfileString("Options", "PurgeOldAge", "7", szBuffer, sizeof(szBuffer), szBuffer2);
       BeepInfo.nDaysOld = atoi(szBuffer);

       GetProfileString("Signature", "DefaultSignature", "", szBuffer, sizeof(szBuffer), szBuffer2);
       strcpy(BeepInfo.szDefaultSignature, szBuffer);
   }
   else
   {
       if(_getcwd(szBuffer, 512) == NULL)
           memset(szBuffer, 0, sizeof(szBuffer));

       if(strlen(szBuffer) > 2)
           if (szBuffer[strlen(szBuffer) - 1] != '\\')
               strcat(szBuffer, "\\");

       strcpy(WapiInfo.szWAPI_INI, szBuffer2);
	    strcpy(WapiInfo.szPhoneBookDir, szBuffer);
	    strcpy(WapiInfo.szSpoolDir, szBuffer);
	    strcpy(WapiInfo.szPredefinedMsgs, szBuffer);
	    strcpy(WapiInfo.szLogDir, szBuffer);
	    strcpy(WapiInfo.szDataDir, szBuffer);

       WriteProfileString("Options", "DefModem", "", szBuffer2);
       WriteProfileString("Directories", "DataFiles", szBuffer, szBuffer2);
       WriteProfileString("Directories", "Phonebook", szBuffer, szBuffer2);
       WriteProfileString("Directories", "Spool", szBuffer, szBuffer2);
       WriteProfileString("Directories", "Predefined", szBuffer, szBuffer2);
       WriteProfileString("Directories", "LogDirectory", szBuffer, szBuffer2);
       WriteProfileString("Directories", "BinFiles", szBuffer, szBuffer2);

       WriteProfileString("DosBEEP_Setup", "PrinterDev", "0", szBuffer2);

       WapiInfo.dwOptions = 0;
       WapiInfo.dwFlags = 0;

       ltoa(WapiInfo.dwOptions, szBuffer, 10);
       WriteProfileString("Options", "Options", szBuffer, szBuffer2);
       ltoa(WapiInfo.dwFlags, szBuffer, 10);
       WriteProfileString("Options", "Flags", szBuffer, szBuffer2);
       WriteProfileString("Options", "PurgeOldAge", "7", szBuffer2);
       WriteProfileString("Signature", "DefaultSignature", "", szBuffer2);

       BeepInfo.nPrinterPort = PRINTER_PORT_NONE;
   }
}

//--------------------------------------------------------------------
//
// GetProfileString
//
//--------------------------------------------------------------------
BOOL GetProfileString(LPSTR lpszSection, LPSTR lpszEntry, LPSTR lpszDefault, 
                 LPSTR lpszBuffer, int nBufferLen, LPSTR lpszFileName)
{
   int  wFHandle;
   LPSTR spPath;
   LPSTR ppath;
   char  szFile[128];
   char  szBuffer[256];
   char  szEnvPath[512];

   if((wFHandle = _sopen(lpszFileName, _O_RDONLY, _SH_DENYNO)) != -1)
   {
                
       if(FindString(wFHandle, lpszSection, lpszEntry, 
                  lpszDefault, lpszBuffer, nBufferLen) == TRUE)
       {
           close(wFHandle);
           return TRUE;
       }
       else
       {
           close(wFHandle);
           strncpy(lpszBuffer, lpszDefault, nBufferLen);
           return FALSE;
       }
   }
   else
       if((ppath = getenv("PATH")) != NULL)
       {
           
           strcpy(szEnvPath, ppath);
           spPath = strtok(szEnvPath, ";");

           do
           {
               memset(szBuffer, 0, sizeof(szBuffer));     
               strcpy(szBuffer, spPath);
               spPath = NULL;

               if (szBuffer[strlen(szBuffer) - 1] != '\\')
      	            strcat(szBuffer, "\\");

               sprintf(szFile, "%s%s", szBuffer, lpszFileName);
               if((wFHandle = _sopen(szFile, _O_RDONLY, _SH_DENYNO)) != -1)
               {

                   if(FindString(wFHandle, lpszSection, lpszEntry, 
                       lpszDefault, lpszBuffer, nBufferLen) == TRUE)
                   {
                       close(wFHandle);
                       return TRUE;
                   }
                   else
                   {
                       close(wFHandle);
                       strncpy(lpszBuffer, lpszDefault, nBufferLen);
                       return FALSE;
                   }
               }
           }
           while((spPath = strtok(NULL, ";")) != NULL);
       }

   return FALSE;
}

//--------------------------------------------------------------------
//
// WriteProfileString
//
//--------------------------------------------------------------------
BOOL WriteProfileString(LPSTR szSection, LPSTR szEntry, LPSTR szString,
                        LPSTR szFileName)
{
   int  wFHandle;
   LPSTR spPath, ppath;
   char  szFile[128];
   char  szBuffer[256];
   char  szEnvPath[512];

   if((wFHandle = _sopen(szFileName, _O_CREAT|_O_RDWR, _SH_DENYNO, _S_IREAD|_S_IWRITE)) != -1)
   {
       if(WriteString(szFileName, &wFHandle, szSection, szEntry, szString) == TRUE)
       {
           close(wFHandle);
           return TRUE;
       }
       else
       {
           close(wFHandle);
           return FALSE;
       }
   }
   else
       if((ppath = getenv("PATH")) != NULL)
       {
           
           strcpy(szEnvPath, ppath);
           spPath = strtok(szEnvPath, ";");

           do
           {
               memset(szBuffer, 0, sizeof(szBuffer));     
               strcpy(szBuffer, spPath);
               spPath = NULL;

               if (szBuffer[strlen(szBuffer) - 1] != '\\')
      	            strcat(szBuffer, "\\");

               sprintf(szFile, "%s%s", szBuffer, szFileName);
               if((wFHandle = _sopen(szFile, _O_CREAT|_O_RDWR, _SH_DENYNO, _S_IREAD|_S_IWRITE)) != -1)
               {

                   if(WriteString(szFileName, &wFHandle, szSection, szEntry, szString) == TRUE)
                   {
                       close(wFHandle);
                       return TRUE;
                   }
                   else
                   {
                       close(wFHandle);
                       return FALSE;
                   }
               }

           }
           while((spPath = strtok(NULL, ";")) != NULL);
       }

   return FALSE;
}                        

//--------------------------------------------------------------------
//
// FindString
//
//--------------------------------------------------------------------
BOOL FindString(int wFHandle, LPSTR lpszSection, LPSTR lpszEntry, 
                LPSTR lpszDefault, LPSTR lpszBuffer, int nBufferLen)
{
   FILE    *fp;
   char    szBuffer[512];
   char    *temp;
   char    *filebuff;
   long    filelength;
   BOOL    DoLine = FALSE;
   char    *curroffset;
   BOOL    ret = FALSE;

   memset(lpszBuffer, 0, nBufferLen);
   strncpy(lpszBuffer, lpszDefault, nBufferLen);

   if((fp = _fdopen(wFHandle, "r")) != NULL)
   {
       filelength = _filelength(wFHandle);

       if(filelength < 10000)
       {
           if((filebuff = (char *) malloc( (int) ((filelength + 50) * sizeof(char)))) != NULL)
           {
               // preset to default
               strncpy(lpszBuffer, lpszDefault, nBufferLen - 1);

               fread(filebuff, sizeof(char), (int) filelength, fp);
               sprintf(szBuffer, "[%s]", lpszSection);
               if((curroffset = strtok(filebuff, "\n")) != NULL)
               {
                   do
                   {
                       if(strstr(curroffset, szBuffer) != NULL)
                       {
                           if((curroffset = strtok(NULL, "\n")) != NULL)
                           {
                               do
                               {
                                   if(strstr(curroffset, lpszEntry) != NULL)
                                   {
                                       if((curroffset = strstr(curroffset, "=")) != NULL)
                                       {
                                           strncpy(lpszBuffer, curroffset + 1, nBufferLen - 1);
                                           ret = TRUE;
                                           break;
                                       }
                                   }
                               }
                               while((curroffset = strtok(NULL, "\n")) != NULL);

                               if(ret == TRUE)
                                   break;
                           }
                       }
                   }
                   while((curroffset = strtok(NULL, "\n")) != NULL);
               }

               free(filebuff);
               fclose(fp);
               return ret;
           }
           else
               DoLine = TRUE;
       }
       else
           DoLine = TRUE;

       if(DoLine == TRUE)
       {
           while(fgets(szBuffer, sizeof(szBuffer), fp) != NULL)
           {
               if(strstr(szBuffer, lpszSection) != NULL)
               {
                   memset(szBuffer, 0, sizeof(szBuffer));
               
                   while(fgets(szBuffer, sizeof(szBuffer), fp) != NULL)
                   {
                       if(strstr(szBuffer, lpszEntry) != NULL)
                       {
                           temp = strtok(szBuffer, "=");
                           temp = strtok(NULL, "=\n");
                           strncpy(lpszBuffer, temp, nBufferLen - 1);
                           fclose(fp);
                           return TRUE;
                       }
                   }
               }

           }
       }
   }
                       
   fclose(fp);
   return FALSE;
}


//--------------------------------------------------------------------
//
// WriteString
//
//--------------------------------------------------------------------
BOOL WriteString(LPSTR szFileName, int *wFHandle, LPSTR lpszSection, LPSTR lpszEntry, LPSTR szString)
{
   char *pszBuffer;
   char *pStrOff;
   char *pStrEnd;
   char szNewEntry[512];
   int  size;
   char temp[255];

   size = (int) (_filelength(*wFHandle) + 10);

   if((pszBuffer = (char *) malloc(size * (sizeof(char)) ) ) != NULL)
   {
       memset(pszBuffer, 0, size);

       size = _read(*wFHandle, (void *) pszBuffer, size );
       close(*wFHandle);
   
       if(((*wFHandle) = _sopen(szFileName, _O_TRUNC|_O_WRONLY, _SH_DENYNO)) != -1)
       {
           sprintf(temp, "[%s]", lpszSection);
           if((pStrOff = strstr(pszBuffer, temp)) != NULL)
           {
               pStrOff = strstr(pStrOff, "]");
               if((pStrOff = strstr(pStrOff, lpszEntry)) != NULL)
               {
                   pStrEnd = strchr(pStrOff, '\n') + 1;    

                   _write(*wFHandle, (void *) pszBuffer, (pStrOff - pszBuffer));
                   sprintf(szNewEntry, "%s=%s\n", lpszEntry, szString);
                   _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
                   _write(*wFHandle, (void *) (pszBuffer + (pStrEnd - pszBuffer)), (size - (pStrEnd - pszBuffer)));

                   return TRUE;
               }
               else
               {
                   sprintf(temp, "[%s]", lpszSection);
                   if((pStrOff = strstr(pszBuffer, temp)) != NULL)
                   {
                       pStrEnd = strchr(pStrOff, '\n') + 1;    

                       _write(*wFHandle, (void *) pszBuffer, (pStrEnd - pszBuffer));
                       sprintf(szNewEntry, "%s=%s\n", lpszEntry, szString);
                       _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
                       _write(*wFHandle, (void *) (pszBuffer + (pStrEnd - pszBuffer)), (size - (pStrEnd - pszBuffer)));
               
                       return TRUE;
                   }
               }
           }
           else
           {
               _write(*wFHandle, (void *) pszBuffer, size);
               sprintf(szNewEntry, "\n[%s]\n", lpszSection);
               _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
               sprintf(szNewEntry, "%s=%s\n", lpszEntry, szString);
               _write(*wFHandle, (void *) szNewEntry, strlen(szNewEntry));
       
               return TRUE;
           }
       }
   }
   
   return FALSE;
}


//------------------------------------------------------------------------
//
//     DosBEEP function:   void PurgeLogs(int nDaysOld)
//
//     decsription:
//
//------------------------------------------------------------------------
BOOL PurgeLogs(int nDaysOld)
{
	struct _find_t  c_file;
	char            szPath[256];
	char            szLogText[256 + 40];
	BYTE            szDate[20];
	unsigned long   lToday;
	BYTE            szMonth[3];
	BYTE            szDay[3];
	BYTE            szYear[3];

	_strdate((LPSTR) szDate);

	strncpy((LPSTR) szMonth, (LPCSTR) szDate, 3);
	strncpy((LPSTR) szDay, (LPCSTR) & szDate[3], 3);
	strncpy((LPSTR) szYear, (LPCSTR) & szDate[6], 3);

   szMonth[2] = 0;
   szDay[2] = 0;
   szYear[2] = 0;

	lToday = (atoi((LPSTR) szMonth) * 30) + atoi((LPSTR) szDay) + (atoi((LPSTR) szYear) * 365);

	sprintf((LPSTR) szPath, "%s*.log", (LPSTR) WapiInfo.szLogDir);

	if (!_dos_findfirst((LPSTR) szPath, _A_NORMAL, &c_file))
		do
		{
			if ((c_file.name[2] == '-') && (c_file.name[5] == '-'))
			{
				strncpy((LPSTR) szMonth, (LPCSTR) c_file.name, 3);
				strncpy((LPSTR) szDay, (LPCSTR) & c_file.name[3], 3);
				strncpy((LPSTR) szYear, (LPCSTR) & c_file.name[6], 3);
  
               szMonth[2] = 0;
               szDay[2] = 0;
               szYear[2] = 0;

				if ( (int) (((lToday - ((atoi((LPSTR) szMonth) * 30) + atoi((LPSTR) szDay) + (atoi((LPSTR) szYear) * 365))))) > nDaysOld)
				{
					sprintf((LPSTR) szPath, "%s%s", (LPSTR) WapiInfo.szLogDir, (LPSTR) c_file.name);

					remove(szPath);

					sprintf((LPSTR) szLogText, "Purged Old Log File: %s", (LPSTR) szPath);
                   if((WapiInfo.dwOptions & OPT_LOG_TRANSACTIONS) || (WapiInfo.dwOptions & OPT_LOG_ERRORS))
                       WriteToLogFile(szLogText);
				}
			}
		} while (!_dos_findnext(&c_file));

	return TRUE;
}

//------------------------------------------------------------------------
//
//     DosBEEP function:   void WriteToLogFile(szLogText)
//
//     decsription:
//
//------------------------------------------------------------------------
BOOL WriteToLogFile(LPSTR szText)
{
	char            szPath[256];
	char            szLogText[256 + 40] = "";
	BYTE            szDate[20];
	BYTE            szMonth[3];
	BYTE            szDay[3];
	BYTE            szYear[3];
   FILE            *fhLogFile;
   char            szTime[15];
   char            szBuffer[15];
   int             hours = 0;

   RemoveCtlChars(szText);

   //memset(szLogText, 0, sizeof(szLogText));
	_strdate((LPSTR) szDate);
   _strtime(szTime);

   memset(szBuffer, 0, sizeof(szBuffer));
   strncpy(szBuffer, szTime, 2);
   if((hours = atoi(szBuffer)) > 12)
   {   
       hours -=12;
       sprintf(szBuffer, "%02d%-6s PM", hours, &(szTime[2]));
   }
   else 
       sprintf(szBuffer, "%-8s AM", szTime);

	strncpy((LPSTR) szMonth, (LPCSTR) szDate, 2);
   szMonth[2] = 0;
	strncpy((LPSTR) szDay, (LPCSTR) & szDate[3], 2);
   szDay[2] = 0;
	strncpy((LPSTR) szYear, (LPCSTR) & szDate[6], 2);
   szYear[2] = 0;

	sprintf((LPSTR) szPath, "%s%s-%s-%s.log", WapiInfo.szLogDir, szMonth, szDay, szYear);

   if((fhLogFile = fopen(szPath, "a+")) != NULL)
   {
       sprintf(szLogText, "%-8s : %-130s", szBuffer, szText);
       if(strlen(szLogText) > 70)
       {
           fwrite(szLogText, sizeof(char), 70, fhLogFile);
           fwrite("\n", sizeof(char), strlen("\n"), fhLogFile);
           fwrite(&(szLogText[70]), sizeof(char), strlen(szLogText) - 70, fhLogFile);
           fwrite("\n", sizeof(char), strlen("\n"), fhLogFile);
       }
       else
       {
           if(szLogText[strlen(szLogText) - 1] != '\n')
      	        strcat(szLogText, "\n");

           fwrite(szLogText, sizeof(char), strlen(szLogText), fhLogFile);
       }

       fclose(fhLogFile);
   }

	return TRUE;
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawControls(PCONTROL_PARAM pCntlInfo, int index)
{
   int counter;

   for(counter = 0; counter < index; counter++)
   {
       //sprintf(message, "%d - %d", counter, AltKey);
       //StatusBar(message, LEFT, 5, counter + 10, _TBLACK, _TWHITE);
       PrintText((pCntlInfo + counter)->pCntlTitle, (pCntlInfo + counter)->nAlignment, (pCntlInfo + counter)->nAlignmentOffset, (pCntlInfo + counter)->nCntlRow, _TWHITE, _TBLUE);
   }
}






