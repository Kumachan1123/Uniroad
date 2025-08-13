/*
*	@file MiniCharacterSelectStage.cpp
*	@brief ミニキャラクターの処理を定義するクラス
*	@details ミニキャラクターの位置、角度、速度などを管理し、タイルのイベントを処理する。
*/
#include <pch.h>
#include "MiniCharacter.h"
// ノードカウンター
int MiniCharacter::s_nodeCount = 0;
// 部品カウンター
int MiniCharacter::s_partsNumber = 0;
// 重力を定義
const float MiniCharacter::GRAVITY = -9.8f;
/*
*	@brief コンストラクタ
*	@details ミニキャラクターの初期位置と角度を設定し、必要な変数を初期化する。
*	@param parent 親コンポーネント
*	@param initialPosition 初期位置
*	@param initialAngle 初期角度（ラジアン）
*	@return なし
*/
MiniCharacter::MiniCharacter(IComponent* parent, const DirectX::SimpleMath::Vector3& initialPosition, const float& initialAngle)
	: m_parent(parent)// 親コンポーネント
	, m_nodeNumber(MiniCharacter::GetNodeCountAfterCountUp())// ノード番号
	, m_partNumber(MiniCharacter::GetPartsNumber())// 部品番号
	, m_partID(MiniCharacter::MINICHARACTER)// 部品ID
	, m_pCommonResources(nullptr) // 共通リソース
	, m_enteredTilePtr(nullptr)// 入ったタイルのポインタ
	, m_hasEnteredTile(false)// 入った直後フラグ
	, m_isMoving(true)// 移動フラグ
	, m_fallTimer(0.0f)// 落下タイマー
	, m_gameOverSwitchTime(0.0f)// ゲームオーバーのスイッチ時間
	, m_gameClearSwitchTime(0.0f)// ゲームクリアのスイッチ時間
	, m_speed(1.0f)// 移動速度
	, m_fallTimerActive(false)// 落下タイマーが有効かどうか
	, m_hasFallen(false)// 一度だけ落下処理を実行させるためのフラグ
	, m_initialPosition(initialPosition)// 初期位置
	, m_initialAngle(DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3::Up, initialAngle))// 初期角度
	, m_currentPosition{}// 現在の位置
	, m_currentVelocity{}// 現在の速度
	, m_currentAngle{}// 現在の回転角
	, m_prevTileName("StartBlock")// 前フレームで一番近かったタイルの名前
	, m_rotationMiniCharacterAngle{}// プレイヤー回転角
	, m_mass{}// 質量
	, m_miniCharacterVelocity{}// プレイヤー速度
{
}
/*
*	@brief デストラクタ
*	@details ミニキャラクターの後処理を行う。
*	@param なし
*	@return なし
*/
MiniCharacter::~MiniCharacter()
{
	// 終了処理
	Finalize();
}
/*
*	@brief ミニキャラクターの初期化を行う
*	@details ミニキャラクターの初期化を行い、必要なリソースを設定する。
*	@param resources 共通リソース
*	@return なし
*/
void MiniCharacter::Initialize(CommonResources* resources)
{
	// SimpleMathの名前空間を使うためにusing宣言を追加
	using namespace DirectX::SimpleMath;
	// 共通リソースが存在することを確認する
	assert(resources);
	// 共通リソースを設定する
	m_pCommonResources = resources;
	// 初期位置を設定する
	m_initialPosition = GetParent()->GetCSVMap()->GetStart().pos;
	// 現在位置に反映
	m_currentPosition = m_initialPosition;
	// スタート地点の前後左右のタイルを調べてプレイヤーの速度を更新する
	UpdateSpeedByStartTile();
	// ヒツジパーツをアタッチ
	Attach(std::make_unique<Sheep>(this, Vector3(0.0f, 3.5f, 0.0f), 0.0f));
}
/*
*	@brief プレイヤーの位置と角度を更新する
*	@details プレイヤーの位置と角度を更新する。
*	@param elapsedTime 経過時間
*	@param currentPosition 現在の位置
*	@param currentAngle 現在の角度
*	@return なし
*/
void MiniCharacter::Update(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	// SimpleMathの名前空間を使うためにusing宣言を追加
	using namespace DirectX::SimpleMath;
	// タイルイベントを更新する
	UpdateTileEvents();
	// 落下タイマー処理
	UpdateFallTimer(elapsedTime);
	// 重力を加味した座標移動を行う
	Moving(elapsedTime, currentPosition);
	// 揺れ演出
	Shake();
	// 回転の補間
	InterpolateRotation(elapsedTime, currentAngle);
	// 次に生成されるタイルを決めるために速度ベクトルを設定する
	if (m_parent->GetNextTiles() != nullptr && m_currentVelocity != Vector3::Zero)
		m_parent->GetNextTiles()->SetMiniCharacterVelocity(m_currentVelocity);
	// 砲塔部品を更新する　
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)
		MiniCharacterPart->Update(elapsedTime, m_currentPosition, m_currentAngle);
	// ゲームオーバー、ゲームクリア分岐処理
	HandleGameOverAndClear(elapsedTime);


}
/*
*	@brief プレイヤーの部品を追加する
*	@details プレイヤーの部品を追加する。
*	@param MiniCharacterPart 追加する部品
*	@return なし
*/
void MiniCharacter::Attach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品番号を取得
	MiniCharacterPart->Initialize(m_pCommonResources);
	// 部品番号を設定
	m_pMiniCharacterParts.push_back(std::move(MiniCharacterPart));
}

