/*
*	@file MiniCharacterBase.cpp
*	@brief ミニキャラクターのベースクラス
*/
#include <pch.h>
#include "MiniCharacterBase.h"
/*
*	@brief ミニキャラクターのベースクラスのコンストラクタ
*	@details ミニキャラクターのベースクラスのコンストラクタ。
*	@param parent 親コンポーネントへのポインタ
*	@param initialPosition 初期位置
*	@param initialAngle 初期角度
*	@return なし
*/
MiniCharacterBase::MiniCharacterBase(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	:m_pParent(parent)// 親コンポーネントへのポインタ
	, m_nodeNumber(MiniCharacterBase::GetNodeNumber())// ノード番号を取得する
	, m_partNumber(MiniCharacterBase::GetPartNumber())// 部品番号を取得する
	, m_partID(MiniCharacterBase::GetPartID())// 部品IDを取得する
	, m_initialPosition(initialPosition)// 初期位置を設定する
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// 初期回転角を設定する
	, m_currentPosition(initialPosition)// 現在の位置を初期位置に設定する
	, m_currentAngle(m_initialAngle)// 現在の回転角を初期回転角に設定する
	, m_mass(0.0f) // 質量を初期化する
	, m_pCommonResources(nullptr) // 共通リソースへのポインタを初期化する
	, m_pCSVMap(nullptr) // CSVマップへのポインタを初期化する
	, m_pCSVItem(nullptr) // CSVアイテムへのポインタを初期化する
	, m_pNextTiles(nullptr) // 次に現れるタイルのクラスへのポインタを初期化する
	, m_pPlaneArea(nullptr) // 平面エリアへのポインタを初期化する
	, m_pShadowMapLight(nullptr) // シャドウマップライトへのポインタを初期化する
	, m_pCamera(nullptr) // カメラへのポインタを初期化する
	, m_isMoving(false)// ミニキャラの移動フラグを初期化する
	, m_isGameOver(false) // ゲームオーバーフラグを初期化する
	, m_isGameClear(false)	// ゲームクリアフラグを初期化する
	, m_titleAnimationState(NONE)// タイトルシーンにおけるアニメーションステートを初期化する
{
}
/*
*	@brief ミニキャラクターのベースクラスのデストラクタ
*	@details ミニキャラクターのベースクラスのデストラクタ。
*	@param なし
*	@return なし
*/
MiniCharacterBase::~MiniCharacterBase()
{
	// ノードをクリアする
	Finalize();
	// 共通リソースへのポインタをnullptrにする
	m_pCommonResources = nullptr;
	// CSVマップへのポインタをnullptrにする
	m_pCSVMap = nullptr;
	// CSVアイテムへのポインタをnullptrにする
	m_pCSVItem = nullptr;
	// 次に現れるタイルのクラスへのポインタをnullptrにする
	m_pNextTiles = nullptr;
	// 平面エリアへのポインタをnullptrにする
	m_pPlaneArea = nullptr;
}
/*
*	@brief ミニキャラクターのベースクラスの初期化
*	@details ミニキャラクターのベースクラスの初期化。
*	@param commonResources 共通リソースへのポインタ
*	@return なし
*/
void MiniCharacterBase::Initialize(CommonResources* commonResources)
{
	// 共通リソースへのポインタを設定する
	m_pCommonResources = commonResources;
}
/*
*	@brief ミニキャラクターのベースクラスの更新
*	@details ミニキャラクターのベースクラスの更新。
*	@param elapsedTime 経過時間
*	@param currentPosition 現在の位置
*	@param currentAngle 現在の回転角
*	@return なし
*/
void MiniCharacterBase::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// 現在の位置を更新する
	m_currentPosition = currentPosition;
	// 現在の回転角を更新する
	m_currentAngle = currentAngle;
	// ノードを更新する
	for (auto& node : m_nodes)
	{
		// ノードの更新処理を呼び出す
		node->Update(elapsedTime, m_currentPosition, m_currentAngle);
		// nodeがMiniCharacterなら
		if (dynamic_cast<MiniCharacter*>(node.get()))
		{
			// 座標をカメラに渡す座標とする
			m_cameraPosition = node->GetPosition();
			// 座標をアイコンに渡す座標とする
			m_iconPosition = node->GetPosition();
		}
	}
}
/*
*	@brief ミニキャラクターのベースクラスに部品を追加する
*	@details ミニキャラクターのベースクラスに部品を追加する。
*	@param MiniCharacterPart 追加する部品のユニークポインタ
*	@return なし
*/
void MiniCharacterBase::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 追加した部品を初期化する
	MiniCharacterPart->Initialize(m_pCommonResources);
	// ノードに追加する
	m_nodes.emplace_back(std::move(MiniCharacterPart));
}
/*
*	@brief ミニキャラクターのベースクラスから部品を削除する
*	@details ミニキャラクターのベースクラスから部品を削除する。
*	@param MiniCharacterPart 削除する部品のユニークポインタ
*	@return なし
*/
void MiniCharacterBase::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品を削除する
	auto it = std::find_if(m_nodes.begin(), m_nodes.end(),
		[&MiniCharacterPart](const std::unique_ptr<IComponent>& node)
		{
			return node.get() == MiniCharacterPart.get();
		});
	// 削除する部品が見つかった場合は部品を削除する
	if (it != m_nodes.end())	m_nodes.erase(it);
}
/*
*	@brief ミニキャラクターのベースクラスの描画
*	@details ミニキャラクターのベースクラスの描画。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void MiniCharacterBase::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{	// ノードを描画する
	for (auto& node : m_nodes)node->Render(view, proj);
}
/*
*	@brief ミニキャラクターのベースクラスの後処理
*	@details ミニキャラクターのベースクラスの後処理。
*	@param なし
*	@return なし
*/
void MiniCharacterBase::Finalize()
{
	// ノードをクリアする
	m_nodes.clear();
}
