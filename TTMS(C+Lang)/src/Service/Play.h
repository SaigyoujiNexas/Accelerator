#pragma once

#include "../Common/Common.h"
#include <ctype.h>

typedef enum {
	PLAY_TYPE_FILE=1,
	PLAY_TYPE_OPEAR=2,
	PLAY_TYPE_CONCERT=3
} play_type_t;//剧目类型定义，1表示电影，2表示歌剧，3表示音乐会

typedef enum {
	PLAY_RATE_CHILD = 1,
	PLAY_RATE_TEENAGE = 2,
	PLAY_RATE_ADULT = 3
} play_rating_t;//演出级别类型定义，1表示儿童可观看，2表示青少年可观看，3表示成人可观看

typedef struct {//剧目信息定义
	int id;                  //信息ID号
	char name[31];           //剧目名称
	play_type_t type;        //剧目类型
	char area[9];            //剧目来源地区
	play_rating_t rating;    //剧目级别
	int duration; //以分钟为单位   //演出时长
	user_date_t start_date;  //上映日期
	user_date_t end_date;    //结束日期
	int price;               //票的价格
} play_t;


typedef struct play_node {
	play_t data;
	struct play_node *next, *prev;
} play_node_t, *play_list_t;

int Play_Srv_Add(const play_t *data);

int Play_Srv_Modify(const play_t *data);

int Play_Srv_DeleteByID(int ID);

int Play_Srv_FetchByID(int ID, play_t *buf);

int Play_Srv_FetchAll(play_list_t list);

//根据名称获取剧目
int Play_Srv_FetchByName(play_list_t list, char condt[]);
int Play_Srv_FilterByName(play_list_t list, char condt[]);

