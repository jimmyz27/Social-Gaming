#include "GameSession.h"

GameSession::GameSession(std::weak_ptr<User>& owner) : 
    messages({}),
    invitationCode (Invitation::createNewInvitation()),
    // TODO: Replace blank GameSpec/GameState params with actual impl
    game{GameSpecification::GameSpec(), GameState{}},
    owner (owner)
{}

Invitation GameSession::getInvitationCode() const {
    return invitationCode;
}

//TODO: Check for owner, sessionList.IsOwner currently crashes program.
bool GameSession::isOwner(const UserId& user) const {
    return (user == owner.lock()->getUserId());
}

bool GameSession::isGameStarted() const {
    return game.isGameStarted();
}

void GameSession::startGame() {
    game.startGame();
}

void GameSession::endGame() {
    game.endGame();
}

void GameSession::addMessages(const std::string &message) noexcept{
    messages.push_back(message);
}

std::list<std::pair<UserId, std::string>> GameSession::getLobbyMessages() noexcept{
    std::list<std::pair<UserId, std::string>> result = {};

    for(auto& message : messages){
        
       for(auto& player : playerList.users){
            result.push_back(std::make_pair(player.lock()->getUserId(), message));
        }
    }    

    return result;
}

void GameSession::clearMessages() noexcept {
    messages = {};
}

std::list<std::pair<UserId, std::string>> GameSession::updateAndGetAllMessages() noexcept{

    auto messages = getLobbyMessages();

    //TODO: if game started, do something else

    clearMessages();

    return messages;
}

void GameSession::addPlayer(const std::weak_ptr<User>& player) noexcept{
    playerList.users.push_back(player);
}

void GameSession::removePlayer(const std::weak_ptr<User>& player) noexcept{
    
    auto playerIterator = std::find_if(playerList.users.begin(), playerList.users.end(), [player](std::weak_ptr<User> listMember){
        return player.lock()->getUserId() == listMember.lock()->getUserId();
    });
    
    if(playerIterator != playerList.users.end()){
        playerList.users.erase(playerIterator);
    }
}
