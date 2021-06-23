 
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
		printf("\t\t\t\t��ѯ�˵�\n");
		printf("\t\t\t\t[P]��ѯ�ݳ�\n");
		printf("\t\t\t\t[T]��ѯ�ݳ�Ʊ\n");
		printf("\t\t\t\t[R]����\n");
        printf("\n\t\t\t\t==================================================================\n");
		printf("\n\t\t\t\t��ѡ���� :");
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
