#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <time.h>
#include <string.h>

#define SERVICEADDR 0x7f000001
#define BUFLEN 100000

#define SERVICEPORT 8080
const char SERVICENAME[] = "127.0.0.1";

const char risposta[BUFLEN]={"HTTP/1.1 200 OK\r\n\r\n"};
const char fileHTML[BUFLEN]={"./index.html"};


/*//lettura
if((recvLen=recv (sc, rcvbuf, BUFLEN, 0))<0){
			perror ("errore recv: ");
			close(s);
			exit (2);
		}
        */

int main (int argc,char **args) {
    struct sockaddr_in local, remote;
    unsigned int remotelen;
    int s,socket_client;
    char rcvbuf[BUFLEN];
    int recvLen;
    

    
    FILE *f;

    
	

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror ("Creazione Socket: ");
        exit (1);
    }

    printf("Socket creato\n");

    local.sin_family = AF_INET;
    local.sin_port = htons(SERVICEPORT);
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind (s, (struct sockaddr*)&local, sizeof(local)) < 0) {
        perror ("errore Bind: ");
        close(s);
        exit (2);
    }

    printf ("Bind corretta\n");
	
	listen(s, 4);
	printf("listen ok\n");

    remotelen = sizeof(remote);
    printf("remote ok\n");

    printf("In ascolto...\n");

    char bufferFile[BUFLEN];
    char output[BUFLEN];

	while(1){
		strcpy(bufferFile, "");
        strcpy(output,risposta);

		socket_client = accept(s, (struct sockaddr*)&remote, &remotelen);

		printf("\nClient connesso!\n");
		if((recvLen=recv (socket_client, rcvbuf, BUFLEN, 0))<0){
			perror ("errore recv: ");
			close(s);
			exit (2);
		}
		printf("Pacchetto ricevuto:%s\n", rcvbuf);
		
		
		if(rcvbuf[0]=='G' && rcvbuf[1]=='E' && rcvbuf[2]=='T'){//lettura della get
			f=fopen(fileHTML, "r");//apertura file in lettura
			while(fgets(bufferFile, sizeof(bufferFile), f)!=NULL){
				strcat(output, bufferFile);
			}
			fclose(f);
			
			if(send (socket_client, output, strlen(output), 0)<0){
				perror ("errore send: ");
				close(s);
				exit (2);
			}
			printf("\nInvio risposta: %s\n", output);				
			
		}if(rcvbuf[0]=='P' && rcvbuf[1]=='O'&&rcvbuf[2]=='S'&&rcvbuf[3]=='T'){
            f=fopen(fileHTML, "r");//apertura file in lettura
			while(fgets(bufferFile, sizeof(bufferFile), f)!=NULL){
                if(strstr(bufferFile,"<h3>")!=NULL){
                    char temp[BUFLEN]="<h3>";
                    char temp1[BUFLEN]="Post ricevuta";
                    char temp2[BUFLEN]="</h3>";
                    sprintf(temp,"%s%s%s",temp,temp1,temp2);
                    strcat(output,temp);
                }else{
                    strcat(output, bufferFile);
                }				
			}
			fclose(f);
            if(send (socket_client, output, strlen(output), 0)<0){
				perror ("errore send: ");
				close(s);
				exit (2);
			}
			printf("\nInvio risposta: %s\n", output);
        }
		close (socket_client);
	}
		
		
	close (socket_client);
	close (s); 

	
    return 0;
}