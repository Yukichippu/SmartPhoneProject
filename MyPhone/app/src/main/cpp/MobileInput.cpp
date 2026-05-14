//
// Created by k2-takeshita on 2026/05/06.
//
#include "DxLib.h"
#include <math.h>

struct TouchPoint
{
    int x;
    int y;
};

//5/14 new↓
struct LinePos
{
    int setLineX1;  //レーン1のX座標
    int setLineX2;  //レーン2のX座標
    int setLineX3;  //レーン2のX座標
};

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

//5/14 new↓
void SetPlayer(int x)
{
    LinePos* line;

    //定義
    line->setLineX1 = 120;
    line->setLineX2 = 360;
    line->setLineX3 = 600;

    //レーン移動の範囲
    int boxEdgeL1 = line->setLineX1+60;
    int boxEdgeL2 = line->setLineX2+60;
    int boxEdgeL3 = line->setLineX3+60;

    //プレイヤーのY座標固定
    int y = 400;

    DrawBox(x,y,x+200,y+200, GetColor(255,255,255),TRUE);

    if(x < boxEdgeL1){
        //playerの座標をsetLineX1に設定
        DrawBox(line->setLineX1-30,y-30,
                line->setLineX1+30,y+30,
                GetColor(255,255,255),
                TRUE);
    }
    else if(x < boxEdgeL2 && x > boxEdgeL1){
        //playerの座標をsetLineX2に設定
        DrawBox(line->setLineX2-30,y-30,
                line->setLineX2+30,y+30,
                GetColor(255,255,255),
                TRUE);
    }
    else if(x < boxEdgeL3 && x > boxEdgeL2){
        //playerの座標をsetLineX3に設定
        DrawBox(line->setLineX3-30,y-30,
                line->setLineX3+30,y+30,
                GetColor(255,255,255),
                TRUE);
    }
}

float GetDistance(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrtf(dx * dx + dy * dy);
}

void InitMobileInput(MobileInput* input)
{
    input->isTouching = false;

    input->tap = false;
    input->swipe = false;
    input->flick = false;
    input->pinchIn = false;
    input->pinchOut = false;

    input->startX = 0;
    input->startY = 0;
    input->currentX = 0;
    input->currentY = 0;

    input->startTime = 0;
    input->touchCount = 0;

    input->previousPinchDistance = 0.0f;
}

void UpdateMobileInput(MobileInput* input)
{
    const int swipeDistance = 80;
    const int flickTime = 200;
    const float pinchThreshold = 5.0f;

    input->tap = false;
    input->swipe = false;
    input->flick = false;
    input->pinchIn = false;
    input->pinchOut = false;

    int touchNum = GetTouchInputNum();
    input->touchCount = touchNum;

    if (touchNum == 0)
    {
        if (input->isTouching)
        {
            int dx = input->currentX - input->startX;
            int dy = input->currentY - input->startY;
            float distance = GetDistance(
                    (float)input->startX,
                    (float)input->startY,
                    (float)input->currentX,
                    (float)input->currentY
            );

            int elapsedTime = GetNowCount() - input->startTime;

            if (distance < swipeDistance)
            {
                input->tap = true;
            }
            else
            {
                input->swipe = true;

                if (elapsedTime <= flickTime)
                {
                    input->flick = true;
                }
            }
        }

        input->isTouching = false;
        input->previousPinchDistance = 0.0f;
        return;
    }

    int x = 0;
    int y = 0;

    GetTouchInput(0, &x, &y, NULL, NULL);

    if (!input->isTouching)
    {
        input->isTouching = true;

        input->startX = x;
        input->startY = y;
        input->currentX = x;
        input->currentY = y;

        input->startTime = GetNowCount();
    }
    else
    {
        input->currentX = x;
        input->currentY = y;

        //SetPlayer(input->currentX);

    }

    if (touchNum >= 2)
    {
        int x1 = 0;
        int y1 = 0;
        int x2 = 0;
        int y2 = 0;

        GetTouchInput(0, &x1, &y1, NULL, NULL);
        GetTouchInput(1, &x2, &y2, NULL, NULL);

        float currentDistance = GetDistance(
                (float)x1,
                (float)y1,
                (float)x2,
                (float)y2
        );

        if (input->previousPinchDistance == 0.0f)
        {
            input->previousPinchDistance = currentDistance;
            return;
        }

        float diff = currentDistance - input->previousPinchDistance;

        if (fabsf(diff) > pinchThreshold)
        {
            if (diff > 0)
            {
                input->pinchOut = true;
            }
            else
            {
                input->pinchIn = true;
            }
        }

        input->previousPinchDistance = currentDistance;
    }
}

void DrawInputDebug(const MobileInput& input)
{
    DrawFormatString(20, 20, GetColor(255, 255, 255), "Touch Count : %d", input.touchCount);
    DrawFormatString(20, 50, GetColor(255, 255, 255), "Current : %d, %d", input.currentX, input.currentY);
    DrawFormatString(20, 80, GetColor(255, 255, 255), "Start   : %d, %d", input.startX, input.startY);

    if (input.tap)
    {
        DrawString(20, 140, "Tap", GetColor(255, 255, 0));
    }

    if (input.swipe)
    {
        DrawString(20, 170, "Swipe", GetColor(0, 255, 255));
    }

    if (input.flick)
    {
        DrawString(20, 200, "Flick", GetColor(255, 120, 0));
    }

    if (input.pinchIn)
    {
        DrawString(20, 230, "Pinch In", GetColor(255, 100, 255));
    }

    if (input.pinchOut)
    {
        DrawString(20, 260, "Pinch Out", GetColor(100, 255, 100));
    }

    if (input.isTouching)
    {
        DrawCircle(input.currentX, input.currentY, 20, GetColor(255, 255, 255), TRUE);
        DrawLine(input.startX, input.startY, input.currentX, input.currentY, GetColor(255, 255, 0));
    }
}

int android_main()
{
    SetGraphMode(720, 1280, 32);

    if (DxLib_Init() == -1)
    {
        return -1;
    }

    SetDrawScreen(DX_SCREEN_BACK);

    MobileInput input;
    InitMobileInput(&input);

    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();

        SetPlayer(400);

        UpdateMobileInput(&input);
        DrawInputDebug(input);

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}