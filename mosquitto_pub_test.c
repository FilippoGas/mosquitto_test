#include<stdlib.h>
#include<mosquitto.h>
#include<stdio.h>
#include<string.h>

int main(int argc, char const *argv[])
{
    int port = 1883;
    int keepalive = 60;
    bool clean_session = true;
    struct mosquitto *mosq = NULL;



    char *host = (char *)malloc(0);
    char *topic = (char *)malloc(0);
    char *message = (char *)malloc(0);



    host = (char *)realloc(host, (strlen(argv[1])));
    strcat(host, argv[1]);
    
    topic = (char *)realloc(topic, (strlen(argv[2])));
    strcat(topic, argv[2]);

    message = (char *)realloc(message, (strlen(argv[3])));
    strcat(message, argv[3]);



    mosquitto_lib_init();

    mosq = mosquitto_new(NULL,clean_session, NULL);
    if (!mosq)
    {
        fprintf(stderr,"ERROR: Connection failed.\n");
        return 1;
    }
    

    if (mosquitto_connect(mosq,host, port, keepalive))
    {
        fprintf(stderr, "Unable to connect.\n");
        return 1;
    }


    mosquitto_publish(mosq, NULL, topic, strlen(message),message,2,true);


    mosquitto_loop_forever(mosq,-1,1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();


    return 0;
}
