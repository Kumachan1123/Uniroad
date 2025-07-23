/*
*	@file NextTiles.cpp
*	@brief ���̃^�C���N���X
*/
#include <pch.h>
#include "NextTiles.h"
#include <random>
/*
*	@brief �R���X�g���N�^
*	@details ���̃^�C���N���X�̃R���X�g���N�^
*	@param �Ȃ�
*	@return �Ȃ�
*/
NextTiles::NextTiles()
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_viewPortControll() // �r���[�|�[�g�̐���
	, m_pDR(nullptr) // �f�o�C�X���\�[�X
	, m_pCSVMap(nullptr) // CSV�}�b�v�ւ̃|�C���^
	, m_hit(false) // UI�Ƀq�b�g�������ǂ���
	, m_pMouse(nullptr) // �}�E�X�ւ̃|�C���^
	, m_time(0.0f) // �o�ߎ���
	, m_windowHeight(0) // �E�B���h�E�̍���
	, m_windowWidth(0) // �E�B���h�E�̕�
	, m_draggingIndex(-1) // �h���b�O����UI�C���f�b�N�X
	, m_menuIndex(0) // ���ݑI������Ă��郁�j���[�̃C���f�b�N�X
	, m_initialPositions{} // �e�I���\UI�̏����ʒu���X�g
	, m_lastPlacedTileName("") // �Ō�ɒu�����^�C���̖��O
	, m_previousTileName("") // �O�񐶐������^�C����
{
}
/*
*	@brief �f�X�g���N�^
*	@details ���̃^�C���N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
NextTiles::~NextTiles()
{
	// �������Ȃ�
}
/*
*	@brief ������
*	@details ���̃^�C���N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void NextTiles::Initialize(CommonResources* resources, int width, int height)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// ���ʃ��\�[�X���Z�b�g
	m_pCommonResources = resources;
	// �f�o�C�X���\�[�X�擾
	m_pDR = m_pCommonResources->GetDeviceResources();
	// �E�B���h�E��
	m_windowWidth = (int)(width * .3f);
	// �E�B���h�E����
	m_windowHeight = height;
	// UI�ǉ�
	Add("NextTilesBack"
		, Vector2(290.0f, 300.0f)
		, Vector2(0.6f, 0.6f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::ITEM);
	// �g���^�C����錾
	// ���i(��)
	m_tilesDictionary.push_back("StraightHorizontal");
	// ���i(�c)
	m_tilesDictionary.push_back("StraightVertical");
	// �\��
	m_tilesDictionary.push_back("Cross");
	// �E���J�[�u
	m_tilesDictionary.push_back("RightDown");
	// �����J�[�u
	m_tilesDictionary.push_back("LeftDown");
	// �E��J�[�u
	m_tilesDictionary.push_back("RightUp");
	// ����J�[�u
	m_tilesDictionary.push_back("LeftUp");
}
/*
*	@brief �X�V
*	@details ���̃^�C���N���X�̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void NextTiles::Update(const float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// �}�E�X�̏�Ԃ��擾
	auto& mouseState = m_pCommonResources->GetInputManager()->GetMouseState();
	const auto debugString = m_pCommonResources->GetDebugString();
	m_pMouse->SetHitNewTile(false); // �}�E�X�̃q�b�g�t���O�����Z�b�g
	m_pMouse->SetHitNewTileIndex(-1);// �������Ă���V�����o�Ă����^�C���̔ԍ������Z�b�g
	// --- �h���b�O���͑�UI�̃q�b�g��������Ȃ� ---
	if (m_pMouse->IsMouseDrag() && m_draggingIndex >= 0)
	{
		// �h���b�O���̏����������
		if (mouseState.leftButton)
		{
			m_pTile[m_draggingIndex].canvas->SetPosition(m_pMouse->GetPosition());
			m_pMouse->SetNewTilePosition(m_pTile[m_draggingIndex].canvas->GetPosition()); // ���������V�����^�C���̈ʒu���Z�b�g
		}
		else// ���{�^���������u�Ԃ������Z�b�g
		{
			// �p�l���ɓ������Ă��Ȃ���Ό��̈ʒu�ɖ߂�
			if (m_pMouse->GetHitPanelIndex() == -1)	ResetTilePosition();
			else AddToPanel();
			m_pMouse->SetMouseDrag(false);
			m_draggingIndex = -1;
		}
	}
	else
	{
		// �h���b�O���Ă��Ȃ���Ԃ̂݃q�b�g����&�h���b�O�J�n
		for (int i = 0; i < m_pTile.size(); i++)
		{
			if (m_pMouse->GetPosition().x < 0 || m_pMouse->GetPosition().y < 0 ||
				m_pMouse->GetPosition().x >= m_pMouse->GetVpWidthUI() ||
				m_pMouse->GetPosition().y >= m_pMouse->GetVpHeightUI())
				continue;

			if (m_pTile[i].canvas->IsHit(m_pMouse->GetPosition()))
			{
				m_pMouse->SetHitNewTile(true); // �}�E�X�̃q�b�g�t���O���Z�b�g
				m_pMouse->SetHitNewTileIndex(i); // ���������V�����^�C���̃C���f�b�N�X���Z�b�g
				m_pMouse->SetNewTilePosition(m_pTile[i].canvas->GetPosition()); // ���������V�����^�C���̈ʒu���Z�b�g


				// �h���b�O�J�n�i�h���b�O���Ă��Ȃ����̂݁j
				if (mouseState.leftButton && !m_pMouse->IsMouseDrag())
				{
					m_draggingIndex = i;
					m_pMouse->SetMouseDrag(true);
				}
				break;
			}
		}
	}
	// �o�ߎ��Ԃ����Z
	m_time += elapsedTime;
	// 10�b���Ƃ�UI��ǉ�����
	if (m_time >= 2.0f)
	{
		// UI��ǉ�
		AddNextTiles();
		// ���Ԃ����Z�b�g
		m_time = 0.0f;
	}
	// �S�w�iUI�̌o�ߎ��Ԃ��X�V
	for (int i = 0; i < m_pBack.size(); i++)
	{
		// �w�iUI�̎��Ԃ��X�V
		m_pBack[i].canvas->SetTime(m_pBack[i].canvas->GetTime() + elapsedTime);
	}
	// �S�^�C���̌o�ߎ��Ԃ��X�V
	for (int i = 0; i < m_pTile.size(); i++)
	{
		// �^�C���̎��Ԃ��X�V
		m_pTile[i].canvas->SetTime(m_pTile[i].canvas->GetTime() + elapsedTime);
	}
	// �S�ݒu�ς݃^�C���̌o�ߎ��Ԃ��X�V
	for (int i = 0; i < m_pPlacedTile.size(); i++)
	{
		// �ݒu�ς݃^�C���̎��Ԃ��X�V
		m_pPlacedTile[i].canvas->SetTime(m_pPlacedTile[i].canvas->GetTime() + elapsedTime);
	}
	// �I�𒆂�UI������Ȃ���W��ύX����
	if (m_pMouse->GetHitNewTileIndex() >= 0 && mouseState.leftButton)m_pTile[m_pMouse->GetHitNewTileIndex()].canvas->SetPosition(m_pMouse->GetPosition());
	// �f�o�b�O�p�̕������ǉ�
	debugString->AddString("DragNextTile:%i", m_draggingIndex);
}
/*
*	@brief �`��
*	@details ���̃^�C���N���X�̕`����s��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void NextTiles::Render()
{
	// �w�i�̐������J��Ԃ�
	for (unsigned int i = 0; i < m_pBack.size(); i++)
	{
		// �w�i�̕`��
		m_pBack[i].canvas->Render();
	}

	// �ݒu�ς݃^�C���̐������J��Ԃ�
	for (unsigned int i = 0; i < m_pPlacedTile.size(); i++)
	{
		// �ݒu�ς݃^�C���̕`��
		m_pPlacedTile[i].canvas->Render();
	}
	// �^�C���̐������J��Ԃ�
	for (unsigned int i = 0; i < m_pTile.size(); i++)
	{
		// �^�C���̕`��
		m_pTile[i].canvas->Render();
	}
}
/*
*	@brief UI�ǉ�
*	@details ���̃^�C���N���X��UI��ǉ�����
*	@param key �e�N�X�`���̃L�[
*	@param position �ʒu
*	@param scale �X�P�[��
*	@param anchor �A���J�[
*	@param type UI�̎��
*	@return �Ȃ�
*/
void NextTiles::Add(const std::string& key, const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor, UIType type)
{
	// �^�C�����̈ꎞ�ۑ��ϐ�
	TileInfo tileInfo;
	// UI�I�u�W�F�N�g�̐���
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// �w��摜��UI�쐬
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// �E�B���h�E�T�C�Y��ݒ�
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// �^�C������ݒ�
	// �L�����o�X����ۑ�
	tileInfo.canvas = std::move(userInterface);
	// �e�N�X�`���̃L�[��ݒ�
	tileInfo.textureKey = key;
	// �I���\�Ȃ�^�C���Ƃ��āA�I��s�Ȃ�w�i�Ƃ��Ēǉ�		
	if (type == UIType::TILE)m_pTile.push_back(std::move(tileInfo));
	else m_pBack.push_back(std::move(tileInfo));

}

