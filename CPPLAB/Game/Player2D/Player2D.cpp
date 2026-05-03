#include "pch.h"
#include "Player2D.h"

Player2D::Player2D()
	: m_pBillboardSprite(nullptr)
	, m_direction(ObjectDirection::Down)
	, m_currentDirection(ObjectDirection::Down)
	, m_moveState(MoveState::Idle)
	, m_turnTimer(0.0f)
	, m_moveTimer(0.0f)
	, m_position(0.0f, 0.0f, 0.0f)
	, m_moveStartPosition(0.0f, 0.0f, 0.0f)
	, m_targetPosition(0.0f, 0.0f, 0.0f)
	, m_isMoving(false)
	, m_prevKeyboardState{}
{
	Initialize();
}

void Player2D::Initialize()
{
	using namespace DirectX;
	using namespace DirectX::SimpleMath;
	auto textureManager = MyResourecs::Get().GetTextureManager();
	// ビルボードスプライトを生成する。
	m_pBillboardSprite = std::make_unique<BillboardSprite>(this);
	m_pBillboardSprite->SetTexture(textureManager->GetTexture("Player"));
	m_pBillboardSprite->Initialize();
	// ビルボードスプライトの位置とスケール を設定
	m_pBillboardSprite->SetPosition(m_position);
	m_pBillboardSprite->SetScale(0.50f);
	// ビルボード機能の有効/無効を設定
	m_pBillboardSprite->SetBillboard(true);
}

void Player2D::Update(float elapsedTime)
{
	using namespace DirectX::SimpleMath;

	// 入力状態を受け取る
	auto& keyboardState = MyResourecs::Get().GetInputManager()->GetKeyboardState();

	auto isPressed = [&](DirectX::Keyboard::Keys key)
		{
			return keyboardState.IsKeyDown(key);
		};

	auto isNewPress = [&](DirectX::Keyboard::Keys key)
		{
			return keyboardState.IsKeyDown(key) && !m_prevKeyboardState.IsKeyDown(key);
		};

	auto snapToGrid = [](const Vector3& position)
		{
			return Vector3(
				std::round(position.x),
				std::round(position.y),
				std::round(position.z));
		};

	auto beginMove = [&](ObjectDirection direction)
		{
			m_moveState = MoveState::Moving;
			m_isMoving = true;
			m_moveTimer = 0.0f;
			m_moveStartPosition = snapToGrid(m_position);
			m_position = m_moveStartPosition;
			m_targetPosition = m_moveStartPosition;

			switch (direction)
			{
				case ObjectDirection::Up:
					m_targetPosition.z -= 1.0f;
					break;
				case ObjectDirection::Down:
					m_targetPosition.z += 1.0f;
					break;
				case ObjectDirection::Left:
					m_targetPosition.x -= 1.0f;
					break;
				case ObjectDirection::Right:
					m_targetPosition.x += 1.0f;
					break;
			}
		};

	if (m_moveState == MoveState::Idle)
	{
		ObjectDirection inputDirection = m_currentDirection;
		bool hasInput = false;
		bool isNewInput = false;

		if (isPressed(DirectX::Keyboard::Keys::W))
		{
			inputDirection = ObjectDirection::Up;
			hasInput = true;
			isNewInput = isNewPress(DirectX::Keyboard::Keys::W);
		}
		else if (isPressed(DirectX::Keyboard::Keys::S))
		{
			inputDirection = ObjectDirection::Down;
			hasInput = true;
			isNewInput = isNewPress(DirectX::Keyboard::Keys::S);
		}
		else if (isPressed(DirectX::Keyboard::Keys::A))
		{
			inputDirection = ObjectDirection::Left;
			hasInput = true;
			isNewInput = isNewPress(DirectX::Keyboard::Keys::A);
		}
		else if (isPressed(DirectX::Keyboard::Keys::D))
		{
			inputDirection = ObjectDirection::Right;
			hasInput = true;
			isNewInput = isNewPress(DirectX::Keyboard::Keys::D);
		}

		if (hasInput)
		{
			m_direction = inputDirection;
			if (m_currentDirection != m_direction)
			{
				m_moveState = MoveState::Turning;
				m_currentDirection = m_direction;
			}
			else if (!isNewInput)
			{
				beginMove(m_currentDirection);
			}
		}
	}
	else if (m_moveState == MoveState::Turning)
	{
		m_turnTimer += elapsedTime;
		if (m_turnTimer >= TURN_TIME)
		{
			m_turnTimer = 0.0f;
			m_moveState = MoveState::Idle;
		}
	}
	else if (m_moveState == MoveState::Moving && m_isMoving)
	{
		m_moveTimer += elapsedTime;
		float t = m_moveTimer / MOVE_TIME;
		if (t >= 1.0f)
		{
			t = 1.0f;
			m_isMoving = false;
			m_moveState = MoveState::Idle;
		}

		m_position = Vector3::Lerp(m_moveStartPosition, m_targetPosition, t);
	}

	m_pBillboardSprite->SetPosition(m_position);
	m_pBillboardSprite->SetDirection(static_cast<int>(m_currentDirection));
	m_pBillboardSprite->Update(elapsedTime);

	m_prevKeyboardState = keyboardState;
}

void Player2D::Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	m_pBillboardSprite->Render(view, projection);
}

void Player2D::Finalize()
{}
