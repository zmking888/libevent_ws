#include "user.h"


extern vector<user_t*> user_vec;

extern vector<user_t *>valid_user;

user_t *user_create() {
	user_t *user = new (nothrow) user_t;
	if (user) {
		user->id = 0;
		user->wscon = ws_conn_new();
		user->msg = "";
	}
	return user;
}


void user_destroy(user_t *user) {
	if (user) {
		if (user->wscon) {
			ws_conn_free(user->wscon);
		}
		delete user;
	}
}

//create connection to db 
void _HandleCreateChatRoom(jsonxx :: Object &obj){
          //teamtalk connect
          
}


//close connect to 
void _HandleCreateChatRoom(jsonxx :: Object &obj){       

}

//handle login 
/*
{
"cmd":"loginChatRoom",   //或者使用数字表示
"data":
{
"imId":登录者imid,
"name":登录者名字,
"portrait":登录者头像地址,
"liveId":唯一的直播Id
}
}
*/
void _HandleLogin(jsonxx::Object & obj,user_t *user){
         jsonxx::Object object=obj.get<jsonxx::Object>("data");
		 int imid=object.get<jsonxx::Number>("imId");
		 string username=object.get<jsonxx::String>("name");
		 string portrait=object.get<jsonxx::String>("portrait");
		 int liveId=object.get<jsonxx::Number>("liveId");
         vector<user_t *>::iterator iter=user_vec.begin();
         for(;iter<user_vec.end();iter++)
         {
              if(imid==(iter->id) &&imid!=0&&(iter->id!=0))
              {
                break;
              }
         }
		 if(iter!=user_vec.end()){
		 	//has logined
		 }else{
		    iter->id=imid;
			iter->liveId=liveId;
			iter->portrait=portrait;
			iter->name=username;
		 }
		 //teamtalk todo
		 
}




void frame_recv_cb(void *arg) {

	user_t *user = (user_t*)arg;
	if (user->wscon->frame->payload_len > 0) {
		user->msg += string(user->wscon->frame->payload_data, user->wscon->frame->payload_len);
	}
	if (user->wscon->frame->fin == 1) {
		LOG("%s", user->msg.c_str());

		frame_buffer_t *fb = frame_buffer_new(1, 1, user->wscon->frame->payload_len, user->wscon->frame->payload_data);
	    //receive client json data
	    //parse json data
	    jsonxx::Object obj;
		std::istringstream input(fb->data);
	    if(!obj.parse(input)){
			 //user->wscon->close_cb_unit(user->wscon->);
			 //exception
			 //to do
		}
		
		if(obj.has<string>("cmd")&&obj.has<jsonxx::Object>("data")){
			 string cmdstr=obj.get<string>("cmd");
			 string datacontent=obj.get<jsonxx::Object>("data");
			 if(comdstr=="createChatRoom"){
			 	  _HandleCreateChatRoom(obj);//create connect to db 
			 	  
			 }else if(cmdstr=="closeChatRoom"){
			 
			      _HandleCloseChatRoom(obj);//create close connect to db 
			      
			 }else if(cmdstr=="loginChatRoom")//login
			 {     
			      //todo something() 
			      _HandleLogin(obj,user);
			 }else if(cmdstr=="logoutChatRoom "){//logout
			      
			 }else if (cmdstr=="sendMsgToChatRom")//send msg
			 {
			   
			 }else if(cmdstr=="MsgToChatRoom")//msg to char
			 {
			 	
			 }else if(cmdstr=="forbidChatRoom"){//send forbidden mark
                
			 
			 }else if(cmdstr=="getChatRoomForbiddenStatus")//get forbidden status
			 {
			  
			 }else if(cmdstr=="NotifyChatRoomForbiddenStatus"){//notify
			 	
			 }else if(cmdstr=="kickChatRoomMemberStatus"){ //kick chatmember
			 	
			 }
			 
		}else if(obj.has<string>("cmd")&&obj.has<string>("result")&&obj.has<string>("reason")&&obj.has<jsonxx::Object>("data")){
		     //response json
		     
		}else {
		 //todo exception()
		 
		}

		if (fb) {
			//send to other users
			for (int32_t i = 0; i < user_vec.size(); ++i) {
				if (user_vec[i] != user) {
#if 1
					if (send_a_frame(user_vec[i]->wscon, fb) == 0) {
						LOG("i send a message");
					}
#endif
				}
			}

			frame_buffer_free(fb);
		}

		user->msg = "";
	}
}
