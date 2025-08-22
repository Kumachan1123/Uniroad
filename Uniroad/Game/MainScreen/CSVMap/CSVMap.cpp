/*
*	@file CSVMap.cpp
*	@brief CSV�`���̃}�b�v��ǂݍ��݁A�`�悷��N���X
*/
#include <pch.h>
#include "CSVMap.h"


/*
*	@brief �R���X�g���N�^
*	@details �������ɋ��ʃ��\�[�X�ւ̃|�C���^���󂯎��A���������s���B
*	@param resources ���ʃ��\�[�X�ւ̃|�C���^
*	@return �Ȃ�
*/
CSVMap::CSVMap(CommonResources* resources)
{
	// ���ʃ��\�[�X�ւ̃|�C���^��ۑ�
	m_pCommonResources = resources;
	// �}�b�v�̃^�C���̎�����������
	InitializeTileDictionary();
	// �����蔻��`��̏�����
	DrawCollision::Initialize(m_pCommonResources);
	// �[�x�X�e���V���o�b�t�@�̍쐬
	CreateDepthStencilBuffer(m_pCommonResources->GetDeviceResources()->GetD3DDevice());
}
/*
*	@brief �f�X�g���N�^
*	@details �������Ɋm�ۂ������\�[�X���������B
*	@param �Ȃ�
*	@return �Ȃ�
*/
CSVMap::~CSVMap()
{
	// ���ʃ��\�[�X�̉���͕s�v
	// DrawCollision�̏I�������͕s�v
	// �^�C���̎������N���A
	m_tileDictionary.clear();
	// �^�C���̃����_�����O�f�[�^���N���A
	m_tiles.clear();
	// �}�b�v�f�[�^���N���A
	m_mapData.clear();
	// �[�x�X�e���V���o�b�t�@�̉��
	m_pDepthStencilState.Reset();
}
/*
*	@brief �^�C���̎���������������
*	@details �^�C���̎�ނƂ��̏��������ɓo�^����B
*	@param �Ȃ�
*	@return �Ȃ�
*/
void CSVMap::InitializeTileDictionary()
{
	// �^�C���̎�ނƂ��̏��������ɓo�^
	// �ʏ�̏�
	m_tileDictionary["b"] = TileInfo{ "Block", true };
	// �X�^�[�g�n�_
	m_tileDictionary["s"] = TileInfo{ "StartBlock", true };
	// �S�[���n�_
	m_tileDictionary["g"] = TileInfo{ "GoalBlock", true };
	// ���i�i�c�j
	m_tileDictionary["v"] = TileInfo{ "DefaultStraightVerticalBlock", true };
	// ���i�i���j
	m_tileDictionary["h"] = TileInfo{ "DefaultStraightHorizontalBlock", true };
	// �\��
	m_tileDictionary["x"] = TileInfo{ "CrossBlock", true };
	// �E���J�[�u
	m_tileDictionary["rd"] = TileInfo{ "RightDownBlock", true };
	// �����J�[�u
	m_tileDictionary["ld"] = TileInfo{ "LeftDownBlock", true };
	// �E��J�[�u
	m_tileDictionary["ru"] = TileInfo{ "RightUpBlock", true };
	// ����J�[�u
	m_tileDictionary["lu"] = TileInfo{ "LeftUpBlock", true };
	// ��
	m_tileDictionary["0"] = TileInfo{ "", false };
}