/*
*	@brief プレイヤーの部品を削除する
*	@details プレイヤーの部品を削除する(今は何もしない)
*	@param MiniCharacterPart 削除する部品
*	@return なし
*/
void MiniCharacter::Detach(std::unique_ptr<IComponent> MiniCharacterPart)
{
	// 部品を削除する処理は今は何もしない
}
/*
*	@brief プレイヤーを描画する
*	@details プレイヤーを描画する。
*	@param view ビュー行列
*	@param proj プロジェクション行列
*	@return なし
*/
void MiniCharacter::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj)
{
	// 砲塔部品を描画する
	for (auto& MiniCharacterPart : m_pMiniCharacterParts)MiniCharacterPart->Render(view, proj);
#ifdef _DEBUG
	// ---デバッグ表示---
	const auto debugString = m_pCommonResources->GetDebugString();
	// 座標表示
	debugString->AddString("MiniCharacter Position: (%f, %f, %f)",
		m_currentPosition.x, m_currentPosition.y, m_currentPosition.z);
	// 速度表示
	debugString->AddString("MiniCharacter Velocity: (%f, %f, %f)",
		m_currentVelocity.x, m_currentVelocity.y, m_currentVelocity.z);
	// 最も近いタイルの名前を表示
	debugString->AddString("MiniCharacter Tile: %s",
		GetParent()->GetCSVMap()->GetTileData(m_currentPosition).tileInfo.modelName.c_str());
	// 座標表示
	debugString->AddString("CurrentTile Position: (%f, %f, %f)",
		m_currentTilePosition.x, m_currentTilePosition.y, m_currentTilePosition.z);
	// 座標表示
	debugString->AddString("PrevTile Position: (%f, %f, %f)",
		m_prevTilePosition.x, m_prevTilePosition.y, m_prevTilePosition.z);
#endif
}
/*
*	@brief プレイヤーの後処理を行う
*	@details プレイヤーの後処理を行う(今は何もしない)。
*	@param なし
*	@return なし
*/
void MiniCharacter::Finalize()
{

}
/*
*	@brief タイルイベントを更新する
*	@details タイルの中心への移動やタイルのイベントを更新する
*	@param なし
*	@return なし
*/
void MiniCharacter::UpdateTileEvents()
{
	// 現在いるタイルの名前を取得
	std::string currentTileName = GetParent()->GetCSVMap()->GetTileData(m_currentPosition).tileInfo.modelName;
	// 現在いるタイルの位置を取得
	m_currentTilePosition = GetParent()->GetCSVMap()->GetTileData(m_currentPosition).pos;

	// 現在の位置がタイルの中心にいるかどうかを判定
	bool isAtTileCenter = IsAtTileCenter(m_currentPosition, GetParent()->GetCSVMap()->GetTileData(m_currentPosition).pos);


	// 今いるタイルと前フレームでいたタイルが異なる場合、タイルのイベントを処理する
	if (currentTileName != m_prevTileName || m_currentTilePosition != m_prevTilePosition)
	{
		// 前のタイルのイベントを処理
		const auto& prevTile = GetParent()->GetCSVMap()->GetTileData(m_prevPosition);
		// 前のタイルが存在する場合、Exitイベントを呼び出す
		if (prevTile.tileBasePtr) prevTile.tileBasePtr->OnExit(this);
		// 現在のタイルのイベントを処理
		const auto& currentTile = GetParent()->GetCSVMap()->GetTileData(m_currentPosition);
		// 現在のタイルが存在する場合、Enterイベントを呼び出す
		if (currentTile.tileBasePtr ||// 現在のタイルが存在する場合
			(m_prevTileName == "" && currentTileName != ""))// 前のタイルが空白で、現在のタイルが存在する場合
		{
			// Enterイベントを呼び出す
			currentTile.tileBasePtr->OnEnter(this);
			// 現在のタイル名を更新
			m_prevTileName = currentTileName;
			// 前の位置を更新
			m_prevPosition = m_currentPosition;
			// 今いるタイルの位置を保存
			m_prevTilePosition = m_currentTilePosition;
		}
	}
	// 現在の位置がタイルの中心にいるかどうかを確認
	if (isAtTileCenter)
	{
		// タイルの中心にいる場合、移動フラグを更新
		const auto& currentTile = GetParent()->GetCSVMap()->GetTileData(m_currentPosition);
		// 現在のタイルが存在する場合、CenterReachedイベントを呼び出す
		if (currentTile.tileBasePtr)
			currentTile.tileBasePtr->OnCenterReached(this);
		// 行と列を取得
		int row = GetParent()->GetCSVMap()->GetRowFromPosition(m_currentPosition);
		int col = GetParent()->GetCSVMap()->GetColFromPosition(m_currentPosition);
		// CSVアイテムが存在する場合、アイテムを取得する
		const auto& csvItem = GetParent()->GetCSVItem();
		if (!csvItem)return;
		// そのタイルにアイテムがあるなら獲得する
		const auto& item = GetParent()->GetCSVItem()->GetItemData(row, col);
		// アイテムが存在する場合
		if (item.itemInfo.modelName != "" && item.itemBasePtr != nullptr)
		{
			// アイテムを獲得する
			item.itemBasePtr->OnGet(this);
			// アイテムを消す
			//GetParent()->GetCSVItem()->RemoveItem(row, col);
		}
	}
	// 現在のタイル名が空白である場合
	else if (currentTileName == "")
	{
		// 空白タイルにいる場合、移動フラグを折る
		m_isMoving = false;
		// 落下タイマーを開始
		if (!m_fallTimerActive)
		{
			// 落下タイマーを有効にする
			m_fallTimerActive = true;
			// 落下タイマーをリセット
			m_fallTimer = 0.0f;
		}
	}
	// 今いるタイルから進めるタイルを決めるために名前を保存
	if (m_parent->GetNextTiles() != nullptr && currentTileName != "")
		m_parent->GetNextTiles()->SetMiniCharacterTileName(currentTileName);
}
/*
*	@brief 落下タイマーを更新する
*	@details 落下タイマー更新・落下フラグ管理・復帰管理処理
*	@param elapsedTime 経過時間
*	@return なし
*/
void MiniCharacter::UpdateFallTimer(float elapsedTime)
{
	using namespace DirectX::SimpleMath;
	// 落下タイマーが有効で、まだ落下していない場合
	if (m_fallTimerActive && !m_hasFallen)
	{
		// 落下タイマーを更新
		m_fallTimer += elapsedTime;
		// 落下タイマーが3秒を超えたら、落下処理を実行
		if (m_fallTimer >= 3.0f)
		{
			// 速度を0にする
			m_currentVelocity = Vector3::Zero;
			// 移動フラグを折る
			m_isMoving = false;
			// 落下フラグを立てる
			m_hasFallen = true;
		}
	}
	const auto& currentTileName = GetParent()->GetCSVMap()->GetTileData(m_currentPosition).tileInfo.modelName;
	// 落下カウンター中に空白タイルから復帰したら、リセット
	if (m_fallTimerActive && !m_hasFallen && (currentTileName != "" && currentTileName != "Block"))
	{
		// 落下タイマーを無効にする
		m_fallTimerActive = false;
		// 落下タイマーをリセット
		m_fallTimer = 0.0f;
		// 移動フラグを立てる
		m_isMoving = true;
		// 落下フラグを折る
		m_hasFallen = false;
	}
}
/*
*	@brief プレイヤーに重力を適用する
*	@details 重力・速度・座標更新処理
*	@param elapsedTime 経過時間
*	@return なし
*/
void MiniCharacter::Moving(float elapsedTime, const DirectX::SimpleMath::Vector3& currentPosition)
{
	// 落下中の場合
	if (m_hasFallen)
	{
		// 落下中は重力を適用する
		m_currentVelocity.y += GRAVITY * elapsedTime;
		// 速度に適用する
		m_miniCharacterVelocity += m_currentVelocity * elapsedTime;
		// ゲームオーバーのスイッチ時間を更新
		m_gameOverSwitchTime += elapsedTime;
	}
	// 落下していない場合
	else if (m_isMoving)
	{
		// 通常通りの移動処理
		m_miniCharacterVelocity += m_currentVelocity * elapsedTime / 3 * m_speed;

	}
	// 座標に速度を適用する
	m_currentPosition = currentPosition + m_initialPosition + m_miniCharacterVelocity;
}
/*
*	@brief プレイヤーを揺らす
*	@details 揺れクォータニオンを計算する
*	@return なし
*/
void MiniCharacter::Shake()
{
	using namespace DirectX::SimpleMath;
	// 揺れクォータニオン（デフォルトは回転なし）
	m_shakeQuaternion = Quaternion::Identity;
	// 落下タイマーが有効で、まだ落下していない場合
	if (m_fallTimerActive && !m_hasFallen)
	{
		// 揺れの強さ
		float shakeAmount = 0.075f;
		// 揺れの速さ
		float shakeSpeed = 7.0f;
		// 落下タイマーの時間を取得
		float time = m_fallTimer;
		// 揺れの進行度を計算（0.0fから1.0fの範囲）
		float progress = std::min(time / 3.0f, 1.0f);
		// 揺れの振幅を計算（進行度に応じて変化）
		float amp = shakeAmount * (0.5f + 1.0f * progress);
		// 揺れの角度を計算
		float xSwing = sinf(time * shakeSpeed) * amp * (0.8f + 0.4f * sinf(time * 2.0f));
		// z軸の揺れは少し小さくする
		float zSwing = cosf(time * shakeSpeed * 0.7f) * amp * (0.7f + 0.6f * cosf(time * 3.1f));
		// 揺れクォータニオンを作成
		m_shakeQuaternion = Quaternion::CreateFromYawPitchRoll(0.0f, xSwing, zSwing);
	}
}
/*
*	@brief プレイヤーの回転を補間する
*	@details プレイヤーの回転を補間して、滑らかな回転を実現する。
*	@param elapsedTime 経過時間
*	@param currentAngle 現在の回転角
*	@return なし
*
*/
void MiniCharacter::InterpolateRotation(float elapsedTime, const DirectX::SimpleMath::Quaternion& currentAngle)
{
	using namespace DirectX::SimpleMath;
	// 目標回転を計算（速度ベクトルから）
	Quaternion targetQuat;
	// 現在の速度がゼロでない場合、回転を計算
	if (m_currentVelocity.LengthSquared() > 0.0f)
	{
		// 現在の速度ベクトルからヨー角を計算
		float yaw = atan2f(m_currentVelocity.x, m_currentVelocity.z);
		// ヨー角からクォータニオンを作成
		targetQuat = Quaternion::CreateFromYawPitchRoll(yaw, 0.0f, 0.0f);
	}
	else
	{
		// 速度がゼロの場合は、回転なし
		targetQuat = Quaternion::Identity;
	}
	// 現在の回転角を更新する
	float rotateSpeed = 2.0f;
	// 滑らかに回転させるために、現在の回転角と目標回転角を補間
	m_rotationMiniCharacterAngle = Quaternion::Slerp(m_rotationMiniCharacterAngle, targetQuat, elapsedTime * rotateSpeed);
	// 揺れを加味した回転を適用
	m_currentAngle = currentAngle * m_initialAngle * m_rotationMiniCharacterAngle * m_shakeQuaternion;
}

