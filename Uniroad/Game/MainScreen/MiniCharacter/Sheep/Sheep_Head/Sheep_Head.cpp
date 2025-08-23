/*
*	@file Sheep_Head.cpp
*	@brief ひつじの頭クラスの実装
*/
#include "pch.h"
#include "Sheep_Head.h"
#include "Game/MainScreen/MiniCharacter/Sheep/Sheep_Body/Sheep_Body.h"
/*
*	@brief ひつじの頭クラスのコンストラクタ
*	@details ひつじの頭クラスのコンストラクタ
*	@param parent 親コンポーネントへのポインタ
*	@param initialPosition 初期位置
*	@param initialAngle 初期角度（ラジアン）
*	@return なし
*/
SheepHead::SheepHead(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_pParent(parent)// 親コンポーネント
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())// ノード番号
	, m_partNumber(MiniCharacter::GetPartsNumber()) // 部品番号
	, m_partID(MiniCharacter::UNICYCLE_WHEEL) // 部品ID
	, m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pModel(nullptr) // モデルへのポインタ
	, m_time(0.0f) // 時間
	, m_initialPosition(initialPosition)// 初期位置
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// 初期角度
	, m_currentPosition{} // 現在の位置
	, m_currentAngle{} // 現在の回転角
	, m_rotationMiniCharacterAngle{} // プレイヤー回転角
	, m_mass{} // 質量
	, m_MiniCharacterVelocity{}// プレイヤー速度
{
}
/*
*	@brief ひつじの頭クラスのデストラクタ
*	@details ひつじの頭クラスのデストラクタ
*	@param なし
*	@return なし
*/
SheepHead::~SheepHead()
{
	// 後処理を行う
	Finalize();
}
/*
*	@brief ひつじの頭クラスの初期化
*	@details ひつじの頭クラスの初期化を行い、必要なリソースを設定する。
*	@param commonResources 共通リソースへのポインタ
*	@return なし
*/
void SheepHead::Initialize(CommonResources* commonResources)
{
	// 必要な名前空間を使用
	using namespace DirectX;
	// 共通リソースをセット
	m_pCommonResources = commonResources;
	// モデルを読み込む
	ChangeExpression("Sheep_Head");
}
/*
*	@brief ひつじの頭クラスの更新
*	@details ひつじの頭クラスの更新を行い、位置や角度を更新する。
*	@param elapsedTime 経過時間
*	@param currentPosition 現在の位置
*	@param currentAngle 現在の角度
*	@return なし
*/
void SheepHead::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// 必要な名前空間を使用
	using namespace DirectX::SimpleMath;
	// 時間を加算
	m_time += elapsedTime;
	// 現在の位置を更新する
	m_currentPosition = currentPosition + m_initialPosition;
	// 現在の回転角を更新する
	m_currentAngle = m_rotationMiniCharacterAngle * m_initialAngle * currentAngle;
	// ベースを取得
	auto pBase = dynamic_cast<MiniCharacterBase*>(m_pParent->GetParent()->GetParent());
	// シャドウマップにモデルを登録する
	pBase->GetShadowMapLight()->SetShadowModel(m_pModel, m_worldMatrix);
	// ミニキャラクラスを取得する
	auto pMiniCharacter = dynamic_cast<MiniCharacter*>(m_pParent->GetParent());
	// ミニキャラクターが存在しない場合は何もしない
	if (pMiniCharacter == nullptr) return;
	// 表情に応じてモデルを切り替える
	if (pMiniCharacter->GetExpression() == MiniCharacter::Expression::NORMAL)
		ChangeExpression("Sheep_Head");	// 通常の顔
	else if (pMiniCharacter->GetExpression() == MiniCharacter::Expression::HAPPY)
		ChangeExpression("Sheep_HappyHead");	// ゴールの時の顔
	else if (pMiniCharacter->GetExpression() == MiniCharacter::Expression::BAD)
		ChangeExpression("Sheep_BadHead");	// 落ちそうになった時や落ちている時の顔

}
/*
*	@brief ひつじの頭クラスの描画
*	@details ひつじの頭クラスの描画を行う。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void SheepHead::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// 必要な名前空間を使用
	using namespace DirectX::SimpleMath;
	// デバイスコンテキストとステートを取得する
	auto context = m_pCommonResources->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_pCommonResources->GetCommonStates();
	// ワールド行列を生成する
	m_worldMatrix = Matrix::CreateScale(Vector3::One) * // スケール
		Matrix::CreateFromQuaternion(m_currentAngle) * // 回転
		Matrix::CreateTranslation(m_currentPosition); // 位置
	// モデルを描画する
	m_pModel->Draw(context, *states, m_worldMatrix, view, proj, false);
}
/*
*	@brief ひつじの頭クラスの後処理
*	@details ひつじの頭クラスの後処理を行う。
*	@param なし
*	@return なし
*/
void SheepHead::Finalize()
{
	// モデルが存在する場合は解放する
	if (m_pModel)m_pModel = nullptr;
}
/*
*	@brief 表情を切り替える
*	@details モデルキーを渡して表情を切り替える
*	@param key 表情のキー
*	@return なし
*/
void SheepHead::ChangeExpression(const std::string& key)
{
	// モデルマネージャーからモデルを取得する
	m_pModel = m_pCommonResources->GetModelManager()->GetModel(key);
}
