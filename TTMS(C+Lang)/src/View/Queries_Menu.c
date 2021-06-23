 
#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "../View/Ticket_UI.h"

void Queries_Menu(void){
	char choice;
	do {
		system("cls");
        printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t查询菜单\n");
		printf("\t\t\t\t[P]查询演出\n");
		printf("\t\t\t\t[T]查询演出票\n");
		printf("\t\t\t\t[R]返回\n");
        printf("\n\t\t\t\t==================================================================\n");
		printf("\n\t\t\t\t请选择功能 :");
		choice = toupper(getchar());
		while (getchar() != '\n')
			continue;
		switch (choice) {
		case 'P':
		case 'p':
			Schedule_UI_ListAll();
			break;
		case 'T':
		case 't': 
		    Ticket_UI_Query();
			break;
		}
	} while ('R' != choice && 'r' != choice);
	}
