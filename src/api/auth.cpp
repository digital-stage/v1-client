#import "auth.h"
#import <QDebug>

/*
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/http_msg.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::json;                  // JSON library*/

Auth::Auth(const std::string& url) {
    this->url = url;
}

Auth::~Auth() {

}

std::string Auth::signIn(const std::string& email, const std::string& password) {
    qDebug() << "Try siging in";

    /*http_client client1(utility::conversions::to_string_t("https://auth.dstage.org"));
    web::http::http_request request;
    http_request request;
    request.set_method(methods::POST);
    request.headers().add(U("Host"), U("auth.dstage.org"));*/

    if( email == "bla" && password == "bla" ) {
        qDebug() << "Sucessfully signed in";
        return "mytoken";
    }
    qDebug() << "Failed to sign in";
    return "";
}

bool Auth::verifyToken(const std::string& token) {
    if( token == "mytoken" ) {
        qDebug() << "Token is valid";
        return true;
    }
    qDebug() << "Invalid token";
    return false;
}

bool Auth::signOut() {
    qDebug() << "Signing out";
    return true;
}

