//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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
	/// 열의 컬렉션을 나타냅니다.
	/// </summary>
	[System::ComponentModel::EditorAttribute("Ntreev.Windows.Forms.Grid.Design.ColumnCollectionEditor, Ntreev.Windows.Forms.Grid.Design, Version=1.0.4300.26762, Culture=neutral, PublicKeyToken=7a9d7c7c4ba5dfca", System::Drawing::Design::UITypeEditor::typeid)]
	public ref class ColumnCollection : System::Collections::Generic::IEnumerable<_Column^>, System::Collections::IList, GridObject 
	{
		ref class Enumerator : System::Collections::Generic::IEnumerator<_Column^>
		{
		public:
			Enumerator(GrColumnList* pColumnList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property Column^ Current
			{
				virtual Column^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrColumnList*	m_pColumnList;
		};

		ref class ServiceProvider : System::IServiceProvider
		{
		public:
			ServiceProvider(_GridControl^ gridControl)
				: m_gridControl(gridControl)
			{

			}

			virtual object^ GetService(_Type^ serviceType) { return m_gridControl->GetInternalService(serviceType); }

		private:
			_GridControl^ m_gridControl;
		};

	public:	// methods
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
		void Add(_Column^ item);

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
		void AddRange(cli::array<_Column^>^ values);

		/// <summary>
		/// 새로운 열의 인스턴스를 생성하고 컬렉션에 추가합니다.
		/// </summary>
		/// <returns>
		/// 새로 생성된 <see cref="Column"/>의 인스턴스입니다.
		/// </returns>
		_Column^ AddNew();

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
		_Column^ AddNew(string^ name);

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
		_Column^ AddNew(string^ name, _Type^ type);

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
		void Insert(int index, _Column^ item);

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
		_Column^ InsertNew(int index);

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
		_Column^ InsertNew(int index, string^ name);

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
		_Column^ InsertNew(int index, string^ name, _Type^ type);

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
		void Remove(_Column^ item);

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
		int	IndexOf(_Column^ item);

		/// <summary>
		/// 컬렉션에서 열이 포함되어 있는지 확인합니다.
		/// </summary>
		/// <exception cref="System::ArgumentNullException">
		/// 매개변수 item의 값이 null일때
		/// </exception>
		/// <returns>
		/// 컬렉션에 포함되어있다면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		bool Contains(_Column^ item);

		/// <summary>
		/// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
		/// <returns>
		/// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
		/// </returns>
		virtual System::Collections::Generic::IEnumerator<_Column^>^ GetEnumerator()
		{
			return gcnew Enumerator(m_pColumnList);
		}

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
		property Column^ default[int]
		{
			Column^ get(int index);
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
		property Column^ default[string^]
		{
			Column^ get(string^ columnName);
		}

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator_IEnumerable() sealed = System::Collections::IEnumerable::GetEnumerator
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
			Add((_Column^)value);
			return count;
		}

		virtual bool Contains_IList(object^ value) sealed = System::Collections::IList::Contains
		{
			return this->Contains((_Column^)value);
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
			return this->IndexOf((_Column^)value);
		}

		virtual void Insert_IList(int index, object^ value) sealed = System::Collections::IList::Insert
		{
			this->Insert(index, (_Column^)value);
		}

		virtual void Remove_IList(object^ value) sealed = System::Collections::IList::Remove
		{
			this->Remove((_Column^)value);
		}
		
		virtual void CopyTo_ICollection(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(_Column^ item in this)
            {
                array->SetValue(item, index++);
            }
        }

	private: // properties
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

		

	internal: // methods
		ColumnCollection(_GridControl^ gridControl);

		void DeleteAll();

		void SetItemsByDesigner(cli::array<object^>^ values);
		
	internal: // property
		property Column^ default[GrColumn*] 
		{
			Column^ get(GrColumn*); 
		}

		static _Column^ CreateColumnInstance(System::IServiceProvider^ serviceProvider, _Type^ dataType);

		static _Column^ CreateColumnInstanceCore(System::IServiceProvider^ serviceProvider, _Type^ columnType);

	private: // methods

		_Column^ CreateColumnInstance(_PropertyDescriptor^ propertyDescriptor);

		_Column^ CreateColumnInstance(_Type^ dataType);

		string^	NewColumnName();

		_Column^ Bind(_PropertyDescriptor^ propertyDescriptor);

		void Unbind(_Column^ column);

		void currencyManager_ListChanged(object^ sender, System::ComponentModel::ListChangedEventArgs^ e);

		void gridControl_CurrencyManagerChanging(object^ sender, CurrencyManagerChangingEventArgs^ e);

		void gridControl_CurrencyManagerChanged(object^ sender, CurrencyManagerChangedEventArgs^ e);

		void gridControl_Clearing(object^ sender, ClearEventArgs^ e);

		void gridControl_Cleared(object^ sender, ClearEventArgs^ e);

	private:
		GrColumnList* m_pColumnList;
		System::Windows::Forms::CurrencyManager^ m_manager;
		System::ComponentModel::ListChangedEventHandler^ m_listChangedEventHandler;
		System::Collections::ArrayList^ m_tempBindableColumns;
	};

	/// <summary>
	/// 선택된 열의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class SelectedColumnCollection : System::Collections::Generic::IEnumerable<_Column^>, System::Collections::ICollection, GridObject
	{
	private:
		ref class Enumerator : System::Collections::Generic::IEnumerator<_Column^>
		{
		public:
			Enumerator(const GrSelectedColumns* selectedColumns);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property Column^ Current
			{
				virtual Column^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			const GrSelectedColumns* m_selectedColumns;
		};

	public: // methods
		void Add(_Column^ item);	
		
		bool Remove(_Column^ item);
		
		void Clear();

		/// <summary>
		/// 항목의 갯수를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 갯수를 나타내는 <see cref="System::Int32"/>의 정수값입니다.
		/// </returns>
		property int Count
		{
			virtual int get() { return (int)m_selectedColumns->size(); }
		}

		/// <summary>
		/// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
		/// <returns>
		/// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
		/// </returns>
		virtual System::Collections::Generic::IEnumerator<_Column^>^ GetEnumerator()
		{
			return gcnew Enumerator(m_selectedColumns);
		}

	internal: // methods
		SelectedColumnCollection(_GridControl^ gridControl, const GrSelectedColumns* selectedColumns);

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(_Column^ item in this)
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
		const GrSelectedColumns* m_selectedColumns;
	};

	/// <summary>
	/// 표시되는 열의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class VisibleColumnCollection : System::Collections::Generic::IEnumerable<_Column^>, System::Collections::ICollection, GridObject
	{
	private:
		ref class Enumerator : System::Collections::Generic::IEnumerator<_Column^>
		{
		public:
			Enumerator(GrColumnList* pColumnList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property _Column^ Current
			{
				virtual _Column^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrColumnList*	m_pColumnList;
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
		virtual System::Collections::Generic::IEnumerator<_Column^>^ GetEnumerator() { return gcnew Enumerator(m_pColumnList); }

	public: // properties
		/// <summary>
		/// 지정한 인덱스에 있는 열을 가져옵니다.
		/// </summary>
		/// <param name="index">
		/// 가져올 열의 인덱스(0부터 시작)입니다.
		/// </param>
		/// <returns>
		/// 지정한 인덱스의 <see cref="Column"/>입니다.
		/// </returns>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
		/// </exception>
		property Column^ default[int]
		{
			Column^ get(int index);
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
		VisibleColumnCollection(_GridControl^ gridControl);

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(_Column^ item in this)
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
		GrColumnList*		m_pColumnList;
	};

	/// <summary>
	/// 화면에 표시되는 열의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class DisplayableColumnCollection : System::Collections::Generic::IEnumerable<_Column^>, System::Collections::ICollection, GridObject
	{
	private:
		ref class Enumerator : System::Collections::Generic::IEnumerator<_Column^>
		{
		public:
			Enumerator(GrColumnList* pColumnList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();
			property _Column^ Current { virtual _Column^ get(); }

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrColumnList*	m_pColumnList;
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
		virtual System::Collections::Generic::IEnumerator<_Column^>^ GetEnumerator() { return gcnew Enumerator(m_pColumnList); }

	public: // properties
		/// <summary>
		/// 지정한 인덱스에 있는 열을 가져옵니다.
		/// </summary>
		/// <param name="index">
		/// 가져올 열의 인덱스(0부터 시작)입니다.
		/// </param>
		/// <returns>
		/// 지정한 인덱스의 <see cref="Column"/>입니다.
		/// </returns>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
		/// </exception>
		property Column^ default[int]
		{
			Column^ get(int index);
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
		DisplayableColumnCollection(_GridControl^ gridControl);

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(_Column^ item in this)
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
		GrColumnList*		m_pColumnList;
	};

	/// <summary>
	/// 스크롤 되지 않는 열의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class FrozenColumnCollection : System::Collections::Generic::IEnumerable<_Column^>, System::Collections::ICollection, GridObject
	{
	private:
		ref class Enumerator : System::Collections::Generic::IEnumerator<_Column^>
		{
		public:
			Enumerator(GrColumnList* pColumnList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property _Column^ Current
			{
				virtual _Column^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrColumnList*	m_pColumnList;
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
		virtual System::Collections::Generic::IEnumerator<_Column^>^ GetEnumerator() { return gcnew Enumerator(m_pColumnList); }

	public: // properties
		/// <summary>
		/// 지정한 인덱스에 있는 열을 가져옵니다.
		/// </summary>
		/// <param name="index">
		/// 가져올 열의 인덱스(0부터 시작)입니다.
		/// </param>
		/// <returns>
		/// 지정한 인덱스의 <see cref="Column"/>입니다.
		/// </returns>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
		/// </exception>
		property Column^ default[int]
		{
			Column^ get(int index);
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
		FrozenColumnCollection(_GridControl^ gridControl);

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(_Column^ item in this)
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
		GrColumnList*		m_pColumnList;
	};
	/// <summary>
	/// 스크롤 되는 열의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class UnfrozenColumnCollection : System::Collections::Generic::IEnumerable<_Column^>, System::Collections::ICollection, GridObject
	{
	private:
		ref class Enumerator : System::Collections::Generic::IEnumerator<_Column^>
		{
		public:
			Enumerator(GrColumnList* pColumnList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property _Column^ Current
			{
				virtual _Column^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrColumnList*	m_pColumnList;
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
		virtual System::Collections::Generic::IEnumerator<_Column^>^ GetEnumerator() { return gcnew Enumerator(m_pColumnList); }

	public: // properties
		/// <summary>
		/// 지정한 인덱스에 있는 열을 가져옵니다.
		/// </summary>
		/// <param name="index">
		/// 가져올 열의 인덱스(0부터 시작)입니다.
		/// </param>
		/// <returns>
		/// 지정한 인덱스의 <see cref="Column"/>입니다.
		/// </returns>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// index가 0보다 작거나, <see cref="Count"/>보다 클 경우
		/// </exception>
		property Column^ default[int]
		{
			Column^ get(int index);
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
		UnfrozenColumnCollection(_GridControl^ gridControl);

	private: // methods
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(_Column^ item in this)
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
		GrColumnList*		m_pColumnList;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/