#include "StdAfx.h"
#include "UnfrozenColumnCollection.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    UnfrozenColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
        : m_pColumnList(pColumnList)
    {
        m_index = 0;
    }

    UnfrozenColumnCollection::Enumerator::~Enumerator()
    {

    }

    bool UnfrozenColumnCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pColumnList->GetUnfrozenColumnCount();
    }

    void UnfrozenColumnCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Column^ UnfrozenColumnCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = m_pColumnList->GetUnfrozenColumn(m_index - 1);
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    System::Object^ UnfrozenColumnCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current;
    }

    UnfrozenColumnCollection::UnfrozenColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pColumnList = GridCore->GetColumnList();
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ UnfrozenColumnCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::UnfrozenColumnCollection::Enumerator(m_pColumnList); 
    }

    Ntreev::Windows::Forms::Grid::Column^ UnfrozenColumnCollection::default::get(int index)
    {
        if((unsigned int)index >= m_pColumnList->GetUnfrozenColumnCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        GrColumn* pColumn = m_pColumnList->GetUnfrozenColumn(index);
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    int UnfrozenColumnCollection::Count::get()
    {
        return m_pColumnList->GetUnfrozenColumnCount();
    }

    System::Collections::IEnumerator^ UnfrozenColumnCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void UnfrozenColumnCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool UnfrozenColumnCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ UnfrozenColumnCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this; 
    }

    int UnfrozenColumnCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/