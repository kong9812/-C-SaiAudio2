#include "SaiEffect.h"

// 初期化
SAI_EFFECT_INTERFACE::SAI_EFFECT_INTERFACE(SAI_VOICE_TOOL *svt)
{
	// APO
	IUnknown *MyApo;
	MyApo = (IXAPO *)new SAI_DELAY_APO(svt);

	// パラメータ
	saiDelay.volume		= 0.4f;
	saiDelay.delayTime	= 0.25f;
	saiDelay.wavFile	= svt->svtBase.wavFmt;

	// ディスクリプタ
	descriptor.InitialState		= true;
	descriptor.OutputChannels	= 2;
	descriptor.pEffect			= MyApo;

	// チェン
	chain.EffectCount			= 1;
	chain.pEffectDescriptors	= &descriptor;

	// エフェクトの設置
	svt->svtBase.sourceVoice->SetEffectChain(&chain);
	svt->svtBase.sourceVoice->SetEffectParameters(0, &saiDelay, sizeof(saiDelay));

	// APOの解放
	MyApo->Release();
}

// 終了処理
SAI_EFFECT_INTERFACE::~SAI_EFFECT_INTERFACE()
{

}