/*
*	@brief プレイヤーの開始タイルによって速度を更新する
*	@details スタート地点の前後左右のタイルを調べて、プレイヤーの速度を更新する。
*	@param なし
*	@return なし
*/
void MiniCharacter::UpdateSpeedByStartTile()
{
	// SimpleMathの名前空間を使うためにusing宣言を追加
	using namespace DirectX::SimpleMath;
	// 行初期値
	int startRow = -1;
	// 列初期値
	int startCol = -1;
	// スタート位置を探す
	for (int row = 0; row < GetParent()->GetCSVMap()->GetMaxRow(); ++row)
	{
		for (int col = 0; col < GetParent()->GetCSVMap()->GetMaxCol(); ++col)
		{
			// タイルデータを取得
			const auto& tile = GetParent()->GetCSVMap()->GetTileData(row, col);
			// タイルのモデル名が"Start"であるか確認
			if (tile.tileInfo.modelName == "StartBlock")
			{
				// スタート位置を設定
				startRow = row;
				// スタート位置の列を設定
				startCol = col;
				// ループを抜ける
				break;
			}
		}
		// スタート位置が見つかったらループを抜ける
		if (startRow != -1) break;
	}
	// 上下左右の方向情報
	struct Dir { int dRow, dCol; };
	// 方向ごとのベクトルを定義
	const Dir dirs[4] =
	{
		{ -1,  0 }, // 上
		{  1,  0 }, // 下
		{  0, -1 }, // 左
		{  0,  1 }, // 右
	};
	// 対応する速度ベクトルを定義
	const Vector3 velocities[4] =
	{
		{0, 0, -1}, // 上（前進）
		{0, 0,  1}, // 下（後進）
		{-1, 0, 0}, // 左
		{ 1, 0, 0}, // 右
	};
	// スタート位置が見つからなかった場合はデフォルトの速度を設定
	for (int i = 0; i < 4; i++)
	{
		// 隣接するタイルの行と列を計算
		int nRow = startRow + dirs[i].dRow;
		int nCol = startCol + dirs[i].dCol;
		// 隣接するタイルがマップの範囲内であるか確認
		if (nRow >= 0 && nRow < GetParent()->GetCSVMap()->GetMaxCol() &&// 行の範囲チェック
			nCol >= 0 && nCol < GetParent()->GetCSVMap()->GetMaxRow())	// 列の範囲チェック
		{
			// 隣接するタイルのデータを取得
			const auto& neighbor = GetParent()->GetCSVMap()->GetTileData(nRow, nCol);
			// タイルのモデル名を取得
			const std::string& model = neighbor.tileInfo.modelName;
			// モデル名が"DefaultStraightVertical"または"DefaultStraightHorizontal"であるか確認
			if ((i <= 1 && model == "DefaultStraightVerticalBlock") || // 上下
				(i >= 2 && model == "DefaultStraightHorizontalBlock")) // 左右
			{
				// 対応する速度ベクトルを設定
				m_currentVelocity = velocities[i];
				// ループを抜ける
				break;
			}
		}
		else
		{
			m_currentVelocity = Vector3::Left; // デフォルトは後ろ向き
		}
	}
}
/*
*	@brief タイルの中心にいるかどうかを判定する
*	@details プレイヤーの位置がタイルの中心に近いかどうかを判定する。
*	@param charPos プレイヤーの位置
*	@param tileCenter タイルの中心位置
*	@param epsilon 判定の許容誤差
*	@return タイルの中心にいる場合はtrue、そうでない場合はfalse
*/
bool MiniCharacter::IsAtTileCenter(const DirectX::SimpleMath::Vector3& charPos, const DirectX::SimpleMath::Vector3& tileCenter, float epsilon) const
{
	// タイルの中心とプレイヤーの位置の距離を計算
	float distance = (charPos - tileCenter).Length();
	// 距離が許容誤差以下であれば、タイルの中心にいると判断
	return distance < epsilon;
}
/*
*	@brief ゲームオーバーとクリアの処理を行う
*	@details ゲームオーバーやクリアの処理を行う。
*	@param elapsedTime 経過時間
*	@return なし
*/
void MiniCharacter::HandleGameOverAndClear(float elapsedTime)
{
	// 未使用警告非表示
	UNREFERENCED_PARAMETER(elapsedTime);
	// SimpleMathの名前空間を使うためにusing宣言を追加
	using namespace DirectX::SimpleMath;
	// ゴールがアンロックされているかどうかを取得
	bool goalUnlocked = GetParent()->GetCSVItem()->IsGoalUnlocked();
	// 現在のタイルを取得
	const auto& tile = GetParent()->GetCSVMap()->GetTileData(m_currentPosition);
	// 現在のタイルの名前を取得
	std::string goalTileName = tile.tileInfo.modelName;
	// 現在の位置がタイルの中心にいるかどうかを判定
	bool isCenter = IsAtTileCenter(m_currentPosition, tile.pos);
	// アンロックされたゴールの真上にいるならクリア
	if (goalTileName == "GoalBlock" && goalUnlocked == true && isCenter)	dynamic_cast<MiniCharacterBase*>(m_parent)->SetGameClear(true);
	// 途中で止まるか落ちている場合、ゲームオーバー
	else if (m_currentVelocity.y < Vector3::Zero.y || m_currentVelocity == Vector3::Zero)dynamic_cast<MiniCharacterBase*>(m_parent)->SetGameOver(true);
}

