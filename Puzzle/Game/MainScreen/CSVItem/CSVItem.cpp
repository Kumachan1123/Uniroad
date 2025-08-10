/*
*	@file CSVItem.cpp
*	@brief CSV�`���̃A�C�e�����z��ǂݍ��݁A�`�悷��N���X
*/
#include <pch.h>
#include "CSVItem.h"
/*
*	@brief �R���X�g���N�^
*	@details �������ɋ��ʃ��\�[�X�ւ̃|�C���^���󂯎��A���������s���B
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
CSVItem::CSVItem(CommonResources* resources)
	: m_time(0.0f) // �o�ߎ���
	, m_pCommonResources(resources) // ���ʃ��\�[�X�ւ̃|�C���^
	, m_collectedMedals(0) // ���W�������_���̐�
	, m_createdMedals(0) // �����������_���̐�
	, m_goalUnlocked(true)// �S�[�����b�N����������Ă��邩�ǂ���
{
	// �A�C�e���̃^�C���̎�����������
	InitializeTileDictionary();
	// �����蔻��`��̏�����
	DrawCollision::Initialize(m_pCommonResources);
}
/*
*	@brief �f�X�g���N�^
*	@details �������Ɋm�ۂ������\�[�X���������B
*	@param �Ȃ�
*	@return �Ȃ�
*/
CSVItem::~CSVItem()
{
	// ���ʃ��\�[�X�̉���͕s�v
	// DrawCollision�̏I�������͕s�v
	// �^�C���̎������N���A
	m_tileDictionary.clear();
	// �^�C���̃����_�����O�f�[�^���N���A
	m_tiles.clear();

}
/*
*	@brief �^�C���̎���������������
*	@details �^�C���̎�ނƂ��̏��������ɓo�^����B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void CSVItem::InitializeTileDictionary()
{
	// �^�C���̎�ނƂ��̏��������ɓo�^
	// ��
	m_tileDictionary["0"] = ItemInfo{ "", false };
	// ���_��
	m_tileDictionary["m"] = ItemInfo{ "Medal", false };
	// �S�[�����b�N
	m_tileDictionary["l"] = ItemInfo{ "GoalLock", false };
}

/*
*	@brief CSV�`���̃A�C�e����ǂݍ���
*	@details �w�肳�ꂽ�t�@�C���p�X����CSV�`���̃A�C�e���f�[�^��ǂݍ��݁A�^�C���̏�����͂��ăA�C�e�����\�z����B
*	@param filePath �ǂݍ���CSV�t�@�C���̃p�X
*	@return �Ȃ�
*/
void CSVItem::LoadItem(const std::string& filePath)
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// �t�@�C�����J��
	std::ifstream file(filePath);
	// �t�@�C�����J���Ȃ������ꍇ�͉������Ȃ�
	if (!file.is_open()) return;
	// CSV�ǂݍ��ޑO��2�����z����m�ۂ���
	// �s���m��
	m_mapItemData.resize(MAXCOL);
	// �e�s�ɗ񐔊m��
	for (int i = 0; i < MAXCOL; ++i)m_mapItemData[i].resize(MAXRAW);
	// �A�C�e���̏�����
	std::string line;
	// �s�ԍ���������
	int row = 0;
	// �A�C�e�����S�␳�l�v�Z
	float mapWidth = MAXRAW * 2.0f;
	float mapHeight = MAXCOL * 2.0f;
	// ���S�␳�l���v�Z
	float offsetX = mapWidth / 2.0f - 1.0f;
	float offsetZ = mapHeight / 2.0f - 1.0f;
	// �A�C�e���̍s�Ɨ��ǂݍ���
	while (std::getline(file, line) && row < MAXCOL)
	{
		// �s�̕�������J���}�ŕ������ăZ�����Ƃɏ���
		std::stringstream ss(line);
		// �Z���̕�������i�[����ϐ�
		std::string cell;
		// ��ԍ���������
		int col = 0;
		// �Z�����Ƃɏ���
		while (std::getline(ss, cell, ',') && col < MAXRAW)
		{
			// �Z���̕����񂪋�̏ꍇ�̓X�L�b�v
			auto it = m_tileDictionary.find(cell);
			// �Z���̕����񂪎����ɑ��݂���ꍇ
			if (it != m_tileDictionary.end() && it->second.modelName != "")
			{
				// �^�C�������擾
				const ItemInfo& tileInfo = it->second;
				// �^�C���̈ʒu�v�Z�i�A�C�e�����S�␳�j
				float x = static_cast<float>(col * 2) - offsetX;
				float z = static_cast<float>(row * 2) - offsetZ;
				// ���[���h���W���v�Z
				Vector3 pos(x, 3.0f, z);
				// �^�C���𐶐�
				std::unique_ptr<ItemBase> itemBase = ItemFactory::CreateItemByName(tileInfo.modelName);
				// �A�C�e����������
				itemBase->Initialize(m_pCommonResources, tileInfo);
				// �A�C�e���̈ʒu��ݒ�
				itemBase->SetPosition(pos);
				// �A�C�e���̃��[���h�s���ݒ�
				itemBase->SetWorldMatrix(Matrix::CreateScale(tileInfo.scale) * Matrix::CreateTranslation(pos));
				// �A�C�e���̃��f����ݒ�
				itemBase->SetModel(m_pCommonResources->GetModelManager()->GetModel(tileInfo.modelName));
				// �A�C�e���ɍs�Ɨ�̏���ݒ�
				itemBase->SetRow(row);// �s�ԍ���ݒ�
				itemBase->SetCol(col);// ��ԍ���ݒ�
				// �A�C�e���f�[�^�Ƀ^�C������ۑ�
				m_mapItemData[row][col] = MapItemData{ tileInfo, pos, std::move(itemBase) };
				// ���������A�C�e�������_���Ȃ�J�E���g
				if (tileInfo.modelName == "Medal") m_createdMedals++;
				// �S�[�����b�N���������ꂽ������t���O��false�ɂ���
				if (tileInfo.modelName == "GoalLock")
					m_goalUnlocked = false;
			}
			else
			{
				// �^�C���̈ʒu�v�Z�i�A�C�e�����S�␳�j
				float x = static_cast<float>(col * 2) - offsetX;
				float z = static_cast<float>(row * 2) - offsetZ;
				// ���[���h���W���v�Z
				Vector3 pos(x, 0.0f, z);
				// ���[���h�s����쐬�i�X�P�[�����O�ƈʒu�̐ݒ�j
				Matrix world = Matrix::CreateScale(Vector3::One) * Matrix::CreateTranslation(pos);
				// �Z���̕����񂪎����ɑ��݂��Ȃ��ꍇ�͋�̃^�C����ǉ�
				m_tiles.push_back(ItemRenderData{ nullptr, Matrix::Identity });
				// �f�t�H���g�̏��^�C�����g�p
				const ItemInfo& emptyItemInfo = m_tileDictionary[""];
				// �A�C�e���f�[�^�ɋ�̃^�C������ۑ�
				m_mapItemData[row][col] = MapItemData{ emptyItemInfo, pos };
			}
			// �A�C�e���̗�ɒl��ݒ�
			++col;
		}
		// �A�C�e���̍s�ɒl��ݒ�
		++row;
	}
}
/*
*	@brief �X�V����
*	@details �A�C�e���̍X�V�������s��
*	@param elapsedTime �o�ߎ���
*	@return �Ȃ�
*/
void CSVItem::Update(float elapsedTime)
{
	// �o�ߎ��Ԃ��X�V
	m_time += elapsedTime;
	// ��̐��J��Ԃ�
	for (int col = 0; col < MAXCOL; ++col)
	{
		// �s�̐��J��Ԃ�
		for (int row = 0; row < MAXRAW; ++row)
		{
			// �A�C�e��������Ȃ�X�V
			if (m_mapItemData[col][row].itemBasePtr != nullptr)m_mapItemData[col][row].itemBasePtr->Update(elapsedTime);
			// �S�[�����b�N�ȊO�̓X�L�b�v
			if (!dynamic_cast<GoalLock*>(m_mapItemData[col][row].itemBasePtr.get())) continue;
			// ���_���̐�����v������
			if (m_collectedMedals == m_createdMedals)
			{
				// ���b�N���폜
				RemoveItem(col, row);
				// �S�[�����A�����b�N
				m_goalUnlocked = true;
			}

		}
	}
}
/*
*	@brief �����蔻���`�悷��
*	@details �f�o�b�O���[�h�œ����蔻��̃{�b�N�X��`�悷��B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void CSVItem::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// ���g�p�̌x�����o���Ȃ�
	UNREFERENCED_PARAMETER(view);
	UNREFERENCED_PARAMETER(proj);
	// �����蔻��̕`����J�n
#ifdef _DEBUG
	//// �`��J�n
	//DrawCollision::DrawStart(view, proj);
	//// �����蔻��̃{�b�N�X��`��
	//for (int i = 0; i < m_wallBox.size(); i++)	DrawCollision::DrawBoundingBox(m_wallBox[i], Colors::Red);
	//// �`��I��
	//DrawCollision::DrawEnd();

#endif
}
/*
*	@brief �A�C�e����`�悷��
*	@details �A�C�e���̃^�C����`�悷��B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void CSVItem::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	using namespace DirectX::SimpleMath;
	// ��̐��J��Ԃ�
	for (int col = 0; col < MAXCOL; ++col)
	{
		// �s�̐��J��Ԃ�
		for (int row = 0; row < MAXRAW; ++row)
		{
			// �A�C�e��������Ȃ�X�V
			if (m_mapItemData[col][row].itemBasePtr != nullptr)	m_mapItemData[col][row].itemBasePtr->Render(view, proj);
		}
	}
#ifdef _DEBUG
	const auto debugString = m_pCommonResources->GetDebugString();
	debugString->AddString("CountMedals:%i", m_collectedMedals);
	debugString->AddString("GoalUnlocked:%s", m_goalUnlocked ? "true" : "false");
#endif
}


/*
*	@brief �w��ʒu�̃^�C���̃A�C�e�������擾����
*	@details �w�肳�ꂽ��ƍs�̈ʒu�ɂ���^�C���̏����擾����B
*	@param row �s�ԍ�
*	@param col ��ԍ�
*	@return �w��ʒu�̃^�C�����ւ̎Q��
*/
const  MapItemData& CSVItem::GetItemData(int row, int col) const
{
	assert(col >= 0 && col < MAXCOL && row >= 0 && row < MAXRAW);
	return m_mapItemData[row][col];
}
/*
*	@brief �w����W�̃^�C���̃A�C�e�������擾����
*	@details �w�肳�ꂽ���[���h���W�ɍł��߂��^�C���̏����擾����B
*	@param pos ���[���h���W
*	@return �w����W�̃^�C�����ւ̎Q��
*/
const MapItemData& CSVItem::GetItemData(const DirectX::SimpleMath::Vector3& pos) const
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �ŏ������ƑΉ�����^�C���̃C���f�b�N�X��������
	float minDistance = std::numeric_limits<float>::max();
	int closestRow = -1;
	int closestCol = -1;
	// �A�C�e���f�[�^�𑖍����čł��߂��^�C����T��
	for (int row = 0; row < MAXRAW; ++row)
	{
		for (int col = 0; col < MAXCOL; ++col)
		{
			const MapItemData& tile = m_mapItemData[row][col];

			// �^�C���̈ʒu�Ƃ̋������v�Z
			float distance = (tile.pos - pos).LengthSquared();
			// �ŏ��������X�V
			if (distance < minDistance)
			{
				minDistance = distance;
				closestRow = row;
				closestCol = col;
			}
		}
	}
	// �ł��߂��^�C���̏���Ԃ�
	assert(closestRow >= 0 && closestRow < MAXCOL && closestCol >= 0 && closestCol < MAXRAW);

	return m_mapItemData[closestRow][closestCol];
}
/*
*	@brief �w����W�̃^�C���̃A�C�e��������
*	@details �w�肳�ꂽ���[���h���W�ɂ���^�C���̃A�C�e��������������B
*	@param pos ���[���h���W
*	@return �Ȃ�
*/
void CSVItem::RemoveItem(int row, int col)
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	// �A�C�e���̃|�C���^�����Z�b�g���ď���
	m_mapItemData[row][col].itemBasePtr = nullptr; // �A�C�e��������
	return; // ����������I��
}