/*
*	@brief CSV�`���̃}�b�v��ǂݍ���
*	@details �w�肳�ꂽ�t�@�C���p�X����CSV�`���̃}�b�v�f�[�^��ǂݍ��݁A�^�C���̏�����͂��ă}�b�v���\�z����B
*	@param filePath �ǂݍ���CSV�t�@�C���̃p�X
*	@return �Ȃ�
*/
void CSVMap::LoadMap(const std::string& filePath)
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
	m_mapData.resize(MAXCOL);
	// �e�s�ɗ񐔊m��
	for (int i = 0; i < MAXCOL; i++)m_mapData[i].resize(MAXRAW);
	// �}�b�v�̏�����
	std::string line;
	// �s�ԍ���������
	int row = 0;
	// �}�b�v���S�␳�l�v�Z
	float mapWidth = MAXRAW * 2.0f;
	float mapHeight = MAXCOL * 2.0f;
	// ���S�␳�l���v�Z
	float offsetX = mapWidth / 2.0f - 1.0f;
	float offsetZ = mapHeight / 2.0f - 1.0f;

	// �}�b�v�̍s�Ɨ��ǂݍ���
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
			if (it != m_tileDictionary.end())
			{

				// �^�C�������擾
				const TileInfo& tileInfo = it->second;
				// �^�C���̈ʒu�v�Z�i�}�b�v���S�␳�j
				float x = static_cast<float>(col * 2) - offsetX;
				float z = static_cast<float>(row * 2) - offsetZ;
				// ���[���h���W���v�Z
				Vector3 pos(x, 0.0f, z);
				// �^�C���𐶐�
				std::unique_ptr<TileBase> tileBase = TileFactory::CreateTileByName(tileInfo.modelName);
				// �}�b�v�f�[�^�Ƀ^�C������ۑ�
				m_mapData[row][col] = MapTileData{ tileInfo, pos, tileInfo.hasCollision,std::move(tileBase) };
				// ���[���h�s����쐬�i�X�P�[�����O�ƈʒu�̐ݒ�j
				Matrix world = Matrix::CreateScale(tileInfo.scale) * Matrix::CreateTranslation(pos);
				// ���f���擾
				DirectX::Model* model = m_pCommonResources->GetModelManager()->GetModel(tileInfo.modelName);
				// �^�C���f�[�^�ۑ�
				m_tiles.push_back(TileRenderData{ model, world });
				// �����蔻��
				if (tileInfo.hasCollision)
				{
					// �����蔻��p�̃{�b�N�X���쐬
					BoundingBox box;
					// �{�b�N�X�̒��S�Ɗg�嗦��ݒ�
					box.Center = pos;
					// �g�嗦��ݒ�
					box.Extents = tileInfo.scale;
					// �{�b�N�X�̊g�嗦��2�{�ɂ���i�����蔻��p�j
					//m_wallBox.push_back(box);
				}
			}
			else
			{
				// �^�C���̈ʒu�v�Z�i�}�b�v���S�␳�j
				float x = static_cast<float>(col * 2) - offsetX;
				float z = static_cast<float>(row * 2) - offsetZ;
				// ���[���h���W���v�Z
				Vector3 pos(x, 0.0f, z);
				// ���[���h�s����쐬�i�X�P�[�����O�ƈʒu�̐ݒ�j
				Matrix world = Matrix::CreateScale(Vector3::One) * Matrix::CreateTranslation(pos);
				// �Z���̕����񂪎����ɑ��݂��Ȃ��ꍇ�͋�̃^�C����ǉ�
				m_tiles.push_back(TileRenderData{ nullptr, Matrix::Identity });

				// �f�t�H���g�̏��^�C�����g�p
				const TileInfo& emptyTileInfo = m_tileDictionary[""];
				// �}�b�v�f�[�^�ɋ�̃^�C������ۑ�
				m_mapData[row][col] = MapTileData{ emptyTileInfo, pos, false };
			}
			// �}�b�v�̗�ɒl��ݒ�
			++col;
		}
		// �}�b�v�̍s�ɒl��ݒ�
		++row;
	}
}
/*
*	@brief �����蔻���`�悷��
*	@details �f�o�b�O���[�h�œ����蔻��̃{�b�N�X��`�悷��B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void CSVMap::DrawCollision(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
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
*	@brief �}�b�v��`�悷��
*	@details �}�b�v�̃^�C����`�悷��B
*	@param view �r���[�s��
*	@param proj �v���W�F�N�V�����s��
*	@return �Ȃ�
*/
void CSVMap::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// �f�o�C�X�R���e�L�X�g���擾
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// ���ʂ̃X�e�[�g���擾
	auto states = m_pCommonResources->GetCommonStates();
	// �S�^�C����`�悷��
	for (const auto& tile : m_tiles)
	{
		// ���f�������݂���ꍇ�̂ݕ`��
		if (tile.model)
		{
			// ���f���̕`��
			tile.model->Draw(context, *states, tile.world, view, proj, false, [&]
				{
					// �e�������������C�e�B���O
					m_pShadowMapLight->ApplyShader(context, states);
				});
		}
	}
}

/*
*	@brief �w��ʒu�̃^�C�������擾����
*	@details �w�肳�ꂽ��ƍs�̈ʒu�ɂ���^�C���̏����擾����B
*	@param row �s�ԍ�
*	@param col ��ԍ�
*	@return �w��ʒu�̃^�C�����ւ̎Q��
*/
const  MapTileData& CSVMap::GetTileData(int row, int col) const
{
	assert(col >= 0 && col < MAXCOL && row >= 0 && row < MAXRAW);
	return m_mapData[row][col];
}
/*
*	@brief �w����W�̃^�C�������擾����
*	@details �w�肳�ꂽ���[���h���W�ɍł��߂��^�C���̏����擾����B
*	@param pos ���[���h���W
*	@return �w����W�̃^�C�����ւ̎Q��
*/
const MapTileData& CSVMap::GetTileData(const DirectX::SimpleMath::Vector3& pos) const
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �ŏ������ƑΉ�����^�C���̃C���f�b�N�X��������
	float minDistance = std::numeric_limits<float>::max();
	int closestRow = -1;
	int closestCol = -1;

	// ���������}�b�v�̊O���������̃f�[�^��n��
	if (pos.x < -MAXCOL || pos.x >= MAXRAW || pos.z < -MAXCOL || pos.z >= MAXCOL)
	{
		// ��̃^�C������Ԃ�
		return m_outOfMapData;
	}


	// �}�b�v�f�[�^�𑖍����čł��߂��^�C����T��
	for (int row = 0; row < MAXCOL; ++row)
	{
		for (int col = 0; col < MAXRAW; ++col)
		{
			const MapTileData& tile = m_mapData[row][col];
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

	return m_mapData[closestRow][closestCol];
}

int CSVMap::GetRowFromPosition(const DirectX::SimpleMath::Vector3& pos) const
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �}�b�v�̍s�����擾
	int rowCount = static_cast<int>(m_mapData.size());
	// �w��ʒu�̍s�ԍ����v�Z
	int row = static_cast<int>((pos.z + MAXCOL) / 2.0f);
	// �s�ԍ����͈͊O�̏ꍇ��0��Ԃ�
	if (row < 0 || row >= rowCount)
	{
		return -1; // �͈͊O�Ȃ�0��Ԃ�
	}
	return row;
}

