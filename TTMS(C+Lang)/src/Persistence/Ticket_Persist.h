#pragma once

#include "../Service/ticket.h"



int Ticket_Perst_Insert(ticket_list_t list);//�����ݳ��ƻ�ʱ����������Ʊ

int Ticket_Perst_Delete(int schedule_id);

int Ticket_Perst_Update(const ticket_t * data);

int Ticket_Perst_SelectByID(int ID, ticket_t *buf) ;

int Ticket_Perst_SelectAll(ticket_list_t list) ;

int Ticket_Perst_SelectBySchID(ticket_list_t list, int schedule_id);

