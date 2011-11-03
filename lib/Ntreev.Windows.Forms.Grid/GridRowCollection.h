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
#include "GridBase.h"
#include "GridCollection.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 행의 컬렉션을 나타냅니다.
	/// </summary>
	[System::ComponentModel::Editor("Ntreev.Windows.Forms.Grid.Design.RowCollectionEditor, Ntreev.Windows.Forms.Grid.Design, Version=1.1.4324.22060, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", System::Drawing::Design::UITypeEditor::typeid)]
	[System::ComponentModel::Design::Serialization::DesignerSerializer("Ntreev.Windows.Forms.Grid.Design.RowCollectionCodeDomSerializer, Ntreev.Windows.Forms.Grid.Design, Version=1.1.4324.22060, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", "System.ComponentModel.Design.Serialization.CodeDomSerializer")]
	public ref class RowCollection : System::Collections::Generic::IList<Row^>, System::Collections::IList, GridObject
	{
		ref class Enumerator : System::Collections::Generic::IEnumerator<Row^>
		{
		public:
			Enumerator(GrDataRowList* pDataRowList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property Row^ Current
			{
				virtual Row^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrDataRowList*	m_pDataRowList;
		};

		ref class ManagerEventDetach
		{
		public:
			ManagerEventDetach(RowCollection^ rowCollection)
				: m_rowCollection(rowCollection)
			{
				m_rowCollection->m_manager->ListChanged -= m_rowCollection->m_listChangedEventHandler;
				m_rowCollection->m_manager->CurrentChanged -= m_rowCollection->m_currentChangedEventHandler;
				m_rowCollection->m_locked++;
			}
			~ManagerEventDetach()
			{
				m_rowCollection->m_locked--;
				m_rowCollection->m_manager->ListChanged += m_rowCollection->m_listChangedEventHandler;
				m_rowCollection->m_manager->CurrentChanged += m_rowCollection->m_currentChangedEventHandler;
			}

		private:
			RowCollection^ m_rowCollection;
		};
	private: // typedefs
			
	public: // methods
		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual	string^ ToString() override
		{
			return this->Count.ToString();
		}

		/// <summary>
		/// 새로운 행의 인스턴스를 생성하고 컬렉션에 추가합니다.
		/// </summary>
		/// <returns>
		/// 새로 생성된 <see cref="Row"/>의 인스턴스입니다.
		/// </returns>
		Row^ AddNew();

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
		cli::array<Row^>^ AddNew(int count);

		/// <summary>
		/// <see cref="InsertionRow"/>의 값을 이용하여 새로운 행의 인스턴스를 생성하고 컬렉션에 추가합니다.
		/// </summary>
		/// <returns>
		/// 새로 생성된 <see cref="Row"/>의 인스턴스입니다.
		/// </returns>
		/// <exception cref="System::NotSupportedException">
		/// 새로운 행을 생성할 수 없는 경우
		/// </exception>
		Row^ AddNewFromInsertion();

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
		virtual void Add(Row^ item);

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
		virtual bool Remove(Row^ item);

		/// <summary>
		/// 컬렉션에서 행이 포함되어 있는지 확인합니다.
		/// </summary>
		/// <exception cref="System::ArgumentNullException">
		/// 매개변수 item의 값이 null일때
		/// </exception>
		/// <returns>
		/// 컬렉션에 포함되어있다면 true, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		virtual bool Contains(Row^ item);

		/// <summary>
		/// 컬렉션에서 행의 위치를 확인합니다.
		/// </summary>
		/// <exception cref="System::ArgumentNullException">
		/// 매개변수 item의 값이 null일때
		/// </exception>
		/// <returns>
		/// 컬렉션에 포함되어 있지 않다면 -1을 반환합니다.
		/// </returns>
		virtual int IndexOf(Row^ item);

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
		virtual void Insert(int index, Row^ item);

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
		virtual System::Collections::Generic::IEnumerator<Row^>^ GetEnumerator()
		{
			return gcnew Enumerator(m_pDataRowList);
		}

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
		Row^ GetAt(int index)
		{
			return this[index];
		}

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
		//[System::ComponentModel::DesignerSerializationVisibility(System::ComponentModel::DesignerSerializationVisibility::Content)]
		property Row^ default[int]
		{
			virtual Row^ get(int index);
		private:
			virtual void set(int /*index*/, Row^ /*item*/) sealed = System::Collections::Generic::IList<Row^>::default::set {}
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
		property _InsertionRow^ InsertionRow
		{
			_InsertionRow^ get();
		}

	internal: // methods
		RowCollection(_GridControl^ gridControl);

		void Bind(object^ component, int componentIndex);

		void Unbind(int componentIndex);

		void SetItemsByDesigner(cli::array<object^>^ values);

	internal: // properties
		property Row^ default[GrDataRow*]
		{
			Row^ get(GrDataRow* pDataRow);
		}

		property Row^ default[object^]
		{
			Row^ get(object^ component);
		}

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

		virtual void RemoveAt_IList(int index) sealed = System::Collections::IList::RemoveAt
		{
			this->RemoveAt(index);
		}

		virtual int Add_IList(object^ value) sealed = System::Collections::IList::Add
		{
			int count = this->Count;
			Add((Row^)value);
			return count;
		}

		virtual bool Contains_IList(object^ value) sealed = System::Collections::IList::Contains
		{
			return this->Contains((Row^)value);
		}

		virtual void Clear_IList() sealed = System::Collections::IList::Clear
		{
			for(int i=this->Count-1 ; i>=0 ; i--)
			{
				RemoveAt(i);
			}
		}

		virtual int IndexOf_IList(object^ value) sealed = System::Collections::IList::IndexOf
		{
			return this->IndexOf((Row^)value);
		}

		virtual void Insert_IList(int index, object^ value) sealed = System::Collections::IList::Insert
		{
			this->Insert(index, (Row^)value);
		}

		virtual void Remove_IList(object^ value) sealed = System::Collections::IList::Remove
		{
			this->Remove((Row^)value);
		}

		virtual void CopyTo_ICollection(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(Row^ item in this)
            {
                array->SetValue(item, index++);
            }
        }

		/// <summary>
		/// 특정 <see cref="System::Array"/> 인덱스부터 시작하여 행의 목록을 <see cref="System::Array"/>에 복사합니다.
        /// </summary>
		/// <param name="array">
		/// 행의 목록을 복사할 대상인 1차원 <see cref="System::Array"/>입니다. <see cref="System::Array"/>에는 0부터 시작하는 인덱스가 있어야 합니다.
		/// </param>
		/// <param name="arrayIndex">
		/// array에서 복사가 시작되는 인덱스(0부터 시작)입니다.
		/// </param>
		/// <exception cref="System::ArgumentNullException">
		/// array가 null인 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// index가 0보다 작은 경우
		/// </exception>
		/// <exception cref="System::ArgumentException">
		/// array가 다차원 배열인 경우- 또는 - index가 array의 길이보다 크거나 같은 경우- 또는 - 행의 목록의 갯수가
		/// index에서 대상 array 끝까지 사용 가능한 공간보다 큰 경우
		/// </exception>
		virtual void CopyTo_Generic_ICollection(cli::array<Row^>^ array, int arrayIndex) sealed = System::Collections::Generic::ICollection<Row^>::CopyTo
		{
            for each(Row^ item in this)
            {
                array->SetValue(item, arrayIndex++);
            }
        }

		void currencyManager_ListChanged(object^ sender, System::ComponentModel::ListChangedEventArgs^ e);

		void currencyManager_CurrentChanged(object^ sender, System::EventArgs^ e);

		void gridControl_CurrencyManagerChanging(object^ sender, CurrencyManagerChangingEventArgs^ e);

		void gridControl_CurrencyManagerChanged(object^ sender, CurrencyManagerChangedEventArgs^ e);

		void ArgumentTest(Row^ item);

	private: //properties
		property object^ default_IList[int]
		{
			virtual object^ get(int index) sealed = System::Collections::IList::default::get
			{
				return this[index];
			}

			virtual void set(int /*index*/, object^ /*value*/) sealed = System::Collections::IList::default::set
			{

			}
		}

		property bool IsReadOnly_IList
		{
			virtual bool get() sealed = System::Collections::IList::IsReadOnly::get { return false; }
		}

		property bool IsFixedSize_IList
		{
			virtual bool get() sealed = System::Collections::IList::IsFixedSize::get { return false; }
		}


		property bool IsSynchronized_ICollection
        {
			virtual bool get() sealed = System::Collections::ICollection::IsSynchronized::get { return true; }
        }

        property object^ SyncRoot_ICollection
        {
			virtual object^ get() sealed = System::Collections::ICollection::SyncRoot::get { return this; }
        }

        property int Count_ICollection
        {
			virtual int get() sealed = System::Collections::ICollection::Count::get { return this->Count; }
        }

		property bool IsReadOnly
		{
			virtual bool get() sealed = System::Collections::Generic::ICollection<Row^>::IsReadOnly::get { return false; }
		}

	private: // variables
		GrDataRowList*		m_pDataRowList;

		System::Windows::Forms::CurrencyManager^ m_manager;
		int m_locked;
		
		System::EventHandler^ m_currentChangedEventHandler;
		System::ComponentModel::ListChangedEventHandler^ m_listChangedEventHandler;
	};

	/// <summary>
	/// 선택된 행의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class SelectedRowCollection : System::Collections::Generic::IEnumerable<Row^>, System::Collections::ICollection, GridObject
	{
		ref class Enumerator : System::Collections::Generic::IEnumerator<Row^>
		{
		public:
			Enumerator(const GrSelectedRows* selectedRows);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property Row^ Current
			{
				virtual Row^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			const GrSelectedRows* m_selectedRows;
		};

	public: // methods
		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual	string^ ToString() override
		{
			return this->Count.ToString();
		}

		/// <summary>
		/// Row::IsSelected의 값을 true로 설정하고 목록에 추가합니다.
		/// </summary>
		/// <remarks> 
		/// Row::IsSelected 의 값이 이미 true였다면 아무런 동작이 일어나지 않습니다. 
		/// 이 메서드는 Row::IsSelected = true 와 동일한 작업을 수행합니다.
		/// </remarks>
		virtual void Add(Row^ row);	

		/// <summary>
		/// Row::IsSelected의 값을 true로 설정하고 목록에 추가하고 focus행에 해당하는 셀의 포커스를 설정합니다.
		/// </summary>
		/// <remarks>
		/// </remarks>
		/// <param name="focusColumn">
		/// 포커스를 설정할 Column입니다.
		/// null 설정시 가장 최근값으로 포커스를 설정합니다.
		/// </param>
		void Add(Row^ row, Column^ focusColumn);

		/// <summary>
		/// Row::IsSelected의 값을 false로 설정하고 목록에 제거합니다.
		/// </summary>
		/// <remarks> 
		/// Row::IsSelected 의 값이 이미 false였다면 아무런 동작이 일어나지 않습니다. 
		/// 이 메서드는 Row::IsSelected = false 와 동일한 작업을 수행합니다.
		/// </remarks>
		virtual bool Remove(Row^ row);

		/// <summary>
		/// 모든 Row::IsSelected 의 값을 false로 설정하고 제거합니다.
		/// </summary>
		/// <remarks> 
		/// 이 메서드는 GridControl::ClearSelection() 와 동일한 작업을 수행합니다.
		/// </remarks>
		virtual void Clear();

		/// <summary>
		/// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
		/// <returns>
		/// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
		/// </returns>
		virtual System::Collections::Generic::IEnumerator<Row^>^ GetEnumerator()
		{
			return gcnew Enumerator(m_selectedRows);
		}

	public: // properties
		/// <summary>
		/// 항목의 갯수를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 갯수를 나타내는 <see cref="System::Int32"/>의 정수값입니다.
		/// </returns>
		property int Count
		{
			int get() { return (int)m_selectedRows->size(); }
		}

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
		property Row^ default[int]
		{
			Row^ get(int index);
		}

	internal: // methods
		SelectedRowCollection(_GridControl^ gridControl, const GrSelectedRows* selectedRows);

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(Row^ item in this)
            {
                array->SetValue(item, index++);
            }
        }

        property bool IsSynchronized
        {
			virtual bool get() sealed = System::Collections::ICollection::IsSynchronized::get { return true; }
        }

        property object^ SyncRoot
        {
			virtual object^ get() sealed = System::Collections::ICollection::SyncRoot::get { return this; }
        }

        property int Count2
        {
			virtual int get() sealed = System::Collections::ICollection::Count::get { return this->Count; }
        }
#pragma endregion

	private:
		const GrSelectedRows* m_selectedRows;
	};

	/// <summary>
	/// 표시되는 행의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class VisibleRowCollection : System::Collections::Generic::IEnumerable<RowBase^>, System::Collections::ICollection, GridObject
	{
	private:
		ref class Enumerator : System::Collections::Generic::IEnumerator<RowBase^>
		{
		public:
			Enumerator(GrDataRowList* pDataRowList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property RowBase^ Current
			{
				virtual RowBase^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrDataRowList*	m_pDataRowList;
		};

	public: // methods
		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual	string^ ToString() override
		{
			return this->Count.ToString();
		}

		/// <summary>
		/// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
		/// <returns>
		/// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
		/// </returns>
		virtual System::Collections::Generic::IEnumerator<RowBase^>^ GetEnumerator() { return gcnew Enumerator(m_pDataRowList); }

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
		property RowBase^ default[int]
		{
			RowBase^ get(int index);
		}

		/// <summary>
		/// 항목의 갯수를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 갯수를 나타내는 <see cref="System::Int32"/>의 정수값입니다.
		/// </returns>
		property int Count
		{
			int get();
		}

	internal: // methods
		VisibleRowCollection(_GridControl^ gridControl);

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(RowBase^ item in this)
            {
                array->SetValue(item, index++);
            }
        }

        property bool IsSynchronized
        {
			virtual bool get() sealed = System::Collections::ICollection::IsSynchronized::get { return true; }
        }

        property object^ SyncRoot
        {
			virtual object^ get() sealed = System::Collections::ICollection::SyncRoot::get { return this; }
        }

        property int Count2
        {
			virtual int get() sealed = System::Collections::ICollection::Count::get { return this->Count; }
        }
#pragma endregion

	private: // variables
		GrDataRowList*								m_pDataRowList;
	};

	/// <summary>
	/// 화면에 표시되는 행의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class DisplayableRowCollection : System::Collections::Generic::IEnumerable<RowBase^>, System::Collections::ICollection, GridObject
	{
	private:
		ref class Enumerator : System::Collections::Generic::IEnumerator<RowBase^>
		{
		public:
			Enumerator(GrDataRowList* pDataRowList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property RowBase^ Current
			{
				virtual RowBase^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrDataRowList*	m_pDataRowList;
		};

	public: // methods
		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual	string^ ToString() override
		{
			return this->Count.ToString();
		}

		/// <summary>
		/// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
		/// <returns>
		/// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
		/// </returns>
		virtual System::Collections::Generic::IEnumerator<RowBase^>^ GetEnumerator() { return gcnew Enumerator(m_pDataRowList); }

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
		property RowBase^ default[int]
		{
			RowBase^ get(int index);
		}

		/// <summary>
		/// 항목의 갯수를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 갯수를 나타내는 <see cref="System::Int32"/>의 정수값입니다.
		/// </returns>
		property int Count
		{
			int get();
		}

	internal: // methods
		DisplayableRowCollection(_GridControl^ gridControl);

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(RowBase^ item in this)
            {
                array->SetValue(item, index++);
            }
        }

        property bool IsSynchronized
        {
			virtual bool get() sealed = System::Collections::ICollection::IsSynchronized::get { return true; }
        }

        property object^ SyncRoot
        {
			virtual object^ get() sealed = System::Collections::ICollection::SyncRoot::get { return this; }
        }

        property int Count2
        {
			virtual int get() sealed = System::Collections::ICollection::Count::get { return this->Count; }
        }
#pragma endregion

	private: // variables
		GrDataRowList*								m_pDataRowList;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/