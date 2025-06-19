#pragma once
#include "Game/CommonResources/CommonResources.h"
#include "KumachiLib/DrawCollision/DrawCollision.h"
#include "DeviceResources.h"
#include <string>
#include <fstream>
#include <sstream>
#include <SimpleMath.h>
#include <Model.h>
#include <cassert>
#include <Effects.h>
class CommonResources;
class CSVMap
{
public:
	CSVMap(CommonResources* resources);
	~CSVMap();
	void LoadModel();
	void LoadMap(const std::string& filePath);
	void DrawCollision(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Render(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& proj);
private:
	CommonResources* m_commonResources;
	//マップ
	static const int MAXCOL = 5;
	static const int MAXRAW = 5;
	char m_map[MAXCOL][MAXRAW];
	DirectX::SimpleMath::Vector3 m_goalPosition;
	DirectX::BoundingBox m_goalBox;
	std::vector<DirectX::BoundingBox> m_wallBox;
	std::vector<DirectX::SimpleMath::Matrix> worldMatrices;
	DirectX::Model* m_pModel;
	// モデルパス
	wchar_t m_modelPath[256];


};