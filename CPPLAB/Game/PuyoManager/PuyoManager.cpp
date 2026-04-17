#include "pch.h"
#include "PuyoManager.h"

PuyoManager::PuyoManager()
{}

PuyoManager::~PuyoManager()
{}

void PuyoManager::Initialize(CommonResources* resources)
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	// 共通リソースを保存する。
	// 以降のテクスチャ取得やデバイス参照はこのポインタ経由で統一する。
	m_pCommonResources = resources;
	auto deviceResources = m_pCommonResources->GetDeviceResources();
	// シーン内状態を初期化。
// m_time は次の落下ペア生成タイミング管理、m_fallTimer は自然落下管理に使う。
	m_isFalling = false;
	m_fallTimer = 0.0f;
	m_time = 0.0f;
	m_isResolving = false;
	m_resolveNeedsGravity = false;
	m_resolveNeedsFaceChange = false;
	m_resolveNeedsErase = false;
	m_resolveTimer = 0.0f;

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
	for (int i = 0; i < 2; i++)
	{
		m_pLandingPreviewPuyo[i].reset();
	}
}

void PuyoManager::Update(float elapsedTime)
{
	// 次の落下ペア生成用タイマーを進める。
	m_time += elapsedTime;
	if (m_time >= 1.0f && !m_isFalling && !m_isResolving)
	{
		// 落下中ぷよが存在しない時だけ新規生成する。
		GeneratePuyo();
		m_time = 0.0f;
	}

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

		for (int i = 0; i < 2; i++)
		{
			if (m_pLandingPreviewPuyo[i])
			{
				m_pLandingPreviewPuyo[i]->Update(elapsedTime);
			}
		}
	}
	else if (m_isResolving)
	{
		// 連鎖解決は1回のUpdateで完走させず、1秒ごとに段階実行する。
		ResolveBoard(elapsedTime);
	}
}

void PuyoManager::Render()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	const auto deviceResources = m_pCommonResources->GetDeviceResources();
	const auto states = m_pCommonResources->GetCommonStates();
	auto context = deviceResources->GetD3DDeviceContext();
	// グリッドを描画する
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pPuyoGrid[col][row]->Render();
		}
	}
	// 固定ぷよを描画する
	for (int row = 0; row < 12; row++)
	{
		for (int col = 0; col < 6; col++)
		{
			m_pFixedPuyo[col][row]->Render();
		}
	}
	// 着地予定位置プレビューを描画する
	if (m_isFalling)
	{
		for (int i = 0; i < 2; i++)
		{
			if (m_pLandingPreviewPuyo[i])
			{
				m_pLandingPreviewPuyo[i]->Render();
			}
		}
	}
	// 落下中ぷよを描画する
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

void PuyoManager::GeneratePuyo()
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
	for (int i = 0; i < 2; i++)
	{
		m_pLandingPreviewPuyo[i].reset();
	}

	// 色は個別にランダム決定。
	int centerColor = KumachiLib::GenerateRandomMultiplier(0, 4);
	int subColor = KumachiLib::GenerateRandomMultiplier(0, 4);
	m_pFallingPuyo[1][1] = std::make_unique<Puyo>(static_cast<Puyo::PuyoColor>(centerColor));
	m_pFallingPuyo[1 + m_subOffsetCol][1 + m_subOffsetRow] = std::make_unique<Puyo>(static_cast<Puyo::PuyoColor>(subColor));

	m_pLandingPreviewPuyo[0] = std::make_unique<Puyo>(static_cast<Puyo::PuyoColor>(centerColor));
	m_pLandingPreviewPuyo[1] = std::make_unique<Puyo>(static_cast<Puyo::PuyoColor>(subColor));

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
	for (int i = 0; i < 2; i++)
	{
		if (m_pLandingPreviewPuyo[i])
		{
			m_pLandingPreviewPuyo[i]->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
		}
	}

	// 盤面座標から描画座標を同期して落下開始。
	SyncFallingPuyoPosition();
	UpdateLandingPreview();
	m_isFalling = true;
}

void PuyoManager::UpdateFallingPuyo(float elapsedTime)
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

	UpdateLandingPreview();
}

