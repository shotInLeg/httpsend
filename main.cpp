#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "ANetworkTransfer/AHttp/ahttptransfer.h"

using namespace std;

map< string, vector<string> > parseParams( int argc, char * argv[] )
{
    map< string, vector<string> > params;

    string key = "";
    vector< string > prm;

    for( int i = 1; i < argc; i++ )
    {
        if( argv[i][0] == '-' )
        {
            if( key != "" )
            {
                params[key] = prm;
                prm.clear();
            }

            key = argv[i];
        }
        else if( argv[i] )
        {
            prm.push_back( argv[i] );
        }

        if( i == argc-1 )
            if( key != "" )
                params[key] = prm;
    }

    return params;
}

void printParams( std::map< std::string, std::vector<std::string> >& params )
{
    std::map< std::string, std::vector<std::string> >::iterator itr = params.begin();
    for(; itr != params.end(); itr++ )
    {
        std::cout << itr->first << "\n    ";
        for( unsigned i = 0; i < itr->second.size(); i++ )
        {
            std::cout << itr->second[i] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::string> split( const std::string& str, char a )
{
    std::vector<std::string> string_arr;

    std::string tmp = "";
    for( unsigned i = 0; i < str.size(); i++ )
    {
        if( str[i] != a )
            tmp += str[i];
        else
        {
            string_arr.push_back( tmp );
            tmp.clear();
        }

        if( i == str.size()-1 )
            string_arr.push_back( tmp );
    }

    return string_arr;
}


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, 0);
    cout << "Welcome to HTTP Send v0.1" << endl;

    map< string, vector<string> > params = parseParams( argc, argv );

    SimpleRequest request;

    request.protocol = getProtocol( params["-get"][0] );
    request.host = getHost( params["-get"][0] );
    request.action = getAction( params["-get"][0] );
    request.addHeader( "Referer", "www.vk.com" );
    request.addHeader( "User-Agent", "Google Chrome");
    request.addCookie( "CSRF-Token", "sdfsdfsdfsdf" );
    request.addCookie( "uid", "1256155135");

    for( unsigned i = 0; i < params["-sd"].size(); i++ )
    {
        request.addData( split( params["-sd"][i], '=' )[0],  split( params["-sd"][i], '=' )[1] );
    }

    cout << request.getRequest() << endl;

    cout << "ANSWER: ->>>" << endl;

    cout << GET( request ) << endl;

    return 0;
}
