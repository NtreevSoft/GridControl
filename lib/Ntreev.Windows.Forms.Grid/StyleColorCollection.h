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


#pragma once
#include "NativeCollection.h"
#include "GridObject.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	[System::ComponentModel::EditorAttribute("Ntreev.Windows.Forms.Grid.Design.StyleColorCollectionEditor, Ntreev.Windows.Forms.Grid.Design, Version=2.0.4510.20986, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", System::Drawing::Design::UITypeEditor::typeid)]
    public ref class StyleColorCollection
        : Ntreev::Windows::Forms::Grid::GridObject
        , System::Collections::Generic::IList<System::Drawing::Color>
        , System::Collections::IList
    {
    public:
        virtual void RemoveAt(int index);

        virtual void Insert(int index, System::Drawing::Color item);

        virtual void CopyTo(cli::array<System::Drawing::Color>^ array, int arrayIndex);

        virtual void Clear();

        virtual void Add(System::Drawing::Color item);

        virtual int IndexOf(System::Drawing::Color item);

        virtual bool Remove(System::Drawing::Color item);

        virtual bool Contains(System::Drawing::Color item);

    public:
        virtual property System::Drawing::Color default[int]
        {
            System::Drawing::Color get(int index);
            void set(int index, System::Drawing::Color);
        }

        virtual property int Count
        {
            int get();
        }

        virtual property bool IsReadOnly
        {
            bool get();
        }

    internal:
        StyleColorCollection(std::vector<GrColor>* container, System::Drawing::Color color);

	internal: // properties
		property System::Drawing::Color DefaultColor
		{
			System::Drawing::Color get();
		}

    private:
		void Invalidate();

        virtual System::Collections::IEnumerator^ GetEnumerator_System_Collections_IEnumerable() sealed = System::Collections::IEnumerable::GetEnumerator;
        virtual System::Collections::Generic::IEnumerator<System::Drawing::Color>^ GetEnumerator_System_Collections_Generic_IEnumerable_GetEnumerator() sealed
            = System::Collections::Generic::IEnumerable<System::Drawing::Color>::GetEnumerator;

        virtual void Remove_System_Collections_IList(System::Object^ item) sealed = System::Collections::IList::Remove;
        virtual void Insert_System_Collections_IList(int index, System::Object^ item) sealed = System::Collections::IList::Insert;
        virtual void CopyTo_System_Collections_ICollection(System::Array^ array, int arrayIndex) sealed = System::Collections::ICollection::CopyTo;
        virtual int IndexOf_System_Collections_IList(System::Object^ item) sealed = System::Collections::IList::IndexOf;
        virtual int Add_System_Collections_IList(System::Object^ item) sealed = System::Collections::IList::Add;
        virtual bool Contains_System_Collections_IList(System::Object^ item) sealed = System::Collections::IList::Contains;

    private:
        virtual property System::Object^ default_System_Collections_IList[int]
        {
            System::Object^ get(int index) sealed = System::Collections::IList::default::get;
            void set(int index, System::Object^ item) sealed = System::Collections::IList::default::set;
        }

        virtual property System::Object^ SyncRoot_System_Collections_ICollection
        {
            System::Object^ get() sealed = System::Collections::ICollection::SyncRoot::get;
        }

        virtual property bool IsFixedSize_System_Collections_IList
        {
            bool get() sealed = System::Collections::IList::IsFixedSize::get;
        }

        virtual property bool IsSynchronized_System_Collections_ICollection
        {
            bool get() sealed = System::Collections::ICollection::IsSynchronized::get;
        }

    private:
        NativeCollection<System::Drawing::Color, GrColor> m_s;
		System::Drawing::Color m_defaultColor;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/