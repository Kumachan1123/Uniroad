#pragma once
#ifndef Player2D_h
#define Player2D_h
#include <KumachiLib\BillboardSprite\BillboardSprite.h>
#include <Libraries\MyLib\InputManager.h>
#include <Game\MyResources\MyResources.h>
#include <Game\GameObject\IGameObject.h>
#include <Game\Player2D\PlayerDirection.h>
class Player2D : public IGameObject
{
public:
	enum class MoveState
	{
		Idle = 0,// 待機
		Turning = 1,// 向き変更
		Moving = 2// 移動
	};
public:
	Player2D();
	~Player2D() = default;

	void Initialize()override;
	void Update(float elapsedTime)override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)override;
	void Finalize()override;
private:
	const float TURN_TIME = 0.02f;// 向き変更の時間
	const float MOVE_TIME = 0.3f;// 1マス移動にかける時間
private:
	std::unique_ptr<BillboardSprite> m_pBillboardSprite;// ビルボードスプライト
	ObjectDirection m_direction;// プレイヤーの次の向き
	ObjectDirection m_currentDirection;// プレイヤーの現在の向き
	MoveState m_moveState;// プレイヤーの移動状態
	float m_turnTimer;// 向き変更のタイマー
	float m_moveTimer;// 移動タイマー
	DirectX::SimpleMath::Vector3 m_position;// 現在位置
	DirectX::SimpleMath::Vector3 m_moveStartPosition;// 移動開始位置
	DirectX::SimpleMath::Vector3 m_targetPosition;// 移動先位置
	bool m_isMoving;// 移動中フラグ
	DirectX::Keyboard::State m_prevKeyboardState;// 前フレームの入力状態
};
#endif // !Player2D_h
