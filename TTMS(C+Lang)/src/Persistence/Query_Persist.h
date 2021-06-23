#pragma once

#include "../Service/play.h"
#include "../Service/schedule.h"
#include "../Service/studio.h"

//根据名字查询 
int Query_PlayName(char *playName, play_t *_play);

//根据ID查询 
int Query_Schedule_By_PlayId(int playId, schedule_t *sched);


void Query_Studio_By_Schedule(int id, studio_t* name);

int Sold_Ticket_Count(int id);

int Seat_Number_Count(int studio_id);

