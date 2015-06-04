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
#include "NativeClasses.h"
#include "NativeGridRow.h"
#include "TimeTester.h"
#include "FromNative.h"
#include "About.h"
#include "Tooltip.h"
#include "ErrorDescriptor.h"
#include "RowBuilder.h"
#include "Style.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	using namespace Native;
	using namespace System::Windows::Forms;

	GridControl::GridControl()
	{
#ifdef _TIME_TEST
		TimeTester timeTest("GridControl 생성자");
#endif
		this->SetStyle(System::Windows::Forms::ControlStyles::ContainerControl, true);
		m_pGridWindow = new Native::WinFormWindow(this);
		m_pGridCore = new Native::WinFormGridCore(this, m_pGridWindow);
		m_pGridCore->SetFont(GrFont::FromManaged(this->Font));
		ResetBackColor();
		m_pGridCore->SetForeColor(this->ForeColor);
		m_pGridCore->SetBackColor(this->BackColor);
		m_pGridPainter = m_pGridWindow->GetGridPainter();

		m_pColumnList = m_pGridCore->GetColumnList();
		m_pDataRowList = m_pGridCore->GetDataRowList();

		m_pGridCore->SetDisplayRect(this->DisplayRectangle);

#ifdef _TIME_TEST
		Rendering = true;
#endif

		m_dataSource = nullptr;
		m_dataMember = System::String::Empty;
		m_columnBindingCreation = _ColumnBindingCreation::Create;

		m_tooltips = gcnew Ntreev::Windows::Forms::Grid::ToolTip(this, 3);
		m_errorDescriptor = gcnew Ntreev::Windows::Forms::Grid::ErrorDescriptor(this);

		m_columnList = gcnew ColumnCollection(this);
		m_visibleColumnList = gcnew VisibleColumnCollection(this);
		m_displayableColumnList = gcnew DisplayableColumnCollection(this);
		m_backgroundColor = System::Drawing::SystemColors::Control;
		m_paddingColor = System::Drawing::SystemColors::ActiveCaption;
		m_lineColor = GrStyle::Default.LineColor;

		m_unfrozenColumnList = gcnew UnfrozenColumnCollection(this);
		m_rowList = gcnew RowCollection(this);
		m_visibleRowList = gcnew VisibleRowCollection(this);
		m_displayableRowList = gcnew DisplayableRowCollection(this);

		m_selectedColumns = gcnew SelectedColumnCollection(this, this->Selector->GetSelectedColumns());
		m_selectedRows = gcnew SelectedRowCollection(this, this->Selector->GetSelectedRows());

		m_groupRows = gcnew GroupRowCollection(this);

		m_captionRow = gcnew Ntreev::Windows::Forms::Grid::CaptionRow(this, m_pGridCore->GetCaptionRow());
		m_groupPanel = gcnew Ntreev::Windows::Forms::Grid::GroupPanel(this, m_pGridCore->GetGroupPanel());

		m_rowBuilder = gcnew Ntreev::Windows::Forms::Grid::RowBuilder();

		m_defaultMessageBoxCallback = gcnew Ntreev::Windows::Forms::Grid::MessageBoxCallback(this, &GridControl::DefaultShowMessage);

#if _MSC_VER >= 1600
#pragma warning(disable:4564)
		m_defaultDataSource = gcnew System::Data::DataTable();
#pragma warning(default:4564)
#endif

		m_defaultManager = dynamic_cast<System::Windows::Forms::CurrencyManager^>(this->BindingContext[m_defaultDataSource]);
		m_listChangedEventHandler = gcnew System::ComponentModel::ListChangedEventHandler(this, &GridControl::currencyManager_ListChanged);
		m_bindingCompleteEventHandler = gcnew System::Windows::Forms::BindingCompleteEventHandler(this, &GridControl::currencyManager_BindingComplete);
		m_styleDisposedEventHandler = gcnew System::EventHandler(this, &GridControl::style_Disposed);

		this->DoubleBuffered = true;
		this->Name = L"GridControl";

		OnCurrencyManagerChanging(gcnew CurrencyManagerChangingEventArgs(m_defaultManager));
		OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(m_defaultManager));
	}

	GridControl::~GridControl()
	{
		if(m_pGridCore == nullptr)
			return;

		m_disposing = true;

		OnCurrencyManagerChanging(gcnew CurrencyManagerChangingEventArgs(nullptr));
		OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(nullptr));

		this->Style = nullptr;

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

		m_disposing = false;
	}

	bool GridControl::ProcessTabKey(bool /*forward*/)
	{
		return true;
	}

	Row^ GridControl::NewRowFromBuilder(RowBuilder^ rowBuilder)
	{
		return gcnew Row(rowBuilder);
	}

	GridControl^ GridControl::NewChildGridControl(System::ComponentModel::PropertyDescriptor^ descriptor, Row^ /*row*/)
	{
		GridControl^ control = dynamic_cast<GridControl^>(System::Activator::CreateInstance(this->GetType()));
		control->Name = descriptor->Name;
		control->Caption = descriptor->Name;
		control->IsCaptionRowVisible = this->IsCaptionRowVisible;
		control->IsGroupPanelVisible = false;
		control->IsFrozingSplitterVisible = this->IsFrozingSplitterVisible;
		//control->Padding = System::Windows::Forms::Padding((int)this->Font->Size);
		control->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		//control->Padding = System::Windows::Forms::Padding(1);
		control->Visible = false;
		control->AutoFitColumn = this->AutoFitColumn;
		control->AutoFitRow = this->AutoFitRow;
		control->ReadOnly = this->ReadOnly;
		control->Font = this->Font;

		return control;
	}

	void GridControl::childGridControl_FocusedCellChanged(System::Object^ sender, CellEventArgs^ e)
	{
		GridControl^ control = dynamic_cast<GridControl^>(sender);

		if(e->Cell != nullptr)
		{
			this->Selector->ClearSelection();
			this->Focuser->Set(control->m_pGridRow);
		}
	}

