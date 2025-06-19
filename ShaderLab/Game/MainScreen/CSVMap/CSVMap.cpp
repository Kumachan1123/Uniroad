#include <pch.h>
#include "CSVMap.h"

CSVMap::CSVMap(CommonResources* resources)
{
	m_commonResources = resources;
	DrawCollision::Initialize(m_commonResources);// 当たり判定描画の初期化
	LoadModel();
}

CSVMap::~CSVMap()
{
}

void CSVMap::LoadModel()
{
	using namespace DirectX;
	auto device = m_commonResources->GetDeviceResources()->GetD3DDevice();

	// モデルを読み込む準備
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/models");
	//テクスチャパスを設定する
	wcscpy_s(m_modelPath, L"Resources/models/Block.cmo");

	// モデルを読み込む
	m_pModel = m_commonResources->GetModelManager()->GetModel("Block");
}

void CSVMap::LoadMap(const std::string& filePath)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// csvファイルを開く
	std::ifstream file = std::ifstream(filePath);
	if (!file.is_open())
	{
		return;
	}
	std::string line;
	for (int i = 0; i < MAXCOL && std::getline(file, line); ++i)
	{
		std::stringstream ss(line);
		std::string cell;
		int j = 0;
		while (j < MAXRAW && std::getline(ss, cell, ','))
		{
			if (!cell.empty() && cell[0] == 'w') // カンマで区切られた各セルを処理
			{
				// ()の中身の数値をいじることで間隔を調整することができる
				float x = static_cast<float>(j * 2);
				float z = static_cast<float>(i * 2);
				float y = 0.0f; // 必要に応じてこの値を変更

				// キューブを正しい位置に移動するための変換を適用
				DirectX::SimpleMath::Matrix worldMatrix;
				worldMatrix = DirectX::SimpleMath::Matrix::CreateScale(1.0f); // スケールを適用
				worldMatrix *= DirectX::SimpleMath::Matrix::CreateTranslation(x, y, z);

				// ワールド行列を保存
				worldMatrices.push_back(worldMatrix);
				// 境界ボックスを作成
				DirectX::BoundingBox box;
				box.Center = Vector3(x, y, z);
				box.Extents = Vector3(1.0f, 1.0f, 1.0f); // サイズを調整
				m_wallBox.push_back(box);

			}
			else if (!cell.empty() && cell[0] == 'g')
			{
				// ()の中身の数値をいじることで間隔を調整することができる
				float x = static_cast<float>(j * 2);
				float z = static_cast<float>(i * 2);
				float y = 0.0f; // 必要に応じてこの値を変更

				m_goalPosition = Vector3{ x, y, z };
				// ゴールの境界ボックスを作成
				DirectX::BoundingBox box;
				box.Center = Vector3(x, y, z);
				box.Extents = Vector3(1.0f, 1.0f, 1.0f); // サイズを調整
				m_goalBox = box;
			}
			++j;
		}
	}
}

void CSVMap::DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	UNREFERENCED_PARAMETER(view);// 未使用の警告を出さない
	UNREFERENCED_PARAMETER(proj);// 未使用の警告を出さない
#ifdef _DEBUG
	DrawCollision::DrawStart(view, proj);// 描画開始
	for (int i = 0; i < m_wallBox.size(); i++)
	{
		DrawCollision::DrawBoundingBox(m_wallBox[i], Colors::Red);
	}
	DrawCollision::DrawBoundingBox(m_goalBox, Colors::Green);
	DrawCollision::DrawEnd();// 描画終了

#endif
}

void CSVMap::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	auto context = m_commonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_commonResources->GetCommonStates();

	// worlsMatricesに保存されているデータを用いてBoxを描画する
	for (const auto& world : worldMatrices)
	{


		// モデルの描画（テクスチャONバージョン）
		m_pModel->Draw(context, *states, world, view, proj, false);

	}
	this->DrawCollision(view, proj);
}
