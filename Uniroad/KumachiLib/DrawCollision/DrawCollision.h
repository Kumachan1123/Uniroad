/*
	@file	DrawCollision.h
	@brief	�����蔻��`��p�N���X
*/
#pragma once
#include <Libraries/Microsoft/DebugDraw.h>
#include <DeviceResources.h>
#include "Game/CommonResources/CommonResources.h"
//�O���錾
class CommonResources;

class DrawCollision
{
public:

	// ������
	static void Initialize(CommonResources* commonResources);
	// �`��J�n
	static void DrawStart(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// ���E���`��
	static void DrawBoundingSphere(DirectX::BoundingSphere& bs, DirectX::XMVECTOR color);
	// ���E�{�b�N�X�`��
	static void DrawBoundingBox(DirectX::BoundingBox& bb, DirectX::XMVECTOR color);
	// �`��I��
	static void DrawEnd();
	// ���
	static void Finalize();

private:
	// �x�[�V�b�N�G�t�F�N�g
	static std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// ���̓��C�A�E�g
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	// �v���~�e�B�u�o�b�`
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
	// ���ʃ��\�[�X
	static CommonResources* m_commonResources;
};