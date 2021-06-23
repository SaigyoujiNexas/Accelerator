#include"Sale.h"
#include"../Persistence/Sale_Persist.h"
//添加新订单信息 （需调用持久化层函数Sale_Perst_Insert）
int Sale_Srv_Add(const sale_t *data) 
{
	return Sale_Perst_Insert(data);
}

int Sale_Srv_FetchByTicketID(int ticket_id, sale_t* sale)
{
	return Sale_Perst_SelectByTicketID(ticket_id, sale);
}