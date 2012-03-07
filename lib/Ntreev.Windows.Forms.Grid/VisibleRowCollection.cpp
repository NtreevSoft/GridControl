#include "stdafx.h"
#include "VisibleRowCollection.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{

    VisibleRowCollection::VisibleRowCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pDataRowList = GridCore->GetDataRowList();
    }

    RowBase^ VisibleRowCollection::default::get(int index)
    {
        if((unsigned int)index >= m_pDataRowList->GetVisibleRowCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        IDataRow* pDataRow = m_pDataRowList->GetVisibleRow(index);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<RowBase^>(ref);
    }

    int VisibleRowCollection::Count::get()
    {
        return m_pDataRowList->GetVisibleRowCount();
    }

    VisibleRowCollection::Enumerator::Enumerator(GrDataRowList* pDataRowList)
        : m_pDataRowList(pDataRowList)
    {
        m_index = 0;
    }

    VisibleRowCollection::Enumerator::~Enumerator()
    {

    }

    bool VisibleRowCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pDataRowList->GetVisibleRowCount();
    }

    void VisibleRowCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    RowBase^ VisibleRowCollection::Enumerator::Current::get()
    {
        IDataRow* pDataRow = m_pDataRowList->GetVisibleRow(m_index - 1);
        System::Object^ ref = pDataRow->ManagedRef;
        return safe_cast<RowBase^>(ref);
    }

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/