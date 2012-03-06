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


#include "stdafx.h"
#include "GridControl.h"
#include "GridColumn.h"

#include "GridCell.h"
#include "GridRow.h"
#include "GridCaption.h"
#include "GridGroupingRow.h"
#include "GridEvent.h"
#include "GridResource.h"
#include "GridCollection.h"
#include "GridRowCollection.h"
#include "GridColumnCollection.h"
#include "GridWin32.h"
#include "GridTimeTest.h"
#include "GridColumnExtension.h"

// Private include
#include "About.h"
#include "GridTooltip.h"
#include "GridErrorDescriptor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    GridControl::GridControl()
    {
#ifdef _TIME_TEST
        GridTimeTest timeTest("GridControl 생성자");
#endif

        m_pGridWindow = new Native::WinFormWindow(this);
        m_pGridCore = new Native::WinFormGridCore(this, m_pGridWindow);
        m_pGridPainter = m_pGridWindow->GetGridPainter();

        m_pColumnList = m_pGridCore->GetColumnList();
        m_pDataRowList = m_pGridCore->GetDataRowList();
        m_pInsertionRow = m_pGridCore->GetInsertionRow();

        m_pGridCore->SetDisplayRect(this->DisplayRectangle);

#ifdef _TIME_TEST
        Rendering   = true;
#endif

        m_dataSource = nullptr;
        m_dataMember = System::String::Empty;

        m_tooltips = gcnew Ntreev::Windows::Forms::Grid::ToolTip(this, 3);
        m_errorDescriptor = gcnew Ntreev::Windows::Forms::Grid::ErrorDescriptor(this);

        m_columnList = gcnew ColumnCollection(this);
        m_visibleColumnList = gcnew VisibleColumnCollection(this);
        m_displayableColumnList = gcnew DisplayableColumnCollection(this);
        m_frozenColumnList = gcnew FrozenColumnCollection(this);
        m_unfrozenColumnList = gcnew UnfrozenColumnCollection(this);
        m_rowList = gcnew RowCollection(this);
        m_visibleRowList = gcnew VisibleRowCollection(this);
        m_displayableRowList = gcnew DisplayableRowCollection(this);
        m_style = gcnew Design::Style();

        m_selectedColumns = gcnew SelectedColumnCollection(this, Selector->GetSelectedColumns());
        m_selectedRows = gcnew SelectedRowCollection(this, Selector->GetSelectedRows());

        m_captionRow = gcnew Ntreev::Windows::Forms::Grid::CaptionRow(this, m_pGridCore->GetCaptionRow());
        m_groupingRow = gcnew Ntreev::Windows::Forms::Grid::GroupingRow(this, m_pGridCore->GetGroupingList());
        m_insertionRow = gcnew Ntreev::Windows::Forms::Grid::InsertionRow(this, m_pGridCore->GetInsertionRow());

        m_defaultDataSource = gcnew System::Data::DataTable();
        m_defaultManager = dynamic_cast<System::Windows::Forms::CurrencyManager^>(this->BindingContext[m_defaultDataSource]);
        m_listChangedEventHandler = gcnew System::ComponentModel::ListChangedEventHandler(this, &GridControl::currencyManager_ListChanged);
        m_bindingCompleteEventHandler = gcnew System::Windows::Forms::BindingCompleteEventHandler(this, &GridControl::currencyManager_BindingComplete);

        this->DoubleBuffered = true;
        this->Name = L"GridControl";

        OnCurrencyManagerChanging(gcnew CurrencyManagerChangingEventArgs(m_defaultManager));
        OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(m_defaultManager));
    }

    GridControl::~GridControl()
    {
        if(m_pGridCore == nullptr)
            return;

        OnCurrencyManagerChanging(gcnew CurrencyManagerChangingEventArgs(nullptr));
        OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(nullptr));

        this->Columns->DeleteAll();

        delete m_rowList;

        if(m_pGridCore != nullptr)
        {
            delete m_pGridCore;
            m_pGridCore = nullptr;
        }
        if(m_pGridPainter != nullptr)
        {
            delete m_pGridPainter;
            m_pGridPainter = nullptr;
        }
    }

    bool GridControl::ProcessTabKey(bool /*forward*/)
    {
        return true;
    }


#ifdef _DEBUG
    void GridControl::OnInvalidated(System::Windows::Forms::InvalidateEventArgs^ e)
    {
        UserControl::OnInvalidated(e);
        System::Diagnostics::Debug::WriteLine(e->InvalidRect);
    }

    void GridControl::OnCursorChanged(System::EventArgs^ e)
    {
        UserControl::OnCursorChanged(e);
        System::Diagnostics::Debug::WriteLine("OnCursorChanged");
    }
