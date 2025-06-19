#pragma once
/// <summary>
/// バイナリファイルを扱うクラス
/// </summary>


#include <memory>
namespace KumachiLib
{
	class BinaryFile
	{
	protected:

		//	データ
		std::unique_ptr<char[]> m_data;

		//	サイズ
		unsigned int m_size;

	public:

		//	ファイル名を指定してロード
		static KumachiLib::BinaryFile LoadFile(const wchar_t* fileName);

		BinaryFile();

		//	ムーブコンストラクタ
		BinaryFile(BinaryFile&& in);

		//	アクセサ
		char* GetData() { return m_data.get(); }
		unsigned int GetSize() const { return m_size; }
	};
}