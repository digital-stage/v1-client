#ifndef AUTH_H
#define AUTH_H

#import <string>
#include "curl/curl.h"

using namespace std;

class Auth
{
public:
    Auth(string *url);
    ~Auth();

    bool verifyToken(string *token);
    string signIn(string *email, string *password);
    bool signOut();



protected:
private:
    CURL *curl;
    string *url;
};

#endif // AUTH_H