#endif

    void GridControl::OnGotFocus(System::EventArgs^ e)
    {
        m_pGridWindow->OnGotFocus();
        UserControl::OnGotFocus(e);
    }

    void GridControl::OnLostFocus(System::EventArgs^ e)
    {
        m_pGridWindow->OnLostFocus();
        UserControl::OnLostFocus(e);
    }

    void GridControl::OnLayout(System::Windows::Forms::LayoutEventArgs^ e)
    {
        UserControl::OnLayout(e);
        m_pGridWindow->OnSizeChanged(this->DisplayRectangle);
        m_pGridCore->Update();
    }

    void GridControl::OnPrint(System::Windows::Forms::PaintEventArgs^ e)
    {
        OnPaint(e);
    }

    void GridControl::OnPaint(System::Windows::Forms::PaintEventArgs^ e)
    {
        try
        {
            GrRect clipping(e->ClipRectangle);

            clipping.right  = System::Math::Min(clipping.right, this->DisplayRectangle.Right);
            clipping.bottom = System::Math::Min(clipping.bottom, this->DisplayRectangle.Bottom); 

            System::IntPtr hdc = e->Graphics->GetHdc();
            try
            {
                m_pGridWindow->OnSizeChanged(this->DisplayRectangle);
                m_pGridCore->Update();
                m_pGridWindow->OnPaint(hdc.ToPointer(), clipping);
            }
            catch(System::Exception^ exception)
            {
                throw exception;
            }
            finally
            {
                e->Graphics->ReleaseHdc(hdc);
            }

            UserControl::OnPaint(e);
        }
        catch(System::Exception^ exception)
        {
            using namespace System::Drawing;
            e->Graphics->Clear(Color::White);

            StringFormat^ format = gcnew StringFormat(StringFormat::GenericDefault);

            PointF location((PointF)this->DisplayRectangle.Location);
            SizeF size = e->Graphics->MeasureString(exception->Message, this->Font, this->DisplayRectangle.Width, format);

            e->Graphics->DrawString(exception->Message, this->Font, Brushes::Black, RectangleF(location, size), format);

            size = e->Graphics->MeasureString(exception->StackTrace, this->Font, this->DisplayRectangle.Width, format);
            location.Y += size.Height;
            e->Graphics->DrawString(exception->StackTrace, this->Font, Brushes::Black, RectangleF(location, size), format);
        }
    }

    void GridControl::PostPaint(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle clipRectangle)
    {
        PaintColumnControls(graphics, clipRectangle);

        if(IsRowNumberVisible == false)
            PaintRowState(graphics);

        m_errorDescriptor->Paint(graphics);
    }

    void GridControl::PaintColumnControls(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle clipRectangle)
    {
        for(uint i=0 ; i<m_pColumnList->GetDisplayableColumnCount() ; i++)
        {
            GrColumn* pColumn = m_pColumnList->GetDisplayableColumn(i);

            int x = pColumn->GetX();
            int r = x + pColumn->GetWidth();

            if(x >= clipRectangle.Right || r < clipRectangle.Left)
                continue;

            Column^ column = m_columnList[pColumn];
            Ntreev::Windows::Forms::Grid::ViewType viewType = column->ViewType;
            if(viewType == Ntreev::Windows::Forms::Grid::ViewType::Text)
                continue;

            for(uint j=0 ; j<m_pDataRowList->GetDisplayableRowCount() + 1 ; j++)
            {
                GrDataRow* pDataRow = nullptr;
                if(j==0)
                    pDataRow = m_pInsertionRow->GetVisible() == true ? m_pInsertionRow : nullptr;
                else
                    pDataRow = dynamic_cast<GrDataRow*>(m_pDataRowList->GetDisplayableRow(j - 1));

                if(pDataRow == nullptr)
                    continue;

                int y = pDataRow->GetY();
                int b = y + pDataRow->GetHeight();
                if(y >= clipRectangle.Bottom || b < clipRectangle.Top)
                    continue;

                GrItem* pItem = pDataRow->GetItem(pColumn);
                System::Drawing::Rectangle paintRect = pItem->GetClientRect();
                paintRect.Offset(pItem->GetLocation());

                if(viewType == Ntreev::Windows::Forms::Grid::ViewType::Icon)
                {
                    paintRect.Width = DEF_ICON_SIZE;
                    paintRect.X -= (DEF_ICON_SIZE + column->CellPadding.Left);
                }

                //if(pItem->GetControlVisible() == true)
                // paintRect.Width -= pItem->GetControlRect().GetWidth();

                Cell^ cell = Cell::FromNative(pItem);
                column->PaintValue(graphics, paintRect, cell, cell->Value);
            }
        }
    }

    void GridControl::PaintRowState(System::Drawing::Graphics^ g)
    {
        IDataRow* pFocusedRow = Focuser->GetFocusedRow();
        if(pFocusedRow == nullptr || pFocusedRow->GetDisplayable() == false)
            return;

        if(EditingCell != nullptr)
        {
            GrRect bound = EditingCell->NativeRef->GetDataRow()->GetRect();
            GrPoint center = bound.GetCenter();

            System::Drawing::Bitmap^ image = _Resources::RowEditing;
            center.x -= (image->Width / 2);
            center.y -= (image->Height / 2);
            g->DrawImageUnscaled(image, center.x, center.y);
        }
        else if(FocusedRow->IsEdited == true)
        {
            GrRect bound = pFocusedRow->GetRect();
            GrPoint center = bound.GetCenter();

            System::Drawing::Bitmap^ image = _Resources::RowEditing;
            center.x -= (image->Width / 2);
            center.y -= (image->Height / 2);
            g->DrawImageUnscaled(image, center.x, center.y);
        }
        else if(pFocusedRow->GetDisplayable() == true)
        {
            GrRect bound = pFocusedRow->GetRect();
            GrPoint center = bound.GetCenter();

            System::Drawing::Bitmap^ image = _Resources::RowFocused;
            if(pFocusedRow == m_pInsertionRow)
                image = _Resources::InsertionRowFocused;
            center.x -= (image->Width / 2);
            center.y -= (image->Height / 2);
            g->DrawImageUnscaled(image, center.x, center.y);
        }

        if(m_pInsertionRow->GetVisible() == true && m_pInsertionRow->HasFocused() == false)
        {
            GrRect bound = m_pInsertionRow->GetRect();
            GrPoint center = bound.GetCenter();

            System::Drawing::Bitmap^ image = _Resources::InsertionRow;
            center.x -= (image->Width / 2);
            center.y -= (image->Height / 2);
            g->DrawImageUnscaled(image, center.x, center.y);
        }
    }

    void GridControl::SetDataConnection(System::Object^ dataSource, System::String^ dataMember)
    {
        using namespace System::ComponentModel;

        DataBindingRef(this);

        if(m_dataSource == dataSource && m_dataMember == dataMember)
            return;

        System::Windows::Forms::CurrencyManager^ manager;

        if(dataSource == nullptr)
        {
            manager = m_defaultManager;
        }
        else
        {
            ISupportInitializeNotification^ support = dynamic_cast<ISupportInitializeNotification^>(dataSource);

            if(support != nullptr && support->IsInitialized == false)
            {
                m_dataMember = dataMember;
                support->Initialized += gcnew System::EventHandler(this, &GridControl::dataSource_Initialized);
                return;
            }

            manager = dynamic_cast<System::Windows::Forms::CurrencyManager^>(this->BindingContext[dataSource, dataMember]);

            if(manager == nullptr)
            {
                throw gcnew System::NotSupportedException("데이터 소스 초기화에 실패했습니다. 데이터 소스가 IList, IListSource 또는 IBindingList 인터페이스를 구현하는 개체인지 확인하십시오");
            }
            else if(support != nullptr)
            {
                support->Initialized -= gcnew System::EventHandler(this, &GridControl::dataSource_Initialized);
            }
        }

        CurrencyManagerChangingEventArgs e(manager);
        OnCurrencyManagerChanging(%e);

        if(e.Cancel == true)
        {
            throw gcnew System::NotSupportedException(e.CancelReason);
        }

        if(m_manager != manager)
            ClearCore(true);

        m_dataSource = dataSource;
        m_dataMember = dataMember;

        OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(manager));
        OnDataBindingComplete(System::EventArgs::Empty);

        m_pGridCore->Invalidate();

        //if(dataSource != nullptr && dataMember != System::String::Empty && manager == nullptr)
        //{
        // throw gcnew System::NotSupportedException("데이터 소스 초기화에 실패했습니다. 데이터 소스가 IList, IListSource 또는 IBindingList 인터페이스를 구현하는 개체인지 확인하십시오");
        //}
    }

    void GridControl::dataSource_Initialized(System::Object^ sender, System::EventArgs^ /*e*/)
    {
        using namespace System::ComponentModel;

        ISupportInitializeNotification^ support = dynamic_cast<ISupportInitializeNotification^>(sender);

        if(support->IsInitialized == true)
        {
            support->Initialized -= gcnew System::EventHandler(this, &GridControl::dataSource_Initialized);
            SetDataConnection(sender, m_dataMember);
        }
    }

    void GridControl::Update()
    {
        m_pGridCore->Update();
        UserControl::Update();
    }

    CellBase^ GridControl::GetAt(System::Drawing::Point pt)
    {
        GrHitTest hitTest;
        if(m_pGridCore->HitTest(pt, &hitTest) == false)
            return nullptr;

        System::Object^ e = hitTest.pHitted->ManagedRef;
        return safe_cast<CellBase^>(e);
    }

    Cell^ GridControl::GetCellAt(System::Drawing::Point pt)
    {
        GrHitTest hitTest;
        if(m_pGridCore->HitTest(pt, &hitTest) == false)
            return nullptr;
        GrItem* pItem = dynamic_cast<GrItem*>(hitTest.pHitted);
        if(pItem == nullptr)
            return nullptr;

        System::Object^ e = pItem->ManagedRef;
        return safe_cast<Cell^>(e);
    }

    void GridControl::WndProc(System::Windows::Forms::Message% m)
    {
        switch(m.Msg)
        {
        case Win32::WM::WM_HSCROLL:
            {
                Native::WinFormScroll* pScroll = (Native::WinFormScroll*)m_pGridCore->GetHorzScroll();
                pScroll->WndProc(m.HWnd, m.WParam);
            }
            return;
        case Win32::WM::WM_VSCROLL:
            {

                Native::WinFormScroll* pScroll = (Native::WinFormScroll*)m_pGridCore->GetVertScroll();
                pScroll->WndProc(m.HWnd, m.WParam);

            }
            return;
        }
        UserControl::WndProc(m); 
    }

    System::Object^ GridControl::GetInternalService(System::Type^ serviceType)
    {
        return UserControl::GetService(serviceType);
    }

    void GridControl::OnMouseLeave(System::EventArgs^ e)
    {
        m_pGridWindow->OnMouseLeave();
        UserControl::OnMouseLeave(e);
    }

    void GridControl::OnMouseMove(System::Windows::Forms::MouseEventArgs^ e)
    {
        m_pGridWindow->OnMouseMove(e->Location, e->Button == System::Windows::Forms::MouseButtons::Left);
        UserControl::OnMouseMove(e);
    }

    void GridControl::OnMouseDown(System::Windows::Forms::MouseEventArgs^ e)
    {
        //System::Diagnostics::Trace::WriteLine(System::String::Format("mouse down : {0}", System::DateTime::Now));
        m_buttonDownTime = System::DateTime::Now;
        UserControl::OnMouseDown(e);
        if(e->Button == System::Windows::Forms::MouseButtons::Left)
            m_pGridWindow->OnMouseDown(e->Location);
    }

    void GridControl::OnMouseUp(System::Windows::Forms::MouseEventArgs^ e)
    {
        //System::Diagnostics::Trace::WriteLine(System::String::Format("mouse up : {0}", System::DateTime::Now));
        UserControl::OnMouseUp(e);
        if(e->Button == System::Windows::Forms::MouseButtons::Left)
            m_pGridWindow->OnMouseUp(e->Location);
        //System::Diagnostics::Trace::WriteLine(System::String::Format("mouse up end: {0}", System::DateTime::Now));
    }

    void GridControl::OnMouseClick(System::Windows::Forms::MouseEventArgs^ e)
    {
        UserControl::OnMouseClick(e);
        if(e->Button == System::Windows::Forms::MouseButtons::Left)
            m_pGridWindow->OnMouseClick(e->Location);
    }

    void GridControl::OnMouseDoubleClick(System::Windows::Forms::MouseEventArgs^ e)
    {
        //System::Diagnostics::Trace::WriteLine(System::String::Format("mouse double click : {0}", System::DateTime::Now));
        //System::TimeSpan timeSpan = System::DateTime::Now - m_buttonDownTime;
        //if(timeSpan.Milliseconds >= System::Windows::Forms::SystemInformation::DoubleClickTime)
        //    return;

        UserControl::OnMouseDoubleClick(e);
        if(e->Button == System::Windows::Forms::MouseButtons::Left)
            m_pGridWindow->OnMouseDoubleClick(e->Location);
    }

    void GridControl::OnMouseWheel(System::Windows::Forms::MouseEventArgs^ e)
    {
        UserControl::OnMouseWheel(e);
        m_pGridWindow->OnMouseWheel(e->Location, e->Delta);
    }

    Ntreev::Windows::Forms::Grid::CaptionRow^ GridControl::CaptionRow::get()
    {
        return m_captionRow;
    }

    Ntreev::Windows::Forms::Grid::GroupingRow^ GridControl::GroupingRow::get()
    {
        return m_groupingRow;
    }

    System::Object^ GridControl::DataSource::get()
    {
        return m_dataSource;
    }

    void GridControl::DataSource::set(System::Object^ value)
    {
        if(m_dataSource != value)
        {
            if(value == nullptr || m_dataSource != nullptr)
                m_dataMember = System::String::Empty;

            SetDataConnection(value, m_dataMember);
            if(m_dataSource == value)
                OnDataSourceChanged(System::EventArgs::Empty);
        }
    }

    System::String^ GridControl::DataMember::get()
    {
        if(m_dataSource == nullptr)
            return System::String::Empty;
        return m_dataMember;
    }

    void GridControl::DataMember::set(System::String^ value)
    {
        if(m_dataMember != value)
        {
            SetDataConnection(DataSource, value);
            if(m_dataMember == value)
                OnDataMemberChanged(System::EventArgs::Empty);
        }
    }

    bool GridControl::AutoFitColumn::get()
    {
        return m_pGridCore->GetAutoFitColumn(); 
    }

    void GridControl::AutoFitColumn::set(bool value)
    {
        m_pGridCore->SetAutoFitColumn(value); 
    }

    bool GridControl::AutoFitRow::get()
    {
        return m_pGridCore->GetAutoFitRow(); 
    }

    void GridControl::AutoFitRow::set(bool value)
    {
        m_pGridCore->SetAutoFitRow(value); 
    }

    int GridControl::ColumnSplitter::get()
    {
        return m_pGridCore->GetColumnSplitter();  
    }

    void GridControl::ColumnSplitter::set(int value)
    {
        m_pGridCore->SetColumnSplitter(value);
    }

    int GridControl::RowSplitter::get()
    {
        return m_pGridCore->GetRowSplitter();  
    }

    void GridControl::RowSplitter::set(int value)
    {
        m_pGridCore->SetRowSplitter(value);
    }

    ColumnCollection^ GridControl::Columns::get()
    {
        return m_columnList;
    }

    VisibleColumnCollection^ GridControl::VisibleColumns::get()
    {
        return m_visibleColumnList;
    }

    DisplayableColumnCollection^ GridControl::DisplayableColumns::get()
    {
        return m_displayableColumnList;
    }

    FrozenColumnCollection^ GridControl::FrozenColumns::get()
    {
        return m_frozenColumnList;
    }

    UnfrozenColumnCollection^ GridControl::UnfrozenColumns::get()
    {
        return m_unfrozenColumnList;
    }

    SelectedColumnCollection^ GridControl::SelectedColumns::get()
    {
        return m_selectedColumns;
    }

    Column^ GridControl::FocusedColumn::get()
    {
        Cell^ focusedCell = FocusedCell;
        if(focusedCell == nullptr)
            return nullptr;
        return focusedCell->Column;
    }

    void GridControl::OnFontChanged(System::EventArgs^ e)
    {
        m_pGridWindow->SetFont(this->Font);
        UserControl::OnFontChanged(e);
        System::Diagnostics::Debug::WriteLine("Invalidate");
    }

    void GridControl::OnCurrencyManagerChanging(CurrencyManagerChangingEventArgs^ e)
    {
        if(m_manager != nullptr)
        {
            m_manager->ListChanged -= m_listChangedEventHandler;
            m_manager->BindingComplete -= m_bindingCompleteEventHandler;
        }

        this->CurrencyManagerChanging(this, e);
    }

    void GridControl::OnCurrencyManagerChanged(CurrencyManagerChangedEventArgs^ e)
    {
        m_manager = e->CurrecnyManager;

        if(m_manager != nullptr)
        {
            m_pGridCore->Reserve(m_manager ->GetItemProperties()->Count, m_manager ->List->Count);
            m_manager->ListChanged += m_listChangedEventHandler;
            m_manager->BindingComplete += m_bindingCompleteEventHandler;

            this->CurrencyManagerChanged(this, e);

            m_insertionRow->SetDefaultValue();
        }
        m_pGridCore->Invalidate();
    }

    void GridControl::currencyManager_ListChanged(System::Object^ /*sender*/, System::ComponentModel::ListChangedEventArgs^ e)
    {
        switch(e->ListChangedType)
        {
        case System::ComponentModel::ListChangedType::Reset:
            {
                m_pGridCore->Reserve(m_manager->GetItemProperties()->Count, m_manager->List->Count);

                if(this->BindingContext->Contains(m_dataSource, m_dataMember) == false)
                {
                    m_dataMember = System::String::Empty;
                }
            }
            break;
        }
    }

    void GridControl::currencyManager_BindingComplete(System::Object^ /*sender*/, System::Windows::Forms::BindingCompleteEventArgs^ /*e*/)
    {

    }

    void GridControl::RemoveRow(Row^ row)
    {
        m_rowList->Remove(row);
    }

    Row^ GridControl::AddNewRow()
    {
        return m_rowList->AddNew();
    }

    Row^ GridControl::AddNewRowFromInsertion()
    {
        return m_rowList->AddNewFromInsertion();
    }

    bool GridControl::IsInputKey(System::Windows::Forms::Keys keyData) 
    {
        using namespace System::Windows::Forms;

        bool result = false;

        Keys key = keyData & Keys::KeyCode;

        switch (key)
        {
        case Keys::Up:
        case Keys::Down:
        case Keys::Right:
        case Keys::Left:
            result = true;
            break;

        default:
            result = UserControl::IsInputKey(keyData);
            break;
        }

        return result;
    }

    bool GridControl::ProcessCmdKey(System::Windows::Forms::Message% msg, System::Windows::Forms::Keys keyData)
    {
        using namespace System::Windows::Forms;
        m_msg = msg;
        return UserControl::ProcessCmdKey(msg, keyData);
    }

    void GridControl::OnPreviewKeyDown(System::Windows::Forms::PreviewKeyDownEventArgs^ e)
    {
        using namespace System::Windows::Forms;

        m_pGridWindow->OnKeyDown((GrKeys)e->KeyCode);

        switch(e->KeyCode)
        {
        case Keys::Enter:
            {
                if(m_focusedCell != nullptr)
                {
                    if(m_focusedCell->Row == m_insertionRow)
                    {
                        Row^ row = AddNewRowFromInsertion();
                        if(row == nullptr)
                            break;

                        Cell^ cell = row->Cells[m_focusedCell->Column];
                        cell->Select(Ntreev::Windows::Forms::Grid::SelectionType::Normal);
                        cell->Focus();
                        cell->BringIntoView();
                    }
                    else
                    {
                        EditCell(m_focusedCell, Design::EditingReason());
                    }
                }
            }
            break;
        case Keys::F12:
            {
                if(m_insertionRow->IsVisible == true)
                {
                    m_insertionRow->Select(Ntreev::Windows::Forms::Grid::SelectionType::Normal);
                    m_insertionRow->Focus();
                    m_insertionRow->BringIntoView();
                }
            }
            break;
        case Keys::F2:
            {
                if(m_focusedCell != nullptr)
                {
                    EditCell(m_focusedCell, Design::EditingReason());
                }
                else
                {
                    break;
                }
            }
            break;
        case Keys::Escape:
            {
                if(m_focusedCell != nullptr && m_focusedCell->IsEdited == true)
                {
                    m_focusedCell->CancelEdit();
                    if(m_pGridCore->IsGrouped() == true)
                        m_pGridCore->GetDataRowList()->Update(true);
                }
                else if(FocusedRow != nullptr && FocusedRow->IsEdited == true)
                {
                    FocusedRow->CancelEdit();
                    if(m_pGridCore->IsGrouped() == true)
                        m_pGridCore->GetDataRowList()->Update(true);
                }
                Invalidate(false);
            }
            break;
        case Keys::ProcessKey:
            {
                Column^ column = this->FocusedColumn;

                if(column != nullptr)
                {
                    System::Char imeChar = Win32::API::ImmGetVirtualKey(Handle);
                    Design::EditingReason er(e->KeyValue);
                    if(System::Char::IsLetter(imeChar) && column->CanEditInternal(m_focusedCell, er))
                    {
                        EditCell(m_focusedCell, er);
                    }
                }
            }
            break;
        }
        UserControl::OnPreviewKeyDown(e);
    }

    void GridControl::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
    {
        switch(e->KeyCode)
        {
        case System::Windows::Forms::Keys::Enter:
            {
                e->SuppressKeyPress = true;
            }
        case System::Windows::Forms::Keys::Escape:
            {
                e->SuppressKeyPress = true;
            }
            break;
        }
        if(e->Control == true)
            e->SuppressKeyPress = true;
        UserControl::OnKeyDown(e);
    }

    void GridControl::OnKeyPress(System::Windows::Forms::KeyPressEventArgs^ e)
    {
        if(m_focusedCell != nullptr)
        {
            Column^ column = m_focusedCell->Column;
            Design::EditingReason reason(e->KeyChar);
            if(column->CanEditInternal(m_focusedCell, reason) == true)
            {
                EditCell(m_focusedCell, reason);
                return;
            }
        }
        UserControl::OnKeyPress(e);
    }

    void GridControl::InvokeSelectedRowsChanged()
    {
        OnSelectedRowsChanged(System::EventArgs::Empty);
    }

    void GridControl::InvokeSelectedColumnsChanged()
    {
        OnSelectedColumnsChanged(System::EventArgs::Empty);
    }

    void GridControl::InvokeSelectionChanged()
    {
        OnSelectionChanged(System::EventArgs::Empty);
    }

    void GridControl::InvokeFocusChanging()
    {

    }

    void GridControl::InvokeFocusChanged()
    {
        GrItem* pFocusedItem = Focuser->GetItem();

        Column^ oldFocusedColumn = FocusedColumn;
        Row^ oldFocusedRow    = FocusedRow;

        if(pFocusedItem == nullptr)
        {
            m_focusedCell = nullptr;
        }
        else
        {
            m_focusedCell = Cell::FromNative(pFocusedItem);
        }

        if(FocusedColumn != oldFocusedColumn)
        {
            OnFocusedColumnChanged(System::EventArgs::Empty);
        }

        if(FocusedRow != oldFocusedRow)
        {
            if(oldFocusedRow != nullptr)
                oldFocusedRow->ApplyEdit();
            OnFocusedRowChanged(System::EventArgs::Empty);
        }

        OnFocusedCellChanged(gcnew CellEventArgs(m_focusedCell));
    }

    bool GridControl::ShouldSerializeColumns()
    {
        return this->Columns->Count != 0;
    }

    bool GridControl::ShouldSerializeRows()
    {
        return this->Rows->Count != 0;
    }

    void GridControl::ResetColumns()
    {

    }

    void GridControl::ResetRows()
    {

    }

    void GridControl::InvokeColumnWidthChanged(Column^ column)
    {
        ColumnEventArgs e(column);
        OnColumnWidthChanged(%e);

        if(this->Site != nullptr)
        {
            using namespace System::ComponentModel;
            using namespace System::ComponentModel::Design;
            IComponentChangeService^ service = dynamic_cast<IComponentChangeService^>(GetService(IComponentChangeService::typeid));
            PropertyDescriptor^ propertyDescriptor = TypeDescriptor::GetProperties(column)["Width"];
            service->OnComponentChanging(column, propertyDescriptor);
            service->OnComponentChanged(column, propertyDescriptor, column->Width, column->Width);
        }
    }

    void GridControl::InvokeColumnFrozenChanged(Column^ column)
    {
        ColumnEventArgs e(column);
        OnColumnFrozenChanged(%e);

        if(this->Site != nullptr)
        {
            using namespace System::ComponentModel;
            using namespace System::ComponentModel::Design;
            IComponentChangeService^ service = dynamic_cast<IComponentChangeService^>(GetService(IComponentChangeService::typeid));
            PropertyDescriptor^ propertyDescriptor = TypeDescriptor::GetProperties(column)["IsFrozen"];
            service->OnComponentChanging(column, propertyDescriptor);
            service->OnComponentChanged(column, propertyDescriptor, !column->IsFrozen, column->IsFrozen);
        }
    }

    void GridControl::EditCell(Cell^ cell, Design::EditingReason editBy)
    {
        if(cell->IsReadOnly == true)
            return;

        if(cell->IsFocused == false)
            cell->IsFocused = true;

        m_pGridCore->EditItem(cell->NativeRef, editBy.ToNative());
    }

    void GridControl::ClearSelection()
    {
        Selector->ClearSelection();
        Focuser->Set(IFocusable::Null);
    }

    void GridControl::SelectAll()
    {
        if(m_pGridCore->GetMultiSelect() == false)
            throw gcnew System::InvalidOperationException();

        Selector->SelectAll();
    }

    void GridControl::BringIntoView(Cell^ cell)
    {
        m_pGridCore->BringIntoView(cell->NativeRef);
    }

    void GridControl::BringIntoView(Row^ row)
    {
        m_pDataRowList->BringIntoView(row->NativeRef);
    }

    void GridControl::BringIntoView(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        m_pColumnList->BringIntoView(column->NativeRef);
    }

    void GridControl::Clear()
    {
        OnCurrencyManagerChanging(gcnew CurrencyManagerChangingEventArgs(m_defaultManager));
        ClearCore(false);

        m_dataSource = nullptr;
        m_dataMember = System::String::Empty;
        m_manager  = nullptr;

        OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(m_defaultManager));
    }

    void GridControl::ClearCore(bool dataSourceOnly)
    {
        ClearEventArgs e(dataSourceOnly);
        OnClearing(%e);

        m_pGridCore->Clear();
        m_focusedCell = nullptr;

        m_defaultDataSource->Clear();

        OnCleared(%e);
    }

    RowCollection^ GridControl::Rows::get()
    {
        return m_rowList;
    }

    VisibleRowCollection^ GridControl::VisibleRows::get()
    {
        return m_visibleRowList;
    }

    DisplayableRowCollection^ GridControl::DisplayableRows::get()
    {
        return m_displayableRowList;
    }

    Ntreev::Windows::Forms::Grid::InsertionRow^ GridControl::InsertionRow::get()
    {
        return m_insertionRow; 
    }

    Cell^ GridControl::FocusedCell::get()
    {
        return m_focusedCell;
    }

    void GridControl::FocusedCell::set(Cell^ value)
    {
        if(value != nullptr)
            Focuser->Set(value->NativeRef);
        else
            Focuser->Set((IFocusable*)nullptr);
    }

    Cell^ GridControl::EditingCell::get()
    {
        //if(m_states->State != GridState::State::ItemEditing)
        // return nullptr;
        return m_focusedCell;
    }

    IToolTip^ GridControl::ToolTip::get()
    {
        return m_tooltips;
    }

    Design::Style^ GridControl::Style::get()
    {
        return m_style;
    }

    void GridControl::Style::set(Design::Style^ value)
    {
        if(value != nullptr)
            m_pGridCore->SetStyle(value->NativeStyle);
        else
            m_pGridCore->SetStyle(nullptr);
        m_style = value;
        Invalidate(false);
    }

    SelectedRowCollection^ GridControl::SelectedRows::get()
    {
        return m_selectedRows;  
    }

    Row^ GridControl::FocusedRow::get()
    {
        Cell^ focusedCell = FocusedCell;
        if(focusedCell == nullptr)
            return nullptr;
        return focusedCell->Row;
    }

    bool GridControl::IsInsertionRowVisible::get()
    {
        return m_insertionRow->IsVisible;
    }

    void GridControl::IsInsertionRowVisible::set(bool value)
    {
        m_insertionRow->IsVisible = value;
    }

    bool GridControl::IsCaptionRowVisible::get()
    {
        return m_captionRow->IsVisible;
    }

    void GridControl::IsCaptionRowVisible::set(bool value)
    {
        if(value == m_captionRow->IsVisible)
            return;

        m_captionRow->IsVisible = value;
    }

    bool GridControl::IsGroupingRowVisible::get()
    {
        return m_groupingRow->IsVisible;
    }

    void GridControl::IsGroupingRowVisible::set(bool value)
    {
        if(value == m_groupingRow->IsVisible)
            return;

        m_groupingRow->IsVisible = value;
    }

    bool GridControl::IsFrozingSplitterVisible::get()
    {
        return m_pColumnList->GetColumnSplitter()->GetVisible();
    }

    void GridControl::IsFrozingSplitterVisible::set(bool value)
    {
        m_pColumnList->GetColumnSplitter()->SetVisible(value);
    }

    bool GridControl::EnableColumnMoving::get()
    {
        return m_pGridCore->CanBeColumnMoving();
    }

    void GridControl::EnableColumnMoving::set(bool value)
    {
        m_pGridCore->EnableColumnMoving(value);
    }

    bool GridControl::EnableColumnResizing::get()
    {
        return m_pGridCore->CanBeColumnResizing();
    }

    void GridControl::EnableColumnResizing::set(bool value)
    {
        m_pGridCore->EnableColumnResizing(value);
    }

    bool GridControl::EnableColumnFrozing::get()
    {
        return m_pGridCore->CanBeColumnFrozing();
    }

    void GridControl::EnableColumnFrozing::set(bool value)
    {
        m_pGridCore->EnableColumnFrozing(value);
    }

    bool GridControl::EnableColumnSorting::get()
    {
        return m_pGridCore->CanBeColumnSorting();
    }

    void GridControl::EnableColumnSorting::set(bool value)
    {
        m_pGridCore->EnableColumnSorting(value);
    }

    bool GridControl::ReadOnly::get()
    {
        return m_pGridCore->GetReadOnly();
    }

    void GridControl::ReadOnly::set(bool value)
    {
        m_pGridCore->SetReadOnly(value);
    }

    bool GridControl::EnableRowResizing::get()
    {
        return m_pGridCore->CanBeRowResizing();
    }

    void GridControl::EnableRowResizing::set(bool value)
    {
        m_pGridCore->EnableRowResizing(value);
    }

    bool GridControl::EnableGrouping::get()
    {
        return m_pGridCore->CanBeGrouping();
    }

    void GridControl::EnableGrouping::set(bool value)
    {
        m_pGridCore->EnableGrouping(value);
    }

    bool GridControl::FullRowSelect::get()
    {
        return m_pGridCore->GetFullRowSelect();
    }

    void GridControl::FullRowSelect::set(bool value)
    {
        m_pGridCore->SetFullRowSelect(value);
    }

    bool GridControl::HideSelection::get()
    {
        return m_pGridCore->GetHideSelection();
    }

    void GridControl::HideSelection::set(bool value)
    {
        m_pGridCore->SetHideSelection(value);
    }

    bool GridControl::MultiSelect::get()
    {
        return m_pGridCore->GetMultiSelect();
    }

    void GridControl::MultiSelect::set(bool value)
    {
        m_pGridCore->SetMultiSelect(value);
    }

    bool GridControl::RowHighlight::get()
    {
        return m_pGridCore->GetRowHighlight();
    }

    void GridControl::RowHighlight::set(bool value)
    {
        m_pGridCore->SetRowHighlight(value);
    }

    Ntreev::Windows::Forms::Grid::RowHighlightType GridControl::RowHighlightType::get()
    {
        return (Ntreev::Windows::Forms::Grid::RowHighlightType)m_pGridCore->GetRowHighlightType();;
    }

    void GridControl::RowHighlightType::set(Ntreev::Windows::Forms::Grid::RowHighlightType value)
    {
        m_pGridCore->SetRowHighlightType((GrRowHighlightType)value);
    }

    bool GridControl::IsGrouped::get()
    {
        return m_pGridCore->IsGrouped();
    }

    bool GridControl::InvokeValueChanging(Cell^ cell, System::Object^ value, System::Object^ oldValue)
    {
        ValueChangingEventArgs e(cell, value, oldValue);
        OnValueChanging(%e);
        return e.Cancel != true;
    }

    void GridControl::InvokeValueChanged(Cell^ cell)
    {
        OnValueChanged(gcnew CellEventArgs(cell));
    }

    bool GridControl::InvokeInsertionRowInserting(Row^ row)
    {
        InsertionRowInsertingEventArgs e(row);
        OnInsertionRowInserting(%e);
        return e.Cancel != true;
    }

    void GridControl::InvokeInsertionRowInserted(Row^ row)
    {
        RowEventArgs e(row);
        OnInsertionRowInserted(%e);
    }

    bool GridControl::InvokeRowInserting(System::Object^ component)
    {
        RowInsertingEventArgs e(component);
        OnRowInserting(%e);
        return e.Cancel != true;
    }

    void GridControl::InvokeRowInserted(Row^ row)
    {
        OnRowInserted(gcnew RowInsertedEventArgs(row));
    }

    bool GridControl::InvokeRowRemoving(Row^ row)
    {
        RowRemovingEventArgs e(row);
        OnRowRemoving(%e);
        return e.Cancel != true;
    }

    void GridControl::InvokeRowRemoved(RowRemovedEventArgs^ e)
    {
        OnRowRemoved(e);
    }

    bool GridControl::InvokeColumnInserting(Column^ column)
    {
        ColumnInsertingEventArgs e(column);
        OnColumnInserting(%e);
        return e.Cancel == false;
    }

    void GridControl::InvokeColumnInserted(Column^ column)
    {
        ColumnEventArgs e(column);
        OnColumnInserted(%e);
    }

    Column^ GridControl::InvokeColumnBinding(System::ComponentModel::PropertyDescriptor^ propertyDescriptor, Column^ existColumn)
    {
        ColumnBindingEventArgs e(propertyDescriptor, existColumn);
        OnColumnBinding(%e);
        return e.BindingColumn;
    }

    void GridControl::InvokeColumnBinded(Column^ column)
    {
        ColumnEventArgs e(column);
        OnColumnBinded(%e);
    }

    bool GridControl::InvokeColumnMouseDown(Column^ column, System::Drawing::Point clientLocation)
    {
        ColumnMouseEventArgs ce(column, clientLocation);
        OnColumnMouseDown(%ce);
        return ce.Handled == true;
    }

    bool GridControl::InvokeColumnMouseUp(Column^ column, System::Drawing::Point clientLocation)
    {
        ColumnMouseEventArgs ce(column, clientLocation);
        OnColumnMouseUp(%ce);
        return ce.Handled == true;
    }

    void GridControl::InvokeColumnMouseEnter(Column^ column, System::Drawing::Point clientLocation)
    {
        ColumnMouseEventArgs ce(column, clientLocation);
        OnColumnMouseEnter(%ce);
    }

    bool GridControl::InvokeColumnMouseMove(Column^ column, System::Drawing::Point clientLocation)
    {
        ColumnMouseEventArgs ce(column, clientLocation);
        OnColumnMouseMove(%ce);

        return ce.Handled;
    }

    void GridControl::InvokeColumnMouseLeave(Column^ column)
    {
        ColumnMouseEventArgs ce(column, System::Drawing::Point::Empty);
        OnColumnMouseLeave(%ce);
    }

    bool GridControl::InvokeBeginEdit(Cell^ cell)
    {
        BeginEditEventArgs e(cell);
        OnBeginEdit(%e);
        return e.Cancel != true;
    }

    void GridControl::InvokeEndEdit(CellEventArgs^ e)
    {
        OnEndEdit(e);
    }

    void GridControl::InvokeScroll(System::Windows::Forms::ScrollEventArgs^ e)
    {
        OnScroll(e);
        m_pGridCore->Invalidate();
    }

    bool GridControl::InvokeCellMouseMove(Cell^ cell, System::Drawing::Point clientLocation)
    {
        CellMouseEventArgs e(cell, clientLocation);
        OnCellMouseMove(%e);
        return e.Handled;
    }

    void GridControl::InvokeCellClick(Cell^ cell)
    {
        OnCellClick(gcnew CellEventArgs(cell));
    }

    void GridControl::InvokeCellDoubleClick(Cell^ cell)
    {
        OnCellDoubleClick(gcnew CellEventArgs(cell));
    }

    void GridControl::OnValueChanging(ValueChangingEventArgs^ e)
    {
        ValueChanging(this, e);
    }

    void GridControl::OnValueChanged(CellEventArgs^ e)
    {
        ValueChanged(this, e);

        Cell^ cell = e->Cell;
        if(cell->IsDisplayable == true)
            Invalidate(cell->Bounds);
    }

    void GridControl::OnRowInserting(RowInsertingEventArgs^ e)
    {
        RowInserting(this, e);
    }

    void GridControl::OnRowInserted(RowInsertedEventArgs^ e)
    {
        RowInserted(this, e);
    }

    void GridControl::OnRowRemoving(RowRemovingEventArgs^ e)
    {
        RowRemoving(this, e);
    }

    void GridControl::OnRowRemoved(RowRemovedEventArgs^ e)
    {
        RowRemoved(this, e);
    }

    void GridControl::OnInsertionRowInserting(InsertionRowInsertingEventArgs^ e)
    {
        InsertionRowInserting(this, e);
    }

    void GridControl::OnInsertionRowInserted(RowEventArgs^ e)
    {
        Refresh();
        InsertionRowInserted(this, e);
    }

    void GridControl::OnSelectedRowsChanged(System::EventArgs^ e)
    {
        SelectedRowsChanged(this, e);
    }

    void GridControl::OnSelectedColumnsChanged(System::EventArgs^ e)
    {
        SelectedColumnsChanged(this, e);
    }

    void GridControl::OnSelectionChanged(System::EventArgs^ e)
    {
        SelectionChanged(this, e);
    }

    void GridControl::OnFocusedRowChanged(System::EventArgs^ e)
    {
        FocusedRowChanged(this, e);
    }

    void GridControl::OnFocusedColumnChanged(System::EventArgs^ e)
    {
        FocusedColumnChanged(this, e);
    }

    void GridControl::OnFocusedCellChanged(CellEventArgs^ e)
    {
        FocusedCellChanged(this, e);

        if(e->Cell != nullptr)
        {
            m_manager->Position = e->Cell->Row->ComponentIndex;
        }
    }

    void GridControl::OnColumnInserting(ColumnInsertingEventArgs^ e)
    {
        ColumnInserting(this, e);
    }

    void GridControl::OnColumnInserted(ColumnEventArgs^ e)
    {
        Column^ column = e->Column;
        m_insertionRow->NewCell(column);
        for each(Row^ item in m_rowList)
        {
            item->NewCell(column);
        }

        ColumnInserted(this, e);
    }

    void GridControl::OnColumnBinding(ColumnBindingEventArgs^ e)
    {
        ColumnBinding(this, e);
    }

    void GridControl::OnColumnBinded(ColumnEventArgs^ e)
    {
        Column^ column = e->Column;
        m_insertionRow->NewCell(column);
        for each(Row^ item in m_rowList)
        {
            item->NewCell(column);
        }

        ColumnBinded(this, e);
    }

    void GridControl::OnColumnWidthChanged(ColumnEventArgs^ e)
    {
        ColumnWidthChanged(this, e);
    }

    void GridControl::OnColumnFrozenChanged(ColumnEventArgs^ e)
    {
        ColumnFrozenChanged(this, e);
    }

    void GridControl::OnColumnMouseEnter(ColumnMouseEventArgs^ e)
    {
        if(this->Site == nullptr)
        {
            System::Drawing::Point location = e->Column->Bounds.Location;
            location.Y -= 50;

            m_tooltips->Show(e->Column->Tooltip);
        }

        ColumnMouseEnter(this, e);
    }

    void GridControl::OnColumnMouseLeave(ColumnMouseEventArgs^ e)
    {
        if(this->Site == nullptr)
        {
            m_tooltips->Hide();
        }

        ColumnMouseLeave(this, e);
    }

    void GridControl::OnColumnMouseDown(ColumnMouseEventArgs^ e)
    {
        ColumnMouseDown(this, e);
    }

    void GridControl::OnColumnMouseUp(ColumnMouseEventArgs^ e)
    {
        ColumnMouseUp(this, e);
    }

    void GridControl::OnColumnMouseMove(ColumnMouseEventArgs^ e)
    {
        ColumnMouseMove(this, e);
    }

    void GridControl::OnCellMouseMove(CellMouseEventArgs^ e)
    {
        CellMouseMove(this, e);
    }

    void GridControl::OnCellClick(CellEventArgs^ e)
    {
        CellClick(this, e);  
    }

    void GridControl::OnCellDoubleClick(CellEventArgs^ e)
    {
        CellDoubleClick(this, e);  
    }

    void GridControl::OnBeginEdit(BeginEditEventArgs^ e)
    {
        BeginEdit(this, e);
    }

    void GridControl::OnEndEdit(CellEventArgs^ e)
    {
        EndEdit(this, e);
    }

    void GridControl::OnClearing(ClearEventArgs^ e)
    {
        Clearing(this, e);
    }

    void GridControl::OnCleared(ClearEventArgs^ e)
    {
        Cleared(this, e);
        Invalidate(false);
    }

    void GridControl::OnDataSourceChanged(System::EventArgs^ e)
    {
        DataSourceChanged(this, e);
    }

    void GridControl::OnDataMemberChanged(System::EventArgs^ e)
    {
        DataMemberChanged(this, e);
    }

    void GridControl::OnDataBindingComplete(System::EventArgs^ e)
    {
        try
        {
            Column^ column = m_visibleColumnList->Count > 0 ? m_visibleColumnList[0] : nullptr;
            Row^ row = m_visibleRowList->Count > 0 ? (Ntreev::Windows::Forms::Grid::Row^)m_visibleRowList[0] : nullptr;
            if(column == nullptr || row == nullptr)
                return;
            GrItem* pItem = row->Cells[column]->NativeRef;
            Selector->SelectItem(pItem, GrSelectionType_Normal);
            Selector->SetAnchor(pItem);
            Focuser->Set(pItem);
        }
        catch(System::Exception^)
        {

        }

        DataBindingComplete(this, e);
    }

    void GridControl::OnStateBegin(System::EventArgs^ e)
    {
        StateBegin(this, e);
    }

    void GridControl::OnStateEnd(System::EventArgs^ e)
    {
        StateEnd(this, e);
    }

    bool GridControl::IsRowNumberVisible::get()
    {
        return m_pDataRowList->GetRowNumberVisible();
    }

    void GridControl::IsRowNumberVisible::set(bool value)
    {
        m_pDataRowList->SetRowNumberVisible(value);
    }

    bool GridControl::IsRowHeaderVisible::get()
    {
        return m_pGridCore->GetRowVisible();
    }

    void GridControl::IsRowHeaderVisible::set(bool value)
    {
        m_pGridCore->SetRowVisible(value);
    }

    GrGridCore* GridControl::GridCore::get()
    {
        return m_pGridCore; 
    }

    GrGridPainter* GridControl::GridPainter::get()
    {
        return m_pGridPainter; 
    }

    GrItemSelector* GridControl::Selector::get()
    {
        return m_pGridCore->GetItemSelector();
    }

    GrFocuser* GridControl::Focuser::get()
    {
        return m_pGridCore->GetFocuser();
    }

    Ntreev::Windows::Forms::Grid::ErrorDescriptor^ GridControl::ErrorDescriptor::get()
    {
        return m_errorDescriptor; 
    }

    GridControl::DataBindingRef::DataBindingRef(GridControl^ gridControl)
        : m_gridControl(gridControl)
    {
        m_gridControl->m_dataBindingRef++;
        m_gridControl->SuspendLayout();
    }

    GridControl::DataBindingRef::~DataBindingRef()
    {
        m_gridControl->m_dataBindingRef--;
        m_gridControl->ResumeLayout();
    }

    System::Drawing::Rectangle GridControl::DataRectangle::get()
    {
        return m_pGridCore->GetDataRect();
    }

    bool GridControl::DesignTimeSetCursor()
    {
        GrStateManager* pStateManager = m_pGridCore->GetStateManager();
        
        switch (pStateManager->GetGridState())
        {
        case GrGridState_ColumnPressing:
        case GrGridState_ColumnResizing:
        case GrGridState_ColumnSplitterMoving:
        case GrGridState_RowResizing:
        case GrGridState_GroupingInfoPressing:
        case GrGridState_GroupingCellPressing:
        case GrGridState_GroupingExpandPressing:
            return pStateManager->SetCursor();
        default:
            break;
        }
        return false;
    }

    bool GridControl::DesignTimeHitTest(System::Drawing::Point globalLocation)
    {
        System::Drawing::Point point = this->PointToClient(globalLocation);

        try
        {
            GrStateManager* pStateManager = m_pGridCore->GetStateManager();

            switch (pStateManager->GetHitTest(point))
            {
            case GrGridState_ColumnPressing:
            case GrGridState_ColumnResizing:
            case GrGridState_ColumnSplitterMoving:
            case GrGridState_RowResizing:
            case GrGridState_GroupingInfoPressing:
            case GrGridState_GroupingCellPressing:
            case GrGridState_GroupingExpandPressing:
                return true;
            default:
                break;

            }
        }
        catch (System::Exception^)
        {
            return false;
        }


        //if (this->DisplayRectangle.Contains(point) == false)
        //{
        // return true;
        //}

        return false;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/