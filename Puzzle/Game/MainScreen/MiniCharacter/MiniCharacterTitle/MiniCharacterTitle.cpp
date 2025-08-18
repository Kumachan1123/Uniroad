/*
*	@file MiniCharacterTitle.cpp
*	@brief �^�C�g���V�[���p�̃~�j�L�������Ǘ�����N���X
*/
#include "pch.h"
#include "MiniCharacterTitle.h"
// �m�[�h�J�E���^�[
int MiniCharacterTitle::s_nodeCount = 0;
// ���i�J�E���^�[
int MiniCharacterTitle::s_partsNumber = 0;

/*
*	@brief �R���X�g���N�^
*	@details �^�C�g���V�[���p�̃~�j�L�����̏����ʒu�Ɗp�x��ݒ肵�A�K�v�ȕϐ�������������B
*	@param parent �e�R���|�[�l���g
*	@param initialPosition �����ʒu
*	@param initialAngle �����p�x�i���W�A���j
*	@return �Ȃ�
*/
MiniCharacterTitle::MiniCharacterTitle(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_parent(parent)// �e�R���|�[�l���g
	, m_nodeNumber(MiniCharacterTitle::GetNodeCountAfterCountUp())// �m�[�h�ԍ�
	, m_partNumber(MiniCharacterTitle::GetPartsNumber())// ���i�ԍ�
	, m_partID(MiniCharacterTitle::MINICHARACTER)// ���iID
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X
	, m_enteredTilePtr(nullptr)// �������^�C���̃|�C���^
	, m_hasEnteredTile(false)// ����������t���O
	, m_isMoving(true)// �ړ��t���O
	, m_fallTimer(0.0f)// �����^�C�}�[
	, m_fallTimerActive(false)// �����^�C�}�[���L�����ǂ���
	, m_hasFallen(false)// ��x�����������������s�����邽�߂̃t���O
	, m_initialPosition(initialPosition)// �����ʒu
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// �����p�x
	, m_currentPosition{}// ���݂̈ʒu
	, m_currentVelocity{}// ���݂̑��x
	, m_currentAngle{}// ���݂̉�]�p
	, m_destinationPosition(initialPosition)// �ړI�n�̈ʒu
	, m_prevTileName("Start")// �O�t���[���ň�ԋ߂������^�C���̖��O
	, m_rotationMiniCharacterAngle{}// �v���C���[��]�p
	, m_mass{}// ����
	, m_miniCharacterVelocity{}// �v���C���[���x
	, m_titleAnimationState(NONE)// �^�C�g���V�[���ɂ�����A�j���[�V�����X�e�[�g
{
}
/*
*	@brief �f�X�g���N�^
*	@details �^�C�g���V�[���p�̃~�j�L�����̌㏈�����s���B
*	@param �Ȃ�
*	@return �Ȃ�
*/
MiniCharacterTitle::~MiniCharacterTitle()
{
	Finalize();
}
/*
*	@brief �^�C�g���V�[���p�̃~�j�L�����̏��������s��
*	@details �^�C�g���V�[���p�̃~�j�L�����̏��������s���A�K�v�ȃ��\�[�X��ݒ肷��B
*	@param commonResources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void MiniCharacterTitle::Initialize(CommonResources* commonResources)
{
	// SimpleMath�̖��O��Ԃ��g�����߂�using�錾��ǉ�
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X�����݂��邱�Ƃ��m�F����
	assert(commonResources);
	// ���ʃ��\�[�X��ݒ肷��
	m_pCommonResources = commonResources;
	// ���݈ʒu�ɔ��f
	m_currentPosition = m_initialPosition;
	// �q�c�W�p�[�c���A�^�b�`
	Attach(std::make_unique<Sheep>(this, Vector3(0.0f, 3.5f, 0.0f), 0.0f));
	// �e���쐬
	m_pShadow = std::make_unique<Shadow>();
	// �e�̏�����
	m_pShadow->Initialize(m_pCommonResources);
	// �p�[�e�B�N�����쐬����
	m_pParticle = std::make_unique<Particle>(Utility::Type::STEAM, 1.0f);
	// �p�[�e�B�N��������������
	m_pParticle->Initialize(m_pCommonResources);
}
/*
*	@brief �^�C�g���V�[���p�̃~�j�L�����̍X�V�������s��
*	@details �^�C�g���V�[���p�̃~�j�L�����̈ʒu��p�x���X�V����B
*	@param elapsedTime �o�ߎ���
*	@param currentPosition ���݂̈ʒu
*	@param currentAngle ���݂̊p�x
*	@return �Ȃ�
*/
void MiniCharacterTitle::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(currentPosition);
	// ��]�̕��
	InterpolateRotation(currentAngle);
	// �e�R���|�[�l���g�̃|�C���^�[�ɕϊ�
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// �e�R���|�[�l���g�����݂��邱�Ƃ��m�F
	assert(parent);
	// �^�C�g���V�[���p�A�j���[�V����
	ExecuteAnimation(elapsedTime);
	// ���i���X�V����@
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	// �p�[�e�B�N���̍X�V

	m_pParticle->SetParams(SetParticleParams());
	m_pParticle->Update(elapsedTime);

}
/*
*	@brief �^�C�g���V�[���p�̃~�j�L�����̒ǉ�����������
*	@details �^�C�g���V�[���p�̃~�j�L�����ɕ��i��ǉ�����B
*	@param MiniCharacterPart �ǉ����镔�i
*	@return �Ȃ�
*/
void MiniCharacterTitle::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// ���i�ԍ����擾
	MiniCharacterPart->Initialize(m_pCommonResources);
	// ���i�ԍ���ݒ�
	m_pMiniCharacterParts.push_back(std::move(MiniCharacterPart));
}
/*
*	@brief �^�C�g���V�[���p�̃~�j�L�����̕��i���폜����
*	@details �^�C�g���V�[���p�̃~�j�L�������畔�i���폜����B
*	@param MiniCharacterPart �폜���镔�i
*	@return �Ȃ�
*/
void MiniCharacterTitle::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// ���i���폜���鏈���͍��͉������Ȃ�
}
/*
*	@brief �^�C�g���V�[���p�̃~�j�L�����̕`�揈�����s��
*	@details �^�C�g���V�[���p�̃~�j�L������`�悷��B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void MiniCharacterTitle::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{

	// ���i��`�悷��
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)MiniCharacterPart->Render(view, proj);
	// �e��`�悷��
	m_pShadow->Render(view, proj, m_currentPosition, 1.0f);
	// �e�R���|�[�l���g�̃|�C���^�[�ɕϊ�
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// �p�[�e�B�N���̃r���{�[�h�s����쐬
	m_pParticle->CreateBillboard(parent->GetCamera()->GetTargetPosition(), parent->GetCamera()->GetEyePosition(), parent->GetCamera()->GetUpPosition());
	// �p�[�e�B�N���`��
	m_pParticle->Render(parent->GetCamera()->GetViewMatrix(), parent->GetCamera()->GetProjectionMatrix());

#ifdef _DEBUG
	// ---�f�o�b�O�\��---
	const auto debugString = m_pCommonResources->GetDebugString();
	// ���W�\��
	debugString->AddString("MiniCharacter Position: (%f, %f, %f)",
		m_currentPosition.x, m_currentPosition.y, m_currentPosition.z);
	// ���x�\��
	debugString->AddString("MiniCharacter Velocity: (%f, %f, %f)",
		m_currentVelocity.x, m_currentVelocity.y, m_currentVelocity.z);
#endif // DEBUG

}
/*
*	@brief �^�C�g���V�[���p�̃~�j�L�����̌㏈�����s��
*	@details �^�C�g���V�[���p�̃~�j�L�����̌㏈�����s���B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MiniCharacterTitle::Finalize()
{
}
/*
*	@brief �^�C�g���V�[���p�̃~�j�L�����̉�]���Ԃ���
*	@details �^�C�g���V�[���p�̃~�j�L�����̉�]���Ԃ���B
*	@param currentAngle ���݂̉�]�p
*	@return �Ȃ�
*/
void MiniCharacterTitle::InterpolateRotation(const DirectX::SimpleMath::Quaternion& currentAngle)
{
	using namespace DirectX::SimpleMath;
	// �ڕW��]���v�Z�i���x�x�N�g������j
	Quaternion targetQuat;


	// ���݂̑��x���[���łȂ��ꍇ�A��]���v�Z
	if (m_currentVelocity.LengthSquared() > 0.0001f)
	{
		// ���݂̑��x�x�N�g�����烈�[�p���v�Z
		float yaw = atan2f(m_currentVelocity.x, m_currentVelocity.z);
		// ���[�p����N�H�[�^�j�I�����쐬
		targetQuat = Quaternion::CreateFromYawPitchRoll(yaw, 0.0f, 0.0f);
	}
	else
	{
		// ���x���[���̏ꍇ�́A��]�Ȃ�
		targetQuat = Quaternion::Identity;
	}
	// ���݂̉�]�p���X�V����
	float rotateSpeed = 0.05f;
	// ���炩�ɉ�]�����邽�߂ɁA���݂̉�]�p�ƖڕW��]�p����
	m_rotationMiniCharacterAngle = Quaternion::Slerp(m_rotationMiniCharacterAngle, targetQuat, rotateSpeed);
	// �h�������������]��K�p
	m_currentAngle = currentAngle * m_initialAngle * m_rotationMiniCharacterAngle * m_shakeQuaternion;
}

