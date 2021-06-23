/*
* Copyright(C), 2007-2008, XUPT Univ.
* ������ţ�TTMS_UC_01		 
* File name: Studio_UI.h			  
* Description : �ݳ������������ͷ�ļ�	
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��	
*/
#pragma once

#include "../Service/Schedule.h"
#include "../Service/Studio.h"
#include "../Common/List.h"
#include "../View/Schedule_UI.h"
//��ʶ����TTMS_SCU_Studio_UI_MgtEnt  
//���ܣ�����IDɾ���ݳ������� 
void Studio_UI_MgtEntry(void);

//��ʶ����TTMS_SCU_Studio_UI_Add 
//���ܣ�������ݳ�������
int Studio_UI_Add(void);

//��ʶ����TTMS_SCU_Studio_UI_Mod
//���ܣ�����ID�޸��ݳ�������
int Studio_UI_Modify(int id);


//��ʶ����TTMS_SCU_Studio_UI_Del
//���ܣ�����IDɾ���ݳ������� 
int Studio_UI_Delete(int id);

void Studio_UI_Show_Studio(studio_list_t head, Pagination_t paging);


