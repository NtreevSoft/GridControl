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


#include "StdAfx.h"
#include "GridGroupingRow.h"
#include "GridUtil.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	GroupingRow::GroupingRow(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, GrGroupingList* pGroupingList) 
		: CellBase(gridControl, pGroupingList) , m_pGroupingList(pGroupingList)
	{
		this->Text = "그룹핑할 컬럼을 이쪽으로 드래그하여 옮기세요";
	}

	System::String^ GroupingRow::Text::get()
	{
		return gcnew System::String(m_pGroupingList->GetText());
	}

	void GroupingRow::Text::set(System::String^ text)
	{
		m_pGroupingList->SetText(ToNativeString::Convert(text));
	}

	bool GroupingRow::IsVisible::get()
	{
		return m_pGroupingList->GetVisible();
	}

	void GroupingRow::IsVisible::set(bool value)
	{
		m_pGroupingList->SetVisible(value);
	}

    bool GroupingRow::ShouldSerializeText()
	{
		return this->Text != "그룹핑할 컬럼을 이쪽으로 드래그하여 옮기세요";
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/