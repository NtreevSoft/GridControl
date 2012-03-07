#pragma once
#include "RowBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 표시되는 행의 컬렉션을 나타냅니다.
    /// </summary>
    public ref class VisibleRowCollection : Ntreev::Windows::Forms::Grid::GridObject,
        System::Collections::Generic::IEnumerable<RowBase^>, System::Collections::ICollection
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
            property System::Object^ Current2
            {
                virtual System::Object^ get() sealed = System::Collections::IEnumerator::Current::get { return Current; }
            }

        private:
            unsigned int   m_index;
            GrDataRowList* m_pDataRowList;
        };

    public: // methods
        /// <summary>
        /// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
        /// </summary>
        /// <returns>
        /// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
        /// </returns>
        virtual System::String^ ToString() override
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
        VisibleRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

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

        property System::Object^ SyncRoot
        {
            virtual System::Object^ get() sealed = System::Collections::ICollection::SyncRoot::get { return this; }
        }

        property int Count2
        {
            virtual int get() sealed = System::Collections::ICollection::Count::get { return this->Count; }
        }
#pragma endregion

    private: // variables
        GrDataRowList*        m_pDataRowList;
    };

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/