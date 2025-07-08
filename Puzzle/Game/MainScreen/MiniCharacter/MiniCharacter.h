#pragma once
// 標準ライブラリ
#include <cassert>
// DirectX
#include <SimpleMath.h>
#include <Model.h>
#include <Effects.h>
#include <memory>
#include <DeviceResources.h>
// 外部ライブラリ
#include "Libraries/MyLib/MemoryLeakDetector.h"
#include "Libraries/MyLib/InputManager.h"
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "Game/Scenes/Scene/Scene.h"
#include "Game/MainScreen/MiniCharacter/Interface/IComposite.h"
#include "Game/MainScreen/MiniCharacter/MiniCharacterBody/MiniCharacterBody.h"
// 前方宣言
class CommonResources;

class MiniCharacter : public IComposite
{
public:
	// 部品ID
	enum PartID : int
	{
		MINICHARACTER = -1,
		BODY = 0,
		HEAD = 1,
		ARM_LEFT = 2,
		ARM_RIGHT = 3,
	};
public:
	// アクセサ
	// ノード番号を取得する
	int GetNodeNumber() const { return m_nodeNumber; }
	// 部品IDを取得する
	int GetPartID() const { return  m_partID; }
	// 部品番号を取得する
	int GetPartNumber() const { return m_partNumber; }
	// 親を取得する
	IComponent* GetParent() const { return m_parent; }
	// 現在の位置を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }
	// 現在の位置を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& currretPosition) { m_currentPosition = currretPosition; }
	// 現在の回転角を取得する
	DirectX::SimpleMath::Quaternion GetAngle() const { return m_currentAngle; }
	// 現在の回転角を設定する
	void SetAngle(const DirectX::SimpleMath::Quaternion& currentAngle) { m_currentAngle = currentAngle; }
	// 質量を取得する
	float GetMass() const { return m_mass; }
	// 質量を設定する
	void SetMass(const float& mass) { m_mass = mass; }
	// モデルを取得する
	DirectX::Model* GetModel() { return nullptr; }
	// ワールド行列を取得する
	DirectX::SimpleMath::Matrix& GetWorldMatrix() { return m_worldMatrix; }
public:
	// コンストラクタ
	MiniCharacter(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle);
	// デストラクタ
	~MiniCharacter();
	// 初期化する
	void Initialize(CommonResources* commonResources);
	// 更新する
	void Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle);
	// 部品を追加する
	void Attach(std::unique_ptr<IComponent> MiniCharacterPart);
	// 部品を削除する
	void Detach(std::unique_ptr<IComponent> MiniCharacterPart);
	// 描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 後処理を行う
	void Finalize();
public:
	// ノードカウントアップした後ノードカウントを取得する
	static int GetNodeCountAfterCountUp() { return ++s_nodeCount; }
	// ノードカウントを取得する
	static int GetNodeCount() { return s_nodeCount; }
	// 部品番号をリセットする(-1から開始する)
	static void ResetPartsNumber() { s_partsNumber = -1; }
	// 部品番号を取得する
	static int GetPartsNumber() { return s_partsNumber; }
	// 部品番号増やす
	static void IncrementPartsNumber() { s_partsNumber++; }
private:
	// 砲塔カウント
	static int s_nodeCount;
	// 砲塔部品カウント
	static int s_partsNumber;
private:
	// 共通リソース
	CommonResources* m_commonResources;
	// 親
	IComponent* m_parent;
	// ノード番号
	int m_nodeNumber;
	// 部品番号
	int m_partNumber;
	// 部品ID
	int m_partID;
	// 初期位置
	DirectX::SimpleMath::Vector3 m_initialPosition;
	// 初期回転角
	DirectX::SimpleMath::Quaternion m_initialAngle;
	// 現在の位置
	DirectX::SimpleMath::Vector3 m_currentPosition;
	// 現在の回転角
	DirectX::SimpleMath::Quaternion m_currentAngle;
	// プレイヤー回転角
	DirectX::SimpleMath::Quaternion m_rotationMiniCharacterAngle;
	// プレイヤー速度
	DirectX::SimpleMath::Vector3 m_MiniCharacterVelocity;
	// 質量
	float m_mass;
	// 部品配列
	std::vector<std::unique_ptr<IComponent>> m_pMiniCharacterParts;
	//	ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
};