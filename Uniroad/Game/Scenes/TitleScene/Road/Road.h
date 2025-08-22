/*
*	@file Road.h
*	@brief ���H�̕`����s���N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <vector>
#include <memory>
// DirectX
#include <DeviceResources.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
#include <Libraries/Microsoft/ReadData.h>
// ����w�b�_�[�t�@�C��
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/ShadowMapLight/ShadowMapLight.h"

// �O���錾
class CommonResources;

// ���H�̕`����s���N���X
class Road
{
public:
	// �A�N�Z�T
	// �V���h�E�}�b�v���C�g���擾����
	ShadowMapLight* GetShadowMapLight() const { return m_pShadowMapLight; }
	// �V���h�E�}�b�v���C�g��ݒ肷��
	void SetShadowMapLight(ShadowMapLight* shadowMapLight) { m_pShadowMapLight = shadowMapLight; }
public:
	// public�֐�
	// �R���X�g���N�^
	Road(CommonResources* resources);
	// �f�X�g���N�^
	~Road();
	// ������
	void Initialize();
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// �I��
	void Finalize();
private:
	// private�֐�
	// �[�x�X�e���V���o�b�t�@��ݒ肷��
	void CreateDepthStencilBuffer(ID3D11Device* pDevice);
private:
	// private�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// �V���h�E�}�b�v���C�g
	ShadowMapLight* m_pShadowMapLight;
	// �[�x�X�e���V���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	// ���H�̃��f���ւ̃|�C���^
	std::vector<DirectX::Model*> m_pModels;
	// ���H�̈ʒu
	std::vector<DirectX::SimpleMath::Vector3> m_positions;
	// ���H�̉�]�p
	DirectX::SimpleMath::Quaternion m_rotation;
	// ���H�̃��[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�}�g���b�N�X
	DirectX::SimpleMath::Matrix m_view;
	// �v���W�F�N�V�����}�g���b�N�X
	DirectX::SimpleMath::Matrix m_proj;
	// ����
	float m_time;
	// �X�N���[�����x
	float m_scrollSpeed;
};