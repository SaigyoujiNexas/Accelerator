
#include "Account.h"
#include "EntityKey.h"
#include "../Common/List.h"
#include "../Persistence/Account_Persist.h"
#include "../Service/EntityKey.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


account_t gl_CurUser;
int Account_Srv_Logout(){
	account_t data={0, USR_ADMIN, "Anonymous",""};
	gl_CurUser= data;
	if(gl_CurUser.type== USR_ADMIN){
		return 1;
	}
	else{
		return 0;
	}
}

//创建系统初始化账号admin
void Account_Srv_InitSys()
{
	if(!Account_Perst_CheckAccFile() ){
		account_t data={ 0 , USR_ADMIN , "admin" ,"123456" };	
		data.id=EntKey_Srv_CompNewKey("user");	
		Account_Perst_Insert(&data);
	}
}

int Account_Srv_Verify(char usrName[], char pwd[])
{
	account_t buf;
	if(!Account_Perst_SelByName(usrName, &buf))
	{
		return 0;
	}
	if(strcmp(pwd,buf.password)!=0)
	{
		return 0;
	}
	return 1;
}

account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) 
{
	account_list_t curPos;
	List_ForEach(list,curPos)
	{
		if(strcmp(curPos->data.username,usrName)==0)
		{
			return curPos;
		}
	}
	return NULL ;																								
}


int Account_Srv_Add(const account_t *data)
{
	return Account_Perst_Insert(data);
}


int Account_Srv_Modify(const account_t *data)
{
	return Account_Perst_Update(data);
}


int Account_Srv_DeleteByID(int usrID)
{
	return Account_Perst_DeleteByID(usrID);
}


int Account_Srv_FetchByID(int usrID, account_t *buf)
{
	return Account_Perst_SelectByID(usrID, buf);
}


int Account_Srv_FetchByName(char usrName[], account_t *buf)
{
	return  Account_Perst_SelByName(usrName, buf);
}


int Account_Srv_FetchAll(account_list_t list)
{
	return Account_Perst_SelectAll(list);
}


int Account_Srv_FetchByType(account_list_t list, account_type_t type)
{
	List_Free(list, account_node_t);
	int count = 0;
	account_list_t use;
	account_node_t* pos;
	List_Init(use, account_node_t);
	Account_Perst_SelectAll(use);
	List_ForEach(use, pos)
	{
		if (pos->data.type == type)
		{
			account_node_t* newNode = (account_node_t*)malloc(sizeof(account_node_t));
			if (!newNode)
			{
				printf("堆栈已满！");
				return 0;
			}
			newNode->data = pos->data;
			List_AddTail(list, newNode);
			count++;
		}
	}
	List_Destroy(use, account_node_t);
	return count;
}