/*
*	@brief ����I��UI��ǉ�����
*	@details �X�V���ɒ���I�Ɉ��UI��ǉ�����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void NextTiles::AddNextTiles()
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// UI�̐���5�ȏ�Ȃ�ǉ����Ȃ�
	if (m_pTile.size() == 5)return;
	// ��ƂȂ�^�C��
	std::string tileName;
	// �Ō�ɒu�����^�C�����Ȃ��ꍇ�͍��~�j�L����������^�C������ɂ���
	if (m_lastPlacedTileName == "")
		tileName = m_miniCharacterTileName;
	else
		tileName = m_lastPlacedTileName;
	// ���ɐi�߂�^�C���̃��X�g���擾
	std::vector<std::string> availableTiles = GetAvailableNextTiles(tileName, m_miniCharacterVelocity);
	// �����̐ݒ�
	std::random_device seed;
	// �����Z���k�E�c�C�X�^�@
	std::default_random_engine engine(seed());
	// �����_���Ȕ͈͂�ݒ�
	std::uniform_int_distribution<int> rand(0, (int)availableTiles.size() - 1);
	// �����_���ȃC���f�b�N�X���擾
	int randomIndex = rand(engine);
	// Y���W�𒲐�
	float positionY = 480.0f - (float(m_pTile.size()) * 90.0f);
	// X���W�͌Œ�
	const float positionX = 290.0f;
	// �ʒu��ݒ�
	Vector2 position(positionX, positionY);
	// UI�ǉ�
	Add(availableTiles[randomIndex]
		, position
		, Vector2(0.6f, 0.6f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::TILE);
	// �����ʒu��ۑ�
	m_initialPositions.push_back(position);


}
/*
*	@brief �V�����p�l����z�u
*	@details �V�����p�l����z�u����
*	@param �Ȃ�
*	@return �Ȃ�
*/
void NextTiles::AddToPanel()
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// �^�C���f�[�^����łȂ��ꍇ�͉������Ȃ�
	if (m_pCSVMap->GetTileData(m_pMouse->GetHitPanelRowIndex(), m_pMouse->GetHitPanelColIndex()).tileInfo.modelName == "")
	{
		// �h���b�O���̃^�C�����p�l���ɔz�u
		m_pTile[m_draggingIndex].canvas->SetPosition(m_pMouse->GetPanelPosition());
		// �Ō�ɒu�����^�C����ۑ�
		m_lastPlacedTileName = m_pTile[m_draggingIndex].textureKey;
		// �p�l���ɐV�����^�C����z�u
		m_pCSVMap->SetTileModel(m_pMouse->GetHitPanelRowIndex(), m_pMouse->GetHitPanelColIndex(), m_pTile[m_draggingIndex].textureKey);
		// �z�u�����^�C���͕ʂ̔z��Ɉڂ��A���̔z�񂩂�͏���
		m_pPlacedTile.push_back(std::move(m_pTile[m_draggingIndex]));
		// ���̔z�񂩂�������炻�̏ꏊ��empty�ɂȂ�
		m_pTile.erase(m_pTile.begin() + m_draggingIndex);
		// �z�u�\�^�C���̍��W���l�߂�
		for (unsigned int i = 0; i < m_pTile.size(); i++)
		{
			// Y���W�𒲐�
			float positionY = 480.0f - (float(i) * 90.0f);
			// X���W�͌Œ�
			const float positionX = 290.0f;
			// �ʒu��ݒ�
			Vector2 position(positionX, positionY);
			// �^�C���̈ʒu���X�V
			m_pTile[i].canvas->SetPosition(Vector2(m_pTile[i].canvas->GetPosition().x, positionY));
		}


	}
	else
	{
		// �^�C�������ɑ��݂���ꍇ�͌��̈ʒu�ɖ߂�
		ResetTilePosition();
	}

}
/*
*	@brief ���̈ʒu�Ƀ^�C����߂�
*	@details ���̈ʒu�Ƀ^�C����߂�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void NextTiles::ResetTilePosition()
{
	// �h���b�O���̃^�C��������ꍇ�͌��̈ʒu�ɖ߂�
	m_pTile[m_draggingIndex].canvas->SetPosition(m_initialPositions[m_draggingIndex]);
}
/*
*	@brief �ڑ��\�Ȏ��̃^�C�����擾
*	@details ���݂̃^�C�����Ƒ��x����ڑ��\�Ȏ��̃^�C�����擾����
*	@param currentTileName ���݂̃^�C����
*	@param velocity �~�j�L�����̑��x�x�N�g��
*	@return �ڑ��\�Ȏ��̃^�C���̖��O�̃��X�g
*/
std::vector<std::string> NextTiles::GetAvailableNextTiles(const std::string& currentTileName, const DirectX::SimpleMath::Vector3& velocity) const
{

	// �i�s�������擾
	Direction dir = GetDirectionFromVelocity(velocity);
	// �ڑ��\�ȃ^�C���̃��X�g��Ԃ�
	return TileConnectionTable::GetConnectableTiles(currentTileName, dir);

}
/*
*	@brief ���x����i�s�������擾
*	@details �~�j�L�����̑��x�x�N�g������i�s�������擾����
*	@param velocity �~�j�L�����̑��x�x�N�g��
*	@return �i�s����
*/
Direction NextTiles::GetDirectionFromVelocity(const DirectX::SimpleMath::Vector3& velocity) const
{
	// ���x�x�N�g����X, Z�������`�F�b�N���Đi�s����������
	// Z���������Ȃ��A���Ȃ牺
	if (velocity.z > 0) return Direction::UP;
	if (velocity.z < 0) return Direction::DOWN;
	// X���������Ȃ�E�A���Ȃ獶
	if (velocity.x > 0) return Direction::RIGHT;
	if (velocity.x < 0) return Direction::LEFT;
	// ���x�[����΂߂͗�O����
	return Direction::UP; // �f�t�H���g
}

