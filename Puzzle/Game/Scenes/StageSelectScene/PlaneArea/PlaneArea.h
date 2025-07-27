/*
*	@file PlaneArea.h
*	@brief ���ʃG���A�N���X
*/
#pragma once
// �W�����C�u����
#include <cassert>
#include <memory>
// DirectX
#include <DeviceResources.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <DirectXColors.h>
// �O�����C�u����
#include <Libraries/MyLib/DebugCamera.h>
#include <Libraries/MyLib/DebugString.h>
#include <Libraries/MyLib/GridFloor.h>
#include <Libraries/MyLib/InputManager.h>
#include <Libraries/MyLib/MemoryLeakDetector.h>
// ����w�b�_�[�t�@�C��
#include "Game/SceneManager/IScene.h"
#include "Game/CommonResources/CommonResources.h"

// �O���錾
class CommonResources;

// ���ʃG���A�N���X
class PlaneArea
{
public:
	// �A�N�Z�T
	// �r���[�s���ݒ�
	void SetView(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
	// �ˉe�s���ݒ�
	void SetProjection(const DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }
	// ���ʂ̒��_�z���z��ɓo�^
	void AddPlane(const std::vector<DirectX::SimpleMath::Vector3>& vertices) { m_debugPlaneVerticesPosition.push_back(vertices); }
public:
	// public�֐�
	// �R���X�g���N�^
	PlaneArea(CommonResources* resources);
	// �f�X�g���N�^
	~PlaneArea();
	// ������
	void Initialize();
	// �X�V
	void Update(float elapsedTime);
	// �`��
	void Render();
	// �I������
	void Finalize();
private:
	// private�֐�
	// �}�E�X���W���烏�[���h���C�𐶐�
	DirectX::SimpleMath::Ray ScreenPointToRay(
		int mouseX, int mouseY,
		int screenWidth, int screenHeight);
	// Plane�ƃ��C�̌�������
	bool RayIntersectPlane(
		const DirectX::SimpleMath::Ray& ray,
		const DirectX::SimpleMath::Plane& plane,
		DirectX::SimpleMath::Vector3& outIntersection);
	bool RayIntersectPlane(const DirectX::SimpleMath::Ray& ray,
		const DirectX::SimpleMath::Plane& plane,
		const std::vector<DirectX::SimpleMath::Vector3>& rectVertices,
		DirectX::SimpleMath::Vector3& outIntersection);
	// ���`��i�f�o�b�O�j
	void DrawDebugLine(const std::vector<DirectX::SimpleMath::Vector3>& vertices);
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// ���_�z��
	//DirectX::SimpleMath::Vector3 m_debugPlaneVerticesPosition[4];
	std::vector<std::vector<DirectX::SimpleMath::Vector3>> m_debugPlaneVerticesPosition;
	// �F
	DirectX::SimpleMath::Color m_color;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_projection;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_pBasicEffect;
	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_pBatch;
	// �C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

};