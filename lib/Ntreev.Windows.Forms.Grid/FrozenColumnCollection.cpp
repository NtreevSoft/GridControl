//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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
#include "FrozenColumnCollection.h"
#include "GridControl.h"
#include "FromNative.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    FrozenColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
        : m_pColumnList(pColumnList)
    {
        m_index = 0;
    }

    FrozenColumnCollection::Enumerator::~Enumerator()
    {

    }

    bool FrozenColumnCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pColumnList->GetFrozenColumnCount();
    }

    void FrozenColumnCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Column^ FrozenColumnCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = m_pColumnList->GetFrozenColumn(m_index - 1);
        System::Object^ ref = pColumn->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Column^>(ref);
    }

    System::Object^ FrozenColumnCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current; 
    }

    FrozenColumnCollection::FrozenColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pColumnList = this->GridCore->GetColumnList();
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ FrozenColumnCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::FrozenColumnCollection::Enumerator(m_pColumnList); 
    }

    Ntreev::Windows::Forms::Grid::Column^ FrozenColumnCollection::default::get(int index)
    {
        if((unsigned int)index >= m_pColumnList->GetFrozenColumnCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        GrColumn* pColumn = m_pColumnList->GetFrozenColumn(index);
        return FromNative::Get(pColumn);
    }

    int FrozenColumnCollection::Count::get()
    {
        return m_pColumnList->GetFrozenColumnCount();
    }

    System::Collections::IEnumerator^ FrozenColumnCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void FrozenColumnCollection::CopyT_System_Collections_ICollectiono(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool FrozenColumnCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true; 
    }

    System::Object^ FrozenColumnCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this; 
    }

    int FrozenColumnCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count; 
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/