#include "Main.h"

// �O���[�o���ϐ�
SAI_AUDIO2_INTERFACE *saiAudio2;	// SaiAudio2�̃C���^�[�t�F�[�X


// �v���g�^�C�v�錾
void Init(void);		// ������
void Uninit(void);		// �I������

// ���C���֐�
int main(void)
{


}

// ������
void Init(void)
{
	// �C���^�[�t�F�[�X�쐬
	saiAudio2 = new SAI_AUDIO2_INTERFACE;
}

// �I������
void Uninit(void)
{
	// �C���^�[�t�F�[�X���
	delete saiAudio2;
}