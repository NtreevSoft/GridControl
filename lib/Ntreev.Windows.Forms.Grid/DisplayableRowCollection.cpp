#include "stdafx.h"
#include "DisplayableRowCollection.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    DisplayableRowCollection::DisplayableRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pDataRowList = GridCore->GetDataRowList();
    }

    RowBase^ DisplayableRowCollection::default::get(int index)
    {
        if((unsigned int)index >= m_pDataRowList->GetDisplayableRowCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(index);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<RowBase^>(ref);
    }

    int DisplayableRowCollection::Count::get()
    {
        return m_pDataRowList->GetDisplayableRowCount();
    }


    DisplayableRowCollection::Enumerator::Enumerator(GrDataRowList* pDataRowList)
        : m_pDataRowList(pDataRowList)
    {
        m_index = 0;
    }

    DisplayableRowCollection::Enumerator::~Enumerator()
    {

    }

    bool DisplayableRowCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pDataRowList->GetDisplayableRowCount();
    }

    void DisplayableRowCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    RowBase^ DisplayableRowCollection::Enumerator::Current::get()
    {
        IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(m_index - 1);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<RowBase^>(ref);
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/