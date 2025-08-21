/*
*	@file Medal.cpp
*	@brief ���_���N���X
*/
#include "pch.h"
#include "Medal.h"
/*
*	@brief �R���X�g���N�^
*	@details ���_���N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Medal::Medal()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_pCamera(nullptr) // �J�����ւ̃|�C���^
	, m_pParticle(nullptr) // �p�[�e�B�N���V�X�e���ւ̃|�C���^
	, m_pMiniCharacter(nullptr) // �~�j�L�����N�^�[�ւ̃|�C���^
	, m_itemInfo() // �A�C�e�����
	, m_row(-1) // �s�ԍ��i�ۑ��p�j
	, m_col(-1) // ��ԍ��i�ۑ��p�j
	, m_pModel(nullptr) // ���f���ւ̃|�C���^
	, m_time(0.0f) // �o�ߎ���
	, m_deleteTime(0.0f) // ���_�����l������ď�����܂ł̎���
	, m_rotationSpeed(0.0f) // ��]���x
	, m_rotation(DirectX::SimpleMath::Quaternion::Identity) // ��]
	, m_isCollected(false) // ���_�����l�����ꂽ���ǂ���
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���_���N���X�̃f�X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
Medal::~Medal()
{
	// �������Ȃ�
}
/*
*	@brief ������
*	@details ���_���N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param info �A�C�e�����
*	@return �Ȃ�
*/
void Medal::Initialize(CommonResources* resources, const ItemInfo& info)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ݒ�
	m_pCommonResources = resources;
	// �A�C�e������ݒ�
	m_itemInfo = info;
	// �����̉�]���x��ݒ�
	m_rotationSpeed = DEFAULT_ROTATION_SPEED;
	// �p�[�e�B�N�����쐬����
	m_pParticle = std::make_unique<Particle>(Utility::Type::SHINE, 50);
	// �p�[�e�B�N��������������
	m_pParticle->Initialize(m_pCommonResources);
	// �e���쐬����
	m_pShadow = std::make_unique<Shadow>();
	// �e�Ƀ��f����n��
	m_pShadow->SetModel(m_pModel);
	// �e��������
	m_pShadow->Initialize(m_pCommonResources);
}
/*
*	@brief �X�V
*	@details ���_���̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Medal::Update(float elapsedTime)
{
	// DirectX::SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// ���Ԃ����Z
	m_time += elapsedTime;
	// �l�����ꂽ��
	if (m_isCollected)
	{
		// ������܂ł̎��Ԃ����Z
		m_deleteTime += elapsedTime;
		// Y���W�����炩�ɕς���
		m_position.y += Easing::EaseInCirc(m_deleteTime / 3.0f);
		// �l�����ꂽ�������܂ł̎��Ԃ�2�b�𒴂�����
		if (m_deleteTime > 2.0f)
		{
			// �A�C�e�����폜
			OnDiscard(m_pMiniCharacter);
			// �������I����
			return;
		}
	}
	// ���Ԍo�߂ŉ�]������
	m_rotation = Quaternion::CreateFromYawPitchRoll(m_time * DirectX::XM_PI * 2.0f / 5.0f * m_rotationSpeed, 0.0f, 0.0f);
	// �p�[�e�B�N���̍X�V
	m_pParticle->SetParams(SetParticleParams());
	m_pParticle->Update(elapsedTime);
}
/*
*	@brief �����蔻��`��
*	@details ���_���̓����蔻���`�悷��
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void Medal::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectX::SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// ���f�����ݒ肳��Ă��Ȃ��ꍇ�͉������Ȃ�
	if (m_pModel == nullptr) return;
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ���ʂ̃X�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// ���[���h�s���ݒ�
	m_worldMatrix = Matrix::CreateScale(m_itemInfo.scale) * Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);
	// �p�[�e�B�N���̃r���{�[�h�s����쐬
	m_pParticle->CreateBillboard(GetCamera()->GetTargetPosition(), GetCamera()->GetEyePosition(), GetCamera()->GetUpPosition());
	// �p�[�e�B�N���̕`��
	m_pParticle->Render(view, proj);
	// �e�p�ɍ��W���`
	Vector3 shadowPosition = m_position + Vector3(0.0f, -2.99f, 0.0f);
	// �e�̕`��
	if (!m_isCollected)m_pShadow->RenderCircleShadow(view, proj, shadowPosition, 1.0f);
	// ���f���̕`��
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);

}
/*
*	@brief �A�C�e���̎擾������
*	@details ���_�����擾�����Ƃ��A��������������Z����
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void Medal::OnGet(MiniCharacter* miniCharacter)
{
	// ���Ɋl������Ă���ꍇ�͉������Ȃ�
	if (m_isCollected) return;
	// �~�j�L�����N�^�[�̃��_�����������Z
	miniCharacter->GetParent()->GetCSVItem()->CountMedals();
	// ��]���x���l�����̑��x�ɐݒ�
	m_rotationSpeed = COLLECTED_ROTATION_SPEED;
	// ���_�����l�����ꂽ�t���O��ݒ�
	m_isCollected = true;
	// �|�C���^�[��ݒ�
	m_pMiniCharacter = miniCharacter;
}
/*
*	@brief �A�C�e���̎g�p������
*	@details ���_�����g�p�����Ƃ��̏������s��
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void Medal::OnUse(MiniCharacter* miniCharacter)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(miniCharacter);
}
/*
*	@brief �A�C�e���̔j��������
*	@details ���_����j�������Ƃ��̏������s��
*	@param miniCharacter �~�j�L�����N�^�[�ւ̃|�C���^
*	@return �Ȃ�
*/
void Medal::OnDiscard(MiniCharacter* miniCharacter)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(miniCharacter);
	// �A�C�e�����폜
	m_pMiniCharacter->GetParent()->GetCSVItem()->RemoveItem(m_row, m_col);
}
/*
*	@brief �p�[�e�B�N���̃p�����[�^�[��ݒ肷��
*	@details ���_���̃p�[�e�B�N���̃p�����[�^�[��ݒ肷��
*	@param �Ȃ�
*	@return �p�[�e�B�N���̃p�����[�^�[
*/
Utility::ParticleParams Medal::SetParticleParams() const
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �����̐ݒ�
	std::random_device seed;
	// �����Z���k�E�c�C�X�^�@
	std::default_random_engine engine(seed());
	// �����_���Ȋp�x
	std::uniform_real_distribution<> angleDist(0, XM_2PI);
	// �����_���ȑ��x�͈̔͂�ݒ�
	std::uniform_real_distribution<> speedDist(0.5f, 2.0f);
	// XY���ʏ�̃����_���Ȋp�x
	float randAngleXY = static_cast<float>(angleDist(engine));
	// XZ���ʏ�̃����_���Ȋp�x
	float randAngleXZ = static_cast<float>(angleDist(engine));
	// �����_���ȑ��x
	float speed = static_cast<float>(speedDist(engine));
	// �����_���ȕ����̑��x�x�N�g��
	Vector3 randomVelocity = speed * Vector3(
		cosf(randAngleXY) * sinf(randAngleXZ),	 // X����s
		0.5f,									 // Y����
		sinf(randAngleXY) * sinf(randAngleXZ)	 // Z����
	);
	// �p�[�e�B�N���̃p�����[�^�[��ݒ�
	Utility::ParticleParams params{};
	params.life = 1.0f;
	params.pos = m_position;
	params.velocity = randomVelocity;
	params.accele = Vector3(0.0f, 0.0f, 0.0f);// �����x
	params.rotateAccele = Vector3::One; // ��]�����x
	params.rotate = Vector3(0.0f, 0.0f, 0.0f); // ������]
	params.startScale = Vector3(1.0f, 1.0f, 0.0f); // �����X�P�[��
	params.endScale = Vector3(0.01f, 0.01f, 0.0f); // �ŏI�X�P�[���i�������Ȃ�j
	params.startColor = Vector4(0, 1, 1, 0.75); // �����J���[�i���j
	params.endColor = Vector4(0, 0.25, 0.75, 0); // �ŏI�J���[�i���������j
	params.type = Utility::Type::SHINE; // �p�[�e�B�N���̃^�C�v
	return params;
}
