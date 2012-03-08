#pragma once
#include "Column.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// ��ũ�� ���� �ʴ� ���� �÷����� ��Ÿ���ϴ�.
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
        /// �÷����� �ݺ��ϴ� �����ڸ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// �����ڸ� ��Ÿ���� <see cref="System::Collections::Generic::IEnumerator"/>�Դϴ�.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ GetEnumerator();

    public: // properties

        /// <summary>
        /// ������ �ε����� �ִ� ���� �����ɴϴ�.
        /// </summary>
        /// <param name="index">
        /// ������ ���� �ε���(0���� ����)�Դϴ�.
        /// </param>
        /// <returns>
        /// ������ �ε����� <see cref="Column"/>�Դϴ�.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index�� 0���� �۰ų�, <see cref="Count"/>���� Ŭ ���
        /// </exception>
        property Ntreev::Windows::Forms::Grid::Column^ default[int]
        {
            Ntreev::Windows::Forms::Grid::Column^ get(int index);
        }

        /// <summary>
        /// �׸��� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������ ��Ÿ���� <see cref="System::Int32"/>�� �������Դϴ�.
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