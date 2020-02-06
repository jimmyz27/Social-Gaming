#include "GameSession.h"
#include "Invitation.h"
#include <unordered_map>
#include <unordered_set>
#include <optional>

namespace GameSessionManager{
    
    namespace {
        struct InvitationHash {
            std::size_t operator()(const Invitation& invitation) const {
                std::hash<std::string> myHash;
                return myHash(invitation.toString());
            }
        };

        static std::list<GameSession> _sessionsList;
        static std::unordered_set<Invitation, InvitationHash> _inviteCodes;
        static std::unordered_map<Invitation, GameSession, InvitationHash> _invitationToGameSessionMap; 
    }

    GameSession createGameSession(int ownerId);
    std::optional<GameSession> joinGameSession(int playerId, const Invitation& invitation);
    void endGameSession(const GameSession& gameSession);
    size_t totalSessionCount();
    std::optional<Invitation> sessionExists(const Invitation& invitation);

    GameSession createGameSession(int ownerId){
        GameSession gameSession{ownerId};
        _sessionsList.push_back(gameSession);
        _inviteCodes.insert(gameSession.getInvitationCode());
        _invitationToGameSessionMap.insert(std::make_pair(gameSession.getInvitationCode().toString(), gameSession));
        return gameSession;
    }

    std::optional<GameSession> joinGameSession(int playerId, const Invitation& invitation){
        auto found = _invitationToGameSessionMap.find(invitation);
        if (found == _invitationToGameSessionMap.end()){
            return std::nullopt;
        }
        auto gameSession = found->second;
        gameSession.addUserToSession(playerId);
        return std::optional<GameSession>{gameSession};
    }

    void endGameSession(const GameSession& gameSession){
        auto invitationCode = gameSession.getInvitationCode();
        if (sessionExists(invitationCode)){
            _invitationToGameSessionMap.erase(invitationCode);
            _inviteCodes.erase(gameSession.getInvitationCode());
            _sessionsList.remove(gameSession);
        }
    }

    size_t totalSessionCount(){
        return _sessionsList.size();
    }

    std::optional<Invitation> sessionExists(const Invitation& invitation) {
        auto found = _invitationToGameSessionMap.find(invitation);
        if (found == _invitationToGameSessionMap.end()){
            return std::nullopt;
        }
        return std::optional<Invitation>{invitation};
    }
}