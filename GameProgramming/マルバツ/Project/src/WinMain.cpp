
// ==============================
// 2020年 前期 
// ゲームプログラミング１
// 制作課題 その２
// ==============================

/**
■説明
	以下のプログラムは [ 〇×(三目並べ) ] というゲームをコーディングし、
	実行を確認したうえでその一部分を削除したものです。

	削除した部分は　※※　と記載し、
	本来入るべき処理の内容をコメントしてあります。

	コメント内容や他のコードを読み解き、ゲームを完成させなさい。

	※Hit And Blowに比べてコメントのヒントが少ないので
	　どのような処理を書けばよいか良く考えましょう
 */

/**
■制作環境
	このプログラムは、[ ＤＸライブラリ ] というライブラリを用いて作られています。
		ホームページ：https://dxlib.xsrv.jp/
		リファレンス：https://dxlib.xsrv.jp/dxfunc.html

	ＤＸライブラリについて興味のある人は、
	ホームページ、リファレンスページ（用意されている関数の説明）を見てみてください。
*/

/**
■〇×(三目並べ)のルール
	ルールについては、自分で調べて把握するようにしてください
*/

#include "WinMain.h"
#include "InputManager.h"
#include "DrawManager.h"
#include <stdio.h>
#include <time.h>

// ==============================
// グローバル変数
// ==============================
int map[STAGE_HEIGHT][STAGE_WIDTH];		// 横:STAGE_WIDTH、縦:STAGE_HEIGHTのint型２次元配列 map を宣言
int mapStone = 0;							// ターンが終わった回数を数える変数

// ==============================
// 関数プロトタイプ宣言
// ==============================
// 盤面の x, y の位置に石が置けるかどうか
// ※※ // bool型の戻り値、int型の引数x,yを持つIsPutStone関数を宣言
bool IsPutStone(int x, int y);

// 勝者が居るかを調べる
// ※※ // int型の戻り値を持つCheckWinner関数を宣言
int CheckWinner();

// ==============================
// Main関数
// ==============================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ＤＸライブラリを使う上での初期化処理
	// ----------------------------------------------------
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(TRUE);
	SetGraphMode(WINDOW_W, WINDOW_H, 32);
	SetBackgroundColor(125, 125, 255);
	SetMainWindowText("マルバツ");
	if (DxLib_Init() == -1) { return -1; }

	SetDrawScreen(DX_SCREEN_BACK);

	// ローカル変数宣言
	// -------------------------------------------------------------------------------------------
	int pos_x = 0;				// 整数型の変数pos_xを宣言し、0で初期化		// X座標選択位置
	int pos_y = 0;				// 整数型の変数pos_yを宣言し、0で初期化		// Y座標選択位置
	int turn = STONE_WHITE;		// 整数型の変数turnを宣言し、STONE_WHITEで初期化	// 現在の手番
	int winner = WINNER_NON;	// 整数型の変数winnerを宣言し、WINNER_NONで初期化	// 勝利者

	// 各種初期化処理
	// ----------------------------------------------------
	InputInit();			// 入力処理初期化関数の呼び出し
	DrawInit();				// 描画処理初期化関数の呼び出し

	// mapの初期化
	// 二次元配列mapの全要素を STONE_MAX で初期化する
	for (int i = 0; i < STAGE_WIDTH; i++)
	{
		for (int j = 0; j < STAGE_HEIGHT; j++)
		{
			map[j][i] = STONE_MAX;
		}
	}

	// ゲームのメインループ
	// 画面を１回表示する毎にwhile分を１回処理する
	// --------------------------------------------------------------------
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ＤＸライブラリを使う上で、１フレーム分の処理を始めるためのお約束
		// ----------------------------------------------------------------
		ClearDrawScreen();
		clsDx();

		// 以下、毎フレーム更新する処理
		// ----------------------------------------------------
		InputUpdate();				// 入力処理更新関数の呼び出し
		winner = CheckWinner();		// ※※ winner に勝利者情報を代入	// 勝利者のチェック

		// --- 入力状況をチェックして、適切な処理を行う
		// 決着がついてない時だけ入力を受け付けるように if文 でチェックする
		if( winner == WINNER_NON )
		{
			// 上下左右の入力があった時の処理
			if(IsPushKey(MY_INPUT_UP))
			{
				pos_y--;//※※ pos_yの値を 1 減らす
				if (pos_y < 0)
				{
					pos_y = 0;
				}
			}
			else if(IsPushKey(MY_INPUT_DOWN))
			{
				pos_y++;//※※ pos_yの値を 1 増やす
				if (pos_y >= 3)
				{
					pos_y = 2;
				}
			}
			else if(IsPushKey(MY_INPUT_LEFT))
			{
				pos_x--;//※※ pos_xの値を 1 減らす
				if (pos_x < 0)
				{
					pos_x = 0;
				}
			}
			else if(IsPushKey(MY_INPUT_RIGHT))
			{
				pos_x++;//※※ pos_xの値を 1 増やす
				if (pos_x >= 3)
				{
					pos_x = 2;
				}
			}
			// 決定(=エンターキー)が押された時の処理
			else if(IsPushKey(MY_INPUT_ENTER))
			{
				// 現在の座標が有効か if文 でチェックし、
				// 結果が true の時、以下の処理を行う
				if (IsPutStone(pos_x,pos_y) == true)
				{
					map[pos_y][pos_x] = turn;
					turn++;
					turn = turn % 2;
					mapStone++;
					// ※※
					// 以下の処理を実装する
					// 選択されている座標と対応するmap配列の要素へturnの値を代入
					// 次のターンに回すため、turnの値を変更する
					// count++はターンが終わった回数を数えている
				}
			}
		}

		// 以下、描画処理
		// ----------------------------------------------------
		DrawInformation(turn);		// ※※　// 情報文章を描画
		DrawGameClear(winner);		// ※※　// ゲームクリアの文字を描画
		DrawBgLine();				// ※※　// 枠線を描画
		for (int i = 0; i < STAGE_WIDTH; i++)
		{
			for (int j = 0; j < STAGE_HEIGHT; j++)
			{
				DrawStone(i, j, map[j][i]);
				// ※※　// ２重for文を使って盤面の石を描画する
			}
		}
		DrawCursor(pos_x, pos_y);	// ※※　// カーソルを描画

		// ＤＸライブラリを使う上で、モニターへゲーム画面を表示するためのお約束
		// 必ずループの最後で呼び出す
		// ----------------------------------------------------
		ScreenFlip();
	}

	// 後始末
	// ----------------------------------------------------
	DrawEnd();		// 描画処理終了

	// ＤＸライブラリを使う上での終了処理
	// ----------------------------------------------------
	DxLib_End();
	return 0;
}

