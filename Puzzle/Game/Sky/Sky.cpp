/*
*	@file Sky.cpp
*	@brief ��̕`����s���N���X
*/
#include "pch.h"
#include "Sky.h"
/*
*	@brief �R���X�g���N�^
*	@details ��̕`����s���N���X�̃R���X�g���N�^
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
Sky::Sky(CommonResources* resources)
	: m_pCommonResources(resources) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pModel(nullptr) // ���f���ւ̃|�C���^
	, m_position(0.0f, 40.0f, 0.0f) // ��̈ʒu
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ��̉�]
	, m_world(DirectX::SimpleMath::Matrix::Identity) // ���[���h�s��
	, m_view(DirectX::SimpleMath::Matrix::Identity) // �r���[�}�g���b�N�X
	, m_proj(DirectX::SimpleMath::Matrix::Identity) // �v���W�F�N�V�����}�g���b�N�X
	, m_time(0.0f)// ����
{
}
/*
*	@brief �f�X�g���N�^
*	@details ��̕`����s���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Sky::~Sky()
{
}
/*
*	@brief ������
*	@details ��̕`����s���N���X�̏��������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Sky::Initialize()
{

	const auto device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();
	// ���f����ǂݍ���
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Sky");
	// �V�F�[�_��ǂݍ��ނ��߂̕ϐ�
	std::vector<uint8_t> blob;
	// ���_�V�F�[�_�����[�h����
	blob = DX::ReadData(L"Resources/Shaders/Sky/VS_Sky.cso");
	DX::ThrowIfFailed(
		device->CreateVertexShader(blob.data(), blob.size(), nullptr, m_vs.ReleaseAndGetAddressOf())
	);
	// ���C�g�p�̃s�N�Z���V�F�[�_�����[�h����
	blob = DX::ReadData(L"Resources/Shaders/Sky/PS_Sky.cso");
	DX::ThrowIfFailed(
		device->CreatePixelShader(blob.data(), blob.size(), nullptr, m_ps.ReleaseAndGetAddressOf())
	);

}
/*
*	@brief �X�V
*	@details ��̕`����s���N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Sky::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// ���Ԍo�߂ŉ�]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_time, 0.0f, 0.0f);
	// ���[���h�s����X�V
	m_world = Matrix::CreateScale(0.2f) * Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);
}
/*
*	@brief �`��
*	@details ��̕`����s��
*	@param view �r���[�}�g���b�N�X
*	@param proj �v���W�F�N�V�����}�g���b�N�X
*	@return �Ȃ�
*/
void Sky::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	const auto states = m_pCommonResources->GetCommonStates();
	m_pModel->Draw(context, *states, m_world, view, proj, false, [&]
		{
			// �V�F�[�_��ݒ肷��
			context->VSSetShader(m_vs.Get(), nullptr, 0);
			context->PSSetShader(m_ps.Get(), nullptr, 0);
		});
}


void Sky::Finalize()
{
}
