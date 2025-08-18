/*
*	@file MiniCharacterSelectStage.cpp
*	@brief �~�j�L�����N�^�[�̏������`����N���X
*	@details �~�j�L�����N�^�[�̈ʒu�A�p�x�A���x�Ȃǂ��Ǘ����A�^�C���̃C�x���g����������B
*/
#include <pch.h>
#include "MiniCharacterSelectStage.h"
// �m�[�h�J�E���^�[
int MiniCharacterSelectStage::s_nodeCount = 0;
// ���i�J�E���^�[
int MiniCharacterSelectStage::s_partsNumber = 0;
/*
*	@brief �R���X�g���N�^
*	@details �~�j�L�����N�^�[�̏����ʒu�Ɗp�x��ݒ肵�A�K�v�ȕϐ�������������B
*	@param parent �e�R���|�[�l���g
*	@param initialPosition �����ʒu
*	@param initialAngle �����p�x�i���W�A���j
*	@return �Ȃ�
*/
MiniCharacterSelectStage::MiniCharacterSelectStage(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_parent(parent)// �e�R���|�[�l���g
	, m_nodeNumber(MiniCharacterSelectStage::GetNodeCountAfterCountUp())// �m�[�h�ԍ�
	, m_partNumber(MiniCharacterSelectStage::GetPartsNumber())// ���i�ԍ�
	, m_partID(MiniCharacterSelectStage::MINICHARACTER)// ���iID
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
{
}
/*
*	@brief �f�X�g���N�^
*	@details �~�j�L�����N�^�[�̌㏈�����s���B
*	@param �Ȃ�
*	@return �Ȃ�
*/
MiniCharacterSelectStage::~MiniCharacterSelectStage()
{
	// �~�j�L�����N�^�[�̌㏈�����s��
	Finalize();
}
/*
*	@brief �~�j�L�����N�^�[�̏��������s��
*	@details �~�j�L�����N�^�[�̏��������s���A�K�v�ȃ��\�[�X��ݒ肷��B
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void MiniCharacterSelectStage::Initialize(CommonResources* resources)
{
	// SimpleMath�̖��O��Ԃ��g�����߂�using�錾��ǉ�
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X�����݂��邱�Ƃ��m�F����
	assert(resources);
	// ���ʃ��\�[�X��ݒ肷��
	m_pCommonResources = resources;
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
*	@brief �v���C���[�̈ʒu�Ɗp�x���X�V����
*	@details �v���C���[�̈ʒu�Ɗp�x���X�V����B
*	@param elapsedTime �o�ߎ���
*	@param currentPosition ���݂̈ʒu
*	@param currentAngle ���݂̊p�x
*	@return �Ȃ�
*/
void MiniCharacterSelectStage::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
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
	// �ړI�n��ݒ�
	if (parent->GetPlaneArea()->GetHitPlaneIndex() > -1 && parent->GetPlaneArea()->IsHitPlane())
		m_destinationPosition = parent->GetPlaneArea()->GetPlanePosition(parent->GetPlaneArea()->GetHitPlaneIndex());
	// �ړI�n�ɂނ����đ��x���X�V����
	m_currentVelocity = (m_destinationPosition - m_currentPosition) * elapsedTime * 3.0f;
	m_currentVelocity.y = 0.0f;
	// �ړ����Ȃ�p�[�e�B�N���̐������ĊJ����
	if (m_isMoving && m_currentVelocity.LengthSquared() > 0.0001f)m_pParticle->Start();
	// �v���C���[�̈ʒu���X�V����
	m_currentPosition += m_currentVelocity;
	m_currentPosition.y = m_initialPosition.y;
	// �e�ɃJ�����ɓn�����߂̍��W��n��
	parent->SetCameraPosition(m_currentPosition);
	// ���i���X�V����@
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	// �p�[�e�B�N���̍X�V
	m_pParticle->SetParams(SetParticleParams());
	m_pParticle->Update(elapsedTime);
}
/*
*	@brief �v���C���[�̕��i��ǉ�����
*	@details �v���C���[�̕��i��ǉ�����B
*	@param MiniCharacterPart �ǉ����镔�i
*	@return �Ȃ�
*/
void MiniCharacterSelectStage::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// ���i�ԍ����擾
	MiniCharacterPart->Initialize(m_pCommonResources);
	// ���i�ԍ���ݒ�
	m_pMiniCharacterParts.push_back(std::move(MiniCharacterPart));
}

