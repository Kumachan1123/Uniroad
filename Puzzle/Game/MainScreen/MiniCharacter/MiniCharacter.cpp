/*
*	@file MiniCharacterSelectStage.cpp
*	@brief �~�j�L�����N�^�[�̏������`����N���X
*	@details �~�j�L�����N�^�[�̈ʒu�A�p�x�A���x�Ȃǂ��Ǘ����A�^�C���̃C�x���g����������B
*/
#include <pch.h>
#include "MiniCharacter.h"
// �m�[�h�J�E���^�[
int MiniCharacter::s_nodeCount = 0;
// ���i�J�E���^�[
int MiniCharacter::s_partsNumber = 0;
// �d�͂��`
const float MiniCharacter::GRAVITY = -9.8f;
/*
*	@brief �R���X�g���N�^
*	@details �~�j�L�����N�^�[�̏����ʒu�Ɗp�x��ݒ肵�A�K�v�ȕϐ�������������B
*	@param parent �e�R���|�[�l���g
*	@param initialPosition �����ʒu
*	@param initialAngle �����p�x�i���W�A���j
*	@return �Ȃ�
*/
MiniCharacter::MiniCharacter(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_parent(parent)// �e�R���|�[�l���g
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())// �m�[�h�ԍ�
	, m_partNumber(MiniCharacter::GetPartsNumber())// ���i�ԍ�
	, m_partID(MiniCharacter::MINICHARACTER)// ���iID
	, m_pCommonResources(nullptr) // ���ʃ��\�[�X
	, m_enteredTilePtr(nullptr)// �������^�C���̃|�C���^
	, m_hasEnteredTile(false)// ����������t���O
	, m_isMoving(true)// �ړ��t���O
	, m_fallTimer(0.0f)// �����^�C�}�[
	, m_gameOverSwitchTime(0.0f)// �Q�[���I�[�o�[�̃X�C�b�`����
	, m_gameClearSwitchTime(0.0f)// �Q�[���N���A�̃X�C�b�`����
	, m_speed(1.0f)// �ړ����x
	, m_fallTimerActive(false)// �����^�C�}�[���L�����ǂ���
	, m_hasFallen(false)// ��x�����������������s�����邽�߂̃t���O
	, m_initialPosition(initialPosition)// �����ʒu
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// �����p�x
	, m_currentPosition{}// ���݂̈ʒu
	, m_currentVelocity{}// ���݂̑��x
	, m_currentAngle{}// ���݂̉�]�p
	, m_prevTileName("StartBlock")// �O�t���[���ň�ԋ߂������^�C���̖��O
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
MiniCharacter::~MiniCharacter()
{
	// �I������
	Finalize();
}
/*
*	@brief �~�j�L�����N�^�[�̏��������s��
*	@details �~�j�L�����N�^�[�̏��������s���A�K�v�ȃ��\�[�X��ݒ肷��B
*	@param resources ���ʃ��\�[�X
*	@return �Ȃ�
*/
void MiniCharacter::Initialize(CommonResources* resources)
{
	// SimpleMath�̖��O��Ԃ��g�����߂�using�錾��ǉ�
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X�����݂��邱�Ƃ��m�F����
	assert(resources);
	// ���ʃ��\�[�X��ݒ肷��
	m_pCommonResources = resources;
	// �����ʒu��ݒ肷��
	m_initialPosition = GetParent()->GetCSVMap()->GetStart().pos;
	// ���݈ʒu�ɔ��f
	m_currentPosition = m_initialPosition;
	// �X�^�[�g�n�_�̑O�㍶�E�̃^�C���𒲂ׂăv���C���[�̑��x���X�V����
	UpdateSpeedByStartTile();
	// �q�c�W�p�[�c���A�^�b�`
	Attach(std::make_unique<Sheep>(this, Vector3(0.0f, 3.5f, 0.0f), 0.0f));
}
/*
*	@brief �v���C���[�̈ʒu�Ɗp�x���X�V����
*	@details �v���C���[�̈ʒu�Ɗp�x���X�V����B
*	@param elapsedTime �o�ߎ���
*	@param currentPosition ���݂̈ʒu
*	@param currentAngle ���݂̊p�x
*	@return �Ȃ�
*/
void MiniCharacter::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// SimpleMath�̖��O��Ԃ��g�����߂�using�錾��ǉ�
	using namespace DirectX::SimpleMath;
	// �^�C���C�x���g���X�V����
	UpdateTileEvents();
	// �����^�C�}�[����
	UpdateFallTimer(elapsedTime);
	// �d�͂������������W�ړ����s��
	Moving(elapsedTime, currentPosition);
	// �h�ꉉ�o
	Shake();
	// ��]�̕��
	InterpolateRotation(elapsedTime, currentAngle);
	// ���ɐ��������^�C�������߂邽�߂ɑ��x�x�N�g����ݒ肷��
	if (m_parent->GetNextTiles() != nullptr && m_currentVelocity != Vector3::Zero)
		m_parent->GetNextTiles()->SetMiniCharacterVelocity(m_currentVelocity);
	// �C�����i���X�V����@
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	// �Q�[���I�[�o�[�A�Q�[���N���A���򏈗�
	HandleGameOverAndClear(elapsedTime);


}
/*
*	@brief �v���C���[�̕��i��ǉ�����
*	@details �v���C���[�̕��i��ǉ�����B
*	@param MiniCharacterPart �ǉ����镔�i
*	@return �Ȃ�
*/
void MiniCharacter::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
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
void MiniCharacter::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
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
void MiniCharacter::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// �C�����i��`�悷��
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)MiniCharacterPart->Render(view, proj);
#ifdef _DEBUG
	// ---�f�o�b�O�\��---
	const auto debugString = m_pCommonResources->GetDebugString();
	// ���W�\��
	debugString->AddString("MiniCharacter Position: (%f, %f, %f)",
		m_currentPosition.x, m_currentPosition.y, m_currentPosition.z);
	// ���x�\��
	debugString->AddString("MiniCharacter Velocity: (%f, %f, %f)",
		m_currentVelocity.x, m_currentVelocity.y, m_currentVelocity.z);
	// �ł��߂��^�C���̖��O��\��
	debugString->AddString("MiniCharacter Tile: %s",
		GetParent()->GetCSVMap()->GetTileData(m_currentPosition).tileInfo.modelName.c_str());
	// ���W�\��
	debugString->AddString("CurrentTile Position: (%f, %f, %f)",
		m_currentTilePosition.x, m_currentTilePosition.y, m_currentTilePosition.z);
	// ���W�\��
	debugString->AddString("PrevTile Position: (%f, %f, %f)",
		m_prevTilePosition.x, m_prevTilePosition.y, m_prevTilePosition.z);
