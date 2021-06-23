
#include "Play.h"
#include "../Common/list.h"
#include "../Persistence/Play_Persist.h"
#include <string.h>

int Play_Srv_Add(const play_t *data) {
	return Play_Perst_Insert(data);
}

int Play_Srv_Modify(const play_t *data) {
	return Play_Perst_Update(data);
}
int Play_Srv_DeleteByID(int ID) {
	return Play_Perst_DeleteByID(ID);
}

int Play_Srv_FetchByID(int ID, play_t *buf) {
	return Play_Perst_SelectByID(ID, buf);
}

int Play_Srv_FetchAll(play_list_t list) {
	return Play_Perst_SelectAll(list);
}

int Play_Srv_FetchByName(play_list_t list, char condt[])
{
	return Play_Perst_SelectByName(list,condt);
}


int Play_Srv_FilterByName(play_list_t list, char condt[])
{
	assert(list != NULL);
	int retval = 0;
	play_node_t* pos;
	play_node_t* temp;
	List_ForEach(list, pos)
	{
		if (!strcmp(pos->data.name, condt))
		{
			temp = pos;
			pos = pos->prev;
			List_DelNode(temp);
			free(temp);
			retval++;
		}
	}
	return retval;
}


