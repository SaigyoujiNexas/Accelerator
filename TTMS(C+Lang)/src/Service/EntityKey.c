#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EntityKey.h"
#include "../Common/Common.h"
#include "../Common/list.h"
#include "../Persistence/EntityKey_Persist.h"


long EntKey_Srv_CompNewKey(char entName[]) {
	return EntKey_Srv_CompNewKeys(entName, 1);
}

long EntKey_Srv_CompNewKeys(char entName[], int count) {
	char entUpperName[41];

	if (count < 1) {
		printf("Entity count must be bigger than 0!\n");
		return 0;
	}

	int nameLen = strlen(entName);
	if (nameLen < 0 || nameLen > 40) {
		printf("Entity name must be within 1~40!\n");
		return 0;
	}

	memcpy(entUpperName, entName, nameLen);
	entUpperName[nameLen] = '\0';


	Str2Upper(entUpperName);

	return EntKey_Perst_GetNewKeys(entUpperName, count);
}

void EntKey_Srv_Add2List(entkey_list_t keyList, long key) 
{
	assert(NULL != keyList);
	if (!EntKey_Srv_CheckExist(keyList, key)) {
		entkey_node_t *newNode = (entkey_node_t *) malloc(
				sizeof(entkey_node_t));
		if (NULL == newNode) {
			printf("Memory overflow!\n");
			return;
		} else {

			newNode->data.key = key;
			List_AddTail(keyList, newNode);
		}
	}
}

int EntKey_Srv_CheckExist(entkey_list_t keyList, long key){
	assert(NULL!=keyList);
	entkey_node_t *p;
	List_ForEach(keyList, p){
		if(p->data.key==key)
			return 1;
	}
	return 0;
}
