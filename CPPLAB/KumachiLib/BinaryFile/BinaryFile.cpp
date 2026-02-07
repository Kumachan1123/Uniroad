#include "pch.h"
#include "BinaryFile.h"

#include <fstream>
#include <assert.h>


KumachiLib::BinaryFile KumachiLib::BinaryFile::LoadFile(const wchar_t* fileName)
{
	BinaryFile bin;

	std::ifstream ifs;

	//	ファイルオープン
	ifs.open(fileName, std::ios::in | std::ios::binary);

	//	読み込み失敗時、強制終了
	assert(ifs);

	//	ファイルサイズを取得
	ifs.seekg(0, std::fstream::end);
	std::streamoff eofPos = ifs.tellg();
	ifs.clear();
	ifs.seekg(0, std::fstream::beg);
	std::streamoff begPos = ifs.tellg();
	bin.m_size = (unsigned int)(eofPos - begPos);

	//	読み込むためのメモリを確保
	bin.m_data.reset(new char[bin.m_size]);

	//	ファイル先頭からバッファへコピー 
	ifs.read(bin.m_data.get(), bin.m_size);

	//	ファイルクローズ
	ifs.close();

	return std::move(bin);
}

KumachiLib::BinaryFile::BinaryFile()
{
	m_size = 0;
}

KumachiLib::BinaryFile::BinaryFile(BinaryFile&& in)
{
	m_data = std::move(in.m_data);
	m_size = in.m_size;
}
