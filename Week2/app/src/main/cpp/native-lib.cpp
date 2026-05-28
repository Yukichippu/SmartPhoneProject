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

//プレイヤー
struct Ball
{
    Vector2 pos;
    Vector2 velocity;
    float radius;
};

//敵
struct Enemy
{
    Vector2 pos;
    float radius;
    bool alive;
};

//距離判定
bool IsHit(Ball ball, Enemy enemy)
{
    float dx = ball.pos.x - enemy.pos.x;
    float dy = ball.pos.y - enemy.pos.y;

    float dist = std::sqrt(dx * dx + dy * dy);

    return dist < (ball.radius + enemy.radius);
}

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

    ball.pos = {300.0f, 300.0f};
    ball.velocity = {0.0f, 0.0f};
    ball.radius = 40.0f;

    const int ENEMY_NUM = 5;

    Enemy enemies[ENEMY_NUM];

    for (int i = 0; i < ENEMY_NUM; i++)
    {
        enemies[i].pos =
                {
                        100.0f + i * 100.0f,
                        700.0f
                };

        enemies[i].radius = 30.0f;
        enemies[i].alive = true;
    }

    bool dragging = false;

    Vector2 dragStart = {0.0f, 0.0f};
    Vector2 dragNow   = {0.0f, 0.0f};

    while (ProcessMessage() == 0)
    {
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

                dir.x = dragStart.x - dragNow.x;
                dir.y = dragStart.y - dragNow.y;

                float power = Length(dir);

                Vector2 normal = Normalize(dir);

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

        for (int i = 0; i < ENEMY_NUM; i++)
        {
            if (!enemies[i].alive)
            {
                continue;
            }

            if (IsHit(ball, enemies[i]))
            {
                enemies[i].alive = false;
            }
        }

        ClearDrawScreen();

        for (int i = 0; i < ENEMY_NUM; i++)
        {
            if (!enemies[i].alive)
            {
                continue;
            }

            DrawCircle(
                    (int)enemies[i].pos.x,
                    (int)enemies[i].pos.y,
                    (int)enemies[i].radius,
                    GetColor(0, 255, 0),
                    TRUE
            );
        }

        DrawCircle(
                (int)ball.pos.x,
                (int)ball.pos.y,
                (int)ball.radius,
                GetColor(255, 80, 80),
                TRUE
        );

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
                "敵に当たると消える",
                GetColor(255, 255, 255)
        );

        ScreenFlip();
    }

    DxLib_End();

    return 0;
}