#include<mosquitto.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>






void my_message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *message)
{
    if (message->payloadlen)    
    {
        printf("topic:%s, message:%s\n", message->topic, message->payload);
    }else{
        printf("%s (null)\n",message->topic);
    }
    fflush(stdout);
    
};





int main(int argc, char const *argv[])
{
    int i;
    int port = 1883;
    int keepalive = 60;
    bool clean_session = true;
    struct mosquitto *mosq = NULL;

    char *host = (char *)malloc(0);
    char *topic = (char *)malloc(0);
    
    host = (char *)realloc(host, (strlen(argv[1])));
    strcat(host, argv[1]);
    
    topic = (char *)realloc(topic, (strlen(argv[2])));
    strcat(topic, argv[2]);

    
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL,clean_session,NULL);
    if (!mosq)
    {
        fprintf(stderr,"ERROR: out of memory.\n");
        return 1;
    }
    
    mosquitto_message_callback_set(mosq, my_message_callback);


    if (mosquitto_connect(mosq,host, port, keepalive))
    {
        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }

    if (mosquitto_subscribe(mosq, NULL, topic, 2))
    {
        fprintf(stderr,"Connection failed.\n");
    }
    
    
    mosquitto_loop_forever(mosq,-1,1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
