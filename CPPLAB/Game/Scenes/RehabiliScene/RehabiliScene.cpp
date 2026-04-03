/*
*	@file RehabiliScene.cpp
*	@brief ゲームづくりのリハビリ用シーンクラス
*/

#include "pch.h"
#include "RehabiliScene.h"
/*
*	@brief コンストラクタ
*	@details ゲームづくりのリハビリ用シーンクラスのコンストラクタ
*	@param sceneID シーンID
*	@return なし
*/
RehabiliScene::RehabiliScene(IScene::SceneID sceneID)
	: m_pCommonResources(nullptr) // 共通リソースへのポインタ
	, m_pTPCamera(nullptr) // カメラへのポインタ
	, m_view() // ビュー行列
	, m_projection() // 射影行列
	, m_isChangeScene(false) // シーン変更フラグ
	, m_isFalling(false)// 落ちているか
	, m_nextSceneID(sceneID) // 次のシーンID
	, m_time(0.0f) // 時間
	, m_fallCenterCol(2)
	, m_fallCenterRow(1)
	, m_subOffsetCol(0)
	, m_subOffsetRow(-1)
	, m_fallTimer(0.0f)
	, m_prevLeftKey(false)
	, m_prevRightKey(false)
	, m_prevAKey(false)
	, m_prevDKey(false)
{}
/*
*	@brief デストラクタ
*	@details ゲームづくりのリハビリ用シーンクラスのデストラクタ
*	@param なし
*	@return なし
*/
RehabiliScene::~RehabiliScene()
{}
/*
*	@brief 初期化
*	@details ゲームづくりのリハビリ用シーンクラスの初期化を行う
*	@param resources 共通リソースへのポインタ
*	@return なし
*/
void RehabiliScene::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースを保存する。
	// 以降のテクスチャ取得やデバイス参照はこのポインタ経由で統一する。
	m_pCommonResources = resources;
	auto deviceResources = m_pCommonResources->GetDeviceResources();

	// カメラを生成して、ビュー・射影の準備を完了させる。
	CreateCamera();

	// シーン内状態を初期化。
	// m_time は次の落下ペア生成タイミング管理、m_fallTimer は自然落下管理に使う。
	m_isFalling = false;
	m_fallTimer = 0.0f;
	m_time = 0.0f;

	// 盤面背景（グリッド）を6x12で生成。
	// 見た目のマス位置と内部座標を一致させるため GridToPosition を必ず通す。
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pPuyoGrid[col][row] = std::make_unique<PuyoGrid>();
			m_pPuyoGrid[col][row]->SetPosition(GridToPosition(col, row));
			m_pPuyoGrid[col][row]->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
		}
	}

	// 固定盤面は最初すべて空マス（None）で埋める。
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			RecreateFixedPuyo(col, row, Puyo::PuyoColor::None);
		}
	}

	// 落下中ぷよの作業領域をクリア。
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			m_pFallingPuyo[col][row].reset();
		}
	}
}
/*
*	@brief 更新
*	@details ゲームづくりのリハビリ用シーンクラスの更新を行う
*	@param elapsedTime 経過時間
*	@return なし
*/
void RehabiliScene::Update(float elapsedTime)
{

	// 次の落下ペア生成用タイマーを進める。
	m_time += elapsedTime;
	if (m_time >= 1.0f && !m_isFalling)
	{
		// 落下中ぷよが存在しない時だけ新規生成する。
		GeneratePuyo();
		m_time = 0.0f;
	}

	// カメラ更新（ゲームロジックとは独立）。
	m_pTPCamera->SetTime(m_time);
	m_pTPCamera->Update();
	m_debugCamera->Update(m_pCommonResources->GetInputManager());
	m_view = m_pTPCamera->GetViewMatrix();

	// シーン遷移入力。
	auto keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();
	if (keyState.Enter)m_isChangeScene = true;

	// 固定盤面のぷよ更新。
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pFixedPuyo[col][row]->Update(elapsedTime);
		}
	}

	// 落下中ぷよがある場合のみ、入力と自然落下を進行させる。
	if (m_isFalling)
	{
		UpdateFallingPuyo(elapsedTime);
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				if (m_pFallingPuyo[col][row])
				{
					m_pFallingPuyo[col][row]->Update(elapsedTime);
				}
			}
		}
	}
}
/*
*	@brief 描画
*	@details ゲームづくりのリハビリ用シーンクラスの描画を行う
*	@param なし
*	@return なし
*/
void RehabiliScene::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	const auto states = m_pCommonResources->GetCommonStates();
	auto context = deviceResources->GetD3DDeviceContext();

	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pPuyoGrid[col][row]->Render();
		}
	}

	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pFixedPuyo[col][row]->Render();
		}
	}

	if (m_isFalling)
	{
		for (int row = 0; row < 3; row++)
		{
			for (int col = 0; col < 3; col++)
			{
				if (m_pFallingPuyo[col][row])
				{
					m_pFallingPuyo[col][row]->Render();
				}
			}
		}
	}
}
/*
*	@brief 終了
*	@details ゲームづくりのリハビリ用シーンクラスの終了処理を行う
*	@param なし
*	@return なし
*/
void RehabiliScene::Finalize()
{

}
/*
*	@brief シーン変更
*	@details シーン変更の有無を取得する
*	@param なし
*	@return シーンID
*/
IScene::SceneID RehabiliScene::GetNextSceneID() const
{
	// シーン変更がないならすぐ戻る
	if (!m_isChangeScene)return IScene::SceneID::NONE;
	// ステージセレクトへ
	return IScene::SceneID::TEST;
}
/*
* 	@brief カメラに関する設定をする
*	@details カメラの作成と設定を行う
*	@param なし
*	@return なし
*/
void RehabiliScene::CreateCamera()
{
	// DirectXの名前空間の使用
	using namespace DirectX;
	// SimpleMathの名前空間の使用
	using namespace DirectX::SimpleMath;
	// 出力サイズを取得する
	RECT rect = m_pCommonResources->GetDeviceResources()->GetOutputSize();
	// 固定カメラを作成する
	m_pTPCamera = std::make_unique<TPCamera>();
	// 固定カメラを初期化する
	m_pTPCamera->SetCommonResources(m_pCommonResources);
	m_pTPCamera->Initialize((int)(rect.right), rect.bottom);
	// デバッグカメラを作成する
	m_debugCamera = std::make_unique<mylib::DebugCamera>();
	// デバッグカメラを初期化する
	m_debugCamera->Initialize(rect.right, rect.bottom);
	// 射影行列を作成する
	m_projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(FOV),// 視野角
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),// アスペクト比
		0.1f, 10000.0f);// ニアクリップ距離、ファークリップ距離
	// カメラに射影行列をセット
	m_pTPCamera->SetProjectionMatrix(m_projection);
}
/*
*	@brief SDKメッシュを作る
*	@details SDKメッシュの作成を行う
*	@param name SDKメッシュの名前
*	@return なし
*/
void RehabiliScene::CreateSDKMesh(std::wstring name)
{
	// DirectXの名前空間の使用
	using namespace DirectX;
	// deviceを取得する
	ID3D11Device* device = m_pCommonResources->GetDeviceResources()->GetD3DDevice();

	// ファイルパス
	std::wstring filePath = L"Resources/SDKMeshes/" + name + L"/" + name + L".sdkmesh";
	// フォルダパス
	std::wstring folderPath = L"Resources/SDKMeshes/" + name;



}
/*
*	@brief ぷよを作る
*	@details ぷよの作成を行う
*	@param なし
*	@return なし
*/
void RehabiliScene::GeneratePuyo()
{
	using namespace DirectX;
	auto deviceResources = m_pCommonResources->GetDeviceResources();

	// 新規ペアの初期姿勢を設定。
	// 軸: (2,1) / 子: 軸の上(0,-1)
	m_fallCenterCol = 2;
	m_fallCenterRow = 1;
	m_subOffsetCol = 0;
	m_subOffsetRow = -1;
	m_fallTimer = 0.0f;

	// 出現位置が埋まっている場合は生成不可。
	const int subCol = m_fallCenterCol + m_subOffsetCol;
	const int subRow = m_fallCenterRow + m_subOffsetRow;
	if (!IsCellEmpty(m_fallCenterCol, m_fallCenterRow) || !IsCellEmpty(subCol, subRow))
	{
		return;
	}

	// 作業領域をクリアしてから新しい2連ぷよを配置。
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			m_pFallingPuyo[col][row].reset();
		}
	}

	// 色は個別にランダム決定。
	int centerColor = KumachiLib::GenerateRandomMultiplier(0, 4);
	int subColor = KumachiLib::GenerateRandomMultiplier(0, 4);
	m_pFallingPuyo[1][1] = std::make_unique<Puyo>(static_cast<Puyo::PuyoColor>(centerColor));
	m_pFallingPuyo[1 + m_subOffsetCol][1 + m_subOffsetRow] = std::make_unique<Puyo>(static_cast<Puyo::PuyoColor>(subColor));

	// 実体化したぷよを描画可能な状態へ初期化。
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (m_pFallingPuyo[col][row])
			{
				m_pFallingPuyo[col][row]->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
			}
		}
	}

	// 盤面座標から描画座標を同期して落下開始。
	SyncFallingPuyoPosition();
	m_isFalling = true;
}

