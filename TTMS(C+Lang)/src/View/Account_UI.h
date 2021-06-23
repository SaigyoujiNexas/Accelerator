
#pragma once

#include "../Common/list.h"
#include "../Service/account.h"

static const int ACCOUNT_PAGE_SIZE = 3;

int SysLogin();
void Account_UI_MgtEntry();
int Account_UI_Add(account_list_t list);
int Account_UI_Modify(account_list_t list,char usrName[]);
int Account_UI_Delete(account_list_t list,char usrName[]);
int Account_UI_Query(account_list_t list,char usrName[]);
void Account_UI_ShowList(account_list_t list, Pagination_t paging);



