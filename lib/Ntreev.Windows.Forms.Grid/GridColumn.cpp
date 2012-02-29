//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
#include "GridColumn.h"
#include "GridControl.h"
#include "GridCell.h"
#include "GridRow.h"
#include "GridCollection.h"
#include "GridColumnCollection.h"
#include "GridRowCollection.h"
#include "GridResource.h"
#include "GridAttributes.h"
#include "GridNativeClass.h"
#include "GridListBox.h"
#include "GridTextBox.h"
#include "GridFlagControl.h"

#include <vcclr.h>

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    class CustomPaint
    {
    public:
        CustomPaint(gcroot<Column^> column) : m_column(column)
        {

        }

        static bool	ColumnBackgroundPaint(GrGridPainter* pPainter, const GrColumn* pColumn, const GrRect& paintRect, void* dwUserData)
        {
            CustomPaint* pCustomPainter = (CustomPaint*)dwUserData;
            return pCustomPainter->OnColumnBackGroundPaint(pPainter, pColumn, paintRect);
        }

        static bool	ColumnContentsPaint(GrGridPainter* pPainter, const GrColumn* pColumn, const GrRect& paintRect, void* dwUserData)
        {
            CustomPaint* pCustomPainter = (CustomPaint*)dwUserData;
            return pCustomPainter->OnColumnContentsPaint(pPainter, pColumn, paintRect);
        }

        bool OnColumnBackGroundPaint(GrGridPainter* pPainter, const GrColumn* /*pColumn*/, const GrRect& paintRect)
        {
            System::IntPtr hdc(pPainter->GetDevice());
            System::Drawing::Graphics^ g = System::Drawing::Graphics::FromHdc(hdc);
            Column^ column = m_column;
            ColumnPainter^ columnPainter = column->ColumnPainter;
            bool b = columnPainter->PaintBackground(g, paintRect, column);
            delete g;
            pPainter->ReleaseDevice(hdc.ToPointer());
            return b;
        }

        bool OnColumnContentsPaint(GrGridPainter* pPainter, const GrColumn* /*pColumn*/, const GrRect& paintRect)
        {
            System::IntPtr hdc(pPainter->GetDevice());
            System::Drawing::Graphics^ g = System::Drawing::Graphics::FromHdc(hdc);
            Column^ column = m_column;
            ColumnPainter^ columnPainter = column->ColumnPainter;
            bool b = columnPainter->PaintContents(g, paintRect, column);
            delete g;
            pPainter->ReleaseDevice(hdc.ToPointer());
            return b;
        }

    private:
        gcroot<Column^>			m_column;
    };

    ColumnPainter::ColumnPainter()
    {
        using namespace System::Windows::Forms::VisualStyles;

        try
        {
            m_sortedDownRenderer = gcnew VisualStyleRenderer(VisualStyleElement::Header::SortArrow::SortedDown);
        }
        catch(System::Exception^)
        {

        }

        try
        {
            m_sortedUpPainter = gcnew VisualStyleRenderer(VisualStyleElement::Header::SortArrow::SortedUp);
        }
        catch(System::Exception^)
        {

        }
    }

    void ColumnPainter::DrawSortArrow(System::Drawing::Graphics^ g, System::Drawing::Rectangle paintRect, SortType sortType)
    {
        using namespace System::Windows::Forms;

        switch(sortType)
        {
        case SortType::Up:
            if(m_sortedUpPainter)
                m_sortedUpPainter->DrawBackground(g, paintRect);
            else
                ControlPaint::DrawScrollButton(g, paintRect, ScrollButton::Up, ButtonState::Flat);
            break;
        case SortType::Down:
            if(m_sortedDownRenderer)
                m_sortedDownRenderer->DrawBackground(g, paintRect);
            else
                ControlPaint::DrawScrollButton(g, paintRect, ScrollButton::Down, ButtonState::Flat);
            break;
        }
    }

    bool RowComparerUp(GrGridCore* /*pGridCore*/, const GrDataRow* pDataRow1, const GrDataRow* pDataRow2, const GrColumn* pColumn)
    {
        Cell^ cell1 = Cell::FromNative(pDataRow1->GetItem(pColumn));
        Cell^ cell2 = Cell::FromNative(pDataRow2->GetItem(pColumn));

        System::Object^ value1 = cell1->Value;
        System::Object^ value2 = cell2->Value;

        if(value1 == System::DBNull::Value)
            value1 = nullptr;
        if(value2 == System::DBNull::Value)
            value2 = nullptr;

        if(value1 != nullptr && value2 != nullptr)
        {
            int result;
            System::Collections::IComparer^ comparer = cell1->Column->SortComparer;
            if(comparer != nullptr)
            {
                result = comparer->Compare(value1, value2);
            }
            else
            {
                System::IComparable^ comparable = dynamic_cast<System::IComparable^>(value1);
                if(comparable != nullptr)
                    result = comparable->CompareTo(value2);
                else
                    result = value1->ToString()->CompareTo(value2->ToString());
            }

            if(result == 0)
                return pDataRow1->GetDataRowIndex() < pDataRow2->GetDataRowIndex();

            return result < 0;
        }
        else if(value1 == nullptr)
        {
            if(value2 == nullptr)
                return pDataRow1->GetDataRowIndex() < pDataRow2->GetDataRowIndex();
            return true;
        }

        return false;
    }

    bool RowComparerDown(GrGridCore* pGridCore, const GrDataRow* pDataRow1, const GrDataRow* pDataRow2, const GrColumn* pColumn)
    {
        return RowComparerUp(pGridCore, pDataRow2, pDataRow1, pColumn);
    }

    Column::Column()
        : m_pColumn(new GrColumn()), CellBase(m_pColumn)
    {
        m_pColumn->SetSortComparer(GrSort_Up, RowComparerUp);
        m_pColumn->SetSortComparer(GrSort_Down, RowComparerDown);

        m_pCustomPaint = new CustomPaint(this);
        m_pColumn->ManagedRef = this;

        SetEditStyleToNative();
    }

    Column::~Column()
    {
        Lock(this);

        if((m_site != nullptr) && (m_site->Container != nullptr))
        {
            m_site->Container->Remove(this);
        }

        this->Disposed_IComponent(this, System::EventArgs::Empty);

        if(m_pColumn != nullptr)
        {
            if(m_pColumn->GetIndex() != INVALID_INDEX)
            {
                GrColumnList* pColumnList = m_pColumn->GetColumnList();
                pColumnList->RemoveColumn(m_pColumn);
            }
            m_pColumn->ManagedRef = nullptr;
            m_pColumn = nullptr;
        }
    }

    System::String^ Column::ToString()
    {
        return this->ColumnName;
    }

    System::String^ Column::Title::get()
    {
        if(m_title == nullptr)
            return this->ColumnName;
        return m_title;
    }

    void Column::Title::set(System::String^ value)
    {
        m_title = value;
        AsyncDisplayText();
    }

    System::String^ Column::ColumnName::get()
    {
        if(m_propertyDescriptor != nullptr)
            return m_propertyDescriptor->Name;

        if(m_name == nullptr)
            return System::String::Empty;

        return m_name;
    }

    void Column::ColumnName::set(System::String^ value)
    {
        if(value == m_name)
            return;

        if(m_propertyDescriptor != nullptr)
            throw gcnew System::InvalidOperationException("바인딩된 컬럼에는 이름을 설정할 수 없습니다.");

        if(value == nullptr || value == System::String::Empty)
            throw gcnew System::ArgumentException("빈 문자열의 이름은 설정할 수 없습니다.");

        if(GridControl != nullptr)
        {
            if(GridControl->Columns[value] != nullptr)
                throw gcnew System::ArgumentException(System::String::Format("\"{0}\"의 이름을 가진 항목이 이미 있습니다.", value));
        }

        m_name = value;

        if(m_title == nullptr)
        {
            this->Title = nullptr;
        }
    }

    System::String^ Column::Tooltip::get()
    {
        if(m_tooltip == nullptr)
        {
            if(m_propertyDescriptor != nullptr)
                return m_propertyDescriptor->Description;
            return System::String::Empty;
        }
        return m_tooltip;
    }

    void Column::Tooltip::set(System::String^ value)
    {
        if(value == nullptr)
            value = System::String::Empty;
        m_tooltip = value;
        m_pColumn->SetTooltip(ToNativeString::Convert(value));
    }

    System::ComponentModel::PropertyDescriptor^ Column::PropertyDescriptor::get()
    {
        return m_propertyDescriptor;
    }

    void Column::propertyDescriptor_ValueChanged(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
    {
        AsyncDisplayText();
    }

    void Column::PropertyDescriptor::set(System::ComponentModel::PropertyDescriptor^ value)
    {
        if(value != nullptr)
        {
            if(m_defaultValue == nullptr)
            {
                System::ComponentModel::DefaultValueAttribute^ attr = dynamic_cast<System::ComponentModel::DefaultValueAttribute^>(value->Attributes[System::ComponentModel::DefaultValueAttribute::typeid]);
                if(attr != nullptr)
                {
                    m_defaultValue = attr->Value;
                }
                else if(value->PropertyType->IsEnum == true)
                {
                    System::Array^ values = System::Enum::GetValues(value->PropertyType);
                    if(values->Length > 0)
                        m_defaultValue = values->GetValue(0);
                }
                else if(value->PropertyType->IsValueType == true)
                {
                    m_defaultValue = System::Activator::CreateInstance(value->PropertyType);
                }
            }

            FrozenAttribute^ frozenAttr = dynamic_cast<FrozenAttribute^>(value->Attributes[FrozenAttribute::typeid]);
            if(frozenAttr != nullptr)
                this->IsFrozen = frozenAttr->Frozen;

            MovableAttribute^ movableAttr = dynamic_cast<MovableAttribute^>(value->Attributes[MovableAttribute::typeid]);
            if(movableAttr != nullptr)
                this->IsMovable = movableAttr->IsMovable;

            ResizableAttribute^ resizableAttr = dynamic_cast<ResizableAttribute^>(value->Attributes[ResizableAttribute::typeid]);
            if(resizableAttr != nullptr)
                this->IsResizable = resizableAttr->Resizable;

            SortableAttribute^ sortableAttr = dynamic_cast<SortableAttribute^>(value->Attributes[SortableAttribute::typeid]);
            if(sortableAttr != nullptr)
                this->CanBeSorted = sortableAttr->Sortable;

            m_typeEditor = Design::TypeEditor::FromDataType(value->PropertyType);

            switch(this->View)
            {
            case ViewType::Text:
                m_pColumn->SetItemTextVisible(true);
                m_pColumn->SetItemIcon(false);
                break;
            case ViewType::Icon:
                m_pColumn->SetItemTextVisible(true);
                m_pColumn->SetItemIcon(true);
                break;
            case ViewType::Custom:
                m_pColumn->SetItemTextVisible(false);
                m_pColumn->SetItemIcon(false);
                break;
            }
            SetEditStyleToNative();
        }
        else
        {
            m_typeEditor = nullptr;
        }

        m_propertyDescriptor = value;

        m_pColumn->SetReadOnly(this->IsReadOnly);
        m_pColumn->SetVisible(this->IsVisible);

        AsyncDisplayText();
    }

    int Column::Width::get()
    {
        return CellBase::Width::get();
    }

    void Column::Width::set(int width)
    {
        if(width < 0)
            throw gcnew System::ArgumentOutOfRangeException("width", "width는 0보다 커야 합니다.");

        m_pColumn->SetWidth(width);
    }

    int Column::MinWidth::get()
    {
        return m_pColumn->GetMinWidth();
    }

    void Column::MinWidth::set(int width)
    {
        if(width < 0)
            throw gcnew System::ArgumentOutOfRangeException("width", "width는 0보다 커야 합니다.");
        if(width > MaxWidth)
            throw gcnew System::ArgumentOutOfRangeException("width", "width가 최대 너비보다 큽니다.");
        m_pColumn->SetMinWidth(width);
    }

    int Column::MaxWidth::get()
    {
        return m_pColumn->GetMaxWidth();
    }

    void Column::MaxWidth::set(int width)
    {
        if(width < 0)
            throw gcnew System::ArgumentOutOfRangeException("width", "width는 0보다 커야 합니다.");
        if(width < MinWidth)
            throw gcnew System::ArgumentOutOfRangeException("width", "width가 최소 너비보다 작습니다.");
        m_pColumn->SetMaxWidth(width);
    }

    bool Column::IsVisible::get()
    {
        if(m_propertyDescriptor != nullptr && m_propertyDescriptor->IsBrowsable == false)
            return false;
        return m_pColumn->GetVisible();
    }

    void Column::IsVisible::set(bool value)
    {
        m_pColumn->SetVisible(value);
    }

    bool Column::IsMovable::get()
    {
        return m_pColumn->GetMovable();
    }

    void Column::IsMovable::set(bool value)
    {
        m_pColumn->SetMovable(value);
    }

    bool Column::IsResizable::get()
    {
        return m_pColumn->GetResizable();
    }

    void Column::IsResizable::set(bool value)
    {
        m_pColumn->SetResizable(value);
    }

    bool Column::IsFrozen::get()
    {
        return m_pColumn->GetFrozen();
    }

    void Column::IsFrozen::set(bool value)
    {
        m_pColumn->SetFrozen(value);
    }

    bool Column::IsReadOnly::get()
    {
        if(m_propertyDescriptor != nullptr && m_propertyDescriptor->IsReadOnly == true)
            return true;

        return m_pColumn->GetReadOnly();
    }

    void Column::IsReadOnly::set(bool value)
    {
        if(m_propertyDescriptor != nullptr && m_propertyDescriptor->IsReadOnly == true)
        {
            throw gcnew System::InvalidOperationException("읽기 전용으로 바인딩된 컬럼에는 설정할 수 없습니다.");
        }
        m_pColumn->SetReadOnly(value);
    }

    bool Column::CanBeSorted::get()
    {
        return m_pColumn->CanBeSort();
    }

    void Column::CanBeSorted::set(bool value)
    {
        m_pColumn->EnableSort(value);
    }

    System::Type^ Column::DataType::get()
    {
        if(m_dataType == nullptr)
        {
            if(m_propertyDescriptor != nullptr)
                return m_propertyDescriptor->PropertyType;
            return System::String::typeid;
        }
        return m_dataType;
    }

    void Column::DataType::set(System::Type^ value)
    {
        using namespace System::ComponentModel;
        using namespace System::Drawing::Design;

        if(value == nullptr)
            value = System::String::typeid;

        if(m_dataType == value)
            return;

        if(m_propertyDescriptor != nullptr)
        {
            System::ComponentModel::TypeConverter^ converter = TypeDescriptor::GetConverter(value);

            if(converter->CanConvertFrom(m_propertyDescriptor->PropertyType) == false)
            {
                System::Text::StringBuilder^ builder = gcnew System::Text::StringBuilder();

                builder->AppendLine(System::String::Format("데이터소스에서 데이터 타입을 변환할 수가 없습니다. 새로운 TypeConverter를 구현하세요"));
                builder->AppendLine(System::String::Format("    DataSource  : {0}", m_propertyDescriptor->PropertyType));
                builder->AppendLine(System::String::Format("    GridControl : {0}", value));

                throw gcnew System::NotSupportedException(builder->ToString());
            }
        }

        m_typeEditor = Design::TypeEditor::FromDataType(value);
        switch(this->View)
        {
        case ViewType::Text:
            m_pColumn->SetItemTextVisible(true);
            m_pColumn->SetItemIcon(false);
            break;
        case ViewType::Icon:
            m_pColumn->SetItemTextVisible(true);
            m_pColumn->SetItemIcon(true);
            break;
        case ViewType::Custom:
            m_pColumn->SetItemTextVisible(false);
            m_pColumn->SetItemIcon(false);
            break;
        }
        m_dataType = value;
        SetEditStyleToNative();
    }

    System::ComponentModel::TypeConverter^ Column::TypeConverter::get()
    {
        if(m_typeConverter == nullptr)
        {
            if(m_propertyDescriptor != nullptr)
                return m_propertyDescriptor->Converter;
            return System::ComponentModel::TypeDescriptor::GetConverter(this->DataType);
        }

        return m_typeConverter;
    }

    void Column::TypeConverter::set(System::ComponentModel::TypeConverter^ converter)
    {
        m_typeConverter = converter;
    }

    Ntreev::Windows::Forms::Grid::SortType Column::SortType::get()
    {
        return (Ntreev::Windows::Forms::Grid::SortType)m_pColumn->GetSortType();	
    }

    void Column::SortType::set(Ntreev::Windows::Forms::Grid::SortType value)
    {
        m_pColumn->SetSortType((GrSort)value);
    }

    System::Collections::IComparer^ Column::SortComparer::get()
    {
        return m_sortComparer;
    }

    void Column::SortComparer::set(System::Collections::IComparer^ value)
    {
        m_sortComparer = value;
    }

    int Column::VisibleIndex::get()
    {
        return (int)m_pColumn->GetVisibleIndex();
    }

    int Column::DisplayIndex::get()
    {
        return (int)m_pColumn->GetDisplayIndex();	
    }

    int Column::Index::get()
    {
        return (int)m_pColumn->GetIndex();
    }

    System::Object^ Column::DefaultValue::get()
    {
        if(m_defaultValue == nullptr || m_defaultValue == System::DBNull::Value)
            return m_defaultValue;

        if(m_defaultValue->GetType() == this->DataType)
            return m_defaultValue;

        if(this->TypeConverter->CanConvertFrom(m_defaultValue->GetType()) == false)
            return nullptr;
        return this->TypeConverter->ConvertFrom(m_defaultValue);
    }

    void Column::DefaultValue::set(System::Object^ value)
    {
        m_defaultValue = value;
    }

    System::Drawing::StringAlignment Column::CellAlignment::get()
    {
        GrHorzAlign align = m_pColumn->GetItemHorzAlign();
        return (System::Drawing::StringAlignment)align;
    }

    void Column::CellAlignment::set(System::Drawing::StringAlignment value)
    {
        m_pColumn->SetItemHorzAlign((GrHorzAlign)value);
    }

    System::Drawing::StringAlignment Column::CellLineAlignment::get()
    {
        GrVertAlign align = m_pColumn->GetItemVertAlign();
        return (System::Drawing::StringAlignment)align;
    }

    void Column::CellLineAlignment::set(System::Drawing::StringAlignment value)
    {
        m_pColumn->SetItemVertAlign((GrVertAlign)value);
    }

    bool Column::IsSelected::get()
    {
        return m_pColumn->GetSelected();
    }

    void Column::IsSelected::set(bool value)
    {
        m_pColumn->SetSelected(value);
    }

    bool Column::IsFullSelected::get()
    {
        return m_pColumn->GetFullSelected();	
    }

    bool Column::IsGrouped::get()
    {
        return m_pColumn->GetGrouped();
    }

    void Column::IsGrouped::set(bool value)
    {
        m_pColumn->SetGrouped(value);
    }

    bool Column::CanBeGrouping::get()
    {
        return m_pColumn->CanBeGrouping();
    }

    void Column::CanBeGrouping::set(bool value)
    {
        m_pColumn->EnableGrouping(value);
    }

    Ntreev::Windows::Forms::Grid::ColumnPainter^ Column::ColumnPainter::get()
    {
        return m_columnPainter;
    }

    bool Column::CellWordWrap::get()
    {
        return m_pColumn->GetItemWordWrap();
    }

    void Column::CellWordWrap::set(bool value)
    {
        m_pColumn->SetItemWordWrap(value);
    }

    bool Column::CellMultiline::get()
    {
        return m_pColumn->GetItemMultiline();
    }

    void Column::CellMultiline::set(bool value)
    {
        m_pColumn->SetItemMultiline(value);
    }

    Ntreev::Windows::Forms::Grid::ColumnState Column::ColumnState::get()
    {
        if(m_pColumn->GetMousePressed() == true)
            return Ntreev::Windows::Forms::Grid::ColumnState::Pressed;
        else if(m_pColumn->GetMouseOvered() == true)
            return Ntreev::Windows::Forms::Grid::ColumnState::Hot;
        return Ntreev::Windows::Forms::Grid::ColumnState::Normal;
    }

    void Column::ColumnPainter::set(Ntreev::Windows::Forms::Grid::ColumnPainter^ columnPainter)
    {
        m_columnPainter = columnPainter;
        if(m_columnPainter)
        {
            m_pColumn->m_fnColumnBackgroundPaint	= CustomPaint::ColumnBackgroundPaint;
            m_pColumn->m_fnColumnContentsPaint		= CustomPaint::ColumnContentsPaint;
            m_pColumn->m_pColumnPaintData			= (void*)m_pCustomPaint;
        }
        else
        {
            m_pColumn->m_fnColumnBackgroundPaint	= nullptr;
            m_pColumn->m_fnColumnContentsPaint		= nullptr;
            m_pColumn->m_pColumnPaintData			= nullptr;
        }
    }

    System::Drawing::Color Column::CellForeColor::get()
    {
        return m_pColumn->GetItemForeColor();
    }

    void Column::CellForeColor::set(System::Drawing::Color value)
    {
        m_pColumn->SetItemForeColor(value);
    }

    System::Drawing::Color Column::CellBackColor::get()
    {
        return m_pColumn->GetItemBackColor();
    }

    void Column::CellBackColor::set(System::Drawing::Color value)
    {
        m_pColumn->SetItemBackColor(value);
    }

    System::Windows::Forms::Padding Column::CellPadding::get()
    {
        return m_pColumn->GetItemPadding();
    }

    void Column::CellPadding::set(System::Windows::Forms::Padding value)
    {
        m_pColumn->SetItemPadding(value);
    }

    System::Drawing::Font^ Column::CellFont::get()
    {
        GrFont* pFont = m_pColumn->GetItemFont();
        if(pFont == nullptr)
            return nullptr;

        return Native::WinFormFontManager::ToManagedFont(pFont);
    }

    void Column::CellFont::set(System::Drawing::Font^ value)
    {
        if(value == nullptr)
            m_pColumn->SetItemFont(nullptr);
        else
            m_pColumn->SetItemFont(Native::WinFormFontManager::FromManagedFont(value));
    }

    int Column::CellMinHeight::get()
    {
        return m_pColumn->GetItemMinHeight();
    }

    void Column::CellMinHeight::set(int value)
    {
        m_pColumn->SetItemMinHeight(value);
    }

    Ntreev::Windows::Forms::Grid::ClickEditType Column::ClickEditType::get()
    {
        return (Ntreev::Windows::Forms::Grid::ClickEditType)m_pColumn->GetItemClickEditing();
    }

    void Column::ClickEditType::set(Ntreev::Windows::Forms::Grid::ClickEditType value)
    {
        m_pColumn->SetItemClickEditing((GrClickEditing)value);
    }

    ViewType Column::View::get()
    {
        if(m_typeEditor == nullptr)
            return ViewType::Text;
        return m_typeEditor->View;
    }

    void Column::RefreshAll()
    {
        for each(Row^ item in GridControl->Rows)
        {
            Cell^ cell = item->Cells[this];
            cell->UpdateNativeText();
        }
    }

    void Column::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, ICell^ cell, System::Object^ value)
    {
        m_typeEditor->PaintValue(graphics, paintRect, cell, value);
    }

    void Column::ResetCellBackColor()
    {
        this->CellBackColor = System::Drawing::Color::Empty;
    }

    void Column::ResetCellPadding()
    {
        this->CellPadding = GrPadding::Default;
    }

    void Column::ResetCellForeColor()
    {
        this->CellForeColor = System::Drawing::Color::Empty;
    }

    void Column::ResetCellFont()
    {
        m_pColumn->SetItemFont(nullptr);
    }

    Column^ Column::FromNative(const GrColumn* pColumn)
    {
        if(pColumn == nullptr)
            return nullptr;
        System::Object^ ref = pColumn->ManagedRef;
        return safe_cast<Column^>(ref);
    }

    void Column::OnGridControlAttachedInternal()
    {
        OnGridControlAttached();
    }

    void Column::OnGridControlDetachedInternal()
    {
        OnGridControlDetached();
    }

    System::Object^ Column::EditValueInternal(Design::IEditorService^ editorService, ICell^ cell, System::Object^ value)
    {
        return EditValue(editorService, cell, value);
    }

    System::Object^ Column::EditValue(Design::IEditorService^ editorService, ICell^ cell, System::Object^ value)
    {
        return TypeEditor->EditValue(editorService, cell, value);
    }

    bool Column::CanEdit(ICell^ cell, Design::EditingReason reason)
    {
        return TypeEditor->CanEdit(cell, reason);
    }

    bool Column::CanEditInternal(ICell^ cell, Design::EditingReason reason)
    {
        return CanEdit(cell, reason);
    }

    Design::EditStyle Column::GetEditStyle()
    {
        return this->TypeEditor->GetEditStyle();
    }

    uint Column::ColumnID::get()
    {
        return m_pColumn->GetColumnID();	
    }

    void Column::ValidateDefaultValue()
    {
        if(m_dataType == nullptr || m_defaultValue == nullptr)
            return;
    }

    bool Column::ShouldSerializeCellForeColor()
    {
        return m_pColumn->GetItemForeColor() != GrColor::Empty;
    }

    bool Column::ShouldSerializeCellBackColor()
    {
        return m_pColumn->GetItemBackColor() != GrColor::Empty;
    }

    bool Column::ShouldSerializeCellPadding()
    {
        return m_pColumn->GetItemPadding() != GrPadding::Default;
    }

    bool Column::ShouldSerializeCellFont()
    {
        return m_pColumn->GetItemFont() != nullptr;
    }

    bool Column::ShouldSerializeTitle()
    {
        return this->Title != this->ColumnName;
    }

    bool Column::ShouldSerializeDataType()
    {
        if(m_dataType == nullptr)
            return false;

        if(m_propertyDescriptor == nullptr)
        {
            return m_dataType != System::String::typeid;
        }
        return m_dataType != m_propertyDescriptor->PropertyType;
    }

    bool Column::ShouldSerializeTypeConverter()
    {
        if(m_propertyDescriptor == nullptr)
        {
            return System::ComponentModel::TypeDescriptor::GetConverter(this->DataType) != m_typeConverter;
        }
        return m_typeConverter != m_propertyDescriptor->Converter;
    }

    void Column::AsyncDisplayText()
    {
        m_pColumn->SetText(ToNativeString::Convert(this->Title));
    }

    bool Column::CanConvertFrom(System::Type^ sourceType)
    {
        if(m_dataType == nullptr)
            return true;
        return this->TypeConverter->CanConvertFrom(sourceType);
    }

    System::Object^ Column::ConvertFromSource(System::Object^ value)
    {
        if(value == nullptr || value == System::DBNull::Value)
            return value;

        if(m_propertyDescriptor == nullptr)
            return value;

        if(m_propertyDescriptor->PropertyType == this->DataType)
            return value;

        System::ComponentModel::TypeConverter^ typeConverter = nullptr;

        if(m_typeConverter == nullptr)
        {
            typeConverter = System::ComponentModel::TypeDescriptor::GetConverter(this->DataType);
        }
        else
        {
            typeConverter = m_typeConverter;
        }

        return typeConverter->ConvertFrom(value);
    }

    System::Object^ Column::ConvertToSource(System::Object^ value)
    {
        if(value == nullptr || value == System::DBNull::Value)
            return value;

        if(m_propertyDescriptor == nullptr)
            return value;

        if(m_propertyDescriptor->PropertyType == this->DataType)
            return value;

        System::ComponentModel::TypeConverter^ typeConverter = nullptr;

        if(m_typeConverter == nullptr)
        {
            typeConverter = System::ComponentModel::TypeDescriptor::GetConverter(this->DataType);
        }
        else
        {
            typeConverter = m_typeConverter;
        }

        return typeConverter->ConvertTo(value, m_propertyDescriptor->PropertyType);
    }

    void Column::Site_IComponent::set(System::ComponentModel::ISite^ value)
    {
        m_site = value;
    }

    Design::TypeEditor^ Column::TypeEditor::get()
    {
        if(m_typeEditor == nullptr)
            return %Design::StringTypeEditor::Default;
        return m_typeEditor;
    }

    void Column::SetEditStyleToNative()
    {
        switch(GetEditStyle())
        {
        case Design::EditStyle::Control:
            m_pColumn->SetItemType(GrItemType_Control);
            break;
        case Design::EditStyle::DropDown:
            m_pColumn->SetItemType(GrItemType_DropDown);
            break;
        case Design::EditStyle::Modal:
            m_pColumn->SetItemType(GrItemType_Modal);
            break;
        }
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/