void RehabiliScene::UpdateFallingPuyo(float elapsedTime)
{
	auto keyState = m_pCommonResources->GetInputManager()->GetKeyboardState();

	// 入力エッジ判定用の現在状態。
	const bool leftDown = keyState.IsKeyDown(DirectX::Keyboard::Keys::Left);
	const bool rightDown = keyState.IsKeyDown(DirectX::Keyboard::Keys::Right);
	const bool aDown = keyState.IsKeyDown(DirectX::Keyboard::Keys::A);
	const bool dDown = keyState.IsKeyDown(DirectX::Keyboard::Keys::D);

	// 横移動は押した瞬間のみ反応。
	if (leftDown && !m_prevLeftKey)
	{
		TryMoveFallingPuyo(-1, 0);
	}
	if (rightDown && !m_prevRightKey)
	{
		TryMoveFallingPuyo(1, 0);
	}

	// 回転も押した瞬間のみ反応。
	if (aDown && !m_prevAKey)
	{
		TryRotateFallingPuyo(false);
	}
	if (dDown && !m_prevDKey)
	{
		TryRotateFallingPuyo(true);
	}

	// 前フレーム状態を更新。
	m_prevLeftKey = leftDown;
	m_prevRightKey = rightDown;
	m_prevAKey = aDown;
	m_prevDKey = dDown;

	// 自然落下タイマー。
	m_fallTimer += elapsedTime;
	if (m_fallTimer >= FALL_INTERVAL)
	{
		m_fallTimer = 0.0f;
		// 1マス下に移動できなければ着地として固定する。
		if (!TryMoveFallingPuyo(0, 1))
		{
			LockFallingPuyo();
		}
	}
}

