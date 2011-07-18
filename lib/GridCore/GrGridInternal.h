#pragma once
#include "GrGridType.h"
#include "GrGridCell.h"

class GrUtil
{
public:
	static uint			Sum(uint n1, int n2);
	static uint			LastIndex(uint nCount);
	static uint			MakeHash(const wchar_t* strText);
};

struct GrWordDesc
{
	int	pos;
	int	length;
	int	width;
	int	validWidth;
};

typedef std::vector<GrWordDesc>		WordList;
typedef std::vector<GrLineDesc>		LineList;

class GrFont;

class GrTextUtil
{
public:
	static void SingleLine(GrLineDesc* pLine, const std::wstring& strText, const GrFont* pFont);
	static void MultiLine(std::vector<GrLineDesc>* pLines, const std::wstring& strText, int nCellWidth, const GrFont* pFont, bool bWordWrap);

private:
	static void WordWrap(WordList* pWordList, const std::wstring& strText, const GrFont* pFont, int nCellWidth);
	static void DoMultiline(std::vector<GrLineDesc>* pLines, const std::wstring& strText, const GrFont* pFont);
	static void DoMultilineWordWrap(std::vector<GrLineDesc>* pLines, const std::wstring& strText, int nCellWidth, const GrFont* pFont);

	static WordList	m_sWordList;
};