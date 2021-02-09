#import "auth.h"

Auth::Auth(string *url) {
    this->url = url;
    this->curl = curl_easy_init();
}

Auth::~Auth() {

}

string Auth::signIn(string *email, string *password) {
    return "mytoken";
}

bool Auth::verifyToken(string *token) {
    return token->compare("mytoken") == 0;
}

bool Auth::signOut() {
    return true;
}

