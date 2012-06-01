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
#include "StyleFontCollection.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    StyleFontCollection::StyleFontCollection()
    {

    }

    void StyleFontCollection::RemoveAt(int index)
    {
        m_s.RemoveAt(index);
    }

    void StyleFontCollection::Insert(int index, System::Drawing::Font^ item)
    {
        m_s.Insert(index, item);
    }

    void StyleFontCollection::CopyTo(cli::array<System::Drawing::Font^>^ array, int arrayIndex)
    {
        m_s.CopyTo(array, arrayIndex);
    }

    void StyleFontCollection::Clear()
    {
        m_s.Clear();
    }

    void StyleFontCollection::Add(System::Drawing::Font^ item)
    {
        m_s.Add(item);
    }

    int StyleFontCollection::IndexOf(System::Drawing::Font^ item)
    {
        return m_s.IndexOf(item);
    }

    bool StyleFontCollection::Remove(System::Drawing::Font^ item)
    {
        return m_s.Remove(item);
    }

    bool StyleFontCollection::Contains(System::Drawing::Font^ item)
    {
        return m_s.Contains(item);
    }

    System::Drawing::Font^ StyleFontCollection::default::get(int index)
    {
        return m_s[index];
    }

    void StyleFontCollection::default::set(int index, System::Drawing::Font^ item)
    {
        m_s[index] = item;
    }

    int StyleFontCollection::Count::get()
    {
        return m_s.Count;
    }

    bool StyleFontCollection::IsReadOnly::get()
    {
        return m_s.IsReadOnly;
    }
        
    System::Collections::IEnumerator^ StyleFontCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return dynamic_cast<System::Collections::IEnumerable^>(%m_s)->GetEnumerator();
    }

    System::Collections::Generic::IEnumerator<System::Drawing::Font^>^ StyleFontCollection::GetEnumerator_System_Collections_Generic_IEnumerable_GetEnumerator()
    {
        return dynamic_cast<System::Collections::Generic::IEnumerable<System::Drawing::Font^>^>(%m_s)->GetEnumerator();
    }

    void StyleFontCollection::Remove_System_Collections_IList(System::Object^ item)
    {
        dynamic_cast<System::Collections::IList^>(%m_s)->Remove(item);
    }

    void StyleFontCollection::Insert_System_Collections_IList(int index, System::Object^ item)
    {
        dynamic_cast<System::Collections::IList^>(%m_s)->Insert(index, item);
    }

    void StyleFontCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int arrayIndex)
    {
        dynamic_cast<System::Collections::ICollection^>(%m_s)->CopyTo(array, arrayIndex);
    }

    int StyleFontCollection::IndexOf_System_Collections_IList(System::Object^ item)
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->IndexOf(item);
    }

    int StyleFontCollection::Add_System_Collections_IList(System::Object^ item)
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->Add(item);
    }

    bool StyleFontCollection::Contains_System_Collections_IList(System::Object^ item)
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->Contains(item);
    }

    System::Object^ StyleFontCollection::default_System_Collections_IList::get(int index)
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)[index];
    }

    void StyleFontCollection::default_System_Collections_IList::set(int index, System::Object^ item)
    {
        dynamic_cast<System::Collections::IList^>(%m_s)[index] = item;
    }

    System::Object^ StyleFontCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return dynamic_cast<System::Collections::ICollection^>(%m_s)->SyncRoot;
    }

    bool StyleFontCollection::IsFixedSize_System_Collections_IList::get()
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->IsFixedSize;
    }

    bool StyleFontCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return dynamic_cast<System::Collections::IList^>(%m_s)->IsSynchronized;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/