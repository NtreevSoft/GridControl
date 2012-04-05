﻿//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4478.19833
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
#include "Column.h"

#include "Cell.h"
#include "Row.h"
#include "CaptionRow.h"
#include "GroupRow.h"
#include "GroupPanel.h"
#include "Events.h"
#include "Resources.h"
#include "CellCollection.h"
#include "RowCollection.h"
#include "SelectedRowCollection.h"
#include "VisibleRowCollection.h"
#include "DisplayableRowCollection.h"
#include "ColumnCollection.h"
#include "SelectedColumnCollection.h"
#include "VisibleColumnCollection.h"
#include "DisplayableColumnCollection.h"
#include "FrozenColumnCollection.h"
#include "UnfrozenColumnCollection.h"
#include "GroupRowCollection.h"
#include "NativeUtilities.h"
#include "TimeTester.h"
#include "NativeClasses.h"
#include "FromNative.h"
#include "About.h"
#include "Tooltip.h"
#include "ErrorDescriptor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    GridControl::GridControl()
    {
#ifdef _TIME_TEST
        TimeTester timeTest("GridControl 생성자");
#endif

        m_pGridWindow = new Native::WinFormWindow(this);
        m_pGridCore = new Native::WinFormGridCore(this, m_pGridWindow);
        m_pGridPainter = m_pGridWindow->GetGridPainter();

        m_pColumnList = m_pGridCore->GetColumnList();
        m_pDataRowList = m_pGridCore->GetDataRowList();

        m_pGridCore->SetDisplayRect(this->DisplayRectangle);

#ifdef _TIME_TEST
        Rendering = true;
#endif

        m_dataSource = nullptr;
        m_dataMember = System::String::Empty;

        m_tooltips = gcnew Ntreev::Windows::Forms::Grid::ToolTip(this, 3);
        m_errorDescriptor = gcnew Ntreev::Windows::Forms::Grid::ErrorDescriptor(this);

        m_columnList = gcnew ColumnCollection(this);
        m_visibleColumnList = gcnew VisibleColumnCollection(this);
        m_displayableColumnList = gcnew DisplayableColumnCollection(this);


        m_unfrozenColumnList = gcnew UnfrozenColumnCollection(this);
        m_rowList = gcnew RowCollection(this);
        m_visibleRowList = gcnew VisibleRowCollection(this);
        m_displayableRowList = gcnew DisplayableRowCollection(this);
        m_style = gcnew Ntreev::Windows::Forms::Grid::Style();

        m_selectedColumns = gcnew SelectedColumnCollection(this, this->Selector->GetSelectedColumns());
        m_selectedRows = gcnew SelectedRowCollection(this, this->Selector->GetSelectedRows());

        m_groupRows = gcnew GroupRowCollection(this);

        m_captionRow = gcnew Ntreev::Windows::Forms::Grid::CaptionRow(this, m_pGridCore->GetCaptionRow());
        m_groupPanel = gcnew Ntreev::Windows::Forms::Grid::GroupPanel(this, m_pGridCore->GetGroupPanel());

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

        if(e->InvalidRect.Left == 0 && e->InvalidRect.Top == 0)
            int eqwr=0;
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

            clipping.right = System::Math::Min(clipping.right, this->DisplayRectangle.Right);
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
        GrDataRow* pInsertionRow = m_pDataRowList->GetInsertionRow();
        for(unsigned int i=0 ; i<m_pColumnList->GetDisplayableColumnCount() ; i++)
        {
            GrColumn* pColumn = m_pColumnList->GetDisplayableColumn(i);

            int x = pColumn->GetX();
            int r = x + pColumn->GetWidth();

            if(x >= clipRectangle.Right || r < clipRectangle.Left)
                continue;

            Ntreev::Windows::Forms::Grid::Column^ column = m_columnList[pColumn];
            Ntreev::Windows::Forms::Grid::ViewType viewType = column->ViewType;
            if(viewType == Ntreev::Windows::Forms::Grid::ViewType::Text)
                continue;

            for(unsigned int j=0 ; j<m_pDataRowList->GetDisplayableRowCount() + 1 ; j++)
            {
                GrDataRow* pDataRow = nullptr;
                if(j==0)
                    pDataRow = pInsertionRow->GetVisible() == true ? pInsertionRow : nullptr;
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

                Ntreev::Windows::Forms::Grid::Cell^ cell = FromNative::Get(pItem);
                column->PaintValue(graphics, paintRect, cell, cell->Value);
            }
        }
    }

    void GridControl::PaintRowState(System::Drawing::Graphics^ g)
    {
        IDataRow* pFocusedRow = this->Focuser->GetFocusedRow();
        if(pFocusedRow == nullptr || pFocusedRow->GetDisplayable() == false)
            return;

        Ntreev::Windows::Forms::Grid::Row^ row = dynamic_cast<Ntreev::Windows::Forms::Grid::Row^>(FocusedRow);

        if(EditingCell != nullptr)
        {
            GrRect bound = EditingCell->NativeRef->GetDataRow()->GetRect();
            GrPoint center = bound.GetCenter();

            System::Drawing::Bitmap^ image = _Resources::RowEditing;
            center.x -= (image->Width / 2);
            center.y -= (image->Height / 2);
            g->DrawImageUnscaled(image, center.x, center.y);
        }
        else if(row && row->IsEdited == true)
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
            if(pFocusedRow->GetRowType() == GrRowType_InsertionRow)
                image =_Resources::InsertionRowFocused;
            center.x -= (image->Width / 2);
            center.y -= (image->Height / 2);
            g->DrawImageUnscaled(image, center.x, center.y);
        }

        GrDataRow* pInsertionRow = m_pDataRowList->GetInsertionRow();
        if(pInsertionRow->GetVisible() == true && pInsertionRow->HasFocused() == false)
        {
            GrRect bound = pInsertionRow->GetRect();
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
                throw gcnew System::NotSupportedException(_Resources::DataSourceInitializingFailed);
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

    Ntreev::Windows::Forms::Grid::CellBase^ GridControl::GetAt(System::Drawing::Point pt)
    {
        GrHitTest hitTest;
        if(m_pGridCore->HitTest(pt, &hitTest) == false)
            return nullptr;

        System::Object^ e = hitTest.pHitted->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::CellBase^>(e);
    }

    Ntreev::Windows::Forms::Grid::Cell^ GridControl::GetCellAt(System::Drawing::Point pt)
    {
        GrHitTest hitTest;
        if(m_pGridCore->HitTest(pt, &hitTest) == false)
            return nullptr;
        GrItem* pItem = dynamic_cast<GrItem*>(hitTest.pHitted);
        if(pItem == nullptr)
            return nullptr;

        System::Object^ e = pItem->ManagedRef;
        return safe_cast<Ntreev::Windows::Forms::Grid::Cell^>(e);
    }

    void GridControl::ShowAbout()
    {
        Ntreev::Windows::Forms::Grid::About^ form = gcnew Ntreev::Windows::Forms::Grid::About();
        form->ShowDialog();
    }

    void GridControl::WndProc(System::Windows::Forms::Message% m)
    {
        switch(m.Msg)
        {
        case Native::WM::WM_HSCROLL:
            {
                Native::WinFormScroll* pScroll = (Native::WinFormScroll*)m_pGridCore->GetHorzScroll();
                pScroll->WndProc(m.HWnd, m.WParam);
            }
            return;
        case Native::WM::WM_VSCROLL:
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
        UserControl::OnMouseLeave(e);
        m_pGridWindow->OnMouseLeave();
    }

    void GridControl::OnMouseMove(System::Windows::Forms::MouseEventArgs^ e)
    {
        UserControl::OnMouseMove(e);
        m_pGridWindow->OnMouseMove(e->Location, e->Button == System::Windows::Forms::MouseButtons::Left);
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
        // return;

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

    Ntreev::Windows::Forms::Grid::GroupPanel^ GridControl::GroupPanel::get()
    {
        return m_groupPanel;
    }

    System::String^ GridControl::Caption::get()
    {
        return m_captionRow->Text;
    }

    void GridControl::Caption::set(System::String^ value)
    {
        m_captionRow->Text = value;
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

    Ntreev::Windows::Forms::Grid::Column^ GridControl::FocusedColumn::get()
    {
        Ntreev::Windows::Forms::Grid::Cell^ focusedCell = FocusedCell;
        if(focusedCell == nullptr)
            return nullptr;
        return focusedCell->Column;
    }

    void GridControl::OnFontChanged(System::EventArgs^ e)
    {
        m_pGridWindow->SetFont(this->Font);
        UserControl::OnFontChanged(e);
#ifdef _DEBUG
        System::Diagnostics::Debug::WriteLine("Invalidate");
#endif
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

    void GridControl::OnCurrencyManagerChanged(Ntreev::Windows::Forms::Grid::CurrencyManagerChangedEventArgs^ e)
    {
        m_manager = e->CurrecnyManager;

        if(m_manager != nullptr)
        {
            m_pGridCore->Reserve(m_manager->GetItemProperties()->Count, m_manager ->List->Count);
            m_manager->ListChanged += m_listChangedEventHandler;
            m_manager->BindingComplete += m_bindingCompleteEventHandler;

            this->CurrencyManagerChanged(this, e);
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

        switch(keyData)
        {
        case Keys::Enter:
            {
                if(m_focusedCell == nullptr)
                    break;
                
                if(m_focusedCell->Row == this->InsertionRow)
                {
                    Ntreev::Windows::Forms::Grid::Row^ row = this->Rows->AddFromInsertion();
                    if(row != nullptr)
                    {
                        Ntreev::Windows::Forms::Grid::Cell^ cell = row->Cells[m_focusedCell->Column];
                        cell->Select(Ntreev::Windows::Forms::Grid::SelectionType::Normal);
                        cell->Focus();
                        cell->BringIntoView();

                    }
                }
                else
                {
                    EditCell(m_focusedCell, Ntreev::Windows::Forms::Grid::EditingReason(keyData));
                }
                return true;
            }
            break;
        }

        return UserControl::ProcessCmdKey(msg, keyData);
    }

    void GridControl::OnPreviewKeyDown(System::Windows::Forms::PreviewKeyDownEventArgs^ e)
    {
        using namespace System::Windows::Forms;

        if(m_pGridWindow->OnKeyDown((GrKeys)e->KeyCode) == true)
        {
            UserControl::OnPreviewKeyDown(e);
            return;
        }


        switch(e->KeyCode)
        {
        //case Keys::Enter:
        //    {
        //        if(m_focusedCell != nullptr)
        //        {
        //            if(m_focusedCell->Row == this->InsertionRow)
        //            {
        //                Ntreev::Windows::Forms::Grid::Row^ row = this->Rows->AddFromInsertion();
        //                if(row == nullptr)
        //                    break;

        //                Ntreev::Windows::Forms::Grid::Cell^ cell = row->Cells[m_focusedCell->Column];
        //                cell->Select(Ntreev::Windows::Forms::Grid::SelectionType::Normal);
        //                cell->Focus();
        //                cell->BringIntoView();
        //            }
        //            else
        //            {
        //                EditCell(m_focusedCell, Ntreev::Windows::Forms::Grid::EditingReason(e->KeyCode));
        //            }
        //        }
        //    }
        //    break;
        case Keys::F12:
            {
                if(this->InsertionRow->IsVisible == true)
                {
                    this->InsertionRow->Select(Ntreev::Windows::Forms::Grid::SelectionType::Normal);
                    this->InsertionRow->Focus();
                    this->InsertionRow->BringIntoView();
                }
            }
            break;
        case Keys::F2:
            {
                if(m_focusedCell != nullptr)
                {
                    EditCell(m_focusedCell, Ntreev::Windows::Forms::Grid::EditingReason(e->KeyCode));
                }
                else
                {
                    break;
                }
            }
            break;
        case Keys::Escape:
            {
                if(m_focusedCell != nullptr)
                {
                    if(m_focusedCell->IsEdited == true)
                    {
                        m_focusedCell->CancelEdit();
                        if(m_pGridCore->IsGrouped() == true)
                            m_pGridCore->GetDataRowList()->Update(true);
                    }
                    else if(m_focusedCell->Row->IsEdited == true)
                    {
                        m_focusedCell->Row->CancelEdit();
                        if(m_pGridCore->IsGrouped() == true)
                            m_pGridCore->GetDataRowList()->Update(true);
                    }
                    Invalidate(false);
                }
            }
            break;
        case Keys::ProcessKey:
            {
                Ntreev::Windows::Forms::Grid::Column^ column = this->FocusedColumn;

                if(column != nullptr)
                {
                    System::Char imeChar = Native::Methods::ImmGetVirtualKey(Handle);
                    Ntreev::Windows::Forms::Grid::EditingReason er(e->KeyValue);
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
        case System::Windows::Forms::Keys::Escape:
            {
                e->SuppressKeyPress = true;
            }
        default:
            {
                Ntreev::Windows::Forms::Grid::Column^ column = this->FocusedColumn;

                if(column != nullptr)
                {
                    Ntreev::Windows::Forms::Grid::EditingReason er(e->KeyCode);
                    if(column->CanEditInternal(m_focusedCell, er) == true)
                    {
                        EditCell(m_focusedCell, er);
                        e->SuppressKeyPress = true;
                    }
                }
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
            Ntreev::Windows::Forms::Grid::Column^ column = m_focusedCell->Column;
            Ntreev::Windows::Forms::Grid::EditingReason reason(e->KeyChar);
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
        m_oldFocusedColumn = FocusedColumn;
        m_oldFocusedRow = FocusedRow;
    }

    void GridControl::InvokeFocusChanged()
    {
        GrItem* pFocusedItem = this->Focuser->GetItem();

        if(pFocusedItem == nullptr)
        {
            m_focusedCell = nullptr;
        }
        else
        {
            m_focusedCell = FromNative::Get(pFocusedItem);
        }

        if(FocusedColumn != m_oldFocusedColumn)
        {
            OnFocusedColumnChanged(System::EventArgs::Empty);
        }

        if(FocusedRow != m_oldFocusedRow)
        {
            if(m_focusedCell != nullptr)
                m_focusedCell->Row->ApplyEdit();
            OnFocusedRowChanged(System::EventArgs::Empty);
        }

        m_oldFocusedColumn = FocusedColumn;
        m_oldFocusedRow = FocusedRow;

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

    bool GridControl::ShouldSerializeCaption()
    {
        return m_captionRow->Text != _Resources::GetString("CaptionRow.Text");
    }

    void GridControl::InvokeColumnWidthChanged(Ntreev::Windows::Forms::Grid::Column^ column)
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

    void GridControl::InvokeColumnFrozenChanged(Ntreev::Windows::Forms::Grid::Column^ column)
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

    void GridControl::EditCell(Ntreev::Windows::Forms::Grid::Cell^ cell, Ntreev::Windows::Forms::Grid::EditingReason editBy)
    {
        if(cell->IsReadOnly == true)
            return;

        if(cell->IsFocused == false)
            cell->IsFocused = true;

        m_pGridCore->EditItem(cell->NativeRef, editBy.ToNative());
    }

    void GridControl::ClearSelection()
    {
        this->Selector->ClearSelection();
        this->Focuser->Set(IFocusable::Null);
    }

    void GridControl::SelectAll()
    {
        if(m_pGridCore->GetMultiSelect() == false)
            throw gcnew System::InvalidOperationException();

        this->Selector->SelectAll();
    }

    void GridControl::BringIntoView(Ntreev::Windows::Forms::Grid::Cell^ cell)
    {
        m_pGridCore->BringIntoView(cell->NativeRef);
    }

    void GridControl::BringIntoView(Ntreev::Windows::Forms::Grid::Row^ row)
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
        m_manager = nullptr;

        OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(m_defaultManager));
    }

    void GridControl::ClearCore(bool dataSourceOnly)
    {
        ClearEventArgs e(dataSourceOnly);
        OnClearing(%e);

        m_pGridCore->Clear();
        m_focusedCell = nullptr;
        m_oldFocusedColumn = nullptr;
        m_oldFocusedRow = nullptr;

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

    Ntreev::Windows::Forms::Grid::Row^ GridControl::InsertionRow::get()
    {
        return this->Rows->InsertionRow; 
    }

    Ntreev::Windows::Forms::Grid::Cell^ GridControl::FocusedCell::get()
    {
        return m_focusedCell;
    }

    void GridControl::FocusedCell::set(Ntreev::Windows::Forms::Grid::Cell^ value)
    {
        if(value != nullptr)
            this->Focuser->Set(value->NativeRef);
        else
            this->Focuser->Set((IFocusable*)nullptr);
    }

    Ntreev::Windows::Forms::Grid::Cell^ GridControl::EditingCell::get()
    {
        return m_focusedCell;
    }

    Ntreev::Windows::Forms::Grid::ToolTip^ GridControl::ToolTip::get()
    {
        return m_tooltips;
    }

    Ntreev::Windows::Forms::Grid::Style^ GridControl::Style::get()
    {
        return m_style;
    }

    void GridControl::Style::set(Ntreev::Windows::Forms::Grid::Style^ value)
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

    GroupRowCollection^ GridControl::GroupRows::get()
    {
        return m_groupRows; 
    }

    Ntreev::Windows::Forms::Grid::RowBase^ GridControl::FocusedRow::get()
    {
        IDataRow* pDataRow = this->Focuser->GetFocusedRow();
        return FromNative::Get(pDataRow);
    }

    bool GridControl::IsInsertionRowVisible::get()
    {
        return this->InsertionRow->IsVisible;
    }

    void GridControl::IsInsertionRowVisible::set(bool value)
    {
        this->InsertionRow->IsVisible = value;
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

    bool GridControl::IsGroupPanelVisible::get()
    {
        return m_groupPanel->IsVisible;
    }

    void GridControl::IsGroupPanelVisible::set(bool value)
    {
        if(value == m_groupPanel->IsVisible)
            return;

        m_groupPanel->IsVisible = value;
    }

    bool GridControl::IsFrozingSplitterVisible::get()
    {
        return m_pColumnList->GetColumnSplitter()->GetVisible();
    }

    void GridControl::IsFrozingSplitterVisible::set(bool value)
    {
        m_pColumnList->GetColumnSplitter()->SetVisible(value);
    }

    bool GridControl::IsColumnMovable::get()
    {
        return m_pGridCore->GetColumnMovable();
    }

    void GridControl::IsColumnMovable::set(bool value)
    {
        m_pGridCore->SetColumnMovable(value);
    }

    bool GridControl::IsColumnResizable::get()
    {
        return m_pGridCore->GetColumnResizable();
    }

    void GridControl::IsColumnResizable::set(bool value)
    {
        m_pGridCore->SetColumnResizable(value);
    }

    bool GridControl::IsColumnFreezable::get()
    {
        return m_pGridCore->GetColumnFreezable();
    }

    void GridControl::IsColumnFreezable::set(bool value)
    {
        m_pGridCore->SetColumnFreezable(value);
    }

    bool GridControl::IsColumnSortable::get()
    {
        return m_pGridCore->GetColumnSortable();
    }

    void GridControl::IsColumnSortable::set(bool value)
    {
        m_pGridCore->SetColumnSortable(value);
    }

    bool GridControl::ReadOnly::get()
    {
        return m_pGridCore->GetReadOnly();
    }

    void GridControl::ReadOnly::set(bool value)
    {
        m_pGridCore->SetReadOnly(value);
    }

    bool GridControl::IsRowResizable::get()
    {
        return m_pGridCore->GetRowResizable();
    }

    void GridControl::IsRowResizable::set(bool value)
    {
        m_pGridCore->SetRowResizable(value);
    }

    bool GridControl::IsGroupable::get()
    {
        return m_pGridCore->GetGroupable();
    }

    void GridControl::IsGroupable::set(bool value)
    {
        m_pGridCore->SetGroupable(value);
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

    bool GridControl::InvokeValueChanging(Ntreev::Windows::Forms::Grid::Cell^ cell, System::Object^ value, System::Object^ oldValue)
    {
        ValueChangingEventArgs e(cell, value, oldValue);
        OnValueChanging(%e);
        return e.Cancel != true;
    }

    void GridControl::InvokeValueChanged(Ntreev::Windows::Forms::Grid::Cell^ cell)
    {
        OnValueChanged(gcnew CellEventArgs(cell));
    }

    bool GridControl::InvokeRowInserting(System::Object^ component)
    {
        Ntreev::Windows::Forms::Grid::RowInsertingEventArgs e(component);
        OnRowInserting(%e);
        return e.Cancel != true;
    }

    void GridControl::InvokeRowInserted(Ntreev::Windows::Forms::Grid::Row^ row)
    {
        OnRowInserted(gcnew RowInsertedEventArgs(row));
    }

    bool GridControl::InvokeRowRemoving(Ntreev::Windows::Forms::Grid::Row^ row)
    {
        RowRemovingEventArgs e(row);
        OnRowRemoving(%e);
        return e.Cancel != true;
    }

    void GridControl::InvokeRowRemoved(RowRemovedEventArgs^ e)
    {
        OnRowRemoved(e);
    }

    bool GridControl::InvokeColumnInserting(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        ColumnInsertingEventArgs e(column);
        OnColumnInserting(%e);
        return e.Cancel == false;
    }

    void GridControl::InvokeColumnInserted(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        ColumnEventArgs e(column);
        OnColumnInserted(%e);
    }

    Ntreev::Windows::Forms::Grid::Column^ GridControl::InvokeColumnBinding(System::ComponentModel::PropertyDescriptor^ propertyDescriptor, Ntreev::Windows::Forms::Grid::Column^ existColumn)
    {
        ColumnBindingEventArgs e(propertyDescriptor, existColumn);
        OnColumnBinding(%e);
        return e.BindingColumn;
    }

    void GridControl::InvokeColumnBinded(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        ColumnEventArgs e(column);
        OnColumnBinded(%e);
    }

    bool GridControl::InvokeColumnMouseDown(Ntreev::Windows::Forms::Grid::Column^ column, System::Drawing::Point clientLocation)
    {
        ColumnMouseEventArgs ce(column, clientLocation);
        OnColumnMouseDown(%ce);
        return ce.Handled == true;
    }

    bool GridControl::InvokeColumnMouseUp(Ntreev::Windows::Forms::Grid::Column^ column, System::Drawing::Point clientLocation)
    {
        ColumnMouseEventArgs ce(column, clientLocation);
        OnColumnMouseUp(%ce);
        return ce.Handled == true;
    }

    void GridControl::InvokeColumnMouseEnter(Ntreev::Windows::Forms::Grid::Column^ column, System::Drawing::Point clientLocation)
    {
        ColumnMouseEventArgs ce(column, clientLocation);
        OnColumnMouseEnter(%ce);
    }

    bool GridControl::InvokeColumnMouseMove(Ntreev::Windows::Forms::Grid::Column^ column, System::Drawing::Point clientLocation)
    {
        ColumnMouseEventArgs ce(column, clientLocation);
        OnColumnMouseMove(%ce);

        return ce.Handled;
    }

    void GridControl::InvokeColumnMouseLeave(Ntreev::Windows::Forms::Grid::Column^ column)
    {
        ColumnMouseEventArgs ce(column, System::Drawing::Point::Empty);
        OnColumnMouseLeave(%ce);
    }

    bool GridControl::InvokeBeginEdit(Ntreev::Windows::Forms::Grid::Cell^ cell)
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

    bool GridControl::InvokeCellMouseMove(Ntreev::Windows::Forms::Grid::Cell^ cell, System::Drawing::Point clientLocation)
    {
        CellMouseEventArgs e(cell, clientLocation);
        OnCellMouseMove(%e);
        return e.Handled;
    }

    void GridControl::InvokeCellClick(Ntreev::Windows::Forms::Grid::Cell^ cell)
    {
        OnCellClick(gcnew CellEventArgs(cell));
    }

    void GridControl::InvokeCellDoubleClick(Ntreev::Windows::Forms::Grid::Cell^ cell)
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

        Ntreev::Windows::Forms::Grid::Cell^ cell = e->Cell;
        if(cell->IsDisplayable == true)
            Invalidate(cell->Bounds);
    }

    void GridControl::OnRowInserting(Ntreev::Windows::Forms::Grid::RowInsertingEventArgs^ e)
    {
        RowInserting(this, e);
    }

    void GridControl::OnRowInserted(Ntreev::Windows::Forms::Grid::RowInsertedEventArgs^ e)
    {
        RowInserted(this, e);
    }

    void GridControl::OnRowRemoving(Ntreev::Windows::Forms::Grid::RowRemovingEventArgs^ e)
    {
        RowRemoving(this, e);
    }

    void GridControl::OnRowRemoved(RowRemovedEventArgs^ e)
    {
        RowRemoved(this, e);
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

        Ntreev::Windows::Forms::Grid::Cell^ cell = e->Cell;
        if(cell != nullptr && cell->Row != this->InsertionRow)
        {
            if(m_manager->Position != cell->Row->ComponentIndex)
            {
                m_manager->Position = cell->Row->ComponentIndex;
            }
        }
    }

    void GridControl::OnColumnInserting(Ntreev::Windows::Forms::Grid::ColumnInsertingEventArgs^ e)
    {
        ColumnInserting(this, e);
    }

    void GridControl::OnColumnInserted(Ntreev::Windows::Forms::Grid::ColumnEventArgs^ e)
    {
        Ntreev::Windows::Forms::Grid::Column^ column = e->Column;
        this->InsertionRow->NewCell(column);
        for each(Ntreev::Windows::Forms::Grid::Row^ item in m_rowList)
        {
            item->NewCell(column);
        }

        ColumnInserted(this, e);
    }

    void GridControl::OnColumnBinding(Ntreev::Windows::Forms::Grid::ColumnBindingEventArgs^ e)
    {
        ColumnBinding(this, e);
    }

    void GridControl::OnColumnBinded(Ntreev::Windows::Forms::Grid::ColumnEventArgs^ e)
    {
        Ntreev::Windows::Forms::Grid::Column^ column = e->Column;
        this->InsertionRow->NewCell(column);
        for each(Ntreev::Windows::Forms::Grid::Row^ item in m_rowList)
        {
            item->NewCell(column);
        }

        ColumnBinded(this, e);
    }

    void GridControl::OnColumnWidthChanged(Ntreev::Windows::Forms::Grid::ColumnEventArgs^ e)
    {
        ColumnWidthChanged(this, e);
    }

    void GridControl::OnColumnFrozenChanged(Ntreev::Windows::Forms::Grid::ColumnEventArgs^ e)
    {
        ColumnFrozenChanged(this, e);
    }

    void GridControl::OnColumnMouseEnter(Ntreev::Windows::Forms::Grid::ColumnMouseEventArgs^ e)
    {
        if(this->Site == nullptr)
        {
            System::Drawing::Point location = e->Column->Bounds.Location;
            location.Y -= 50;

            m_tooltips->Show(e->Column->Tooltip);
        }

        ColumnMouseEnter(this, e);
    }

    void GridControl::OnColumnMouseLeave(Ntreev::Windows::Forms::Grid::ColumnMouseEventArgs^ e)
    {
        if(this->Site == nullptr)
        {
            m_tooltips->Hide();
        }

        ColumnMouseLeave(this, e);
    }

    void GridControl::OnColumnMouseDown(Ntreev::Windows::Forms::Grid::ColumnMouseEventArgs^ e)
    {
        ColumnMouseDown(this, e);
    }

    void GridControl::OnColumnMouseUp(Ntreev::Windows::Forms::Grid::ColumnMouseEventArgs^ e)
    {
        ColumnMouseUp(this, e);
    }

    void GridControl::OnColumnMouseMove(Ntreev::Windows::Forms::Grid::ColumnMouseEventArgs^ e)
    {
        ColumnMouseMove(this, e);
    }

    void GridControl::OnCellMouseMove(Ntreev::Windows::Forms::Grid::CellMouseEventArgs^ e)
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

    void GridControl::OnBeginEdit(Ntreev::Windows::Forms::Grid::BeginEditEventArgs^ e)
    {
        BeginEdit(this, e);
    }

    void GridControl::OnEndEdit(CellEventArgs^ e)
    {
        EndEdit(this, e);
    }

    void GridControl::OnClearing(Ntreev::Windows::Forms::Grid::ClearEventArgs^ e)
    {
        Clearing(this, e);
    }

    void GridControl::OnCleared(Ntreev::Windows::Forms::Grid::ClearEventArgs^ e)
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
            Ntreev::Windows::Forms::Grid::Column^ column = m_visibleColumnList->Count > 0 ? m_visibleColumnList[0] : nullptr;
            Ntreev::Windows::Forms::Grid::Row^ row = m_visibleRowList->Count > 0 ? (Ntreev::Windows::Forms::Grid::Row^)m_visibleRowList[0] : nullptr;
            if(column == nullptr || row == nullptr)
                return;
            GrItem* pItem = row->Cells[column]->NativeRef;
            this->Selector->SelectItem(pItem, GrSelectionType_Normal);
            this->Selector->SetAnchor(pItem);
            this->Focuser->Set(pItem);
        }
        catch(System::Exception^)
        {

        }

        DataBindingComplete(this, e);
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

    Native::WinFormGridCore* GridControl::GridCore::get()
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

    bool GridControl::CanEnableIme::get()
    {
        return true; 
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
            case GrGridState_GroupPressing:
                return true;
            default:
                break;

            }
        }
        catch (System::Exception^)
        {
            return false;
        }

        if (this->DisplayRectangle.Contains(point) == false)
        {
         return true;
        }

        return false;
    }
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/