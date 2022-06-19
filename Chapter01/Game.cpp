// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const int paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mlsRunning(true)
,mPaddleDir(0)
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

    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f / 2.0f;
    mBallPos.x = 1024.0f / 2.0f;
    mBallPos.y = 768.0f / 2.0f;

    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;

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

    // Update paddle direction baseed on W/S keys
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W]) {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        mPaddleDir += 1;
    }
}


void Game::UpdateGame() {
    // 前のフレームから16msが経過するまで待つ
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // daltatimeは前のフレームとの時刻の差を秒に変換した値
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

    // デルタタイムを最大値で制限する
    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    // 時刻を更新(次のフレームのために)
    mTicksCount = SDL_GetTicks();

    // Update paddle position based on direction
    if (mPaddleDir != 0) {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        if (mPaddlePos.y < (paddleH/2.0f + thickness)) {
            mPaddlePos.y = paddleH/2.0f + thickness;
        } else if (mPaddlePos.y > (768.0f - paddleH/2.0f - thickness)) {
            mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
        }
    }

    // Update ball position based on ball velocity
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    // Did the ball collide with the top wall?
    if (mBallPos.y <= thickness) {
        mBallVel.y *= -1.0f;
    }

    // Did the ball collide with the bottom wall?
    if (mBallPos.y >= (768 - thickness)) {
        mBallVel.y *= -1.0f;
    }

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

    // Draw bottom wall
    wall.y = 768 - thickness;
    SDL_RenderFillRect(mRenderer, &wall);

    // Draw right wall
    wall.x = 1024 - thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(mRenderer, &wall);

    // Draw paddle
    SDL_Rect paddle {
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - paddleH/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRenderer, &paddle);

    // Draw ball
    SDL_Rect ball {
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    SDL_RenderFillRect(mRenderer, &ball);

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