int CSVMap::GetColFromPosition(const DirectX::SimpleMath::Vector3& pos) const
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �}�b�v�̗񐔂��擾
	int colCount = static_cast<int>(m_mapData[0].size());
	// �w��ʒu�̗�ԍ����v�Z
	int col = static_cast<int>((pos.x + MAXRAW) / 2.0f);
	// ��ԍ����͈͊O�̏ꍇ��0��Ԃ�
	if (col < 0 || col >= colCount)
	{
		return -1; // �͈͊O�Ȃ�0��Ԃ�
	}
	return col;
}

/*
*	@brief �w�肵���ʒu�Ɏw�肵�����f����z�u����
*	@details �w�肳�ꂽ�s�Ɨ�̈ʒu�ɁA�w�肳�ꂽ���f�����̃^�C����z�u����B
*	@param row �s�ԍ�
*	@param col ��ԍ�
*	@param modelName ���f����
*	@return �Ȃ�
*/
void CSVMap::SetTileModel(int row, int col, const std::string& modelName)
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	assert(col >= 0 && col < MAXCOL && row >= 0 && row < MAXRAW);
	// ���f��������łȂ��ꍇ
	if (!modelName.empty())
	{
		// �^�C�������X�V
		m_mapData[row][col].tileInfo.modelName = modelName;
		// ���f�����擾
		DirectX::Model* model = m_pCommonResources->GetModelManager()->GetModel(modelName);
		// �^�C���𐶐�
		std::unique_ptr<TileBase> tileBase = TileFactory::CreateTileByName(m_mapData[row][col].tileInfo.modelName);
		// �^�C���̃|�C���^�[��ݒ�
		m_mapData[row][col].tileBasePtr = std::move(tileBase);
		// �^�C���̈ʒu���v�Z
		Vector3 pos = m_mapData[row][col].pos;
		// ���[���h���W���v�Z
		Matrix world = Matrix::CreateScale(m_mapData[row][col].tileInfo.scale) * Matrix::CreateTranslation(pos);
		// �^�C���̃����_�����O�f�[�^���X�V
		m_tiles.push_back(TileRenderData{ model, world });
	}
	else
	{
		// ���f��������̏ꍇ�͉������Ȃ�
	}
}
/*
*	@brief �X�^�[�g�n�_��Ԃ�
*	@details modelname��Start�̃^�C���̈ʒu��Ԃ��B
*	@param �Ȃ�
*	@return �X�^�[�g�n�_�ւ̎Q��
*/
const MapTileData& CSVMap::GetStart() const
{
	// DirectX��SimpleMath�̖��O��Ԃ��g�p
	using namespace DirectX::SimpleMath;
	// �}�b�v�f�[�^�𑖍����ăX�^�[�g�n�_��T��
	for (const auto& row : m_mapData)
	{
		// �e�s�̃^�C���𑖍�
		for (const auto& tile : row)
		{
			// �^�C���̃��f������"Start"�̏ꍇ
			if (tile.tileInfo.modelName == "StartBlock")
			{
				// �X�^�[�g�n�_�̈ʒu��Ԃ�
				return tile;
			}
		}
	}
	// �X�^�[�g�n�_��������Ȃ��ꍇ��(0,0)�̈ʒu��Ԃ�
	return   m_mapData[0][0];
}
/*
*	@brief �[�x�X�e���V���o�b�t�@���쐬����
*	@details Direct3D�f�o�C�X���g�p���Đ[�x�X�e���V���o�b�t�@���쐬����
*	@param pDevice Direct3D�f�o�C�X�ւ̃|�C���^
*	@return �Ȃ�
*/
void CSVMap::CreateDepthStencilBuffer(ID3D11Device* pDevice)
{
	// �[�x�X�e���V�������`����
	D3D11_DEPTH_STENCIL_DESC desc = {};
	// ���F���`�掞�ɃX�e���V���o�b�t�@�̒l���C���N�������g����
	desc.DepthEnable = true;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �[�x�l�ȉ��Ȃ�X�V����

	desc.StencilEnable = true;									// �X�e���V���e�X�g���s��
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff�i�}�X�N�l�j
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff�i�}�X�N�l�j

	// �|���S���̕\�ʂ̐ݒ�
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	// ���ʂ������ݒ�
	desc.BackFace = desc.FrontFace;
	// �[�x�X�e���V���X�e�[�g���쐬����
	DX::ThrowIfFailed(pDevice->CreateDepthStencilState(&desc, m_pDepthStencilState.ReleaseAndGetAddressOf()));
}