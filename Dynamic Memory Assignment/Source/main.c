#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STACK_SIZE 4

typedef struct layer Layer;
typedef struct route Route;
typedef struct client Client;
typedef struct log Log;
typedef struct stack Stack;
typedef struct queue Queue;
typedef struct chunk_carrier Chunks;
typedef Layer* layerPtr ;
typedef Route* routePtr;
typedef Client* clientPtr;
typedef Log* logPtr;
typedef Stack* sPtr;
typedef Queue* qPtr;

clientPtr search_client(clientPtr, char*);

void split_chunks(Chunks*, char*, int);

layerPtr pop(sPtr);

void push(sPtr, layerPtr);

void enqueue(qPtr, sPtr);

sPtr dequeue(qPtr);

void init_queue(qPtr, int, int);

void init_stack(sPtr);

void strip(char*, char*);

int seek_index(clientPtr, char*);

void send(clientPtr, clientPtr, clientPtr, clientPtr, int, char*);

void show(qPtr, char*, int);

void log_event (clientPtr, char, char*, char*);

void show_log (clientPtr, char*, int);

struct layer{

    /* FOR APPLICATION LAYER*/
    char ID_sender[2];
    char ID_reciever[2];
    char* message_chunk;

    /*FOR TRANSPORT LAYER*/
    char port_sender[8];
    char port_reciever[8];

    /*FOR NETWORK LAYER*/
    char IP_sender[8];
    char IP_reciever[8];

    /*FOR PHYSICAL LAYER*/
    char MAC_sender[11];
    char MAC_reciever[11];

};

struct stack{
    int top;
    int hop;
    Layer layer_stack[MAX_STACK_SIZE];
};

struct queue{
    int rear;
    int front;
    sPtr layer_queue;
};

struct route{
    char dest[2];
    char reciever[2];
};

struct log{
    char time_stamp[17];
    char activity[18];
    char success[4];
    char reciever[2];
    char sender[2];
};

struct client{
    char ID[2];
    char MAC[11];
    char IP[8];
    int num_route;
    int num_log;
    int hops;
    logPtr client_log;
    Route* routing_table;
    Queue incoming;
    Queue outgoing;
};

struct chunk_carrier{
    int chunk_number;
    char** chunks;
};
/* ____________________________________________________________________________________________________________________________*/
/*|____________________________________________________________________________________________________________________________|*/

