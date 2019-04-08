#include "Main.h"

// グローバル変数
SAI_AUDIO2_INTERFACE *saiAudio2;	// SaiAudio2のインターフェース


// プロトタイプ宣言
void Init(void);		// 初期化
void Uninit(void);		// 終了処理

// メイン関数
int main(void)
{


}

// 初期化
void Init(void)
{
	// インターフェース作成
	saiAudio2 = new SAI_AUDIO2_INTERFACE;
}

// 終了処理
void Uninit(void)
{
	// インターフェース解放
	delete saiAudio2;
}