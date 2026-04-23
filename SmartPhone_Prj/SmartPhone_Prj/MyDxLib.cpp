#include <DxLib.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    ChangeWindowMode(TRUE); // ウィンドウモードに設定h
    SetGraphMode(480, 800, 32);
    DxLib_Init(); // DXライブラリ初期化
    SetDrawScreen(DX_SCREEN_BACK); // 裏画面に描画設定
    int x = 50; // 描画するX座標
    int y = 50; // 描画するY座標
    int color = GetColor(255, 255, 255); // 白色のカラーコード取得
    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {
        DrawString(x, y, L"Hello, DX Library!", color); // テキスト描画
    }
    DxLib_End(); // DXライブラリ終了処理
    return 0;
}