int main(int argc, char* argv[]) {

    char line[255];
    char* ptr;
    int chunksize = (int) strtol(argv[4], &ptr, 10);;
    int client_count = 0;
    int total_clients = 0;
    int cnt=0;
    int cnt_1;
    int client_index=0;
    char* sender_and_reciever[2];
    char* message_carrier;
    int chunkcounter;
    char port1[8];
    char port2[8];
    strcpy(port1, argv[5]);
    strcpy(port2, argv[6]);
    Layer new_layer;
    Stack new_stack;
    Client new_client;
    clientPtr clients;
    Chunks all_chunks;

    init_stack(&new_stack);
    new_stack.layer_stack->message_chunk = malloc((chunksize + 1) * sizeof(char));
    new_layer.message_chunk = malloc((chunksize + 1) * sizeof(char));
    FILE* client_file;
    client_file = fopen(argv[1], "r");

    FILE* routing;
    routing = fopen(argv[2], "r");

    FILE* commands;
    commands = fopen(argv[3], "r");


    /*###############################################*/
    /*#READING CLIENTS FILE & INITIALIZING CLIENTS: #*/
    /*###############################################*/
    fscanf(client_file, "%s", line);
    total_clients = (int) strtol(line, &ptr, 10);
    clients = malloc((total_clients + 1) * sizeof(Client));
    while(fscanf(client_file, "%s", line) != EOF){
        if(cnt == 0){
            strcpy(new_client.ID, line);
            cnt++;
        }
        else if(cnt == 1){
            strcpy(new_client.IP, line);
            cnt++;
        }
        else if(cnt == 2){
            strcpy(new_client.MAC, line);
            new_client.num_log=0;
            new_client.client_log =malloc(sizeof(Log));
            clients[client_count++] = new_client;
            cnt++;
        }
        cnt %=3;
    }

    cnt = 0;
    client_count = 0;


    /*############################################*/
    /*#READING ROUTE FILE & INITIALIZING ROUTES: #*/
    /*############################################*/
    while(fscanf(routing, "%s", line) != EOF) {
        if(cnt%2 == 0){
            if(cnt == 0){
                clients[client_count].routing_table = malloc(1 * sizeof(Route));
                strcpy(clients[client_count].routing_table[0].dest, line);
                clients[client_count].num_route = 0;
                cnt++;
            }
            else{
                clients[client_count].routing_table = realloc(clients[client_count].routing_table, (cnt/2+1) * sizeof(Route));
                strcpy(clients[client_count].routing_table[cnt/2].dest, line);
                cnt++;
            }
        }
        else if(cnt%2 == 1){
            if(cnt == 1){
                strcpy(clients[client_count].routing_table[0].reciever, line);
                clients[client_count].num_route = 1;
                cnt++;
            }
            else{
                strcpy(clients[client_count].routing_table[cnt/2].reciever, line);
                cnt++;
                clients[client_count].num_route = cnt/2;
            }
        }
        if(cnt%2 == 1 && strcmp("-", line)==0){
            cnt = 0;
            client_count++;
        }
    }

    /*########################################*/
    /*#READING COMMANDS AND PROCESSING EACH: #*/
    /*########################################*/

    fscanf(commands, "%s", line);
    while(fscanf(commands, "%s", line) != EOF){
        if(line[0]>=65 && line[0]<=90) {
            if (strcmp(line, "MESSAGE") == 0) {

                printf("\n---------------------------------------------------------------------------------------\n"
                               "Command: %s", line);
                /*####################################*/
                /*#Initializing sender and reciever: #*/
                /*####################################*/
                fscanf(commands, "%s", line);
                printf(" %s", line);
                sender_and_reciever[0] = malloc((strlen(line) + 1) * sizeof(char));
                strcpy(sender_and_reciever[0], line);
                fscanf(commands, "%s", line);
                printf(" %s", line);
                sender_and_reciever[1] = malloc((strlen(line) + 1) * sizeof(char));
                strcpy(sender_and_reciever[1], line);

                /*#########################################################################*/
                /*#GETTING RID OF "#" CHARACTERS AND COPYING MESSAGE TO SPLIT INTO CHUNKS #*/
                /*#########################################################################*/
                fscanf(commands, "%s", line);
                printf(" %s", line);
                if(line[0] == '#' && line[strlen(line)-1] != '#') {
                    message_carrier = malloc((strlen(line) + 2) * sizeof(char));
                    strip(line, message_carrier);
                    strcat(message_carrier, " ");
                    fscanf(commands, "%s", line);
                    printf(" %s", line);
                    while (line[strlen(line) - 1] != '#') {
                        message_carrier = realloc(message_carrier, (strlen(message_carrier) + strlen(line) + 2));
                        strcat(message_carrier, line);
                        strcat(message_carrier, " ");
                        fscanf(commands, "%s", line);
                        printf(" %s", line);
                    }
                    printf("\n---------------------------------------------------------------------------------------\n");
                    message_carrier = realloc(message_carrier, (strlen(message_carrier) + strlen(line) + 2));
                    strip(line, message_carrier);
                }
                else{
                    message_carrier = malloc((strlen(line)-1) * sizeof(char));
                    strip(line, message_carrier);
                }
                printf("\nMessage to be sent: %s\n", message_carrier);
                split_chunks(&all_chunks, message_carrier, chunksize);

                /*INITIALIZING INCOMING&OUTGOING QUEUES: */
                for (cnt = 0; cnt < total_clients; cnt++) {
                    init_queue(&clients[cnt].incoming, all_chunks.chunk_number, chunksize);
                    init_queue(&clients[cnt].outgoing, all_chunks.chunk_number, chunksize);
                }
                cnt = 0;

                new_layer.message_chunk = malloc((chunksize + 1) * sizeof(char));

                for (chunkcounter = 0; chunkcounter < all_chunks.chunk_number; chunkcounter++) {
                    init_stack(&new_stack);
                    strcpy(new_layer.message_chunk, all_chunks.chunks[chunkcounter]);
                    strcpy(new_layer.ID_reciever, search_client(clients, sender_and_reciever[1])->ID);
                    strcpy(new_layer.ID_sender, search_client(clients, sender_and_reciever[0])->ID);
                    push(&new_stack, &new_layer);

                    strcpy(new_layer.port_sender, port1);
                    strcpy(new_layer.port_reciever, port2);
                    push(&new_stack, &new_layer);

                    strcpy(new_layer.IP_reciever, search_client(clients, sender_and_reciever[1])->IP);
                    strcpy(new_layer.IP_sender, search_client(clients, sender_and_reciever[0])->IP);
                    push(&new_stack, &new_layer);

                    strcpy(new_layer.MAC_reciever, search_client(clients, sender_and_reciever[1])->MAC);
                    strcpy(new_layer.MAC_sender, search_client(clients, sender_and_reciever[0])->MAC);
                    push(&new_stack, &new_layer);
                    printf("Frame #%d\n"
                                   "Sender MAC address: %s, Receiver MAC address: %s\n"
                                   "Sender IP address: %s, Receiver IP address: %s\n"
                                   "Sender port number: %s, Receiver port number: %s\n"
                                   "Sender ID: %s, Receiver ID: %s\n"
                                   "Message chunk carried: %s\n"
                                   "--------\n", chunkcounter + 1, new_layer.MAC_sender, new_layer.MAC_reciever,
                           new_layer.IP_sender, new_layer.IP_reciever, new_layer.port_sender, new_layer.port_reciever,
                           new_layer.ID_sender, new_layer.ID_reciever, new_layer.message_chunk);

                    enqueue(&search_client(clients, sender_and_reciever[0])->outgoing, &new_stack);
                }
            } else if (strcmp(line, "SHOW_FRAME_INFO") == 0) {

                printf("\n---------------------------------------------------------------------------------------\n"
                               "Command: %s", line);
                fscanf(commands, "%s", line);
                printf(" %s", line);
                client_index = seek_index(clients, line);
                fscanf(commands, "%s", line);
                printf(" %s", line);

                if (strcmp(line, "in") == 0) {
                    fscanf(commands, "%s", line);
                    printf(" %s", line);
                    printf("\n---------------------------------------------------------------------------------------\n");
                    if (clients[client_index].incoming.rear >= (int) strtol(line, &ptr, 10) &&
                        (int) strtol(line, &ptr, 10) >= clients[client_index].incoming.front) {
                        show(&clients[client_index].incoming, clients[client_index].ID, (int) strtol(line, &ptr, 10));
                    } else {
                        printf("No such frame.");
                    }

                } else if (strcmp(line, "out") == 0) {
                    fscanf(commands, "%s", line);
                    printf(" %s", line);
                    printf("\n---------------------------------------------------------------------------------------\n");
                    if (clients[client_index].outgoing.rear >= (int) strtol(line, &ptr, 10) &&
                        (int) strtol(line, &ptr, 10) >= clients[client_index].outgoing.front) {
                        show(&clients[client_index].outgoing, clients[client_index].ID, (int) strtol(line, &ptr, 10));
                    } else {
                        printf("No such frame.\n");
                    }
                }
            } else if (strcmp(line, "SHOW_Q_INFO") == 0) {

                printf("\n---------------------------------------------------------------------------------------\n"
                               "Command: %s", line);
                fscanf(commands, "%s", line);
                printf(" %s", line);
                client_index = seek_index(clients, line);
                fscanf(commands, "%s", line);
                printf(" %s", line);
                printf("\n---------------------------------------------------------------------------------------\n");
                if (strcmp(line, "in") == 0) {
                    printf("Client %s Incoming Queue Status\n", clients[client_index].ID);
                    show(&clients[client_index].incoming, clients[client_index].ID, -1);
                } else if (strcmp(line, "out") == 0) {
                    printf("Client %s Outgoing Queue Status\n", clients[client_index].ID);
                    show(&clients[client_index].outgoing, clients[client_index].ID, -1);


                }
            } else if (strcmp(line, "SEND") == 0) {

                printf("\n---------------------------------------------------------------------------------------\n"
                               "Command: %s", line);
                fscanf(commands, "%s", line);
                printf(" %s ", line);
                printf("\n---------------------------------------------------------------------------------------\n");
                if(strcmp(line, sender_and_reciever[0]) == 0){
                    log_event(search_client(clients, sender_and_reciever[0]), 'R', sender_and_reciever[1], sender_and_reciever[0]);
                    log_event(search_client(clients, sender_and_reciever[0]), 'F', sender_and_reciever[1], sender_and_reciever[0]);
                    send(search_client(clients, sender_and_reciever[0]),
                         search_client(clients, sender_and_reciever[1]),
                         search_client(clients, sender_and_reciever[1]), clients, all_chunks.chunk_number,
                         sender_and_reciever[0]);
                }
                else{
                    printf("No message in outgoing queue of client %s", line);
                }
            } else if (strcmp(line, "PRINT_LOG") == 0) {

                printf("\n---------------------------------------------------------------------------------------\n"
                               "Command: %s", line);
                fscanf(commands, "%s", line);
                printf(" %s ", line);
                printf("\n---------------------------------------------------------------------------------------\n");
                printf("Client %s Logs:\n", line);
                show_log(search_client(clients, line), message_carrier, all_chunks.chunk_number);
            }
            else{

                printf("\n---------------------------------------------------------------------------------------\n"
                               "Invalid Command: %s", line);
            }
        }
        else{
            if(line[0]>=90 || line[0]<=65){
                printf(" %s", line);
            }
        }

    }


    /*####################*/
    /*#FREE'ING POINTERS: */
    /*####################*/

    for(cnt = 0; cnt<total_clients; cnt++){
        for(cnt_1 = 0; cnt_1<all_chunks.chunk_number; cnt_1++){
            free(clients[cnt].outgoing.layer_queue[cnt_1].layer_stack[0].message_chunk);
            free(clients[cnt].incoming.layer_queue[cnt_1].layer_stack[0].message_chunk);
        }
        free(clients[cnt].routing_table);
        free(clients[cnt].outgoing.layer_queue);
        free(clients[cnt].incoming.layer_queue);
        free(clients[cnt].client_log);
    }

    free(new_stack.layer_stack->message_chunk);
    free(new_layer.message_chunk);
    for(cnt = 0; cnt<all_chunks.chunk_number;cnt++){
        free(all_chunks.chunks[cnt]);
    }
    free(all_chunks.chunks);
    free(sender_and_reciever[0]);
    free(sender_and_reciever[1]);
    free(clients);
    free(message_carrier);
    fclose(client_file);
    fclose(routing);
    fclose(commands);
    return 0;
}
/* ____________________________________________________________________________________________________________________________*/
/*|____________________________________________________________________________________________________________________________|*/


