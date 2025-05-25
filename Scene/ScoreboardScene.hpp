#ifndef SCOREBOARD_SCENE_HPP
#define SCOREBOARD_SCENE_HPP

#include "Engine/IScene.hpp"
#include <allegro5/allegro_audio.h>

struct PlayerData{
    std::string name;
    int score=0;
    Engine::IObject* rankLabel=nullptr,* nameLabel=nullptr,* scoreLabel=nullptr;
};


class ScoreboardScene : public Engine::IScene {
public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void Terminate() override;
    void ToPage(int page);
    void BackOnClick(int stage);
    void PrevOnClick();
    void NextOnClick();

private:
    int w=Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h=Engine::GameEngine::GetInstance().GetScreenSize().y;

    int page;
    std::vector<PlayerData> playerDataList;
    void removePlayerLabels();
};

#endif // SCOREBOARD_SCENE_HPP