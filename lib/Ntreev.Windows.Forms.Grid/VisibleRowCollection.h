#pragma once
#include "RowBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// ǥ�õǴ� ���� �÷����� ��Ÿ���ϴ�.
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
        /// ���� <see cref="System:Object"/>�� ��Ÿ���� <see cref="System::String"/>�� ��ȯ�մϴ�.
        /// </summary>
        /// <returns>
        /// ���� <see cref="System:Object"/>�� ��Ÿ���� <see cref="System::String"/>�Դϴ�.
        /// </returns>
        virtual System::String^ ToString() override
        {
            return this->Count.ToString();
        }

        /// <summary>
        /// �÷����� �ݺ��ϴ� �����ڸ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// �����ڸ� ��Ÿ���� <see cref="System::Collections::Generic::IEnumerator"/>�Դϴ�.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<RowBase^>^ GetEnumerator() { return gcnew Enumerator(m_pDataRowList); }

    public: // properties
        /// <summary>
        /// ������ �ε����� �ִ� <see cref="Row"/>�� �����ɴϴ�.
        /// </summary>
        /// <param name="index">
        /// ������ <see cref="Row"/>�� �ε���(0���� ����)�Դϴ�.
        /// </param>
        /// <returns>
        /// ������ �ε����� <see cref="Row"/>�Դϴ�.
        /// </returns>
        /// <exception cref="System::ArgumentOutOfRangeException">
        /// index�� 0���� �۰ų�, <see cref="Count"/>���� Ŭ ���
        /// </exception>
        property RowBase^ default[int]
        {
            RowBase^ get(int index);
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
        VisibleRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

    private: // methods
        virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
        {
            return GetEnumerator();
        }

#pragma region ICollection ���
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