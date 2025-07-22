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
#include "Game/MainScreen/CSVMap/CSVMap.h"
#include "Game/MainScreen/CSVItem/CSVItem.h"
#include "Game/ControllScreen/NextTiles/NextTiles.h"
class CommonResources;
class MiniCharacterBase : public IComposite
{
public:
	// アクセサ
	// ノード番号を取得する
	int GetNodeNumber() const { return m_nodeNumber; }
	// 部品IDを取得する
	int GetPartID() const { return  m_partID; }
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
	// 質量を取得する
	float GetMass() const { return m_mass; }
	// 質量を設定する
	void SetMass(const float& mass) { m_mass = mass; }
	// モデルを取得する
	DirectX::Model* GetModel() { return nullptr; }
	// CSVマップを取得する
	CSVMap* GetCSVMap() { return m_pCSVMap; }
	// CSVマップを設定する
	void SetCSVMap(CSVMap* csvMap) { m_pCSVMap = csvMap; }
	// CSVアイテムを取得する
	CSVItem* GetCSVItem() { return m_pCSVItem; }
	// CSVアイテムを設定する
	void SetCSVItem(CSVItem* csvItem) { m_pCSVItem = csvItem; }
	// 次に現れるタイルのクラスを取得する
	NextTiles* GetNextTiles() { return m_pNextTiles; }
	// 次に現れるタイルのクラスを設定する
	void SetNextTiles(NextTiles* nextTiles) { m_pNextTiles = nextTiles; }
public:
	// コンストラクタ
	MiniCharacterBase(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle);
	// デストラクタ
	~MiniCharacterBase();
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
	// 親
	IComponent* m_pParent;
	// 共通リソース
	CommonResources* m_pCommonResources;
	// CSVマップ
	CSVMap* m_pCSVMap;
	// CSVアイテム
	CSVItem* m_pCSVItem;
	// 次に現れるタイルのクラス
	NextTiles* m_pNextTiles;
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
	// 質量
	float m_mass;
	// ノード配列
	std::vector<std::unique_ptr<IComponent>> m_nodes;
};