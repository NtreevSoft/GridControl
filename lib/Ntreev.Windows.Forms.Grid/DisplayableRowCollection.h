#pragma once
#include "RowBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// ȭ�鿡 ǥ�õǴ� ���� �÷����� ��Ÿ���ϴ�.
    /// </summary>
    public ref class DisplayableRowCollection : Ntreev::Windows::Forms::Grid::GridObject,
        System::Collections::Generic::IEnumerable<Ntreev::Windows::Forms::Grid::RowBase^>,
        System::Collections::ICollection
    {
    private: // classes

        ref class Enumerator : System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::RowBase^>
        {
        public:
            Enumerator(GrDataRowList* pDataRowList);
            ~Enumerator();
            virtual bool MoveNext();
            virtual void Reset();

            property Ntreev::Windows::Forms::Grid::RowBase^ Current
            {
                virtual Ntreev::Windows::Forms::Grid::RowBase^ get();
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

    public: // methods

        /// <summary>
        /// �÷����� �ݺ��ϴ� �����ڸ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// �����ڸ� ��Ÿ���� <see cref="System::Collections::Generic::IEnumerator"/>�Դϴ�.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::RowBase^>^ GetEnumerator();

    public: // properties

        /// <summary>
        /// ������ �ε����� �ִ� <see cref="RowBase"/>�� �����ɴϴ�.
        /// </summary>
        /// <param name="index">
        /// ������ <see cref="RowBase"/>�� �ε���(0���� ����)�Դϴ�.
        /// </param>
        /// <returns>
        /// ������ �ε����� <see cref="RowBase"/>�Դϴ�.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index�� 0���� �۰ų�, <see cref="Count"/>���� Ŭ ���
        /// </exception>
        property Ntreev::Windows::Forms::Grid::RowBase^ default[int]
        {
            Ntreev::Windows::Forms::Grid::RowBase^ get(int index);
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

        DisplayableRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

    private: // methods

        virtual System::Collections::IEnumerator^ GetEnumerator_System_Collections_IEnumerable() sealed = System::Collections::IEnumerable::GetEnumerator;

        virtual void CopyTo_System_Collections_ICollection(System::Array^ array, int index) sealed = System::Collections::ICollection::CopyTo;

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

        GrDataRowList* m_pDataRowList;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/