bool RehabiliScene::TryMoveFallingPuyo(int dx, int dy)
{
	const int centerCol = m_fallCenterCol + dx;
	const int centerRow = m_fallCenterRow + dy;
	const int subCol = m_fallCenterCol + m_subOffsetCol + dx;
	const int subRow = m_fallCenterRow + m_subOffsetRow + dy;

	if (!IsInsideBoard(centerCol, centerRow) || !IsInsideBoard(subCol, subRow))
	{
		return false;
	}
	if (!IsCellEmpty(centerCol, centerRow) || !IsCellEmpty(subCol, subRow))
	{
		return false;
	}

	m_fallCenterCol = centerCol;
	m_fallCenterRow = centerRow;
	SyncFallingPuyoPosition();
	return true;
}

void RehabiliScene::TryRotateFallingPuyo(bool isRightRotation)
{
	int nextOffsetCol = 0;
	int nextOffsetRow = 0;

	if (isRightRotation)
	{
		// 右回転: (x, y) -> (-y, x)
		nextOffsetCol = -m_subOffsetRow;
		nextOffsetRow = m_subOffsetCol;
	}
	else
	{
		// 左回転: (x, y) -> (y, -x)
		nextOffsetCol = m_subOffsetRow;
		nextOffsetRow = -m_subOffsetCol;
	}

	const int nextSubCol = m_fallCenterCol + nextOffsetCol;
	const int nextSubRow = m_fallCenterRow + nextOffsetRow;

	if (!IsInsideBoard(nextSubCol, nextSubRow) || !IsCellEmpty(nextSubCol, nextSubRow))
	{
		return;
	}

	const int oldIndexCol = 1 + m_subOffsetCol;
	const int oldIndexRow = 1 + m_subOffsetRow;
	const int newIndexCol = 1 + nextOffsetCol;
	const int newIndexRow = 1 + nextOffsetRow;

	// 子ぷよを新しい相対位置のスロットへ移す
	if (m_pFallingPuyo[oldIndexCol][oldIndexRow])
	{
		m_pFallingPuyo[newIndexCol][newIndexRow] = std::move(m_pFallingPuyo[oldIndexCol][oldIndexRow]);
	}

	m_subOffsetCol = nextOffsetCol;
	m_subOffsetRow = nextOffsetRow;
	SyncFallingPuyoPosition();
}

