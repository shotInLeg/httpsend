#ifndef AHTTPPARSE_H
#define AHTTPPARSE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <cstring>
#include <cstdlib>
#include <cstdio>

std::string getProtocol( const std::string& url )
{
    std::string protocol = "";
    char last_symbol = '\0';
    for( int i = 0; i < (int)url.size(); i++ )
    {
        protocol += url[i];

        if( (last_symbol == '\\' && url[i] == '\\') || (last_symbol == '/' && url[i] == '/') )
            break;

        last_symbol = url[i];
    }

    //std::cout << "protocol -> " << protocol << std::endl;

    return protocol;
}

std::string getHost( const std::string& url )
{
    std::string protocol = getProtocol(url);
    std::string url_copy = url;
    url_copy = url_copy.replace(url_copy.find_first_of(protocol), protocol.size(), "");

    //std::cout << "host -> " << url_copy << std::endl;

    std::string host = "";
    for(int i = 0; i < (int)url_copy.size(); i++)
    {
        if( url_copy[i] == '\\' || url_copy[i] == '/' )
            break;

        host += url_copy[i];
    }

    return host;
}

std::string getAction( const std::string& url )
{
    std::string proto_host = getProtocol(url) + getHost(url);
    std::string url_copy = url;
    url_copy = url_copy.replace(url_copy.find_first_of(proto_host), proto_host.size(), "");

    std::string url_path = "";
    for(int i = 0; i < (int)url_copy.size(); i++)
    {
        if( url_copy[i] == '?' )
            break;

        url_path += url_copy[i];
    }

    return url_path;
}

#endif // AHTTPPARSE_H
