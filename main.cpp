#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>




using namespace std;

int main(){
    //create server
    int listening = socket(AF_INET,SOCK_STREAM, 0);
    if(listening == -1 ){
        cerr<< "cant create sock" ;
        return -1;
    }

    //bind the socket to ip/tcp
    sockaddr_in hint ;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);

    inet_pton(listening , "0.0.0.0" , &hint.sin_addr);//internet command pointer to a sintring to a number// converts the string


    if(bind(listening, (sockaddr*)&hint , sizeof(hint))== -1){
        cerr<< "cant bind to address" ;
        return -2;
    }



    // MARK THE socket for listening

    if(listen(listening , SOMAXCONN)){
        cerr<< "cant listen" ;
        return -3;
    }

    // accept call

    sockaddr_in client ;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];
//  https://pubs.opengroup.org/onlinepubs/009604499/functions/accept.html
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    if(clientSocket == -1){
        cerr<< "proble with client connn";
        return -4;
    }



    memset(host, 0 , NI_MAXHOST);
    memset(svc, 0 , NI_MAXSERV);


    int result = getnameinfo((sockaddr*)&client , sizeof(client) , host , NI_MAXHOST ,svc , NI_MAXSERV, 0 );




    if(result ==0 ){
        cout << host << "connected on " << svc << "\n";

    }else{

        inet_ntop(AF_INET, &client.sin_addr , host , NI_MAXHOST);
        cout << host << "connected on " << ntohs(client.sin_port) <<"\n";

    }

    // clos the listening socket


    close(listening);


    // while receving the socket display a messaqge

    char buf[4096];
    while(true ){
        //clear buffer
        memset(buf , 0 , 4096);
        //wait for message

        int bytesRecv = recv(clientSocket , buf , 4096 , 0 );
        if(bytesRecv == -1){
            cerr << "issue \n";
            break;
        }
        if(bytesRecv == 0 ){
            cout << "the client dissconnected \n";

            break;
        }



        //display message
        cout<< "receved " << string(buf ,0 , bytesRecv) << endl;

        //resend for messageeee
        // https://pubs.opengroup.org/onlinepubs/009695399/functions/send.html
        send(clientSocket , buf , bytesRecv +1 , 0);

        }

    //close socket
    close(clientSocket);

    return 0;
}
