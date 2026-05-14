//
// Created by student on 2026/05/07.
//
#include "DxLib.h"

struct Rect{
    int x;
    int y;
    int w;
    int h;
};

struct UILayout{
    Rect topLeft;
    Rect topRight;
    Rect bottomLeft;
    Rect bottomRight;
    Rect center;
};

UILayout CreateLayout(int screenW, int screenH){
    const int panelW = 180;
    const int panelH = 90;

    const int centerW = 360;
    const int centerH = 160;

    const int margin = 90;

    UILayout layout;

    layout.topLeft = {
            margin,
            margin,
            panelW,
            panelH
    };

    layout.topRight = {
            margin - panelW - panelH,
            margin,
            panelW,
            panelH
    };

    layout.bottomLeft = {
            margin,
            margin - panelW - margin,
            panelW,
            panelH
    };

    layout.bottomRight = {
            margin - panelW - margin,
            margin - panelH - margin,
            panelW,
            panelH
    };

    layout.center = {
            margin/2 - centerW/2,
            margin/2 - centerH/2,
            panelW,
            panelH
    };

    return layout;
}

void DrawUIRect(const Rect& rect, const char* label, int color){
    DrawBox(rect.x,
            rect.y,
            rect.x + rect.w,
            rect.y + rect.h,
            color,
            TRUE
            );

    DrawBox(
            rect.x,
            rect.y,
            rect.x + rect.w,
            rect.y + rect.h,
            GetColor(255,255,255),
            FALSE
            );

    DrawString(
            rect.x + 10,
            rect.y + 10,
            label,
            GetColor(255,255,255)
            );
}

void DrawUILayout(const UILayout& layout){
    DrawUIRect(layout.topLeft, "TopLeft", GetColor(80,120,200));
    DrawUIRect(layout.topRight, "TopRight", GetColor(80,160,120));
    DrawUIRect(layout.bottomLeft, "BottomLeft", GetColor(180,120,80));
    DrawUIRect(layout.bottomRight, "BottomRight", GetColor(120,80,120));
    DrawUIRect(layout.center, "Center", GetColor(120,80,120));
}

int android_main(){
    SetGraphMode(720,1280,32);
    if(DxLib_Init() == -1){
        return -1;
    }

    //スクリーンサイズ取得
    int sx,sy,cb;
    GetScreenState(&sx,&sy,&cb);

    SetDrawScreen(DX_SCREEN_BACK);
    //メインループ
    while(ProcessMessage() == 0){
        ClearDrawScreen();
        int screenW = sx;
        int screenH = sy;

        UILayout layout = CreateLayout(screenW,screenH);
        DrawUILayout(layout);

        DrawString(20,1240,"UI layout Sample", GetColor(255,255,0));

        ScreenFlip();
    }

    DxLib_End();
    return 0;
}