/*
*	@file Panel.cpp
*	@brief �p�l���N���X
*/
#include <pch.h>
#include "Panel.h"
// 1�^�C���̕�
const float Panel::TILE_SIZE = 90.0f;
// �^�C���̖���(�␳�l���l���j
const int Panel::TILE_COUNT = 5;
// �^�C���̍��W�̕␳�l
const float Panel::TILE_POSITION_CORRECTION = 0.6f;
// �^�C���̖����i�␳�l�l���j
const float Panel::TILE_COUNT_CORRECTED = TILE_COUNT - TILE_POSITION_CORRECTION;
// 3D��ԏ�̃v���C���[�̍��W�̕␳�l
const float Panel::PLAYER_POSITION_CORRECTION = 4.0f;
// 3D��ԏ�̃^�C���̕��i�␳�l�l���j
const float Panel::TILE_SIZE_3D = 8.75f;
// �`��I�t�Z�b�gX
const float Panel::DRAW_OFFSET_X = -350.0f;
// �`��I�t�Z�b�gY
const float Panel::DRAW_OFFSET_Y = 420.0f;

/*
*	@brief �R���X�g���N�^
*	@details �p�l���N���X�̃R���X�g���N�^
*	@param mapSizeX �}�b�v�̃T�C�YX
*	@param mapSizeY �}�b�v�̃T�C�YY
*	@return �Ȃ�
*/
Panel::Panel(int mapSizeX, int mapSizeY)
	: m_pCommonResources(nullptr) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_viewPortControll() // �r���[�|�[�g�̐���
	, m_pDR(nullptr) // �f�o�C�X���\�[�X
	, m_pCSVMap(nullptr) // CSV�}�b�v�ւ̃|�C���^
	, m_pCSVItem(nullptr) // CSV�A�C�e���ւ̃|�C���^
	, m_pMouse(nullptr) // �}�E�X�ւ̃|�C���^
	, m_hit(false) // UI�Ƀq�b�g�������ǂ���
	, m_time(0.0f) // �o�ߎ���
	, m_windowHeight(0) // �E�B���h�E�̍���
	, m_windowWidth(0) // �E�B���h�E�̕�
	, m_menuIndex(0) // ���ݑI������Ă��郁�j���[�̃C���f�b�N�X
	, m_mapSizeX(mapSizeX) // �}�b�v�̃T�C�YX
	, m_mapSizeY(mapSizeY) // �}�b�v�̃T�C�YY
	, m_row(-1) // �s�ԍ��i�ۑ��p�j
	, m_col(-1) // ��ԍ��i�ۑ��p�j
{
}
/*
*	@brief �f�X�g���N�^
*	@details �p�l���N���X�̃f�X�g���N�^(�����ł͉������Ȃ�)
*	@param �Ȃ�
*	@return �Ȃ�
*/
Panel::~Panel()
{
	/*do nothing*/
}
/*
*	@brief ������
*	@details �p�l���N���X�̏��������s��
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@param width �E�B���h�E�̕�
*	@param height �E�B���h�E�̍���
*	@return �Ȃ�
*/
void Panel::Initialize(CommonResources* resources, int width, int height)
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
	// ���[�̈ʒu
	const float startX = Screen::CENTER_X - (TILE_SIZE * 2);
	// ��[�̈ʒu
	const float startY = Screen::CENTER_Y - (TILE_SIZE * 2);
	// �O���b�h�̃L�[�����肷�邽�߂̃��[�v
	for (int row = 0; row < m_mapSizeX; ++row)
	{
		for (int col = 0; col < m_mapSizeY; ++col)
		{
			// CSV�}�b�v�̃^�C�������擾
			const MapTileData& tileData = m_pCSVMap->GetTileData(row, col);
			// �O���b�h�̃L�[������
			std::string textureKey = ((row + col) % 2 == 0) ? "GridA" : "GridB";
			// �^�C���̎�ނɉ����ăe�N�X�`���L�[��ύX
			textureKey = tileData.tileInfo.modelName.empty() ? textureKey : tileData.tileInfo.modelName;
			// �ʒu�v�Z
			float posX = startX + col * TILE_SIZE + DRAW_OFFSET_X;
			float posY = startY + row * TILE_SIZE + DRAW_OFFSET_Y;
			Vector2 pos(posX, posY);
			// �^�C���̎�ނɉ����ăe�N�X�`���L�[��ύX���ĕ��ׂ�
			Add(textureKey
				, pos
				, Vector2(TILE_POSITION_CORRECTION)
				, KumachiLib::ANCHOR::MIDDLE_CENTER
				, UIType::TILE);
			// �A�C�e����z�u����
			PlaceItems(m_pCSVItem->GetItemData(row, col), row, col, pos);
			// �v���C���[��z�u����
			PlacePlayer(tileData, row, col, pos);

		}
	}
}
/*
*	@brief �X�V
*	@details �p�l���̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Panel::Update(const float elapsedTime)
{
	// �K�v�Ȗ��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �}�E�X�̃q�b�g�t���O�����Z�b�g
	m_pMouse->SetHit(false);
	// �}�E�X�̃q�b�g�p�l���C���f�b�N�X�����Z�b�g
	m_pMouse->SetHitPanelIndex(-1);
	// UI�v�f���ƂɃq�b�g������s��
	for (int i = 0; i < m_pTiles.size(); i++)
	{
		// �}�E�X���r���[�|�[�g�O�Ȃ�X�L�b�v
		if (m_pMouse->GetPosition().x < 0 || m_pMouse->GetPosition().y < 0 ||
			m_pMouse->GetPosition().x >= m_pMouse->GetVpWidthUI() ||
			m_pMouse->GetPosition().y >= m_pMouse->GetVpHeightUI())
			continue;
		// �q�b�g����iUI�v�f���Ɓj
		if (m_pTiles[i]->IsHit(m_pMouse->GetPosition()))
		{
			m_pMouse->SetHit(true); // �}�E�X�̃q�b�g�t���O���Z�b�g
			m_pMouse->SetHitPanelIndex(i);// ���������p�l���̃C���f�b�N�X���Z�b�g
			m_pMouse->SetPanelPosition(m_pTiles[i]->GetPosition()); // ���������p�l���̈ʒu���Z�b�g
			// ���������p�l���̍s�ԍ���ݒ�
			m_pMouse->SetHitPanelRowIndex(i / m_mapSizeX);
			// ���������p�l���̗�ԍ���ݒ�
			m_pMouse->SetHitPanelColIndex(i % m_mapSizeX);
			break;
		}
	}
	// �o�ߎ��Ԃ����Z
	m_time += elapsedTime;
	// �SUI�v�f�̌o�ߎ��Ԃ��X�V
	// �^�C���̌o�ߎ��Ԃ��X�V
	for (int i = 0; i < m_pTiles.size(); i++)m_pTiles[i]->SetTime(m_pTiles[i]->GetTime() + elapsedTime);
	// �A�C�e���̌o�ߎ��Ԃ��X�V
	for (int i = 0; i < m_pItems.size(); i++)
	{
		// �A�C�e���̃f�[�^�����݂���ꍇ�̂ݍX�V
		if (m_pCSVItem->GetItemData(m_pItems[i].second.row, m_pItems[i].second.col).itemBasePtr != nullptr)
			// �A�C�e���̌o�ߎ��Ԃ��X�V
			m_pItems[i].first->SetTime(m_pItems[i].first->GetTime() + elapsedTime);

	}
	// �v���C���[�A�C�R���̍X�V
	UpdatePlayerIcons(elapsedTime);
}
/*
*	@brief �v���C���[�A�C�R���̍X�V
*	@details �v���C���[�A�C�R���̍X�V���s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void Panel::UpdatePlayerIcons(const float elapsedTime)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// ���[�̈ʒu
	const float startX = Screen::CENTER_X - (TILE_SIZE * 2);
	// ��[�̈ʒu
	const float startY = Screen::CENTER_Y - (TILE_SIZE * 2);
	// 3D��ԏ�̍��W�i-4�`4�j���^�C�����W�i0�`5�j�ɐ��`�ϊ�
	float tileCoordX = ((m_playerPosition.x) + PLAYER_POSITION_CORRECTION) * TILE_COUNT_CORRECTED / TILE_SIZE_3D;
	float tileCoordZ = ((m_playerPosition.z) + PLAYER_POSITION_CORRECTION) * TILE_COUNT_CORRECTED / TILE_SIZE_3D;
	// �ʒu�v�Z
	float posX = startX + (tileCoordX)*TILE_SIZE + DRAW_OFFSET_X;
	float posY = startY + (tileCoordZ)*TILE_SIZE + DRAW_OFFSET_Y;
	Vector2 position(posX, posY);
	// �v���C���[�̈ʒu���X�V
	for (unsigned int i = 0; i < m_pPlayerIcons.size(); i++)
	{
		// �v���C���[�A�C�R���̌o�ߎ��Ԃ��X�V
		m_pPlayerIcons[i]->SetTime(m_pPlayerIcons[i]->GetTime() + elapsedTime);
		// �v���C���[�A�C�R���̈ʒu���X�V
		m_pPlayerIcons[i]->SetPosition(position);
	}
}
/*
*	@brief �`��
*	@details �����ł̓v���C���[�̃A�C�R����`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Panel::Render()
{
	// �v���C���[�A�C�R���̕`��
	for (unsigned int i = 0; i < m_pPlayerIcons.size(); i++)m_pPlayerIcons[i]->Render();
}

/*
*	@brief �^�C�����̕`��
*	@details �p�l���ɔz�u���ꂽ�^�C������`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Panel::DrawTiles()
{
	// �^�C���̕`��
	for (unsigned int i = 0; i < m_pTiles.size(); i++)m_pTiles[i]->Render();
}
/*
*	@brief �A�C�e�����̕`��
*	@details �p�l���ɔz�u���ꂽ�A�C�e������`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Panel::DrawItems()
{
	// �A�C�e���̕`��
	for (int i = 0; i < m_pItems.size(); i++)
	{
		// �A�C�e���̃f�[�^�����݂���ꍇ�̂ݕ`��
		if (m_pCSVItem->GetItemData(m_pItems[i].second.row, m_pItems[i].second.col).itemBasePtr != nullptr)
			m_pItems[i].first->Render();
	}
}
/*
*	@brief �A�C�e���̔z�u
*	@details �p�l���ɃA�C�e����z�u����iInitialize����Ɨ�)
*	@param itemData �A�C�e���̃f�[�^
*	@param row �A�C�e���̍s�ԍ�
*	@param col �A�C�e���̗�ԍ�
*/
void Panel::PlaceItems(const MapItemData& itemData, int row, int col, const DirectX::SimpleMath::Vector2& pos)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// �A�C�e�����Ȃ��ꍇ�̓X�L�b�v
	if (itemData.itemInfo.modelName.empty())return;
	// �A�C�e���̈ʒu���v�Z
	Vector2 itemPos = pos;
	// �A�C�e���̃e�N�X�`���L�[���擾
	std::string itemTextureKey = itemData.itemInfo.modelName;
	// �s�Ɨ��ۑ�
	m_row = row;
	m_col = col;
	// �A�C�e����ǉ�
	Add(itemTextureKey
		, itemPos
		, Vector2(TILE_POSITION_CORRECTION, TILE_POSITION_CORRECTION)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::ITEM);
}
/*
*	@brief �v���C���[�̔z�u
*	@details �z�u�����^�C�����X�^�[�g�n�_�̎��A�p�l���Ƀv���C���[��z�u����iInitialize����Ɨ�)
*/
void Panel::PlacePlayer(const MapTileData& tileData, int row, int col, const DirectX::SimpleMath::Vector2& pos)
{
	// ���O��Ԃ̃G�C���A�X
	using namespace DirectX::SimpleMath;
	// �X�^�[�g�n�_�̃^�C�����ǂ������m�F
	if (tileData.tileInfo.modelName != "StartBlock")return;
	// �s�Ɨ��ۑ�
	m_row = row;
	m_col = col;
	// �v���C���[��ǉ�
	Add("PlayerIcon"
		, pos
		, Vector2(0.6f, 0.6f)
		, KumachiLib::ANCHOR::MIDDLE_CENTER
		, UIType::PLAYERICON);
}
/*
*	@brief �p�l���̒ǉ�
*	@details �p�l����UI�v�f��ǉ�����
*	@param key �摜�̃L�[
*	@param position �ʒu
*	@param scale �X�P�[��
*	@param anchor �A���J�[
*	@param type UI�̎��
*	@return �Ȃ�
*/
void Panel::Add(const std::string& key,
	const DirectX::SimpleMath::Vector2& position,
	const DirectX::SimpleMath::Vector2& scale,
	KumachiLib::ANCHOR anchor, UIType type)
{
	// UI�I�u�W�F�N�g�̐���
	std::unique_ptr<Canvas> userInterface = std::make_unique<Canvas>(m_pCommonResources);
	// �w��摜��UI�쐬
	userInterface->Create(m_pDR, key, position, scale, anchor);
	// �E�B���h�E�T�C�Y��ݒ�
	userInterface->SetWindowSize(m_windowWidth, m_windowHeight);
	// UI�̎�ނɉ����ď����𕪊�
	// �I���\�ȃA�C�e���Ȃ�
	if (type == UIType::TILE)m_pTiles.push_back(std::move(userInterface));
	// �A�C�e���Ȃ�
	else if (type == UIType::ITEM)
	{
		// �A�C�e������ݒ�
		ItemInfo itemInfo{};
		// ���W�ς݃t���O��������
		itemInfo.isCollected = false;
		// �s�ԍ���ݒ�
		itemInfo.row = m_row;
		// ��ԍ���ݒ�
		itemInfo.col = m_col;
		// �A�C�e���̃y�A���`
		std::pair<std::unique_ptr<Canvas>, ItemInfo> item;
		// UI�I�u�W�F�N�g���Z�b�g
		item.first = std::move(userInterface);
		// �A�C�e�������Z�b�g
		item.second = itemInfo;
		// �A�C�e�����p�l���ɒǉ�
		m_pItems.push_back(std::move(item));
	}
	// �v���C���[�A�C�R���Ȃ�
	else if (type == UIType::PLAYERICON)
	{
		// �v���C���[�A�C�R�����p�l���ɒǉ�
		m_pPlayerIcons.push_back(std::move(userInterface));
	}
	// ���m��UI�^�C�v
	else throw std::runtime_error("Unknown UIType in Panel::Add");
}
