#include "DxLib.h"
#include <cmath>

struct Vector2
{
    float x;
    float y;
};

float Length(Vector2 v)
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

Vector2 Normalize(Vector2 v)
{
    float len = Length(v);

    if (len == 0.0f)
    {
        return {0.0f, 0.0f};
    }

    return {v.x / len, v.y / len};
}

struct Ball
{
    Vector2 pos;
    Vector2 velocity;
    float radius;
};

int android_main()
{
    if (DxLib_Init() < 0)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    const int SCREEN_W = 720;
    const int SCREEN_H = 1280;

    SetGraphMode(SCREEN_W,SCREEN_H,32);

    Ball ball;

    ball.pos = {400.0f, 300.0f};
    ball.velocity = {0.0f, 0.0f};
    ball.radius = 40.0f;

    bool dragging = false;

    Vector2 dragStart = {0.0f, 0.0f};
    Vector2 dragNow   = {0.0f, 0.0f};

    while (ProcessMessage() == 0)
    {
        // タッチ座標取得
        int touchNum = GetTouchInputNum();

        if (touchNum > 0)
        {
            int x, y;
            GetTouchInput(0, &x, &y);

            dragNow = {(float)x, (float)y};

            if (!dragging)
            {
                dragging = true;
                dragStart = dragNow;
            }
        }
        else
        {
            if (dragging)
            {
                Vector2 dir;

                // 引っ張った方向の逆へ飛ぶ
                dir.x = dragStart.x - dragNow.x;
                dir.y = dragStart.y - dragNow.y;

                // 距離
                float power = Length(dir);

                // 正規化
                Vector2 normal = Normalize(dir);

                // スピード設定
                ball.velocity.x = normal.x * power * 0.15f;
                ball.velocity.y = normal.y * power * 0.15f;
            }

            dragging = false;
        }


        ball.pos.x += ball.velocity.x;
        ball.pos.y += ball.velocity.y;

        if (ball.pos.x - ball.radius < 0)
        {
            ball.pos.x = ball.radius;
            ball.velocity.x *= -1.0f;
        }

        if (ball.pos.x + ball.radius > SCREEN_W)
        {
            ball.pos.x = SCREEN_W - ball.radius;
            ball.velocity.x *= -1.0f;
        }

        if (ball.pos.y - ball.radius < 0)
        {
            ball.pos.y = ball.radius;
            ball.velocity.y *= -1.0f;
        }

        if (ball.pos.y + ball.radius > SCREEN_H)
        {
            ball.pos.y = SCREEN_H - ball.radius;
            ball.velocity.y *= -1.0f;
        }

        ball.velocity.x *= 0.995f;
        ball.velocity.y *= 0.995f;

        ClearDrawScreen();

        // ボール
        DrawCircle(
                (int)ball.pos.x,
                (int)ball.pos.y,
                (int)ball.radius,
                GetColor(255, 80, 80),
                TRUE
        );

        // ドラッグ線
        if (dragging)
        {
            DrawLine(
                    (int)dragStart.x,
                    (int)dragStart.y,
                    (int)dragNow.x,
                    (int)dragNow.y,
                    GetColor(255, 255, 255)
            );
        }

        DrawString(
                20,
                20,
                "ドラッグして飛ばす",
                GetColor(255, 255, 255)
        );

        ScreenFlip();
    }

    DxLib_End();

    return 0;
}