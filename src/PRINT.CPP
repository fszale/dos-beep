
#include <dos.h>
#include <errno.h>
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

extern struct videoconfig vc;
extern WAPIINFO        WapiInfo;
extern DosBEEPINFO     BeepInfo;
extern char            message[80];
extern BOOL            AltKey;

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void PrintDatabase(int num)
{
   long    offset = NOT_SELECTED;
   char                line[256];
   int     Overflow = 1;
   int     Stop = FALSE;
   short   loop = TRUE;

   switch(BeepInfo.nPrinterPort)
   {
       case PRINTER_PORT_1:
           strcpy(line, "LPT1");
       break;
       case PRINTER_PORT_2:
           strcpy(line, "LPT2");
       break;
       case PRINTER_PORT_3:
           strcpy(line, "LPT3");
       break;
       case PRINTER_PORT_NONE:
       default:
           strcpy(message, "No default Printer available.");
           return;
       break;
   }

   switch(num)
   {
       case 1: //subscriber
       {
           WAPISUBSCRIBERINFO  SubInfo;
           //WapiInfo.dwOptions
           FILE    *ph;
           int     index = 100;
           int     PageCounter = 0;
           int     counter;

           if((ph = fopen(line, "w")) != NULL)
           {
               if( Open_DBFile(&WapiInfo, SUBSCRIBER_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
               {
                   StatusBar("Printing...", CENTER, 0, 25, _TBLACK, _TWHITE);

                   if( Subscriber_GetfirstRec(&WapiInfo, SUBSCRIBER_DB, 0, (PVOID) &SubInfo) == CALL_SUCCESS )
                   {
                       do
                       {
                           if ( index > 60 )
                           {
                               fprintf(ph,"\r\n");    

                               sprintf(line, "%25cUser Database Listing%25c", ' ',' ');
                               fprintf(ph,"%s\r\n", line);    

                               memset(line, 0, sizeof(line));
                               for(counter = 0; counter < 75; counter++)
                                   line[counter] = '-';
                               fprintf(ph,"%s\r\n", line);    

                               sprintf(line, "%-30s%-30s%-15s", "User Name", "Service Name", "PIN");
                               fprintf(ph,"%s\r\n", line);    

                               memset(line, 0, sizeof(line));
                               for(counter = 0; counter < 75; counter++)
                                   line[counter] = '-';
                               fprintf(ph,"%s\r\n", line);    

                               PageCounter++;
                               index = 13;
                           }

                           sprintf(line, "%-30s%-30s%-15s", SubInfo.szSubscriberName, 
                                                           SubInfo.szServiceName,
                                                           SubInfo.szPIN);
                           fprintf(ph,"%s\r\n", line);    

                           index++;

                           if ( index > 60 )
                           {
                               memset(line, 0, sizeof(line));
                               for(counter = 0; counter < 75; counter++)
                                   line[counter] = '-';
                               fprintf(ph,"\r\n%s\r\n", line);    

                               sprintf(line, "%35cPage %d%35c", ' ', PageCounter, ' '); 
                               fprintf(ph,"\r\n%s\r\n\f", line);    
                           }
                       }
                       while ( (Subscriber_GetnextRec(&WapiInfo, SUBSCRIBER_DB, 0, (PVOID) &SubInfo) == CALL_SUCCESS) && (Overflow < MAX_SUBSCRIBERS ));

                       if ( index < 60 )
                       {
                           for(counter = 0; counter < (61 - index); counter++)
                               fprintf(ph,"\r\n");    

                           memset(line, 0, sizeof(line));
                           for(counter = 0; counter < 75; counter++)
                               line[counter] = '-';
                           fprintf(ph,"\r\n%s\r\n", line);    

                           sprintf(line, "%35cPage %d%35c", ' ', PageCounter, ' '); 
                           fprintf(ph,"\r\n%s\r\n\f", line);    
                       }

                       strcpy(message, "Finished Printing.");
                   }
                   else
                   {
                       strcpy(message, "ERROR : Subscribers were not found.");
                       Beep(698, 700);
                   }

                   Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
                   Stop = TRUE;
               }

               fclose(ph);
           }
       }
       break;

       case 0: //services
       {
           WAPISERVICEINFO ServiceInfo;
           //WapiInfo.dwOptions
           FILE    *ph;
           int     index = 100;
           int     szBuffer[30];
           int     PageCounter = 0;
           int     counter;

           if((ph = fopen(line, "w")) != NULL)
           {
               if( Open_DBFile(&WapiInfo, SERVICE_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
               {
                   StatusBar("Printing...", CENTER, 0, 25, _TBLACK, _TWHITE);

                   if( Service_GetfirstRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) &ServiceInfo) == CALL_SUCCESS )
                   {
                       do
                       {
                           if ( index > 60 )
                           {
                               sprintf(line, "%25cService Database Listing%25c\n", ' ',' ');
                               fprintf(ph,"%s\r\n", line);    
                           
                               PageCounter++;
                               index = 2;
                           }

                           memset(line, 0, sizeof(line));
                           for(counter = 0; counter < 75; counter++)
                               line[counter] = '-';
                           fprintf(ph,"%s\r\n", line);    

                           sprintf(line, "Service Name     :%s", ServiceInfo.szServiceName);
                           fprintf(ph,"%s\r\n", line);    
                       
                           memset(line, 0, sizeof(line));
                           for(counter = 0; counter < 75; counter++)
                               line[counter] = '-';
                           fprintf(ph,"%s\r\n", line);    
   
                           sprintf(line, "Installed Modem  :%-30s", ServiceInfo.szInstalledModem);
                           fprintf(ph,"%s\r\n", line);    

                           ConvertParams(szBuffer, ServiceInfo.byDataBits, DOS_DATABITS);
                           sprintf(line, "Phone Number     :%-30sData Bits    :%-10s", ServiceInfo.szPhoneNumber, szBuffer);
                           fprintf(ph,"%s\r\n", line);    

                           ConvertParams(szBuffer, ServiceInfo.byParity, DOS_PARITY);
                           sprintf(line, "Parity           :%-30s",szBuffer);
                           fprintf(ph,"%s", line);    
                       
                           ConvertParams(szBuffer, ServiceInfo.byStopBits, DOS_STOPBITS);
                           sprintf(line, "Stop Bits    :%-10s", szBuffer);
                           fprintf(ph,"%s\r\n", line);    

                           ConvertParams(szBuffer, ServiceInfo.dwBaud, DOS_BAUD);
                           sprintf(line, "Baud             :%-30sPassword     :%-10s", szBuffer, ServiceInfo.uProtocol.Pet.szPassword);
                           fprintf(ph,"%s\r\n", line);    

                           sprintf(line, "Length           :%-30d", ServiceInfo.uProtocol.Pet.wMaxMsgLength);
                           fprintf(ph,"%s\r\n\r\n", line);    

                           index+=10;

                           if ( index > 60 )
                           {
                               for(counter = 0; counter < (62 - index); counter++)
                                   fprintf(ph,"\r\n");    

                               memset(line, 0, sizeof(line));
                               for(counter = 0; counter < 75; counter++)
                                   line[counter] = '-';
                               fprintf(ph,"%s\r\n", line);    

                               sprintf(line, "%35cPage %d%35c", ' ', PageCounter, ' '); 
                               fprintf(ph,"\r\n%s\r\n\f", line);    
                           }

                       }
                       while ( (Service_GetnextRec(&WapiInfo, SERVICE_TYPE1, 0, (PVOID) &ServiceInfo) == CALL_SUCCESS) && (Overflow < MAX_SERVICES));
                           
                       if ( index < 60 )
                       {
                           for(counter = 0; counter < (60 - index); counter++)
                               fprintf(ph,"\r\n");    

                           memset(line, 0, sizeof(line));
                           for(counter = 0; counter < 75; counter++)
                               line[counter] = '-';
                           fprintf(ph,"%s\r\n", line);    

                           sprintf(line, "%35cPage %d%35c", ' ', PageCounter, ' '); 
                           fprintf(ph,"\r\n%s\r\n\f", line);    
                       }

                       strcpy(message, "Finished Printing.");
                   }
                   else
                   {
                       strcpy(message, "ERROR : Services were not found.");
                       Beep(698, 700);
                   }

                   Close_DBFile(&WapiInfo, SERVICE_DB);

                   Stop = TRUE;
               }

               fclose(ph);
           }
       }
       break;

       case 2: //groups
       {
           WAPIGROUPINFO GroupInfo;
           FILE    *ph;
           int     index = 100;
           int     PageCounter = 0;
           int     counter;
           char    szLastGroupName[50] = "";
           char    szCurrentGroupName[50] = " ";
           int     GroupMemberCount = 0;
           int     GroupCounter = 1;
           BOOL    once = FALSE;

           if((ph = fopen(line, "w")) != NULL)
           {
               if( Open_DBFile(&WapiInfo, GROUP_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
               {
                   StatusBar("Printing...", CENTER, 0, 25, _TBLACK, _TWHITE);

                   if( Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS )
                   {
                       char count[10];
                       char entry[50];

                       do
                       {
                           if(strcmp(szCurrentGroupName, GroupInfo.szGroupName) != 0)
                           {
                               if(once == TRUE)
                               {
                                   itoa(index, count, 10);
                                   sprintf(entry, "Members%s", szCurrentGroupName);
                                   WriteProfileString("Count", entry, count, "TEMP.TMP");
                               }
                               once = TRUE;
                               strcpy(szCurrentGroupName, GroupInfo.szGroupName);
                               index = 0;
                           }
                           else
                               index++;
                       }
                       while ( (Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS) && (Overflow < MAX_SUBSCRIBERS ));
                       
                       itoa(index, count, 10);
                       sprintf(entry, "Members%s", szCurrentGroupName);
                       WriteProfileString("Count", entry, count, "TEMP.TMP");
                   }

                   index = 100;

                   if( Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS )
                   {
                       do
                       {
                           if ( index > 53 )
                           {
                               fprintf(ph,"\r\n");    

                               sprintf(line, "%25cGroup Database Listing%25c", ' ',' ');
                               fprintf(ph,"%s\r\n", line);    

                               PageCounter++;
                               index = 2;
                           }

                           if(strcmp(szCurrentGroupName, GroupInfo.szGroupName) != 0)
                           {
                               char szCount[10];

                               strcpy(szCurrentGroupName, GroupInfo.szGroupName);
                           
                               memset(line, 0, sizeof(line));
                               for(counter = 0; counter < 75; counter++)
                                   line[counter] = '-';
                               fprintf(ph,"\r\n%s\r\n", line);  

                               sprintf(line, "Members%s", szCurrentGroupName);
                               GetProfileString("Count", line, NULL, szCount, sizeof(szCount), "TEMP.TMP");

                               GroupMemberCount = atoi(szCount);
                               sprintf(line, "Group Name : %-31sTotal Number of Members : %d", GroupInfo.szGroupName, GroupMemberCount);
                               fprintf(ph,"%s\r\n", line);    

                               memset(line, 0, sizeof(line));
                               for(counter = 0; counter < 75; counter++)
                                   line[counter] = '-';
                               fprintf(ph,"%s\r\n", line);    
                               index += 4;
                           }
                           else
                           {
                               strcpy(line, GroupInfo.szSubscriberName);
                               fprintf(ph,"%s\r\n", line);    
                               index++;
                           }

                           if ( index > 53 )
                           {
                               memset(line, 0, sizeof(line));
                               for(counter = 0; counter < 75; counter++)
                                   line[counter] = '-';
                               fprintf(ph,"\r\n%s\r\n", line);    

                               sprintf(line, "%35cPage %d%35c", ' ', PageCounter, ' '); 
                               fprintf(ph,"\r\n%s\r\n\f", line);    
                           }
                       }
                       while ( (Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS) && (Overflow < MAX_SUBSCRIBERS ));

                       if ( index < 60 )
                       {
                           for(counter = 0; counter < (54 - index); counter++)
                               fprintf(ph,"\r\n");    

                           memset(line, 0, sizeof(line));
                           for(counter = 0; counter < 75; counter++)
                               line[counter] = '-';
                           fprintf(ph,"\r\n%s\r\n", line);    

                           sprintf(line, "%35cPage %d%35c", ' ', PageCounter, ' '); 
                           fprintf(ph,"\r\n%s\f", line);    
                       }

                       strcpy(message, "Finished Printing.");
                   }
                   else
                   {
                       strcpy(message, "ERROR : Groups were not found.");
                       Beep(698, 700);
                   }


                   remove("TEMP.TMP");
                   Close_DBFile(&WapiInfo, GROUP_DB);
               }

               Stop = TRUE;
               fclose(ph);
           }
       }
       break;

       case 3: //logs
       {
           struct _find_t  pc_file;
           ITEM    *pLineList = NULL;
           int     index = 0, entries = 0, retval = 0;
           char    szSourceName[256];

           if((pLineList = (ITEM *) malloc( 61 * sizeof(ITEM) )) != NULL)
           {
               if (WapiInfo.szLogDir[strlen(WapiInfo.szLogDir) - 1] != '\\')
                   strcat(WapiInfo.szLogDir, "\\");

	            sprintf(szSourceName, "%s*.LOG", WapiInfo.szLogDir);
	            if (_dos_findfirst(szSourceName, _A_NORMAL, &pc_file) != 0)
               {
                   strcpy(message, "ERROR: No Log files found.");
                   free(pLineList);
                   return;
               }

	            do
	            {
                   pLineList[index].iHilite = -1;
                   strcpy((LPSTR) pLineList[index].achItem, (LPCSTR) pc_file.name);
                   sprintf(szSourceName, "%s%s", WapiInfo.szLogDir, pc_file.name);
                   index++;
	            }
	            while ((_dos_findnext(&pc_file) == 0) && (index < 31));
 
               if(index > 19)
                   entries = 19;
               else
                   entries = index;
               
               StatusBar("Select log to print.", CENTER, 0, 25, _TBLACK, _TWHITE);

               if((retval = ListBox( 7, 39, 12, entries, pLineList, index, 0, HILIGHT, NULL)) >= 0)
               {
                   char    filename[15];
                   char    path[256];
                   char    szBuffer[256];
                   FILE    *fhLogFile;
                   FILE    *ph;
                   int     counter = 0;
                   int     PageCounter = 0;
                   
                   index = 100;
          
                   strcpy(filename, pLineList[retval].achItem);
                   sprintf(message, "Printing log %s.", filename);
                   StatusBar(message, CENTER, 0, 25, _TBLACK, _TWHITE);
           
                   free(pLineList);
                   pLineList = NULL;

                   sprintf(path, "%s%s", WapiInfo.szLogDir, filename);
                   strupr(path);

                   if((fhLogFile = fopen(path, "r")) != NULL)
                   {
                       if((ph = fopen(line, "w")) != NULL)
                       {
                           fseek(fhLogFile, 0, SEEK_SET);
                           while((fgets( szBuffer, 255, fhLogFile) != NULL))
                           {
                               if ( index > 53 )
                               {
                                   sprintf(line, "%20c%-35.35s%20c\n", ' ', path, ' ');
                                   fprintf(ph,"%s\r\n", line);    

                                   PageCounter++;
                                   index = 2;
                               }
                               
                               fprintf(ph,"%s\r", szBuffer);    
                               index++;

                               if ( index > 53 )
                               {
                                   memset(line, 0, sizeof(line));
                                   for(counter = 0; counter < 75; counter++)
                                       line[counter] = '-';
                                   fprintf(ph,"\r\n%s\r\n", line);    

                                   sprintf(line, "%35cPage %d%35c", ' ', PageCounter, ' '); 
                                   fprintf(ph,"\r\n%s\r\n\f", line);    
                               }
                           }

                           if ( index < 60 )
                           {
                               for(counter = 0; counter < (55 - index); counter++)
                                   fprintf(ph,"\r\n");    

                               memset(line, 0, sizeof(line));
                               for(counter = 0; counter < 75; counter++)
                                   line[counter] = '-';
                               fprintf(ph,"%s\r\n", line);    

                               sprintf(line, "%35cPage %d%35c", ' ', PageCounter, ' '); 
                               fprintf(ph,"%s\r\n\f", line);    
                           }
                           fclose(ph);

                           strcpy(message, "Finished Printing.");
                       }
                       else
                       {
                           strcpy(message, "ERROR : Could not access the printer.");
                           Beep(698, 700);
                       }

                       fclose(fhLogFile);
                   }
                   else
                   {
                       strcpy(message, "ERROR: Could not open Log file.");
                       Beep(698, 700);
                   }
               }
               else    
                   free(pLineList);
           }
           else
               strcpy(message, "ERROR: Out of memory.");

           Stop = TRUE;

       }
       break;

       default:
           break;
   }
}

