#include "demo.h"
#include <pthread.h>


static struct event_base *base = NULL;
static evconnlistener *listener = NULL;
vector<user_t*> user_vec;


static const uint32_t WS_REQ_ONCE_READ = 1;
//static const uint32_t MAX_WS_REQ_LEN = 10240;
//static const uint64_t MAX_MSG_LEN = 1024000;


#define LOG(format, args...) fprintf(stdout, format"\n", ##args)


void user_disconnect(user_t *user) {
	if (user) {
		//update user list
		for (vector<user_t*>::iterator iter = user_vec.begin(); iter != user_vec.end(); ++iter) {
			if (*iter == user) {
				user_vec.erase(iter);
				break;
			}
		}
		user_destroy(user);
	}
	LOG("now, %d users connecting", user_vec.size());
}


void user_disconnect_cb(void *arg) {
	LOG("%s", __func__);
	user_t *user = (user_t*)arg;
	user_disconnect(user);
}


void listencb(struct evconnlistener *listener, evutil_socket_t clisockfd, struct sockaddr *addr, int len, void *ptr) {
	struct event_base *eb = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(eb, clisockfd, BEV_OPT_CLOSE_ON_FREE);
	LOG("a user logined in, socketfd = %d", bufferevent_getfd(bev));

	//create a user
	user_t *user = user_create();
	user->wscon->bev = bev;
	user_vec.push_back(user);
	//ws_conn_setcb(wscon, HANDSHAKE, testfunc, (void*)"haha");
	ws_conn_setcb(user->wscon, FRAME_RECV, frame_recv_cb, bev);
	ws_conn_setcb(user->wscon, CLOSE, user_disconnect_cb, bev);

	ws_serve_start(user->wscon);
}


static void * HandleReceivePeerServerData(void *arg){
           pthread_detach(pthread_self());
           
		   
}

int main() {
	//SIGPIPE ignore
	pthread_t pid;
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	if (sigaction(SIGPIPE, &act, NULL) == 0) {
		LOG("SIGPIPE ignore");
	}

	//initialize
	setbuf(stdout, NULL);
	base = event_base_new();
	assert(base);

	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr =inet_addr("172.16.103.125");
	srvaddr.sin_port = htons(9000);

	listener = evconnlistener_new_bind(base, listencb, NULL, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 500, (const struct sockaddr*)&srvaddr, sizeof(struct sockaddr));
	assert(listener);

	//启动另外一个服务

    pthread_create(&pid,NULL,HandleReceivePeerServerData,NULL);

	
	event_base_dispatch(base);

	LOG("loop exit");

	evconnlistener_free(listener);
	event_base_free(base);

	return 0;
}
