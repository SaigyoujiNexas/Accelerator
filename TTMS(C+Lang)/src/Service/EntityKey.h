#pragma once

typedef struct {
	char entyName[41];
	long key;
} entity_key_t;

typedef struct entity_key_node{
	entity_key_t data;
	struct entity_key_node *prev;
	struct entity_key_node *next;
}entkey_node_t, *entkey_list_t;

long EntKey_Srv_CompNewKey(char entName[]);

long EntKey_Srv_CompNewKeys(char entName[], int count);

void EntKey_Srv_Add2List(entkey_list_t keyList, long key);

int EntKey_Srv_CheckExist(entkey_list_t keyList, long key);