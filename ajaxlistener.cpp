#include "ajaxlistener.h"






static String readsock(SOCKET sock){
    String ret = "";
    unsigned int len = 1000;
    unsigned int actual = 0;
    while(1){
        len *= 2;
        ret.reserve(len);
        ret.resize(len);
        int tm = recv( sock, (char*)ret.c_str()+actual, len-actual, 0 );
        if( tm >= 0 ) actual += tm;
        if( ret.indexof("\r\n\r\n") >= 0 ) break;
        if( tm <= 0 ) break;


    }
    ret.resize(actual);
    return ret;
}









void ajaxlistener::runcallback(void* sel){
    cbargs* self = (cbargs*) sel;
    //String cook = self->con->cookie;
    self->self->callback( *(self->con), self->data );
    //self->self->remove(cook);
    self->self->connections[self->con->cookie] = 0;
    delete self->con;
    delete self;
}
void ajaxlistener::docallback( ajaxconnection* c, void* d ){
    cbargs* a = new cbargs();
    a->self = this;
    a->con = c;
    a->data = d;
    call_thread( runcallback, (void*) a );
}

void ajaxlistener::ajaxconnection::lock(){
    mutex_lock(locked);
}
void ajaxlistener::ajaxconnection::unlock(){
    mutex_unlock(locked);
}


ajaxlistener::ajaxconnection::ajaxconnection(){
    locked = mutex_create();
    sendqueue = "";
    polling = 0;
}

int ajaxlistener::ajaxconnection::send(string data){
    if( polling > 0 ){
        printf("SENTlOl\n");
        lock();
        int ret = updatetime(sendhttp( polling, 200, "text/plain", sendqueue+data ));
        sendqueue = "";
        polling = 0;
        unlock();
        return ret;
    }
    lock();
    printf("SEPOOP\n");
    sendqueue+=data;
    unlock();
    return -1;
}

int ajaxlistener::remove(String cookie){
    printf("Closing %s\n", cookie.c_str()); fflush(stdout);
    int ret = 0;
    ajaxconnection* c = connections[cookie];
    if( c != 0 ){
        //delete c;
        connections[cookie]=0;
        ret = 1;
    }
    return ret;
}
int ajaxlistener::isvalid(String cookie){
    return connections[cookie] != 0;
}

String ajaxlistener::ajaxconnection::recv(int block){
    if( block )
        while( dataqueue.size() == 0 ){ Sleep(1); }
    lock();
    String ret = "";
    for( unsigned int i = 0; i < dataqueue.size(); ++i ){
        ret += dataqueue.front();
        dataqueue.pop_front();
    }
    unlock();
    updatetime(ret.size());
    return ret;
}
int ajaxlistener::ajaxconnection::close(){

    printf("Closing %s\n", cookie.c_str()); fflush(stdout);
    parent->remove(cookie);
    return 1;
}
void ajaxlistener::ajaxconnection::push( SOCKET sock, String data ){
    lock();

    dataqueue.push_back(data);
    if( sendqueue.size() > 0 ){

        if( sock != 0 ){
            printf("SendingA: %s", sendqueue.c_str());

            updatetime(sendhttp( sock, 200, "text/html", sendqueue ));
            if( polling != 0 )
                updatetime(sendhttp( polling, 200, "text/html", "" ));
            polling = 0;
            sendqueue = "";
        }
        else if( polling != 0 ){
            printf("SendingB: %s", sendqueue.c_str());
            updatetime(sendhttp( polling, 200, "text/html", sendqueue ) );
            polling = 0;
            sendqueue = "";
        }

    }
    else
    if( sock != 0 ){
        if( polling != 0 )
            updatetime(sendhttp( polling, 200, "text/html", "" ));
        polling = sock;
    }

    unlock();
}

String ajaxlistener::generatecookie(SOCKET sock){
        long a = 1;// = (long) this;
        a *= time(0);
        a *= getip(sock);
        String ret = "";
        for( int i = 0; i < 32; ++i ){
            int t = (char)(i*rand()*a%255);
            const char* lookup = "0123456789ABCDEF";
            ret += lookup[t & 0xF];
            ret += lookup[(t>>4) & 0xF];
        }
        return "A";
        return ret;
    }

ajaxlistener::ajaxlistener(){
    sock = 0;
}



int ajaxlistener::listen( int port, int (*cb)(ajaxconnection&,void*), void* userdata ){
    sock = bindSocket( port );
    ::listen( sock, 10 );
    callback = cb;
    while( true ){
        SOCKET acceptsock = accept( sock, 0, 0 );
        if( acceptsock == 0 ) {fprintf(stderr, "Error reading from AJAX Socket %i\n", port); break;}
        String a = readsock( acceptsock );

        http h = parsehttp( a );
        if( h.path == "/ajax.php" ){
            if( (h.urldata["session"]).tolower() == "new"){
                ajaxconnection* c = new ajaxconnection();
                c->ip = getip(acceptsock);
                c->cookie = generatecookie(acceptsock);
                c->parent = this;

                if( connections[c->cookie] != 0 ){
                    delete c;
                    sendhttp( acceptsock, 404, "text/html", "error" );
                    continue;
                }

                c->state = 200;
                c->polling = 0;
                c->timesincelast = time(0);
                c->timeout = 30;

                sendhttp( acceptsock, 200, "text/html", c->cookie );
                connections[c->cookie] = c;
                docallback(c, userdata);

            }
            else if( h.urldata["session"] != "" ){

                ajaxconnection *c = connections[h.urldata["session"]];

                if( c != 0 && c->ip == getip(acceptsock)){
                    if( c != 0 ){
                        c->push( h.urldata["control"].tolower() == "poll" ? acceptsock : 0, h.urldata["data"] );
                        if( h.urldata["control"].tolower() != "poll" ) c->updatetime(sendhttp( acceptsock, 200, "text/html", "" ));
                    }
                    else{
                        sendhttp( acceptsock, 404, "text/html", "Not found" );

                    }
                }
                else{
                    sendhttp( acceptsock, 404, "text/html", "Not found" );

                }
            }
            else{

            }
        }
        if( h.path == "/index.html" || h.path == "/" ){
            printf("lol");
            servefile( acceptsock, "index.html" );
        }
        //h.urldata

    }
    return 0;
}

int ajaxlistener::ajaxconnection::updatetime(int val){
    if( val > 0 )
        timesincelast = time(0);
    if( ((signed int)time(0) - (signed int)timesincelast) > (signed int)timeout )
        close();
        printf("Diff: %i\n", (int)(time(0) - timesincelast)); fflush(stdout);
    return val;
}

ajaxlistener::ajaxconnection::~ajaxconnection(){
    lock();
    parent->connections[cookie] = 0;
    if( polling != 0 ){
        sendhttp( polling, 404, "text/html", "Connection Closed" );

    }
    sendqueue = "";
    while( dataqueue.size() > 0 ) { dataqueue.back() = ""; dataqueue.pop_back(); }
    cookie = "";


    unlock();
}
