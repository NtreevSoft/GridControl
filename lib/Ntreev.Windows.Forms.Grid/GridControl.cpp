﻿#include "stdafx.h"
#include "GridControl.h"
#include "GridColumn.h"

#include "GridCell.h"
#include "GridRow.h"
#include "GridCaption.h"
#include "GridEvent.h"
#include "GridResource.h"
#include "GridCollection.h"
#include "GridRowCollection.h"
#include "GridColumnCollection.h"
#include "GridDebug.h"
#include "GridWin32.h"
#include "GridTimeTest.h"
#include "GridColumnExtension.h"

// Private include
#include "About.h"
#include "GridTooltip.h"
#include "GridCellIterator.h"
#include "GridErrorDescriptor.h"
#include "GridState.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	class NativeInvaliator : public GrGridInvalidator
	{
	public:
		NativeInvaliator(gcroot<_GridControl^> pGrid) : m_pGrid(pGrid)
		{ 
			
		}

		void Invalidate()
		{
			m_pGrid->Invalidate(false);
		}

		void Invalidate(int x, int y, int width, int height)
		{
			m_pGrid->Invalidate(_Rectangle(x, y, width, height), false);
		}

		gcroot<_GridControl^> m_pGrid;
	};

	class NativeEvent : GrObject
	{
	public:
		NativeEvent(gcroot<_GridControl^> pGrid) : m_pGrid(pGrid)
		{ 
			
		}

		~NativeEvent()
		{

		}

		void gridCore_FocusChanging(GrObject*, GrEventArgs*)
		{
			m_pGrid->gridCore_FocusChanging();
		}

		void gridCore_FocusChanged(GrObject*, GrEventArgs*)
		{
			m_pGrid->gridCore_FocusChanged();
		}

		void gridCore_SelectedRowsChanged(GrObject*, GrEventArgs*)
		{
			m_pGrid->gridCore_SelectedRowsChanged();
		}

		void gridCore_SelectedColumnsChanged(GrObject*, GrEventArgs*)
		{
			m_pGrid->gridCore_SelectedColumnsChanged();
		}

		void gridCore_SelectionChanged(GrObject*, GrEventArgs*)
		{
			m_pGrid->gridCore_SelectionChanged();
		}

		void dataColumnList_ColumnWidthChanged(GrObject*, GrColumnEventArgs* e)
		{
			m_pGrid->dataColumnList_ColumnWidthChanged(e->GetColumn());
		}

		void dataRowList_RowVisibleChanged(GrObject*, GrEventArgs*)
		{
			m_pGrid->dataRowList_RowVisibleChanged();
		}

		gcroot<_GridControl^> m_pGrid;
	};

	GridControl::GridControl()
	{
#ifdef _TIME_TEST
		Private::GridTimeTest timeTest("GridControl 생성자");
#endif
		m_pGridCore			= new GrGridCore();
		m_pGridCore->SetInvalidator(new NativeInvaliator(this));
		m_pGridRenderer		= GrGridRendererCreator((void*)Handle.ToPointer());
		m_pColumnList		= m_pGridCore->GetColumnList();
		m_pDataRowList		= m_pGridCore->GetDataRowList();
		m_pInsertionRow		= m_pGridCore->GetInsertionRow();

		m_pGridCore->SetFont(this->Font);

#ifdef _TIME_TEST
		Rendering			= true;
#endif
		m_states = gcnew GridState::StateManager(this);

		m_bCtrlKey				= false;
		m_bShiftKey				= false;
		m_bAltKey				= false;

		m_enableColumnMoving	= true;
		m_enableColumnResizing	= true;
		m_enableColumnFrozing	= true;
		m_enableSorting			= true;
		m_readOnly				= false;
		m_enableRowResizing		= true;
		m_fullRowSelect			= false;
		m_hideSelection			= false;
		m_multiSelect			= true;

		m_dataSource			= nullptr;
		m_dataMember			= "";

		m_tooltips				= gcnew Private::GridTooltip(this, 3);
		m_errorDescriptor		= gcnew Private::ErrorDescriptor(this);

		m_columnList			= gcnew ColumnCollection(this);
		m_visibleColumnList		= gcnew VisibleColumnCollection(this);
		m_displayableColumnList	= gcnew DisplayableColumnCollection(this);
		m_frozenColumnList		= gcnew FrozenColumnCollection(this);
		m_unfrozenColumnList	= gcnew UnfrozenColumnCollection(this);
		m_rowList				= gcnew RowCollection(this);
		m_visibleRowList		= gcnew VisibleRowCollection(this);
		m_displayableRowList	= gcnew DisplayableRowCollection(this);
		m_style					= gcnew Design::Style();

		m_selectedColumns		= gcnew SelectedColumnCollection(this, Selector->GetSelectedColumns());
		m_selectedRows			= gcnew SelectedRowCollection(this, Selector->GetSelectedRows());
		
		m_cellIterator			= gcnew _CellIterator(this);
		m_captionRow			= gcnew _CaptionRow(this, m_pGridCore->GetCaptionRow());
		m_insertionRow			= gcnew _InsertionRow(this, m_pGridCore->GetInsertionRow());

		m_hScrollProperty		= gcnew HScrollProperty(this, UserControl::HorizontalScroll);
		m_vScrollProperty		= gcnew VScrollProperty(this, UserControl::VerticalScroll);

		m_pEvent				= new NativeEvent(this);

		m_defaultDataSource		= gcnew System::Data::DataTable();
		m_defaultManager		= dynamic_cast<_CurrencyManager^>(this->BindingContext[m_defaultDataSource]);

		DoubleBuffered			= m_pGridRenderer->DoubleBuffered();
		//BorderStyle			= System::Windows::Forms::BorderStyle::FixedSingle;
		//AllowDrop				= true;

		InitializeComponent();
		SetNativeEvent(true);

		this->CurrencyManagerChanged(this, gcnew CurrencyManagerChangedEventArgs(m_defaultManager));
	}

	GridControl::~GridControl()
	{
		this->Columns->DeleteAll();

		delete m_rowList;
		delete m_pEvent;
		m_pEvent = nullptr;

		if(m_pGridCore != nullptr)
		{
			GrGridInvalidator* pInvalidator = m_pGridCore->GetInvalidator();
			if(pInvalidator != nullptr)
				delete pInvalidator;

			delete m_pGridCore;
			m_pGridCore = nullptr;
		}
		if(m_pGridRenderer != nullptr)
		{
			delete m_pGridRenderer;
			m_pGridRenderer = nullptr;
		}
	}

	bool GridControl::ProcessTabKey(bool /*forward*/)
	{
		return true;
	}

	void GridControl::OnGotFocus(_EventArgs^ e)
	{
		Debug::OutputFunction function(__FUNCTION__);

		m_pGridCore->SetSelectionVisible(true);	
		Invalidate(false);

		UserControl::OnGotFocus(e);
	}

	void GridControl::OnLostFocus(_EventArgs^ e)
	{
		Debug::OutputFunction function(__FUNCTION__);

		if(m_hideSelection == true)
		{
			m_pGridCore->SetSelectionVisible(false);
			Invalidate(false);
		}

		ControlKey	= false;
		ShiftKey	= false;
		m_bAltKey   = false;

		UserControl::OnLostFocus(e);
	}

	void GridControl::OnLayout(System::Windows::Forms::LayoutEventArgs^ e)
	{
		UpdateGridRect();
		UserControl::OnLayout(e);
	}

	void GridControl::OnPrint(System::Windows::Forms::PaintEventArgs^ e)
	{
		OnPaint(e);
	}

	void GridControl::OnPaint(System::Windows::Forms::PaintEventArgs^ e)
	{
		_Graphics^ graphics = e->Graphics;
		Debug::OutputFunction function(__FUNCTION__);
		
		try
		{
			UpdateGridRect();

			GrRect clipping(e->ClipRectangle);

			clipping.right  = System::Math::Min(clipping.right, this->DisplayRectangle.Right);
			clipping.bottom = System::Math::Min(clipping.bottom, this->DisplayRectangle.Bottom);		

			System::IntPtr hdc = e->Graphics->GetHdc();
			try
			{
				m_pGridRenderer->OnBeginRender(hdc.ToPointer());
				m_pGridCore->Render(m_pGridRenderer, &clipping);
				m_pGridRenderer->OnEndRender();
			}
			catch(System::Exception^ exception)
			{
				throw exception;
			}
			finally
			{
				e->Graphics->ReleaseHdc(hdc);
			}

			PaintColumnControls(graphics, e->ClipRectangle);

			if(IsRowNumberVisible == false)
				PaintRowState(graphics);
					
			m_errorDescriptor->Paint(graphics);

			UserControl::OnPaint(e);
		}
		catch(System::Exception^ exception)
		{
			using namespace System::Drawing;
			e->Graphics->Clear(Color::White);

			PointF location((PointF)this->DisplayRectangle.Location);
			SizeF size = e->Graphics->MeasureString(exception->Message, this->Font);
			e->Graphics->DrawString(exception->Message, this->Font, Brushes::Black, location);

			location.Y += size.Height;
			e->Graphics->DrawString(exception->StackTrace, this->Font, Brushes::Black, location);
		}
	}

	void GridControl::PaintColumnControls(_Graphics^ graphics, _Rectangle clipRectangle)
	{
		Private::RenderingStyle^ style = gcnew Private::RenderingStyle();
		for(uint i=0 ; i<m_pColumnList->GetDisplayableColumnCount() ; i++)
		{
			GrColumn* pColumn = m_pColumnList->GetDisplayableColumn(i);

			int x = pColumn->GetDisplayX();
			int r = x + pColumn->GetWidth();

			if(x >= clipRectangle.Right || r < clipRectangle.Left)
				continue;

			IValuePainter^ valuePainter = dynamic_cast<IValuePainter^>(m_columnList[pColumn]);
			if(valuePainter == nullptr)
				continue;
			if(valuePainter->PaintValueSupported == false)
				continue;
			for(uint j=0 ; j<m_pDataRowList->GetDisplayableRowCount() ; j++)
			{
				GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(m_pDataRowList->GetDisplayableRow(j));
				if(pDataRow == nullptr)
					continue;

				int y = pDataRow->GetDisplayY();
				int b = y + pDataRow->GetHeight();
				if(y >= clipRectangle.Bottom || b < clipRectangle.Top)
					continue;

				_Cell^ cell = m_rowList[pDataRow][pColumn];
				style->Cell = cell;
				_Rectangle clipRect = cell->ClientRectangle;

				if(pColumn->GetItemType() != GrItemType_Control && cell->NativeRef->GetSelected() == true)
					clipRect.Width -= 15;
				
				valuePainter->PaintValue(graphics, cell->ClientRectangle, clipRect, style, cell->Value);
			}

			if(m_insertionRow->IsVisible == true)
			{
				_Cell^ cell = m_insertionRow[pColumn];
				style->Cell = cell;
				_Rectangle clipRect = cell->ClientRectangle;

				if(pColumn->GetItemType() != GrItemType_Control && cell->NativeRef->GetSelected() == true)
					clipRect.Width -= 15;
				valuePainter->PaintValue(graphics, cell->ClientRectangle, clipRect, style, cell->Value);
			}
		}
	}

	void GridControl::PaintRowState(_Graphics^ g)
	{
		IDataRow* pFocusedRow = Focuser->GetFocusedRow();
		if(EditingCell != nullptr)
		{
			GrRect bound = EditingCell->NativeRef->GetDataRow()->GetDisplayRect();
			GrPoint center = bound.GetCenter();

			_Bitmap^ image = _Resources::RowEditing;
			center.x -= (image->Width / 2);
			center.y -= (image->Height / 2);
			g->DrawImageUnscaled(image, center.x, center.y);
		}
		else if(FocusedRow != nullptr && FocusedRow->IsEdited == true)
		{
			GrRect bound = FocusedRow->NativeRef->GetDisplayRect();
			GrPoint center = bound.GetCenter();

			_Bitmap^ image = _Resources::RowEditing;
			center.x -= (image->Width / 2);
			center.y -= (image->Height / 2);
			g->DrawImageUnscaled(image, center.x, center.y);
		}
		else if(pFocusedRow && pFocusedRow->GetDisplayable() == true)
		{
			GrRect bound = pFocusedRow->GetDisplayRect();
			GrPoint center = bound.GetCenter();

			_Bitmap^ image = _Resources::RowFocused;
			if(pFocusedRow == m_pInsertionRow)
				image = _Resources::InsertionRowFocused;
			center.x -= (image->Width / 2);
			center.y -= (image->Height / 2);
			g->DrawImageUnscaled(image, center.x, center.y);
		}

		if(m_pInsertionRow->GetVisible() == true && m_pInsertionRow->HasFocused() == false)
		{
			GrRect bound = m_pInsertionRow->GetDisplayRect();
			GrPoint center = bound.GetCenter();

			_Bitmap^ image = _Resources::InsertionRow;
			center.x -= (image->Width / 2);
			center.y -= (image->Height / 2);
			g->DrawImageUnscaled(image, center.x, center.y);
		}
	}

	void GridControl::SetDataConnection(object^ dataSource, string^ dataMember)
	{
		//if(m_dataSource != dataSource || m_dataMember != dataMember)
		//{
		//	//Clear();
		//}

		//m_dataSource = dataSource;
		//m_dataMember = dataMember;

		if(dataSource == nullptr)
		{
			if(dataMember == string::Empty)
				Clear();
			return;
		}

		_CurrencyManager^ manager;
		try
		{
			manager = dynamic_cast<_CurrencyManager^>(this->BindingContext[dataSource, dataMember]);
		}
		catch(System::Exception^)
		{
			return;
		}

		if(manager != nullptr)
		{
			if(m_dataMember != dataMember)
				Clear();

			m_dataSource = dataSource;
			m_dataMember = dataMember;

			m_manager = manager;

			SetNativeEvent(false);

			OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(manager));

			m_insertionRow->SetDefaultValue();

			UpdateGridRect();

			SetNativeEvent(true);

			
			

			//InitializeDataSource();
			//m_manager->ListChanged += gcnew System::ComponentModel::ListChangedEventHandler(this, &GridControl::manager_ListChanged);

			OnDataBindingComplete(_EventArgs::Empty);
			Invalidate(false);
		}
	}

	void GridControl::UpdateGridRect()
	{
		m_pGridCore->SetDisplayRect(DisplayRectangle);

		if(m_pGridCore->Update() == true)
		{
			m_hScrollProperty->UpdateVisible();
			m_vScrollProperty->UpdateVisible();
			m_hScrollProperty->Update();
			m_vScrollProperty->Update();
		}

		uint nStartVisibleCol = m_hScrollProperty->Visible == true ? m_hScrollProperty->Value : 0;
		uint nStartVisibleRow = m_vScrollProperty->Visible == true ? m_vScrollProperty->Value : 0;
		m_pGridCore->Clip(nStartVisibleCol, nStartVisibleRow);

		UpdateDataRectangle();
	}


	_Cell^ GridControl::GetCellAt(_Point pt)
	{
		HitTest hitTest;
		if(DoHitTest(pt, hitTest) == false)
			return nullptr;
		GrItem* pItem = dynamic_cast<GrItem*>(hitTest.pHittedCell);
		if(pItem == nullptr)
			return nullptr;

		object^ e = pItem->ManagedRef;
		return safe_cast<_Cell^>(e);
	}

	void GridControl::OnVisibleChanged(_EventArgs^ e)
	{
		m_states->ChangeDefaultState();
		UserControl::OnVisibleChanged(e);
	}

	void GridControl::OnSizeChanged(_EventArgs^ e)
	{
		m_states->ChangeDefaultState();
		UserControl::OnSizeChanged(e);
	}

	void GridControl::OnClientSizeChanged(_EventArgs^ e)
	{
		UpdateDataRectangle();
		UserControl::OnClientSizeChanged(e);
	}

	void GridControl::UpdateDataRectangle()
	{
		int left = m_pColumnList->GetUnfrozenColumnCount() > 0 ? m_pColumnList->GetUnfrozenColumn(0)->GetX() : ClientRectangle.Left;
		int top  = m_pDataRowList->GetY();
		m_dataRectangle = _Rectangle::FromLTRB(left, top, ClientRectangle.Right, ClientRectangle.Bottom);
	}

	void GridControl::WndProc(System::Windows::Forms::Message% m)
	{
		switch(m.Msg)
		{
		case Win32::API::WM_HSCROLL:
			m_hScrollProperty->WndProc(m.WParam.ToInt32());
			return;
		case Win32::API::WM_VSCROLL:
			m_vScrollProperty->WndProc(m.WParam.ToInt32());
			return;
		}
		UserControl::WndProc(m);	
	}

	bool GridControl::DoHitTest(_Point pt, HitTest% hitTest)
	{
		GrHitTest ht;
		if(HitTester->DisplayTest(GrPoint(pt.X, pt.Y), &ht) == true)
		{
			hitTest.pHittedCell	= ht.pHitted;
			hitTest.localPoint	= ht.ptLocal;
			hitTest.cellRect	= ht.rtRect;
		}
		else
		{
			hitTest.pHittedCell = nullptr;
		}
	
		return hitTest.pHittedCell != nullptr;
	}

	object^ GridControl::GetInternalService(_Type^ serviceType)
	{
		return UserControl::GetService(serviceType);
	}

	//void GridControl::OnMouseMove(System::Windows::Forms::MouseEventArgs^ e)
	//{
	//	UserControl::OnMouseMove(e);
	//	return;
	//	_HitTest hitOld = m_hitTest;
	//	DoHitTest(e->Location, m_hitTest);

	//	switch(e->Button)
	//	{
	//	case System::Windows::Forms::MouseButtons::Left:
	//		OnMouseMoveLButton(e);
	//		break;
	//	case System::Windows::Forms::MouseButtons::Middle:
	//		OnMouseMoveMButton(e);
	//		break;
	//	case System::Windows::Forms::MouseButtons::None:
	//		HitState = m_hitTest.HitState;
	//		OnMouseMoveNone(e, m_hitTest, hitOld);
	//		break;
	//	}
	//}

	//void GridControl::OnMouseUp(System::Windows::Forms::MouseEventArgs^ e)
	//{
	//	Debug::OutputFunction function(__FUNCTION__);
	//	UserControl::OnMouseUp(e);
	//	return;
	//	HitState = m_hitTest.HitState;

	//	switch(e->Button)
	//	{
	//	case System::Windows::Forms::MouseButtons::Left:
	//		OnMouseLButtonUp(e);
	//		break;
	//	case System::Windows::Forms::MouseButtons::Middle:
	//		OnMouseMButtonUp(e);
	//		break;
	//	case System::Windows::Forms::MouseButtons::Right:
	//		OnMouseRButtonUp(e);
	//		break;
	//	}
	//	
	//}

	//void GridControl::OnMouseDoubleClick(System::Windows::Forms::MouseEventArgs^ e)
	//{
	//	Debug::OutputFunction function(__FUNCTION__);
	//	//if(m_bCreated == false)
	//		
	//	UserControl::OnMouseDoubleClick(e);
	//	return;
	//	if(DoHitTest(e->Location, m_hitTest) == false)
	//		return;

	//	HitState = m_hitTest.HitState;
	//	switch(e->Button)
	//	{
	//	case System::Windows::Forms::MouseButtons::Left:
	//		OnMouseLButtonDoubleClick(e);
	//		break;
	//	case System::Windows::Forms::MouseButtons::Right:
	//		OnMouseRButtonDoubleClick(e);
	//		break;
	//	}
	//}

	//void GridControl::OnMouseDown(System::Windows::Forms::MouseEventArgs^ e)
	//{
	//	Debug::OutputFunction function(__FUNCTION__);
	//	UserControl::OnMouseDown(e);
	//	return;
	//	if(DoHitTest(e->Location, m_hitTest) == false)
	//	{
	//		return;
	//	}

	//	HitState = m_hitTest.HitState;
	//	switch(e->Button)
	//	{
	//	case System::Windows::Forms::MouseButtons::Left:
	//		OnMouseLButtonDown(e);
	//		break;
	//	case System::Windows::Forms::MouseButtons::Middle:
	//		OnMouseMButtonDown(e);
	//		break;
	//	case System::Windows::Forms::MouseButtons::Right:
	//		OnMouseRButtonDown(e);
	//		break;
	//	}
	//	//UserControl::OnMouseDown(e);
	//}

	void GridControl::OnMouseLeave(_EventArgs^ e)
	{
		m_pGridCore->SetMouseOver(NULL, GrPoint::Empty);
		UserControl::OnMouseLeave(e);
	}

	_CaptionRow^ GridControl::CaptionRow::get()
	{
		return m_captionRow;
	}

	object^ GridControl::DataSource::get()
	{
		return m_dataSource;
	}

	void GridControl::DataSource::set(object^ value)
	{
		if(m_dataSource != value)
		{
			if(value == nullptr)
				m_dataMember = string::Empty;

			SetDataConnection(value, m_dataMember);
			if(m_dataSource == value)
				OnDataSourceChanged(_EventArgs::Empty);
		}
	}

	string^ GridControl::DataMember::get()
	{
		if(m_manager == nullptr)
			return string::Empty;
		return m_dataMember;
	}

	void GridControl::DataMember::set(string^ value)
	{
		if(m_dataMember != value)
		{
			SetDataConnection(DataSource, value);
			if(m_dataMember == value)
				OnDataMemberChanged(_EventArgs::Empty);
		}
	}

	//IColumnSet^ GridControl::ColumnSet::get()
	//{
	//	if(m_columnSet == nullptr)
	//		return m_defaultColumnSet;
	//	return m_columnSet;
	//}

	//void GridControl::ColumnSet::set(IColumnSet^ value)
	//{
	//	m_columnSet = value;
	//}

	//IRowSet^ GridControl::RowSet::get()
	//{
	//	return m_rowSet;
	//}

	//void GridControl::RowSet::set(IRowSet^ value)
	//{
	//	m_rowSet = value;
	//}

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

	void GridControl::OnLeave(_EventArgs^ e)
	{
		Debug::OutputFunction function(__FUNCTION__);

		m_states->ChangeDefaultState();

		ControlKey	= false;
		ShiftKey	= false;
		m_bAltKey   = false;

		UserControl::OnLeave(e);
	}

	void GridControl::OnPaddingChanged(_EventArgs^ e)
	{
		UserControl::OnPaddingChanged(e);
		Invalidate(false);
	}

	void GridControl::OnFontChanged(_EventArgs^ e)
	{
		m_pGridCore->SetFont(this->Font);
		Invalidate(false);
		UserControl::OnFontChanged(e);
		Debug::WriteLine("Invalidate");
	}

	void GridControl::OnCurrencyManagerChanged(CurrencyManagerChangedEventArgs^ e)
	{
		if(e->CurrecnyManager != nullptr)
		{
			m_pGridCore->Reserve(m_manager->GetItemProperties()->Count, m_manager->List->Count);
		}

		this->CurrencyManagerChanged(this, e);
	}

	void GridControl::OnKeyPress(System::Windows::Forms::KeyPressEventArgs^ e)
	{
		if(dynamic_cast<IEditByChar^>(FocusedColumn))
		{
			EditCell(m_focusedCell, gcnew EditingReason(e->KeyChar, false));
		}
		UserControl::OnKeyPress(e);
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

	bool GridControl::DefaultEditKeyTest(System::Windows::Forms::Keys key)
	{
		if(key == _Keys::F2 || key == _Keys::Enter)
			return true;
		return false;
	}

	bool GridControl::ProcessCmdKey(System::Windows::Forms::Message% msg, _Keys keyData)
	{
		_Keys keyCode = (_Keys)((int)keyData & (int)_Keys::KeyCode);

		if(keyCode == _Keys::Tab)
		{
			if(m_bShiftKey == true)
				m_cellIterator->MoveLeft(_SelectionRange::One);
			else
				m_cellIterator->MoveRight(_SelectionRange::One);
			Update();
			return true;
		}

		if(m_states->State == GridState::State::ItemEditing)
			return System::Windows::Forms::UserControl::ProcessCmdKey(msg, keyData);

		switch(keyCode)
		{
		case _Keys::Up:
			{
				m_cellIterator->MoveUp(this->SelectionRange);
			}
			break;
		case _Keys::Down:
			{
				m_cellIterator->MoveDown(this->SelectionRange);
			}
			break;
		case _Keys::Left:
			{
				m_cellIterator->MoveLeft(this->SelectionRange);
			}
			break;
		case _Keys::Right:
			{
				m_cellIterator->MoveRight(this->SelectionRange);
			}
			break;
		case _Keys::End:
			{
				if(m_bCtrlKey == true)
					m_cellIterator->LastRow(this->SelectionRange);
				else
					m_cellIterator->LastCell(this->SelectionRange);
			}
			break;
		case _Keys::Home:
			{
				if(m_bCtrlKey == true)
					m_cellIterator->FirstRow(this->SelectionRange);
				else
					m_cellIterator->FirstCell(this->SelectionRange);
			}
			break;
		case _Keys::PageUp:
			{
				m_cellIterator->PageUp(this->SelectionRange);
			}
			break;
		case _Keys::PageDown:
			{
				m_cellIterator->PageDown(this->SelectionRange);
			}
			break;
		case _Keys::Enter:
			{
				if(m_focusedCell == nullptr)
					return UserControl::ProcessCmdKey(msg, keyData);

				if(m_focusedCell->Row == m_insertionRow)
				{
					Row^ row = AddNewRowFromInsertion();
					if(row == nullptr)
						break;

					_Cell^ cell = row[m_focusedCell->Column];
					cell->Select(_SelectionType::Normal);
					cell->Focus();
					cell->EnsureVisible();
				}
				else if(m_focusedCell != nullptr)
				{
					EditCell(m_focusedCell, gcnew EditingReason(keyCode));
				}
			}
			break;
		case _Keys::F12:
			{
				if(m_insertionRow->IsVisible == true)
				{
					m_insertionRow->Select(_SelectionType::Normal);
					m_insertionRow->Focus();
					m_insertionRow->EnsureVisible();
				}
			}
			break;
		case _Keys::F2:
			{
				if(m_focusedCell != nullptr)
				{
					EditCell(m_focusedCell, gcnew EditingReason(keyCode));
				}
				else
				{
					return UserControl::ProcessCmdKey(msg, keyData);
				}
			}
			break;

		case _Keys::Escape:
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

		default:
			{
				Column^ column = FocusedColumn;
				IEditByKeyCode^ keyEdit = dynamic_cast<IEditByKeyCode^>(column);
				if(keyEdit != nullptr && keyEdit->KeyTest(keyCode) == true)
				{
					EditCell(m_focusedCell, gcnew EditingReason(keyCode));
					break;
				}
			}
			return UserControl::ProcessCmdKey(msg, keyData);
		}
		Update();
		return true;
	}

	void GridControl::OnPreviewKeyDown(System::Windows::Forms::PreviewKeyDownEventArgs^ e)
	{
		ControlKey	= e->Control;
		ShiftKey	= e->Shift;
		m_bAltKey   = e->Alt;

		switch(e->KeyCode)
		{
		case _Keys::ProcessKey:
			{
				if(dynamic_cast<IEditByIme^>(FocusedColumn))
				{
					_Char nnn = Win32::API::ImmGetVirtualKey(Handle);
					if(_Char::IsLetter(nnn))
					{
						EditCell(m_focusedCell, gcnew EditingReason((System::Char)e->KeyValue, true));
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
		case _Keys::Enter:
			{
				e->SuppressKeyPress = true;
			}
		case _Keys::Escape:
			{
				e->SuppressKeyPress = true;
			}
			break;
		}
		if(e->Control == true)
			e->SuppressKeyPress = true;
		UserControl::OnKeyDown(e);
	}

	void GridControl::OnKeyUp(System::Windows::Forms::KeyEventArgs^ e)
	{
		Debug::OutputFunction function(__FUNCTION__);
		
		ControlKey	= e->Control;
		ShiftKey	= e->Shift;
		m_bAltKey   = e->Alt;
		UserControl::OnKeyUp(e);
	}

	void GridControl::gridCore_SelectedRowsChanged()
	{
		OnSelectedRowsChanged(_EventArgs::Empty);
	}

	void GridControl::gridCore_SelectedColumnsChanged()
	{
		OnSelectedColumnsChanged(_EventArgs::Empty);
	}

	void GridControl::gridCore_SelectionChanged()
	{
		OnSelectionChanged(_EventArgs::Empty);
	}

	void GridControl::gridCore_FocusChanging()
	{
		if(m_states->State == GridState::State::ItemEditing)
			m_states->ChangeDefaultState();
	}

	void GridControl::gridCore_FocusChanged()
	{
		Debug::OutputFunction function(__FUNCTION__);
		GrItem* pFocusedItem = Focuser->GetItem();

		Column^ oldFocusedColumn = FocusedColumn;
		Row^	oldFocusedRow    = FocusedRow;
		

		if(pFocusedItem == nullptr)
		{
			m_focusedCell = nullptr;
		}
		else
		{
			m_focusedCell = Cell::FromNative(pFocusedItem);
		}

#ifndef _TIME_TEST
		if(pFocusedItem)
			Debug::WriteLine("Focused Item is existed");
		else
			Debug::WriteLine("Focused Item is null");
#endif
		if(FocusedColumn != oldFocusedColumn)
		{
			OnFocusedColumnChanged(_EventArgs::Empty);
		}

		if(FocusedRow != oldFocusedRow)
		{
			if(oldFocusedRow != nullptr)
				oldFocusedRow->ApplyEdit();
			OnFocusedRowChanged(_EventArgs::Empty);
		}

		OnFocusedCellChanged(gcnew CellEventArgs(m_focusedCell));
	}

	void GridControl::dataRowList_RowVisibleChanged()
	{
		
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

	void GridControl::dataColumnList_ColumnWidthChanged(GrColumn* pColumn)
	{
		Column^ column = Column::FromNative(pColumn);
		ColumnEventArgs e(column);
		OnColumnWidthChanged(%e);
	}

	void GridControl::EditCell(Cell^ cell, EditingReason^ editBy)
	{
		if(cell->IsFocused == false)
			cell->IsFocused = true;

		m_states->ChangeState(GridState::State::ItemEditing, cell->NativeRef, editBy);
	}

	void GridControl::EndEditCell()
	{
		m_states->ChangeDefaultState();
	}

	void GridControl::ClearSelection()
	{
		Selector->ClearSelection();
		Focuser->Set(IFocusable::Null);
	}

	void GridControl::SelectAll()
	{
		if(m_multiSelect == false)
			throw gcnew System::InvalidOperationException();

		Selector->SelectAll();
	}

	void GridControl::EnsureVisible(Cell^ cell)
	{
		GrItem* pItem = cell->NativeRef;
		if(pItem->NeedToEnsure() == false)
			return;

		if(m_pGridCore->ShouldUpdate())
			UpdateGridRect();

		HorizontalScroll->EnsureVisible(cell->Column);
		VerticalScroll->EnsureVisible(cell->Row);
	}

	void GridControl::EnsureVisible(_Row^ row)
	{
		VerticalScroll->EnsureVisible(row);
	}

	void GridControl::EnsureVisible(_Column^ column)
	{
		HorizontalScroll->EnsureVisible(column);
	}

	void GridControl::SetNativeEvent(bool attached)
	{
		GrItemSelector* pItemSelector = m_pGridCore->GetItemSelector();
		GrFocuser* pFocuser = m_pGridCore->GetFocuser();
		if(attached == true)
		{
			pFocuser->FocusChanging.Add(m_pEvent,				&NativeEvent::gridCore_FocusChanging);
			pFocuser->FocusChanged.Add(m_pEvent,				&NativeEvent::gridCore_FocusChanged);
			pItemSelector->SelectedRowsChanged.Add(m_pEvent,	&NativeEvent::gridCore_SelectedRowsChanged);
			pItemSelector->SelectedColumnsChanged.Add(m_pEvent,	&NativeEvent::gridCore_SelectedColumnsChanged);
			pItemSelector->SelectionChanged.Add(m_pEvent,		&NativeEvent::gridCore_SelectionChanged);
			m_pColumnList->ColumnWidthChanged.Add(m_pEvent,		&NativeEvent::dataColumnList_ColumnWidthChanged);
			m_pDataRowList->VisibleChanged.Add(m_pEvent,		&NativeEvent::dataRowList_RowVisibleChanged);
		}
		else
		{
			m_pDataRowList->VisibleChanged.Remove(m_pEvent,		&NativeEvent::dataRowList_RowVisibleChanged);
			m_pColumnList->ColumnWidthChanged.Remove(m_pEvent,	&NativeEvent::dataColumnList_ColumnWidthChanged);
			pItemSelector->SelectionChanged.Remove(m_pEvent,	&NativeEvent::gridCore_SelectionChanged);
			pItemSelector->SelectedColumnsChanged.Remove(m_pEvent, &NativeEvent::gridCore_SelectedColumnsChanged);
			pItemSelector->SelectedRowsChanged.Remove(m_pEvent,	&NativeEvent::gridCore_SelectedRowsChanged);
			pFocuser->FocusChanged.Remove(m_pEvent,				&NativeEvent::gridCore_FocusChanged);
			pFocuser->FocusChanging.Remove(m_pEvent,			&NativeEvent::gridCore_FocusChanging);
		}
	}

	void GridControl::Clear()
	{
		OnClearing(_EventArgs::Empty);
		//SetNativeEvent(false);
		m_pGridCore->Clear();
		m_focusedCell	= nullptr;
		
		m_defaultDataSource->Clear();

		m_dataSource	= nullptr;
		m_dataMember	= "";

		OnCurrencyManagerChanged(gcnew CurrencyManagerChangedEventArgs(nullptr));
		m_manager = nullptr;

		OnCleared(_EventArgs::Empty);
		Invalidate(false);
	}

	_SelectionType GridControl::SelectionType::get()
	{
		return m_selectionType;
	}

	void GridControl::SelectionType::set(_SelectionType value)
	{
		if(m_selectionType == value)
			return;
		m_selectionType = value;
		Debug::WriteLine(m_selectionType);
	}

	_SelectionRange GridControl::SelectionRange::get()
	{
		return m_selectionRange;
	}

	void GridControl::SelectionRange::set(_SelectionRange value)
	{
		if(m_selectionRange == value)
			return;
		m_selectionRange = value;
		Debug::WriteLine(m_selectionRange);
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

	_InsertionRow^ GridControl::InsertionRow::get()
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
		if(m_states->State != GridState::State::ItemEditing)
			return nullptr;
		return m_focusedCell;
	}

	CellIterator^ GridControl::CellIterator::get()
	{
		return m_cellIterator;
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
		Invalidate(false);
	}

	bool GridControl::EnableColumnMoving::get()
	{
		return m_enableColumnMoving;
	}

	void GridControl::EnableColumnMoving::set(bool value)
	{
		m_enableColumnMoving = value;
	}

	bool GridControl::EnableColumnResizing::get()
	{
		return m_enableColumnResizing;
	}

	void GridControl::EnableColumnResizing::set(bool value)
	{
		m_enableColumnResizing = value;
	}

	bool GridControl::EnableColumnFrozing::get()
	{
		return m_enableColumnFrozing;
	}

	void GridControl::EnableColumnFrozing::set(bool value)
	{
		m_enableColumnFrozing = value;
	}

	bool GridControl::EnableSorting::get()
	{
		return m_enableSorting;
	}

	void GridControl::EnableSorting::set(bool value)
	{
		m_enableSorting = value;
	}

	bool GridControl::ReadOnly::get()
	{
		return m_readOnly;
	}

	void GridControl::ReadOnly::set(bool value)
	{
		m_readOnly = value;
	}

	bool GridControl::EnableRowResizing::get()
	{
		return m_enableRowResizing;
	}

	void GridControl::EnableRowResizing::set(bool value)
	{
		m_enableRowResizing = value;
	}

	bool GridControl::EnableGrouping::get()
	{
		return m_pGridCore->GetEnableGrouping();
	}

	void GridControl::EnableGrouping::set(bool value)
	{
		if(value == m_pGridCore->GetEnableGrouping())
			return;

		m_pGridCore->SetEnableGrouping(value);
		Invalidate(false);
	}

	bool GridControl::FullRowSelect::get()
	{
		return m_fullRowSelect;
	}

	void GridControl::FullRowSelect::set(bool value)
	{
		m_fullRowSelect = value;
		Invalidate(false);
	}

	bool GridControl::HideSelection::get()
	{
		return m_hideSelection;
	}

	void GridControl::HideSelection::set(bool value)
	{
		m_hideSelection = value;
		Invalidate(false);
	}

	bool GridControl::MultiSelect::get()
	{
		return m_multiSelect;
	}

	void GridControl::MultiSelect::set(bool value)
	{
		m_multiSelect = value;
	}

	bool GridControl::RowHighlight::get()
	{
		return m_pGridCore->GetRowHighlight();
	}
	
	void GridControl::RowHighlight::set(bool value)
	{
		m_pGridCore->SetRowHighlight(value);
		Invalidate(false);
	}

	_RowHighlightType GridControl::RowHighlightType::get()
	{
		return (_RowHighlightType)m_pGridCore->GetRowHighlightType();;
	}
	
	void GridControl::RowHighlightType::set(_RowHighlightType value)
	{
		m_pGridCore->SetRowHighlightType((GrRowHighlightType)value);
		Invalidate(false);
	}

	bool GridControl::IsGrouped::get()
	{
		return m_pGridCore->IsGrouped();
	}

	void GridControl::ControlKey::set(bool value)
	{
		if(value == true && m_multiSelect == true)
			SelectionType = _SelectionType::Add;
		else
			SelectionType = _SelectionType::Normal;
		m_bCtrlKey = value;
	}

	void GridControl::ShiftKey::set(bool value)
	{
		if(value == true && m_multiSelect == true)
			SelectionRange = _SelectionRange::Multi;
		else
			SelectionRange = _SelectionRange::One;
		m_bShiftKey = value;
	}

	bool GridControl::InvokeValueChanging(Cell^ cell, object^ value, object^ oldValue)
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

	void GridControl::InvokeInsertionRowInserted(_Row^ row)
	{
		RowEventArgs e(row);
		OnInsertionRowInserted(%e);
	}

	bool GridControl::InvokeRowInserting(object^ component)
	{
		RowInsertingEventArgs e(component);
		OnRowInserting(%e);
		return e.Cancel != true;
	}

	void GridControl::InvokeRowInserted(Row^ row)
	{
		OnRowInserted(gcnew RowEventArgs(row));
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

	bool GridControl::InvokeColumnInserting(_Column^ column)
	{
		ColumnInsertingEventArgs e(column);
		OnColumnInserting(%e);
		return e.Cancel == false;
	}

	void GridControl::InvokeColumnInserted(_Column^ column)
	{
		ColumnEventArgs e(column);
		OnColumnInserted(%e);
	}

	void GridControl::InvokeColumnBinding(ColumnBindingEventArgs^ e)
	{
		OnColumnBinding(e);
	}

	void GridControl::InvokeColumnBinded(ColumnEventArgs^ e)
	{
		OnColumnBinded(e);
	}

	bool GridControl::InvokeColumnMouseDown(Column^ column, _Point clientLocation)
	{
		ColumnMouseEventArgs ce(column, clientLocation);
		OnColumnMouseDown(%ce);
		return ce.Handled == true;
	}

	bool GridControl::InvokeColumnMouseUp(Column^ column, _Point clientLocation)
	{
		ColumnMouseEventArgs ce(column, clientLocation);
		OnColumnMouseUp(%ce);
		return ce.Handled == true;
	}

	void GridControl::InvokeColumnMouseEnter(Column^ column, _Point clientLocation)
	{
		ColumnMouseEventArgs ce(column, clientLocation);
		OnColumnMouseEnter(%ce);
	}

	void GridControl::InvokeColumnMouseMove(Column^ column, _Point clientLocation)
	{
		ColumnMouseEventArgs ce(column, clientLocation);
		OnColumnMouseMove(%ce);
	}
	
	void GridControl::InvokeColumnMouseLeave(Column^ column)
	{
		ColumnMouseEventArgs ce(column, _Point::Empty);
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
	}

	void GridControl::InvokeCellDoubleClicked(Cell^ cell)
	{
		OnCellDoubleClicked(gcnew CellEventArgs(cell));
	}

	void GridControl::OnValueChanging(ValueChangingEventArgs^ e)
	{
		ValueChanging(this, e);
	}

	void GridControl::OnValueChanged(CellEventArgs^ e)
	{
		ValueChanged(this, e);

		Cell^ cell = e->Cell;
		if(cell->IsDisplayed == true)
			Invalidate(cell->DisplayRectangle);
	}

	void GridControl::OnRowInserting(RowInsertingEventArgs^ e)
	{
		RowInserting(this, e);
	}

	void GridControl::OnRowInserted(RowEventArgs^ e)
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
		UpdateGridRect();
		InsertionRowInserted(this, e);
		Invalidate(false);
	}

	void GridControl::OnSelectedRowsChanged(_EventArgs^ e)
	{
		SelectedRowsChanged(this, e);
	}

	void GridControl::OnSelectedColumnsChanged(_EventArgs^ e)
	{
		SelectedColumnsChanged(this, e);
	}

	void GridControl::OnSelectionChanged(_EventArgs^ e)
	{
		SelectionChanged(this, e);
	}

	void GridControl::OnFocusedRowChanged(_EventArgs^ e)
	{
		FocusedRowChanged(this, e);
	}

	void GridControl::OnFocusedColumnChanged(_EventArgs^ e)
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

	void GridControl::OnColumnMouseEnter(ColumnMouseEventArgs^ e)
	{
		_Point location = e->Column->DisplayRectangle.Location;
		location.Y -= 50;
		
		m_tooltips->Show(e->Column->Tooltip);
		ColumnMouseEnter(this, e);
	}

	void GridControl::OnColumnMouseLeave(ColumnMouseEventArgs^ e)
	{
		m_tooltips->Hide();
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

	void GridControl::OnCellDoubleClicked(CellEventArgs^ e)
	{
		CellDoubleClicked(this, e);		
	}

	void GridControl::OnBeginEdit(BeginEditEventArgs^ e)
	{
		BeginEdit(this, e);
	}

	void GridControl::OnEndEdit(CellEventArgs^ e)
	{
		EndEdit(this, e);
	}

	void GridControl::OnClearing(_EventArgs^ e)
	{
		Clearing(this, e);
	}

	void GridControl::OnCleared(_EventArgs^ e)
	{
		Cleared(this, e);
	}

	void GridControl::OnDataSourceChanged(_EventArgs^ e)
	{
		DataSourceChanged(this, e);
	}

	void GridControl::OnDataMemberChanged(_EventArgs^ e)
	{
		DataMemberChanged(this, e);
	}

	void GridControl::OnDataBindingComplete(_EventArgs^ e)
	{
		try
		{
			_Column^ column = m_visibleColumnList->Count > 0 ? m_visibleColumnList[0] : nullptr;
			_Row^ row = m_visibleRowList->Count > 0 ? (_Row^)m_visibleRowList[0] : nullptr;
			if(column == nullptr || row == nullptr)
				return;
			GrItem* pItem = row[column]->NativeRef;
			Selector->SelectItem(pItem, GrSelectionType_Normal);
			Selector->SetAnchor(pItem);
			Focuser->Set(pItem);
		}
		catch(System::Exception^ e)
		{
			Debug::WriteLine(e->Message);
		}

		DataBindingComplete(this, e);
	}

	void GridControl::OnStateBegin(_EventArgs^ e)
	{
		StateBegin(this, e);
	}
	
	void GridControl::OnStateEnd(_EventArgs^ e)
	{
		StateEnd(this, e);
	}

	_Rectangle GridControl::DataRectangle::get()
	{
		return m_dataRectangle;
	}

	bool GridControl::IsRowNumberVisible::get()
	{
		return m_pDataRowList->GetRowNumberVisible();
	}
	
	void GridControl::IsRowNumberVisible::set(bool value)
	{
		m_pDataRowList->SetRowNumberVisible(value);
	}

	bool GridControl::IsMarginVisible::get()
	{
		return m_pGridCore->GetMarginVisible();
	}
	
	void GridControl::IsMarginVisible::set(bool value)
	{
		m_pGridCore->SetMarginVisible(value);
		Invalidate(false);
	}

	GrItemSelector* GridControl::Selector::get()
	{
		return m_pGridCore->GetItemSelector();
	}

	GrFocuser* GridControl::Focuser::get()
	{
		return m_pGridCore->GetFocuser();
	}

	GrHitTester* GridControl::HitTester::get()
	{
		return m_pGridCore->GetHitTester();
	}

	GrSelectionType GridControl::NativeSelectionType::get()
	{
		return (GrSelectionType)m_selectionType;
	}

	HScrollProperty^ GridControl::HorizontalScroll::get()
	{
		return m_hScrollProperty;
	}
	
	VScrollProperty^ GridControl::VerticalScroll::get()
	{
		return m_vScrollProperty;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/