#endif
}
/*
*	@brief �v���C���[�̌㏈�����s��
*	@details �v���C���[�̌㏈�����s��(���͉������Ȃ�)�B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MiniCharacter::Finalize()
{

}
/*
*	@brief �^�C���C�x���g���X�V����
*	@details �^�C���̒��S�ւ̈ړ���^�C���̃C�x���g���X�V����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MiniCharacter::UpdateTileEvents()
{
	// ���݂���^�C���̖��O���擾
	std::string currentTileName = GetParent()->GetCSVMap()->GetTileData(m_currentPosition).tileInfo.modelName;
	// ���݂���^�C���̈ʒu���擾
	m_currentTilePosition = GetParent()->GetCSVMap()->GetTileData(m_currentPosition).pos;

	// ���݂̈ʒu���^�C���̒��S�ɂ��邩�ǂ����𔻒�
	bool isAtTileCenter = IsAtTileCenter(m_currentPosition, GetParent()->GetCSVMap()->GetTileData(m_currentPosition).pos);


	// ������^�C���ƑO�t���[���ł����^�C�����قȂ�ꍇ�A�^�C���̃C�x���g����������
	if (currentTileName != m_prevTileName || m_currentTilePosition != m_prevTilePosition)
	{
		// �O�̃^�C���̃C�x���g������
		const auto& prevTile = GetParent()->GetCSVMap()->GetTileData(m_prevPosition);
		// �O�̃^�C�������݂���ꍇ�AExit�C�x���g���Ăяo��
		if (prevTile.tileBasePtr) prevTile.tileBasePtr->OnExit(this);
		// ���݂̃^�C���̃C�x���g������
		const auto& currentTile = GetParent()->GetCSVMap()->GetTileData(m_currentPosition);
		// ���݂̃^�C�������݂���ꍇ�AEnter�C�x���g���Ăяo��
		if (currentTile.tileBasePtr ||// ���݂̃^�C�������݂���ꍇ
			(m_prevTileName == "" && currentTileName != ""))// �O�̃^�C�����󔒂ŁA���݂̃^�C�������݂���ꍇ
		{
			// Enter�C�x���g���Ăяo��
			currentTile.tileBasePtr->OnEnter(this);
			// ���݂̃^�C�������X�V
			m_prevTileName = currentTileName;
			// �O�̈ʒu���X�V
			m_prevPosition = m_currentPosition;
			// ������^�C���̈ʒu��ۑ�
			m_prevTilePosition = m_currentTilePosition;
		}
	}
	// ���݂̈ʒu���^�C���̒��S�ɂ��邩�ǂ������m�F
	if (isAtTileCenter)
	{
		// �^�C���̒��S�ɂ���ꍇ�A�ړ��t���O���X�V
		const auto& currentTile = GetParent()->GetCSVMap()->GetTileData(m_currentPosition);
		// ���݂̃^�C�������݂���ꍇ�ACenterReached�C�x���g���Ăяo��
		if (currentTile.tileBasePtr)
			currentTile.tileBasePtr->OnCenterReached(this);
		// �s�Ɨ���擾
		int row = GetParent()->GetCSVMap()->GetRowFromPosition(m_currentPosition);
		int col = GetParent()->GetCSVMap()->GetColFromPosition(m_currentPosition);
		// CSV�A�C�e�������݂���ꍇ�A�A�C�e�����擾����
		const auto& csvItem = GetParent()->GetCSVItem();
		if (!csvItem)return;
		// ���̃^�C���ɃA�C�e��������Ȃ�l������
		const auto& item = GetParent()->GetCSVItem()->GetItemData(row, col);
		// �A�C�e�������݂���ꍇ
		if (item.itemInfo.modelName != "" && item.itemBasePtr != nullptr)
		{
			// �A�C�e�����l������
			item.itemBasePtr->OnGet(this);
			// �A�C�e��������
			//GetParent()->GetCSVItem()->RemoveItem(row, col);
		}
	}
	// ���݂̃^�C�������󔒂ł���ꍇ
	else if (currentTileName == "")
	{
		// �󔒃^�C���ɂ���ꍇ�A�ړ��t���O��܂�
		m_isMoving = false;
		// �����^�C�}�[���J�n
		if (!m_fallTimerActive)
		{
			// �����^�C�}�[��L���ɂ���
			m_fallTimerActive = true;
			// �����^�C�}�[�����Z�b�g
			m_fallTimer = 0.0f;
		}
	}
	// ������^�C������i�߂�^�C�������߂邽�߂ɖ��O��ۑ�
	if (m_parent->GetNextTiles() != nullptr && currentTileName != "")
		m_parent->GetNextTiles()->SetMiniCharacterTileName(currentTileName);
}
/*
*	@brief �����^�C�}�[���X�V����
*	@details �����^�C�}�[�X�V�E�����t���O�Ǘ��E���A�Ǘ�����
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void MiniCharacter::UpdateFallTimer(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �����^�C�}�[���L���ŁA�܂��������Ă��Ȃ��ꍇ
	if (m_fallTimerActive && !m_hasFallen)
	{
		// �����^�C�}�[���X�V
		m_fallTimer += elapsedTime;
		// �����^�C�}�[��3�b�𒴂�����A�������������s
		if (m_fallTimer >= 3.0f)
		{
			// ���x��0�ɂ���
			m_currentVelocity = Vector3::Zero;
			// �ړ��t���O��܂�
			m_isMoving = false;
			// �����t���O�𗧂Ă�
			m_hasFallen = true;
		}
	}
	const auto& currentTileName = GetParent()->GetCSVMap()->GetTileData(m_currentPosition).tileInfo.modelName;
	// �����J�E���^�[���ɋ󔒃^�C�����畜�A������A���Z�b�g
	if (m_fallTimerActive && !m_hasFallen && (currentTileName != "" && currentTileName != "Block"))
	{
		// �����^�C�}�[�𖳌��ɂ���
		m_fallTimerActive = false;
		// �����^�C�}�[�����Z�b�g
		m_fallTimer = 0.0f;
		// �ړ��t���O�𗧂Ă�
		m_isMoving = true;
		// �����t���O��܂�
		m_hasFallen = false;
	}
}
/*
*	@brief �v���C���[�ɏd�͂�K�p����
*	@details �d�́E���x�E���W�X�V����
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void MiniCharacter::Moving(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition)
{
	// �������̏ꍇ
	if (m_hasFallen)
	{
		// �������͏d�͂�K�p����
		m_currentVelocity.y += GRAVITY * elapsedTime;
		// ���x�ɓK�p����
		m_miniCharacterVelocity += m_currentVelocity * elapsedTime;
		// �Q�[���I�[�o�[�̃X�C�b�`���Ԃ��X�V
		m_gameOverSwitchTime += elapsedTime;
	}
	// �������Ă��Ȃ��ꍇ
	else if (m_isMoving)
	{
		// �ʏ�ʂ�̈ړ�����
		m_miniCharacterVelocity += m_currentVelocity * elapsedTime / 3 * m_speed;

	}
	// ���W�ɑ��x��K�p����
	m_currentPosition = currentPosition + m_initialPosition + m_miniCharacterVelocity;
}
/*
*	@brief �v���C���[��h�炷
*	@details �h��N�H�[�^�j�I�����v�Z����
*	@return �Ȃ�
*/
void MiniCharacter::Shake()
{
	using namespace DirectX::SimpleMath;
	// �h��N�H�[�^�j�I���i�f�t�H���g�͉�]�Ȃ��j
	m_shakeQuaternion = Quaternion::Identity;
	// �����^�C�}�[���L���ŁA�܂��������Ă��Ȃ��ꍇ
	if (m_fallTimerActive && !m_hasFallen)
	{
		// �h��̋���
		float shakeAmount = 0.075f;
		// �h��̑���
		float shakeSpeed = 7.0f;
		// �����^�C�}�[�̎��Ԃ��擾
		float time = m_fallTimer;
		// �h��̐i�s�x���v�Z�i0.0f����1.0f�͈̔́j
		float progress = std::min(time / 3.0f, 1.0f);
		// �h��̐U�����v�Z�i�i�s�x�ɉ����ĕω��j
		float amp = shakeAmount * (0.5f + 1.0f * progress);
		// �h��̊p�x���v�Z
		float xSwing = sinf(time * shakeSpeed) * amp * (0.8f + 0.4f * sinf(time * 2.0f));
		// z���̗h��͏�������������
		float zSwing = cosf(time * shakeSpeed * 0.7f) * amp * (0.7f + 0.6f * cosf(time * 3.1f));
		// �h��N�H�[�^�j�I�����쐬
		m_shakeQuaternion = Quaternion::CreateFromYawPitchRoll(0.0f, xSwing, zSwing);
	}
}
/*
*	@brief �v���C���[�̉�]���Ԃ���
*	@details �v���C���[�̉�]���Ԃ��āA���炩�ȉ�]����������B
*	@param elapsedTime �o�ߎ���
*	@param currentAngle ���݂̉�]�p
*	@return �Ȃ�
*
*/
void MiniCharacter::InterpolateRotation(float elapsedTime, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	using namespace DirectX::SimpleMath;
	// �ڕW��]���v�Z�i���x�x�N�g������j
	Quaternion targetQuat;
	// ���݂̑��x���[���łȂ��ꍇ�A��]���v�Z
	if (m_currentVelocity.LengthSquared() > 0.0f)
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
	float rotateSpeed = 2.0f;
	// ���炩�ɉ�]�����邽�߂ɁA���݂̉�]�p�ƖڕW��]�p����
	m_rotationMiniCharacterAngle = Quaternion::Slerp(m_rotationMiniCharacterAngle, targetQuat, elapsedTime * rotateSpeed);
	// �h�������������]��K�p
	m_currentAngle = currentAngle * m_initialAngle * m_rotationMiniCharacterAngle * m_shakeQuaternion;
}

