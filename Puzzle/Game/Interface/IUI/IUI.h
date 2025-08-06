/*
*	@file		IUI.h
*	@brief	�@UI�̃C���^�[�t�F�[�X�N���X
*/
#pragma once
// �W�����C�u����
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "KumachiLib/Anchor/Anchor.h"
// �O���錾
class CommonResources;

// UI�̃C���^�[�t�F�[�X�N���X
class IUI
{
public:
	// �񋓌^
	//UI�̎��
	enum class UIType
	{
		TILE,	// �^�C��
		ITEM,	// �A�C�e��
		BUTTON,	// �{�^��

	};
public:
	// public�֐�
	// �f�X�g���N�^
	virtual ~IUI() = default;
	// ������
	virtual void Initialize(CommonResources* resources, int width, int height) = 0;
	// �X�V
	virtual void Update(const float elapsedTime) = 0;
	// �`��
	virtual void Render() = 0;
	// UI�ǉ�	
	virtual void Add(
		const std::string& key					// �e�N�X�`���̃L�[
		, const DirectX::SimpleMath::Vector2& position	// �ʒu
		, const DirectX::SimpleMath::Vector2& scale	// �X�P�[��
		, KumachiLib::ANCHOR anchor				// �A���J�[
		, UIType type) = 0;			// UI�̎��	
};