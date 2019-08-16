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

BOOL FindSubscriber(ITEM *Member, int Count, LPSTR retBuffer);

extern struct videoconfig vc;
extern WAPIINFO            WapiInfo;
extern char                message[80];
extern int                 sf;
extern BOOL                KeyDown;
extern BOOL                AltKey;
extern ITEM                mnuHelp[];
extern CONTROL_PARAM       CntlInfo[13];
extern WAPISPOOLERINFO     CurrentPage;

//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
void DrawGroupScreen()
{
   _displaycursor( _GCURSOROFF );
   _setbkcolor( (long)_BLACK);
   Box( 2, 0, 21, vc.numtextcols - 2, _TWHITE, _TBLUE, BORDER);
   StatusBar("", LEFT, 0, 25, _TWHITE, _TWHITE);

   PrintText("  Group Edit  ", CENTER, 0, 2, _TWHITE, _TBLUE);
   PrintText("Non Member", LEFT, 5, 9, _TBRIGHTWHITE, _TBLUE);
   PrintText("Member", LEFT, 42, 9, _TBRIGHTWHITE, _TBLUE);
}
//----------------------------------------------------------------
//
//
//----------------------------------------------------------------
int ListGroups(LPWAPIGROUPINFO pGroupInfo, short row, short col, int maxWidth, int maxEntries)
{
   ITEM *pSubList = NULL;
   int index = 0, offset = NOT_SELECTED;
   int Height;
   int Overflow = 1;
   char szLastGroup[60] = "";
   char retBuffer[31];
   char buffer[50];

   if(!pGroupInfo)
      return NOT_SELECTED;

   if((pSubList = (ITEM *) malloc( MAX_GROUPS * sizeof(ITEM) )) == NULL)
      return NOT_SELECTED;

   StatusBar("Loading Group Database...", CENTER, 0, 25, _TBLACK, _TWHITE);

   if( Open_DBFile(&WapiInfo, GROUP_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
   {
      if( Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) pGroupInfo) == CALL_SUCCESS )
      {
         do
         {
            if(strcmp(szLastGroup, pGroupInfo->szGroupName) != 0)
            {
               pSubList[index].iHilite = -1;
               strcpy((LPSTR) pSubList[index].achItem, (LPCSTR) pGroupInfo->szGroupName);

               strcpy(szLastGroup, pGroupInfo->szGroupName);

               Overflow++;
               index++;
            }
         }
         while ( (Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) pGroupInfo) == CALL_SUCCESS) && (Overflow < MAX_GROUPS ));

         if( index > maxEntries )
            Height = maxEntries;
         else
            Height = index;

         StatusBar("Select a group.", LEFT, 40, 25, _TBLACK, _TWHITE);

         sprintf(buffer, " Total # of groups : %d ", index);
         PrintText(buffer, LEFT, 1, 25, _TBRIGHTWHITE, _TBLACK);

         offset = ListBox( row, col, maxWidth, Height, pSubList, index, 0, HILIGHT|COPY_SELECTION, retBuffer);
         StatusBar("", CENTER, 0, 25, _TBLACK, _TWHITE);

         if ( offset == -1 )
         {
            free(pSubList);
            Close_DBFile(&WapiInfo, GROUP_DB);
            return NOT_SELECTED;
         }

         //strcpy((LPSTR) pGroupInfo->szGroupName, (LPCSTR) pSubList[offset].achItem);
         //strcpy((LPSTR) pGroupInfo->szSubscriberName, (LPCSTR) pSubscribers[offset].achItem);
         strcpy((LPSTR) pGroupInfo->szGroupName, (LPCSTR) retBuffer);
      }

      Close_DBFile(&WapiInfo, GROUP_DB);
   }
   else
      offset = NOT_SELECTED;

   free(pSubList);
   return (WORD) offset;
}
//--------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------
BOOL SetGroupData(ITEM **Member, ITEM **NonMember, int *MemberCount, int *NonMemberCount)
{
   int index = 0;
   int Overflow = 0;

   if((*Member = (ITEM *) malloc( MAX_SUBSCRIBERS * sizeof(ITEM) )) == NULL)
       return FALSE;
   else
   {
       if((*NonMember = (ITEM *) malloc( MAX_SUBSCRIBERS * sizeof(ITEM) )) == NULL)
       {
           free(*Member);
           return FALSE;
       }
       else
       {
           memset(*Member, 0, MAX_SUBSCRIBERS * sizeof(ITEM));
           memset(*NonMember, 0, MAX_SUBSCRIBERS * sizeof(ITEM));

           FillGroupListBox(*Member, MemberCount, "##ALL_NON_MEMBERS_INTERNAL##", *NonMember, NonMemberCount);
       }

      Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
   }
      
   *MemberCount = 0;
   return TRUE;
}


