#pragma once
class CommonResources;
class CSVMap;
class CSVItem;
class NextTiles;
// IComponentインターフェースを定義する
class IComponent
{
public:
	// ノード番号を取得する
	virtual int GetNodeNumber() const = 0;
	// 部品IDを取得する
	virtual int GetPartID() const = 0;
	// 部品番号を取得する
	virtual int GetPartNumber() const = 0;
	// 親を取得する
	virtual IComponent* GetParent() const = 0;
	// 現在の位置を取得する
	virtual DirectX::SimpleMath::Vector3 GetPosition() const = 0;
	// 現在の位置を設定する
	virtual void SetPosition(const DirectX::SimpleMath::Vector3& curretPosition) = 0;
	// 現在の回転角を取得する
	virtual DirectX::SimpleMath::Quaternion GetAngle() const = 0;
	// 現在の回転角を設定する
	virtual void SetAngle(const DirectX::SimpleMath::Quaternion& angle) = 0;
	// 質量を取得する
	virtual float GetMass() const = 0;
	// 質量を設定する
	virtual void SetMass(const float& mass) = 0;
	// モデルを取得する
	virtual DirectX::Model* GetModel() = 0;
	// CSVマップを取得する
	virtual CSVMap* GetCSVMap() = 0;
	// CSVアイテムを取得する 
	virtual CSVItem* GetCSVItem() = 0;
	// 次に現れるタイルのクラスを取得する
	virtual NextTiles* GetNextTiles() = 0;

public:
	// 仮想デストラクタ
	virtual ~IComponent() = default;
	// 初期化する
	virtual void Initialize(CommonResources* commonResources) = 0;
	// 更新する
	virtual void Update(float elapsedTime, const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Quaternion& angle) = 0;
	// 描画する
	virtual void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj) = 0;
	// 後処理を行う
	virtual void Finalize() = 0;
};