clientPtr search_client(clientPtr cls, char ID[2]){
    int cnt = 0;
    while(strcmp(cls[cnt].ID, ID) != 0){
        cnt++;
    }
    return &cls[cnt];
}

int seek_index(clientPtr cls, char* ID){
    int cnt = 0;
    while(strcmp(cls[cnt].ID, ID) != 0){
        cnt++;
    }
    return cnt;
}

layerPtr pop(sPtr st){
    if(st->top > -1){
        return &(st->layer_stack[st->top--]);
    }

}

void push(sPtr st, layerPtr new_layer){
    if(st->top < MAX_STACK_SIZE){
        if(st->top == 2) {
            strcpy(st->layer_stack[3].MAC_sender, new_layer->MAC_sender);
            strcpy(st->layer_stack[3].MAC_reciever, new_layer->MAC_reciever);
            st->top++;
                }

        else if(st->top == 1){

            strcpy(st->layer_stack[2].IP_reciever,new_layer->IP_reciever);
            strcpy(st->layer_stack[2].IP_sender,new_layer->IP_sender);
            st->top++;
                }
        else if(st->top == 0){

            strcpy(st->layer_stack[1].port_reciever, new_layer->port_reciever);
            strcpy(st->layer_stack[1].port_sender, new_layer->port_sender);
            st->top++;
                }
        else if(st->top == -1){
            strcpy(st->layer_stack[0].message_chunk, new_layer->message_chunk);
            strcpy(st->layer_stack[0].ID_sender, new_layer->ID_sender);
            strcpy(st->layer_stack[0].ID_reciever, new_layer->ID_reciever);
            st->top++;
                }


    }
}

