// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;


Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mlsRunning(true)
{

}


bool Game::Initialize() {
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0) {
        SDL_Log("SDLを初期化できません:%s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow(
        "Game Programming in C++(第1章)",   // ウィンドウのタイトル
        100,    // ウィンドウ左上隅のx座標
        100,    // ウィンドウ左上隅のy座標
        1024,   // ウィンドウの幅(width)
        768,    // ウィンドウの高さ(height)
        0       // フラグ (設定しない時は0)
    );
    if (!mWindow) {
        SDL_Log("ウィンドウの作成に失敗しました:%s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(
        mWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC        
    );
    if (!mRenderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }


    return true;
}


void Game::Shutdown() {
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}


void Game::ProcessInput() {
    SDL_Event event;

    // キューにイベントがあれば繰り返す
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            mlsRunning = false;
            break;
        }        
    }

    // キーボードの状態を取得する
    const Uint8* state = SDL_GetKeyboardState(NULL);
    // [ESC]キーが押されていてもループを終える
    if (state[SDL_SCANCODE_ESCAPE]) {
        mlsRunning = false;
    }
}


void Game::UpdateGame() {

}


void Game::GenerateOutput() {
    // Set draw color to blue
    SDL_SetRenderDrawColor(
        mRenderer,
        0,      // R
        0,      // G
        255,    // B
        255     // A
    );

    // Clear back buffer
    SDL_RenderClear(mRenderer);

    // Draw walls
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    // Draw top wall
    SDL_Rect wall {
        0,          // 左上隅のx
        0,          // 左上隅のy
        1024,       // 幅
        thickness   // 高さ
    };
    SDL_RenderFillRect(mRenderer, &wall);

    // Swqp front buffer and back buffer
    SDL_RenderPresent(mRenderer);
}


void Game::RunLoop() {
    while (mlsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}