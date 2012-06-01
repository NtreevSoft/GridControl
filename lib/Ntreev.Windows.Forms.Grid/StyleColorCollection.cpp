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
#include "StyleColorCollection.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    StyleColorCollection::StyleColorCollection(std::vector<GrColor>* container)
        : m_s(container)
    {

    }

    void StyleColorCollection::RemoveAt(int index)
    {
        m_s.RemoveAt(index);
    }

    void StyleColorCollection::Insert(int index, System::Drawing::Color item)
    {
        m_s.Insert(index, item);
    }

    void StyleColorCollection::CopyTo(cli::array<System::Drawing::Color>^ array, int arrayIndex)
    {
        m_s.CopyTo(array, arrayIndex);
    }

    void StyleColorCollection::Clear()
    {
        m_s.Clear();
    }

    void StyleColorCollection::Add(System::Drawing::Color item)
    {
        m_s.Add(item);
        if(this->GridControl != nullptr)
        {
            this->GridControl->Invalidate();
        }
    }

    int StyleColorCollection::IndexOf(System::Drawing::Color item)
    {
        return m_s.IndexOf(item);
    }

    bool StyleColorCollection::Remove(System::Drawing::Color item)
    {
        return m_s.Remove(item);
    }

    bool StyleColorCollection::Contains(System::Drawing::Color item)
    {
        return m_s.Contains(item);
    }

    System::Drawing::Color StyleColorCollection::default::get(int index)
    {
        return m_s[index];
    }

    void StyleColorCollection::default::set(int index, System::Drawing::Color item)
    {
        m_s[index] = item;
    }

    int StyleColorCollection::Count::get()
    {
        return m_s.Count;
    }

    bool StyleColorCollection::IsReadOnly::get()
    {
        return m_s.IsReadOnly;
    }

    System::Collections::IEnumerator^ StyleColorCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return dynamic_cast<System::Collections::IEnumerable^>(%m_s)->GetEnumerator();
    }

    System::Collections::Generic::IEnumerator<System::Drawing::Color>^ StyleColorCollection::GetEnumerator_System_Collections_Generic_IEnumerable_GetEnumerator()
    {
        return dynamic_cast<System::Collections::Generic::IEnumerable<System::Drawing::Color>^>(%m_s)->GetEnumerator();
    }

    void StyleColorCollection::Remove_System_Collections_IList(System::Object^ item)
    {
        dynamic_cast<System::Collections::IList^>(%m_s)->Remove(item);
    }

    void StyleColorCollection::Insert_System_Collections_IList(int index, System::Object^ item)
    {
        dynamic_cast<System::Collections::IList^>(%m_s)->Insert(index, item);
    }

    void StyleColorCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int arrayIndex)
    {
        dynamic_cast<System::Collections::ICollection^>(%m_s)->CopyTo(array, arrayIndex);
    }

    int StyleColorCollection::IndexOf_System_Collections_IList(System::Object^ item)
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->IndexOf(item);
    }

    int StyleColorCollection::Add_System_Collections_IList(System::Object^ item)
    {
        if(this->GridControl != nullptr)
            this->GridControl->Invalidate();
        return dynamic_cast<System::Collections::IList^>(%m_s)->Add(item);
    }

    bool StyleColorCollection::Contains_System_Collections_IList(System::Object^ item)
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->Contains(item);
    }

    System::Object^ StyleColorCollection::default_System_Collections_IList::get(int index)
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)[index];
    }

    void StyleColorCollection::default_System_Collections_IList::set(int index, System::Object^ item)
    {
        dynamic_cast<System::Collections::IList^>(%m_s)[index] = item;
    }

    System::Object^ StyleColorCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return dynamic_cast<System::Collections::ICollection^>(%m_s)->SyncRoot;
    }

    bool StyleColorCollection::IsFixedSize_System_Collections_IList::get()
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->IsFixedSize;
    }

    bool StyleColorCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->IsSynchronized;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/