void enqueue(qPtr q, sPtr ly){
    if(q->rear == -1){
        strcpy(q->layer_queue[++q->rear].layer_stack[0].message_chunk, ly->layer_stack[0].message_chunk);
        strcpy(q->layer_queue[q->rear].layer_stack[0].ID_sender, ly->layer_stack[0].ID_sender);
        strcpy(q->layer_queue[q->rear].layer_stack[0].ID_reciever, ly->layer_stack[0].ID_reciever);

        strcpy(q->layer_queue[q->rear].layer_stack[1].port_sender, ly->layer_stack[1].port_sender);
        strcpy(q->layer_queue[q->rear].layer_stack[1].port_reciever, ly->layer_stack[1].port_reciever);

        strcpy(q->layer_queue[q->rear].layer_stack[2].IP_reciever, ly->layer_stack[2].IP_reciever);
        strcpy(q->layer_queue[q->rear].layer_stack[2].IP_sender, ly->layer_stack[2].IP_sender);

        strcpy(q->layer_queue[q->rear].layer_stack[3].MAC_sender, ly->layer_stack[3].MAC_sender);
        strcpy(q->layer_queue[q->rear].layer_stack[3].MAC_reciever, ly->layer_stack[3].MAC_reciever);
        q->layer_queue[q->rear].top = ly->top;
        q->layer_queue[q->rear].hop = ly->hop;
        q->front++;
    }
    else{
        strcpy(q->layer_queue[++q->rear].layer_stack[0].message_chunk, ly->layer_stack[0].message_chunk);
        strcpy(q->layer_queue[q->rear].layer_stack[0].ID_sender, ly->layer_stack[0].ID_sender);
        strcpy(q->layer_queue[q->rear].layer_stack[0].ID_reciever, ly->layer_stack[0].ID_reciever);

        strcpy(q->layer_queue[q->rear].layer_stack[1].port_sender, ly->layer_stack[1].port_sender);
        strcpy(q->layer_queue[q->rear].layer_stack[1].port_reciever, ly->layer_stack[1].port_reciever);

        strcpy(q->layer_queue[q->rear].layer_stack[2].IP_reciever, ly->layer_stack[2].IP_reciever);
        strcpy(q->layer_queue[q->rear].layer_stack[2].IP_sender, ly->layer_stack[2].IP_sender);

        strcpy(q->layer_queue[q->rear].layer_stack[3].MAC_sender, ly->layer_stack[3].MAC_sender);
        strcpy(q->layer_queue[q->rear].layer_stack[3].MAC_reciever, ly->layer_stack[3].MAC_reciever);
        q->layer_queue[q->rear].top = ly->top;
        q->layer_queue[q->rear].hop = ly->hop;
    }
}

