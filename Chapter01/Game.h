// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <SDL.h>

class Game
{
public:
    Game();

    // ゲームを初期化する
    bool Initialize();
    // ゲームオーバーまでゲームループを実行する
    void RunLoop();
    // ゲームをシャットダウンする
    void Shutdown();

private:
    // ゲームループのためのヘルパー関数
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // SDLが作るウィンドウ
    SDL_Window* mWindow;
    // ゲームの続行を指示する
    bool mlsRunning;
    SDL_Renderer* mRenderer;
};