void RehabiliScene::LockFallingPuyo()
{
	// 軸ぷよが存在しない場合は安全に終了。
	if (!m_pFallingPuyo[1][1])
	{
		m_isFalling = false;
		return;
	}

	// 落下中2個の盤面座標と色を固定盤面へ転写する。
	struct FallingCell
	{
		int col;
		int row;
		Puyo::PuyoColor color;
	};

	FallingCell cells[2] =
	{
		{ m_fallCenterCol, m_fallCenterRow, m_pFallingPuyo[1][1]->GetColor() },
		{ m_fallCenterCol + m_subOffsetCol, m_fallCenterRow + m_subOffsetRow, m_pFallingPuyo[1 + m_subOffsetCol][1 + m_subOffsetRow]->GetColor() }
	};

	for (const auto& cell : cells)
	{
		if (!IsInsideBoard(cell.col, cell.row))
		{
			continue;
		}
		RecreateFixedPuyo(cell.col, cell.row, cell.color);
	}

	// 落下作業領域をクリアして状態を停止。
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			m_pFallingPuyo[col][row].reset();
		}
	}

	m_isFalling = false;
	m_fallTimer = 0.0f;

	// 設置後の盤面解決（下詰め・消去・連鎖）を実行。
	ResolveBoard();
}

void RehabiliScene::ResolveBoard()
{
	while (true)
	{
		// 通常設置時でも必ず下詰めして、空中に残るぷよを解消する。
		ApplyGravityToBoard();

		// 4つ以上連結が無ければ盤面安定。
		if (!EraseConnectedPuyo())
		{
			break;
		}
		// 消去が発生した場合は、次ループで再び下詰め→再判定（連鎖）
	}
}

bool RehabiliScene::EraseConnectedPuyo()
{
	bool visited[6][12] = {};
	std::vector<std::pair<int, int>> eraseCells;

	// 一番左上から走査（上→下、左→右）
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			if (visited[col][row])
			{
				continue;
			}

			const auto color = m_pFixedPuyo[col][row]->GetColor();
			if (color == Puyo::PuyoColor::None)
			{
				visited[col][row] = true;
				continue;
			}

			// 同色連結を収集して、4個以上なら消去対象へ追加。
			std::vector<std::pair<int, int>> connected;
			CollectConnectedPuyo(col, row, color, visited, connected);
			if (connected.size() >= 4)
			{
				eraseCells.insert(eraseCells.end(), connected.begin(), connected.end());
			}
		}
	}

	if (eraseCells.empty())
	{
		return false;
	}

	// 消去対象を空マスへ置換。
	for (const auto& cell : eraseCells)
	{
		RecreateFixedPuyo(cell.first, cell.second, Puyo::PuyoColor::None);
	}
	return true;
}

void RehabiliScene::CollectConnectedPuyo(int col, int row, Puyo::PuyoColor color, bool visited[6][12], std::vector<std::pair<int, int>>& outCells) const
{
	// 盤面外は探索終了。
	if (!IsInsideBoard(col, row))
	{
		return;
	}
	// 訪問済みセルは再探索しない。
	if (visited[col][row])
	{
		return;
	}
	// 色が一致しないセルは連結対象外。
	if (m_pFixedPuyo[col][row]->GetColor() != color)
	{
		return;
	}

	// 同色セルとして記録し、上下左右へ再帰探索する。
	visited[col][row] = true;
	outCells.emplace_back(col, row);

	CollectConnectedPuyo(col - 1, row, color, visited, outCells);
	CollectConnectedPuyo(col + 1, row, color, visited, outCells);
	CollectConnectedPuyo(col, row - 1, color, visited, outCells);
	CollectConnectedPuyo(col, row + 1, color, visited, outCells);
}

