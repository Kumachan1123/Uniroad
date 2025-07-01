/*
	@file	GridFloor.h
	@brief	グリッド床を描画するクラス
*/
#pragma once

namespace mylib
{
	class GridFloor
	{
	private:
		// データメンバ
		DirectX::CommonStates* m_commonStates;
		std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
		std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_primitiveBatch;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
		DirectX::SimpleMath::Color m_color;
		float m_size;
		size_t m_divs;


	public:
		// コンストラクタ
		GridFloor(
			ID3D11Device* device,
			ID3D11DeviceContext* context,
			DirectX::CommonStates* states,
			const DirectX::FXMVECTOR& color = DirectX::Colors::White,
			float size = 50.0f,	// 床の大きさ
			size_t divs = 25	// 分割数
		);

		// デストラクタ
		~GridFloor() = default;

		// 床を描画する
		void Render(
			ID3D11DeviceContext* context,
			const DirectX::SimpleMath::Matrix& view,
			const DirectX::SimpleMath::Matrix& projection
		);

		// 線の色を指定する
		void SetColor(const DirectX::FXMVECTOR& color)
		{
			m_color = color;
		}
	};
}
