//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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


#include "stdafx.h"
#include "RowBaseCollection.h"
#include "FromNative.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    RowBaseCollection::Enumerator::Enumerator(IDataRow* pDataRow)
        : m_pDataRow(pDataRow)
    {
        m_index = 0;
    }

    RowBaseCollection::Enumerator::~Enumerator()
    {

    }

    bool RowBaseCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pDataRow->GetChildCount();
    }

    void RowBaseCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    RowBase^ RowBaseCollection::Enumerator::Current::get()
    {
        IDataRow* pDataRow = (IDataRow*)m_pDataRow->GetChild(m_index - 1);
        return FromNative::Get(pDataRow);
    }

    System::Object^ RowBaseCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return this->Current; 
    }

    RowBaseCollection::RowBaseCollection(IDataRow* pDataRow)
        : m_pDataRow(pDataRow)
    {
        
    }

    System::Collections::Generic::IEnumerator<RowBase^>^ RowBaseCollection::GetEnumerator()
    {
        return gcnew RowBaseCollection::Enumerator(m_pDataRow); 
    }

    RowBase^ RowBaseCollection::default::get(int index)
    {
        if(index >= this->Count)
            throw gcnew System::ArgumentOutOfRangeException("index");

        IDataRow* pDataRow = (IDataRow*)m_pDataRow->GetChild(index);
        return FromNative::Get(pDataRow);
    }

    int RowBaseCollection::Count::get()
    {
        return m_pDataRow->GetChildCount();
    }

    System::Collections::IEnumerator^ RowBaseCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void RowBaseCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::RowBase^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool RowBaseCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true; 
    }

    System::Object^ RowBaseCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int RowBaseCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count; 
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/