//--------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------
void AddItem(ITEM *list, int *Count, LPSTR retBuffer)
{
   int index;
   int counter;
   char Entry[100];
   char Entry1[100];

   strcpy(Entry1, retBuffer);
   _strlwr(Entry1);

   // sort entry
   if(*Count == 0)
   {
       (list + 0)->iHilite = -1;
       strcpy((LPSTR) ((list) + 0)->achItem, (LPCSTR) retBuffer);
      
       (*Count)++;
   }
   else
   {
       // search
       index = 0;
       while(index < *Count)
       {
           strcpy(Entry, (list + index)->achItem);
           _strlwr(Entry);

           if(strcmp(Entry1, Entry) < 0)
               break;

           index++;
       }
           
       // shuffle
       for(counter = (*Count); counter > (index); counter--)
       {
           (list + counter)->iHilite = (list + (counter - 1))->iHilite;
           strcpy((LPSTR) ((list) + counter)->achItem, (LPCSTR) ((list) + (counter - 1))->achItem);
       }
       (*Count)++;

       // copy
       (list + index)->iHilite = -1;
       strcpy((LPSTR) ((list) + index)->achItem, (LPCSTR) retBuffer);
   }
}

//--------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------
void FillGroupListBox(ITEM *list, int *Count, LPSTR Group, ITEM *NonMember, int *NonMemberCount)
{
   int Overflow = 0;
   WAPIGROUPINFO GroupInfo;

   *Count = 0;
 
   if(strcmp(Group, "##ALL_NON_MEMBERS_INTERNAL##") == 0)
   {
       if( Open_DBFile(&WapiInfo, SUBSCRIBER_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
       {    
           if( Subscriber_GetfirstRec(&WapiInfo, SUBSCRIBER_DB, 0, (PVOID) &CurrentPage.SubscriberInfo) == CALL_SUCCESS )
           {
               *NonMemberCount = 0;
               do
               {
                   strcpy((NonMember + *NonMemberCount)->achItem, CurrentPage.SubscriberInfo.szSubscriberName);
                   (*NonMemberCount)++;
                   Overflow++;
               }
               while ( (Subscriber_GetnextRec(&WapiInfo, SUBSCRIBER_DB, 0, (PVOID) &CurrentPage.SubscriberInfo) == CALL_SUCCESS) && (Overflow < MAX_SUBSCRIBERS ));
           }
            
           Close_DBFile(&WapiInfo, SUBSCRIBER_DB);
       }
   }
   else
   {
       if( Open_DBFile(&WapiInfo, GROUP_DB, WapiInfo.szPhoneBookDir) == CALL_SUCCESS )
       {
           StatusBar("Loading group members...", CENTER, 0, 25, _TBLACK, _TWHITE);
           if( Group_GetfirstRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS )
           {
               do
               {                      
                   if(strcmp(Group, GroupInfo.szGroupName) == 0)
                   {
                       strcpy((list + *Count)->achItem, GroupInfo.szSubscriberName);
                       (*Count)++;
                       DeleteItem(NonMember, NonMemberCount, GroupInfo.szSubscriberName);
                       Overflow++;
                   }
               }
               while ( (Group_GetnextRec(&WapiInfo, GROUP_TYPE1, 0, (PVOID) &GroupInfo) == CALL_SUCCESS) && (Overflow < MAX_SUBSCRIBERS ));
           }
           Close_DBFile(&WapiInfo, GROUP_DB);
           StatusBar("Group members have been loaded.", CENTER, 0, 25, _TBLACK, _TWHITE);
       }
   }
}

//--------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------
void DeleteItem(ITEM *list, int *Count, LPSTR retBuffer)
{
   int index;

   for(index = 0; index < *Count; index++)
   {
       if(strcmp(((list) + index)->achItem, retBuffer) == 0)
       {
           index++;

           if(index < *Count)
           {
               for(;index < *Count; index++)
               {
                   strcpy(((list) + index - 1)->achItem, ((list) + index)->achItem);
                   memset(((list) + index)->achItem, 0, sizeof(((list) + index)->achItem));
               }
           }
           else
               memset(((list) + index - 1)->achItem, 0, sizeof(((list) + index - 1)->achItem));

           *Count -= 1;
           break;
       }
   }
}


//--------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------
BOOL FindSubscriber(ITEM *Member, int Count, LPSTR retBuffer)
{
   int index;

   for(index = 0; index < Count; index++)
       if(strcmp( (Member + index)->achItem, retBuffer) == 0)
           return TRUE;
   
   return FALSE;
}


//--------------------------------------------------------------------------
//
//
//
//--------------------------------------------------------------------------
int  ListEntries(ITEM *list, int Count, LPSTR member, short row, short col, int maxWidth, int Height)
{
   int offset = 0;

   if((offset = ListBox( row, col, maxWidth, Height, list, Count, 0, HILIGHT, NULL)) >= 0)
   {
       strcpy(member, (list + offset)->achItem);
       return (WORD) offset;
   }
   else
       return NOT_SELECTED;
}


