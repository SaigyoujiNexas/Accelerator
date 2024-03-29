#pragma once

#include "../Service/Account.h"

int Account_Perst_CheckAccFile();

int Account_Perst_SelByName(char usrName[], account_t *buf);
int Account_Perst_Insert(const account_t *data);
int Account_Perst_Update(const account_t *data);
int Account_Perst_DeleteByID(int id);
int Account_Perst_SelectByID(int id, account_t *buf);
int Account_Perst_SelectAll(account_list_t list);
