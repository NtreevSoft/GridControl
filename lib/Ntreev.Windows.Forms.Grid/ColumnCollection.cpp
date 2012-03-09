//=====================================================================================================================
// Ntreev Grid for .Net 2.0.0.0
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


#include "StdAfx.h"
#include "ColumnCollection.h"
#include "Column.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ColumnCollection::Enumerator::Enumerator(GrColumnList* pColumnList)
        : m_pColumnList(pColumnList), m_index(0)
    {

    }

    ColumnCollection::Enumerator::~Enumerator()
    {

    }

    bool ColumnCollection::Enumerator::MoveNext()
    {
        m_index++;
        return m_index <= m_pColumnList->GetColumnCount();
    }

    void ColumnCollection::Enumerator::Reset()
    {
        m_index = 0;
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::Enumerator::Current::get()
    {
        GrColumn* pColumn = m_pColumnList->GetColumn(m_index - 1);
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    System::Object^ ColumnCollection::Enumerator::Current_System_Collections_IEnumerator::get()
    {
        return Current; 
    }

    ColumnCollection::ServiceProvider::ServiceProvider(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : m_gridControl(gridControl)
    {

    }

    System::Object^ ColumnCollection::ServiceProvider::GetService(System::Type^ serviceType)
    {
        return m_gridControl->GetInternalService(serviceType); 
    }

    ColumnCollection::ColumnCollection(Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : GridObject(gridControl)
    {
        m_pColumnList = GridCore->GetColumnList();

        m_listChangedEventHandler = gcnew System::ComponentModel::ListChangedEventHandler(this, &ColumnCollection::currencyManager_ListChanged);

        gridControl->CurrencyManagerChanging += gcnew CurrencyManagerChangingEventHandler(this, &ColumnCollection::gridControl_CurrencyManagerChanging);
        gridControl->CurrencyManagerChanged += gcnew CurrencyManagerChangedEventHandler(this, &ColumnCollection::gridControl_CurrencyManagerChanged);
        gridControl->Clearing += gcnew ClearEventHandler(this, &ColumnCollection::gridControl_Clearing);
        gridControl->Cleared += gcnew ClearEventHandler(this, &ColumnCollection::gridControl_Cleared);
    }

    void ColumnCollection::Add(Ntreev::Windows::Forms::Grid::Column^ item)
    {
        Insert(Count, item);
    }

    void ColumnCollection::AddRange(cli::array<Ntreev::Windows::Forms::Grid::Column^>^ values)
    {
        if(values == nullptr)
            throw gcnew System::ArgumentNullException("values");

        for each(Ntreev::Windows::Forms::Grid::Column^ item in values)
        {
            Add(item);
        }
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::Add()
    {
        return Insert(this->Count);
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::Add(System::String^ name)
    {
        return Insert(this->Count, name);
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::Add(System::String^ name, System::Type^ type)
    {
        return Insert(this->Count, name, type);
    }

    void ColumnCollection::Remove(Ntreev::Windows::Forms::Grid::Column^ item)
    {
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        if(item->Index == 0xffffffff)
            throw gcnew System::ArgumentException("이미 제거된 항목입니다.");

        item->GridControl = nullptr;
        m_pColumnList->RemoveColumn(item->NativeRef);
    }

    int ColumnCollection::IndexOf(Ntreev::Windows::Forms::Grid::Column^ item)
    {
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        return item->Index;
    }

    void ColumnCollection::Insert(int index, Ntreev::Windows::Forms::Grid::Column^ item)
    {
        if(index < 0 || index > this->Count)
            throw gcnew System::ArgumentOutOfRangeException("index");
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        if(item->Index != 0xffffffff)
            throw gcnew System::ArgumentException("이미 추가된 항목입니다.");

        if(this[item->ColumnName] != nullptr)
            throw gcnew System::ArgumentException(System::String::Format("\"{0}\"의 이름을 가진 항목이 이미 있습니다.", item->ColumnName));

        if(item->ColumnName == System::String::Empty)
            item->ColumnName = NewColumnName();

        if(GridControl->InvokeColumnInserting(item) == false)
            return;

        item->GridControl = GridControl;
        m_pColumnList->InsertColumn(item->NativeRef, index);

        GridControl->InvokeColumnInserted(item);
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::Insert(int index)
    {
        return Insert(index, NewColumnName());
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::Insert(int index, System::String^ name)
    {
        return Insert(index, name, System::String::typeid);
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::Insert(int index, System::String^ name, System::Type^ type)
    {
        if(name == nullptr || name == System::String::Empty)
            name = NewColumnName();
        if(type == nullptr)
            type = System::String::typeid;

        Ntreev::Windows::Forms::Grid::Column^ column = CreateColumnInstance();
        column->ColumnName = name;
        column->DataType = type;
        Insert(index, column);
        return column;
    }

    void ColumnCollection::RemoveAt(int index)
    {
        Ntreev::Windows::Forms::Grid::Column^ column = this[index];
        Remove(column);
    }

    bool ColumnCollection::Contains(Ntreev::Windows::Forms::Grid::Column^ item)
    {
        if(item == nullptr)
            throw gcnew System::ArgumentNullException("item");
        return item->Index >= 0;
    }

    System::Collections::Generic::IEnumerator<Ntreev::Windows::Forms::Grid::Column^>^ ColumnCollection::GetEnumerator()
    {
        return gcnew Ntreev::Windows::Forms::Grid::ColumnCollection::Enumerator(m_pColumnList);
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::Bind(System::ComponentModel::PropertyDescriptor^ propertyDescriptor)
    {
        Ntreev::Windows::Forms::Grid::Column^ existColumn = this[propertyDescriptor->Name];
        Ntreev::Windows::Forms::Grid::Column^ column = GridControl->InvokeColumnBinding(propertyDescriptor, existColumn);
        if(column == nullptr)
        {
            column = CreateColumnInstance();
        }

        if(existColumn == nullptr)
            column->HasLifeline = true;

        if(GridControl->InvokeColumnInserting(column) == false)
            return nullptr;

        if(column->GridControl == nullptr)
        {
            column->GridControl = GridControl;
            column->PropertyDescriptor = propertyDescriptor;
            m_pColumnList->AddColumn(column->NativeRef);
        }
        else
        {
            column->PropertyDescriptor = propertyDescriptor;
        }

        GridControl->InvokeColumnInserted(column);
        GridControl->InvokeColumnBinded(column);

        return column;
    }

    void ColumnCollection::Unbind(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        column->PropertyDescriptor = nullptr;

        if(column->HasLifeline == true)
        {
            column->GridControl = nullptr;
            m_pColumnList->RemoveColumn(column->NativeRef);
            delete column;
        }
    }

    void ColumnCollection::DeleteAll()
    {
        while(this->Count > 0)
        {
            Ntreev::Windows::Forms::Grid::Column^ column = this[this->Count - 1];
            delete column;
        }
    }

    void ColumnCollection::SetItemsByDesigner(cli::array<System::Object^>^ values)
    {
        GrGroupPanel* pGroupPanel = GridCore->GetGroupPanel();
        std::vector<GrColumn*> groupings;
        groupings.reserve(pGroupPanel->GetGroupCount());

        for(unsigned int i=0 ; i<pGroupPanel->GetGroupCount() ; i++)
        {
            GrColumn* pColumn = pGroupPanel->GetGroup(i)->GetColumn();
            groupings.push_back(pColumn);
        }

        for(unsigned int i=m_pColumnList->GetColumnCount()-1 ; i>=0 && i<m_pColumnList->GetColumnCount() ; i--)
        {
            GrColumn* pColumn = m_pColumnList->GetColumn(i);
            m_pColumnList->RemoveColumn(pColumn);
        }

        m_pColumnList->Update(true);

        for_each(std::vector<GrColumn*>, groupings, value)
        {
            value->SetGrouped(false);
        }

        for each(System::Object^ item in values)
        {
            Ntreev::Windows::Forms::Grid::Column^ column = dynamic_cast<Ntreev::Windows::Forms::Grid::Column^>(item);
            if(column->ColumnID == INVALID_INDEX)
                this->Add_System_Collections_IList(item);
            else
                m_pColumnList->AddColumn(column->NativeRef);
        }

        for_each(std::vector<GrColumn*>, groupings, value)
        {
            if(value->GetIndex() == INVALID_INDEX)
                continue;
            value->SetGrouped(true);
        }
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::default::get(int index)
    {
        if(index < 0 || index >= (int)m_pColumnList->GetColumnCount())
            throw gcnew System::ArgumentOutOfRangeException("index");
        GrColumn* pColumn = m_pColumnList->GetColumn((unsigned int)index);
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    int ColumnCollection::Count::get()
    {
        return m_pColumnList->GetColumnCount();
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::default::get(System::String^ columnName)
    {
        if(columnName == nullptr)
            columnName = System::String::Empty;

        for each(Ntreev::Windows::Forms::Grid::Column^ column in this)
        {
            if(column->ColumnName == columnName)
                return column;
        }
        return nullptr;
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::default::get(GrColumn* pColumn)
    {
        return Ntreev::Windows::Forms::Grid::Column::FromNative(pColumn);
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::CreateColumnInstance(System::IServiceProvider^ serviceProvider, System::Type^ columnType)
    {
        using namespace System::ComponentModel;
        using namespace System::ComponentModel::Design;
        if(serviceProvider != nullptr)
        {
            IDesignerHost^ designerHost = dynamic_cast<IDesignerHost^>(serviceProvider->GetService(IDesignerHost::typeid));
            if(designerHost != nullptr)
            {
                return dynamic_cast<Ntreev::Windows::Forms::Grid::Column^>(designerHost->CreateComponent(columnType));
            }
        }
        return dynamic_cast<Ntreev::Windows::Forms::Grid::Column^>(TypeDescriptor::CreateInstance(serviceProvider, columnType, nullptr, nullptr));
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::CreateColumnInstance()
    {
        ServiceProvider^ serviceProvider = gcnew ServiceProvider(this->GridControl);
        return CreateColumnInstance(serviceProvider, Column::typeid);
    }

    Ntreev::Windows::Forms::Grid::Column^ ColumnCollection::CreateColumnInstance(System::IServiceProvider^ serviceProvider)
    {
        return CreateColumnInstance(serviceProvider, Column::typeid);
    }

    System::String^ ColumnCollection::NewColumnName()
    {
        int columnIndex = 0;

        System::String^ name;
        do
        {
            name = System::String::Format("Column{0}", columnIndex++);
        }
        while(this[name] != nullptr);

        return name;
    }

    void ColumnCollection::currencyManager_ListChanged(System::Object^ sender, System::ComponentModel::ListChangedEventArgs^ e)
    {
        using namespace System::ComponentModel;
        using namespace System::ComponentModel::Design;

        switch(e->ListChangedType)
        {
        case System::ComponentModel::ListChangedType::PropertyDescriptorAdded:
            {
                Bind(e->PropertyDescriptor);
            }
            break;
        case System::ComponentModel::ListChangedType::PropertyDescriptorChanged:
            {
                using namespace System::Windows::Forms;
                using namespace System::Collections::Generic;

                CurrencyManager^ currencyManager = dynamic_cast<CurrencyManager^>(sender);

                List<Ntreev::Windows::Forms::Grid::Column^>^ boundColumns = gcnew List<Ntreev::Windows::Forms::Grid::Column^>();

                for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
                {
                    if(item->PropertyDescriptor != nullptr)
                        boundColumns->Add(item);
                }

                System::ComponentModel::PropertyDescriptor^ changedPropertyDescriptor = nullptr;

                for each(System::ComponentModel::PropertyDescriptor^ item in currencyManager->GetItemProperties())
                {
                    Ntreev::Windows::Forms::Grid::Column^ column = this[item->Name];
                    if(column != nullptr)
                    {
                        column->PropertyDescriptor = item;
                        boundColumns->Remove(column);
                    }
                    else
                    {
                        changedPropertyDescriptor = item;
                    }
                }

                if(boundColumns->Count == 1 && changedPropertyDescriptor != nullptr)
                {
                    boundColumns[0]->PropertyDescriptor = changedPropertyDescriptor;
                }
            }
            break;
        case System::ComponentModel::ListChangedType::PropertyDescriptorDeleted:
            {
                Ntreev::Windows::Forms::Grid::Column^ column = this[e->PropertyDescriptor->Name];
                System::Diagnostics::Debug::Assert(column != nullptr);
                Unbind(column);
            }
            break;
        case System::ComponentModel::ListChangedType::Reset:
            {
                PropertyDescriptorCollection^ props = m_manager->GetItemProperties();

                for each(PropertyDescriptor^ item in props)
                {
                    Bind(item);
                }
            }
            break;
        }
    }

    void ColumnCollection::gridControl_CurrencyManagerChanging(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::CurrencyManagerChangingEventArgs^ e)
    {
        System::Windows::Forms::CurrencyManager^ manager = e->CurrecnyManager;

        if(manager == nullptr)
            return;

        if(e->Cancel == true)
            return;

        for each(System::ComponentModel::PropertyDescriptor^ item in manager->GetItemProperties())
        {
            Ntreev::Windows::Forms::Grid::Column^ column = this[item->Name];
            if(column == nullptr)
                continue;

            if(column->DataType != item->PropertyType)
            {
                if(column->CanConvertFrom(item->PropertyType) == true)
                    continue;

                System::Text::StringBuilder^ builder = gcnew System::Text::StringBuilder();

                builder->AppendLine(System::String::Format("{0} 은 연결될 데이터 소스와 데이터 타입이 다릅니다.", item->Name));
                builder->AppendLine(System::String::Format(" GridControl : {0}", column->DataType));
                builder->AppendLine(System::String::Format(" DataSource : {0}", item->PropertyType));

                e->Cancel = true;
                e->CancelReason = builder->ToString();
            }

            if(e->Cancel == true)
                return;
        }
    }

    void ColumnCollection::gridControl_CurrencyManagerChanged(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs^ e)
    {
        if(m_manager != nullptr)
        {
            m_manager->ListChanged -= m_listChangedEventHandler;
        }

        m_manager = e->CurrecnyManager;

        if(m_manager == nullptr)
            return;

        for each(System::ComponentModel::PropertyDescriptor^ item in m_manager->GetItemProperties())
        {
            Bind(item);
        }

        m_manager->ListChanged += m_listChangedEventHandler;
    }

    void ColumnCollection::gridControl_Clearing(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::ClearEventArgs^ e)
    {
        if(e->DataSourceOnly == true)
        {
            m_tempBindableColumns = gcnew System::Collections::ArrayList();

            for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
            {
                if(item->HasLifeline == false || item->PropertyDescriptor == nullptr)
                    m_tempBindableColumns->Add(item);
            }

            for each(Ntreev::Windows::Forms::Grid::Column^ item in m_tempBindableColumns)
            {
                this->Remove(item);
            }
        }

        DeleteAll();
    }

    void ColumnCollection::gridControl_Cleared(System::Object^ /*sender*/, Ntreev::Windows::Forms::Grid::ClearEventArgs^ e)
    {
        if(e->DataSourceOnly == true)
        {
            for each(Ntreev::Windows::Forms::Grid::Column^ item in m_tempBindableColumns)
            {
                this->Add(item);
            }

            delete m_tempBindableColumns;
        }
    }

    System::Collections::IEnumerator^ ColumnCollection::GetEnumerator_System_Collections_IEnumerable()
    {
        return GetEnumerator();
    }

    void ColumnCollection::RemoveAt_System_Collections_IList(int index)
    {
        this->RemoveAt(index);
    }

    int ColumnCollection::Add_System_Collections_IList(System::Object^ value)
    {
        int count = this->Count;
        Add((Ntreev::Windows::Forms::Grid::Column^)value);
        return count;
    }

    bool ColumnCollection::Contains_System_Collections_IList(System::Object^ value)
    {
        return this->Contains((Ntreev::Windows::Forms::Grid::Column^)value);
    }

    void ColumnCollection::Clear_System_Collections_IList()
    {
        for(int i=this->Count-1 ; i>=0 ; i--)
        {
            RemoveAt(i);
        }
    }

    int ColumnCollection::IndexOf_System_Collections_IList(System::Object^ value)
    {
        return this->IndexOf((Ntreev::Windows::Forms::Grid::Column^)value);
    }

    void ColumnCollection::Insert_System_Collections_IList(int index, System::Object^ value)
    {
        this->Insert(index, (Ntreev::Windows::Forms::Grid::Column^)value);
    }

    void ColumnCollection::Remove_System_Collections_IList(System::Object^ value)
    {
        this->Remove((Ntreev::Windows::Forms::Grid::Column^)value);
    }

    void ColumnCollection::CopyTo_System_Collections_ICollection(System::Array^ array, int index)
    {
        for each(Ntreev::Windows::Forms::Grid::Column^ item in this)
        {
            array->SetValue(item, index++);
        }
    }

    System::Object^ ColumnCollection::default_System_Collections_IList::get(int index)
    {
        return this[index];
    }

    void ColumnCollection::default_System_Collections_IList::set(int /*index*/, System::Object^ /*value*/)
    {

    }

    bool ColumnCollection::IsReadOnly_System_Collections_IList::get()
    {
        return false;
    }

    bool ColumnCollection::IsFixedSize_System_Collections_IList::get()
    {
        return false;
    }

    bool ColumnCollection::IsSynchronized_System_Collections_ICollection::get()
    {
        return true;
    }

    System::Object^ ColumnCollection::SyncRoot_System_Collections_ICollection::get()
    {
        return this;
    }

    int ColumnCollection::Count_System_Collections_ICollection::get()
    {
        return this->Count;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/