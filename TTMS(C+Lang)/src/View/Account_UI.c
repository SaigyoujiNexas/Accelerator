
#include "Account_UI.h"
#include "../Common/List.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include <stdio.h>
#include<assert.h>
#include<string.h>
#include <io.h>
#include <time.h>
#include <conio.h>


void inputPasswd(char passwd[], int min, int max)
{
    char s[20];
    char temp[2];
    int n;
    strcpy(s," ");
    while(1)
    {
        while(1)
        {
			system("cls");
            n=_getch();
            if(n == 13)
            {
                break;
            }
            printf("*");
            memset(temp,0,sizeof(temp));
            sprintf(temp,"%c",n);
            strcat(s,temp);
        }
        if(strlen(s) > max+1)
        {
            memset(s, 0, sizeof(s));
            printf("\n����̫���ˣ�����������:");
            strcpy(s," ");
            continue;
        }
        if(strlen(s) < min+1)
        {
            memset(s, 0, sizeof(s));
            printf("\n����̫��~~~������~~~:");
            strcpy(s," ");
            continue;
        }
        break;
    }
    printf("\n");
    strcpy(passwd, s+1);
}


int SysLogin() {
	char user[30],pass[30];
	printf("========================\n");
	printf("\n");
	printf("      �������˺ţ�");
	scanf("%s",user);
	printf("\n");
	printf("      ���������룺");
	inputPasswd(pass,0,20);
	printf("\n");
	//printf(" ==\n");
	printf("========================\n");
	while(getchar() != '\n')continue;
	if(Account_Srv_Verify(user,pass))
	{
		return 1;
	}
    return 0;
}

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
void Account_UI_MgtEntry() {
	int i;
	char userName[30];
	char choice;

	account_list_t head;
	account_node_t *pos;
	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;

	//��������
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t%*s",32,"�˺��б�\n");
		printf("\t\t\t\t%-10s  %-18s  %-18s  %-18s\n", "ID", "�û���", "����", "ְλ");
		printf("\t\t\t\t------------------------------------------------------------------\n");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i){
			printf("\t\t\t\t%-10d  %-18s  %-18s", pos->data.id,pos->data.username, pos->data.password);
			if (pos->data.type == USR_CLERK)
				printf("  %-18s\n", "��ƱԱ");
			else if (pos->data.type == USR_MANG)
				printf("  %-18s\n", "����");
			else if (pos->data.type == USR_ADMIN)
				printf("  %-18s\n", "����Ա");
			else
				printf("  %-18s\n", "δ֪�˻��� �����ԱѸ��ɾ��");

		}

		printf("\t\t\t\t------- �� %2d ��----------------------- �� %2d/%2d ҳ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t\t******************************************************************\n");
		printf("\t\t\t\t          [P]��һҳ[N]��һҳ[A]���[D]ɾ��[U]����[R]����            ");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t���빦��ǰ����ĸ��ѡ����:" );
		
		choice = toupper(getchar());
 		//scanf("%*c");
		//fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(NULL)) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t\t����Ҫ�޸ĵ��û���:");
			scanf("%s",userName);
			while (getchar() != '\n')
				continue;
			if (Account_UI_Modify(NULL, userName))
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t\t\t����Ҫɾ�����û���:");
			scanf("%s",userName);
			while(getchar() != '\n')continue;
			if (Account_UI_Delete(NULL, userName))
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 's':
		case 'S':
			/*printf("Input the room ID:");
			scanf("%d", &id);
			while(getchar() != '\n')continue;
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Account_Srv_FetchAll(head);
			List_Paging(head, paging, account_node_t);
			*/break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, account_node_t);}



int Account_UI_Add(account_list_t list ) {
	int type;

	account_t data;

	do {
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t%32s\n","������û�");
		printf("\t\t\t\t------------------------------------------------------------------\n");
		fflush(stdin);
		printf("\t\t\t\t[1]����Ա       [2]����        [9]ϵͳ����Ա\n");
		printf("\t\t\t\t==================================================================\n");
		printf("\t\t\t\t��ѡ���˺����ͣ�");
		scanf("%d",&type);
		while(getchar() != '\n')continue;
	}
	while(type!=0 && type!=1 && type!=2 && type!=9);
	data.type = type;
	data.id = EntKey_Srv_CompNewKey("Account");
	printf("\t\t\t\t�������˺ţ�");
	scanf("%s",data.username);
	while(getchar() != '\n')continue;
	printf("\t\t\t\t���������룺");
	scanf("%s",data.password);
	while(getchar() != '\n')continue;
	if(Account_Srv_Add(&data))
	{
		printf("\t\t\t\t��ӡ����������������ɹ�!\n");
		while(getchar() != '\n')continue;
		return 1;
	}
	else {
		printf("\t\t\t\t��ӡ�������������������ʧ��!\n");
		while(getchar() != '\n')continue;
	}
	return 0;
}


int Account_UI_Modify(account_list_t list ,char usrName[]) {
	account_list_t head;
	account_list_t data = (account_list_t)malloc(sizeof(account_node_t));
	//puts("123");


	List_Init(head, account_node_t);
	Account_Srv_FetchAll(head);
	data=Account_Srv_FindByUsrName(head, usrName);
	//puts("456");
	if(data == NULL)
	{
		printf("\t\t\t\tô�ø��˺�\n");
		return 0;
	}
	printf("\t\t\t\t�����������룺");
	scanf("%s",data->data.password);
	while(getchar() != '\n')continue;
	Account_Srv_Modify(&(data->data));
	printf("\t\t\t\t�޸ĳɹ�\n");

	return 1;
}


int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_t buf;
	if(Account_Srv_FetchByName(usrName, &buf))
	{
		Account_Srv_DeleteByID(buf.id);
		printf("\t\t\t\tɾ���ɹ�\n");
		return 1;
	}
	printf("\t\t\t\tô�ø��˺�\n");
       	return  0;
}


int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_t buf;
	if(Account_Srv_FetchByName(usrName, &buf)){
		return 1;
	}
	printf("\t\t\t\tô�ø��˺�\n");

	return  0;
}


void Account_UI_ShowList(account_list_t list, Pagination_t paging)
{
	account_node_t* pos;
	int i;
	printf("\n\t\t\t\t==================================================================\n");
	printf("\t\t\t\t%s\n", "�˺��б�");
	printf("\t\t\t\t%-10s  %-18s\n", "ID", "�û���");
	printf("------------------------------------------------------------------\n");
	//��ʾ����
	Paging_ViewPage_ForEach(list, paging, account_node_t, pos, i) {
		printf("\t\t\t\t%-10d  %-18s ", pos->data.id, pos->data.username);
	}
	printf("\t\t\t\t------- �� %2d ��----------------------- �� %2d/%2d ҳ----\n",
		paging.totalRecords, Pageing_CurPage(paging),
		Pageing_TotalPages(paging));

}
