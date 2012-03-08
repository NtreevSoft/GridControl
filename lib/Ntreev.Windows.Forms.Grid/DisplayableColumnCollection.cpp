#include "StdAfx.h"
#include "DisplayableColumnCollection.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    DisplayableColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
        : m_pColumnList(pColumnList)
    {
        m_index = 0;
    }

    DisplayableColumnCollection::Enumerator::~Enumerator()
    {

    }

    bool DisplayableColumnCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pColumnList->GetDisplayableColumnCount();
    }

    void DisplayableColumnCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Column^ DisplayableColumnCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = m_pColumnList->GetDisplayableColumn(m_index - 1);
        System::Object^ ref = pColumn->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Column^>(ref);
    }

    System::Object^ DisplayableColumnCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current;
    }

    DisplayableColumnCollection::DisplayableColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pColumnList = GridCore->GetColumnList();
    }

    Ntreev::Windows::Forms::Grid::Column^ DisplayableColumnCollection::default::get(int index)
    {
        if((unsigned int)index >= m_pColumnList->GetDisplayableColumnCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        GrColumn* pColumn = m_pColumnList->GetDisplayableColumn(index);
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    int DisplayableColumnCollection::Count::get()
    {
        return m_pColumnList->GetDisplayableColumnCount();
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ DisplayableColumnCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::DisplayableColumnCollection::Enumerator(m_pColumnList); 
    }

    System::Collections::IEnumerator^ DisplayableColumnCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void DisplayableColumnCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool DisplayableColumnCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ DisplayableColumnCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this; 
    }

    int DisplayableColumnCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count; 
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/