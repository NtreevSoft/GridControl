#pragma once
#include "Column.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// 스크롤 되지 않는 열의 컬렉션을 나타냅니다.
    /// </summary>
    public ref class FrozenColumnCollection : System::Collections::Generic::IEnumerable<Ntreev::Windows::Forms::Grid::Column^>, System::Collections::ICollection, GridObject
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

    public: // methods

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
        /// 가져올 열의 인덱스(0부터 시작)입니다.
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
            int get();
        }

    internal: // methods

        FrozenColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

    private: // methods

        virtual System::Collections::IEnumerator^ GetEnumerator_System_Collections_IEnumerable() sealed = System::Collections::IEnumerable::GetEnumerator;

        virtual void CopyT_System_Collections_ICollectiono(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo;

    private: // properties

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

    private: // variables

        GrColumnList* m_pColumnList;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/