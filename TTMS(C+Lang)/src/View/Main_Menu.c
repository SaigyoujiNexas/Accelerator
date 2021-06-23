/*
* Copyright(C), 2007-2008, XUPT Univ.	 
* File name: Main_Menu.c			 	 
* Description : TTMS ϵͳ���˵�	
* Author:   XUPT  		 
* Version:  v.1 	 
* Da
*/

#include "../View/Main_Menu.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../View/Studio_UI.h"
#include "../View/Play_UI.h"
#include "Queries_Menu.h"
#include "Sale_UI.h"
#include "Account_UI.h"
#include "../View/StaSales_UI.h"
#include "../Service/Account.h"
#include "../Common/common.h"

const account_t* User;
//ϵͳ���˵� 
void Main_Menu(void)
{
	char choice;
	int out=0;

	while (1)
	{
		system("cls");
		printf("\t\t\t\t�����������������������������������\n");
		printf("\t\t\t\t************************* ��ԺƱ�����ϵͳ *************************\n\n");
		printf("\t\t\t\t[1] ������½\t");
		printf("[2] �˺ŵ�½\t[3]�˳�");
		printf("\n\t\t\t\t====================================================================\n");
		printf("\t\t\t\t��������ĸѡ��ѡ�");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		if (choice == '0')
		{
			Color_Egg();
			return;
		}
		if (choice == '1')
		{
			account_t use;
			use.id = 0;
			use.type = USR_ADMIN;
			use.password[0] = '\0';
			use.username[0] = '\0';
			User = &use;
			Main_Menu_Admin();
		}
		else if (choice == '2')
			Main_Menu_SignIn();
		else
		{
			while (choice != '1' && choice != '2' && choice != '3')
			{
				printf("\t\t\t\t����ָ� ���������룺");
				choice = toupper(getchar());
				while (getchar() != '\n')
					continue;
			}
		}
		if (choice == '0')
		{
			Color_Egg();
			return;
		}
		if (choice == '3')
			break;
	}
	return;
}

void Main_Menu_SignIn(void)
{
	int count = 0;
	account_t usr;
	char user[30];
	char passwd[30];
	char* temp;
	account_t use;
	system("cls");
	printf("\t\t\t\t====================================================================\n");
	printf("\t\t\t\t************************* ��ԺƱ�����ϵͳ *************************\n");
	printf("\t\t\t\t�������½�û���(����Exit������һҳ): ");
	scanf("%s", user);
	while (getchar() != '\n')
		continue;
	if (!strcmp((temp = strtoUpper(user)), "EXIT"))
	{
		free(temp);
		return;
	}
	while (Account_Srv_FetchByName(user, &use) == 0)
	{
		printf("\t\t\t\t�û��������ڣ�\n\t\t\t\t����������(����Exit������һҳ):");
		scanf("%s", user);
		while (getchar() != '\n')
			continue;
		if (!strcmp((temp = strtoUpper(user)), "EXIT"))
		{
			free(temp);
			return;
		}
	}
	printf("\n\t\t\t\t�������½����:");
	InputPasswd(passwd);
	while (strcmp(use.password, passwd))
	{
		count++;
		printf("\t\t\t\t������� ����������:");
		InputPasswd(passwd);
		if (count == 2)
			Rnm();
	}
	Account_Srv_FetchByName(user, &usr);
	User = &usr;
	if (use.type == USR_CLERK)
		Main_Menu_CLERK();
	else if (use.type == USR_MANG)
		Main_Menu_Manager();
	else if (use.type == USR_ADMIN)
		Main_Menu_Admin();
}

void Main_Menu_Admin(void)
{
	char choice;
	do {
		system("cls");
		printf("\t\t\t\t====================================================================\n");
		printf("\t\t\t\t************************* ��ԺƱ�����ϵͳ *************************\n");
		printf("\t\t\t\t[S]��Ժ����    \t\t\t");
		printf("[A]ϵͳ�û�����\t\t\t[E]�˳���¼\n");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t��������ĸѡ������Ҫ�ķ���");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice) {
		case 'S':
		case 's':
			Studio_UI_MgtEntry();
			break;
		case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;
		}
	} while ('E' != choice && 'e' != choice);
}


void Main_Menu_Manager(void)
{
	char choice;
	do {
		system("cls");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t************************* ��ԺƱ�����ϵͳ *************************\n");
		printf("\t\t\t\t[P]��Ŀ����\t\t\t[S]��Ժ����\n");
		printf("\t\t\t\t[Q]��ѯ        \t\t\t[N]ͳ��Ʊ��\n");
		printf("\t\t\t\t[W]ͳ��Ա�����۶�\t\t[E]�˳�\n");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t��������ĸѡ������Ҫ�ķ���");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice) {
		case 'S':
		case 's':
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry();
			break;
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'N':
		case 'n':
			SalesAnalysis_UI_MgtEntry();
			break;
		case 'W':
			StaSales_UI_Clerk();
			break;
		}
	} while ('E' != choice && 'e' != choice);

}

void Main_Menu_CLERK(void)
{
	char choice;
	do {
		system("cls");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t************************* ��ԺƱ�����ϵͳ *************************\n");
		printf("\t\t\t\t[T]��Ʊ        \t\t\t[R]��Ʊ\n");
		printf("\t\t\t\t[Q]��ѯ        \t\t\t[N]ͳ��Ʊ��\n");
		printf("\t\t\t\t[W]ͳ�Ƹ������۶�\t\t[E]�˳�\n");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t��������ĸѡ������Ҫ�ķ���");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice) {
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'T':
		case 't':
			Sale_UI_MgtEntry();
			break;
		case 'R':
		case 'r':
			Sale_UI_ReturnTicket();
			break;
		case 'N':
		case 'n':
			SalesAnalysis_UI_MgtEntry();
			break;
		case 'W':
			StaSales_UI_Self();
			break;
		}
	} while ('E' != choice && 'e' != choice);
}

void Color_Egg(void)
{
	system("cls");
	printf("������\n");
	char str[2001];
	FILE* fp;
	fp = fopen("color_egg.txt", "r");
	while (fgets(str, 2000, fp))
	{
		fputs(str, stdout);
	}
	fclose(fp);
	while (getchar() != '\n')
		continue;
}

void Rnm()
{
	system("cls");
	printf("������\n");
	char str[2001];
	FILE* fp;
	fp = fopen("Rnm.txt", "r");
	while (fgets(str, 2000, fp))
	{
		fputs(str, stdout);
	}
	fclose(fp);
	while (getchar() != '\n')
		continue;
	exit(0);
}