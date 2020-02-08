#include "include/GameSessionManager.h"

GameSession GameSessionManager::createGameSession(User& owner){
    GameSession gameSession{owner};
    _sessionsList.insert(gameSession);
    _inviteCodes.insert(gameSession.getInvitationCode());
    _invitationToGameSessionMap.insert(std::make_pair(gameSession.getInvitationCode(), gameSession));
    return gameSession;
}

std::optional<GameSession> GameSessionManager::joinGameSession(User& user, const Invitation& invitation){
    auto gameSession = findGameSession(invitation);
    if (gameSession.has_value()){
        gameSession.value().addUserToSession(user);
    }
    return gameSession;
}

std::optional<GameSession> GameSessionManager::findGameSession(const Invitation& invitation){
    auto found = _invitationToGameSessionMap.find(invitation);
    if (found == _invitationToGameSessionMap.end()){
        return std::nullopt;
    }
    auto gameSession = found->second;
    return std::optional<GameSession>{gameSession};
}

void GameSessionManager::endGameSession(GameSession& gameSession){
    auto invitationCode = gameSession.getInvitationCode();
    if (sessionExists(invitationCode)){
        gameSession.removeAllUsersfromSession();
        _invitationToGameSessionMap.erase(invitationCode);
        _inviteCodes.erase(invitationCode);
        _sessionsList.erase(gameSession);
    }
}

size_t GameSessionManager::totalSessionCount(){
    return _sessionsList.size();
}

std::optional<Invitation> GameSessionManager::sessionExists(const std::string& invitationString){
    return sessionExists(Invitation{invitationString});
}

std::optional<Invitation> GameSessionManager::sessionExists(const Invitation& invitation) {
    auto found = _invitationToGameSessionMap.find(invitation);
    if (found == _invitationToGameSessionMap.end()){
        return std::nullopt;
    }
    return std::optional<Invitation>{found->first};
}