/*
*	@brief �v���C���[�̊J�n�^�C���ɂ���đ��x���X�V����
*	@details �X�^�[�g�n�_�̑O�㍶�E�̃^�C���𒲂ׂāA�v���C���[�̑��x���X�V����B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void MiniCharacter::UpdateSpeedByStartTile()
{
	// SimpleMath�̖��O��Ԃ��g�����߂�using�錾��ǉ�
	using namespace DirectX::SimpleMath;
	// �s�����l
	int startRow = -1;
	// �񏉊��l
	int startCol = -1;
	// �X�^�[�g�ʒu��T��
	for (int row = 0; row < GetParent()->GetCSVMap()->GetMaxRow(); ++row)
	{
		for (int col = 0; col < GetParent()->GetCSVMap()->GetMaxCol(); ++col)
		{
			// �^�C���f�[�^���擾
			const auto& tile = GetParent()->GetCSVMap()->GetTileData(row, col);
			// �^�C���̃��f������"Start"�ł��邩�m�F
			if (tile.tileInfo.modelName == "StartBlock")
			{
				// �X�^�[�g�ʒu��ݒ�
				startRow = row;
				// �X�^�[�g�ʒu�̗��ݒ�
				startCol = col;
				// ���[�v�𔲂���
				break;
			}
		}
		// �X�^�[�g�ʒu�����������烋�[�v�𔲂���
		if (startRow != -1) break;
	}
	// �㉺���E�̕������
	struct Dir { int dRow, dCol; };
	// �������Ƃ̃x�N�g�����`
	const Dir dirs[4] =
	{
		{ -1,  0 }, // ��
		{  1,  0 }, // ��
		{  0, -1 }, // ��
		{  0,  1 }, // �E
	};
	// �Ή����鑬�x�x�N�g�����`
	const Vector3 velocities[4] =
	{
		{0, 0, -1}, // ��i�O�i�j
		{0, 0,  1}, // ���i��i�j
		{-1, 0, 0}, // ��
		{ 1, 0, 0}, // �E
	};
	// �X�^�[�g�ʒu��������Ȃ������ꍇ�̓f�t�H���g�̑��x��ݒ�
	for (int i = 0; i < 4; i++)
	{
		// �אڂ���^�C���̍s�Ɨ���v�Z
		int nRow = startRow + dirs[i].dRow;
		int nCol = startCol + dirs[i].dCol;
		// �אڂ���^�C�����}�b�v�͈͓̔��ł��邩�m�F
		if (nRow >= 0 && nRow < GetParent()->GetCSVMap()->GetMaxCol() &&// �s�͈̔̓`�F�b�N
			nCol >= 0 && nCol < GetParent()->GetCSVMap()->GetMaxRow())	// ��͈̔̓`�F�b�N
		{
			// �אڂ���^�C���̃f�[�^���擾
			const auto& neighbor = GetParent()->GetCSVMap()->GetTileData(nRow, nCol);
			// �^�C���̃��f�������擾
			const std::string& model = neighbor.tileInfo.modelName;
			// ���f������"DefaultStraightVertical"�܂���"DefaultStraightHorizontal"�ł��邩�m�F
			if ((i <= 1 && model == "DefaultStraightVerticalBlock") || // �㉺
				(i >= 2 && model == "DefaultStraightHorizontalBlock")) // ���E
			{
				// �Ή����鑬�x�x�N�g����ݒ�
				m_currentVelocity = velocities[i];
				// ���[�v�𔲂���
				break;
			}
		}
		else
		{
			m_currentVelocity = Vector3::Left; // �f�t�H���g�͌�����
		}
	}
}
/*
*	@brief �^�C���̒��S�ɂ��邩�ǂ����𔻒肷��
*	@details �v���C���[�̈ʒu���^�C���̒��S�ɋ߂����ǂ����𔻒肷��B
*	@param charPos �v���C���[�̈ʒu
*	@param tileCenter �^�C���̒��S�ʒu
*	@param epsilon ����̋��e�덷
*	@return �^�C���̒��S�ɂ���ꍇ��true�A�����łȂ��ꍇ��false
*/
bool MiniCharacter::IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon) const
{
	// �^�C���̒��S�ƃv���C���[�̈ʒu�̋������v�Z
	float distance = (charPos - tileCenter).Length();
	// ���������e�덷�ȉ��ł���΁A�^�C���̒��S�ɂ���Ɣ��f
	return distance < epsilon;
}
/*
*	@brief �Q�[���I�[�o�[�ƃN���A�̏������s��
*	@details �Q�[���I�[�o�[��N���A�̏������s���B
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void MiniCharacter::HandleGameOverAndClear(float elapsedTime)
{
	// ���g�p�x����\��
	UNREFERENCED_PARAMETER(elapsedTime);
	// SimpleMath�̖��O��Ԃ��g�����߂�using�錾��ǉ�
	using namespace DirectX::SimpleMath;
	// �S�[�����A�����b�N����Ă��邩�ǂ������擾
	bool goalUnlocked = GetParent()->GetCSVItem()->IsGoalUnlocked();
	// ���݂̃^�C�����擾
	const auto& tile = GetParent()->GetCSVMap()->GetTileData(m_currentPosition);
	// ���݂̃^�C���̖��O���擾
	std::string goalTileName = tile.tileInfo.modelName;
	// ���݂̈ʒu���^�C���̒��S�ɂ��邩�ǂ����𔻒�
	bool isCenter = IsAtTileCenter(m_currentPosition, tile.pos);
	// �A�����b�N���ꂽ�S�[���̐^��ɂ���Ȃ�N���A
	if (goalTileName == "GoalBlock" && goalUnlocked == true && isCenter)	dynamic_cast<MiniCharacterBase*>(m_parent)->SetGameClear(true);
	// �r���Ŏ~�܂邩�����Ă���ꍇ�A�Q�[���I�[�o�[
	else if (m_currentVelocity.y < Vector3::Zero.y || m_currentVelocity == Vector3::Zero)dynamic_cast<MiniCharacterBase*>(m_parent)->SetGameOver(true);
}

