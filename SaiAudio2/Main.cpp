#include "Main.h"

// グローバル変数
SAI_AUDIO2_INTERFACE	*saiAudio2;		// SaiAudio2のインターフェース
SAI_EFFECT_INTERFACE	*saiEffect;		// SaiAudio2のエフェクトインターフェース
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

	// ピーチ
	saiAudio2->SetPitch(&testSoundSVT);

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

	// テストサウンドのSVTの初期化
	testSoundSVT.svtBase.loopCnt		= SAI_LOOP_CNT_INFINITE;

	// 初期化の検証
	if (saiAudio2->InitCheck())
	{
		// 読み込み
		testSoundSVT.svtBase.sourceVoice = saiAudio2->LoadXAudio2Voice(SOUND_PATH, &testSoundSVT);
	}

	// テスト用(Get)
	testSoundSVT.svtSecondary.freqPich = saiAudio2->GetPitch(&testSoundSVT);
	// テスト用(Set)
	testSoundSVT.svtSecondary.freqPich = 1.5f;

	// エフェクトインターフェースの作成
	saiEffect = new SAI_EFFECT_INTERFACE(&testSoundSVT);

}

// 終了処理
void Uninit(void)
{
	// インターフェース解放
	delete saiAudio2;
	delete saiEffect;
}