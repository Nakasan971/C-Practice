// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SDL/SDL.h"
#include <vector>

struct Vector2{
    float x;
    float y;
};

struct Ball{
    Vector2 mBallPos;       //ボールのポジション
    Vector2 mBallVel;       //ボールの移動
};

class Game
{
public:
    Game();
    bool Initialize();  //ゲームの初期化
    void GameLoop();    //ゲーム中（ループ）
    void Shutdown();    //ゲーム終了
private:
    void ProcessInit();     //入力受付
    void UpdateGame();      //ゲーム更新
    void GenerateOutput();  //出力生成
    bool mIsRunning;        //ゲーム続行の指示
    SDL_Window* mWindow;    //ウィンドウ
    SDL_Renderer* mRenderer;//レンダラー
    Uint32 mTicksCount;     //フレームの時間差保持
    int mPaddleDir;         //パドルの方向
    int twoPaddleDir;       //パドル2の方向
    Vector2 mPaddlePos;     //パドルのポジション
    Vector2 twoPaddlePos;   //パドル2のポジション
    std::vector<Ball> mBall;//Ball型動的配列
};
