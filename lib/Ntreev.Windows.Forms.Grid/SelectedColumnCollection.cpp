#include "StdAfx.h"
#include "SelectedColumnCollection.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    SelectedColumnCollection::Enumerator::Enumerator(const GrSelectedColumns* selectedColumns)
        : m_selectedColumns(selectedColumns)
    {
        m_index = 0;
    }

    SelectedColumnCollection::Enumerator::~Enumerator()
    {

    }

    bool SelectedColumnCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_selectedColumns->size();
    }

    void SelectedColumnCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Column^ SelectedColumnCollection::Enumerator::Current::get()
    {
        const GrColumn* pColumn = (*m_selectedColumns)[m_index - 1];
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    System::Object^ SelectedColumnCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current; 
    }

    SelectedColumnCollection::SelectedColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, const GrSelectedColumns* selectedColumns) 
        : GridObject(gridControl), m_selectedColumns(selectedColumns)
    {

    }

    void SelectedColumnCollection::Add(Ntreev::Windows::Forms::Grid::Column^ item)
    {
        if(item->IsSelected == true)
            return;
        item->IsSelected = true;
    }

    bool SelectedColumnCollection::Remove(Ntreev::Windows::Forms::Grid::Column^ item)
    {
        if(item->IsSelected == false)
            return false;
        item->IsSelected = false;
        return true;
    }

    void SelectedColumnCollection::Clear()
    {
        GridControl->ClearSelection();
    }

    int SelectedColumnCollection::Count::get()
    {
        return (int)m_selectedColumns->size();
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ SelectedColumnCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::SelectedColumnCollection::Enumerator(m_selectedColumns);
    }

    System::Collections::IEnumerator^ SelectedColumnCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void SelectedColumnCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool SelectedColumnCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ SelectedColumnCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this; 
    }

    int SelectedColumnCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/