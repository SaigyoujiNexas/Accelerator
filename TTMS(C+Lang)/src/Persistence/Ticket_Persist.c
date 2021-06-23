

#include "Ticket_Persist.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>

static const char TICKET_DATA_FILE[]="Ticket.dat";
static const char TICKET_DATA_TEMP_FILE[]="TicketTemp.dat";

int Ticket_Perst_Insert(ticket_list_t list)
{
	ticket_node_t *pos;

	int rtn = 0;
	ticket_t data;
	FILE *fp = fopen(TICKET_DATA_FILE, "ab");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}
	List_ForEach(list,pos)
	{
		data=pos->data;
		fwrite(&data,sizeof(ticket_t),1,fp);
		rtn++;
	}
	fclose(fp);
	return rtn;
}

int Ticket_Perst_Delete(int schedule_id)
{

	FILE *fpSour, *fpTarg;

	if (rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE) < 0)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(TICKET_DATA_FILE, "wb");
	if (NULL == fpTarg || NULL == fpSour)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t buf;

	int found = 0;
	while (!feof(fpSour))
	{
		if (fread(&buf, sizeof(ticket_t), 1, fpSour))
		{
			if (schedule_id == buf.schedule_id)
			{
				found =1;
				continue;
			}
			fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	remove(TICKET_DATA_TEMP_FILE);

	return found;

}

int Ticket_Perst_Update(const ticket_t * data)
{
	assert(NULL!=data);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(ticket_t), 1, fp))
		{
			if (buf.id == data->id)
			{
				fseek(fp, -((long)sizeof(ticket_t)), SEEK_CUR);
				fwrite(data, sizeof(ticket_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}


int Ticket_Perst_SelectByID(int ID, ticket_t *buf)
{
	assert(NULL!=buf);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t data;
	int found = 0;

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp))
		{
			if (ID == data.id)
			{
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}


int Ticket_Perst_SelectAll(ticket_list_t list)
{
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
	if (_access(TICKET_DATA_FILE, 0))
		return 0;

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)){
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf("错误！\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}


int Ticket_Perst_SelectBySchID(ticket_list_t list, int schedule_id)
{
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (_access(TICKET_DATA_FILE, 0))
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		fread(&data, sizeof(ticket_t), 1, fp);
		if (data.schedule_id == schedule_id)
		{
			newNode = (ticket_node_t*)malloc(sizeof(ticket_node_t));
			if (!newNode)
			{
				printf("错误\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}


