#pragma once
#include "../Service/Ticket.h"
#include "../Service/Sale.h"
#include "../Service/SalesAnalysis.h"
#include "../Service/Play.h"
#include "../Service/Sale.h"

//存储新订单信息
int Ticket_Perst_SelByID(int id,ticket_t *buf);
int Sale_Perst_Insert(const sale_t *data);
int Sale_Perst_SelectByTicketID(int ticket_id, sale_t* sale);


int Salesanalysis_Perst_Insert(const salesanalysis_t* data);
int SalesAnalysis_Perst_SelectAll(salesanalysis_list_t list);
int Sale_Perst_SelectALL(sale_list_t list);


int Sale_Perst_SelByUsrID(sale_list_t list, int usrID);


