/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: common.h			 
* Author:   XUPT  		 
* Version:  v.1 	 
* Date: 	2015年4月22日		 
*/

#pragma once

//日期结构体 
typedef struct {
	int year;
	int month;
	int day;
}user_date_t;

//时间结构体 
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



//将字符串str就地转换为大写字符串，并返回字符串头指针
char *Str2Upper(char *str);


//将字符串str就地转换为小写字符串，并返回字符串头指针
char *Str2Lower(char *str);

//比较日期dt1, dt2的大小。相等返回0，dt1<dt2返回-1，否则1
int DateCmp(user_date_t dt1, user_date_t dt2);

//获取系统当前日期
user_date_t DateNow();

//获取系统当前时间
user_time_t TimeNow();
char* s_gets(char* str, int n);
char* strtoUpper(char* str);

int time_Check(user_time_t time);

int date_Check(user_date_t date);