#ifdef _DEBUG
	void GridControl::OnInvalidated(System::Windows::Forms::InvalidateEventArgs^ e)
	{
		UserControl::OnInvalidated(e);
		//System::Diagnostics::Debug::WriteLine(e->InvalidRect);
	}

	void GridControl::OnCursorChanged(System::EventArgs^ e)
	{
		UserControl::OnCursorChanged(e);
		//System::Diagnostics::Debug::WriteLine("OnCursorChanged");
	}
#endif

	void GridControl::OnGotFocus(System::EventArgs^ e)
	{
		m_pGridWindow->OnGotFocus();
		UserControl::OnGotFocus(e);

		//System::Console::WriteLine(this->Handle);
	}

	void GridControl::OnEnter(System::EventArgs^ e)
	{
		UserControl::OnEnter(e);

		if(this->FocusedRow == this->InsertionRow)
		{
			this->Rows->BeginInsertion();
		}
	}

	void GridControl::OnLeave(System::EventArgs^ e)
	{
		UserControl::OnLeave(e);

		if(this->FocusedRow == this->InsertionRow)
		{
			this->Rows->EndInsertion();
		}
	}

	void GridControl::OnLostFocus(System::EventArgs^ e)
	{
		if(this->EditingCell == nullptr && this->FocusedRow != this->InsertionRow)
		{
			Row^ row = dynamic_cast<Row^>(this->FocusedRow);
			if(row != nullptr)
			{
				this->EndCurrentEdit(row);
			}
		}

		m_pGridWindow->OnLostFocus();
		UserControl::OnLostFocus(e);
	}

	void GridControl::OnLayout(System::Windows::Forms::LayoutEventArgs^ e)
	{
		UserControl::OnLayout(e);
		m_pGridWindow->OnSizeChanged(this->DisplayRectangle);
		m_pGridCore->Update();
	}

	void GridControl::OnForeColorChanged(System::EventArgs^ e)
	{
		m_pGridCore->SetForeColor(this->ForeColor);
		UserControl::OnForeColorChanged(e);
	}

	void GridControl::OnBackColorChanged(System::EventArgs^ e)
	{
		m_pGridCore->SetBackColor(this->BackColor);
		UserControl::OnBackColorChanged(e);
	}

	void GridControl::OnBackgroundColorChanged(System::EventArgs^ e)
	{
		this->BackgroundColorChanged(this, e);
		this->Invalidate();
	}

	void GridControl::OnPaddingColorChanged(System::EventArgs^ e)
	{
		this->BackgroundColorChanged(this, e);
		this->Invalidate();
	}

	void GridControl::OnLineColorChanged(System::EventArgs^ e)
	{
		m_pGridCore->SetLineColor(this->LineColor);
		this->LineColorChanged(this, e);
	}

	void GridControl::OnScroll(System::Windows::Forms::ScrollEventArgs^ e)
	{
		this->Scroll(this, e);
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

	void GridControl::OnPaintBackground(System::Windows::Forms::PaintEventArgs^ e)
	{
		using namespace System::Drawing;

		if(this->BackgroundImage != nullptr)
		{
			try
			{
				m_paintBackground = true;
				UserControl::OnPaintBackground(e);
			}
			finally
			{
				m_paintBackground = false;
			}
		}
		else
		{
			SolidBrush br(m_backgroundColor);
			SolidBrush br2(m_paddingColor);
			e->Graphics->FillRectangle(%br2, e->ClipRectangle);
			e->Graphics->FillRectangle(%br, this->DisplayRectangle);
		}
	}

	Ntreev::Windows::Forms::Grid::Row^ GridControl::CreateRow(GrDataRow* pDataRow)
	{
		m_rowBuilder->GridControl = this;
		m_rowBuilder->NativeRef = pDataRow;
		return NewRowFromBuilder(m_rowBuilder);
	}

	void GridControl::EndCurrentEdit(Row^ row)
	{
		try
		{
			m_manager->EndCurrentEdit();
			row->EndEditInternal();
		}
		catch(System::Exception^ e)
		{
			m_manager->CancelCurrentEdit();
			row->CancelEdit();
			this->ShowMessage(e->Message, "Error", System::Windows::Forms::MessageBoxButtons::OK, System::Windows::Forms::MessageBoxIcon::Error);
		}
	}

	void GridControl::DeleteFocusedCell()
	{
		this->Focuser->Delete();
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

		GrItem* pMouseOvered = nullptr;

		for(auto pColumn : m_pColumnList->Displayables)
		{
			if(pColumn->IntersectsHorzWith(clipRectangle) == false)
				continue;

			Column^ column = m_columnList[pColumn];
			ViewType viewType = column->ViewType;
			if(viewType == ViewType::Text)
				continue;

			//unsigned int drows = m_pDataRowList->GetDisplayableRowCount();
			for(unsigned int j=0 ; j<m_pDataRowList->GetDisplayableRowCount() + 1 ; j++)
			{
				GrDataRow* pDataRow = nullptr;
				if(j==0)
					pDataRow = pInsertionRow->GetVisible() == true ? pInsertionRow : nullptr;
				else
					pDataRow = dynamic_cast<GrDataRow*>(m_pDataRowList->GetDisplayableRow(j - 1));

				if(pDataRow == nullptr)
					continue;

				if(pDataRow->IntersectsVertWith(clipRectangle) == false)
					continue;

				GrItem* pItem = pDataRow->GetItem(pColumn);
				if(pItem->GetMouseOvered() == true)
					pMouseOvered = pItem;
				else
					this->PaintCell(graphics, clipRectangle, pItem);
			}
		}

		if(pMouseOvered != nullptr)
			this->PaintCell(graphics, clipRectangle, pMouseOvered);
	}

	void GridControl::PaintCell(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle clipRectangle, GrItem* pItem)
	{
		Cell^ cell = FromNative::Get(pItem);

		Column^ column = cell->Column;
		ViewType viewType = column->ViewType;
		System::Drawing::Rectangle paintRect = pItem->GetClientRect();
		paintRect.Offset(pItem->GetLocation());

		if(viewType == ViewType::Icon)
		{
			paintRect.Width = DEF_ICON_SIZE;
			paintRect.X -= (DEF_ICON_SIZE + column->CellPadding.Left);
		}

		if(pItem->GetClipped() == true)
			paintRect.Intersect(clipRectangle);
		column->PaintValue(graphics, paintRect, cell, cell->DisplayValue);
	}

	void GridControl::PaintRowState(System::Drawing::Graphics^ g)
	{
		using namespace System::Drawing;

		IDataRow* pFocusedRow = this->Focuser->GetFocusedRow();
		if(pFocusedRow == nullptr || pFocusedRow->GetDisplayable() == false)
			return;

		Row^ row = dynamic_cast<Row^>(this->FocusedRow);

		if(EditingCell != nullptr)
		{
			GrRect bound = EditingCell->NativeRef->GetDataRow()->GetRect();
			GrPoint center = bound.GetCenter();

			Bitmap^ image = _Resources::RowEditing;
			center.x -= (image->Width / 2);
			center.y -= (image->Height / 2);
			g->DrawImageUnscaled(image, center.x, center.y);
		}
		else if(row && row->IsEdited == true)
		{
			GrRect bound = pFocusedRow->GetRect();
			GrPoint center = bound.GetCenter();

			Bitmap^ image = _Resources::RowEditing;
			center.x -= (image->Width / 2);
			center.y -= (image->Height / 2);
			g->DrawImageUnscaled(image, center.x, center.y);
		}
		else if(pFocusedRow->GetDisplayable() == true)
		{
			GrRect bound = pFocusedRow->GetRect();
			GrPoint center = bound.GetCenter();

			Bitmap^ image = _Resources::RowFocused;
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

			Bitmap^ image = _Resources::InsertionRow;
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

		CurrencyManagerChangingEventArgs e1(manager);
		OnCurrencyManagerChanging(%e1);

		if(e1.Cancel == true)
		{
			throw gcnew System::NotSupportedException(e1.CancelReason);
		}

		bool managerChanged = m_manager != manager;

		if(managerChanged == true)
			ClearCore(true);

		m_dataSource = dataSource;
		m_dataMember = dataMember;

		CurrencyManagerChangedEventArgs e2(manager);
		OnCurrencyManagerChanged(%e2);

		if(managerChanged == true)
		{
			OnDataBindingComplete(System::EventArgs::Empty);
			m_pGridCore->Invalidate();
		}

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

	//void GridControl::Update()
	//{
	//	m_pGridCore->Update();

	//	UserControl::Update();
	//}

	void GridControl::Update(bool forceUpdate)
	{
		m_pGridCore->Update();
		if(forceUpdate == true)
			UserControl::Update();
	}

	Ntreev::Windows::Forms::Grid::MessageBoxCallback^ GridControl::MessageBoxCallback::get()
	{
		if(m_messageBoxCallback == nullptr)
			return m_defaultMessageBoxCallback;
		return m_messageBoxCallback;
	}

	void GridControl::MessageBoxCallback::set(Ntreev::Windows::Forms::Grid::MessageBoxCallback^ value)
	{
		m_messageBoxCallback = value;
	}

	DialogResult GridControl::ShowMessage(System::String^ text)
	{
		return this->ShowMessage(text, MessageBoxButtons::OK);
	}

	DialogResult GridControl::ShowMessage(System::String^ text, MessageBoxButtons buttons)
	{
		return this->ShowMessage(text, System::String::Empty, buttons);
	}

	DialogResult GridControl::ShowMessage(System::String^ text, MessageBoxButtons buttons, MessageBoxIcon icon)
	{
		return this->ShowMessage(text, System::String::Empty, buttons, icon);
	}

	DialogResult GridControl::ShowMessage(System::String^ text, System::String^ caption, MessageBoxButtons buttons)
	{
		return this->ShowMessage(text, caption, buttons, MessageBoxIcon::None);
	}

	DialogResult GridControl::ShowMessage(System::String^ text, System::String^ caption, MessageBoxButtons buttons, MessageBoxIcon icon)
	{
		return this->MessageBoxCallback(text, caption, buttons, icon);
	}

	bool GridControl::ShowQuestion(System::String^ message)
	{
		return this->ShowMessage(message, "Question", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == DialogResult::Yes;
	}

	void GridControl::ShowError(System::String^ message)
	{
		this->ShowMessage(message, "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}

	DialogResult GridControl::DefaultShowMessage(System::String^ text, System::String^ caption, MessageBoxButtons buttons, MessageBoxIcon icon)
	{
		return MessageBox::Show(text, caption, buttons, icon);
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

	Cell^ GridControl::GetFirstVisibleCell()
	{
		Row^ firstRow;
		if(this->InsertionRow->IsVisible == true)
		{
			firstRow = this->InsertionRow;
		}
		else if(this->Rows->Count > 0)
		{
			firstRow = this->Rows[0];
		}

		if(firstRow != nullptr && firstRow->Cells->Count > 0)
		{
			return firstRow->Cells[0];
		}

		return nullptr;
	}

	void GridControl::WndProc(System::Windows::Forms::Message% m)
	{
		switch(m.Msg)
		{
			//case Native::WM::WM_SETFOCUS:
			//    {
			//        this->DefWndProc(m);
			//        this->OnGotFocus(System::EventArgs::Empty);
			//    }
			//    return;
			//case Native::WM::WM_KILLFOCUS:
			//    {
			//        this->DefWndProc(m);
			//        this->OnLostFocus(System::EventArgs::Empty);
			//    }
			//    return;
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

		//if(m_pGridRow == nullptr)
		//{
		//    this->FocusInternal();
		//}
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

	_ColumnBindingCreation GridControl::ColumnBindingCreation::get()
	{
		return m_columnBindingCreation;
	}

	void GridControl::ColumnBindingCreation::set(_ColumnBindingCreation value)
	{
		if(m_columnBindingCreation == value)
			return;
		m_columnBindingCreation = value;
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
		Ntreev::Windows::Forms::Grid::Cell^ focusedCell = FocusedCell;
		if(focusedCell == nullptr)
			return nullptr;
		return focusedCell->Column;
	}

	void GridControl::OnFontChanged(System::EventArgs^ e)
	{
		m_pGridCore->SetFont(GrFont::FromManaged(this->Font));
		UserControl::OnFontChanged(e);
#ifdef _DEBUG
		//System::Diagnostics::Debug::WriteLine("Invalidate");
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
			m_pGridCore->Reserve(m_manager->GetItemProperties()->Count, m_manager->List->Count);
			m_manager->ListChanged += m_listChangedEventHandler;
			m_manager->BindingComplete += m_bindingCompleteEventHandler;

			this->CurrencyManagerChanged(this, e);
		}
		m_pGridCore->Invalidate();
	}

	void GridControl::OnEditClosed(System::EventArgs^ e)
	{
		this->EditClosed(this, e);
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

	void GridControl::style_Disposed(System::Object^ sender, System::EventArgs^ /*e*/)
	{
		if(sender == m_style)
		{
			this->Style = nullptr;
		}
	}

	bool GridControl::IsInputKey(System::Windows::Forms::Keys keyData) 
	{
		using namespace System::Windows::Forms;

		//Keys key = keyData & Keys::KeyCode;

		//switch (key)
		//{
		//case Keys::Tab:
		//case Keys::Up:
		//case Keys::Down:
		//case Keys::Right:
		//case Keys::Left:
		//    return true;
		//default:
		//    break;
		//}

		return UserControl::IsInputKey(keyData);
	}

	bool GridControl::ProcessDialogKey(System::Windows::Forms::Keys keyData)
	{
		Keys keys = keyData & Keys::KeyCode;
		if(m_pGridWindow->OnKeyDown((GrKeys)keys) == true)
			return true;

		if ((keyData & (Keys::Alt | Keys::Control)) == Keys::None)
		{
			switch(keys)
			{
			case Keys::Up:
			case Keys::Down:
			case Keys::Left:
				{
					if(dynamic_cast<GridControl^>(this->Parent) != nullptr)
						break;
				}
				return true;
			case Keys::Right:
				{
					GrGridRow* pGridRow = dynamic_cast<GrGridRow*>(this->Focuser->GetFocusedRow());
					if(pGridRow != nullptr)
					{
						pGridRow->SetFocus();
					}

					if(dynamic_cast<GridControl^>(this->Parent) != nullptr)
						break;
				}
				return true;
			case Keys::Enter:
				{
					if(this->ReadOnly == false && this->InsertionRow->IsFocused == true)
					{
						Ntreev::Windows::Forms::Grid::Row^ row = this->Rows->AddFromInsertion();
						if(row != nullptr)
						{
							//Ntreev::Windows::Forms::Grid::Cell^ cell = row->Cells[m_focusedCell->Column];
							//cell->Select(SelectionType::Normal);
							//cell->Focus();
							//cell->BringIntoView();
						}
						return true;
					}
				}
				break;
			case Keys::Tab:
				{
					UserControl::ProcessDialogKey(keyData);
					return true;
				}
				break;
			}
		}

		return UserControl::ProcessDialogKey(keyData);
	}

	bool GridControl::ProcessDialogChar(wchar_t charCode)
	{
		if(m_focusedCell != nullptr && ((UserControl::ModifierKeys & (Keys::Control | Keys::Alt)) == Keys::None))
		{
			Column^ column = m_focusedCell->Column;
			EditingReason reason(charCode);
			if(column->CanEditInternal(m_focusedCell, reason) == true)
			{
				this->EditCell(m_focusedCell, reason);
				return true;
			}
		}

		return UserControl::ProcessDialogChar(charCode);
	}

	// 키에 의해서 편집이 시작되는 경우는 여기다가.
	bool GridControl::ProcessCmdKey(System::Windows::Forms::Message% msg, System::Windows::Forms::Keys keyData)
	{
		using namespace System::Windows::Forms;

		switch(keyData)
		{
		case Keys::Enter:
			{
				if(m_focusedCell != nullptr && m_focusedCell->Row != this->InsertionRow)
				{
					this->EditCell(m_focusedCell, EditingReason(keyData));
					return true;
				}
			}
			break;
		case (Keys::Down | Keys::Alt):
			{
				if(m_focusedCell != nullptr && m_focusedCell->Column->GetEditStyle() == Design::EditStyle::DropDown)
				{
					this->EditCell(m_focusedCell, EditingReason(keyData));
					return true;
				}
			}
			break;
		case Keys::F2:
			{
				if(m_focusedCell != nullptr)
				{
					this->EditCell(m_focusedCell, EditingReason(keyData));
					return true;
				}
			}
			break;
		case Keys::F12:
			{
				if(this->InsertionRow->IsVisible == true)
				{
					this->InsertionRow->Select(SelectionType::Normal);
					this->InsertionRow->Focus();
					this->InsertionRow->BringIntoView();
					return true;
				}
			}
			break;
		case Keys::Escape:
			{
				if(m_focusedCell == nullptr)
					break;

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
				else
				{
					return false;
				}

				this->Invalidate(false);
				return true;
			}
			break;
		default:
			{
				Column^ column = this->FocusedColumn;

				if(column != nullptr)
				{
					EditingReason er(keyData);
					if(column->CanEditInternal(m_focusedCell, er) == true)
					{
						this->EditCell(m_focusedCell, er);
						return true;
					}
				}
			}
			break;
		}

		return UserControl::ProcessCmdKey(msg, keyData);
	}

	void GridControl::OnPreviewKeyDown(System::Windows::Forms::PreviewKeyDownEventArgs^ e)
	{
		using namespace System::Windows::Forms;

		UserControl::OnPreviewKeyDown(e);

		switch(e->KeyCode)
		{
		case Keys::ProcessKey:
			{
				if(e->Control == true || e->Alt == true)
					break;

				Column^ column = this->FocusedColumn;

				if(column == nullptr)
				{
					GridControl^ parent = dynamic_cast<GridControl^>(this->Parent);
					if(parent != nullptr && parent->FocusedCell != nullptr)
					{
						parent->OnPreviewKeyDown(e);
					}
				}
				else
				{
					System::Char imeChar = Native::Methods::ImmGetVirtualKey(Handle);
					EditingReason er(e->KeyValue);
					if(System::Char::IsLetter(imeChar) && column->CanEditInternal(m_focusedCell, er))
					{
						this->EditCell(m_focusedCell, er);
					}
				}
			}
			break;
		}
	}

	void GridControl::OnKeyDown(System::Windows::Forms::KeyEventArgs^ e)
	{
		using namespace Native;
		using namespace System::Windows::Forms;

		UserControl::OnKeyDown(e);
		return;

		////if(m_pGridWindow->OnKeyDown((GrKeys)e->KeyCode) == true)
		////{
		////    e->Handled = true;
		////    e->SuppressKeyPress = true;
		////    return;
		////}

		//switch(e->KeyCode)
		//{
		//    //case Keys::Right:
		//    //    {
		//    //        GrGridRow* pGridRow = dynamic_cast<GrGridRow*>(this->Focuser->GetFocusedRow());
		//    //        if(pGridRow != nullptr)
		//    //        {
		//    //            pGridRow->FocusGridControl();
		//    //            e->Handled = true;
		//    //        }
		//    //    }
		//    //    break;
		//    //case Keys::Left:
		//    //    {
		//    //        int eqwr=0;
		//    //    }
		//    //    break;
		//    //case Keys::Enter:
		//    //    {
		//    //        if(this->InsertionRow->IsFocused == true)
		//    //        {
		//    //            Ntreev::Windows::Forms::Grid::Row^ row = this->Rows->AddFromInsertion();
		//    //            if(row != nullptr)
		//    //            {
		//    //                //Ntreev::Windows::Forms::Grid::Cell^ cell = row->Cells[m_focusedCell->Column];
		//    //                //cell->Select(SelectionType::Normal);
		//    //                //cell->Focus();
		//    //                //cell->BringIntoView();
		//    //            }
		//    //            e->SuppressKeyPress = true;
		//    //        }
		//    //    }
		//    //    break;
		//case Keys::Tab:
		//case Keys::Escape:
		//    {
		//        e->SuppressKeyPress = true;
		//    }
		//default:
		//    {
		//        Column^ column = this->FocusedColumn;

		//        if(column != nullptr)
		//        {
		//            EditingReason er(e->KeyCode);
		//            if(column->CanEditInternal(m_focusedCell, er) == true)
		//            {
		//                this->EditCell(m_focusedCell, er);
		//                e->SuppressKeyPress = true;
		//            }
		//        }
		//    }
		//    break;
		//}
		//if(e->Control == true)
		//    e->SuppressKeyPress = true;
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
		if(m_disposing == true)
			return;

		GrItem* pFocusedItem = this->Focuser->GetItem();

		if(pFocusedItem == nullptr)
		{
			m_focusedCell = nullptr;
		}
		else
		{
			m_focusedCell = FromNative::Get(pFocusedItem);
		}

		if(this->FocusedColumn != m_oldFocusedColumn)
		{
			OnFocusedColumnChanged(System::EventArgs::Empty);
		}

		if(this->FocusedRow != m_oldFocusedRow)
		{
			OnFocusedRowChanged(System::EventArgs::Empty);
		}

		OnFocusedCellChanged(gcnew CellEventArgs(m_focusedCell));
	}

	GridControl^ GridControl::InvokeNewChildGridControl(GrGridRow* pGridRow)
	{
		using namespace Native;

		System::ComponentModel::PropertyDescriptor^ descriptor = pGridRow->GetPropertyDescriptor();
		Row^ row = pGridRow->GetParentRow();

		GridControl^ control = this->NewChildGridControl(descriptor, row);
		control->Visible = false;
		control->Name = descriptor->Name;
		this->Controls->Add(control);
		control->FocusedCellChanged += gcnew CellEventHandler(this, &GridControl::childGridControl_FocusedCellChanged);
		control->m_pGridRow = pGridRow;

		return control;
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
		//this->Columns->Clear();
	}

	void GridControl::ResetRows()
	{
		this->Rows->Clear();
	}

	void GridControl::ResetBackgroundColor()
	{
		this->BackgroundColor = System::Drawing::SystemColors::Control;
	}

	void GridControl::ResetPaddingColor()
	{
		this->PaddingColor = System::Drawing::SystemColors::ActiveCaption;
	}

	void GridControl::ResetLineColor()
	{
		this->LineColor = GrStyle::Default.LineColor;
	}

	void GridControl::ResetBackColor()
	{
		this->BackColor = GrStyle::Default.BackColor;
	}

	void GridControl::ResetForeColor()
	{
		this->ForeColor = GrStyle::Default.ForeColor;
	}

	bool GridControl::ShouldSerializeCaption()
	{
		return m_captionRow->Text != _Resources::GetString("CaptionRow.Text");
	}

	bool GridControl::ShouldSerializeBackgroundColor()
	{
		return m_backgroundColor != System::Drawing::SystemColors::Control;
	}

	bool GridControl::ShouldSerializePaddingColor()
	{
		return m_paddingColor != System::Drawing::SystemColors::ActiveCaption;
	}

	bool GridControl::ShouldSerializeLineColor()
	{
		return this->LineColor != (System::Drawing::Color)GrStyle::Default.LineColor;
	}

	bool GridControl::ShouldSerializeBackColor()
	{
		return this->BackColor != (System::Drawing::Color)GrStyle::Default.BackColor;
	}

	bool GridControl::ShouldSerializeForeColor()
	{
		return this->ForeColor != (System::Drawing::Color)GrStyle::Default.ForeColor;
	}

	void GridControl::ClearInsertion()
	{
		this->InsertionRow->CancelEdit();
		for each(Ntreev::Windows::Forms::Grid::Cell^ cell in this->InsertionRow->Cells)
		{
			cell->ClearLocalValue();
			cell->UpdateNativeText();
		}
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

	void GridControl::InvokeColumnVisibleIndexChanged(Column^ column)
	{
		//ColumnEventArgs e(column);
		//OnColumnFrozenChanged(%e);

		if(this->Site != nullptr)
		{
			using namespace System::ComponentModel;
			using namespace System::ComponentModel::Design;
			IComponentChangeService^ service = dynamic_cast<IComponentChangeService^>(GetService(IComponentChangeService::typeid));
			PropertyDescriptor^ propertyDescriptor = TypeDescriptor::GetProperties(column)["VisibleIndex"];
			service->OnComponentChanging(column, propertyDescriptor);
			service->OnComponentChanged(column, propertyDescriptor, nullptr, column->VisibleIndex);
		}
	}

	void GridControl::EditCell(Ntreev::Windows::Forms::Grid::Cell^ cell, EditingReason editBy)
	{
		if(cell->IsReadOnly == true)
			return;

		if(cell->IsFocused == false)
			cell->Focus();

		try
		{
			m_isEditing = true;
			bool focused = this->Focused;
			m_pGridCore->EditItem(cell->NativeRef, editBy.ToNative());
			if(focused == true)
				this->Focus();
		}
		finally
		{
			m_isEditing = false;
		}
	}

	void GridControl::CloseEdit()
	{
		OnEditClosed(System::EventArgs::Empty);
	}

	void GridControl::ClearSelection()
	{
		this->ClearSelection(false);
	}

	void GridControl::ClearSelection(bool keepFocus)
	{
		this->Selector->ClearSelection();
		if(keepFocus == false)
			this->Focuser->Set(IFocusable::Null);
	}

	void GridControl::SelectAll()
	{
		if(m_pGridCore->GetMultiSelect() == false)
			throw gcnew System::InvalidOperationException();

		this->Selector->SelectAll();
	}

	void GridControl::Select(System::Collections::Generic::IEnumerable<Row^>^ rows)
	{
		if(m_pGridCore->GetMultiSelect() == false)
			throw gcnew System::InvalidOperationException();

		GrDataRows dataRows;
		for each(Row^ item in rows)
		{
			dataRows.insert(item->NativeRef);
		}

		this->Selector->SelectDataRows(&dataRows, GrSelectionType_Normal);
	}

	void GridControl::Select(System::Collections::Generic::IEnumerable<Cell^>^ cells)
	{
		if(m_pGridCore->GetMultiSelect() == false)
			throw gcnew System::InvalidOperationException();

		GrItems items;
		for each(Cell^ item in cells)
		{
			items.insert(item->NativeRef);
		}

		this->Selector->SelectItems(&items, GrSelectionType_Normal);
	}

	void GridControl::BringIntoView(Ntreev::Windows::Forms::Grid::Cell^ cell)
	{
		m_pGridCore->BringIntoView(cell->NativeRef);
	}

	void GridControl::BringIntoView(Ntreev::Windows::Forms::Grid::Row^ row)
	{
		m_pDataRowList->BringIntoView(row->NativeRef);
	}

	void GridControl::BringIntoView(Column^ column)
	{
		m_pColumnList->BringIntoView(column->NativeRef);
	}

	void GridControl::DisplayFirst(RowBase^ rowBase)
	{
		m_pDataRowList->DisplayFirst(rowBase->NativeRef);
	}

	void GridControl::DisplayFirst(Column^ column)
	{
		m_pColumnList->DisplayFirst(column->NativeRef);
	}

	void GridControl::Clear()
	{
		m_disposing = true;
		try
		{
			OnCurrencyManagerChanging(gcnew CurrencyManagerChangingEventArgs(m_defaultManager));
			ClearCore(false);

			m_dataSource = nullptr;
			m_dataMember = System::String::Empty;
			m_manager = nullptr;

			OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(m_defaultManager));
		}
		finally
		{
			m_disposing = false;
		}
	}

	void GridControl::ClearCore(bool dataSourceOnly)
	{
		m_pGridCore->Update(true);

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
		return m_editingCell;
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
		if(m_style != nullptr)
		{
			m_style->Disposed -= m_styleDisposedEventHandler; 
			m_style->GridControl = nullptr;
		}

		if(value != nullptr)
		{
			m_pGridCore->SetStyle(value->NativeStyle);
			value->Disposed += m_styleDisposedEventHandler;
			value->GridControl = this;
		}
		else
		{
			m_pGridCore->SetStyle(nullptr);
		}

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

	bool GridControl::FillBlank::get()
	{
		return m_pGridCore->GetFillBlank();
	}

	void GridControl::FillBlank::set(bool value)
	{
		m_pGridCore->SetFillBlank(value);
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
		GridControl^ parent = dynamic_cast<GridControl^>(this->Parent);
		if(parent != nullptr && parent->ReadOnly == true)
			return true;
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

	bool GridControl::IsRowMovable::get()
	{
		return m_pGridCore->GetRowMovable();
	}

	void GridControl::IsRowMovable::set(bool value)
	{
		m_pGridCore->SetRowMovable(value);
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

	void GridControl::InvokeReset()
	{
		OnReseted(System::EventArgs::Empty);
	}

	bool GridControl::InvokeValueChanging(Ntreev::Windows::Forms::Grid::Cell^ cell, System::Object^ value, System::Object^ oldValue)
	{
		ValueChangingEventArgs e(cell, value, oldValue);
		OnValueChanging(%e);
		return e.Cancel != true;
	}

	void GridControl::InvokeValueChanged(Ntreev::Windows::Forms::Grid::Cell^ cell)
	{
		//m_manager->EndCurrentEdit();
		OnValueChanged(gcnew CellEventArgs(cell));
	}

	bool GridControl::InvokeRowInserting(Ntreev::Windows::Forms::Grid::Row^ row)
	{
		Ntreev::Windows::Forms::Grid::RowInsertingEventArgs e(row);
		OnRowInserting(%e);
		return e.Cancel != true;
	}

	void GridControl::InvokeRowInserted(Ntreev::Windows::Forms::Grid::Row^ row)
	{
		OnRowInserted(gcnew RowInsertedEventArgs(row));
	}

	bool GridControl::InvokeRowBinding(System::Object^ component)
	{
		RowBindingEventArgs e(component);
		OnRowBinding(%e);
		return e.Cancel != true;
	}

	void GridControl::InvokeRowBinded(Ntreev::Windows::Forms::Grid::Row^ row)
	{
		RowEventArgs e(row);
		OnRowBinded(%e);
	}

	void GridControl::InvokeRowUnbinding(Ntreev::Windows::Forms::Grid::Row^ row)
	{
		RowEventArgs e(row);
		OnRowUnbinding(%e);
	}

	void GridControl::InvokeRowUnbinded(Ntreev::Windows::Forms::Grid::Row^ row)
	{
		RowEventArgs e(row);
		OnRowUnbinded(%e);
	}

	bool GridControl::InvokeRowMoving(Row^ row, int index)
	{
		RowMovingEventArgs e(row, index);
		OnRowMoving(%e);
		return e.Cancel != true;
	}

	void GridControl::InvokeRowMoved(Row^ row)
	{
		m_pGridCore->Update();
		RowEventArgs e(row);
		OnRowMoved(%e);
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

	void GridControl::InvokeRowChanged(Row^ row)
	{
		OnRowChanged(gcnew RowEventArgs(row));
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

		if(e.Cancel == true)
		{
			if(existColumn != nullptr)
				existColumn->IsVisible = false;
			return nullptr;
		}

		if(e.BindingColumn == nullptr && this->ColumnBindingCreation != Ntreev::Windows::Forms::Grid::ColumnBindingCreation::None)
		{
			e.BindingColumn = this->Columns->CreateColumnInstance();
		}

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

	bool GridControl::InvokeEditBegun(Ntreev::Windows::Forms::Grid::Cell^ cell)
	{
		EditBegunEventArgs e(cell);
		OnEditBegun(%e);

		if(e.Cancel == true)
			return false;

		Ntreev::Windows::Forms::Grid::Row^ row = cell->Row;


		if(row->IsBeingEdited == false)
			row->BeginEdit();

		m_editingCell = cell;
		return true;
	}

	void GridControl::InvokeEditEnded(CellEventArgs^ e)
	{
		m_editingCell = nullptr;
		OnEditEnded(e);
	}

	void GridControl::InvokeScroll(System::Windows::Forms::ScrollEventArgs^ e)
	{
		m_pGridCore->Invalidate();
		m_pGridCore->Update();
		OnScroll(e);
	}

	void GridControl::InvokeCellMouseEnter(Ntreev::Windows::Forms::Grid::Cell^ cell)
	{
		CellEventArgs e(cell);
		OnCellMouseEnter(%e);
	}

	bool GridControl::InvokeCellMouseMove(Ntreev::Windows::Forms::Grid::Cell^ cell, System::Drawing::Point clientLocation)
	{
		CellMouseEventArgs e(cell, clientLocation);
		OnCellMouseMove(%e);
		return e.Handled;
	}

	void GridControl::InvokeCellMouseLeave(Ntreev::Windows::Forms::Grid::Cell^ cell)
	{
		CellEventArgs e(cell);
		OnCellMouseLeave(%e);
	}

	void GridControl::InvokeCellClick(Ntreev::Windows::Forms::Grid::Cell^ cell)
	{
		OnCellClick(gcnew CellEventArgs(cell));
	}

	void GridControl::InvokeCellDoubleClick(Ntreev::Windows::Forms::Grid::Cell^ cell)
	{
		OnCellDoubleClick(gcnew CellEventArgs(cell));
	}

	void GridControl::InvokeDisplayRectangleChanged()
	{
		OnDisplayRectangleChanged(System::EventArgs::Empty);
	}

	void GridControl::InvokeVisibleHeightChanged()
	{
		OnVisibleHeightChanged(System::EventArgs::Empty);
	}

	void GridControl::OnReseted(System::EventArgs^ e)
	{
		Reseted(this, e);
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

	void GridControl::OnRowChanged(RowEventArgs^ e)
	{
		RowChanged(this, e);
	}

	void GridControl::OnRowBinding(Ntreev::Windows::Forms::Grid::RowBindingEventArgs^ e)
	{
		RowBinding(this, e);
	}

	void GridControl::OnRowBinded(Ntreev::Windows::Forms::Grid::RowEventArgs^ e)
	{
		RowBinded(this, e);
	}

	void GridControl::OnRowUnbinding(Ntreev::Windows::Forms::Grid::RowEventArgs^ e)
	{
		RowUnbinding(this, e);
	}

	void GridControl::OnRowUnbinded(Ntreev::Windows::Forms::Grid::RowEventArgs^ e)
	{
		RowUnbinded(this, e);
	}

	void GridControl::OnRowMoving(RowMovingEventArgs^ e)
	{
		RowMoving(this, e);
	}

	void GridControl::OnRowMoved(Ntreev::Windows::Forms::Grid::RowEventArgs^ e)
	{
		RowMoved(this, e);
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
		Row^ row = dynamic_cast<Row^>(m_oldFocusedRow);

		if(row != nullptr)
		{
			if(row->IsBeingEdited == true && row != this->InsertionRow)
			{
				this->EndCurrentEdit(row);
			}
		}

		if(m_manager != nullptr)
		{
			if(this->FocusedRow == this->InsertionRow)
			{
				this->Rows->BeginInsertion();
			}
			else if(m_oldFocusedRow == this->InsertionRow)
			{
				this->Rows->EndInsertion();
			}
		}

		FocusedRowChanged(this, e);
	}

	void GridControl::OnFocusedColumnChanged(System::EventArgs^ e)
	{
		FocusedColumnChanged(this, e);
	}

	void GridControl::OnFocusedCellChanged(CellEventArgs^ e)
	{
		FocusedCellChanged(this, e);
	}

	void GridControl::OnColumnInserting(ColumnInsertingEventArgs^ e)
	{
		ColumnInserting(this, e);
	}

	void GridControl::OnColumnInserted(ColumnEventArgs^ e)
	{
		Column^ column = e->Column;
		this->InsertionRow->NewCell(column);
		for each(Ntreev::Windows::Forms::Grid::Row^ item in m_rowList)
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
		this->InsertionRow->NewCell(column);
		for each(Ntreev::Windows::Forms::Grid::Row^ item in m_rowList)
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

	void GridControl::OnCellMouseEnter(Ntreev::Windows::Forms::Grid::CellEventArgs^ e)
	{
		try
		{
			Cell^ cell = e->Cell;
			System::String^ invalidValue = cell->InvalidValue;
			System::String^ error = cell->Error;
			System::String^ sourceError = cell->SourceError;
			if(invalidValue != System::String::Empty)
			{
				this->ToolTip->Show(invalidValue);
			}
			else if(error != System::String::Empty)
			{
				this->ToolTip->Show(error);
			}
			else if(sourceError != System::String::Empty)
			{
				this->ToolTip->Show(sourceError);
			}
		}
		catch(System::Exception^)
		{
		}

		CellMouseEnter(this, e);
	}

	void GridControl::OnCellMouseMove(Ntreev::Windows::Forms::Grid::CellMouseEventArgs^ e)
	{
		CellMouseMove(this, e);
	}

	void GridControl::OnCellMouseLeave(Ntreev::Windows::Forms::Grid::CellEventArgs^ e)
	{
		this->ToolTip->Hide();
		CellMouseLeave(this, e);
	}

	void GridControl::OnCellClick(CellEventArgs^ e)
	{
		CellClick(this, e); 
	}

	void GridControl::OnCellDoubleClick(CellEventArgs^ e)
	{
		CellDoubleClick(this, e); 
	}

	void GridControl::OnDisplayRectangleChanged(System::EventArgs^ e)
	{
		DisplayRectangleChanged(this, e);
	}

	void GridControl::OnVisibleHeightChanged(System::EventArgs^ e)
	{
		VisibleHeightChanged(this, e);
	}

	void GridControl::OnEditBegun(Ntreev::Windows::Forms::Grid::EditBegunEventArgs^ e)
	{
		EditBegun(this, e);
	}

	void GridControl::OnEditEnded(CellEventArgs^ e)
	{
		EditEnded(this, e);
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
			Column^ column = m_visibleColumnList->Count > 0 ? m_visibleColumnList[0] : nullptr;
			Ntreev::Windows::Forms::Grid::Row^ row = m_visibleRowList->Count > 0 ? (Ntreev::Windows::Forms::Grid::Row^)m_visibleRowList[0] : nullptr;
			if(column != nullptr && row != nullptr)
			{
				GrItem* pItem = row->Cells[column]->NativeRef;
				this->Selector->SetSelectionGroup(pItem);
				this->Selector->SelectItem(pItem, GrSelectionType_Normal);
				this->Selector->SetAnchor(pItem);
				this->Focuser->Set(pItem);
			}
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

	System::Windows::Forms::BindingContext^ GridControl::BindingContext::get()
	{
		System::Windows::Forms::BindingContext^ bindingContext = UserControl::BindingContext;
		if(bindingContext == nullptr)
		{
			bindingContext = gcnew System::Windows::Forms::BindingContext();
			this->BindingContext = bindingContext;
		}

		return bindingContext;
	}

	void GridControl::BindingContext::set(System::Windows::Forms::BindingContext^ value)
	{
		UserControl::BindingContext = value;
	}


	System::Drawing::Rectangle GridControl::DisplayRectangle::get()
	{
		System::Drawing::Rectangle clientRectangle = UserControl::ClientRectangle;
		clientRectangle.X += this->Padding.Left;
		clientRectangle.Y += this->Padding.Top;
		clientRectangle.Width -= this->Padding.Horizontal;
		clientRectangle.Height -= this->Padding.Vertical;
		return clientRectangle;
	}

	bool GridControl::HScrollVisible::get()
	{
		return m_pGridCore->GetHorzScroll()->GetVisible();
	}

	bool GridControl::VScrollVisible::get()
	{
		return m_pGridCore->GetVertScroll()->GetVisible();
	}

	bool GridControl::CanEnableIme::get()
	{
		return true; 
	}

	System::Drawing::Color GridControl::BackgroundColor::get()
	{
		return m_backgroundColor;
	}

	void GridControl::BackgroundColor::set(System::Drawing::Color value)
	{
		if(m_backgroundColor == value)
			return;
		m_backgroundColor = value;
		OnBackgroundColorChanged(System::EventArgs::Empty);
	}

	System::Drawing::Color GridControl::PaddingColor::get()
	{
		return m_paddingColor;
	}

	void GridControl::PaddingColor::set(System::Drawing::Color value)
	{
		if(m_paddingColor == value)
			return;
		m_paddingColor = value;
		OnPaddingColorChanged(System::EventArgs::Empty);
	}

	System::Drawing::Color GridControl::LineColor::get()
	{
		return m_lineColor;
	}

	void GridControl::LineColor::set(System::Drawing::Color value)
	{
		if(m_lineColor == value)
			return;
		m_lineColor = value;
		OnLineColorChanged(System::EventArgs::Empty);
	}

	System::Drawing::Color GridControl::BackColor::get()
	{
		if(m_paintBackground == true)
			return m_backgroundColor;
		return UserControl::BackColor;
	}

	void GridControl::BackColor::set(System::Drawing::Color value)
	{
		UserControl::BackColor = value;
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

	System::Windows::Forms::Control^ GridControl::GetChildAt(System::Windows::Forms::Control^ control, System::Drawing::Point location)
	{
		System::Drawing::Point pt = control->PointToClient(location);
		System::Windows::Forms::Control^ found = control->GetChildAtPoint(pt);
		if (found != nullptr && found->HasChildren == true)
			return this->GetChildAt(found, location);
		if(found == nullptr && control->Bounds.Contains(pt) == true)
			return control;
		return found;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/