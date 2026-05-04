#pragma once
#ifndef MapTile_H
#define MapTile_H
#include <Game\GameObject\IGameObject.h>
#include <Game\MyResources\MyResources.h>
#include <KumachiLib\DrawPolygon\DrawPolygon.h>
#include <KumachiLib\CreateShader\CreateShader.h>

class MapTile : public IGameObject
{
public:
	// アクセサ
	// 座標取得
	DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
	// テクスチャをセット
	void SetTexture(ID3D11ShaderResourceView* texture) { m_pTexture = texture; }
	// 位置をセット
	void SetPosition(const DirectX::SimpleMath::Vector3& position) { m_position = position; }
	// マップチップの行数と列数をセット
	void SetFrame(int rows, int cols) { m_frameRows = rows; m_frameCols = cols; }
	// マップチップの番号をセット
	void SetChipNum(int chipNum) { m_chipNum = chipNum; }
	// マップ上での行と列を取得
	int GetRow() const { return m_row; }
	int GetCol() const { return m_col; }

	// マップ上での位置をセット
	void SetMapPosition(int row, int col) { m_row = row; m_col = col; }

public:
	MapTile();
	~MapTile() = default;
	void Initialize()override;
	void Update(float elapsedTime)override;
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)override;
	void Finalize()override;
private:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix matWorld;
		DirectX::SimpleMath::Matrix matView;
		DirectX::SimpleMath::Matrix matProj;
		DirectX::SimpleMath::Vector4 count;
		DirectX::SimpleMath::Vector4 height;
		DirectX::SimpleMath::Vector4 width;
	};

	DrawPolygon* m_pDrawPolygon;
	CreateShader* m_pCreateShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBuffer;
	DrawPolygon::Shaders m_shaders;
	DirectX::VertexPositionTexture m_vertices[4];
	ID3D11ShaderResourceView* m_pTexture;
	ConstBuffer m_constBuffer;
	// マップチップの行数と列数
	int m_frameRows;//行
	int m_frameCols;//列
	// マップチップの番号
	int m_chipNum;
	// タイル一枚の座標
	DirectX::SimpleMath::Vector3 m_position;
	// マップのタイルの位置（行と列）
	int m_row;// 行
	int m_col;// 列
};
#endif // !MapTile_H
