#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "PlayScene.hpp"
#include "Scene/ScoreboardScene.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"

using namespace std;

void ScoreboardScene::Initialize() {
    w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    AddNewObject(new Engine::Label("Scoreboard", "pirulen.ttf", 64, w/2, 50, 255, 255, 255, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("Rank", "pirulen.ttf", 44, w/2 - 380, 130, 255, 255, 255, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("Player", "pirulen.ttf", 44, w/2, 130, 255, 255, 255, 255, 0.5, 0.5));
    AddNewObject(new Engine::Label("Score", "pirulen.ttf", 44, w/2 + 340, 130, 255, 255, 255, 255, 0.5, 0.5));

    Engine::ImageButton *btn;

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w/2 - 200, 700, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 1));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, w/2, 750, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w/2 - 700, 700, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::PrevOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev Page", "pirulen.ttf", 36, w/2 - 700 + 200, 750, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("stage-select/dirt.png", "stage-select/floor.png", w/2 + 300, 700, 400, 100);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::NextOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next Page", "pirulen.ttf", 36, w/2 + 300 + 200, 750, 0, 0, 0, 255, 0.5, 0.5));

    PlayerData tmpData;
    ifstream scoreFile("../Resource/scoreboard.txt");
    while (scoreFile >> tmpData.name >> tmpData.score) {
        playerDataList.push_back(tmpData);
    }
    sort(playerDataList.begin(), playerDataList.end(), [](const PlayerData& a, const PlayerData& b) {
        return a.score > b.score;
    });

    ToPage(1);
}

void ScoreboardScene::Terminate() {
    playerDataList.clear();
    IScene::Terminate();
}

void ScoreboardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void ScoreboardScene::PrevOnClick() {
    if (page > 1) {
        removePlayerLabels();
        ToPage(page - 1);
    }
}
void ScoreboardScene::NextOnClick() {
    if (page < (playerDataList.size() / 10) + 1) {
        removePlayerLabels();
        ToPage(page + 1);
    }
}

void ScoreboardScene::removePlayerLabels() {
    for(size_t i = 0 ; i < min<size_t>(playerDataList.size()-10*(page-1), 10); ++i) {
        RemoveObject(playerDataList[i+10*(page-1)].rankLabel->GetObjectIterator());
        RemoveObject(playerDataList[i+10*(page-1)].nameLabel->GetObjectIterator());
        RemoveObject(playerDataList[i+10*(page-1)].scoreLabel->GetObjectIterator());
        playerDataList[i+10*(page-1)].rankLabel = nullptr;
        playerDataList[i+10*(page-1)].nameLabel = nullptr;
        playerDataList[i+10*(page-1)].scoreLabel = nullptr;
    }
}

void ScoreboardScene::ToPage(int page) {
    this->page = page;
    for (size_t i = 0 ; i < min<size_t>(playerDataList.size()-10*(page-1), 10); ++i) {
        playerDataList[i+10*(page-1)].rankLabel = new Engine::Label(to_string(i+1+10*(page-1)), "pirulen.ttf", 36, w/2-400, 200 + i * 48, 255, 255, 255, 255, 0, 0.5);
        playerDataList[i+10*(page-1)].nameLabel = new Engine::Label(playerDataList[i+10*(page-1)].name, "pirulen.ttf", 36, w/2-250, 200 + i * 48, 255, 255, 255, 255, 0, 0.5);
        playerDataList[i+10*(page-1)].scoreLabel = new Engine::Label(to_string(playerDataList[i+10*(page-1)].score), "pirulen.ttf", 36, w/2+300, 200 + i * 48, 255, 255, 255, 255, 0, 0.5);
        AddNewObject(playerDataList[i+10*(page-1)].rankLabel);
        AddNewObject(playerDataList[i+10*(page-1)].nameLabel);
        AddNewObject(playerDataList[i+10*(page-1)].scoreLabel);
    }
}
