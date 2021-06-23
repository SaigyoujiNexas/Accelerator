#pragma once

#include "../Common/list.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include "../Service/Schedule.h"
#include "../View/Play_UI.h"
#include "../View/Schedule_UI.h"

static const int TICKET_PAGE_SIZE=6;

void Ticket_UI_Query(void);

inline char Ticket_UI_Status2Char(ticket_status_t status);
//根据票ID打印票
int Ticket_UI_ShowTicket(int ticket_id);

//根据计划ID显示所有票
void Ticket_UI_ListBySch(const schedule_t* sch, ticket_list_t tickList, seat_list_t seatList);

void Ticket_UI_ShowList(ticket_list_t ticket, Pagination_t paging);

