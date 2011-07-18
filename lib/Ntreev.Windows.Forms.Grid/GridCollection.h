#pragma once
#include "GridBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 셀의 컬렉션을 나타냅니다.
	/// </summary>
	public ref class CellCollection : System::Collections::Generic::IEnumerable<Cell^>, System::Collections::ICollection
	{
		ref class Enumerator : System::Collections::Generic::IEnumerator<Cell^>
		{
		public:
			Enumerator(GrDataRow* pDataRow, GrColumnList* pColumnList);
			~Enumerator();
			virtual bool MoveNext();
			virtual void Reset();

			property Cell^ Current
			{
				virtual Cell^ get();
			}

		private:
			property object^ Current2
			{
				virtual object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
			}

		private:
			uint			m_index;
			GrDataRow*		m_pDataRow;
			GrColumnList*	m_pColumnList;
		};

	public: // methods
		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual	string^ ToString() override;

	public:
		/// <summary>
		/// 행의 이름으로 셀을 가져옵니다.
		/// </summary>
		/// <param name="columnName">
		/// 행의 이름입니다.
		/// </param>
		/// <returns>
		/// 찾지 못하는 경우 null을 반환합니다.
		/// </returns>
		property Cell^ default[string^]
		{
			Cell^ get(string^ columnName);
		}

		/// <summary>
		/// 행의 인스턴스로 셀을 가져옵니다.
		/// </summary>
		/// <param name="column">
		/// 찾을 <see cref="Column"/>의 인스턴스입니다.
		/// </param>
		/// <returns>
		/// 찾지 못하는 경우 null을 반환합니다.
		/// </returns>
		/// <exception cref="System::NullReferenceException">
		/// 매개변수가 null일 경우
		/// </exception>
		property Cell^ default[_Column^]
		{
			Cell^ get(_Column^ column);
		}

		/// <summary>
		/// 지정한 인덱스에 있는 셀을 가져옵니다.
		/// </summary>
		/// <param name="index">
		/// 가져올 셀의 인덱스(0부터 시작)입니다.
		/// </param>
		/// <returns>
		/// 지정한 인덱스의 셀입니다.
		/// </returns>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 인덱스가 0보다 작거나, <see cref="Count"/>보다 클 경우
		/// </exception>
		property Cell^ default[int]
		{
			Cell^ get(int index);
		}

		/// <summary>
		/// 셀의 갯수를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 갯수를 나타내는 <see cref="System::Int32"/>의 정수값입니다.
		/// </returns>
		property int Count
		{
			int get();
		}

		/// <summary>
		/// 컬렉션을 반복하는 열거자를 가져옵니다.
        /// </summary>
		/// <returns>
		/// 열거자를 나타내는 <see cref="System::Collections::Generic::IEnumerator"/>입니다.
		/// </returns>
		virtual System::Collections::Generic::IEnumerator<Cell^>^ GetEnumerator();

	private:
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

#pragma region ICollection 멤버
		virtual void CopyTo(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo
        {
            for each(Cell^ item in this)
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

	internal: // methods			
		CellCollection(_Row^ row);

	internal:
		property Cell^ default[GrColumn*]
		{
			Cell^ get(GrColumn* pColumn);
		}

	private: // variables
		_Row^						m_row;
		GrColumnList*				m_pColumnList;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/