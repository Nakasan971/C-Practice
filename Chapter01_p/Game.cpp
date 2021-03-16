// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mIsRunning(true)
,mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mPaddleDir(0)
,mBall(2)
{
    
}
bool Game::Initialize(){
    //SDL初期化
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if(sdlResult != 0){
        SDL_Log("SDLを初期化できません:%s",SDL_GetError());
        return false;
    }
    //ウィンドウ生成
    mWindow = SDL_CreateWindow("Chapter1_Pra", 100, 100, 1024, 768, 0);
    if(!mWindow){
        SDL_Log("ウィンドウの作成に失敗しました:%s",SDL_GetError());
        return false;
    }
    //レンダラー作成
    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!mRenderer){
        SDL_Log("レンダラーの作成に失敗しました：%s",SDL_GetError());
        return false;
    }
    //オブジェクト座標初期化
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f/2.0f;
    twoPaddlePos.x = (1024.0f - thickness) - 10.0f;
    twoPaddlePos.y = 768.0f/2.0f;
    //１つ目のボール
    mBall[0].mBallPos.x = 1024.0f/2.0f;
    mBall[0].mBallPos.y = 768.0f/2.0f;
    mBall[0].mBallVel.x = -200.0f;
    mBall[0].mBallVel.y = 235.0f;
    //２つ目のボール
    mBall[1].mBallPos.x = 1024.0f/2.0f;
    mBall[1].mBallPos.y = 768.0f/2.0f;
    mBall[1].mBallVel.x = 200.0f;
    mBall[1].mBallVel.y = 235.0f;
    return true;
}

void Game::GameLoop(){
    while(mIsRunning){
        ProcessInit();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInit(){
    //[ウィンドウを閉じる]で終了
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }
    //[esc]で停止
    const Uint8* state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunning = false;
    }
    //パドルの移動入力
    mPaddleDir = 0;
    twoPaddleDir = 0;
    if(state[SDL_SCANCODE_W]){
        mPaddleDir -= 1;
    }
    if(state[SDL_SCANCODE_S]){
        mPaddleDir += 1;
    }
    if(state[SDL_SCANCODE_I]){
        twoPaddleDir -= 1;
    }
    if(state[SDL_SCANCODE_K]){
        twoPaddleDir += 1;
    }
}

void Game::UpdateGame(){
    //前フレームから16ms待つ
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount+16));
    //前フレームとの時刻差（秒）
    float deltaTime = (SDL_GetTicks()-mTicksCount)/1000.0f;
    if(deltaTime > 0.05f){
        deltaTime = 0.05f;
    }
    //時刻更新
    mTicksCount = SDL_GetTicks();
    //パドルの移動
    if(mPaddleDir != 0){
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        if(mPaddlePos.y < (paddleH/2.0f + thickness)){
            mPaddlePos.y = paddleH/2.0f + thickness;
        }
        else if (mPaddlePos.y > (768.0f - paddleH/2.0f - thickness)){
            mPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
        }
    }
    //パドル2の移動
    if(twoPaddleDir != 0){
        twoPaddlePos.y += twoPaddleDir * 300.0f * deltaTime;
        if(twoPaddlePos.y < (paddleH/2.0f + thickness)){
            twoPaddlePos.y = paddleH/2.0f + thickness;
        }
        else if (twoPaddlePos.y > (768.0f - paddleH/2.0f - thickness)){
            twoPaddlePos.y = 768.0f - paddleH/2.0f - thickness;
        }
    }
    for(int i = 0;i < mBall.size();i++){
        //ボールの移動
        mBall[i].mBallPos.x += mBall[i].mBallVel.x * deltaTime;
        mBall[i].mBallPos.y += mBall[i].mBallVel.y * deltaTime;
        //パドルで跳ね返る
        float diff = mPaddlePos.y - mBall[i].mBallPos.y;
        float twodiff = twoPaddlePos.y - mBall[i].mBallPos.y;
        diff = (diff > 0.0f) ? diff : -diff;
        twodiff = (twodiff > 0.0f)? twodiff : -twodiff;
        if (diff <= paddleH / 2.0f &&
            mBall[i].mBallPos.x <= 25.0f &&
            mBall[i].mBallPos.x >= 20.0f &&
            mBall[i].mBallVel.x < 0.0f){
            mBall[i].mBallVel.x *= -1.0f;
        }
        //パドル2で跳ね返る
        else if (twodiff <= paddleH / 2.0f &&
                 mBall[i].mBallPos.x >= 999.0f &&
                 mBall[i].mBallPos.x <= 1004.0f &&
                 mBall[i].mBallVel.x > 0.0f){
            mBall[i].mBallVel.x *= -1.0f;
        }
        //ボールが端まで行くと
        else if (mBall[i].mBallPos.x <= 0.0f ||
                 1024.0f <= mBall[i].mBallPos.x){
            mIsRunning = false;
        }
        /*右壁で跳ね返る
         else if (mBallPos.x >= (1024.0f - thickness) &&
                mBallVel.x > 0.0f){
         mBallVel.x *= -1.0f;
         }*/
        //天井で跳ね返る
        if (mBall[i].mBallPos.y <= thickness &&
            mBall[i].mBallVel.y < 0.0f){
            mBall[i].mBallVel.y *= -1;
        }
        //床で跳ね返る
        else if (mBall[i].mBallPos.y >= (768 - thickness) &&
                 mBall[i].mBallVel.y > 0.0f){
            mBall[i].mBallVel.y *= -1;
        }
    }
}

void Game::GenerateOutput(){
    //青背景にセット
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    //バックバッファをクリア
    SDL_RenderClear(mRenderer);
    //色設定（白）
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_Rect wall{
        0,          //x
        0,          //y
        1024,       //幅
        thickness   //高さ
    };
    //天井描画
    SDL_RenderFillRect(mRenderer, &wall);
    //床描画
    wall.y = 768-thickness;
    SDL_RenderFillRect(mRenderer, &wall);
    /*右壁描画
    wall.x = 1024-thickness;
    wall.y = 0;
    wall.w = thickness;
    wall.h = 1024;
    SDL_RenderFillRect(mRenderer, &wall);*/
    //ボール描画
    SDL_Rect ball{
        static_cast<int>(mBall[0].mBallPos.x - thickness/2),
        static_cast<int>(mBall[0].mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    //複数個描画
    for(int i = 0;i < mBall.size();i++){
        ball.x = static_cast<int>(mBall[i].mBallPos.x - thickness/2);
        ball.y = static_cast<int>(mBall[i].mBallPos.y - thickness/2);
        SDL_RenderFillRect(mRenderer, &ball);
    }
    //パドル描画
    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y - paddleH/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRenderer, &paddle);
    //パドル2描画
    paddle.x = static_cast<int>(twoPaddlePos.x);
    paddle.y = static_cast<int>(twoPaddlePos.y - paddleH/2);
    SDL_RenderFillRect(mRenderer, &paddle);
    //バッファの交換（フロント＜＝＞バック）
    SDL_RenderPresent(mRenderer);
}

void Game::Shutdown(){
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}
