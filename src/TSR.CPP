#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <share.h>
#include <errno.h>
#include <bios.h>
#include <direct.h>
#include <graph.h>
#include <dos.h>
#include <stdio.h>
#include <process.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
extern "C"
{
#include "cbtree.h"
}
#include "convert.h"
#include "wapi.h"
#include "dosbeep.h"

extern "C"
{
#include "ttsr.h"                       /* Include file for TTSR        */
}

#define NOHEAP  1
#define ALLHEAP 2
#define ALLSTACK 3

#define ERANGE      34

unsigned short idnum;
unsigned SmallSize, FullSize;
char far *biosvid;     
struct videoconfig vc;
short rows = 0;

BOOL GetProfileString(LPSTR lpszSection, LPSTR lpszEntry, LPSTR lpszDefault, 
                 LPSTR lpszBuffer, int nBufferLen, LPSTR lpszFileName);
BOOL FindString(int wFHandle, LPSTR lpszSection, LPSTR lpszEntry, 
                LPSTR lpszDefault, LPSTR lpszBuffer, int nBufferLen);

int getvid(int left, int top, int right, int bottom, char *buffer);
int putvid(int left, int top, int right, int bottom, char *buffer);
unsigned short SizeOfCode(void);
void SpawnIt(void);
char far path[_MAX_DIR] = "";
char savescreen[4040];
char *ptr;

void main(int argc, char **argv)
{
    TsSetDefStk(0x2555);

    _getvideoconfig( &vc );

    if(vc.monitor == _MONO)                 /* If monochrome ....              */
    {
        FP_SEG(biosvid) = 0xb000;
        FP_OFF(biosvid) = 0;
    }
    else                                /* That means color ....           */
    {
        FP_SEG(biosvid) = 0xb800;
        FP_OFF(biosvid) = 0;
    }

    if(argc > 1)
    {
       char dir[_MAX_DIR];

       if (argv[1][strlen(argv[1]) - 1] != '\\')
           sprintf(dir, "%s\\dosbeep.exe", argv[1]);
       else
           sprintf(dir, "%sdosbeep.exe", argv[1]);

       if(_access(dir, 0) != 0)
       {
           if(strcmp(strupr(argv[1]), "/R") != 0)
           {
               printf("[DBTSR v. 1.00.00]\n");
               printf("[Copyright (c) 1994 by Fourth Wave Technologies, Inc.]\n");
               printf("Usage:\n");
               printf("    dbtsr /[r,h,?] path\n\n");
               printf("Description:\n");
               printf(" dbtsr - Loads the DosBEEP TSR.\n");
               printf(" /h or /? - Help on DosBEEP TSR.\n");
               printf(" /r - Removes DosBEEP TSR from memory.\n");
               printf(" path - Any valid path name for DOSBEEP.EXE.\n");
               return;
           }
       }
       else
       {
           strcpy(path, argv[1]);
           if (path[strlen(path) - 1] != '\\')
               strcat(path, "\\");
       }
    }

    if(TsCheckResident("DBTSR   ",(unsigned *) &idnum) == 0xffff)
    {
       if(idnum & 0xff00)              /* if released                     */
       {
           TsRestart((unsigned short) (idnum & 0x00ff));
       }

       if(argc > 1)
       {
           if(strcmp(strupr(argv[1]), "/R") == 0)
           {
               puts("Releasing DosBEEP TSR. Please Wait...");
               TsRelease(idnum);
               puts("DosBEEP TSR was successfully released.");
               return;
           }
       }

       puts("DosBEEP TSR is already loaded.");
    }
    else
    {
       if((argc > 1) && (strcmp(strupr(argv[1]), "/R") == 0))
       {
           puts("DosBEEP TSR can not be released before it is loaded.");
           exit(2);
       }

       if(argc == 1)
       {
           char    szBuffer2[256];
           char    szBuffer[256];
           
           if(GetProfileString("WAPI", "INI", NULL, szBuffer, sizeof(szBuffer), "WIN.INI") == FALSE)
               strcpy(szBuffer, "WAPI.INI");

           if(GetProfileString("Directories", "BinFiles", NULL, szBuffer2, sizeof(szBuffer2), szBuffer) == FALSE)
               strcpy(szBuffer, "DOSBEEP.HLP");
           else
               sprintf(szBuffer, "%s%s", szBuffer2, "DOSBEEP.EXE");

           if(_access(szBuffer, 0) != 0)
           {
               if(_getcwd(path, _MAX_DIR) == NULL)
               {
                   if(errno == ERANGE)
                   {
                       puts("Path name is too long.");
                       exit(3);
                   }
                   else
                   {
                       puts("Could not get the current working directory.");
                       exit(4);
                   }
               }
           }
           else
               strcpy(path, szBuffer2);

           if (path[strlen(path) - 1] != '\\')
               strcat(path, "\\");
       }
    }


    if(TsCheckHotkey(TSRHOT_D) == 0xffff) /* is hotkey safe?                 */
    {
        puts("Another TSR is using the same Hot Key.\n");
        exit(5);
    }

    TsSwapType(0);      
    TsSwapPath("C:\\DBTSR.SW1");

    if( TsDoInit(
        TSRHOT_D,
        TSRPOPALT + TSRPOPCTRL,
        TSRUSEPOPUP,        // + TSRUSEBACK + TSRUSETIMER + TSRUSEUSER,
        SizeOfCode()) )
        puts("ERROR : Could not initialize DosBEEP TSR.\n");
}

