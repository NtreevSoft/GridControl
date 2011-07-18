#include "StdAfx.h"
#include "GrGridInternal.h"
#include "GrGridRenderer.h"
#include <assert.h>


uint GrUtil::Sum(uint n1, int n2)
{
	uint nReturn = n1 + n2;
	if(n2 < 0 && n1 < nReturn)
	{
		return 0;
	}
	return nReturn;
}

uint GrUtil::LastIndex(uint nCount)
{
	assert(nCount > 0);
	return nCount - 1;
}

uint GrUtil::MakeHash(const wchar_t* strText)
{
	uint hashValue = 5381;
	while(*strText != 0)
	{
		hashValue = ((hashValue << 5) + hashValue) + (uint)(*strText); // dwHash * 33 + ch
		strText++;
	}   
	return hashValue;
}

void GrTextUtil::SingleLine(GrLineDesc* pLine, const std::wstring& strText, const GrFont* pFont)
{
	std::wstring::const_iterator itor = strText.begin();
	memset(pLine, 0, sizeof(GrLineDesc));
	pLine->nLength = strText.length();

	while(itor != strText.end())
	{
		wchar_t s = *itor;
		if(s != L'\n')
		{
			int nCharWidth = pFont->GetWidth(s);
			pLine->nWidth += nCharWidth;
		}
		itor++;
	}
}

void GrTextUtil::MultiLine(std::vector<GrLineDesc>* pLines, const std::wstring& strText, int nCellWidth, const GrFont* pFont, bool bWordWrap)
{
	if(bWordWrap == true)
		DoMultilineWordWrap(pLines, strText, nCellWidth, pFont);
	else
		DoMultiline(pLines, strText, pFont);
}

void GrTextUtil::DoMultiline(std::vector<GrLineDesc>* pLines, const std::wstring& strText, const GrFont* pFont)
{
	uint pos = 0;
	GrLineDesc cl;
	cl.nWidth  = 0;
	cl.nLength = 0;
	cl.nTextBegin = 0;

	while(pos != strText.length())
	{
		wchar_t s = strText.at(pos);
		int nCharWidth = pFont->GetWidth(s);
		if(s == L'\n')
		{
			pLines->push_back(cl);
			cl.nTextBegin = pos;
			cl.nLength = 0;
			cl.nWidth = 0;
		}
		else 
		{
			cl.nWidth += nCharWidth;
			cl.nLength++;
		}
		pos++;
	}

	pLines->push_back(cl);
}

void GrTextUtil::WordWrap(WordList* pWordList, const std::wstring& strText, const GrFont* pFont, int nCellWidth)
{
	bool wordBreak = false;

	std::wstring::const_iterator itor = strText.begin();
	GrWordDesc wd;
	uint pos = 0;
	memset(&wd, 0, sizeof(GrWordDesc));

	while(pos != strText.length())
	{
		wchar_t s = strText.at(pos);
		int width = pFont->GetWidth(s);

		if((wordBreak == true && s != L' ') || wd.width + width > nCellWidth || s > 0xff || s == L'\n')
		{
			pWordList->push_back(wd);
			memset(&wd, 0, sizeof(GrWordDesc));
			wd.pos = pos;
			wordBreak = false;
		}

		wd.length++;
		wd.width += width;

		if(s == L' ')
			wordBreak = true;
		else
			wd.validWidth += width;

		pos++;
	}

	if(wd.length != 0)
	{
		pWordList->push_back(wd);
	}
}

void GrTextUtil::DoMultilineWordWrap(std::vector<GrLineDesc>* pLines, const std::wstring& strText, int nCellWidth, const GrFont* pFont)
{
	WordList words;
	WordWrap(&words, strText, pFont, nCellWidth);

	int pos=0;
	GrLineDesc cl;
	memset(&cl, 0, sizeof(GrLineDesc));
	for_stl_const(WordList, words, itor)
	{
		WordList::value_type value = *itor;

		if(cl.nWidth + value.validWidth > nCellWidth || strText.at(value.pos) == L'\n')
		{
			pLines->push_back(cl);
			memset(&cl, 0, sizeof(GrLineDesc));
			cl.nTextBegin = pos;
		}

		cl.nWidth  += value.width;
		cl.nLength += value.length;
		pos += value.length;
	}

	pLines->push_back(cl);
}