bool PuyoManager::TryMoveFallingPuyo(int dx, int dy)
{
	// 移動先の軸ぷよと子ぷよの盤面座標を計算して、両方とも空いているかチェックする。
	const int centerCol = m_fallCenterCol + dx;// 移動先の軸ぷよの列
	const int centerRow = m_fallCenterRow + dy;// 移動先の軸ぷよの行
	const int subCol = m_fallCenterCol + m_subOffsetCol + dx;// 移動先の子ぷよの列
	const int subRow = m_fallCenterRow + m_subOffsetRow + dy;// 移動先の子ぷよの行
	// どちらかが盤面外なら移動不可。
	if (!IsInsideBoard(centerCol, centerRow) || !IsInsideBoard(subCol, subRow))
	{
		return false;
	}
	// どちらかが埋まっているなら移動不可。
	if (!IsCellEmpty(centerCol, centerRow) || !IsCellEmpty(subCol, subRow))
	{
		return false;
	}
	// 両方とも空いているなら移動可能。中心座標を更新して、描画位置も再同期する。
	m_fallCenterCol = centerCol;
	m_fallCenterRow = centerRow;
	SyncFallingPuyoPosition();
	return true;
}

void PuyoManager::TryRotateFallingPuyo(bool isRightRotation)
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

void PuyoManager::LockFallingPuyo()
{// 軸ぷよが存在しない場合は安全に終了。
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
	// 盤面内のセルだけを固定盤面へ転写する。盤面外は無視（通常は回転の結果が該当）。
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
	for (int i = 0; i < 2; i++)
	{
		m_pLandingPreviewPuyo[i].reset();
	}

	m_isFalling = false;
	m_fallTimer = 0.0f;

	// 設置後の盤面解決（落下→消去→再落下→再消去）を1秒間隔で開始する。
	m_isResolving = true;
	m_resolveNeedsGravity = true;
	m_resolveNeedsFaceChange = false;
	m_resolveNeedsErase = false;
	m_resolveTimer = 0.0f;
}

void PuyoManager::ChangePuyoFaceBeforeErase()
{
	bool visited[6][12] = {};

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

			std::vector<std::pair<int, int>> connected;
			CollectConnectedPuyo(col, row, color, visited, connected);
			if (connected.size() >= 4)
			{
				for (const auto& cell : connected)
				{
					// TODO: ここで cell.first, cell.second のぷよに「消去前の表情」フラグを立てる
				}
			}
		}
	}
}

void PuyoManager::ResolveBoard(float elapsedTime)
{
	if (!m_isResolving)
	{
		return;
	}

	m_resolveTimer += elapsedTime;
	if (m_resolveTimer < FALL_INTERVAL)
	{
		return;
	}
	m_resolveTimer = 0.0f;

	if (m_resolveNeedsGravity)
	{
		// このステップは重力落下。
		ApplyGravityToBoard();
		// 次ステップは表情変更。
		m_resolveNeedsGravity = false;
		m_resolveNeedsFaceChange = true;
		return;
	}

	if (m_resolveNeedsFaceChange)
	{
		// このステップは表情変更（現状プレースホルダー）。
		ChangePuyoFaceBeforeErase();
		// 次ステップは消去判定。
		m_resolveNeedsFaceChange = false;
		m_resolveNeedsErase = true;
		return;
	}

	if (m_resolveNeedsErase)
	{
		// このステップは消去判定。
		if (EraseConnectedPuyo())
		{
			// 消去があったら次ステップは再び重力落下。
			m_resolveNeedsErase = false;
			m_resolveNeedsGravity = true;
		}
		else
		{
			// 消去なしなら解決終了。次フレームで新ぷよ生成へ進む。
			m_isResolving = false;
			m_resolveNeedsGravity = false;
			m_resolveNeedsFaceChange = false;
			m_resolveNeedsErase = false;
			m_time = 1.0f;
		}
	}
}

bool PuyoManager::EraseConnectedPuyo()
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

