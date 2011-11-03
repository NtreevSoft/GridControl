//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


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