sPtr dequeue(qPtr q){
    if(q->rear > -1 && q->front <= q->rear){
        return &q->layer_queue[q->front++];
    }
}

void split_chunks(Chunks* ch, char* str, int max_char){
    int chunk_number;
    int i = 0,
            j = 0;
    chunk_number = (int) strlen(str) / max_char + 1;
    ch->chunks = malloc(chunk_number * sizeof(char*));
    for(i; i<chunk_number; i++){
        ch->chunks[i] = malloc(max_char * sizeof(char));
    }
    i=0;
    for(j; j<strlen(str); j++){
        if(j%max_char == 0 && j != 0){i++;}
        ch->chunks[i][j%max_char] = str[j];
    }
    ch->chunk_number = chunk_number;
}

void init_queue(qPtr q, int sizeQ, int word_size){
    int cnt = 0;
    q->rear = -1;
    q->front = -1;
    q->layer_queue = malloc(sizeQ * sizeof(Stack));
    for(cnt; cnt<sizeQ; cnt++){
        q->layer_queue[cnt].layer_stack[0].message_chunk = malloc((word_size + 1)* sizeof(char));
    }
}

void init_stack(sPtr s){
    s->top = -1;
    s->hop = 0;
}

void strip(char* source, char* dest){
    int cnt = 0;
    if(source[0] == '#' && source[strlen(source)-1] == '#'){
        cnt = 1;
        while(cnt<strlen(source)){
            dest[cnt-1] = source[cnt];
            cnt++;
        }
        dest[cnt-2] = '\0';

    }
    else{
    if(source[0] == '#'){
        cnt = 1;
        while(cnt<strlen(source)){
            dest[cnt-1] = source[cnt];
            cnt++;
        }
    }
    if(source[strlen(source)-1] == '#'){
        source[strlen(source)-1] = '\0';
        strcat(dest,source);
    }
    }
}

