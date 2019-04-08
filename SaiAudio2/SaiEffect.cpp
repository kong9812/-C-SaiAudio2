#include "SaiEffect.h"

// ������
SAI_EFFECT_INTERFACE::SAI_EFFECT_INTERFACE(SAI_VOICE_TOOL *svt)
{
	// APO
	IUnknown *SaiDelayApo;
	SaiDelayApo = (IXAPO *)new SAI_DELAY_APO(svt);
	IUnknown *SaiFadeInApo;
	SaiFadeInApo = (IXAPO *)new SAI_FADE_IN_APO(svt);

	// �p�����[�^
	saiDelay.volume		= 0.4f;
	saiDelay.delayTime	= 0.25f;

	saiFadeIn.fadeTime	= 10.0f;

	// �f�B�X�N���v�^
	descriptor[0].InitialState		= true;
	descriptor[0].OutputChannels	= 2;
	descriptor[0].pEffect			= SaiDelayApo;
	descriptor[1].InitialState		= true;
	descriptor[1].OutputChannels	= 2;
	descriptor[1].pEffect			= SaiFadeInApo;

	// �`�F��
	chain.EffectCount			= 2;
	chain.pEffectDescriptors	= descriptor;

	// �G�t�F�N�g�̐ݒu
	svt->svtBase.sourceVoice->SetEffectChain(&chain);
	svt->svtBase.sourceVoice->SetEffectParameters(0, &saiDelay, sizeof(saiDelay));
	svt->svtBase.sourceVoice->SetEffectParameters(1, &saiFadeIn, sizeof(saiFadeIn));

	// APO�̉��
	SaiDelayApo->Release();
	SaiFadeInApo->Release();
}

// �I������
SAI_EFFECT_INTERFACE::~SAI_EFFECT_INTERFACE()
{

}
