#pragma once
#include "Row.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// ���õ� ���� �÷����� ��Ÿ���ϴ�.
    /// </summary>
    public ref class SelectedRowCollection : Ntreev::Windows::Forms::Grid::GridObject, 
        System::Collections::Generic::IEnumerable<Ntreev::Windows::Forms::Grid::Row^>, 
        System::Collections::ICollection
    {
    private: // classes

        ref class Enumerator : System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>
        {
        public:
            Enumerator(const GrSelectedRows* selectedRows);
            ~Enumerator();
            virtual bool MoveNext();
            virtual void Reset();

        public:
            property Ntreev::Windows::Forms::Grid::Row^ Current
            {
                virtual Ntreev::Windows::Forms::Grid::Row^ get();
            }

        private:
            property System::Object^ Current_System_Collections_IEnumerator
            {
                virtual System::Object^ get() sealed = System::Collections::IEnumerator::Current::get;
            }

        private:
            unsigned int m_index;
            const GrSelectedRows* m_selectedRows;
        };

    public: // methods

        /// <summary>
        /// Row::IsSelected�� ���� true�� �����ϰ� ��Ͽ� �߰��մϴ�.
        /// </summary>
        /// <remarks> 
        /// Row::IsSelected �� ���� �̹� true���ٸ� �ƹ��� ������ �Ͼ�� �ʽ��ϴ�. 
        /// �� �޼���� Row::IsSelected = true �� ������ �۾��� �����մϴ�.
        /// </remarks>
        virtual void Add(Ntreev::Windows::Forms::Grid::Row^ row); 

        /// <summary>
        /// Row::IsSelected�� ���� true�� �����ϰ� ��Ͽ� �߰��ϰ� focus�࿡ �ش��ϴ� ���� ��Ŀ���� �����մϴ�.
        /// </summary>
        /// <remarks>
        /// </remarks>
        /// <param name="focusColumn">
        /// ��Ŀ���� ������ Column�Դϴ�.
        /// null ������ ���� �ֱٰ����� ��Ŀ���� �����մϴ�.
        /// </param>
        void Add(Ntreev::Windows::Forms::Grid::Row^ row, Column^ focusColumn);

        /// <summary>
        /// Row::IsSelected�� ���� false�� �����ϰ� ��Ͽ� �����մϴ�.
        /// </summary>
        /// <remarks> 
        /// Row::IsSelected �� ���� �̹� false���ٸ� �ƹ��� ������ �Ͼ�� �ʽ��ϴ�. 
        /// �� �޼���� Row::IsSelected = false �� ������ �۾��� �����մϴ�.
        /// </remarks>
        virtual bool Remove(Ntreev::Windows::Forms::Grid::Row^ row);

        /// <summary>
        /// ��� Row::IsSelected �� ���� false�� �����ϰ� �����մϴ�.
        /// </summary>
        /// <remarks> 
        /// �� �޼���� GridControl::ClearSelection() �� ������ �۾��� �����մϴ�.
        /// </remarks>
        virtual void Clear();

        /// <summary>
        /// �÷����� �ݺ��ϴ� �����ڸ� �����ɴϴ�.
        /// </summary>
        /// <returns>
        /// �����ڸ� ��Ÿ���� <see cref="System::Collections::Generic::IEnumerator"/>�Դϴ�.
        /// </returns>
        virtual System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>^ GetEnumerator();

    public: // properties

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
        property Ntreev::Windows::Forms::Grid::Row^ default[int]
        {
            Ntreev::Windows::Forms::Grid::Row^ get(int index);
        }

    internal: // methods

        SelectedRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, const GrSelectedRows* selectedRows);

    private: // methods

        virtual System::Collections::IEnumerator^ GetEnumerator_System_Collections_IEnumerablen() sealed = System::Collections::IEnumerable::GetEnumerator;

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

        const GrSelectedRows* m_selectedRows;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/