void PuyoManager::CollectConnectedPuyo(int col, int row, Puyo::PuyoColor color, bool visited[6][12], std::vector<std::pair<int, int>>& outCells) const
{// 盤面外は探索終了。
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

bool PuyoManager::ApplyGravityToBoard()
{
	bool changed = false;

	// 各列を独立に下詰めする。
	for (int col = 0; col < 6; col++)
	{
		int writeRow = 11;
		for (int row = 11; row >= 0; row--)
		{
			if (m_pFixedPuyo[col][row]->GetColor() == Puyo::PuyoColor::None)
			{
				continue;
			}

			// writeRowへ詰める。位置が変わる場合のみ、ぷよ自体を下段へ移動させて補間アニメーションを開始する。
			if (row != writeRow)
			{
				auto movedPuyo = std::move(m_pFixedPuyo[col][row]);
				movedPuyo->SetRowCol(writeRow, col);
				movedPuyo->StartSmoothMove(GridToPosition(col, writeRow), FALL_INTERVAL);
				m_pFixedPuyo[col][writeRow] = std::move(movedPuyo);
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

void PuyoManager::RecreateFixedPuyo(int col, int row, Puyo::PuyoColor color)
{
	auto deviceResources = m_pCommonResources->GetDeviceResources();
	// 指定色でPuyoを作り直し、盤面座標と描画座標を必ず同期する。
	m_pFixedPuyo[col][row] = std::make_unique<Puyo>(color);
	m_pFixedPuyo[col][row]->SetRowCol(row, col);
	m_pFixedPuyo[col][row]->SetPosition(GridToPosition(col, row));
	m_pFixedPuyo[col][row]->Initialize(m_pCommonResources, deviceResources->GetOutputSize().right, deviceResources->GetOutputSize().bottom);
}

void PuyoManager::SyncFallingPuyoPosition()
{	// 軸ぷよが無い場合は同期不要。
	if (!m_pFallingPuyo[1][1])
	{
		return;
	}
	// 軸ぷよを新しい盤面座標へ配置。
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

bool PuyoManager::IsInsideBoard(int col, int row) const
{
	return (col >= 0 && col < 6 && row >= 0 && row < 12);
}

bool PuyoManager::IsCellEmpty(int col, int row) const
{
	if (!IsInsideBoard(col, row))
	{
		return false;
	}
	return m_pFixedPuyo[col][row]->GetColor() == Puyo::PuyoColor::None;
}

bool PuyoManager::IsFallingCellAt(int col, int row) const
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

DirectX::SimpleMath::Vector2 PuyoManager::GridToPosition(int col, int row) const
{
	using namespace DirectX::SimpleMath;
	return Vector2(0.4f + col * 0.05f, 0.045f + row * 0.0826f);
}

void PuyoManager::UpdateLandingPreview()
{
	if (!m_isFalling || !m_pFallingPuyo[1][1] || !m_pLandingPreviewPuyo[0] || !m_pLandingPreviewPuyo[1])
	{
		return;
	}

	int drop = 0;
	while (true)
	{
		const int nextCenterCol = m_fallCenterCol;
		const int nextCenterRow = m_fallCenterRow + drop + 1;
		const int nextSubCol = m_fallCenterCol + m_subOffsetCol;
		const int nextSubRow = m_fallCenterRow + m_subOffsetRow + drop + 1;

		if (!IsInsideBoard(nextCenterCol, nextCenterRow) || !IsInsideBoard(nextSubCol, nextSubRow))
		{
			break;
		}
		if (!IsCellEmpty(nextCenterCol, nextCenterRow) || !IsCellEmpty(nextSubCol, nextSubRow))
		{
			break;
		}
		drop++;
	}

	const int centerCol = m_fallCenterCol;
	const int centerRow = m_fallCenterRow + drop;
	const int subCol = m_fallCenterCol + m_subOffsetCol;
	const int subRow = m_fallCenterRow + m_subOffsetRow + drop;

	m_pLandingPreviewPuyo[0]->SetRowCol(centerRow, centerCol);
	m_pLandingPreviewPuyo[0]->SetPosition(GridToPosition(centerCol, centerRow));
	m_pLandingPreviewPuyo[1]->SetRowCol(subRow, subCol);
	m_pLandingPreviewPuyo[1]->SetPosition(GridToPosition(subCol, subRow));
}



