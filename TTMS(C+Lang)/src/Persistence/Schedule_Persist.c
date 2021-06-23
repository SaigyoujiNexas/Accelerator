#include "Schedule_Persist.h"
#include "../Service/schedule.h"
#include "../Common/list.h"
#include "../Service/ticket.h"
#include "../View/Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <assert.h>


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";//保存演出计划信息
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";////删除或更新时，暂时保存演出计划信息

int Schedule_Perst_Insert(const schedule_t *data)
{

	assert(NULL!=data);
	FILE *fp=fopen(SCHEDULE_DATA_FILE,"ab");
	int rtn=0;
	if (NULL==fp)
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	rtn=fwrite(data,sizeof(schedule_t),1,fp);
	fclose(fp);
	return rtn;
}

int Schedule_Perst_Update(const schedule_t *data)
{
	assert(NULL!=data);
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	schedule_t buf;
	int found = 0;

	while (!feof(fp)) 
	{
		if (fread(&buf, sizeof(schedule_t), 1, fp))
		{
			if (buf.id == data->id)
			{

				fseek(fp, -((long)sizeof(schedule_t)), SEEK_CUR);
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}
int Schedule_Perst_DeleteByID(int ID) 
{
	if (rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE) < 0)
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	FILE* fpSour, * fpTarg;

	fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour)
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}
	fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");

	if (NULL == fpTarg)
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_TEMP_FILE);
		return 0;
	}

	schedule_t buf;
	int found = 0;
	while (!feof(fpSour))
	{
		if (fread(&buf, sizeof(schedule_t), 1, fpSour))
		{
			if (ID == buf.id)
			{
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(schedule_t), 1, fpTarg);
		}
	}
	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SCHEDULE_DATA_TEMP_FILE);

	return found;
}


int Schedule_Perst_SelectByID(int ID, schedule_t *buf){
	assert(NULL!=buf);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp))
		{
			if (ID == data.id)
			{
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

int Schedule_Perst_SelectAll(schedule_list_t list){
	schedule_node_t *newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (_access(SCHEDULE_DATA_FILE, _A_NORMAL))
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		while (getchar() != '\n')
			continue;
		return 0;
	}

	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(schedule_t), 1, fp))
		{
			newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
			if (!newNode)
			{
				printf("文件打开失败\n");
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

int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id)
{
	schedule_node_t* newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL != list);

	if (_access(SCHEDULE_DATA_FILE, _A_NORMAL))
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		while (getchar() != '\n')
			continue;
		return 0;
	}

	List_Free(list, schedule_node_t);

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(schedule_t), 1, fp) && data.play_id == play_id)
		{
			newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
			if (!newNode)
			{
				printf("文件打开失败\n");
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


int Schedule_Perst_SelectByStudio(schedule_list_t list, int studio_id)
{
	schedule_t data;
	int recCount = 0;

	assert(NULL != list);

	if (_access(SCHEDULE_DATA_FILE, 0))
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		while (getchar() != '\n')
			continue;
		return 0;
	}

	List_Free(list, schedule_node_t);

	FILE* fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SCHEDULE_DATA_FILE);
		while (getchar() != '\n')
			continue;
		return 0;
	}
	
	while (!feof(fp))
	{
		fread(&data, sizeof(schedule_t), 1, fp);
		if (data.studio_id == studio_id)
		{
			schedule_node_t* newNode = (schedule_node_t*)malloc(sizeof(schedule_node_t));
			if (!newNode)
			{
				printf("文件打开失败\n");
				while (getchar() != '\n')
					continue;
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