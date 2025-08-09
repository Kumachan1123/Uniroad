/*
*	@file ResultUI.h
*	@brief ����UI�N���X
*/
#pragma once
// �W�����C�u����
#include <vector>
#include <string>
// DirectX
#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Mouse.h>
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/InputManager.h>
// ����w�b�_�[�t�@�C��
#include "KumachiLib/Easing/Easing.h"
#include "Game/CommonResources/CommonResources.h"
#include "Game/ControllScreen/Canvas/Canvas.h"
#include "Game/Interface/IUI/IUI.h"
#include "Game/Screen/Screen.h"
#include "KumachiLib/BinaryFile/BinaryFile.h"
#include "KumachiLib/MouseClick/MouseClick.h"
#include "KumachiLib/FileCounter/FileCounter.h"

// �O���錾
class CommonResources;

// ����UI�N���X
class ResultUI : public IUI
{
public:
	// �񋓌^
	// �V�[��ID
	enum SceneID
	{
		NONE = -1, // �����ȃV�[��ID
		SELECT_STAGE,// �X�e�[�W�I��
		REPLAY,// ���v���C
	};
public:
	// �A�N�Z�T
	// �V�[���ԍ��擾
	SceneID GetSceneNum() const { return m_num; }
	// �V�[���ԍ��ݒ�
	void SetSceneNum(SceneID num) { m_num = num; }
	// ���̃N���X���L�����ǂ������擾
	bool IsEnable() const { return m_enable; }
	// ���̃N���X���L�����ǂ�����ݒ�
	void SetEnable(bool enable) { m_enable = enable; }
	// �V�[�����猋�ʂ��󂯎��
	void SetResult(bool gameOver, bool gameClear);
	// �V�[������X�e�[�W�ԍ����󂯎��
	void SetStageNum(int stageNum) { m_stageNum = stageNum; }
	// �X�e�[�W�ԍ����擾
	int GetStageNum() const { return m_stageNum; }
public:
	// public�֐�
	// �R���X�g���N�^
	ResultUI();
	// �f�X�g���N�^
	~ResultUI();
	// ������
	void Initialize(CommonResources* resources, int width, int height) override;
	// �X�V
	void Update(const float elapsedTime) override;
	// �`��
	void Render() override;
	// UI�ǉ�
	void Add(
		const std::string& key,						// �e�N�X�`���̃L�[
		const DirectX::SimpleMath::Vector2& position,	// �ʒu
		const DirectX::SimpleMath::Vector2& scale,	// �X�P�[��
		KumachiLib::ANCHOR anchor,					// �A���J�[
		UIType type) override;						// UI�̎��
private:
	// private�֐�
	// ��ʃ��T�C�Y���̏���
	void OnResize();
private:
	// private�萔
	// �����ȃ��j���[�C���f�b�N�X
	static const int INVALID_MENU_INDEX;
	// �ʏ�̕\���ʒu�␳�l(�S��ʊ�j
	static const float DISPLAY_OFFSET;
	// �{�^���̈ړ��ɂ����鎞��
	static const float MOVE_DURATION;
private:
	// private�֐�
	// �f�o�C�X���\�[�X
	DX::DeviceResources* m_pDR;
	// ���ʃ��\�[�X
	CommonResources* m_pCommonResources;
	// �{�^��
	std::vector<std::unique_ptr<Canvas>> m_pButtons;
	// �E�B���h�E�̕��ƍ���
	int m_windowWidth, m_windowHeight;
	// �O��̃E�B���h�E�̕��ƍ���
	int m_prevWindowWidth, m_prevWindowHeight;
	// ����
	float m_time;
	// �q�b�g�t���O
	bool m_hit;
	// ���̃N���X���L�����ǂ���
	bool m_enable;
	// �{�^���������邩�ǂ���
	bool m_canPress;
	// �q�b�g�t���O��������UI�̃C���f�b�N�X
	int m_menuIndex;
	// �V�[��ID
	SceneID m_num;
	// ���̃X�e�[�W�ԍ�
	int m_stageNum;
	// ���������{�^���̐�
	int m_buttonCount;
	// ��ԊJ�n�ʒu
	std::vector<DirectX::SimpleMath::Vector2> m_prevPositions;
	// �ڕW�ʒu
	std::vector<DirectX::SimpleMath::Vector2> m_targetPositions;
	// �i�s�x
	std::vector<float> m_easeTimers;
};