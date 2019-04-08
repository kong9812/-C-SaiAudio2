#include "SaiEffect.h"

// ������
SAI_EFFECT_INTERFACE::SAI_EFFECT_INTERFACE(SAI_VOICE_TOOL *svt)
{
	// APO
	IUnknown *MyApo;
	MyApo = (IXAPO *)new SAI_DELAY_APO(svt);

	// �p�����[�^
	saiDelay.volume		= 0.4f;
	saiDelay.delayTime	= 0.25f;
	saiDelay.wavFile	= svt->svtBase.wavFmt;

	// �f�B�X�N���v�^
	descriptor.InitialState		= true;
	descriptor.OutputChannels	= 2;
	descriptor.pEffect			= MyApo;

	// �`�F��
	chain.EffectCount			= 1;
	chain.pEffectDescriptors	= &descriptor;

	// �G�t�F�N�g�̐ݒu
	svt->svtBase.sourceVoice->SetEffectChain(&chain);
	svt->svtBase.sourceVoice->SetEffectParameters(0, &saiDelay, sizeof(saiDelay));

	// APO�̉��
	MyApo->Release();
}

// �I������
SAI_EFFECT_INTERFACE::~SAI_EFFECT_INTERFACE()
{

}
