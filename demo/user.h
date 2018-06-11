/**
 *
 * filename: user.h
 * summary:
 * author: caosiyang
 * email: csy3228@gmail.com
 *
 */
#ifndef USER_H
#define USER_H

#include "../connection.h"
#include "jsonxx.h"
#include <stringstream>

using namespace jsonxx;


typedef struct user {
public:
	 user(ws_conn_t * con, uint32_t userid,string nick,string portraits,uint32_t liveIds){
	 	  id=userid;
		  name=nick;
		  portrait=portraits;
		  liveId=liveIds;
		  wscon=con;
	 }
public:
	uint32_t id;
	string name;
	string portrait;
	uint32_t liveId;
	ws_conn_t *wscon;
	string msg;
} user_t;



void _HandleCreateChatRoom(jsonxx::Object &,user_t *);
void _HandleCloseChatRoom(jsonxx::Object&,user_t*);

void _HandleLogin(jsonxx::Object &,user_t*);


user_t *user_create();


void user_destroy(user_t *user);


void frame_recv_cb(void *arg);


#endif
