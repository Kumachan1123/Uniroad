/*
*	@file Particle.cpp
*	@brief �p�[�e�B�N���N���X�̎���
*/
#include "pch.h"
#include "Particle.h"
// �C���v�b�g���C�A�E�g
const std::vector<D3D11_INPUT_ELEMENT_DESC> Particle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,sizeof(DirectX::SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,0, sizeof(DirectX::SimpleMath::Vector3) + sizeof(DirectX::SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
/*
*	@brief	�R���X�g���N�^
*	@detail �N���X�̐����������s��
*	@param type �p�[�e�B�N���̃^�C�v
*	@param size �p�[�e�B�N���̃T�C�Y
*	@param limit �p�[�e�B�N���̐������̐���(�f�t�H���g��100)
*	@return �Ȃ�
*/
Particle::Particle(Utility::Type type, float size, size_t limit)
	: m_pCommonResources{}// ���ʃ��\�[�X
	, m_timer(0.0f)// �o�ߎ���
	, m_elapsedTime(0.0f)// �t���[������
	, m_pDR{}// �f�o�C�X���\�[�X
	, m_pCBuffer{}// �R���X�^���g�o�b�t�@
	, m_pInputLayout{}// ���̓��C�A�E�g
	, m_pTexture{}// �e�N�X�`��
	, m_pVertexShader{}// ���_�V�F�[�_�[
	, m_pPixelShader{}// �s�N�Z���V�F�[�_�[
	, m_pGeometryShader{}// �W�I���g���V�F�[�_�[
	, m_world{}// ���[���h�s��
	, m_view{}// �r���[�s��
	, m_proj{}// �v���W�F�N�V�����s��
	, m_billboard{}// �r���{�[�h�s��
	, m_type{ type }// �p�[�e�B�N���̃^�C�v
	, m_size{ size }// �p�[�e�B�N���̃T�C�Y(�n�����l�̏\�{�j
	, m_params{}// �p�[�e�B�N���̃p�����[�^�[
	, m_anim{ 0 }// �t���[����
	, m_animTime{ 0.0f }// �A�j���[�V��������
	, m_animSpeed{ 30.0f }// �A�j���[�V�����̑��x
	, m_frameRows{ 1 }// �t���[���̍s��
	, m_frameCols{ 1 }// �t���[���̗�
	, m_isCreate{ true }// �p�[�e�B�N���̐����t���O
	, m_limit{ limit }// �p�[�e�B�N���̐������̐���
	, m_pDrawPolygon{ DrawPolygon::GetInstance() }// �|���S���`��N���X
	, m_pCreateShader{ CreateShader::GetInstance() }// �V�F�[�_�[�쐬�N���X
{
}
/*
*	@brief	�f�X�g���N�^
*	@detail �N���X�̔j���������s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
Particle::~Particle() {/*do nothing.*/ }
/*
*	@brief	������
*	@detail �N���X�̏������������s��
*	@param CommonResources* resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void Particle::Initialize(CommonResources* resources)
{
	// ���ʃ��\�[�X���擾
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X���擾
	m_pDR = m_pCommonResources->GetDeviceResources();

	// �摜�̓ǂݍ���
	switch (m_type)
	{
	case Utility::Type::STEAM:// �����C
		//	�A�j���[�V�����̑��x
		m_animSpeed = 1;
		//	�t���[���̗�
		m_frameCols = 1;
		//	�t���[���̍s��
		m_frameRows = 1;
		// �e�N�X�`���̎擾
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Dust"));
		// �s�N�Z���V�F�[�_�[�̍쐬
		m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Particle/PS_Dust.cso", m_pPixelShader);
		break;
	case Utility::Type::SHINE:// ��
		//	�A�j���[�V�����̑��x
		m_animSpeed = 1;
		//	�t���[���̗�
		m_frameCols = 1;
		//	�t���[���̍s��
		m_frameRows = 1;
		// �e�N�X�`���̎擾
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Shine"));// ���C���e�N�X�`��
		m_pTexture.push_back(m_pCommonResources->GetTextureManager()->GetTexture("Shine_Gradation"));// �T�u�e�N�X�`��
		// �s�N�Z���V�F�[�_�[�̍쐬
		m_pCreateShader->CreatePixelShader(L"Resources/Shaders/Particle/PS_Shine.cso", m_pPixelShader);
		break;
	default:// ����ȊO�̃p�[�e�B�N��
		break;
	}
	// �V�F�[�_�[�쐬�N���X�̏�����
	m_pCreateShader->Initialize(m_pDR->GetD3DDevice(), &INPUT_LAYOUT[0], static_cast<UINT>(INPUT_LAYOUT.size()), m_pInputLayout);
	// �V�F�[�_�[�̍쐬
	CreateShaders();
	// �|���S���`��p
	m_pDrawPolygon->InitializePositionColorTexture(m_pDR);
}
/*
*	@brief �X�V
*	@detail �N���X�̍X�V�������s��
*	@param float elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Particle::Update(float elapsedTime)
{
	// �t���[�����Ԃ��Z�b�g
	m_elapsedTime = elapsedTime;
	// �^�C�}�[���X�V
	m_timer += elapsedTime;
	// �A�j���[�V�����̍X�V
	m_animTime += elapsedTime * m_animSpeed;
	// �A�j���[�V�����^�C�}�[����莞�Ԃ𒴂����烊�Z�b�g
	if (m_animTime >= 2.0f)
	{
		// �t���[�������X�V
		m_anim++;
		// �^�C�}�[�����Z�b�g
		m_animTime = 0.0f;
	}
	// �����\�Ȃ�p�[�e�B�N���𐶐�����
	if (m_isCreate && m_particleUtility.size() <= m_limit)
	{
		// �p�[�e�B�N���̐���
		Utility pU(m_params);
		// ���������p�[�e�B�N�������X�g�ɒǉ�
		m_particleUtility.push_back(pU);
	}
	//// �^�C�}�[�����Z�b�g
	//m_timer = 0.0f;
	//	timer��n���čX�V�������s��
	for (std::list<Utility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// �p�[�e�B�N���̍X�V
		if (!(ite)->Update(elapsedTime))
		{
			// �p�[�e�B�N���̍X�V��false��Ԃ����ꍇ�A�܂胉�C�t��0�ȉ��ɂȂ����ꍇ
			ite = m_particleUtility.erase(ite);
			// �C�e���[�^���X�V
			if (ite == m_particleUtility.end()) break;
		}
	}
}
/*
*	@brief �`��
*	@detail �N���X�̕`�揈�����s��
*	@param const DirectX::SimpleMath::Matrix& view �r���[�s��
*	@param const DirectX::SimpleMath::Matrix& proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void Particle::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �����Ԃ��߂�����`�悵�Ȃ�

	// �J�����̕������擾
	Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	// �J�����̕����𐳋K��
	cameraDir.Normalize();
	// �J�����̑O���ɋ߂����Ƀ\�[�g
	m_particleUtility.sort(
		[&](Utility lhs, Utility  rhs)
		{
			// ���ςŃ\�[�g
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
	// ���_���N���A
	m_vertices.clear();
	// ���X�g�̃p�[�e�B�N����S�ĕ`�悷��
	for (Utility& li : m_particleUtility)
	{
		// ���ς��}�C�i�X�̏ꍇ�̓J�����̌��Ȃ̂ŕ\������K�v�Ȃ�
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) continue;
		// ���_�̍\���̂��쐬
		VertexPositionColorTexture vPCT{};
		// ���W
		vPCT.position = XMFLOAT3(li.GetPosition());
		// �e�N�X�`���̐F
		vPCT.color = XMFLOAT4(li.GetNowColor());
		// ���݂̃e�N�X�`���̃X�P�[����XMFLOAT2��X�ɓ����
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);
		// ���_��ǉ�
		m_vertices.push_back(vPCT);
	}
	// �\������_���Ȃ��ꍇ�͕`����I���
	if (m_vertices.empty())return;
	//	�V�F�[�_�[�ɓn���ǉ��̃o�b�t�@���쐬����B(ConstBuffer�j
	// �r���[�s���]�u���ăZ�b�g����
	m_constantBuffer.matView = view.Transpose();
	// �v���W�F�N�V�����s���]�u���ăZ�b�g����
	m_constantBuffer.matProj = proj.Transpose();
	// �r���{�[�h�s������[���h�s��Ƃ��ăZ�b�g����
	m_constantBuffer.matWorld = m_billboard.Transpose();
	// �F���Z�b�g����
	m_constantBuffer.color = Vector4(1, 1, 1, 1);
	// �t���[�������Z�b�g����
	m_constantBuffer.count = Vector4((float)(m_anim));
	// �s�����Z�b�g����
	m_constantBuffer.height = Vector4((float)(m_frameRows));
	// �񐔂��Z�b�g����
	m_constantBuffer.width = Vector4((float)(m_frameCols));
	// �󂯓n���p�o�b�t�@�̓��e�X�V(ConstBuffer����ID3D11Buffer�ւ̕ϊ��j
	m_pDrawPolygon->UpdateSubResources(m_pCBuffer.Get(), &m_constantBuffer);
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_pCBuffer.Get() };
	// �V�F�[�_�[�Ƀo�b�t�@��n��
	m_pDrawPolygon->SetShaderBuffer(0, 1, cb);
	// �`��O�ݒ�
	m_pDrawPolygon->DrawSetting(
		DrawPolygon::SamplerStates::LINEAR_WRAP,// �T���v���[�X�e�[�g
		DrawPolygon::BlendStates::NONPREMULTIPLIED,// �u�����h�X�e�[�g
		DrawPolygon::RasterizerStates::CULL_NONE,// ���X�^���C�U�[�X�e�[�g
		DrawPolygon::DepthStencilStates::DEPTH_NONE); // �[�x�X�e���V���X�e�[�g
	// �`�揀��
	m_pDrawPolygon->DrawStart(m_pInputLayout.Get(), m_pTexture);
	// �V�F�[�_���Z�b�g����
	m_pDrawPolygon->SetShader(m_shaders, nullptr, 0);
	// �w�肵�����W�𒆐S�ɁA�V�F�[�_���Ŕ|���S���𐶐��E�`�悳����
	m_pDrawPolygon->DrawColorTexture(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	// �V�F�[�_�̓o�^���������Ă���
	m_pDrawPolygon->ReleaseShader();
}
/*
*	@brief �r���{�[�h�s��̍쐬
*	@detail �r���{�[�h�s��̍쐬�������s��
*	@param target �����_
*	@param eye �J�����̈ʒu
*	@param up �J�����̏����
*	@return �Ȃ�
*/
void Particle::CreateBillboard(const DirectX::SimpleMath::Vector3& target, const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& up)
{
	// ���O��Ԃ̎g�p
	using namespace DirectX::SimpleMath;
	// �r���{�[�h�s��̍쐬
	m_billboard = Matrix::CreateBillboard(Vector3::Zero, eye - target, up);
	// �P�ʍs��
	Matrix rot = Matrix::Identity;
	// X�����]
	rot._11 = -1;
	// Z�����]
	rot._33 = -1;
	// �J�����̈ʒu
	m_cameraPosition = eye;
	// �J�����̒����_
	m_cameraTarget = target;
	// �r���{�[�h�s��̔��]
	m_billboard = rot * m_billboard;
}
/*
*	@brief	�V�F�[�_�[�̍쐬
*	@detail �V�F�[�_�[�̍쐬�������s��(�s�N�Z���V�F�[�_�[�͎��O�Ƀ^�C�v���Ƃɕ��򂵂č쐬����j
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Particle::CreateShaders()
{
	// ���_�V�F�[�_�[�̍쐬
	m_pCreateShader->CreateVertexShader(L"Resources/Shaders/Particle/VS_Particle.cso", m_pVertexShader);
	// �W�I���g���V�F�[�_�[�̍쐬
	m_pCreateShader->CreateGeometryShader(L"Resources/Shaders/Particle/GS_Particle.cso", m_pGeometryShader);

	// �C���v�b�g���C�A�E�g���󂯎��
	m_pInputLayout = m_pCreateShader->GetInputLayout();
	// �萔�o�b�t�@�쐬
	m_pCreateShader->CreateConstantBuffer(m_pCBuffer, sizeof(ParticleBuffer));
	// �V�F�[�_�[�̍\���̂ɒ��_�V�F�[�_�[���Z�b�g����
	m_shaders.vs = m_pVertexShader.Get();
	// �V�F�[�_�[�̍\���̂Ƀs�N�Z���V�F�[�_�[���Z�b�g����
	m_shaders.ps = m_pPixelShader.Get();
	// �V�F�[�_�[�̍\���̂ɃW�I���g���V�F�[�_�[���Z�b�g����
	m_shaders.gs = m_pGeometryShader.Get();
}