bool RehabiliScene::ApplyGravityToBoard()
{
	bool changed = false;

	// 各列を独立に下詰めする。
	for (int col = 0; col < 6; col++)
	{
		int writeRow = 11;
		for (int row = 11; row >= 0; row--)
		{
			const auto color = m_pFixedPuyo[col][row]->GetColor();
			if (color == Puyo::PuyoColor::None)
			{
				continue;
			}

			// writeRowへ詰める。位置が変わる場合のみ再配置。
			if (row != writeRow)
			{
				RecreateFixedPuyo(col, writeRow, color);
				RecreateFixedPuyo(col, row, Puyo::PuyoColor::None);
				changed = true;
			}
			writeRow--;
		}

		// 念のため残り上部を空マスで埋める。
		for (int row = writeRow; row >= 0; row--)
		{
			if (m_pFixedPuyo[col][row]->GetColor() != Puyo::PuyoColor::None)
			{
				RecreateFixedPuyo(col, row, Puyo::PuyoColor::None);
				changed = true;
			}
		}
	}

	return changed;
}

void RehabiliScene::RecreateFixedPuyo(int col, int row, Puyo::PuyoColor color)
{
	auto deviceResources = m_pCommonResources->GetDeviceResources();
	// 指定色でPuyoを作り直し、盤面座標と描画座標を必ず同期する。
	m_pFixedPuyo[col][row] = std::make_unique<Puyo>(color);
	m_pFixedPuyo[col][row]->SetRowCol(row, col);
	m_pFixedPuyo[col][row]->SetPosition(GridToPosition(col, row));
	m_pFixedPuyo[col][row]->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
}

void RehabiliScene::SyncFallingPuyoPosition()
{
	// 軸ぷよが無い場合は同期不要。
	if (!m_pFallingPuyo[1][1])
	{
		return;
	}

	// 軸ぷよを中心座標へ配置。
	m_pFallingPuyo[1][1]->SetRowCol(m_fallCenterRow, m_fallCenterCol);
	m_pFallingPuyo[1][1]->SetPosition(GridToPosition(m_fallCenterCol, m_fallCenterRow));

	// 子ぷよを相対オフセット位置へ配置。
	if (m_pFallingPuyo[1 + m_subOffsetCol][1 + m_subOffsetRow])
	{
		const int subCol = m_fallCenterCol + m_subOffsetCol;
		const int subRow = m_fallCenterRow + m_subOffsetRow;
		m_pFallingPuyo[1 + m_subOffsetCol][1 + m_subOffsetRow]->SetRowCol(subRow, subCol);
		m_pFallingPuyo[1 + m_subOffsetCol][1 + m_subOffsetRow]->SetPosition(GridToPosition(subCol, subRow));
	}
}

bool RehabiliScene::IsInsideBoard(int col, int row) const
{
	return (col >= 0 && col < 6 && row >= 0 && row < 12);
}

bool RehabiliScene::IsCellEmpty(int col, int row) const
{
	if (!IsInsideBoard(col, row))
	{
		return false;
	}
	return m_pFixedPuyo[col][row]->GetColor() == Puyo::PuyoColor::None;
}

bool RehabiliScene::IsFallingCellAt(int col, int row) const
{
	if (!m_isFalling)
	{
		return false;
	}
	if (m_fallCenterCol == col && m_fallCenterRow == row)
	{
		return true;
	}
	return (m_fallCenterCol + m_subOffsetCol == col) && (m_fallCenterRow + m_subOffsetRow == row);
}

DirectX::SimpleMath::Vector2 RehabiliScene::GridToPosition(int col, int row) const
{
	using namespace DirectX::SimpleMath;
	return Vector2(0.4f + col * 0.05f, 0.045f + row * 0.0826f);
}
