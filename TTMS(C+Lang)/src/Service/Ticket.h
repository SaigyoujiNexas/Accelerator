#pragma once



//Ʊ״̬����
typedef enum{
	TICKET_AVL=0,         //���� 
	TICKET_SOLD=1,        //���� 
	TICKET_RESV=9         //Ԥ�� 
}ticket_status_t;

//Ʊʵ���������� 
typedef struct{
	int id;       
	int schedule_id;
	int seat_id;
	int price;
	ticket_status_t status; 
}ticket_t;

//Ʊ������
typedef struct ticket_node {
	ticket_t data;
	struct ticket_node* next, * prev;
}ticket_node_t;

typedef ticket_node_t* ticket_list_t;


//���ݼƻ�ID��ȡ�����ݳ�Ʊ
int Ticket_Srv_FetchBySchID(ticket_list_t list, int schedule_id);

int Ticket_Srv_FetchAll(ticket_list_t list);

//������λID��list���Ҷ�ӦƱ
ticket_node_t * Ticket_Srv_FetchBySeatID(ticket_list_t list, int seat_id);

void Ticket_Srv_GenBatch(int schedule_id);


int Ticket_Srv_DeleteBatch(int schedule_id);

int Ticket_Srv_Modify(const ticket_t * data);

int Ticket_Srv_FetchByID(int ID, ticket_t *buf) ;

int Ticket_Srv_StatRevBySchID(int schedule_id, int* soldCount);

int Ticket_Srv_FetchSold(ticket_list_t list, ticket_list_t listSold);
