/*
*	@file Sheep_Head.h
*	@brief ひつじの頭クラスのヘッダーファイル
*/
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
#include "Game/Scenes/PlayScene/PlayScene.h"
#include "Game/MainScreen/MiniCharacter/Interface/ILeaf.h"
#include "KumachiLib/Easing/Easing.h"

// 前方宣言
class CommonResources;

// ひつじの頭クラス
class SheepHead : public ILeaf
{
public:
	// アクセサ
	// ノード番号を取得する
	int GetNodeNumber() const { return m_nodeNumber; }
	// 部品IDを取得する
	int GetPartID() const { return m_partID; }
	// 部品番号を取得する
	int GetPartNumber() const { return m_partNumber; }
	// 親を取得する
	IComponent* GetParent() const { return m_pParent; }
	// 現在の位置を取得する
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_currentPosition; }
	// 現在の位置を設定する
	void SetPosition(const DirectX::SimpleMath::Vector3& currretPosition) { m_currentPosition = currretPosition; }
	// 現在の回転角を取得する
	DirectX::SimpleMath::Quaternion GetAngle() const { return m_currentAngle; }
	// 現在の回転角を設定する
	void SetAngle(const DirectX::SimpleMath::Quaternion& currentAngle) { m_currentAngle = currentAngle; }
	// 現在の速度を取得する
	DirectX::SimpleMath::Vector3 GetVelocity() const { return 	DirectX::SimpleMath::Vector3::Zero; }
	// 移動フラグを取得
	bool IsMoving() const { return false; }
	// 質量を取得する
	float GetMass() const { return m_mass; }
	// 質量を設定する
	void SetMass(const float& mass) { m_mass = mass; }
	// モデルを取得する
	DirectX::Model* GetModel() { return m_pModel; }
	// CSVマップを取得する
	CSVMap* GetCSVMap() { return nullptr; }
	// CSVアイテムを取得する
	CSVItem* GetCSVItem() { return nullptr; }
	// 次に現れるタイルのクラスを取得する
	NextTiles* GetNextTiles() { return nullptr; }
	// シャドウマップライトを取得する
	ShadowMapLight* GetShadowMapLight() { return nullptr; }
	// ワールド行列を取得する
	DirectX::SimpleMath::Matrix& GetWorldMatrix() { return m_worldMatrix; }
public:
	// public関数
	// コンストラクタ
	SheepHead(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle);
	// デストラクタ
	~SheepHead();
	// 初期化する
	void Initialize(CommonResources* commonResources);
	// 更新する
	void Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle);
	// 描画する
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
	// 後処理を行う
	void Finalize();


private:
	// private関数
	// 共通リソース
	CommonResources* m_pCommonResources;
	// 親
	IComponent* m_pParent;
	// 時間
	float m_time;
	// モデル
	DirectX::Model* m_pModel;
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
	// ノード配列
	std::vector<std::unique_ptr<IComponent>> m_pNodes;
	//	ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;

};

