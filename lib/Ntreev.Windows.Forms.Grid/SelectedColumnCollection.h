#pragma once
#include "Column.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// ���õ� ���� �÷����� ��Ÿ���ϴ�.
    /// </summary>
    public ref class SelectedColumnCollection : Ntreev::Windows::Forms::Grid::GridObject, 
        System::Collections::Generic::IEnumerable<Ntreev::Windows::Forms::Grid::Column^>,
        System::Collections::ICollection
    {
    private: // classes

        ref class Enumerator : System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>
        {
        public:
            Enumerator(const GrSelectedColumns* selectedColumns);
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
            const GrSelectedColumns* m_selectedColumns;
        };

    public: // methods

        void Add(Ntreev::Windows::Forms::Grid::Column^ item); 

        bool Remove(Ntreev::Windows::Forms::Grid::Column^ item);

        void Clear();

        /// <summary>
        /// �׸��� ������ �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// ������ ��Ÿ���� <see cref="System::Int32"/>�� �������Դϴ�.
        /// </returns>
        property int Count
        {
            virtual int get();
        }

        /// <summary>
        /// �÷����� �ݺ��ϴ� �����ڸ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// �����ڸ� ��Ÿ���� <see cref="System::Collections::Generic::IEnumerator"/>�Դϴ�.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ GetEnumerator();

    internal: // methods

        SelectedColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, const GrSelectedColumns* selectedColumns);

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

        const GrSelectedColumns* m_selectedColumns;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/