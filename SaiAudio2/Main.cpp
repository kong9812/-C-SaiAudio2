#include "Main.h"

// グローバル変数
SAI_AUDIO2_INTERFACE	*saiAudio2;		// SaiAudio2のインターフェース
SAI_VOICE_TOOL			testSoundSVT;	// テストサウンドのSVT

// プロトタイプ宣言
void Init(void);		// 初期化
void Uninit(void);		// 終了処理

// メイン関数
int main(void)
{
	// 初期化
	Init();

	// 再生
	saiAudio2->PlayVoice(&testSoundSVT);

	// キーを押したら進む
	getchar();

	// 終了処理
	Uninit();
}

// 初期化
void Init(void)
{
	// インターフェース作成
	saiAudio2 = new SAI_AUDIO2_INTERFACE;

	// 初期化の検証
	if (saiAudio2->InitCheck())
	{
		// 読み込み
		testSoundSVT.sourceVoice = saiAudio2->LoadXAudio2Voice(SOUND_PATH, &testSoundSVT);
	}
}

// 終了処理
void Uninit(void)
{
	// インターフェース解放
	delete saiAudio2;
}