void send(clientPtr sender, clientPtr reciever, clientPtr final_dest, clientPtr all_clients,
                                                                            int chunk_number, char* first_sender){
    int ind_chunk = 0;
    int j_client = 0;
    int ind_client = 0;
    char MAC_rec[11];
    time_t t;
    struct tm* i;
    char timestr[17];
    time(&t);
    i = localtime(&t);
    strftime(timestr, 17, "%Y-%m-%d %I:%M", i);
    layerPtr last_layer;


    for (ind_client = 0; ind_client<sender->num_route ; ind_client++) {                /*FINDING NEXT RECIEVER*/
        if(strcmp(sender->routing_table[ind_client].dest, final_dest->ID)==0){
            break;
        }
    }

    if(ind_client >= sender->num_route || strcmp(sender->routing_table[ind_client].reciever, "-")==0){      /* NO ROUTES, MESSAGE DROPS */
        if(strcmp(sender->routing_table[ind_client].reciever, "-")==0){
            log_event(search_client(all_clients, sender->routing_table[ind_client].dest), 'D', final_dest->ID, first_sender);
        }
        log_event(sender, 'D', final_dest->ID, first_sender);
        printf("Error: Unreachable destination. Packets are dropped after %d hop\n", sender->outgoing.layer_queue[0].hop);
        return;
    }

    else if(strcmp(sender->routing_table[ind_client].dest, sender->routing_table[ind_client].reciever)==0 &&
            strcmp(sender->routing_table[ind_client].dest, final_dest->ID)==0){         /* MESSAGE RECIEVED AND UNPACKED BY RECIEVER */
        for(ind_chunk = 0; ind_chunk<chunk_number; ind_chunk++) {
            sender->outgoing.layer_queue[ind_chunk].hop++;
            enqueue(&reciever->incoming, dequeue(&sender->outgoing));
        }
        log_event(final_dest, 'U', final_dest->ID, first_sender);
        printf("A message received by client %s from client %s after a total of %d hops.\n", final_dest->ID, first_sender, reciever->incoming.layer_queue[0].hop);
        printf("Message: ");
        for(ind_chunk = 0; ind_chunk<chunk_number; ind_chunk++){
            printf("%s", reciever->incoming.layer_queue[ind_chunk].layer_stack[0].message_chunk);
        }
        return;
    }

    else{                                                                                 /* MESSAGE RECIEVED BUT INTENDED CLIENT IS DIFFERENT, MESSAGE IS FORWARDED*/

        /*#############################################################################*/
        /*#DATA TRANSFER FROM OUTGOING QUEUE OF SENDER TO INCOMING QUEUE OF RECEIVER: #*/
        /*#############################################################################*/
        for(ind_chunk = 0; ind_chunk<chunk_number; ind_chunk++) {
            sender->outgoing.layer_queue[ind_chunk].hop++;
            enqueue(&reciever->incoming, dequeue(&sender->outgoing));
        }

        printf("A message received by client %s, but intended for client %s. Forwarding...\n",
               search_client(all_clients, sender->routing_table[ind_client].reciever)->ID, final_dest->ID);

        for(j_client; j_client<search_client(all_clients, sender->routing_table[ind_client].reciever)->num_route; j_client++){

            if(strcmp(search_client(all_clients, sender->routing_table[ind_client].reciever)->routing_table[j_client].dest, final_dest->ID)==0
                    &&strcmp(search_client(all_clients, sender->routing_table[ind_client].reciever)->routing_table[j_client].reciever, "-")!=0){
                break;
            }
        }

        /*##########################################################################(###############################*/
        /*#MAC ADDRESS RENEWAL & DATA TRANSFER FROM INCOMING QUEUE OF RECIEVER TO OUTGOING QUEUE OF NEW RECEIVER: #*/
        /*#########################################################################################################*/
        if(j_client<search_client(all_clients, sender->routing_table[ind_client].reciever)->num_route){
            strcpy(MAC_rec, search_client(all_clients, search_client(all_clients, sender->routing_table[ind_client].reciever)->
                    routing_table[j_client].reciever)->MAC);
            for(ind_chunk = 0; ind_chunk<chunk_number; ind_chunk++) {
                printf("\tFrame #%d MAC address change: New sender MAC %s, new receiver MAC %s\n",ind_chunk+1,
                       search_client(all_clients, sender->routing_table[ind_client].reciever)->MAC, MAC_rec);
                last_layer = pop(&reciever->incoming.layer_queue[ind_chunk]);
                strcpy(last_layer->MAC_reciever, MAC_rec);
                strcpy(last_layer->MAC_sender, search_client(all_clients, sender->routing_table[ind_client].reciever)->MAC);
                push(&reciever->incoming.layer_queue[ind_chunk], last_layer);
                
                enqueue(&search_client(all_clients, search_client(all_clients, sender->routing_table[ind_client].reciever)->ID)->outgoing, dequeue(&reciever->incoming));
            }
            log_event(search_client(all_clients, sender->routing_table[ind_client].reciever), 'R', final_dest->ID, first_sender);/*LOGGING CURRENT RETRIEVAL*/
            log_event(search_client(all_clients, sender->routing_table[ind_client].reciever), 'F', final_dest->ID, first_sender);/*LOGGING FORWARD OPERATION*/
            send(search_client(all_clients, sender->routing_table[ind_client].reciever),
                 search_client(all_clients, search_client(all_clients, sender->routing_table[ind_client].reciever)->ID),
                 final_dest, all_clients, chunk_number,first_sender);
        }
        else{
            log_event(search_client(all_clients, sender->routing_table[ind_client].reciever), 'R', final_dest->ID, first_sender);
            log_event(search_client(all_clients, sender->routing_table[ind_client].reciever), 'D', final_dest->ID, first_sender);
            printf("Error: Unreachable destination. Packets are dropped after %d hop\n", sender->outgoing.layer_queue[0].hop);
            return;
        }


    }
}

