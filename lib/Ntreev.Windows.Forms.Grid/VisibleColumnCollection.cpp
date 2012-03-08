#include "StdAfx.h"
#include "VisibleColumnCollection.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    VisibleColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
        : m_pColumnList(pColumnList)
    {
        m_index = 0;
    }

    VisibleColumnCollection::Enumerator::~Enumerator()
    {

    }

    bool VisibleColumnCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pColumnList->GetVisibleColumnCount();
    }

    void VisibleColumnCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Column^ VisibleColumnCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = m_pColumnList->GetVisibleColumn(m_index - 1);
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    System::Object^ VisibleColumnCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current; 
    }

    VisibleColumnCollection::VisibleColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pColumnList = GridCore->GetColumnList();
    }

    Ntreev::Windows::Forms::Grid::Column^ VisibleColumnCollection::default::get(int index)
    {
        if((unsigned int)index >= m_pColumnList->GetVisibleColumnCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        GrColumn* pColumn = m_pColumnList->GetVisibleColumn(index);
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    int VisibleColumnCollection::Count::get()
    {
        return m_pColumnList->GetVisibleColumnCount();
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ VisibleColumnCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::VisibleColumnCollection::Enumerator(m_pColumnList);
    }

    System::Collections::IEnumerator^ VisibleColumnCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void VisibleColumnCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool VisibleColumnCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ VisibleColumnCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this; 
    }

    int VisibleColumnCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/