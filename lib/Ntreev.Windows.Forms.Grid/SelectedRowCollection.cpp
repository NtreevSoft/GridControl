#include "stdafx.h"
#include "SelectedRowCollection.h"
#include "Column.h"
#include "GridControl.h"
#include "Resources.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    SelectedRowCollection::SelectedRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, const GrSelectedRows* selectedRows) 
        : GridObject(gridControl), m_selectedRows(selectedRows)
    {

    }

    void SelectedRowCollection::Add(Ntreev::Windows::Forms::Grid::Row^ item)
    {
        using namespace Ntreev::Windows::Forms::Grid::Properties;

        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");

        if(item->Index == INSERTION_ROW)
            throw gcnew System::Exception(Resources::RemoveInsertionRowException);

        if(item->Index == INVALID_INDEX)
            throw gcnew System::ArgumentException("이미 지워지거나 사용되지 않은 row입니다.");

        if(item->IsSelected == true)
            return;
        item->IsSelected = true;
    }

    void SelectedRowCollection::Add(Ntreev::Windows::Forms::Grid::Row^ row, Column^ focusColumn)
    {
        Add(row);

        GrDataRow* pDataRow = row->NativeRef;
        GrItem* pItem = nullptr;
        if(focusColumn == nullptr)
        {
            GrFocuser* pFocuser = GridCore->GetFocuser();
            GrColumn* pLastFocusedColumn = pFocuser->GetLastFocusedColumn();
            if(pLastFocusedColumn != nullptr)
            {
                pItem = pDataRow->GetItem(pLastFocusedColumn);
            }
            else
            {
                GrColumnList* pColumnList = GridCore->GetColumnList();
                if(pColumnList->GetDisplayableColumnCount() != 0)
                {
                    GrColumn* pColumn = pColumnList->GetDisplayableColumn(0);
                    pItem = pDataRow->GetItem(pColumn);
                }
            }
        }
        else
        {
            pItem = pDataRow->GetItem(focusColumn->NativeRef);
        }

        Focuser->Set(pItem);
    }

    bool SelectedRowCollection::Remove(Ntreev::Windows::Forms::Grid::Row^ row)
    {
        if(row->IsSelected == false)
            return false;
        row->IsSelected = false;
        return true;
    }

    void SelectedRowCollection::Clear()
    {
        GridControl->ClearSelection();
    }


    SelectedRowCollection::Enumerator::Enumerator(const GrSelectedRows* selectedRows)
        : m_selectedRows(selectedRows), m_index(0)
    {

    }

    SelectedRowCollection::Enumerator::~Enumerator()
    {

    }

    bool SelectedRowCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_selectedRows->size();
    }

    void SelectedRowCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Row^ SelectedRowCollection::Enumerator::Current::get()
    {
        const GrDataRow* pDataRow = m_selectedRows->at(m_index-1);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Row^>(ref);
    }

    System::Object^ SelectedRowCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current; 
    }

    Ntreev::Windows::Forms::Grid::Row^ SelectedRowCollection::default::get(int index)
    {
        const GrDataRow* pDataRow = m_selectedRows->at(index);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Row^>(ref);
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Row^>^ SelectedRowCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::SelectedRowCollection::Enumerator(m_selectedRows);
    }

    int SelectedRowCollection::Count::get()
    {
        return (int)m_selectedRows->size();
    }

    System::Collections::IEnumerator^ SelectedRowCollection::GetEnumerator_System_Collections_IEnumerablen()
    {
        return GetEnumerator();
    }

    void SelectedRowCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Row^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    bool SelectedRowCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ SelectedRowCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int SelectedRowCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/