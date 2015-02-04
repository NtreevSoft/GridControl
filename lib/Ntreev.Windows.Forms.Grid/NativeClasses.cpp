//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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
#include "NativeClasses.h"
#include "GrDCCreator.h"
#include "NativeUtilities.h"
#include "Resources.h"
#include "Cell.h"
#include "Row.h"
#include "Tooltip.h"
#include "TypeEditorForm.h"
#include "CaptionRow.h"
#include "FromNative.h"
#include "RowCollection.h"

#ifdef _DEBUG
//#define _PRINT_INVALIDATE_RECT
#endif



namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Native
{
    /*GrFont* WinFormFontManager::FromManagedFont(System::Drawing::Font^ font)
    {
        if(font == nullptr)
            return nullptr;
        return GrFontCreator::Create(font->ToHfont().ToPointer());
    }

    System::Drawing::Font^ WinFormFontManager::ToManagedFont(GrFont* pFont)
    {
        if(pFont == nullptr)
            return nullptr;
        if(pFont == m_pDefaultfont)
            return System::Windows::Forms::Control::DefaultFont;

        System::Drawing::Font^ font = pFont->ManagedRef;
        if(font == nullptr)
        {
            System::IntPtr ptr(GrFontCreator::GetFontHandle(pFont));
            font = System::Drawing::Font::FromHfont(ptr);
            font = gcnew System::Drawing::Font(font->FontFamily, font->SizeInPoints, font->Style, System::Windows::Forms::Control::DefaultFont->Unit, font->GdiCharSet);
            pFont->ManagedRef = font;
        }
        return font;
    }*/

    WinFormInvalidator::WinFormInvalidator(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl)
        : m_gridControl(gridControl)
    {
        m_rectType = RectType_Empty;
        m_lockRef = 0;
    }

    void WinFormInvalidator::Invalidate()
    {
        if(m_rectType == RectType_Full)
            return;

        if(m_lockRef == 0)
        {
#ifdef _PRINT_INVALIDATE_RECT
            System::Console::Write("full : ");
#endif
            m_gridControl->Invalidate(false);
        }
        m_rectType = RectType_Full;
    }

    void WinFormInvalidator::Invalidate(int x, int y, int width, int height)
    {
        switch(m_rectType)
        {
        case RectType_Full:
            return;
        case RectType_Custom:
            {
                m_rect.left = std::min(x, m_rect.left);
                m_rect.top = std::min(y, m_rect.top);
                m_rect.right = std::max(x + width, m_rect.right);
                m_rect.bottom = std::max(y + height, m_rect.bottom);
            }
            break;
        case RectType_Empty:
            {
                m_rect = GrRect(GrPoint(x, y), GrSize(width, height));
            }
            break;
        }

        if(m_lockRef == 0)
        {
#ifdef _PRINT_INVALIDATE_RECT
            System::Console::Write("custom : ");
#endif
            m_gridControl->Invalidate(m_rect, false);
        }

        m_rectType = RectType_Custom;
    }

    void WinFormInvalidator::Lock()
    {
        m_lockRef++;
    }

    void WinFormInvalidator::Unlock()
    {
        m_lockRef--;
        if(m_lockRef < 0)
            throw gcnew System::Exception("Invalidator의 잠금해제 횟수가 잠금 횟수보다 큽니다.");

        if(m_rectType == RectType_Custom)
        {
#ifdef _PRINT_INVALIDATE_RECT
            System::Console::Write("custom by unlock {0} : ", c++);
#endif

            m_gridControl->Invalidate(m_rect, false);
        }
        else if(m_rectType == RectType_Full)
        {
#ifdef _PRINT_INVALIDATE_RECT
            System::Console::Write("full by unlock : {0}", w++);
#endif
            m_gridControl->Invalidate(false);
        }
        m_rectType = RectType_Empty;
    }

    void WinFormInvalidator::Reset()
    {
        if(m_lockRef == 0)
            m_rectType = RectType_Empty;
    }

    bool WinFormInvalidator::IsInvalidated() const 
    {
        return m_rectType != RectType_Empty; 
    }

    WinFormScroll::WinFormScroll(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl, int type)
        : m_gridControl(gridControl)
    {
        m_type = type;
        m_min = 0;
        m_max = 100;
        m_value = 0;
        m_smallChange = 1;
        m_largeChange = 10;
    }

    WinFormScroll::~WinFormScroll()
    {

    }

    int WinFormScroll::GetValue() const
    {
        return m_value;
    }

    void WinFormScroll::SetValue(int value)
    {
		using namespace System::Windows::Forms;

		int oldValue = m_value;
        if(this->SetValueCore(value) == false)
            return;

		ScrollEventArgs se(ScrollEventType::ThumbPosition, oldValue, m_value, (ScrollOrientation)m_type);
        m_gridControl->InvokeScroll(%se);
    }

    int WinFormScroll::GetSmallChange() const
    {
        return m_smallChange;
    }

    void WinFormScroll::SetSmallChange(int value)
    {
        m_smallChange = value;
    }

    int WinFormScroll::GetLargeChange() const
    {
        return m_largeChange;
    }

    void WinFormScroll::SetLargeChange(int value)
    {
        m_largeChange = value;
        Native::Methods::SetScrollPage(m_gridControl->Handle, m_type, m_largeChange);
    }

    int WinFormScroll::GetMaximum() const
    {
        return m_max;
    }

    void WinFormScroll::SetMaximum(int value)
    {
        m_max = value;
        Native::Methods::SetScrollRange(m_gridControl->Handle, m_type, m_min, m_max);
    }

    int WinFormScroll::GetMinimum() const
    {
        return m_min;
    }

    void WinFormScroll::SetMinimum(int value)
    {
        m_min = value;
        Native::Methods::SetScrollRange(m_gridControl->Handle, m_type, m_min, m_max);
    }

    bool WinFormScroll::GetVisible() const
    {
        return m_visible;
    }

    void WinFormScroll::SetVisible(bool value)
    {
		if(m_type == 0 && m_gridControl->HScrollInternal == false)
			value = false;
		else if(m_type == 1 && m_gridControl->VScrollInternal == false)
			value = false;

		m_visible = value;

		Native::Methods::SetScrollVisible(m_gridControl->Handle, m_type, m_visible);
    }

    void WinFormScroll::WndProc(System::IntPtr handle, System::IntPtr wParam)
    {
        using namespace System::Windows::Forms;

        int nValue = m_value;

        ScrollEventType ScrollType;

        switch(Native::Methods::LoWord(wParam))
        {
        case SB_ENDSCROLL:
            {
                ScrollType = ScrollEventType::EndScroll;
            }
            break;
        case SB_LEFT:
            {
                nValue = m_min;
                ScrollType = ScrollEventType::First;
            }
            break;
        case SB_RIGHT:
            {
                nValue = m_max;
                ScrollType = ScrollEventType::Last;
            }
            break;
        case SB_LINELEFT:
            {
                nValue -= m_smallChange;
                ScrollType = ScrollEventType::SmallDecrement;
            }
            break;
        case SB_LINERIGHT:
            {
                nValue += m_smallChange;
                ScrollType = ScrollEventType::SmallIncrement;
            }
            break;
        case SB_PAGELEFT:
            {
                nValue -= m_largeChange;
                ScrollType = ScrollEventType::LargeDecrement;
            }
            break;
        case SB_PAGERIGHT:
            {
                nValue += m_largeChange;
                ScrollType = ScrollEventType::LargeIncrement;
            }
            break;
        case SB_THUMBTRACK:
            {
                if(Native::Methods::GetScrollTrackPosition(handle, m_type, &nValue) == false)
                    return;
                ScrollType = ScrollEventType::ThumbTrack;
            }
            break;
        default:
            return;
        }

        SetValue(nValue, (int)ScrollType);
    }

    void WinFormScroll::SetValue(int value, int scrollEventType)
    {
        using namespace System::Windows::Forms;

        int oldValue = m_value;
        if(this->SetValueCore(value) == false)
            return;

        ScrollEventArgs se((ScrollEventType)scrollEventType, oldValue, m_value, (ScrollOrientation)m_type);
        m_gridControl->InvokeScroll(%se);
    }

    bool WinFormScroll::SetValueCore(int value)
    {
        int oldValue = m_value;
        int newValue = this->ValidateValue(value);

        if(oldValue == newValue)
            return false;

        m_value = newValue;
        Native::Methods::SetScrollValue(m_gridControl->Handle, m_type, newValue);
        return true;
    }


    WinFormTimer::Timer::Timer(WinFormTimer* winformTimer, Ntreev::Windows::Forms::Grid::GridControl^ gridControl)
        : m_winformTimer(winformTimer)
    {
        BeginInit();
        Enabled = false;
        SynchronizingObject = gridControl;
        AutoReset = true;
        EndInit();

        this->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &Timer::elapsed);
    }

    void WinFormTimer::Timer::elapsed(System::Object^ /*sender*/, System::Timers::ElapsedEventArgs^ e)
    {
        m_winformTimer->Invoke(e->SignalTime.Millisecond);
    }

    WinFormTimer::WinFormTimer(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl)
        : m_gridControl(gridControl)
    {
        m_timer = gcnew Timer(this, m_gridControl);
        m_timer->AutoReset = true;
    }

    void WinFormTimer::Start()
    {
        m_timer->Start();
    }

    void WinFormTimer::Stop()
    {
        m_timer->Stop();
    }

    void WinFormTimer::SetInterval(time_t interval)
    {
        m_timer->Interval = (double)interval;
    }

    void WinFormTimer::Invoke(time_t signalTime)
    {
        InvokeElapsed(signalTime);
    }

    WinFormWindow::WinFormWindow(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl)
        : m_gridControl(gridControl), m_horzScroll(m_gridControl, 0), m_vertScroll(m_gridControl, 1), m_invalidator(m_gridControl)
    {
        m_pGridPainter = CreateGridPainterDC(m_gridControl->Handle.ToPointer());
        System::IO::Stream^ stream;
        stream = Ntreev::Windows::Forms::Grid::GridControl::typeid->Assembly->GetManifestResourceStream("Ntreev.Windows.Forms.Grid.Cursors.Movable.cur");
        m_cursorMove = gcnew System::Windows::Forms::Cursor(stream);
        delete stream;

        stream = Ntreev::Windows::Forms::Grid::GridControl::typeid->Assembly->GetManifestResourceStream("Ntreev.Windows.Forms.Grid.Cursors.Add.cur");
        m_cursorAdd = gcnew System::Windows::Forms::Cursor(stream);
        delete stream;

        stream = Ntreev::Windows::Forms::Grid::GridControl::typeid->Assembly->GetManifestResourceStream("Ntreev.Windows.Forms.Grid.Cursors.Remove.cur");
        m_cursorRemove = gcnew System::Windows::Forms::Cursor(stream);
        delete stream;

        //m_pFont = WinFormFontManager::FromManagedFont(m_gridControl->Font);
    }

    GrRect WinFormWindow::GetSrceenRect() const
    {
        return System::Windows::Forms::Screen::PrimaryScreen->Bounds;
    }

    GrPoint WinFormWindow::ClientToScreen(const GrPoint& location) const
    {
        return m_gridControl->PointToScreen(location);
    }

    int WinFormWindow::GetMouseWheelScrollLines() const
    {
        return System::Windows::Forms::SystemInformation::MouseWheelScrollLines;
    }

    int WinFormWindow::GetMouseWheelScrollDelta() const
    {
        return System::Windows::Forms::SystemInformation::MouseWheelScrollDelta;
    }

    GrSize WinFormWindow::GetDragSize() const
    {
        return System::Windows::Forms::SystemInformation::DragSize;
    }

    bool WinFormWindow::GetMouseDragEventSupported() const
    {
        return false;
    }

    bool WinFormWindow::SetCursor(int cursor)
    {
        using namespace System::Windows::Forms;
        Cursor^ temp;
        switch(cursor)
        {
        case GrCursor_No:
            temp = Cursors::No;
            break;
        case GrCursor_Wait:
            temp = Cursors::WaitCursor;
            break;
        case GrCursor_Add:
            temp = m_cursorAdd;
            break;
        case GrCursor_Remove:
            temp = m_cursorRemove;
            break;
        case GrCursor_Move:
            temp = m_cursorMove;
            break;
        case GrCursor_HSplit:
            temp = Cursors::HSplit;
            break;
        case GrCursor_VSplit:
            temp = Cursors::VSplit;
            break;
        case GrCursor_SizeWE:
            temp = Cursors::SizeWE;
            break;
        case GrCursor_SizeNS:
            temp = Cursors::SizeNS;
            break;
        default:
            temp = Cursors::Default;
            break;
        }

        //if(m_gridControl->Site != nullptr)
        //{
        // m_gridControl->Cursor = temp;
        // //m_gridControl->CaptionRow->Text = temp->ToString();
        // //return System::Windows::Forms::Cursor::Current != System::Windows::Forms::Cursors::Default;
        // return true;
        //}

        if(m_gridControl->Cursor != temp)
        {
            m_gridControl->Cursor = temp;
#ifdef _DEBUG
            System::Diagnostics::Trace::WriteLine(temp);
#endif
            if(m_gridControl->Site != nullptr)
            {
                System::Windows::Forms::Cursor::Current = temp;
            }

            return true;
        }

        return false;
    }

    GrKeys WinFormWindow::GetModifierKeys() const
    {
        using namespace System::Windows::Forms;

        int modifierKeys = 0;
        if((Control::ModifierKeys & Keys::Control) == Keys::Control)
            modifierKeys |= GrKeys_Control;
        if((Control::ModifierKeys & Keys::Shift) == Keys::Shift)
            modifierKeys |= GrKeys_Shift;
        if((Control::ModifierKeys & Keys::Alt) == Keys::Alt)
            modifierKeys |= GrKeys_Alt;
        return (GrKeys)modifierKeys;
    }

    void WinFormWindow::OnEditValue(GrEditEventArgs* e)
    {
        using namespace System::Windows::Forms;
        using namespace Ntreev::Windows::Forms::Grid::Design;

        GrEditingReason reason = e->GetReason();
        Ntreev::Windows::Forms::Grid::Cell^ cell = FromNative::Get(e->GetItem());
        Ntreev::Windows::Forms::Grid::Row^ row = cell->Row;

        if(m_gridControl->InvokeEditBegun(cell) == false)
            return;

        //m_gridControl->Refresh();
        TypeEditorForm^ form = gcnew TypeEditorForm(m_gridControl, cell, EditingReason(reason));
        m_gridControl->Update();

        try
        {
            System::Object^ newValue = cell->Column->EditValueInternal(form, cell, cell->Value);

            if(form->DialogResult != DialogResult::Cancel && newValue != nullptr)
            {
                if(row != m_gridControl->InsertionRow && cell->Row->Index < 0)
                    throw gcnew System::Exception("행이 삭제 되었을수도 있습니다.");

                if(System::Object::Equals(newValue, cell->Value) == false)
                {
                    cell->Value = newValue;
                    e->SetHandled(true);
                }
				//m_gridControl->Update();
            }
        }
        catch(System::Exception^ e1)
        {
            m_gridControl->ShowMessage(e1->Message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            //cell->CancelEdit();
        }
        finally
        {
            delete form;
        }

        CellEventArgs ee(cell);
        m_gridControl->InvokeEditEnded(%ee);
    }

    GrScroll* WinFormWindow::GetHorzScroll() const
    {
        return const_cast<WinFormScroll*>(&m_horzScroll);
    }

    GrScroll* WinFormWindow::GetVertScroll() const
    {
        return const_cast<WinFormScroll*>(&m_vertScroll);
    }

    GrInvalidator* WinFormWindow::GetInvalidator() const
    {
        return const_cast<WinFormInvalidator*>(&m_invalidator);
    }

    GrGridPainter* WinFormWindow::GetGridPainter() const
    {
        return m_pGridPainter;
    }

    GrTimer* WinFormWindow::CreateTimer()
    {
        return new WinFormTimer(m_gridControl);
    }

    void WinFormWindow::DestroyTimer(GrTimer* pTimer)
    {
        delete pTimer;
    }

    bool WinFormWindow::CanEdit(GrItem* pItem, GrEditingReason reason)
    {
        Ntreev::Windows::Forms::Grid::Cell^ cell = 
            FromNative::Get(pItem);

        //if(GrGridWindow::CanEdit(pItem, reason) == false)
        return cell->Column->CanEditInternal(cell, Ntreev::Windows::Forms::Grid::EditingReason(reason));
    }

    WinFormGridCore::WinFormGridCore(gcroot<Ntreev::Windows::Forms::Grid::GridControl^> gridControl, GrGridWindow* pGridWindow)
        : GrGridCore(pGridWindow), m_gridControl(gridControl)
    {
        GrColumnList* pColumnList = GetColumnList();
        pColumnList->ColumnMouseDown.Add(this, &WinFormGridCore::columnList_ColumnMouseDown);
        pColumnList->ColumnMouseEnter.Add(this, &WinFormGridCore::columnList_ColumnMouseEnter);
        pColumnList->ColumnMouseLeave.Add(this, &WinFormGridCore::columnList_ColumnMouseLeave);
        pColumnList->ColumnMouseMove.Add(this, &WinFormGridCore::columnList_ColumnMouseMove);
        pColumnList->ColumnMouseUp.Add(this, &WinFormGridCore::columnList_ColumnMouseUp);
        pColumnList->ColumnWidthChanged.Add(this,&WinFormGridCore::columnList_ColumnWidthChanged);
        pColumnList->ColumnFrozenChanged.Add(this,&WinFormGridCore::columnList_ColumnFrozenChanged);
		pColumnList->ColumnVisibleIndexChanged.Add(this,&WinFormGridCore::columnList_ColumnVisibleIndexChanged);

        GrFocuser* pFocuser = GetFocuser();
        pFocuser->FocusChanging.Add(this, &WinFormGridCore::focuser_FocusChanging);
        pFocuser->FocusChanged.Add(this, &WinFormGridCore::focuser_FocusChanged);

        GrItemSelector* pItemSelector = GetItemSelector();
        pItemSelector->SelectedRowsChanged.Add(this, &WinFormGridCore::itemSelector_SelectedRowsChanged);
        pItemSelector->SelectedColumnsChanged.Add(this, &WinFormGridCore::itemSelector_SelectedColumnsChanged);
        pItemSelector->SelectionChanged.Add(this, &WinFormGridCore::itemSelector_SelectionChanged);

        GrCaption* pCaption = GetCaptionRow();
        pCaption->HeightChanged.Add(this, &WinFormGridCore::caption_HeightChanged);

		GrDataRowList* pDataRowList = GetDataRowList();
		pDataRowList->DataRowMoved.Add(this, &WinFormGridCore::dataRowList_DataRowMoved);
		pDataRowList->DataRowMoving.Add(this, &WinFormGridCore::dataRowList_DataRowMoving);
    }

    void WinFormGridCore::OnItemMouseMove(GrItemMouseEventArgs* e)
    {
        GrGridCore::OnItemMouseMove(e);
        if(m_gridControl->InvokeCellMouseMove(FromNative::Get(e->GetItem()), e->GetLocation()) == true)
        {
            e->SetHandled(true);
        }
    }

    void WinFormGridCore::OnItemMouseClick(GrItemMouseEventArgs* e)
    {
        GrGridCore::OnItemMouseClick(e);
        m_gridControl->InvokeCellClick(FromNative::Get(e->GetItem()));
    }

    void WinFormGridCore::OnItemMouseDoubleClick(GrItemMouseEventArgs* e)
    {
        GrGridCore::OnItemMouseDoubleClick(e);
        m_gridControl->InvokeCellDoubleClick(FromNative::Get(e->GetItem()));
    }

    void WinFormGridCore::OnItemMouseEnter(GrItemMouseEventArgs* e)
    {
        GrGridCore::OnItemMouseEnter(e);
        m_gridControl->InvokeCellMouseEnter(FromNative::Get(e->GetItem()));
    }

    void WinFormGridCore::OnItemMouseLeave(GrItemMouseEventArgs* e)
    {
        GrGridCore::OnItemMouseLeave(e);
        m_gridControl->InvokeCellMouseLeave(FromNative::Get(e->GetItem()));
    }

    void WinFormGridCore::OnRowMouseEnter(GrRowMouseEventArgs* e)
    {
        GrGridCore::OnRowMouseEnter(e);
        GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(e->GetRow());
        if(pDataRow == nullptr)
            return;

        try
        {
            Ntreev::Windows::Forms::Grid::Row^ row = FromNative::Get(pDataRow);
			System::String^ error = row->Error;
			System::String^ sourceError = row->SourceError;
			if(error != System::String::Empty)
			{
				m_gridControl->ToolTip->Show(error);
			}
			else if(sourceError != System::String::Empty)
			{
				m_gridControl->ToolTip->Show(sourceError);
			}
        }
        catch(System::Exception^)
        {
        }
    }

    void WinFormGridCore::OnRowMouseMove(GrRowMouseEventArgs* /*e*/)
    {

    }

    void WinFormGridCore::OnRowMouseLeave(GrRowMouseEventArgs* e)
    {
        GrGridCore::OnRowMouseLeave(e);
        m_gridControl->ToolTip->Hide();
    }

    void WinFormGridCore::PostPaint(GrGridPainter* pPainter, const GrRect& clipRect) const
    {
        System::Drawing::Graphics^ graphics = System::Drawing::Graphics::FromHdc(System::IntPtr(pPainter->GetDevice()));
        m_gridControl->PostPaint(graphics, clipRect);
        delete graphics;
        GrGridCore::PostPaint(pPainter, clipRect);
    }

    void WinFormGridCore::columnList_ColumnMouseDown(GrObject* /*pSender*/, GrColumnMouseEventArgs* e)
    {
        Column^ column = FromNative::Get(e->GetColumn());
        if(m_gridControl->Site == nullptr)
        {
            bool handled = m_gridControl->InvokeColumnMouseDown(column, e->GetLocation());
            e->SetHandled(handled);
            return;
        }

        using namespace System::ComponentModel::Design;
        ISelectionService^ selectionService = (ISelectionService^)m_gridControl->GetInternalService(ISelectionService::typeid);
        cli::array<System::Object^>^ components = gcnew cli::array<System::Object^>(1) { column, };
        selectionService->SetSelectedComponents(components);
        e->SetHandled(true);
    }

    void WinFormGridCore::columnList_ColumnMouseUp(GrObject* /*pSender*/, GrColumnMouseEventArgs* e)
    {
        Column^ column = FromNative::Get(e->GetColumn());
        bool handled = m_gridControl->InvokeColumnMouseUp(column, e->GetLocation());
        e->SetHandled(handled);
    }

    void WinFormGridCore::columnList_ColumnMouseEnter(GrObject* /*pSender*/, GrColumnMouseEventArgs* e)
    {
        Column^ column = FromNative::Get(e->GetColumn());
        m_gridControl->InvokeColumnMouseEnter(column, e->GetLocation());
    }

    void WinFormGridCore::columnList_ColumnMouseLeave(GrObject* /*pSender*/, GrColumnMouseEventArgs* e)
    {
        Column^ column = FromNative::Get(e->GetColumn());
        m_gridControl->InvokeColumnMouseLeave(column);
    }

    void WinFormGridCore::columnList_ColumnMouseMove(GrObject* /*pSender*/, GrColumnMouseEventArgs* e)
    {
        Column^ column = FromNative::Get(e->GetColumn());
        if(m_gridControl->InvokeColumnMouseMove(column, e->GetLocation()) == true)
        {
            e->SetHandled(true);
        }
    }

    void WinFormGridCore::columnList_ColumnWidthChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
    {
        Column^ column = FromNative::Get(e->GetColumn());
        m_gridControl->InvokeColumnWidthChanged(column);
    }

    void WinFormGridCore::columnList_ColumnFrozenChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
    {
        Column^ column = FromNative::Get(e->GetColumn());
        m_gridControl->InvokeColumnFrozenChanged(column);
    }

	void WinFormGridCore::columnList_ColumnVisibleIndexChanged(GrObject* /*pSender*/, GrColumnEventArgs* e)
    {
        Column^ column = FromNative::Get(e->GetColumn());
        m_gridControl->InvokeColumnVisibleIndexChanged(column);
    }

    void WinFormGridCore::focuser_FocusChanging(GrObject* /*pSender*/, GrFocusChangeArgs* /*e*/)
    {
        m_gridControl->InvokeFocusChanging();
    }

    void WinFormGridCore::focuser_FocusChanged(GrObject* /*pSender*/, GrFocusChangeArgs* /*e*/)
    {
        m_gridControl->InvokeFocusChanged();
    }

    void WinFormGridCore::itemSelector_SelectedRowsChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
    {
        m_gridControl->InvokeSelectedRowsChanged();
    }

    void WinFormGridCore::itemSelector_SelectedColumnsChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
    {
        m_gridControl->InvokeSelectedColumnsChanged();
    }

    void WinFormGridCore::itemSelector_SelectionChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
    {
        m_gridControl->InvokeSelectionChanged();
    }

    void WinFormGridCore::caption_HeightChanged(GrObject* /*pSender*/, GrEventArgs* /*e*/)
    {
        if(m_gridControl->Site == nullptr)
            return;

        using namespace System::ComponentModel;
        using namespace System::ComponentModel::Design;
        IComponentChangeService^ service = dynamic_cast<IComponentChangeService^>(m_gridControl->GetInternalService(IComponentChangeService::typeid));
        PropertyDescriptor^ propertyDescriptor = TypeDescriptor::GetProperties(m_gridControl)["Caption"];
        service->OnComponentChanging(m_gridControl, propertyDescriptor);
        service->OnComponentChanged(m_gridControl, propertyDescriptor, nullptr, nullptr);
    }

	void WinFormGridCore::dataRowList_DataRowMoved(GrObject* /*pSender*/, GrDataRowEventArgs* e)
	{
		Row^ row = FromNative::Get(e->GetDataRow());
		m_gridControl->InvokeRowMoved(row);
	}

	void WinFormGridCore::dataRowList_DataRowMoving(GrObject* /*pSender*/, GrDataRowMovingEventArgs* e)
	{
		Row^ row = FromNative::Get(e->GetDataRow());
		if(m_gridControl->InvokeRowMoving(row, e->GetIndex()) == false)
			e->SetCancel(true);
	}
} /*namespace Native*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/