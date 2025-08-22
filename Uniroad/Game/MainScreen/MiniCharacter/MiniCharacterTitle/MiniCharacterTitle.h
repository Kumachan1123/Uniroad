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
#include <Libraries\MyLib\DebugString.h>
// 自作ヘッダーファイル
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/Math/KumachiLib.h"
#include "Game/MainScreen/MiniCharacter/Interface/IComposite.h"
#include "Game/MainScreen/MiniCharacter/Sheep/Sheep.h"
#include "Game/Scenes/TitleScene/TitleAnimationState/TitleAnimationState.h"
#include "Game/Particle/Particle.h"
#include "Game/Particle/Utility.h"

// 前方宣言
class CommonResources;

class MiniCharacterTitle : public IComposite
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
		SHEEP = 4,
		UNICYCLE_BODY = 5,
		UNICYCLE_WHEEL = 6,
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
	// 現在の速度を取得する
	DirectX::SimpleMath::Vector3 GetVelocity() const { return m_currentVelocity; }
	// 現在の速度を設定する
	void SetVelocity(const DirectX::SimpleMath::Vector3& currentVelocity) { m_currentVelocity = currentVelocity; }
	// 現在の回転角を取得する
	DirectX::SimpleMath::Quaternion GetAngle() const { return m_currentAngle; }
	// 現在の回転角を設定する
	void SetAngle(const DirectX::SimpleMath::Quaternion& currentAngle) { m_currentAngle = currentAngle; }
	// 揺れを取得する
	DirectX::SimpleMath::Quaternion GetShakeQuaternion() const { return m_shakeQuaternion; }
	// 揺れを設定する
	void SetShakeQuaternion(const DirectX::SimpleMath::Quaternion& shakeQuaternion) { m_shakeQuaternion = shakeQuaternion; }
	// 質量を取得する
	float GetMass() const { return m_mass; }
	// 質量を設定する
	void SetMass(const float& mass) { m_mass = mass; }
	// モデルを取得する
	DirectX::Model* GetModel() { return nullptr; }
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
	// タイルに入ったかどうかを確認する
	bool HasEnteredTile(const TileBase* tile) const { return m_enteredTiles.count(tile) > 0; }
	// タイルに入ったことを記録する
	void SetEnteredTile(const TileBase* tile) { m_enteredTiles.insert(tile); }
	// 入ったタイルのセットをクリアする
	void ResetEnteredTiles() { m_enteredTiles.clear(); }
	// 前に入ってたタイルの名前
	const std::string& GetPrevTileName() const { return m_prevTileName; }
	// 移動フラグを取得
	bool IsMoving() const { return m_isMoving; }
	// 移動フラグを設定
	void SetMoving(bool isMoving) { m_isMoving = isMoving; }
	// 落下タイマーが有効かどうかを取得
	bool IsFallTimerActive() const { return m_fallTimerActive; }
	// 落下タイマーが有効かどうかを設定
	void SetFallTimerActive(bool active) { m_fallTimerActive = active; }
	// タイトルシーンにおけるアニメーションステートを取得する
	TitleAnimation GetTitleAnimationState() const;
	// タイトルシーンにおけるアニメーションステートを設定する
	void SetTitleAnimationState(TitleAnimation state);

public:
	// コンストラクタ
	MiniCharacterTitle(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle);
	// デストラクタ
	~MiniCharacterTitle();
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
private:
	// private関数
	// 回転の補間
	void InterpolateRotation(const DirectX::SimpleMath::Quaternion& currentAngle);
	// タイルの中心にいるかどうかを確認する
	bool IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon = 0.01f) const;
	// アニメーションを実行する（タイトルシーン用）
	void ExecuteAnimation(float elapsedTime);
	// パーティクルのパラメーターを設定する
	Utility::ParticleParams SetParticleParams() const;
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
	// private定数
	// ノードカウント
	static int s_nodeCount;
	// 部品カウント
	static int s_partsNumber;
private:
	// privateメンバ変数
	// 共通リソース
	CommonResources* m_pCommonResources;
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
	// 前のタイルの位置
	DirectX::SimpleMath::Vector3 m_prevPosition;
	// 現在の位置
	DirectX::SimpleMath::Vector3 m_currentPosition;
	// 現在の速度
	DirectX::SimpleMath::Vector3 m_currentVelocity;
	// 現在の回転角
	DirectX::SimpleMath::Quaternion m_currentAngle;
	// プレイヤー回転角
	DirectX::SimpleMath::Quaternion m_rotationMiniCharacterAngle;
	// プレイヤー速度
	DirectX::SimpleMath::Vector3 m_miniCharacterVelocity;
	// プレイヤーの揺れ
	DirectX::SimpleMath::Quaternion m_shakeQuaternion;
	// 目的地
	DirectX::SimpleMath::Vector3 m_destinationPosition;
	// 質量
	float m_mass;
	// 部品配列
	std::vector<std::unique_ptr<IComponent>> m_pMiniCharacterParts;
	//	ワールド行列
	DirectX::SimpleMath::Matrix m_worldMatrix;
	// 入ったタイルのセット
	std::set<const TileBase*> m_enteredTiles;
	// 前フレームで一番近かったタイルの名前
	std::string m_prevTileName;
	// 入ったタイルのポインタ
	TileBase* m_enteredTilePtr;
	// 入った直後フラグ
	bool m_hasEnteredTile;
	// 移動フラグ
	bool m_isMoving;
	// 落下タイマーが有効か
	bool m_fallTimerActive;
	// 落下タイマー
	float m_fallTimer;
	// 一度だけ落下処理を実行させるためのフラグ
	bool m_hasFallen;
	// タイトルシーンにおけるアニメーションステート
	TitleAnimation m_titleAnimationState;
	// パーティクル
	std::unique_ptr<Particle> m_pParticle;
};