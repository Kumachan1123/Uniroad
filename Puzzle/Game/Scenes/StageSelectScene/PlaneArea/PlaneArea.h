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
	// ���ʂ̐F��ݒ�
	void SetPlaneColor(const DirectX::SimpleMath::Color& color) { m_debugPlaneVerticesColor.push_back(color); }
	// �w�肳�ꂽ���ʂ̒��S���W���擾����
	DirectX::SimpleMath::Vector3 GetPlanePosition(int index) const;
	// ���ʂ̒��S���W��o�^����
	void AddPlanePosition(const DirectX::SimpleMath::Vector3& position) { m_positions.push_back(position); }
	// �����������ʂ̔ԍ����擾����
	int GetHitPlaneIndex() const { return m_hitPlaneIndex; }
	// �����������ʂ̔ԍ���ݒ肷��
	void SetHitPlaneIndex(int index) { m_hitPlaneIndex = index; }
	// ���炩�̕��ʂƓ������Ă��邩���擾����
	bool IsHitPlane() const { return m_isHitPlane; }
	// ���炩�̕��ʂƓ������Ă��邩��ݒ肷��
	void SetIsHitPlane(bool isHit) { m_isHitPlane = isHit; }

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
	DirectX::SimpleMath::Ray ScreenPointToRay(int mouseX, int mouseY, int screenWidth, int screenHeight);
	// Plane�ƃ��C�̌�������
	bool RayIntersectPlane(int index, const DirectX::SimpleMath::Ray& ray,
		const DirectX::SimpleMath::Plane& plane,
		const std::vector<DirectX::SimpleMath::Vector3>& rectVertices,
		DirectX::SimpleMath::Vector3& outIntersection);
	// ���`��i�f�o�b�O�j
	void DrawDebugLine(const std::vector<DirectX::SimpleMath::Vector3>& vertices, const DirectX::SimpleMath::Color& color);
private:
	// private�����o�ϐ�
	// ���ʃ��\�[�X�ւ̃|�C���^
	CommonResources* m_pCommonResources;
	// ���_�z��
	std::vector<std::vector<DirectX::SimpleMath::Vector3>> m_debugPlaneVerticesPosition;
	// �F
	std::vector<DirectX::SimpleMath::Color> m_debugPlaneVerticesColor;
	// ���ʂ̒��S���W
	std::vector<DirectX::SimpleMath::Vector3> m_positions;
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
	// �����������ʂ̔ԍ�
	int m_hitPlaneIndex;
	// ���炩�̕��ʂƓ������Ă��邩�̃t���O
	bool m_isHitPlane;

};