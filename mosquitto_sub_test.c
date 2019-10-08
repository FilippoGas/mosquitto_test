#include<mosquitto.h>
#include<stdio.h>




void my_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
    printf("%s\n",str);
};

void my_connect_callback(struct mosquitto *mosq, void *userdata, int result)
{
    if (!result)
    {
            mosquitto_subscribe(mosq, NULL, "test",2);
    }else
    {
        fprintf(stderr, "Connection failed.\n");
    }
    
    
};


void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (message->payloadlen)    
    {
        printf("%s %s\n", message->topic, message->payload);
    }else{
        printf("%s (null)\n",message->topic);
    }
    fflush(stdout);
    
};


void my_subscribe_callback(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos){

    int i;
    printf("Subscribed (mid : %d):%d", mid, granted_qos[0]);
    for (i = 1; i < qos_count; i++)
    {
        printf(", %d",granted_qos[i]);
    }
    printf("\n");

}




int main(int argc, char const *argv[])
{
    int i;
    int port = 1883;
    char *host = "localhost";
    int keepalive = 60;
    bool clean_session = true;
    struct mosquitto *mosq = NULL;


    mosquitto_lib_init();

    mosq = mosquitto_new(NULL,clean_session,NULL);
    if (!mosq)
    {
        fprintf(stderr,"ERROR: out of memory.\n");
        return 1;
    }
    
    mosquitto_log_callback_set(mosq,my_log_callback);
    mosquitto_connect_callback_set(mosq, my_connect_callback);
    mosquitto_message_callback_set(mosq, my_message_callback);
    mosquitto_subscribe_callback_set(mosq, my_subscribe_callback);



    mosquitto_publish();
    if (mosquitto_connect(mosq,host, port, keepalive))
    {
        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }
    
    mosquitto_loop_forever(mosq,-1,1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