/*
*	@brief �^�C���̒��S�ɂ��邩�ǂ������m�F����
*	@details �^�C���̒��S�ɂ��邩�ǂ������m�F����B
*	@param charPos �L�����N�^�[�̈ʒu
*	@param tileCenter �^�C���̒��S�ʒu
*	@param epsilon ���e�덷
*	@return �^�C���̒��S�ɂ���ꍇ��true�A�����łȂ��ꍇ��false
*/
bool MiniCharacterTitle::IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon) const
{
	//// �^�C���̒��S�ƃv���C���[�̈ʒu�̋������v�Z
	float distance = (charPos - tileCenter).Length();
	//// ���������e�덷�ȉ��ł���΁A�^�C���̒��S�ɂ���Ɣ��f
	return distance < epsilon;
}
/*
*	@brief �^�C�g���V�[���p�̃A�j���[�V���������s����
*	@details �^�C�g���V�[���p�̃A�j���[�V���������s����B
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void MiniCharacterTitle::ExecuteAnimation(float elapsedTime)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �ڕW���W��������
	// START�ł����ڕW���W
	Vector3 targetPosition(0.0f, -0.45f, 0.0f);
	// CONTINUE�ł����ڕW���W
	Vector3 goalPosition(10.0f, -0.45f, 0.0f);
	// �A�j���[�V�����̎��s
	switch (GetTitleAnimationState())
	{
	case TitleAnimation::START:
		// �J�n�A�j���[�V�����̏���
		// ���݂̈ʒu��ڕW���W�Ɍ����ĕ��
		m_currentPosition = Vector3::Lerp(m_currentPosition, targetPosition, elapsedTime);
		// �ړI�n�ɓ��B������A�j���[�V������ҋ@��ԂɕύX
		if ((m_currentPosition - targetPosition).Length() < 0.1f)SetTitleAnimationState(WAIT);
		break;
	case TitleAnimation::WAIT:
		// �ҋ@�A�j���[�V�����̏���
		break;
	case TitleAnimation::CONTINUE:
		// �A�j���[�V�����ĊJ�̏���
		// ���݂̈ʒu��ڕW���W�Ɍ����ĕ��
		m_currentPosition = Vector3::Lerp(m_currentPosition, goalPosition, elapsedTime * 4.0f);
		// �ړI�n�ɓ��B������A�j���[�V������ҋ@��ԂɕύX
		if ((m_currentPosition - goalPosition).Length() < 0.5f)
			SetTitleAnimationState(END);
		break;
	case TitleAnimation::END:
		// �A�j���[�V�����I���Ȃ̂ŉ������Ȃ�
		break;
	case TitleAnimation::NONE:
		// �A�j���[�V�������Ȃ��ꍇ�͉������Ȃ�
		return;
	}
}
/*
*	@brief �p�[�e�B�N���p�����[�^��ݒ肷��
*	@details �p�[�e�B�N���p�����[�^��ݒ肷��
*	@param �Ȃ�
*	@return �p�[�e�B�N���̃p�����[�^
*/
Utility::ParticleParams MiniCharacterTitle::SetParticleParams() const
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
		1.0f,									 // Y����
		sinf(randAngleXY) * sinf(randAngleXZ)	 // Z����
	);
	// �p�[�e�B�N���̃p�����[�^�[��ݒ�
	Utility::ParticleParams params{};
	params.life = 1.0f;
	params.pos = m_currentPosition + Vector3(0.0f, 1.0f, 0.0f);
	params.velocity = randomVelocity;
	params.accele = Vector3(-5.0f, 0.0f, 0.0f);// �����x
	params.rotateAccele = Vector3::One; // ��]�����x
	params.rotate = Vector3(0.0f, 0.0f, 0.0f); // ������]
	params.startScale = Vector3(1.0f, 1.0f, 0.0f); // �����X�P�[��
	params.endScale = Vector3(0.01f, 0.01f, 0.0f); // �ŏI�X�P�[���i�������Ȃ�j
	params.startColor = Vector4(1, 1, 1, 1); // �����J���[�i���j
	params.endColor = Vector4(1, 1, 0, 0); // �ŏI�J���[�i���������j
	params.type = Utility::Type::STEAM; // �p�[�e�B�N���̃^�C�v
	return params;
}
/*
*	@brief �^�C�g���V�[���p�̃A�j���[�V������Ԃ��擾����
*	@details �^�C�g���V�[���p�̃A�j���[�V������Ԃ��擾����B
*	@param �Ȃ�
*	@return �^�C�g���A�j���[�V�����̏��
*/
TitleAnimation MiniCharacterTitle::GetTitleAnimationState() const
{
	// �e�R���|�[�l���g�̃|�C���^�[�ɕϊ�
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// �e�R���|�[�l���g�����݂��邱�Ƃ��m�F
	assert(parent);
	// �A�j���[�V������Ԃ�Ԃ�
	return parent->GetTitleAnimationState();

}
/*
*	@brief �^�C�g���V�[���p�̃A�j���[�V������Ԃ�ݒ肷��
*	@details �^�C�g���V�[���p�̃A�j���[�V������Ԃ�ݒ肷��B
*	@param state �^�C�g���A�j���[�V�����̏��
*	@return �Ȃ�
*/
void MiniCharacterTitle::SetTitleAnimationState(TitleAnimation state)
{
	// �e�R���|�[�l���g�̃|�C���^�[�ɕϊ�
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// �e�R���|�[�l���g�����݂��邱�Ƃ��m�F
	assert(parent);
	// �A�j���[�V������Ԃ�ݒ肷��
	parent->SetTitleAnimationState(state);
}