/*
*	@file Panel.cpp
*	@brief �p�l���N���X
*/
#include <pch.h>
#include "Panel.h"
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
	// �ꖇ������̕�
	const float tileSize = 90.0f;
	// ���[�̈ʒu
	const float startX = Screen::CENTER_X - (tileSize * 2);
	// ��[�̈ʒu
	const float startY = Screen::CENTER_Y - (tileSize * 2);
	// �O���b�h�̃L�[�����肷�邽�߂̃��[�v
	for (int row = 0; row < 5; ++row)
	{
		for (int col = 0; col < 5; ++col)
		{
			// CSV�}�b�v�̃^�C�������擾
			const MapTileData& tileData = m_pCSVMap->GetTileData(row, col);
			// �O���b�h�̃L�[������
			std::string textureKey = ((row + col) % 2 == 0) ? "GridA" : "GridB";
			// �^�C���̎�ނɉ����ăe�N�X�`���L�[��ύX
			textureKey = tileData.tileInfo.modelName.empty() ? textureKey : tileData.tileInfo.modelName;
			// �ʒu�v�Z
			float posX = startX + col * tileSize - 350.0f;
			float posY = startY + row * tileSize + 420.0f;

			// �^�C���̎�ނɉ����ăe�N�X�`���L�[��ύX���ĕ��ׂ�
			Add(textureKey
				, Vector2(posX, posY)
				, Vector2(0.6f, 0.6f)
				, KumachiLib::ANCHOR::MIDDLE_CENTER
				, UIType::TILE);
			// �A�C�e����z�u����
			const MapItemData& itemData = m_pCSVItem->GetItemData(row, col);
			// �A�C�e�����Ȃ��ꍇ�̓X�L�b�v
			if (itemData.itemInfo.modelName.empty())continue;
			// �A�C�e���̈ʒu���v�Z
			Vector2 itemPos = Vector2(posX, posY);
			// �A�C�e���̃e�N�X�`���L�[���擾
			std::string itemTextureKey = itemData.itemInfo.modelName;
			// �s�Ɨ��ۑ�
			m_row = row;
			m_col = col;
			// �A�C�e����ǉ�
			Add(itemTextureKey
				, itemPos
				, Vector2(0.6f, 0.6f)
				, KumachiLib::ANCHOR::MIDDLE_CENTER
				, UIType::ITEM);

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
	for (int i = 0; i < m_pTiles.size(); i++)
	{
		m_pTiles[i]->SetTime(m_pTiles[i]->GetTime() + elapsedTime);

	}
	for (int i = 0; i < m_pItems.size(); i++)
	{
		if (m_pCSVItem->GetItemData(m_pItems[i].second.row, m_pItems[i].second.col).itemBasePtr != nullptr)
			m_pItems[i].first->SetTime(m_pItems[i].first->GetTime() + elapsedTime);
	}
}
/*
*	@brief �`��
*	@details �p���������ƂŐ��܂ꂽ��̊֐�
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Panel::Render()
{
	// �����ł͉������Ȃ�
	return;
}

/*
*	@brief �^�C�����̕`��
*	@details �p�l���ɔz�u���ꂽ�^�C������`�悷��
*	@param �Ȃ�
*	@return �Ȃ�
*/
void Panel::DrawTiles()
{
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
	for (int i = 0; i < m_pItems.size(); i++)
	{
		if (m_pCSVItem->GetItemData(m_pItems[i].second.row, m_pItems[i].second.col).itemBasePtr != nullptr)
		{
			m_pItems[i].first->Render();
		}
	}
}


void Panel::Add(const std::string& key, const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& scale, KumachiLib::ANCHOR anchor, UIType type)
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
		itemInfo.isCollected = false; // ���W�ς݃t���O��������
		itemInfo.row = m_row; // �s�ԍ���ݒ�
		itemInfo.col = m_col; // ��ԍ���ݒ�
		std::pair<std::unique_ptr<Canvas>, ItemInfo> item;
		item.first = std::move(userInterface); // UI�I�u�W�F�N�g���Z�b�g
		item.second = itemInfo; // �A�C�e�������Z�b�g
		m_pItems.push_back(std::move(item));
	}
	// ���m��UI�^�C�v
	else throw std::runtime_error("Unknown UIType in Panel::Add");
}