void show(qPtr Q, char* ID, int i){
    if(i != -1){
        printf("\nCurrent Frame #%d on the outgoing queue of client %s\n", i, ID);
        printf("Carried Message: \"%s\"\n" , Q->layer_queue[i-1].layer_stack[0].message_chunk);
        printf("Layer 0 info: Sender ID: %s, Receiver ID: %s\n",Q->layer_queue[i-1].layer_stack[0].ID_sender,
               Q->layer_queue[i-1].layer_stack[0].ID_reciever);
        printf("Layer 1 info: Sender port number: %s, Receiver port number: %s\n",
               Q->layer_queue[i-1].layer_stack[1].port_sender,Q->layer_queue[i-1].layer_stack[1].port_reciever);
        printf("Layer 2 info: Sender IP address: %s, Receiver IP address: %s\n",
               Q->layer_queue[i-1].layer_stack[2].IP_sender, Q->layer_queue[i-1].layer_stack[2].IP_reciever);
        printf("Layer 3 info: Sender MAC address: %s, Receiver MAC address: %s\n",
               Q->layer_queue[i-1].layer_stack[3].MAC_sender, Q->layer_queue[i-1].layer_stack[3].MAC_reciever);
    } else{
    	if(Q->rear - Q->front < 0)
        	printf("Current total number of frames: 0\n");
        else
        	printf("Current total number of frames: %d", Q->rear - Q->front +1);
    }
}

