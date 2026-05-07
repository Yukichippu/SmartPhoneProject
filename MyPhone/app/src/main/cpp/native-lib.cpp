#include "DxLib.h"

int penDown; //1=前回ペンダウン、0=前回ペンアップ -1=終了
int penColor; //ペンの色

// プログラムは android_main から始まります
int android_main( void )
{
    if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
    {
        return -1 ;			// エラーが起きたら直ちに終了
    }
    DrawBox( 220, 140, 420, 340, GetColor( 255,255,255 ), TRUE ) ;	// 四角形を描画する

    penDown = 0;
    penColor = GetColor( 255,255,255 ); //白色
    DrawBox( 0, 0, 50, 50, penColor, FALSE ) ;	// 左上すみの四角

    //イベントループ
    while( ProcessMessage() == 0)
    {
        TOUCHINPUTDATA tid;
        TOUCHINPUTPOINT tip;
        int ii,xx,yy;

        // if(GetTouchInputLogNum()==0) continue;//ログの個数
        tid=GetTouchInputLogOne( FALSE ) ; //ログを読み出す。FALSEは削除
        if(tid.PointNum == 0)
        {
            penDown = 0; //ペンアップ検出
        }
        for(ii=0; ii < tid.PointNum; ii++) //座標入力あり
        {
            tip = tid.Point[ii];
            xx = tip.PositionX; //X 座標
            yy = tip.PositionY; //Y 座標
            if( penDown==0 ){ //タッチ検出(前回がペンアップ)
                if( xx < 50 && yy < 50 ) //左上すみタッチ
                {
                    penDown = -1;
                    break;
                }
                if( xx >= 220 && xx <420 && yy >= 140 && yy <340 ) penColor = GetColor( 255,0,0 ); //内側赤
                else penColor = GetColor( 255,255,0 );  //外側 黄色
            }
            DrawCircle( xx , yy , 10 , penColor , TRUE ); //TRUEは塗りつぶし
            penDown = 1;
        }
        if(penDown < 0) break; //終了
    }

    DxLib_End() ;				// ＤＸライブラリ使用の終了処理
    return 0 ;					// ソフトの終了
}