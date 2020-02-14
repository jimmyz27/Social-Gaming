#include "include/GameSession.h"
#include "GameSessionManager.h"
#include "Game.h"
#include <nlohmann/json.hpp>

GameSession::GameSession(User& owner) : 
    _invitationCode (Invitation::createNewInvitation()), 
    _owner (owner),
    _isGameStarted(false)
    // game(nlohmann::json gameSettings) 
{}

Invitation GameSession::getInvitationCode() const {
    return _invitationCode;
}

size_t GameSession::getTotalPlayerCount() const noexcept {
    return _usersInSession.size();
}

std::string GameSession::getSessionName() const {
    return _sessionName;
}

bool GameSession::doesUserOwnGame(const User& user) const {
    return (user == _owner);
}

bool GameSession::isGameStarted() const {
    return _isGameStarted;
}

void GameSession::addUserToSession(User& user) { 
    // TODO: UserList may need to be revised in order to accomodate this operation better.
    _usersInSession.addUser(user.getUserId());
}

void GameSession::removeUserFromSession(User& user) { 
    // TODO: UserList may need to be revised in order to accomodate this operation better.
    _usersInSession.removeUser(user.getUserId());
}

void GameSession::removeAllUsersfromSession(){ 
    _usersInSession.removeAllUsers();
}

void GameSession::setConfigurationSettings(std::string jsonSettings) { 
    _JSONSetting = jsonSettings;
}

void GameSession::startGame() {
    _isGameStarted = true;
}

UserList GameSession::getUsersInSession() {
    return _usersInSession;
}