// ==============================
// 盤面の x, y の位置に石が置けるかどうか
// true = 置ける
// false = 置けない
// ==============================
bool IsPutStone( int x, int y )
{
	if (map[y][x] == STONE_MAX )
	{
		return true;
	}
	else
	{
		return false;
	}
	// ※※　盤面の x, y の位置に石が置けるならtrue,置けないならfalseを返す処理
	
}

// ==============================
// 勝者が居るかを調べる
// ==============================
int CheckWinner()
{
	// ※※　以下の処理を実装する
	// 縦、横、斜めが同じ石かどうかを調べる
	// STONE_WHITE, STONE_BLACK, STONE_MAXを上手く使いましょう
	for (int i = 0; i < STAGE_HEIGHT; i++)
	{
		if (map[i][0] == STONE_WHITE && map[i][1] == STONE_WHITE && map[i][2] == STONE_WHITE)
		{
			return WINNER_WHITE;
		}
		else if (map[i][0] == STONE_BLACK && map[i][1] == STONE_BLACK && map[i][2] == STONE_BLACK)
		{
			return WINNER_BLACK;
		}
	}

	for (int j = 0; j < STAGE_WIDTH; j++)
	{

		if (map[0][j] == STONE_WHITE && map[1][j] == STONE_WHITE && map[2][j] == STONE_WHITE)
		{
			return WINNER_WHITE;
		}
		else if (map[0][j] == STONE_BLACK && map[1][j] == STONE_BLACK && map[2][j] == STONE_BLACK)
		{
			return WINNER_BLACK;
		}
	}

	if (map[0][0] == STONE_WHITE && map[1][1] == STONE_WHITE && map[2][2] == STONE_WHITE || 
		map[2][0] == STONE_WHITE && map[1][1] == STONE_WHITE && map[0][2] == STONE_WHITE)
	{
		return WINNER_WHITE;
	}
	if (map[0][0] == STONE_BLACK && map[1][1] == STONE_BLACK && map[2][2] == STONE_BLACK || 
		map[2][0] == STONE_BLACK && map[1][1] == STONE_BLACK && map[0][2] == STONE_BLACK)
	{
		return WINNER_BLACK;
	}

	// もし、まだ揃っていなかったら、盤面に置かれている石の数を調べる
	// 全てのマスに石が置かれていたら引き分け
	if (mapStone == 9)	
	{
		return WINNER_DRAW;
	}
	
	return WINNER_NON;
	// 上記のいずれかでも無かったらWINNER_NONを返す
	
}