void log_event (clientPtr client, char activity, char* rec_ID, char* send_ID) {
    time_t t;
    struct tm* i;
    char log_time[17];
    time(&t);
    i = localtime(&t);
    strftime(log_time, 17, "%Y-%m-%d %I:%M", i);
    if (client->num_log == 0) {
    } else {
        client->client_log = realloc(client->client_log, (client->num_log + 1) * sizeof(Log));
    }

    strcpy(client->client_log[client->num_log].time_stamp, log_time);
    strcpy(client->client_log[client->num_log].reciever, rec_ID);
    strcpy(client->client_log[client->num_log].sender, send_ID);
    if (activity == 'F') {
            strcpy(client->client_log[client->num_log].success, "Yes");
            strcpy(client->client_log[client->num_log++].activity, "Message Forwarded");}
    else if (activity == 'R') {
        strcpy(client->client_log[client->num_log].success, "Yes");
        strcpy(client->client_log[client->num_log++].activity, "Message Received");
    }
    else if (activity == 'S') {
        strcpy(client->client_log[client->num_log].success, "Yes");
        strcpy(client->client_log[client->num_log++].activity, "Message Sent");
    }
    else if (activity == 'D') {
        strcpy(client->client_log[client->num_log].success, "No");
        strcpy(client->client_log[client->num_log++].activity, "Message Forwarded");
    }
    else if (activity == 'U') {
            strcpy(client->client_log[client->num_log].success, "Yes");
            strcpy(client->client_log[client->num_log++].activity, "Message Unpacked");
    }
}

void show_log(clientPtr client, char* message, int frames){
    int cnt = 0;
    if(client->num_log == 0){
        printf("No logs found for this client\n");
    }
    else{
        for(cnt; cnt<client->num_log; cnt++){
            printf("Log Entry #%d:\n"
                           "Timestamp: %s\n"
                           "Message: %s\n"
                           "Number of frames: %d\n"
                           "Number of hops: %d\n"
                           "Sender ID: %s\n"
                           "Receiver ID: %s\n"
                           "Activity: %s\n"
                           "Success: %s\n", cnt+1, client->client_log[cnt].time_stamp,
                   message, frames, client->num_log, client->client_log[cnt].sender,
                   client->client_log[cnt].reciever, client->client_log[cnt].activity,client->client_log[cnt].success);
            printf("----------\n");
        }
    }
}
