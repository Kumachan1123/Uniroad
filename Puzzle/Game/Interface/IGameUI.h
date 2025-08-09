/*
*	@file IGameUI.h
*	@brief �Q�[��UI�̃C���^�[�t�F�[�X�N���X
*/
#pragma once
// �W�����C�u����
#include <memory>
#include <string>
// DirectX
#include <SimpleMath.h>
// ����w�b�_�[�t�@�C��
#include "KumachiLib/Anchor/Anchor.h"
#include "Game/Interface/IUI.h"

// �O���錾
class CommonResources;

// �Q�[��UI�̃C���^�[�t�F�[�X�N���X
class IGameUI : public IUI
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
	virtual ~IGameUI() = default;
	// UI�ǉ�	
	virtual void Add(
		const std::string& key					// �e�N�X�`���̃L�[
		, const DirectX::SimpleMath::Vector2& position	// �ʒu
		, const DirectX::SimpleMath::Vector2& scale	// �X�P�[��
		, KumachiLib::ANCHOR anchor				// �A���J�[
		, UIType type) = 0;			// UI�̎��	
};