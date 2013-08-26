#ifndef message_h
#define messagep_h

/*
 * message
 */

#define MESSAGE_TYPE_NULL   0
typedef struct _message {
    int     type;
}message;
int message_post( message*, messagebump* );


/*
 * messagebump
 */

#define MESSAGEBUMP_SUSPEND 0
#define MESSAGEBUMP_ACTIVE  1
#define MESSAGEBUMP_MAX     100

typedef struct _messagebump {
    int             state;
    int             current_message;

    message         message_list[MESSAGEBUMP_MAX];
}messagebump;

int messagebump_post_message( messagebump*, message* );
int messagebump_process_message( messagebump* );







#endif