void far pascal TsrMain(void)
{
   char command[_MAX_DIR];
   union REGS regs;
   struct SREGS sregs;

   _getvideoconfig( &vc );
   rows = vc.numtextrows;
   getvid(1,1,80,rows,savescreen); // save screen
   _displaycursor( _GCURSOROFF );

   sprintf(command, "%sdosbeep.exe", path);

   if(_access(command, 0) != 0)
   {
       //putvid(1,1,80,rows,savescreen); // display screen
       _displaycursor( _GCURSORON );
       printf("Could not find %s\n", command);
       TsStuffKeyboard(idnum, "\r", 1, 1);
       return;
   }

   regs.x.ax = 0x4A00;  // set _psp
   regs.x.bx = SmallSize;
   segread(&sregs);
   sregs.es = _psp;
   intdosx(&regs, &regs, &sregs);
   
   //ret = spawnv(_P_WAIT, command, NULL); // run dosbeep.exe
   //ret = spawnl(_P_WAIT, command, command, NULL); // run dosbeep.exe
   _displaycursor( _GCURSOROFF );
   system(command);

   regs.x.ax = 0x4A00;  // restore _psp
   regs.x.bx = FullSize;
   segread(&sregs);
   sregs.es = _psp;
   intdosx(&regs, &regs, &sregs);

   putvid(1,1,80,rows,savescreen); // display screen
   TsStuffKeyboard(idnum, "\r", 1, 1);
   _displaycursor( _GCURSORON );
}

unsigned short SizeOfCode(void)
{
    unsigned int far *PSP;              /* far pointer to PSP              */
    unsigned used;                      /* variable to save paragraphs     */

    FP_SEG(PSP) = _psp-1;
    FP_OFF(PSP) = 3;

    used = *(PSP);                      /* number of paras used by program */

    SmallSize = used;
    used += 0x8000;                     /* add 128K for spawn memory!      */
    FullSize = used;

    return (unsigned short) (used);                       /* return number of paragraphs     */
}

void far pascal TsrCleanUp(unsigned InitOrShutdown)
{
    if(InitOrShutdown)      /* if we're shutting down          */
    {
        _cexit();
    }
}

int getvid(int left, int top, int right, int bottom, char *buffer)
{
    int i,j;
    char far *video;

    for(i = top - 1; i < bottom; i++)
    {
        video = biosvid + (((i * 80) + (left - 1) ) * 2);

        for(j = left - 1; j < right; j++)
        {
            *buffer++ = *video++;
            *buffer++ = *video++;
        }
    }
    return(0);
}

                                        /* put text from buffer into video */
int putvid(int left, int top, int right, int bottom, char *buffer)
{
    int i,j;
    char far *video;

    for(i = top - 1; i < bottom; i++)
    {
        video = biosvid + (((i * 80) + (left - 1) ) * 2);

        for(j = left - 1; j < right; j++)
        {
            *video++ = *buffer++;
            *video++ = *buffer++;
        }
    }
    return(0);
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
           if((filebuff = (char *) malloc((int) ((filelength + 50) * sizeof(char)))) != NULL)
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
