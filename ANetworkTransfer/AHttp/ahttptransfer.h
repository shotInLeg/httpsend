#ifndef AHTTPTRANSFER_H
#define AHTTPTRANSFER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <cstring>
#include <cstdlib>
#include <cstdio>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "ahttpdata.h"
#include "ahttpparse.h"


std::string GET( SimpleRequest& request )
{
    std::string header = request.getRequest();

    int sock;
    struct sockaddr_in addr;
    struct hostent* raw_host;
    raw_host = gethostbyname( request.host.c_str() );

    if (raw_host == NULL)
    {
        std::cout << "ERROR, no such host";
        exit( 0 );
    }

    sock = socket( AF_INET, SOCK_STREAM, 0 );

    addr.sin_family = AF_INET;
    addr.sin_port = htons( 80 );

    bcopy( (char*)raw_host->h_addr, (char*)&addr.sin_addr, raw_host->h_length );

    if( connect( sock, (struct sockaddr *)&addr, sizeof(addr) ) < 0)
    {
        std::cerr << "connect error" << std::endl;
        exit( 2 );
    }


    char * message = new char[ header.size() ];
    for( unsigned i = 0; i < header.size(); i++ )
    {
        message[i] = header[i];
    }

    send(sock, message, header.size(), 0);

    char buf[1000000];
    recv(sock, buf, sizeof(buf), 0);

    std::string answer = "";

    int j = 0;
    while( int( buf[j]) != 0 )
    {
        answer += buf[j];
        j++;
    }

    return answer;
}

#endif // AHTTPTRANSFER_H
