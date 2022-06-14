// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

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


    return true;
}

void Game::Shutdown() {
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}