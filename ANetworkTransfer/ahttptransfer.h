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

struct SimpleData
{
    std::string name;
    std::string value;

    SimpleData(){ name = ""; value = ""; }

    SimpleData( const std::string& _name, const std::string _value )
    {
        name = _name;
        value = _value;
    }
};

struct Header
{
    std::string name;
    std::string value;

    Header(){ name = ""; value = ""; }

    Header( const std::string& _name, const std::string _value )
    {
        name = _name;
        value = _value;
    }
};

struct Cookie
{
    std::string name;
    std::string value;

    Cookie(){ name = ""; value = ""; }

    Cookie( const std::string& _name, const std::string _value )
    {
        name = _name;
        value = _value;
    }
};

struct Response
{
    std::vector< Header > headers;
    std::vector< Cookie > cookies;
    std::string data;

    Response()
    {
        data = "";
    }

    Response( const std::vector< Header >& _headers, const std::vector< Cookie > _cookies, const std::string& _data )
    {
        headers = _headers;
        cookies = _cookies;
        data = _data;
    }

    void addHeader( const std::string& _name, const std::string& _value )
    {
        headers.push_back( Header( _name, _value ) );
    }

    void addCookie( const std::string& _name, const std::string& _value )
    {
        cookies.push_back( Cookie( _name, _value ) );
    }
};

struct SimpleRequest
{
    std::string protocol;
    std::string host;
    std::string action;
    std::vector< Header > headers;
    std::vector< Cookie > cookies;
    std::string data;

    SimpleRequest()
    {
        data = "";
    }

    SimpleRequest( const std::string& _protocol, const std::string& _host, const std::string& _action, const std::vector< Header >& _headers, const std::vector< Cookie > _cookies, const std::string& _data )
    {
        protocol = _protocol;
        host = _host;
        action = _action;
        headers = _headers;
        cookies = _cookies;
        data = _data;
    }

    void addHeader( const std::string& _name, const std::string& _value )
    {
        headers.push_back( Header( _name, _value ) );
    }

    void addHeader( std::map< std::string, std::string >& _headers )
    {
        std::map< std::string, std::string >::iterator itr = _headers.begin();
        for( ; itr != _headers.end(); itr++ )
        {
            headers.push_back( Header( itr->first, itr->second ) );
        }
    }

    void addCookie( const std::string& _name, const std::string& _value )
    {
        cookies.push_back( Cookie( _name, _value ) );
    }

    void addCookie( std::map< std::string, std::string >& _cookies )
    {
        std::map< std::string, std::string >::iterator itr = _cookies.begin();
        for( ; itr != _cookies.end(); itr++ )
        {
            cookies.push_back( Cookie( itr->first, itr->second ) );
        }
    }

    void addData( const std::string& _name, const std::string& _value )
    {
        if( data.empty() )
            data += _name + std::string("=") + _value;
        else
            data += std::string("&") + _name + std::string("=") + _value;
    }

    void addData( std::map< std::string, std::string >& _data )
    {
        std::map< std::string, std::string >::iterator itr = _data.begin();
        for( ; itr != _data.end(); itr++ )
        {
            if( data.empty() )
                data += itr->first + std::string("=") + itr->second;
            else
                data += std::string("&") + itr->first + std::string("=") + itr->second;
        }
    }

    std::string getRequest()
    {
        std::string request = "";

        request += std::string("GET ") + protocol + host + action;
        if(data.size() > 0)
            request += std::string("?") + data;
        request += " HTTP/1.0\r\n";

        request += std::string("Host: ") + host + std::string("\r\n");

        for( unsigned i = 0; i < headers.size(); i++ )
        {
            request += headers[i].name + std::string(": ") + headers[i].value + std::string("\r\n");
        }

        request += std::string("Cookie: income=1;");
        for( unsigned i = 0; i < cookies.size(); i++ )
        {
            request += cookies[i].name + std::string("=") + cookies[i].value + std::string(";");
        }
        request += std::string("\r\n");

        request += std::string("\r\n");

        return request;
    }
};




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
