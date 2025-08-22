#include "pch.h"
#include "DrawCollision.h"


// �ÓI�����o�[�̏�����
std::unique_ptr<DirectX::BasicEffect> DrawCollision::m_basicEffect = nullptr;
Microsoft::WRL::ComPtr<ID3D11InputLayout> DrawCollision::m_pInputLayout = nullptr;
std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> DrawCollision::m_primitiveBatch = nullptr;
CommonResources* DrawCollision::m_commonResources = nullptr;

// ������
void DrawCollision::Initialize(CommonResources* commonResources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X��ݒ�
	// �f�o�C�X�ƃR���e�L�X�g���擾
	m_commonResources = commonResources;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();

	// �x�[�V�b�N�G�t�F�N�g���쐬����
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetVertexColorEnabled(true);

	// ���̓��C�A�E�g���쐬����
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionColor>(
			device,
			m_basicEffect.get(),
			m_pInputLayout.ReleaseAndGetAddressOf()
		)
	);
	// �v���~�e�B�u�o�b�`���쐬����
	m_primitiveBatch = std::make_unique<DirectX::DX11::PrimitiveBatch<DirectX::DX11::VertexPositionColor>>(context);
}

void DrawCollision::DrawStart(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{

	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();
	// �`�悷��
	// �e�p�����[�^��ݒ肷��
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullNone());
	context->IASetInputLayout(m_pInputLayout.Get());
	//** �f�o�b�O�h���[�ł́A���[���h�ϊ�����Ȃ�
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);
	m_basicEffect->Apply(context);
	m_primitiveBatch->Begin();

}

void DrawCollision::DrawBoundingSphere(DirectX::BoundingSphere& bs, DirectX::XMVECTOR color)
{
	// ���E����`��
	DX::Draw(m_primitiveBatch.get(), bs, color);
}

void DrawCollision::DrawBoundingBox(DirectX::BoundingBox& bb, DirectX::XMVECTOR color)
{
	// ���E�{�b�N�X��`��
	DX::Draw(m_primitiveBatch.get(), bb, color);
}

void DrawCollision::DrawEnd()
{
	m_primitiveBatch->End();
}


void DrawCollision::Finalize()
{
	// �ÓI�����o�[�����
	m_basicEffect.reset();
	m_pInputLayout.Reset();
	m_primitiveBatch.reset();
	m_commonResources = nullptr;
}
