
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
            printf("\n密码太长了，请重新输入:");
            strcpy(s," ");
            continue;
        }
        if(strlen(s) < min+1)
        {
            memset(s, 0, sizeof(s));
            printf("\n密码太短~~~，重输~~~:");
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
	printf("      请输入账号：");
	scanf("%s",user);
	printf("\n");
	printf("      请输入密码：");
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

//管理系统用户功能入口函数，显示用户账号管理菜单
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

	//载入数据
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t%*s",32,"账号列表\n");
		printf("\t\t\t\t%-10s  %-18s  %-18s  %-18s\n", "ID", "用户名", "密码", "职位");
		printf("\t\t\t\t------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i){
			printf("\t\t\t\t%-10d  %-18s  %-18s", pos->data.id,pos->data.username, pos->data.password);
			if (pos->data.type == USR_CLERK)
				printf("  %-18s\n", "售票员");
			else if (pos->data.type == USR_MANG)
				printf("  %-18s\n", "经理");
			else if (pos->data.type == USR_ADMIN)
				printf("  %-18s\n", "管理员");
			else
				printf("  %-18s\n", "未知账户， 请管理员迅速删除");

		}

		printf("\t\t\t\t------- 共 %2d 项----------------------- 第 %2d/%2d 页----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t\t******************************************************************\n");
		printf("\t\t\t\t          [P]上一页[N]下一页[A]添加[D]删除[U]更新[R]返回            ");
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t输入功能前的字母，选择功能:" );
		
		choice = toupper(getchar());
 		//scanf("%*c");
		//fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(NULL)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t\t输入要修改的用户名:");
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
			printf("\t\t\t\t输入要删除的用户名:");
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
	//释放链表空间
	List_Destroy(head, account_node_t);}



int Account_UI_Add(account_list_t list ) {
	int type;

	account_t data;

	do {
		printf("\n\t\t\t\t==================================================================\n");
		printf("\t\t\t\t%32s\n","添加新用户");
		printf("\t\t\t\t------------------------------------------------------------------\n");
		fflush(stdin);
		printf("\t\t\t\t[1]销售员       [2]经理        [9]系统管理员\n");
		printf("\t\t\t\t==================================================================\n");
		printf("\t\t\t\t请选择账号类型：");
		scanf("%d",&type);
		while(getchar() != '\n')continue;
	}
	while(type!=0 && type!=1 && type!=2 && type!=9);
	data.type = type;
	data.id = EntKey_Srv_CompNewKey("Account");
	printf("\t\t\t\t请输入账号：");
	scanf("%s",data.username);
	while(getchar() != '\n')continue;
	printf("\t\t\t\t请输入密码：");
	scanf("%s",data.password);
	while(getchar() != '\n')continue;
	if(Account_Srv_Add(&data))
	{
		printf("\t\t\t\t添加。。。。。。。。成功!\n");
		while(getchar() != '\n')continue;
		return 1;
	}
	else {
		printf("\t\t\t\t添加。。。。。。。。。。失败!\n");
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
		printf("\t\t\t\t么得该账号\n");
		return 0;
	}
	printf("\t\t\t\t请输入新密码：");
	scanf("%s",data->data.password);
	while(getchar() != '\n')continue;
	Account_Srv_Modify(&(data->data));
	printf("\t\t\t\t修改成功\n");

	return 1;
}


int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_t buf;
	if(Account_Srv_FetchByName(usrName, &buf))
	{
		Account_Srv_DeleteByID(buf.id);
		printf("\t\t\t\t删除成功\n");
		return 1;
	}
	printf("\t\t\t\t么得该账号\n");
       	return  0;
}


int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_t buf;
	if(Account_Srv_FetchByName(usrName, &buf)){
		return 1;
	}
	printf("\t\t\t\t么得该账号\n");

	return  0;
}


void Account_UI_ShowList(account_list_t list, Pagination_t paging)
{
	account_node_t* pos;
	int i;
	printf("\n\t\t\t\t==================================================================\n");
	printf("\t\t\t\t%s\n", "账号列表");
	printf("\t\t\t\t%-10s  %-18s\n", "ID", "用户名");
	printf("------------------------------------------------------------------\n");
	//显示数据
	Paging_ViewPage_ForEach(list, paging, account_node_t, pos, i) {
		printf("\t\t\t\t%-10d  %-18s ", pos->data.id, pos->data.username);
	}
	printf("\t\t\t\t------- 共 %2d 项----------------------- 第 %2d/%2d 页----\n",
		paging.totalRecords, Pageing_CurPage(paging),
		Pageing_TotalPages(paging));

}
