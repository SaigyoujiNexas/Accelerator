
#include "Play_Persist.h"
#include "../Service/play.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <assert.h>
#include <string.h>

static const char PLAY_DATA_FILE[] = "Play.dat";//�����Ŀ��Ϣ
static const char PLAY_DATA_TEMP_FILE[] = "PlayTmp.dat";//ɾ�������ʱ����ʱ�����Ŀ��Ϣ

int Play_Perst_Insert(const play_t *data) 	
{
	assert(NULL!=data);
	FILE *fp=fopen(PLAY_DATA_FILE,"ab");
	int rtn=0;
	if (NULL==fp) 
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_FILE);
		return 0;
	}
	rtn=fwrite(data,sizeof(play_t),1,fp);
	fclose(fp);
	return rtn;
}

int Play_Perst_Update(const play_t *data) 
{
	assert(NULL!=data);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	play_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(play_t), 1, fp))
		{
			if (buf.id == data->id)
			{
				fseek(fp, -((long)sizeof(play_t)), SEEK_CUR);
				fwrite(data, sizeof(play_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

int Play_Perst_DeleteByID(int ID) 
{
	if(rename(PLAY_DATA_FILE, PLAY_DATA_TEMP_FILE)<0)
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_FILE);
		return 0;
	}
	FILE *fpSour, *fpTarg;
	fpSour = fopen(PLAY_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour )
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_FILE);
		return 0;
	}
	fpTarg = fopen(PLAY_DATA_FILE, "wb");
	if ( NULL == fpTarg )
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_TEMP_FILE);
		return 0;
	}

	play_t buf;
	int found = 0;
	while (!feof(fpSour)) 
	{
		if (fread(&buf,sizeof(play_t),1,fpSour)) 
		{
			if (ID==buf.id) 
			{
				found=1;
				continue;
			}
			fwrite(&buf,sizeof(play_t),1,fpTarg);
		}
	}
	fclose(fpTarg);
	fclose(fpSour);
	
	//ɾ����ʱ�ļ�
	remove(PLAY_DATA_TEMP_FILE);

	return found;
}

int Play_Perst_SelectByID(int ID, play_t *buf) 
{
	assert(NULL!=buf);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	play_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(play_t), 1, fp))
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

int Play_Perst_SelectAll(play_list_t list) 
{
	play_node_t *newNode;
	play_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (_access(PLAY_DATA_FILE, 0))
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	List_Free(list, play_node_t);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(play_t), 1, fp))
		{
			newNode = (play_node_t*) malloc(sizeof(play_node_t));
			if (!newNode)
			{
				printf("�ļ���ʧ��\n");
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

//���ݹؼ��ʻ�ȡ
int Play_Perst_SelectByName(play_list_t list, char condt[]) 
{
	play_node_t *newNode;
	play_t data;
	int recCount = 0;

	assert(NULL!=list);


	if (_access(PLAY_DATA_FILE, 0))
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	List_Free(list, play_node_t);

	FILE *fp = fopen(PLAY_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", PLAY_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(play_t), 1, fp))
			if (strstr(data.name, condt))  //����λ�Ǳ���ӳ������λ�������
			{
				newNode = (play_node_t*) malloc(sizeof(play_node_t));
				if (!newNode)
				{
					printf("��ջ������\n");
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

