#ifndef AUTH_H
#define AUTH_H

#include <string>

int loginSystem();
int authorizeUser(std::string login, std::string password);
std::string funcHash(const std::string& password);

#endif // AUTH_H 