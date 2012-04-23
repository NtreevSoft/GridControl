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
#include "Row.h"
#include "Events.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 행의 컬렉션을 나타냅니다.
    /// </summary>
    [System::ComponentModel::EditorAttribute("Ntreev.Windows.Forms.Grid.Design.RowCollectionEditor, Ntreev.Windows.Forms.Grid.Design, Version=2.0.4478.19833, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", System::Drawing::Design::UITypeEditor::typeid)]
    [System::ComponentModel::Design::Serialization::DesignerSerializerAttribute("Ntreev.Windows.Forms.Grid.Design.RowCollectionCodeDomSerializer, Ntreev.Windows.Forms.Grid.Design, Version=2.0.4478.19833, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", "System.ComponentModel.Design.Serialization.CodeDomSerializer")]
    public ref class RowCollection
        : Ntreev::Windows::Forms::Grid::GridObject
        , System::Collections::Generic::IList<Ntreev::Windows::Forms::Grid::Row^>
        , System::Collections::IList
    {
    private: // classes

        ref class Enumerator : System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>
        {
        public:
            Enumerator(GrDataRowList* pDataRowList);
            ~Enumerator();
            virtual bool MoveNext();
            virtual void Reset();

        public:
            property Ntreev::Windows::Forms::Grid::Row^ Current
            {
                virtual Ntreev::Windows::Forms::Grid::Row^ get();
            }

        private:
            property System::Object^ Current_System_Collections_IEnumerator
            {
                virtual System::Object^ get() sealed = System::Collections::IEnumerator::Current::get;
            }

        private:
            unsigned int m_index;
            GrDataRowList* m_pDataRowList;
        };

        ref class ManagerEventDetach
        {
        public:
            ManagerEventDetach(Ntreev::Windows::Forms::Grid::RowCollection^ rowCollection);
            ~ManagerEventDetach();

        private:
            Ntreev::Windows::Forms::Grid::RowCollection^ m_rowCollection;
        };

    public: // methods

        /// <summary>
        /// 새로운 행의 인스턴스를 생성하고 컬렉션에 추가합니다.
        /// </summary>
        /// <returns>
        /// 새로 생성된 <see cref="Row"/>의 인스턴스입니다.
        /// </returns>
        Ntreev::Windows::Forms::Grid::Row^ Add();

        /// <summary>
        /// 여러개의 새로운 행의 인스턴스를 생성하고 컬렉션에 추가합니다.
        /// </summary>
        /// <returns>
        /// 새로 생성된 <see cref="Row"/>의 인스턴스들입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// 생성 갯수가 0보다 같거나 작을 경우
        /// </exception>
        /// <exception cref="System::NotSupportedException">
        /// 새로운 행을 생성할 수 없는 경우
        /// </exception>
        cli::array<Ntreev::Windows::Forms::Grid::Row^>^ Add(int count);

        /// <summary>
        /// <see cref="InsertionRow"/>의 값을 이용하여 새로운 행의 인스턴스를 생성하고 컬렉션에 추가합니다.
        /// </summary>
        /// <returns>
        /// 새로 생성된 <see cref="Row"/>의 인스턴스입니다.
        /// </returns>
        /// <exception cref="System::NotSupportedException">
        /// 새로운 행을 생성할 수 없는 경우
        /// </exception>
        Ntreev::Windows::Forms::Grid::Row^ AddFromInsertion();

        /// <summary>
        /// 컬렉션에 새로운 행을 추가합니다.
        /// </summary>
        /// <remarks>
        /// 사용자는 <see cref="Row"/>형태의 인스턴스를 새로 생성해 추가 하거나, <see cref="Remove"/>등의 의해 컬렉션에서 제거된 행을 다시 추가할 수 있습니다.
        /// </remarks>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 추가할 행이 이미 존재하거나, 같은 이름을 가진 행이 컬렉션에 이미 존재하는 경우
        /// </exception>
        /// <exception cref="System::NotSupportedException">
        /// 새로운 행을 생성할 수 없는 경우
        /// </exception>
        virtual void Add(Ntreev::Windows::Forms::Grid::Row^ item);

        /// <summary>
        /// 컬렉션에서 행을 제거합니다.
        /// </summary>
        /// <returns>
        /// 제거가 완료되면 true를 반환하고, 그렇지 않으면 false를 반환합니다.
        /// </returns>
        /// <param name="item">
        /// 삭제할 <see cref="Row"/>입니다.
        /// </param>
        /// <exception cref="System::ArgumentNullException">
        /// row가 null일경우 발생합니다.
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 이미 제거가 된 경우 또는 제거할 수 없는 특수한 행일 경우
        /// </exception>
        virtual bool Remove(Ntreev::Windows::Forms::Grid::Row^ item);

        /// <summary>
        /// 컬렉션에서 행이 포함되어 있는지 확인합니다.
        /// </summary>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <returns>
        /// 컬렉션에 포함되어있다면 true, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        virtual bool Contains(Ntreev::Windows::Forms::Grid::Row^ item);

        /// <summary>
        /// 컬렉션에서 행의 위치를 확인합니다.
        /// </summary>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <returns>
        /// 컬렉션에 포함되어 있지 않다면 -1을 반환합니다.
        /// </returns>
        virtual int IndexOf(Ntreev::Windows::Forms::Grid::Row^ item);

        /// <summary>
        /// 컬렉션에 모든 행을 제거합니다.
        /// </summary>
        virtual void Clear();

        /// <summary>
        /// 컬렉션에 새로운 행을 삽입합니다.
        /// </summary>
        /// <remarks>
        /// 사용자는 <see cref="Row"/>형태의 인스턴스를 새로 생성해 삽입 하거나, <see cref="Remove"/>등의 의해 컬렉션에서 제거된 행을 다시 삽입할 수 있습니다.
        /// </remarks>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 삽입할 행이 이미 존재하거나, 같은 이름을 가진 행이 컬렉션에 이미 존재하는 경우
        /// </exception>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        virtual void Insert(int index, Ntreev::Windows::Forms::Grid::Row^ item);

        /// <summary>
        /// 컬렉션에서 지정된 위치의 행을 제거합니다.
        /// </summary>
        /// <param name="index">
        /// 제거할 행의 위치를 나타냅니다.
        /// </param>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        virtual void RemoveAt(int index);

        /// <summary>
        /// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>^ GetEnumerator();

        /// <summary>
        /// 지정한 인덱스에 있는 <see cref="Row"/>를 가져옵니다.
        /// </summary>
        /// <param name="index">
        /// 가져올 <see cref="Row"/>의 인덱스(0부터 시작)입니다.
        /// </param>
        /// <returns>
        /// 지정한 인덱스의 <see cref="Row"/>입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        Ntreev::Windows::Forms::Grid::Row^ GetAt(int index);

    public: // properties
        /// <summary>
        /// 지정한 인덱스에 있는 <see cref="Row"/>를 가져옵니다.
        /// </summary>
        /// <param name="index">
        /// 가져올 <see cref="Row"/>의 인덱스(0부터 시작)입니다.
        /// </param>
        /// <returns>
        /// 지정한 인덱스의 <see cref="Row"/>입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        property Ntreev::Windows::Forms::Grid::Row^ default[int]
        {
            virtual Ntreev::Windows::Forms::Grid::Row^ get(int index);
        private:
            virtual void set(int index, Ntreev::Windows::Forms::Grid::Row^ item) sealed = System::Collections::Generic::IList<Ntreev::Windows::Forms::Grid::Row^>::default::set;
        }

        /// <summary>
        /// 항목의 갯수를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 갯수를 나타내는 <see cref="System::Int32"/>의 정수값입니다.
        /// </returns>
        property int Count
        {
            virtual int get();
            void set(int);
        }

        /// <summary>
        /// <see cref="InsertionRow"/>을 가져옵니다.
        /// </summary>
        property Ntreev::Windows::Forms::Grid::Row^ InsertionRow
        {
            Ntreev::Windows::Forms::Grid::Row^ get();
        }

    internal: // methods

        RowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

        void Bind(System::Object^ component, int componentIndex);

        void Unbind(int componentIndex);

        void SetItemsByDesigner(cli::array<System::Object^>^ values);

    internal: // properties

        property Ntreev::Windows::Forms::Grid::Row^ default[GrDataRow*]
        {
            Ntreev::Windows::Forms::Grid::Row^ get(GrDataRow* pDataRow);
        }

        property Ntreev::Windows::Forms::Grid::Row^ default[System::Object^]
        {
            Ntreev::Windows::Forms::Grid::Row^ get(System::Object^ component);
        }

    private: // methods

        virtual System::Collections::IEnumerator^ GetEnumerator_System_Collections_IEnumerable() sealed = System::Collections::IEnumerable::GetEnumerator;

        virtual void RemoveAt_System_Collections_IList(int index) sealed = System::Collections::IList::RemoveAt;

        virtual int Add_System_Collections_IList(System::Object^ value) sealed = System::Collections::IList::Add;

        virtual bool Contains_System_Collections_IList(System::Object^ value) sealed = System::Collections::IList::Contains;

        virtual void Clear_System_Collections_IList() sealed = System::Collections::IList::Clear;

        virtual int IndexOf_System_Collections_IList(System::Object^ value) sealed = System::Collections::IList::IndexOf;

        virtual void Insert_System_Collections_IList(int index, System::Object^ value) sealed = System::Collections::IList::Insert;

        virtual void Remove_System_Collections_IList(System::Object^ value) sealed = System::Collections::IList::Remove;

        virtual void CopyTo_System_Collections_ICollection(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo;

        virtual void CopyTo_System_Collections_Generic_ICollection(cli::array<Ntreev::Windows::Forms::Grid::Row^>^ array, int arrayIndex) sealed = System::Collections::Generic::ICollection<Ntreev::Windows::Forms::Grid::Row^>::CopyTo;

        void currencyManager_ListChanged(System::Object^ sender, System::ComponentModel::ListChangedEventArgs^ e);

        void currencyManager_CurrentChanged(System::Object^ sender, System::EventArgs^ e);

        void gridControl_CurrencyManagerChanging(System::Object^ sender, CurrencyManagerChangingEventArgs^ e);

        void gridControl_CurrencyManagerChanged(System::Object^ sender, Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs^ e);

        void gridControl_FocusedRowChanged(System::Object^ sender, System::EventArgs^ e);

        void ArgumentTest(Ntreev::Windows::Forms::Grid::Row^ item);

        Ntreev::Windows::Forms::Grid::Row^ InsertCore(int index, Ntreev::Windows::Forms::Grid::Row^ item);

    private: //properties

        property System::Object^ default_System_Collections_IList[int]
        {
            virtual System::Object^ get(int index) sealed = System::Collections::IList::default::get;
            virtual void set(int /*index*/, System::Object^ /*value*/) sealed = System::Collections::IList::default::set;
        }

        property bool IsReadOnly_System_Collections_IList
        {
            virtual bool get() sealed = System::Collections::IList::IsReadOnly::get;
        }

        property bool IsFixedSize_System_Collections_IList
        {
            virtual bool get() sealed = System::Collections::IList::IsFixedSize::get;
        }

        property bool IsSynchronized_System_Collections_ICollection
        {
            virtual bool get() sealed = System::Collections::ICollection::IsSynchronized::get;
        }

        property System::Object^ SyncRoot_System_Collections_ICollection
        {
            virtual System::Object^ get() sealed = System::Collections::ICollection::SyncRoot::get;
        }

        property int Count_System_Collections_ICollection
        {
            virtual int get() sealed = System::Collections::ICollection::Count::get;
        }

        property bool IsReadOnly_System_Collections_Generic_ICollection
        {
            virtual bool get() sealed = System::Collections::Generic::ICollection<Ntreev::Windows::Forms::Grid::Row^>::IsReadOnly::get;
        }

    private: // variables

        int m_locked;
        GrDataRowList* m_pDataRowList;
        System::Windows::Forms::CurrencyManager^ m_manager;
        System::EventHandler^ m_currentChangedEventHandler;
        System::ComponentModel::ListChangedEventHandler^ m_listChangedEventHandler;
        System::Collections::Generic::Dictionary<System::Object^, Ntreev::Windows::Forms::Grid::Row^>^ m_components;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/