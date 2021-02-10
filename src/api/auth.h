#ifndef AUTH_H
#define AUTH_H

#import <string>

class Auth
{
public:
    Auth(const std::string& url);
    ~Auth();

    bool verifyToken(const std::string& token);
    std::string signIn(const std::string& email, const std::string& password);
    bool signOut();


protected:

private:
    std::string url;
};

#endif // AUTH_H
