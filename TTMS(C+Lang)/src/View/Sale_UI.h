#pragma once


#include "../Service/Ticket.h"
#include "../Service/Seat.h"
void Sale_UI_MgtEntry(void);

//���ݾ�Ŀ��ʾ�ݳ��ƻ�����
void Sale_UI_RetfundTicket();
void Sale_UI_ShowScheduler(int playID);
void Sale_UI_ShowTicket(int scheduleID, int playID);
int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList);
void Sale_UI_ReturnTicket();