/*
*	@brief �v���C���[�̕��i���폜����
*	@details �v���C���[�̕��i���폜����(���͉������Ȃ�)
*	@param MiniCharacterPart �폜���镔�i
*	@return �Ȃ�
*/
void MiniCharacterSelectStage::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// ���i���폜���鏈���͍��͉������Ȃ�
}
/*
*	@brief �v���C���[��`�悷��
*	@details �v���C���[��`�悷��B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void MiniCharacterSelectStage::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// ���i��`�悷��
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)MiniCharacterPart->Render(view, proj);
	// �e��`�悷��
	m_pShadow->Render(view, proj, m_currentPosition, 1.0f);
	// �e�R���|�[�l���g�̃|�C���^�[�ɕϊ�
	auto parent = dynamic_cast<MiniCharacterBase*>(m_parent);
	// �O�Ղ̃r���{�[�h�s����쐬
	m_pParticle->CreateBillboard(parent->GetCamera()->GetTargetPosition(), parent->GetCamera()->GetEyePosition(), parent->GetCamera()->GetUpPosition());
	// �O�Օ`��
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
	// �ړ�����������
	debugString->AddString("Is Moving: %s", m_isMoving ? "true" : "false");
#endif // DEBUG



}
/*
*	@brief �v���C���[�̌㏈�����s��
*	@details �v���C���[�̌㏈�����s��(���͉������Ȃ�)�B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MiniCharacterSelectStage::Finalize()
{
	// �������Ȃ�
}

/*
*	@brief �v���C���[�̉�]���Ԃ���
*	@details �v���C���[�̉�]���Ԃ��āA���炩�ȉ�]����������B
*	@param currentAngle ���݂̉�]�p
*	@return �Ȃ�
*
*/
void MiniCharacterSelectStage::InterpolateRotation(const DirectX::SimpleMath::Quaternion& currentAngle)
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
	// ���x���[���̏ꍇ
	else
	{
		// ��]������������
		targetQuat = Quaternion::Identity;
		// �p�[�e�B�N���̐������~�߂�
		m_pParticle->Stop();
	}
	// ���݂̉�]�p���X�V����
	float rotateSpeed = 0.05f;
	// ���炩�ɉ�]�����邽�߂ɁA���݂̉�]�p�ƖڕW��]�p����
	m_rotationMiniCharacterAngle = Quaternion::Slerp(m_rotationMiniCharacterAngle, targetQuat, rotateSpeed);
	// �h�������������]��K�p
	m_currentAngle = currentAngle * m_initialAngle * m_rotationMiniCharacterAngle * m_shakeQuaternion;
}

/*
*	@brief �^�C���̒��S�ɂ��邩�ǂ����𔻒肷��
*	@details �v���C���[�̈ʒu���^�C���̒��S�ɋ߂����ǂ����𔻒肷��B
*	@param charPos �v���C���[�̈ʒu
*	@param tileCenter �^�C���̒��S�ʒu
*	@param epsilon ����̋��e�덷
*	@return �^�C���̒��S�ɂ���ꍇ��true�A�����łȂ��ꍇ��false
*/
bool MiniCharacterSelectStage::IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon) const
{
	//// �^�C���̒��S�ƃv���C���[�̈ʒu�̋������v�Z
	float distance = (charPos - tileCenter).Length();
	//// ���������e�덷�ȉ��ł���΁A�^�C���̒��S�ɂ���Ɣ��f
	return distance < epsilon;
}
/*
*	@brief �p�[�e�B�N���̃p�����[�^�[��ݒ肷��
*	@details �p�[�e�B�N���̃p�����[�^�[��ݒ肷��
*	@param �Ȃ�
*	@return �p�[�e�B�N���̃p�����[�^�[
*/
Utility::ParticleParams MiniCharacterSelectStage::SetParticleParams() const
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
	params.pos = m_currentPosition + Vector3(0.0f, 1.5f, 0.0f);
	params.velocity = randomVelocity;
	params.accele = Vector3(0.0f, 0.0f, 0.0f);// �����x
	params.rotateAccele = Vector3::One; // ��]�����x
	params.rotate = Vector3(0.0f, 0.0f, 0.0f); // ������]
	params.startScale = Vector3(1.0f, 1.0f, 0.0f); // �����X�P�[��
	params.endScale = Vector3(0.01f, 0.01f, 0.0f); // �ŏI�X�P�[���i�������Ȃ�j
	params.startColor = Vector4(1, 1, 1, 1); // �����J���[�i���j
	params.endColor = Vector4(1, 1, 0, 0); // �ŏI�J���[�i���������j
	params.type = Utility::Type::STEAM; // �p�[�e�B�N���̃^�C�v
	return params;
}

