#include "DxLib.h"
#include <math.h>
#include <vector>

struct MobileInput
{
    bool isTouching;

    bool tap;
    bool swipe;
    bool flick;
    bool pinchIn;
    bool pinchOut;

    int startX;
    int startY;
    int currentX;
    int currentY;

    int startTime;
    int touchCount;

    float previousPinchDistance;
};

void InitMobileInput(MobileInput* input)
{
    input->isTouching = false;
    input->flick = false;

    input->startX = 0;
    input->startY = 0;
    input->currentX = 0;
    input->currentY = 0;

    input->startTime = 0;
}

//敵
struct Rectangle {
    int x;
    int y;
    int width;
    int height;
    int speed;
};

std::vector<Rectangle> rectangles;

//弾
struct CircleShot
{
    float x;
    float y;

    float vx;
    float vy;

    bool active;
};

std::vector<CircleShot> circles;

//攻撃範囲
const int FLICK_AREA_X = 200;
const int FLICK_AREA_Y = 1000;
const int FLICK_AREA_W = 320;
const int FLICK_AREA_H = 280;

//敵生成
void CreateRectangle() {
    Rectangle rect;
    rect.x = rand() % 600 + 100;
    rect.y = 0;                       //画面上からスタート
    rect.width = 100;
    rect.height = 100;
    rect.speed = 5;                   //移動速度
    rectangles.push_back(rect);
}

void UpdateAndDrawRectangles() {
    for (int i = 0; i < (int)rectangles.size(); i++) {
        rectangles[i].y += rectangles[i].speed; // 下に移動
        DrawBox(rectangles[i].x, rectangles[i].y,
                rectangles[i].x + rectangles[i].width,
                rectangles[i].y + rectangles[i].height,
                GetColor(255, 0, 0), TRUE);
    }

    // 画面外に出た四角形を削除
    rectangles.erase(
            std::remove_if(rectangles.begin(), rectangles.end(),
                           [](const Rectangle& r) { return r.y > 1280; }), // 画面下端 = 1280
            rectangles.end()
    );
}

//攻撃フラグ
bool IsInFlickArea(int x, int y)
{
    return x >= FLICK_AREA_X &&
           x <= FLICK_AREA_X + FLICK_AREA_W &&
           y >= FLICK_AREA_Y &&
           y <= FLICK_AREA_Y + FLICK_AREA_H;
}

void UpdateCircles()
{
    for(auto& c : circles)
    {
        if(!c.active) continue;

        c.x += c.vx;
        c.y += c.vy;

        DrawCircle(
                (int)c.x,
                (int)c.y,
                20,
                GetColor(0,255,0),
                TRUE);

        if(c.x < 0 || c.x > 720 ||
           c.y < 0 || c.y > 1280)
        {
            c.active = false;
        }
    }
}

//フリック挙動管理
void UpdateMobileInput(MobileInput* input)
{
    const int flickDistance = 80; // 最低移動距離
    const int flickTime = 200;    // 制限時間(ms)

    input->flick = false;

    int touchNum = GetTouchInputNum();

    // 指を離した瞬間
    if (touchNum == 0)
    {
        if (input->isTouching)
        {
            int dx = input->currentX - input->startX;
            int dy = input->currentY - input->startY;

            int elapsedTime = GetNowCount() - input->startTime;

            // フリック判定
            if ((abs(dx) > flickDistance ||
                 abs(dy) > flickDistance) &&
                elapsedTime < flickTime)
            {
                input->flick = true;

                // 開始位置が指定範囲内なら発射
                if (IsInFlickArea(input->startX, input->startY))
                {
                    CircleShot shot;

                    shot.x = input->startX;
                    shot.y = input->startY;

                    float length = sqrtf(
                            (float)(dx * dx + dy * dy));

                    shot.vx = dx / length * 15.0f;
                    shot.vy = dy / length * 15.0f;

                    shot.active = true;

                    circles.push_back(shot);
                }
            }
        }

        input->isTouching = false;
        return;
    }

    int x, y;
    GetTouchInput(0, &x, &y, NULL, NULL);

    // タッチ開始
    if (!input->isTouching)
    {
        input->isTouching = true;

        input->startX = x;
        input->startY = y;

        input->currentX = x;
        input->currentY = y;

        input->startTime = GetNowCount();
    }
        // タッチ中
    else
    {
        input->currentX = x;
        input->currentY = y;
    }
}

bool CheckCollision(const CircleShot& bullet, const Rectangle& enemy)
{
    const float radius = 20.0f;

    float closestX = bullet.x;
    float closestY = bullet.y;

    if (closestX < enemy.x) closestX = enemy.x;
    if (closestX > enemy.x + enemy.width) closestX = enemy.x + enemy.width;

    if (closestY < enemy.y) closestY = enemy.y;
    if (closestY > enemy.y + enemy.height) closestY = enemy.y + enemy.height;

    float dx = bullet.x - closestX;
    float dy = bullet.y - closestY;

    return (dx * dx + dy * dy) <= radius * radius;
}

//当たり判定
void CheckBulletEnemyCollision()
{
    std::vector<int> deleteEnemies;

    for (size_t i = 0; i < circles.size(); i++)
    {
        if (!circles[i].active) continue;

        for (size_t j = 0; j < rectangles.size(); j++)
        {
            if (CheckCollision(circles[i], rectangles[j]))
            {
                circles[i].active = false;
                deleteEnemies.push_back((int)j);
                break;
            }
        }
    }

    // 敵削除
    std::sort(deleteEnemies.rbegin(), deleteEnemies.rend());

    for (int index : deleteEnemies)
    {
        rectangles.erase(rectangles.begin() + index);
    }

    // 非アクティブ弾削除
    circles.erase(
            std::remove_if(
                    circles.begin(),
                    circles.end(),
                    [](const CircleShot& c)
                    {
                        return !c.active;
                    }),
            circles.end());
}

int android_main()
{
    SetGraphMode(720, 1280, 32);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    srand((unsigned int)time(NULL)); // 乱数初期化

    int frameCount = 0;

    MobileInput input;
    InitMobileInput(&input);

    while (ProcessMessage() == 0 && ClearDrawScreen() == 0)
    {
        ClearDrawScreen();

        frameCount++;

        //120フレームごとに生成
        if (frameCount % 120 == 0)
            CreateRectangle();

        UpdateCircles();
        UpdateAndDrawRectangles();

        //攻撃範囲視覚化
        DrawBox(FLICK_AREA_X,
                FLICK_AREA_Y,
                FLICK_AREA_X+FLICK_AREA_W,
                FLICK_AREA_Y+FLICK_AREA_H,
                GetColor(255, 255, 255), TRUE);

        CheckBulletEnemyCollision();

        UpdateMobileInput(&input);

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}