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
	, m_position(DirectX::SimpleMath::Vector3(0.0f, 40.0f, 0.0f)) // ��̈ʒu
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ��̉�]
	, m_scale(DirectX::SimpleMath::Vector3::One) // ��̃X�P�[��
	, m_world(DirectX::SimpleMath::Matrix::Identity) // ���[���h�s��
	, m_view(DirectX::SimpleMath::Matrix::Identity) // �r���[�}�g���b�N�X
	, m_proj(DirectX::SimpleMath::Matrix::Identity) // �v���W�F�N�V�����}�g���b�N�X
	, m_time(0.0f)// ����
	, m_rotationSpeed(0.1f) // ��]���x
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
	// �㏈�����s��
	Finalize();
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
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// ���Ԍo�߂ŉ�]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_time * m_rotationSpeed, 0.0f, 0.0f);
	// ���[���h�s����X�V
	m_world = Matrix::CreateScale(m_scale) * Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);
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
	// �f�o�C�X�R���e�L�X�g���擾
	const auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// �R�����X�e�[�g���擾
	const auto states = m_pCommonResources->GetCommonStates();
	// ���f����`��
	m_pModel->Draw(context, *states, m_world, view, proj, false, [&]
		{
			// ���_�V�F�[�_��ݒ肷��
			context->VSSetShader(m_vs.Get(), nullptr, 0);
			// �s�N�Z���V�F�[�_��ݒ肷��
			context->PSSetShader(m_ps.Get(), nullptr, 0);
		});
}
/*
*	@brief �I��
*	@details ��̕`����s���N���X�̏I���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Sky::Finalize()
{
	// ���f�������݂���ꍇ�͉������
	if (m_pModel) m_pModel = nullptr;
	// �V�F�[�_�[���������
	m_vs.Reset();
	m_ps.Reset();
	// ���\�[�X���������
	m_pCommonResources = nullptr;
}
