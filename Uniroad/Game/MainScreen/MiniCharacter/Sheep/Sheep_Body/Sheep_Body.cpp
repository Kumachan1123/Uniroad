/*
*	@file Sheep_Body.cpp
*	@brief ひつじの胴体クラス
*/
#include "pch.h"
#include "Sheep_Body.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacter.h"
/*
*	@brief ひつじの胴体クラス
*	@detail ひつじの胴体クラスのコンストラクタ
*	@param parent 親コンポーネント
*	@param initialPosition 初期位置
*	@param initialAngle 初期角度
*	@return なし
*/
SheepBody::SheepBody(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)// 親コンポーネント
	, m_nodeNumber{ MiniCharacter::GetNodeCountAfterCountUp() }// ノード番号を取得
	, m_partNumber{ MiniCharacter::GetPartsNumber() }// 部品番号を取得
	, m_partID{ MiniCharacter::BODY } // 部品IDを取得
	, m_pCommonResources(nullptr)// 共通リソース
	, m_pModel(nullptr)// モデル
	, m_initialPosition(initialPosition)// 初期位置
	, m_time(0.0f) // 時間
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle)) // 初期角度
	, m_currentPosition{ 0.0f, 0.0f, 0.0f } // 現在の位置
	, m_currentAngle{} // 現在の角度
	, m_mass(0.0f) // 質量
{
}
/*
*	@brief デストラクタ
*	@detail ひつじの胴体クラスのデストラクタ
*	@param なし
*	@return なし
*/
SheepBody::~SheepBody()
{
	// 後処理を行う
	Finalize();
}
/*
*	@brief 初期化する
*	@detail ひつじの胴体クラスの初期化
*	@param commonResources 共通リソース
*	@return なし
*/
void SheepBody::Initialize(CommonResources* commonResources)
{
	// DirectX::SimpleMathを使用するための名前空間を指定
	using namespace DirectX::SimpleMath;
	// 共通リソースを設定する
	assert(commonResources);
	m_pCommonResources = commonResources;
	// モデルを読み込む
	m_pModel = m_pCommonResources->GetModelManager()->GetModel("Sheep_Body");
	// 頭部を追加
	Attach(std::make_unique<SheepHead>(this, Vector3(0.0f, 1.0f, 0.0f), 0.0f));
	// 一輪車の胴体を追加
	Attach(std::make_unique<UnicycleBody>(this, Vector3(0.0f, -1.0f, 0.0f), 0.0f));

}
/*
*	@brief 更新する
*	@detail ひつじの胴体クラスの更新
*	@param elapsedTime 経過時間
*	@param currentPosition 現在の位置
*	@param currentAngle 現在の角度
*	@return なし
*/
void SheepBody::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// DirectX::SimpleMathを使用するための名前空間を指定
	using namespace DirectX::SimpleMath;
	// 時間を更新する
	m_time += elapsedTime;
	// 現在の位置を更新する
	m_currentPosition = currentPosition + m_initialPosition;
	// 現在の角度を更新する
	m_currentAngle = m_rotationBodyAngle * currentAngle;
	// ワールド行列を生成する
	m_worldMatrix = Matrix::CreateScale(1) * // スケール行列を生成
		Matrix::CreateFromQuaternion(m_currentAngle) * // 回転行列を生成
		Matrix::CreateTranslation(m_currentPosition); // 平行移動行列を生成
	// ベースを取得する
	auto pBase = dynamic_cast<MiniCharacterBase*>(m_pParent->GetParent());
	// シャドウマップにモデルを登録する
	pBase->GetShadowMapLight()->SetShadowModel(m_pModel, m_worldMatrix);
	// 「胴体」部品を更新する
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
	{
		// 部品を更新する
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	}
}
/*
*	@brief 部品を追加する
*	@detail ひつじの胴体クラスに部品を追加する
*	@param MiniCharacterPart 追加する部品
*	@return なし
*/
void SheepBody::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品を初期化する
	MiniCharacterPart->Initialize(m_pCommonResources);
	// 部品を追加する
	m_pMiniCharacterParts.emplace_back(std::move(MiniCharacterPart));
}
/*
*	@brief 部品を削除する
*	@detail ひつじの胴体クラスから部品を削除する
*	@param MiniCharacterPart 削除する部品
*	@return なし
*/
void SheepBody::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 今は何もしない
}
/*
*	@brief 描画する
*	@detail ひつじの胴体クラスの描画
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void SheepBody::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// DirectX::SimpleMathを使用するための名前空間を指定
	using namespace DirectX::SimpleMath;
	/// Direct3Dデバイスコンテキストを取得
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	// 共通ステートを取得
	auto states = m_pCommonResources->GetCommonStates();
	// モデルを描画する
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
	// 「胴体」部品を描画する
	for (auto& part : m_pMiniCharacterParts)part->Render(view, proj);
}
/*
*	@brief 後処理を行う
*	@detail ひつじの胴体クラスの後処理
*	@param なし
*	@return なし
*/
void SheepBody::Finalize()
{
	// モデルを解放する
	if (m_pModel)m_pModel = nullptr;
	// 部品を全て削除する
	m_pMiniCharacterParts.clear();
}
