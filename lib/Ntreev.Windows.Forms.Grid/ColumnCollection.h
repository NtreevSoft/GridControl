//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4510.20986
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
#include "Column.h"
#include "Events.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 열의 컬렉션을 나타냅니다.
    /// </summary>
    [System::ComponentModel::EditorAttribute("Ntreev.Windows.Forms.Grid.Design.ColumnCollectionEditor, Ntreev.Windows.Forms.Grid.Design, Version=2.0.4510.20986, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", System::Drawing::Design::UITypeEditor::typeid)]
    public ref class ColumnCollection
        : Ntreev::Windows::Forms::Grid::GridObject
        , System::Collections::Generic::IEnumerable<Ntreev::Windows::Forms::Grid::Column^>
        , System::Collections::IList
    {
    private: // classes

        ref class Enumerator : System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>
        {
        public:
            Enumerator(GrColumnList* pColumnList);
            ~Enumerator();
            virtual bool MoveNext();
            virtual void Reset();

            property Ntreev::Windows::Forms::Grid::Column^ Current
            {
                virtual Ntreev::Windows::Forms::Grid::Column^ get();
            }

        private:
            property System::Object^ Current_System_Collections_IEnumerator
            {
                virtual System::Object^ get() sealed = System::Collections::IEnumerator::Current::get;
            }

        private:
            unsigned int m_index;
            GrColumnList* m_pColumnList;
        };

        ref class ServiceProvider : System::IServiceProvider
        {
        public:
            ServiceProvider(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);
            virtual System::Object^ GetService(System::Type^ serviceType);

        private:
            Ntreev::Windows::Forms::Grid::GridControl^ m_gridControl;
        };

    public: // methods

        /// <summary>
        /// 컬렉션에 새로운 열을 추가합니다.
        /// </summary>
        /// <remarks>
        /// 일반적으로 데이터 소스와 바인딩 되지 않는 열을 추가 하기위해 사용합니다.
        /// 사용자는 <see cref="Column"/>형태의 인스턴스를 새로 생성해 추가 하거나, <see cref="Remove"/>등의 의해 컬렉션에서 제거된 열을 다시 추가할 수 있습니다.
        /// </remarks>
        /// <param name="item">
        /// 새롭게 추가할 열의 <see cref="Column"/>입니다.
        /// </param>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 추가할 열이 이미 존재하거나, 같은 이름을 가진 열이 컬렉션에 이미 존재하는 경우
        /// </exception>
        void Add(Ntreev::Windows::Forms::Grid::Column^ item);

        /// <summary>
        /// 컬렉션에 여러개의 새로운 열을 추가합니다.
        /// </summary>
        /// <param name="values">
        /// 새롭게 추가할 <see cref="Column"/> 개체의 배열입니다.
        /// </param>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 values의 값이 null일때
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 추가할 열이 이미 존재하거나, 같은 이름을 가진 열이 컬렉션에 이미 존재하는 경우
        /// </exception>
        void AddRange(cli::array<Ntreev::Windows::Forms::Grid::Column^>^ values);

        /// <summary>
        /// 새로운 열의 인스턴스를 생성하고 컬렉션에 추가합니다.
        /// </summary>
        /// <returns>
        /// 새로 생성된 <see cref="Column"/>의 인스턴스입니다.
        /// </returns>
        Ntreev::Windows::Forms::Grid::Column^ Add();

        /// <summary>
        /// 새로운 열의 인스턴스를 생성하고 컬렉션에 추가합니다.
        /// </summary>
        /// <param name="name">
        /// 생성될 열의 이름입니다. null값이거나 빈문자열일 경우 임의의 이름이 지정됩니다.
        /// </param>
        /// <exception cref="System::ArgumentException">
        /// 같은 이름을 가진 열이 컬렉션에 이미 존재하는 경우
        /// </exception>
        /// <returns>
        /// 새로 생성된 <see cref="Column"/>의 인스턴스입니다.
        /// </returns>
        Ntreev::Windows::Forms::Grid::Column^ Add(System::String^ name);

        /// <summary>
        /// 새로운 열의 인스턴스를 생성하고 컬렉션에 추가합니다.
        /// </summary>
        /// <param name="name">
        /// 생성될 열의 이름입니다. null값이거나 빈문자열일 경우 임의의 이름이 지정됩니다.
        /// </param>
        /// <param name="type">
        /// 열이 관리할 데이터의 형식입니다. null값일 경우 string형식으로 대체 됩니다.
        /// </param>
        /// <exception cref="System::ArgumentException">
        /// 같은 이름을 가진 열이 컬렉션에 이미 존재하는 경우
        /// </exception>
        /// <returns>
        /// 새로 생성된 <see cref="Column"/>의 인스턴스입니다.
        /// </returns>
        Ntreev::Windows::Forms::Grid::Column^ Add(System::String^ name, System::Type^ type);

        /// <summary>
        /// 컬렉션에 새로운 열을 삽입합니다.
        /// </summary>
        /// <param name="index">
        /// 삽입할 위치를 나타냅니다.
        /// </param>
        /// <remarks>
        /// 일반적으로 데이터 소스와 바인딩 되지 않는 열을 삽입 하기위해 사용합니다.
        /// 사용자는 <see cref="Column"/>형태의 인스턴스를 새로 생성해 삽입 하거나, <see cref="Remove"/>등의 의해 컬렉션에서 제거된 열을 다시 삽입할 수 있습니다.
        /// </remarks>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 추가할 열이 이미 존재하거나, 같은 이름을 가진 열이 컬렉션에 이미 존재하는 경우
        /// </exception>
        void Insert(int index, Ntreev::Windows::Forms::Grid::Column^ item);

        /// <summary>
        /// 새로운 열의 인스턴스를 생성하고 컬렉션에 삽입합니다.
        /// </summary>
        /// <param name="index">
        /// 삽입할 위치를 나타냅니다.
        /// </param>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        /// <returns>
        /// 새로 생성된 <see cref="Column"/>의 인스턴스입니다.
        /// </returns>
        Ntreev::Windows::Forms::Grid::Column^ Insert(int index);

        /// <summary>
        /// 새로운 열의 인스턴스를 생성하고 컬렉션에 삽입합니다.
        /// </summary>
        /// <param name="index">
        /// 삽입할 위치를 나타냅니다.
        /// </param>
        /// <param name="name">
        /// 생성될 열의 이름입니다. null값이거나 빈문자열일 경우 임의의 이름이 지정됩니다.
        /// </param>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 같은 이름을 가진 열이 컬렉션에 이미 존재하는 경우
        /// </exception>
        /// <returns>
        /// 새로 생성된 <see cref="Column"/>의 인스턴스입니다.
        /// </returns>
        Ntreev::Windows::Forms::Grid::Column^ Insert(int index, System::String^ name);

        /// <summary>
        /// 새로운 열의 인스턴스를 생성하고 컬렉션에 삽입합니다.
        /// </summary>
        /// <param name="index">
        /// 삽입할 위치를 나타냅니다.
        /// </param>
        /// <param name="name">
        /// 생성될 열의 이름입니다. null값이거나 빈문자열일 경우 임의의 이름이 지정됩니다.
        /// </param>
        /// <param name="type">
        /// 열이 관리할 데이터의 형식입니다. null값일 경우 string형식으로 대체 됩니다.
        /// </param>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 같은 이름을 가진 열이 컬렉션에 이미 존재하는 경우
        /// </exception>
        /// <returns>
        /// 새로 생성된 <see cref="Column"/>의 인스턴스입니다.
        /// </returns>
        Ntreev::Windows::Forms::Grid::Column^ Insert(int index, System::String^ name, System::Type^ type);

        /// <summary>
        /// 컬렉션에서 열을 제거합니다.
        /// </summary>
        /// <param name="item">
        /// 제거할 열의 <see cref="Column"/>입니다.
        /// </param>
        /// <remarks>
        /// 데이터 소스와 바인딩 된 열을 제거할 수 없습니다.
        /// </remarks>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 이미 제거가 된 경우 또는 제거할 수 없는 열일 경우
        /// </exception>
        void Remove(Ntreev::Windows::Forms::Grid::Column^ item);

        /// <summary>
        /// 컬렉션에서 지정된 위치의 열을 제거합니다.
        /// </summary>
        /// <param name="index">
        /// 제거할 열의 위치를 나타냅니다.
        /// </param>
        /// <remarks>
        /// 데이터 소스와 바인딩 된 열을 제거할 수 없습니다.
        /// </remarks>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        /// <exception cref="System::ArgumentException">
        /// 제거할 수 없는 열일 경우
        /// </exception>
        void RemoveAt(int index);

        /// <summary>
        /// 컬렉션에서 열의 위치를 확인합니다.
        /// </summary>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <returns>
        /// 컬렉션에 포함되어 있지 않다면 -1을 반환합니다.
        /// </returns>
        int IndexOf(Ntreev::Windows::Forms::Grid::Column^ item);

        /// <summary>
        /// 컬렉션에서 열이 포함되어 있는지 확인합니다.
        /// </summary>
        /// <exception cref="System::ArgumentNullException">
        /// 매개변수 item의 값이 null일때
        /// </exception>
        /// <returns>
        /// 컬렉션에 포함되어있다면 true를, 그렇지 않다면 false를 반환합니다.
        /// </returns>
        bool Contains(Ntreev::Windows::Forms::Grid::Column^ item);

        /// <summary>
        /// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
        /// <returns>
        /// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ GetEnumerator();

    public: // properties

        /// <summary>
        /// 지정한 인덱스에 있는 열을 가져옵니다.
        /// </summary>
        /// <param name="index">
        /// 가져올 열의의 인덱스(0부터 시작)입니다.
        /// </param>
        /// <returns>
        /// 지정한 인덱스의 <see cref="Column"/>입니다.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
        /// </exception>
        property Ntreev::Windows::Forms::Grid::Column^ default[int]
        {
            Ntreev::Windows::Forms::Grid::Column^ get(int index);
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
        }

        /// <summary>
        /// 컬렉션에서 이름으로 열을 찾습니다.
        /// </summary>
        /// <param name="columnName">
        /// 찾을 항목의 이름입니다.
        /// </param>
        /// <returns>
        /// 같은 이름의 열이 없을 경우 null을 반환합니다.
        /// </returns>
        property Ntreev::Windows::Forms::Grid::Column^ default[System::String^]
        {
            Ntreev::Windows::Forms::Grid::Column^ get(System::String^ columnName);
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

    private: // properties

        property System::Object^ default_System_Collections_IList[int]
        {
            virtual System::Object^ get(int index) sealed = System::Collections::IList::default::get;
            virtual void set(int index, System::Object^ value) sealed = System::Collections::IList::default::set;
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

    internal: // methods

        ColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

        void DeleteAll();

        void SetItemsByDesigner(cli::array<System::Object^>^ values);

    internal: // property

        property Ntreev::Windows::Forms::Grid::Column^ default[GrColumn*] 
        {
            Ntreev::Windows::Forms::Grid::Column^ get(GrColumn*); 
        }

        static Ntreev::Windows::Forms::Grid::Column^ CreateColumnInstance(System::IServiceProvider^ serviceProvider);

        static Ntreev::Windows::Forms::Grid::Column^ CreateColumnInstance(System::IServiceProvider^ serviceProvider, System::Type^ columnType);

    private: // methods

        Ntreev::Windows::Forms::Grid::Column^ CreateColumnInstance();

        System::String^ NewColumnName();

        Ntreev::Windows::Forms::Grid::Column^ Bind(System::ComponentModel::PropertyDescriptor^ propertyDescriptor);

        void Unbind(Ntreev::Windows::Forms::Grid::Column^ column);

        void currencyManager_ListChanged(System::Object^ sender, System::ComponentModel::ListChangedEventArgs^ e);

        void gridControl_CurrencyManagerChanging(System::Object^ sender, Ntreev::Windows::Forms::Grid::CurrencyManagerChangingEventArgs^ e);

        void gridControl_CurrencyManagerChanged(System::Object^ sender, Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs^ e);

        void gridControl_Clearing(System::Object^ sender, Ntreev::Windows::Forms::Grid::ClearEventArgs^ e);

        void gridControl_Cleared(System::Object^ sender, Ntreev::Windows::Forms::Grid::ClearEventArgs^ e);

    private: // variables

        GrColumnList* m_pColumnList;
        System::Windows::Forms::CurrencyManager^ m_manager;
        System::ComponentModel::ListChangedEventHandler^ m_listChangedEventHandler;
        System::Collections::ArrayList^ m_tempBindableColumns;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/