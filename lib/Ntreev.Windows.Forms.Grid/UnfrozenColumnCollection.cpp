//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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
#include "UnfrozenColumnCollection.h"
#include "GridControl.h"
#include "FromNative.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    UnfrozenColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
        : m_pColumnList(pColumnList)
    {
        m_index = 0;
    }

    UnfrozenColumnCollection::Enumerator::~Enumerator()
    {

    }

    bool UnfrozenColumnCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pColumnList->GetUnfrozenColumnCount();
    }

    void UnfrozenColumnCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Column^ UnfrozenColumnCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = m_pColumnList->GetUnfrozenColumn(m_index - 1);
        return FromNative::Get(pColumn);
    }

    System::Object^ UnfrozenColumnCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current;
    }

    UnfrozenColumnCollection::UnfrozenColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pColumnList = this->GridCore->GetColumnList();
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ UnfrozenColumnCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::UnfrozenColumnCollection::Enumerator(m_pColumnList); 
    }

    Ntreev::Windows::Forms::Grid::Column^ UnfrozenColumnCollection::default::get(int index)
    {
        if((unsigned int)index >= m_pColumnList->GetUnfrozenColumnCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        GrColumn* pColumn = m_pColumnList->GetUnfrozenColumn(index);
        return FromNative::Get(pColumn);
    }

    int UnfrozenColumnCollection::Count::get()
    {
        return m_pColumnList->GetUnfrozenColumnCount();
    }

    System::Collections::IEnumerator^ UnfrozenColumnCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void UnfrozenColumnCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool UnfrozenColumnCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ UnfrozenColumnCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this; 
    }

    int UnfrozenColumnCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/