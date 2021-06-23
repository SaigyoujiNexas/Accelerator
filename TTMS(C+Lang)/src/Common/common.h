/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: common.h			 
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015��4��22��		 
*/

#pragma once

//���ڽṹ�� 
typedef struct {
	int year;
	int month;
	int day;
}user_date_t;

//ʱ��ṹ�� 
typedef struct {
	int hour;
	int minute;
	int second;
}user_time_t;


struct month {
	char name[10];
	char condense[4];
	int days;
	int number;
};

void InputPasswd(char* passwd);



//���ַ���str�͵�ת��Ϊ��д�ַ������������ַ���ͷָ��
char *Str2Upper(char *str);


//���ַ���str�͵�ת��ΪСд�ַ������������ַ���ͷָ��
char *Str2Lower(char *str);

//�Ƚ�����dt1, dt2�Ĵ�С����ȷ���0��dt1<dt2����-1������1
int DateCmp(user_date_t dt1, user_date_t dt2);

//��ȡϵͳ��ǰ����
user_date_t DateNow();

//��ȡϵͳ��ǰʱ��
user_time_t TimeNow();
char* s_gets(char* str, int n);
char* strtoUpper(char* str);

int time_Check(user_time_t time);

int date_Check(user_date_t date);
