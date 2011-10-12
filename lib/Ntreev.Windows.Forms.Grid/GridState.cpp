//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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
#include "GridState.h"

#include "GridColumn.h"
#include "GridUtil.h"
#include "GridControl.h"
#include "GridCell.h"
#include "GridRow.h"
#include "GridCollection.h"
#include "GridColumnCollection.h"
#include "GridTooltip.h"
#include "GridResource.h"
#include "GridSelectionTimer.h"
#include "GridCellIterator.h"
#include "GridWin32.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace GridState
{
	StateManager::StateManager(_GridControl^ gridControl)
		: GridObject(gridControl)
	{
		m_states = gcnew array<GridState::StateBase^>
		{
			gcnew Normal(gridControl),
				gcnew RootPressing(gridControl),
				gcnew ColumnPressing(gridControl),
				gcnew ColumnResizing(gridControl),
				gcnew ColumnSplitterMoving(gridControl),
				gcnew RowPressing(gridControl),
				gcnew RowResizing(gridControl),
				gcnew GroupingInfoPressing(gridControl),
				gcnew GroupingCellPressing(gridControl),
				gcnew GroupingExpandPressing(gridControl), 
				gcnew ItemPressing(gridControl),
				gcnew ItemButtonPressing(gridControl),
				gcnew ItemEditing(gridControl),
		};

		m_defaultState = m_states[0];
		ChangeDefaultState();

		gridControl->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &StateManager::gridControl_MouseDown);
		gridControl->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &StateManager::gridControl_MouseMove);
		gridControl->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &StateManager::gridControl_MouseUp);
		gridControl->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &StateManager::gridControl_MouseDoubleClick);
		gridControl->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &StateManager::gridControl_Paint);
		gridControl->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &StateManager::gridControl_PreviewKeyDown);
		gridControl->Clearing += gcnew ClearEventHandler(this, &StateManager::gridControl_Clearing);
	}

	void StateManager::ChangeDefaultState()
	{
		if(m_state != m_defaultState)
			ChangeState(m_defaultState, nullptr, nullptr);
	}

	void StateManager::gridControl_MouseDown(object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e)
	{
		if(e->Button != System::Windows::Forms::MouseButtons::Left)
			return;

		m_downState = nullptr;
		HitTest hitTest;
		if(GridControl->DoHitTest(e->Location, hitTest) == true)
		{
			StateBase^ hittedState = GetHitTest(hitTest.pHittedCell, hitTest.localPoint);
			if(hittedState == nullptr)
			{
				ChangeDefaultState();
				return;
			}
			ChangeState(hittedState, hitTest.pHittedCell, nullptr);
			m_downState = m_state;

			if(m_state == hittedState)
			{
				StateChangeService changeService(hittedState->State, hitTest.pHittedCell);
				hittedState->OnMouseDown(e->Location, hitTest, %changeService);

				if(changeService.NextState != hittedState->State)
					ChangeState(changeService.NextState, changeService.Reference, nullptr);

				m_downLocation = e->Location;
				m_dragRectangle = ComputeDragRectangle(e->Location);
			}
		}
	}

	_Rectangle StateManager::ComputeDragRectangle(_Point location)
	{
		_Size	dragSize = System::Windows::Forms::SystemInformation::DragSize;
		return _Rectangle(_Point(location.X - (dragSize.Width / 2),location.Y - (dragSize.Height / 2)),dragSize);
	}

	void StateManager::gridControl_MouseMove(object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e)
	{
		HitTest hitTest;
		GridControl->DoHitTest(e->Location, hitTest);

		switch(e->Button)
		{
		case System::Windows::Forms::MouseButtons::Left:
			{
				if(m_state == nullptr || m_state->Dragable == false)
					return;

				if(m_dragging == false && m_dragRectangle.Contains(e->Location) == false)
				{
					m_dragging = true;
					m_state->OnMouseDragBegin(e->Location);
				}

				if(m_dragging == true)
				{
					m_state->OnMouseDragMove(e->Location, hitTest);
					System::Windows::Forms::Cursor^ cursor = m_state->Cursor;

					if(GridControl->Cursor != cursor)
						GridControl->Cursor = cursor;
				}
			}
			break;
		case System::Windows::Forms::MouseButtons::None:
			{
				System::Windows::Forms::Cursor^ cursor = m_defaultState->Cursor;

				if(hitTest.pHittedCell != nullptr)
				{
					StateBase^ hittedState = GetHitTest(hitTest.pHittedCell, hitTest.localPoint);
					if(hittedState != nullptr)
						cursor = hittedState->Cursor;
				}

				m_state->OnMouseMove(e->Location, hitTest);

				if(GridControl->Cursor != cursor)
					GridControl->Cursor = cursor;
			}
			break;
		}

	}

	void StateManager::gridControl_MouseUp(object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e)
	{
		if(e->Button != System::Windows::Forms::MouseButtons::Left)
			return;

		StateChangeService changeService(m_defaultState->State);

		if(m_state != nullptr)
		{
			HitTest hitTest;
			GridControl->DoHitTest(e->Location, hitTest);

			if(m_dragging == true)
			{
				m_state->OnMouseDragEnd(false, hitTest);
				m_dragging = false;
			}

			m_state->OnMouseUp(e->Location, hitTest, %changeService);
		}

		if(m_state != nullptr && changeService.NextState != m_state->State)
		{
			ChangeState(changeService.NextState, changeService.Reference, changeService.Data);
		}
	}

	void StateManager::gridControl_MouseDoubleClick(object^ /*sender*/, System::Windows::Forms::MouseEventArgs^ e)
	{
		if(e->Button != System::Windows::Forms::MouseButtons::Left)
			return;

		if(m_downState != nullptr)
		{
			HitTest hitTest;
			GridControl->DoHitTest(e->Location, hitTest);

			StateChangeService changeService(m_downState->State);
			m_downState->OnMouseDoubleClick(e->Location, hitTest, %changeService);

			if(changeService.NextState != m_downState->State)
			{
				ChangeState(changeService.NextState, changeService.Reference, changeService.Data);
			}
		}
	}

	void StateManager::gridControl_Paint(object^ /*sender*/, System::Windows::Forms::PaintEventArgs^ e)
	{
		if(m_state != nullptr)
			m_state->OnPaintAdornments(e->Graphics, GridControl->DisplayRectangle);
	}


	void StateManager::gridControl_PreviewKeyDown(object^ /*sender*/, System::Windows::Forms::PreviewKeyDownEventArgs^ e)
	{
		if(m_dragging == true)
		{
			if(e->KeyCode == System::Windows::Forms::Keys::Escape)
			{
				m_state->OnMouseDragEnd(true, HitTest::Empty);
				m_dragging = false;

				StateChangeService changeService(m_defaultState->State);
				m_state->OnMouseUp(_Point::Empty, HitTest::Empty, %changeService);

				if(changeService.NextState == m_defaultState->State)
					ChangeState(changeService.NextState, changeService.Reference, changeService.Data);
			}
		}
	}

	void StateManager::gridControl_Clearing(object^ /*sender*/, ClearEventArgs^ /*e*/)
	{
		ChangeDefaultState();
	}

	void StateManager::ChangeState(_State state, GrCell* pCell, object^ userData)
	{
		ChangeState(m_states[(int)state], pCell, userData);
	}

	void StateManager::ChangeState(StateBase^ state, GrCell* pHitted, object^ userData)
	{
		StateBase^ oldState = m_state;
		if(oldState != nullptr)
		{
			StateChangeService changeService(oldState->State);
			oldState->OnEnd(%changeService);
			if(changeService.NextState != oldState->State)
			{
				ChangeState(changeService.NextState, changeService.Reference, changeService.Data);
				return;
			}
		}

		StateChangeService changeService(state->State);
		state->OnBegin(pHitted, userData, %changeService);

		m_state = state;
		if(GridControl->Cursor != m_state->Cursor)
			GridControl->Cursor = m_state->Cursor;

		if(changeService.NextState != m_state->State)
			ChangeState(changeService.NextState, changeService.Reference, changeService.Data);
	}

	State StateManager::GetHitTest(_Point location)
	{
		HitTest hitTest;
		if(this->GridControl->DoHitTest(location, hitTest) == false)
			return _State::Unknown;
		StateBase^ base = GetHitTest(hitTest.pHittedCell, hitTest.localPoint);
		return base != nullptr ? base->State : _State::Unknown;
	}

	StateBase^ StateManager::GetHitTest(GrCell* pHitted, _Point localHitted)
	{
		for each(StateBase^ item in m_states)
		{
			if(item->GetHitTest(pHitted, localHitted) == true)
			{
				return item;
			}
		}
		return nullptr;
	}

	StateBase::StateBase(_GridControl^ gridControl)
		: GridObject(gridControl)
	{

	}

	bool StateBase::GetHitTest(GrCell* /*pHitted*/, _Point /*localLocation*/)
	{
		return false;
	}

	void StateBase::OnBegin(GrCell* /*pCell*/, object^ /*data*/, IStateChangeService^ /*service*/)
	{

	}

	void StateBase::OnPaintAdornments(_Graphics^ /*g*/, _Rectangle% /*displayRectangle*/)
	{

	}

	void StateBase::OnMouseDown(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{

	}

	void StateBase::OnMouseMove(_Point /*location*/, HitTest /*hitTest*/)
	{

	}

	void StateBase::OnMouseUp(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{

	}

	void StateBase::OnMouseDoubleClick(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{

	}

	void StateBase::OnEnd(IStateChangeService^ /*service*/)
	{

	}

	void StateBase::OnMouseDragBegin(_Point /*location*/)
	{

	}

	void StateBase::OnMouseDragEnd(bool /*cancel*/, HitTest /*hitTest*/)
	{

	}

	void StateBase::OnMouseDragMove(_Point /*location*/, HitTest /*hitTest*/)
	{

	}

	Normal::Normal(_GridControl^ gridControl)
		: StateBase(gridControl)
	{
	}

	void Normal::OnBegin(GrCell* /*pCell*/, object^ /*data*/, IStateChangeService^ /*service*/)
	{
		GridCore->SetMouseUnpressed();
	}

	void Normal::OnMouseMove(_Point /*location*/, HitTest hitTest)
	{
		if(GridCore->SetMouseOver(hitTest.pHittedCell, hitTest.localPoint) == true)
		{
			//if(hitTest.pHittedCell)
			//	InvalidateCell(hitTest.pHittedCell);
			//if(m_pCell != nullptr)
			//	InvalidateCell(m_pCell);
		}


		if(hitTest.pHittedCell != m_pCell)
		{
			if(m_pCell != nullptr)
			{
				switch(m_pCell->GetCellType())
				{
				case GrCellType_Column:
					{
						Column^ column = Column::FromNative((GrColumn*)m_pCell);
						GridControl->InvokeColumnMouseLeave(column);
					}
					break;
				case GrCellType_Item:
					{
						GridControl->ToolTip->Hide();
					}
					break;
				}
			}

			if(hitTest.pHittedCell != nullptr)
			{
				switch(hitTest.CellType)
				{
				case GrCellType_Column:
					{
						Column^ column = Column::FromNative((GrColumn*)hitTest.pHittedCell);
						ColumnMouseEventArgs e(column, hitTest.localPoint);
						GridControl->InvokeColumnMouseEnter(column, hitTest.localPoint);
						GridControl->InvokeColumnMouseMove(column, hitTest.localPoint);
					}
					break;
				case GrCellType_Item:
					{
						try
						{
							GrItem* pItem = dynamic_cast<GrItem*>(hitTest.pHittedCell);
							Cell^ cell = Cell::FromNative(pItem);
							if(cell->ErrorDescription != string::Empty)
								GridControl->ToolTip->Show(cell->ErrorDescription);
						}
						catch(System::Exception^)
						{
						}
					}
					break;
				}
			}
			m_pCell = hitTest.pHittedCell;
		}
	}

	RootPressing::RootPressing(_GridControl^ gridControl) : StateBase(gridControl)
	{

	}

	bool RootPressing::GetHitTest(GrCell* pHitted, _Point /*localLocation*/)
	{
		return pHitted->GetCellType() == GrCellType_Root;
	}

	void RootPressing::OnBegin(GrCell* pCell, object^ /*data*/, IStateChangeService^ /*service*/)
	{
		GridCore->SetMousePressed(pCell);
		Selector->SelectAll();
		Invalidate();
	}

	void RootPressing::OnEnd(IStateChangeService^ /*service*/)
	{
		GridCore->SetMouseUnpressed();
	}

	ColumnPressing::ColumnPressing(_GridControl^ gridControl) : StateBase(gridControl)
	{
		m_timer = gcnew SelectionTimer2(gridControl);
		m_timer->Elapsed += gcnew System::Timers::ElapsedEventHandler(this, &ColumnPressing::timer_Elapsed);

		m_brush = gcnew System::Drawing::SolidBrush(_Color::Black);

		m_pColumnList = GridCore->GetColumnList();

	}

	bool ColumnPressing::GetHitTest(GrCell* pHitted, _Point localLocation)
	{
		if(pHitted->GetCellType() != GrCellType_Column)
			return false;

		if(localLocation.X < GridControl->ColumnSplitter || localLocation.X>= pHitted->GetWidth() - GridControl->ColumnSplitter)
			return false;
		return true;
	}

	void ColumnPressing::OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ /*service*/)
	{
		m_pOldDataColumn = m_pColumn;
		m_pColumn = (GrColumn*)hitTest.pHittedCell;

		_Column^ column = _Column::FromNative(m_pColumn); 

		m_handled = GridControl->InvokeColumnMouseDown(column, hitTest.localPoint);

		if(m_handled == false)
		{
			if(GridControl->SelectionRange == _SelectionRange::Multi)
			{
				Selector->SelectColumns(m_pColumn, Selector->GetColumnAnchor(), GridControl->NativeSelectionType);
				Focuser->Set(m_pColumn);
			}
			else
			{
				switch(GridControl->SelectionType)
				{
				case _SelectionType::Normal:
					{
						Selector->SelectItems(m_pColumn, GrSelectionType_Normal);
						Selector->SetColumnAnchor(m_pColumn);
						Focuser->Set(m_pColumn);

						IDataRow* pFocusedRow = Focuser->GetFocusedRow();
						if(pFocusedRow != nullptr)
							Selector->SetRowAnchor(pFocusedRow);

						if(GridControl->Site != nullptr)
						{
							using namespace System::ComponentModel::Design;
							ISelectionService^ selectionService = (ISelectionService^)GridControl->GetInternalService(ISelectionService::typeid);
							cli::array<object^>^ components = gcnew cli::array<object^>(1) { column, };
							selectionService->SetSelectedComponents(components);
						}
					}
					break;
				default:
					{
						Selector->SelectItems(m_pColumn, GrSelectionType_Add);
						Focuser->Set(m_pColumn);
					}
					break;
				}

			}
			GridCore->SetMousePressed(m_pColumn);

			m_locationStart = location;
		}
		else
		{
			m_locationStart = _Point::Empty;
		}

		if(column->ColumnPainter != nullptr)
		{
			this->Invalidate(column->DisplayRectangle);
		}
	}

	void ColumnPressing::OnMouseDragBegin(_Point /*location*/)
	{
		m_timer->Start();
	}

	void ColumnPressing::OnMouseDragMove(_Point location, HitTest hitTest)
	{
		m_cursor = m_cursor = System::Windows::Forms::Cursors::No;

		if(hitTest.pHittedCell == nullptr)
			return;
		m_timer->MouseLocation = location;

		m_targetType = TargetType::Unknown;

		switch(hitTest.CellType)
		{
		case GrCellType_Column:
			{
				if(GridControl->EnableColumnMoving == false || m_pColumn->GetMovable() == false)
					break;
				if(m_timer->CanHScroll == true)
					break;

				GrColumn* pTarget = (GrColumn*)hitTest.pHittedCell;
				if(pTarget != m_pColumn)
				{
					if(pTarget->GetFrozen() == true)
					{
						uint targetIndex;
						if(hitTest.localPoint.X < pTarget->GetWidth() / 2)
							targetIndex = pTarget->GetFrozenIndex();
						else
							targetIndex = pTarget->GetFrozenIndex() + 1;

						if(m_pColumn->GetFrozen() == false)
						{
							m_targetType = TargetType::Frozen;
						}
						else if(targetIndex != m_pColumn->GetFrozenIndex() + 1 && targetIndex != m_pColumn->GetFrozenIndex()) 
						{
							m_targetType = TargetType::Frozen;
						}
						else
						{
							m_targetType = TargetType::Unknown;
						}

						if(targetIndex < m_pColumnList->GetFrozenColumnCount())
							m_targetCell = m_pColumnList->GetFrozenColumn(targetIndex);
						else
							m_targetCell = nullptr;
					}
					else
					{
						uint targetIndex;
						if(hitTest.localPoint.X < pTarget->GetWidth() / 2)
							targetIndex = pTarget->GetScrollableIndex();
						else
							targetIndex = pTarget->GetScrollableIndex() + 1;

						if(m_pColumn->GetFrozen() == true)
						{
							m_targetType = TargetType::Unfrozen;
						}
						else if(targetIndex != m_pColumn->GetScrollableIndex() + 1 && targetIndex != m_pColumn->GetScrollableIndex()) 
						{
							m_targetType = TargetType::Unfrozen;
						}
						else
						{
							m_targetType = TargetType::Unknown;
						}

						if(targetIndex < m_pColumnList->GetUnfrozenColumnCount())
							m_targetCell = m_pColumnList->GetUnfrozenColumn(targetIndex);
						else
							m_targetCell = nullptr;
					}
				}
			}
			break;
		case GrCellType_GroupingInfo:
			{
				if(m_pColumn->CanBeGrouped() == false)
					break;

				if(m_pColumn->GetGrouped() == false)
				{
					m_targetCell = (GrGroupingInfo*)hitTest.pHittedCell;
					m_targetType = TargetType::GroupingList;
				}
			}
			break;
		case GrCellType_Row:
			{
				if(m_pColumn->CanBeGrouped() == false)
					break;

				if(m_pColumn->GetGrouped() == false && dynamic_cast<GrGroupingList*>(hitTest.pHittedCell) != nullptr)
				{
					m_targetCell = nullptr;
					m_targetType = TargetType::GroupingList;
				}
			}
			break;
		default:
			break;
		}

		switch(m_targetType)
		{
		case TargetType::GroupingList:
			{
				m_cursor = Cursors::Add;
			}
			break;
		case TargetType::Frozen:
		case TargetType::Unfrozen:
			{
				m_cursor = Cursors::Movable;
			}
			break;
		}

		Invalidate();
	}

	void ColumnPressing::OnMouseDragEnd(bool cancel, HitTest /*hitTest*/)
	{
		if(cancel == false)
		{
			switch(m_targetType)
			{
			case TargetType::Frozen:
				{
					m_pColumnList->MoveToFrozen(m_pColumn, (GrColumn*)m_targetCell);
				}
				break;
			case TargetType::Unfrozen:
				{
					m_pColumnList->MoveToUnfrozen(m_pColumn, (GrColumn*)m_targetCell);
				}
				break;
			case TargetType::GroupingList:
				{
					GrGroupingList* pGroupingList = GridCore->GetGroupingList();
					m_pColumn->SetGrouped(true);
					if(m_targetCell != nullptr)
					{
						pGroupingList->ChangeGroupingInfo(m_pColumn->GetGroupingInfo(), (GrGroupingInfo*)m_targetCell);
					}
				}
				break;
			}
			Invalidate();
		}
		m_targetType = TargetType::Unknown;
		m_cursor = nullptr;
		m_timer->Stop();
	}

	void ColumnPressing::OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ /*service*/)
	{
		_Column^ column = _Column::FromNative(m_pColumn); 

		if(m_handled == true)
		{
			m_handled = GridControl->InvokeColumnMouseUp(column, hitTest.localPoint);
		}

		if(m_handled == false)
		{
			if(EnableSorting(location) == true)
			{
				int n = ((int)m_pColumn->GetSortType() + 1) % (int)GrSort::Count;
				GrSort::Type sortType = (GrSort::Type)n;

				m_pColumn->SetSortType(sortType);
			}
		}

		if(column->ColumnPainter != nullptr)
		{
			this->Invalidate(column->DisplayRectangle);
		}

		m_timer->Stop();
		GridCore->SetMouseUnpressed();
	}

	void ColumnPressing::OnPaintAdornments(_Graphics^ g, _Rectangle% /*displayRectangle*/)
	{
		const int padding = 2;
		switch(m_targetType)
		{
		case TargetType::Frozen:
			{
				int x;
				if(m_targetCell == nullptr)
				{
					x = m_pColumnList->GetColumnSplitter()->GetDisplayPosition().x;
				}
				else
				{
					x = ((GrColumn*)m_targetCell)->GetDisplayX();
				}
				g->FillRectangle(m_brush, _Rectangle::FromLTRB(x - padding, m_pColumnList->GetY(), x + padding, m_pColumnList->GetY() + m_pColumnList->GetHeight()));
			}
			break;
		case TargetType::Unfrozen:
			{
				int x;
				if(m_targetCell == nullptr)
				{
					x = m_pColumnList->GetDisplayableRight();
				}
				else
				{
					x = ((GrColumn*)m_targetCell)->GetDisplayX();
				}
				g->FillRectangle(m_brush, _Rectangle::FromLTRB(x - padding, m_pColumnList->GetY(), x + padding, m_pColumnList->GetY() + m_pColumnList->GetHeight()));
			}
			break;
		case TargetType::GroupingList:
			{
				if(m_targetCell != nullptr)
				{
					int x = m_targetCell->GetX();
					g->FillRectangle(m_brush, _Rectangle::FromLTRB(x - padding, m_targetCell->GetY(), x + padding, m_targetCell->GetY() + m_targetCell->GetHeight()));
				}
			}
			break;
		default:
			return;
		}
	}

	bool ColumnPressing::EnableSorting(_Point location)
	{
		if(GridControl->EnableSorting == false)
			return false;

		if(m_targetType == TargetType::Unknown && 
			m_pOldDataColumn == m_pColumn &&
			location == m_locationStart)
		{
			return true;
		}
		return false;
	}

	void ColumnPressing::timer_Elapsed(System::Object^ /*sender*/, System::Timers::ElapsedEventArgs^ /*e*/)
	{
		m_timer->DoScroll();
		Invalidate(); 
	}

	ColumnPressing::SelectionTimer2::SelectionTimer2(_GridControl^ gridControl) : Private::SelectionTimer(gridControl)
	{
		VScrollEnabled			= false;
		IntervalAcceleration	= false;
		ScrollAcceleration		= false;
	}

	_Rectangle ColumnPressing::SelectionTimer2::InsideRectangle::get()
	{
		GrColumnList* pColumnList = GridControl->GridCore->GetColumnList();
		const int gap = 20;
		_Rectangle rect = GridControl->DataRectangle;
		return _Rectangle::FromLTRB(rect.Left + gap, pColumnList->GetY(), rect.Right - gap, rect.Bottom);
	}

	_Rectangle ColumnPressing::SelectionTimer2::OutsideRectangle::get()
	{
		GrColumnList* pColumnList = GridControl->GridCore->GetColumnList();
		_Rectangle rect = GridControl->DataRectangle;
		return _Rectangle::FromLTRB(rect.Left, pColumnList->GetY(), rect.Right, rect.Bottom);
	}


	ColumnResizing::ColumnResizing(_GridControl^ gridControl)
		: StateBase(gridControl)
	{
		using namespace System::Drawing;
		using namespace System::Drawing::Drawing2D;

		m_pen = gcnew Pen(Color::Black);
		m_pen->DashStyle = DashStyle::Custom;
		m_pen->DashCap	 = DashCap::Round;
		cli::array<System::Single>^ dashPattern = {1.0f, 1.0f, };

		m_pen->DashPattern = dashPattern;
	}

	bool ColumnResizing::GetHitTest(GrCell* pHitted, _Point localLocation)
	{
		if(pHitted->GetCellType() != GrCellType_Column)
			return false;

		GrColumn* pColumn = (GrColumn*)pHitted;
		return GetResizingColumn(pColumn, localLocation) != nullptr;
	}

	GrColumn* ColumnResizing::GetResizingColumn(GrColumn* pColumn, _Point localPoint)
	{
		if(localPoint.X >= pColumn->GetWidth() - GridControl->ColumnSplitter)
		{
			if(pColumn->GetResizable() == true)
				return pColumn;
		}
		else if(localPoint.X < GridControl->ColumnSplitter)
		{
			uint nIndex = pColumn->GetVisibleIndex();
			if((int)nIndex > 0)
			{
				GrColumnList* pColumnList = pColumn->GetColumnList();
				pColumn = pColumnList->GetVisibleColumn(nIndex - 1);
				if(pColumn->GetResizable() == true)
					return pColumn;
			}
		}
		return nullptr;
	}

	void ColumnResizing::OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ /*service*/)
	{
		m_pColumn = GetResizingColumn((GrColumn*)hitTest.pHittedCell, hitTest.localPoint);

		GrRect rtDisplay = m_pColumn->GetDisplayRect();

		m_resizingStart		= rtDisplay.right;
		m_resizingLocation  = rtDisplay.right;
		m_resizingMin		= rtDisplay.left + m_pColumn->GetMinWidth();
		m_resizingMax		= rtDisplay.left + m_pColumn->GetMaxWidth();

		m_downX = location.X;
	}

	void ColumnResizing::OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle)
	{
		g->DrawLine(m_pen, m_resizingLocation, displayRectangle.Top, m_resizingLocation, displayRectangle.Bottom);
	}

	void ColumnResizing::OnMouseDragMove(_Point location, HitTest /*hitTest*/)
	{
		int nOldX = m_resizingLocation;
		m_resizingLocation = (location.X - m_downX) + m_resizingStart;
		m_resizingLocation = System::Math::Max(m_resizingLocation, m_resizingMin);
		m_resizingLocation = System::Math::Min(m_resizingLocation, m_resizingMax);

		if(nOldX != m_resizingLocation)
		{
			int x1, x2;
			x1 = System::Math::Min(nOldX, m_resizingLocation) - 1;
			x2 = System::Math::Max(nOldX, m_resizingLocation) + 1;
			Invalidate(x1, GridControl->DisplayRectangle.Top, x2, GridControl->DisplayRectangle.Bottom);
		}
	}

	void ColumnResizing::OnMouseDragEnd(bool cancel, HitTest /*hitTest*/)
	{
		if(cancel == true)
		{
			Invalidate();
			return;
		}

		int nNewWidth = m_pColumn->GetWidth() + (m_resizingLocation - m_resizingStart);
		if(m_pColumn->GetWidth() != nNewWidth)
		{
			GrColumnList* pColumnList = m_pColumn->GetColumnList();
			int x = int::MaxValue;
			if(m_pColumn->GetFullSelected() == true)
			{
				for(uint i=0 ; i<pColumnList->GetVisibleColumnCount() ; i++)
				{
					GrColumn* pColumn = pColumnList->GetVisibleColumn(i);
					if(pColumn->GetFullSelected() == false)
						continue;
					if(pColumn->GetResizable() == false)
						continue;
					pColumn->SetWidth(nNewWidth);
					x = System::Math::Min(x, pColumn->GetDisplayX());
				}
				if(x == int::MaxValue)
					x = 0;
			}
			else
			{
				m_pColumn->SetWidth(nNewWidth);
				x = m_pColumn->GetDisplayX();
			}
			Invalidate(x, GridControl->DisplayRectangle.Top, GridControl->DisplayRectangle.Right, GridControl->DisplayRectangle.Bottom);
		}
		else
		{
			Invalidate();
		}
	}

	void ColumnResizing::OnMouseDoubleClick(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{
		GrColumnList* pColumnList = GridCore->GetColumnList();

		if(m_pColumn->GetFullSelected() == true)
		{
			for(uint i=0 ; i<pColumnList->GetVisibleColumnCount() ; i++)
			{
				GrColumn* pColumn = pColumnList->GetVisibleColumn(i);
				if(pColumn->GetFullSelected() == false)
					continue;
				if(pColumn->GetResizable() == false)
					continue;
				pColumn->SetFit();
			}
		}
		else
		{
			m_pColumn->SetFit();
		}
		Invalidate();
	}

	ColumnSplitterMoving::ColumnSplitterMoving(_GridControl^ gridControl)
		: StateBase(gridControl)
	{
		using namespace System::Drawing;
		using namespace System::Drawing::Drawing2D;
		m_brush = gcnew HatchBrush(HatchStyle::Percent50, Color::White, Color::Black);
	}

	bool ColumnSplitterMoving::GetHitTest(GrCell* pHitted, _Point /*localLocation*/)
	{
		return pHitted->GetCellType() == GrCellType_Splitter;	
	}

	void ColumnSplitterMoving::OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle)
	{
		g->FillRectangle(m_brush, m_location, displayRectangle.Top, DEF_SPLITTER_SIZE, displayRectangle.Height);	
	}

	void ColumnSplitterMoving::OnBegin(GrCell* pCell, object^ /*data*/, IStateChangeService^ /*service*/)
	{
		m_pColumnSplitter = dynamic_cast<GrColumnSplitter*>(pCell);
	}

	void ColumnSplitterMoving::OnMouseDragBegin(_Point /*location*/)
	{
		m_location = m_pColumnSplitter->GetDisplayPosition().x;
		Invalidate();
	}

	void ColumnSplitterMoving::OnMouseDragMove(_Point location, HitTest /*hitTest*/)
	{
		GrColumnList* pColumnList = m_pColumnSplitter->GetColumnList();

		int oldLocation = m_location;

		for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
		{
			const GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
			GrRect rtDisplay = pColumn->GetDisplayRect();
			if(location.X < rtDisplay.left || location.X >= rtDisplay.right)
				continue;
			if(pColumn->HasFlag(GRCF_CELL_CLIPPED))
				continue;
			int nCenter = rtDisplay.GetCenter().x;

			if(location.X > nCenter)
			{
				if(pColumn->GetFrozen())
				{
					m_location = rtDisplay.right;
					m_frozenIndex = i;
				}
				else
				{
					m_location = rtDisplay.right - DEF_SPLITTER_SIZE;
					m_frozenIndex = i;
				}
			}
			else
			{
				if(pColumn->GetFrozen())
					m_location = rtDisplay.left;
				else
					m_location = rtDisplay.left - DEF_SPLITTER_SIZE;
				m_frozenIndex = i - 1;
			}
		}

		if(oldLocation != m_location)
		{
			Invalidate(oldLocation - 2, GridControl->DisplayRectangle.Top, oldLocation + DEF_SPLITTER_SIZE + 2, GridControl->DisplayRectangle.Bottom);
			Invalidate(m_location - 2, GridControl->DisplayRectangle.Top, m_location + DEF_SPLITTER_SIZE + 2, GridControl->DisplayRectangle.Bottom);
		}
	}

	void ColumnSplitterMoving::OnMouseDragEnd(bool cancel, HitTest /*hitTest*/)
	{
		if(cancel == true)
		{
			Invalidate(m_location - 2, GridControl->DisplayRectangle.Top, m_location + DEF_SPLITTER_SIZE + 2, GridControl->DisplayRectangle.Bottom);
			return;
		}

		GrColumnList* pColumnList = m_pColumnSplitter->GetColumnList();

		for(uint i=0 ; i<pColumnList->GetDisplayableColumnCount() ; i++)
		{
			GrColumn* pColumn = pColumnList->GetDisplayableColumn(i);
			bool oldFrozen = pColumn->GetFrozen();
			if(m_frozenIndex == -1)
				pColumn->SetFrozen(false);
			else
				pColumn->SetFrozen(i <= (uint)m_frozenIndex);

			if(oldFrozen != pColumn->GetFrozen())
			{

			}
		}

		Invalidate();
		//Invalidate(pColumnList->, GridControl->DisplayRectangle.Top, m_location + DEF_SPLITTER_SIZE + 2, GridControl->DisplayRectangle.Bottom);
	}

	ItemPressing::ItemPressing(_GridControl^ gridControl)
		: StateBase(gridControl)
	{
		m_timer = gcnew Private::SelectionTimer(gridControl);
		m_timer->Interval = 300;
		m_timer->Elapsed  += gcnew System::Timers::ElapsedEventHandler(this, &ItemPressing::timer_Elapsed);
	}

	bool ItemPressing::GetHitTest(GrCell* pHitted, _Point localLocation)
	{
		if(pHitted->GetCellType() != GrCellType_Item)
			return false;

		GrItem* pItem = (GrItem*)pHitted;
		if(pItem->GetReadOnly() == true)
			return true;
		if(pItem->GetFocused() == true)
			return false;

		if(pHitted->HitMouseOverTest(localLocation) != GrMouseOverState_In)
			return false;
		Cell^ cell = Cell::FromNative(pItem);
		return cell->IsBeingEdited == false;
	}

	void ItemPressing::OnMouseDown(_Point /*location*/, HitTest hitTest, IStateChangeService^ /*service*/)
	{
		m_pItem = dynamic_cast<GrItem*>(hitTest.pHittedCell);

		if(GridControl->SelectionRange == _SelectionRange::Multi)
		{
			if(Selector->CanSelect(m_pItem) == true)
			{
				GrIndexRange columnRange = Selector->GetColumnSelections(m_pItem);
				GrIndexRange rowRange    = Selector->GetRowSelections(m_pItem);

				switch(GridControl->SelectionType)
				{
				case _SelectionType::Add:
					break;
				case _SelectionType::Normal:
					Selector->ClearSelection();
					break;
				}

				if(rowRange != GrIndexRange::Empty)
				{
					Selector->BeginSelecting();
					Selector->Selecting(columnRange, rowRange);
					Focuser->SetFocusing(m_pItem);
				}
			}
		}
		else
		{
			switch(GridControl->SelectionType)
			{
			case _SelectionType::Add:
				{
					if(Selector->CanSelect(m_pItem) == true)
					{
						if(GridControl->FullRowSelect == true)
							Selector->SelectItems(m_pItem->GetDataRow(), GrSelectionType_Add);
						else
							Selector->SelectItem(m_pItem, GrSelectionType_Add);
						Selector->SetAnchor(m_pItem);
						Focuser->Set(m_pItem);
					}
				}
				break;
			case _SelectionType::Normal:
				{
					if(GridControl->FullRowSelect == true)
						Selector->SelectItems(m_pItem->GetDataRow(), GrSelectionType_Normal);
					else
						Selector->SelectItem(m_pItem, GrSelectionType_Normal);

					Selector->SetSelectionGroup(m_pItem);
					Selector->SetAnchor(m_pItem);
					Focuser->Set(m_pItem);
				}
				break;
			}

			GridCore->SetMousePressed(m_pItem);
			GridControl->EnsureVisible(GridControl->FocusedCell);
		}
	}

	void ItemPressing::OnMouseDoubleClick(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{
		Cell^ cell = Cell::FromNative(m_pItem);
		if(cell->IsReadOnly == true)
		{
			GridControl->InvokeCellDoubleClicked(cell);
		}
	}

	void ItemPressing::OnMouseUp(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{
		if(Selector->IsSelecting())
			Selector->EndSelecting(GridControl->NativeSelectionType);
	}

	void ItemPressing::OnMouseDragBegin(_Point /*location*/)
	{
		if(Selector->IsSelecting() == false)
			Selector->BeginSelecting();

		switch(GridControl->SelectionType)
		{
		case _SelectionType::Add:
			break;
		case _SelectionType::Normal:
			Selector->ClearSelection();
			break;
		}

		GridCore->SetMouseOver(nullptr, GrPoint::Empty);
		m_timer->Start();
	}

	void ItemPressing::OnMouseDragMove(_Point location, HitTest /*hitTest*/)
	{
		FindSelection fs(GridControl, location);

		Selector->Selecting(fs.ColumnRange, fs.RowRange);
		Focuser->SetFocusing(fs.Focus);
		m_timer->MouseLocation = GridControl->PointToScreen(location);
	}

	void ItemPressing::OnMouseDragEnd(bool /*cancel*/, HitTest /*hitTest*/)
	{
		m_timer->Stop();

		if(Selector->IsSelecting())
			Selector->EndSelecting(GridControl->NativeSelectionType);
	}

	void ItemPressing::timer_Elapsed(System::Object^ /*sender*/, System::Timers::ElapsedEventArgs^ /*e*/)
	{
		if(m_timer->DoScroll() == true)
			Invalidate(); 
	}

	ItemPressing::FindSelection::FindSelection(_GridControl^ gridControl, _Point location)
		: GridObject(gridControl)
	{
		GrIndexRange columnRange = GetColumnSelections(location.X);
		GrIndexRange rowRange    = GetRowSelections(location.Y);

		m_columnBegin	= columnRange.GetMinValue();
		m_columnEnd		= columnRange.GetMaxValue();
		m_rowBegin		= rowRange.GetMinValue();
		m_rowEnd		= rowRange.GetMaxValue();

		GrColumn* pFocusColumn = GetFocusColumn(m_columnBegin, m_columnEnd);
		IDataRow*     pFocusRow    = GetFocusRow(m_rowBegin, m_rowEnd);

		m_pFocus = pFocusRow->GetFocusable(pFocusColumn);
	}

	GrIndexRange ItemPressing::FindSelection::GetColumnSelections(int x)
	{
		GrColumnList*  pColumnList  = GridCore->GetColumnList();
		x = pColumnList->DisplayToGlobal(x);

		GrIntRange range(x, Selector->GetColumnAnchor()->GetX());

		GrIndexRange indexRange;
		pColumnList->HitTest(range.GetMinValue(), range.GetMaxValue(), &indexRange);
		return indexRange;
	}

	GrIndexRange ItemPressing::FindSelection::GetRowSelections(int y)
	{
		GrDataRowList* pDataRowList = GridCore->GetDataRowList();

		if(Selector->GetRowAnchor() == GridCore->GetInsertionRow())
			return GrIndexRange(INSERTION_ROW, INSERTION_ROW+1);
		y = pDataRowList->DisplayToGlobal(y);

		GrIntRange range(y, Selector->GetRowAnchor()->GetY());

		GrIndexRange indexRange;
		pDataRowList->HitTest(range.GetMinValue(), range.GetMaxValue(), &indexRange);
		return indexRange;
	}

	IDataRow* ItemPressing::FindSelection::GetFocusRow(uint begin, uint end)
	{
		GrDataRowList* pDataRowList = GridCore->GetDataRowList();

		IDataRow* pAnchorRow = Selector->GetRowAnchor();
		IDataRow* pFocusRow = NULL;

		int distance = 0;

		for(uint i=begin ; i<end ; i++)
		{
			IDataRow* pDataRow = pDataRowList->GetVisibleRow(i);

			int dist = abs(pDataRow->GetY() - pAnchorRow->GetY());

			if(dist >= distance)
			{
				pFocusRow = pDataRow;
				distance = dist;
			}
		}

		return pFocusRow;
	}

	GrColumn* ItemPressing::FindSelection::GetFocusColumn(uint begin, uint end)
	{
		GrColumnList* pColumnList = GridCore->GetColumnList();

		GrColumn* pAnchorColumn = Selector->GetColumnAnchor();
		GrColumn* pFocusColumn  = NULL;

		int distance = 0;

		for(uint i=begin ; i<end ; i++)
		{
			GrColumn* pColumn = pColumnList->GetVisibleColumn(i);

			int dist = abs(pColumn->GetX() - pAnchorColumn->GetX());

			if(dist >= distance)
			{
				pFocusColumn = pColumn;
				distance = dist;
			}
		}

		return pFocusColumn;
	}

	ItemEditing::ItemEditing(_GridControl^ gridControl) 
		: StateBase(gridControl)
	{
		m_dropDownForm = gcnew Ntreev::Windows::Forms::Grid::Private::ColumnDropDownForm(gridControl);
	}

	bool ItemEditing::GetHitTest(GrCell* pHitted, _Point /*localLocation*/)
	{
		GrItem* pItem = dynamic_cast<GrItem*>(pHitted);
		if(pItem == nullptr)
			return false;
		return pItem->GetFocused() == true;
	}

	void ItemEditing::OnBegin(GrCell* pCell, object^ data, IStateChangeService^ service)
	{
		m_cell = Cell::FromNative((GrItem*)pCell);
		EditingReason^ reason = dynamic_cast<EditingReason^>(data);

		if(reason != nullptr && m_cell != nullptr)
			service->NextState = OnBegin(reason);
	}

	void ItemEditing::OnMouseDown(_Point location, HitTest /*hitTest*/, IStateChangeService^ service)
	{
		service->NextState = OnBegin(gcnew EditingReason(location));
	}

	void ItemEditing::OnMouseUp(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ service)
	{
		service->NextState = this->State;
	}

	_State ItemEditing::OnBegin(EditingReason^ by)
	{
		using namespace System::Windows::Forms;

		if(GridControl->InvokeBeginEdit(m_cell) == false)
			return _State::Normal;

		GridControl->FocusedCell->Row->BeginEdit();
		m_result	= Result::None;
		m_column	= m_cell->Column;

		EditValueEventArgs eve(by, m_cell->Value);
		m_column->Invoke_EditValue(%eve);
		if(object::Equals(eve.Value, m_cell->Value) == false)
			m_cell->Value = eve.Value;

		if(eve.SuppressEditing == true)
		{
			m_cell = nullptr;
			return _State::Normal;
		}

		IControlAttacher^ controlAttacher = dynamic_cast<IControlAttacher^>(m_column);
		if(controlAttacher != nullptr)
		{
			Control^ editingControl = controlAttacher->EditingControl;
			editingControl->SuspendLayout();
			editingControl->ImeMode		= GridControl->ImeMode;
			editingControl->AutoSize	= false;
			editingControl->BackColor	= m_cell->BackColor;
			editingControl->ForeColor	= m_cell->ForeColor;
			editingControl->Font		= m_cell->Font;
			editingControl->ResumeLayout();
		}

		m_cell->NativeRef->LockColor(true);
		GridControl->Refresh();

		if(controlAttacher != nullptr)
		{
			controlAttacher->InvokeAttaching(by, m_cell->Value);
		}

		switch(m_column->EditingType)
		{
		case _EditingType::Control:
			{
				IColumnControl^ columnControl = dynamic_cast<IColumnControl^>(m_column);

				Application::AddMessageFilter(this);

				_Rectangle clientRectangle = m_cell->ClientRectangle;
				Control^ editingControl = columnControl->EditingControl;
				GridControl->SuspendLayout();
				columnControl->SetControlLayout(columnControl->EditingControl, clientRectangle.Location, clientRectangle.Size);
				editingControl->Visible	= true;
				GridControl->Controls->Add(editingControl);
				GridControl->ResumeLayout(false);
				GridControl->PerformLayout();

				try
				{
					columnControl->SetValue(m_cell->Value);
				}
				catch(System::Exception^)
				{

				}

				if(by->EditingReasonType == EditingReasonType::Mouse)
				{
					_Point pt = by->Location;
					_Rectangle rectangle(editingControl->Location, editingControl->Size);
					if(rectangle.Contains(pt) == true)
					{
						pt = this->GridControl->PointToScreen(pt);
						pt = editingControl->PointToClient(pt);
						Win32::API::SendMessage(editingControl->Handle, Win32::API::WM_LBUTTONDOWN, System::IntPtr::Zero, Win32::API::MakeLParam(pt.X,pt.Y));
						//Win32::API::InvokeLButtonDownEvent(pt);
					}
				}
				else
				{
					editingControl->Focus();
				}

				columnControl->InvokeAttached(by, m_cell->Value);
			}
			break;
		case _EditingType::DropDown:
			{
				IColumnDropDown^ columnDropDown = dynamic_cast<IColumnDropDown^>(m_column);
				_Rectangle screenRect = GridControl->RectangleToScreen(m_cell->DisplayRectangle);

				m_dropDownForm->Show(columnDropDown, m_cell, screenRect);
				m_dropDownForm->VisibleChanged += gcnew System::EventHandler(this, &ItemEditing::dropDownForm_VisibleChanged);

				try
				{
					columnDropDown->SetValue(m_cell->Value);
				}
				catch(System::Exception^)
				{

				}

				columnDropDown->InvokeAttached(by, m_cell->Value);
			}
			break;
		case _EditingType::Modal:
			break;
		}

		m_column->EditingResultChanged += gcnew ColumnEventHandler(this, &ItemEditing::OnEditingResultChanged);

		if(controlAttacher != nullptr)
			controlAttacher->EditingControl->PreviewKeyDown += gcnew PreviewKeyDownEventHandler(this, &ItemEditing::OnColumnControlPreviewKeyDown);

		GridCore->ShowClippedText(false);

		return State;
	}

	void ItemEditing::OnEnd(IStateChangeService^ /*service*/)
	{
		using namespace System::Windows::Forms;
		if(m_cell == nullptr)
			return;

		m_cell->NativeRef->LockColor(false);

		CellEventArgs^ e = gcnew CellEventArgs(m_cell);
		GridControl->InvokeEndEdit(e);

		GridCore->ShowClippedText(true);

		if(m_cell->IsReadOnly == true)
			m_result = Result::Cancel;

		bool modified = false;
		switch(m_result)
		{
		case Result::OK:
			modified = true;
			break;
		case Result::None:
			if(m_column->DefaultEditingResult == _EditingResult::Ok)
				modified = true;
			break;
		}

		IControlAttacher^ controlAttacher = dynamic_cast<IControlAttacher^>(m_column);
		if(controlAttacher != nullptr)
		{
			controlAttacher->InvokeDetaching(modified, m_cell->Value);
			if(modified == true)
			{
				try
				{
					m_cell->Value = controlAttacher->GetValue();
				}
				catch(System::Exception^ e)
				{
					System::Windows::Forms::MessageBox::Show(e->Message);
					modified = false;
				}
			}

			controlAttacher->EditingControl->PreviewKeyDown -= gcnew PreviewKeyDownEventHandler(this,	&ItemEditing::OnColumnControlPreviewKeyDown);
		}

		m_column->EditingResultChanged -= gcnew ColumnEventHandler(this, &ItemEditing::OnEditingResultChanged);

		switch(m_column->EditingType)
		{
		case _EditingType::Control:
			{
				IColumnControl^ columnControl = dynamic_cast<IColumnControl^>(m_column);
				Application::RemoveMessageFilter(this);
				columnControl->EditingControl->Visible = false;
				GridControl->Controls->Remove(columnControl->EditingControl);
			}
			break;
		case _EditingType::DropDown:
			{
				m_dropDownForm->VisibleChanged -= gcnew System::EventHandler(this, &ItemEditing::dropDownForm_VisibleChanged);
				m_dropDownForm->Visible = false;
			}
			break;
		case _EditingType::Modal:
			break;
		}

		if(controlAttacher != nullptr)
		{
			controlAttacher->InvokeDetached(modified, m_cell->Value);
			GridControl->ImeMode = controlAttacher->EditingControl->ImeMode;
		}

		if(GridControl->Focused == false)
			GridControl->Focus();

		if(GridCore->IsGrouped() == true)
			GridCore->GetDataRowList()->Update(true);

		GridControl->EnsureVisible(m_cell);
	}

	bool ItemEditing::PreFilterMessage(System::Windows::Forms::Message% m)
	{
		switch (m.Msg)
		{				
		case Win32::API::WM_LBUTTONDOWN:
		case Win32::API::WM_RBUTTONDOWN:
		case Win32::API::WM_MBUTTONDOWN:
		case Win32::API::WM_NCLBUTTONDOWN:
		case Win32::API::WM_NCRBUTTONDOWN:
		case Win32::API::WM_NCMBUTTONDOWN:
			{
				_Point cursorPos = System::Windows::Forms::Cursor::Position;
				_Rectangle rect = GridControl->RectangleToScreen(GridControl->DisplayRectangle);
				if (!rect.Contains(cursorPos))
				{
					LeaveEdit();
				}
			}
			break;
		}

		return false;
	}

	void ItemEditing::dropDownForm_VisibleChanged(object^ /*sender*/, _EventArgs^ /*e*/)
	{
		LeaveEdit();
	}

	void ItemEditing::OnEditingResultChanged(object^ /*sender*/, ColumnEventArgs^ e)
	{
		LeaveEdit(e->Column->EditingResult == _EditingResult::Ok ? true : false);
	}

	void ItemEditing::OnColumnControlPreviewKeyDown(object^ /*sender*/, System::Windows::Forms::PreviewKeyDownEventArgs^ ke)
	{
		switch(ke->KeyValue)
		{
		case _Keys::Escape:
			{
				LeaveEdit(false);
			}
			break;
		case _Keys::Enter:
			{
				if(ke->Control == true || ke->Alt == true || ke->Shift == true)
					break;
				LeaveEdit(true);
			}
			break;
		case _Keys::Tab:
			{
				LeaveEdit();
				if(ke->Shift == true)
					GridControl->CellIterator->MoveLeft(_SelectionRange::One);
				else
					GridControl->CellIterator->MoveRight(_SelectionRange::One);
			}
			break;
		}
	}

	void ItemEditing::LeaveEdit(bool update)
	{
		if(update == true)
			m_result = Result::OK;
		else
			m_result = Result::Cancel;
		GridControl->EndEditCell();
	}

	void ItemEditing::LeaveEdit()
	{
		LeaveEdit(m_column->DefaultEditingResult == _EditingResult::Ok);
	}

	ItemButtonPressing::ItemButtonPressing(_GridControl^ gridControl)
		: StateBase(gridControl)
	{

	}

	bool ItemButtonPressing::GetHitTest(GrCell* pHitted, _Point localLocation)
	{
		if(pHitted->GetCellType() != GrCellType_Item)
			return false;
		return pHitted->HitMouseOverTest(localLocation) == GrMouseOverState_Control;
	}

	void ItemButtonPressing::OnMouseDown(_Point /*location*/, HitTest hitTest, IStateChangeService^ /*service*/)
	{
		m_pItem = dynamic_cast<GrItem*>(hitTest.pHittedCell);
		Focuser->Set(m_pItem);
		GridCore->SetMousePressed(m_pItem);
		Invalidate();
	}

	void ItemButtonPressing::OnMouseUp(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ service)
	{
		service->NextState = _State::ItemEditing;
		service->Reference = m_pItem;
		service->Data = gcnew EditingReason();
		GridCore->SetMouseUnpressed();
		Invalidate();
	}


	GroupingExpandPressing::GroupingExpandPressing(_GridControl^ gridControl)
		: StateBase(gridControl)
	{

	}

	bool GroupingExpandPressing::GetHitTest(GrCell* pHitted, _Point localLocation)
	{
		GrGroupingCell* pGroupingCell = dynamic_cast<GrGroupingCell*>(pHitted);
		if(pGroupingCell == nullptr)
			return false;

		return pGroupingCell->HitMouseOverTest(localLocation) == GrMouseOverState_Control;
	}

	void GroupingExpandPressing::OnBegin(GrCell* pCell, object^ /*data*/, IStateChangeService^ /*service*/)
	{
		GrGroupingCell* pGroupingCell = dynamic_cast<GrGroupingCell*>(pCell);

		m_pGroupingRow = (GrGroupingRow*)pGroupingCell->GetRow();
	}

	void GroupingExpandPressing::OnMouseUp(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{
		m_pGroupingRow->Expand(!m_pGroupingRow->IsExpanded());
		Invalidate();
	}

	GroupingCellPressing::GroupingCellPressing(_GridControl^ gridControl)
		: StateBase(gridControl)
	{

	}

	bool GroupingCellPressing::GetHitTest(GrCell* pHitted, _Point localLocation)
	{
		GrGroupingCell* pGroupingCell = dynamic_cast<GrGroupingCell*>(pHitted);
		if(pGroupingCell == nullptr)
			return false;

		return pGroupingCell->HitMouseOverTest(localLocation) == GrMouseOverState_In;
	}

	void GroupingCellPressing::OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ /*service*/)
	{
		m_pGroupingCell = dynamic_cast<GrGroupingCell*>(hitTest.pHittedCell);
		IDataRow* pDataRow = m_pGroupingCell->GetIDataRow();

		if(GridControl->SelectionRange == _SelectionRange::Multi)
		{
			if(Selector->CanSelect(pDataRow) == true)
			{
				Selector->SelectDataRows(pDataRow, Selector->GetRowAnchor(), GridControl->NativeSelectionType);
				Focuser->Set(m_pGroupingCell);
			}
		}
		else
		{
			switch(GridControl->SelectionType)
			{
			case _SelectionType::Normal:
				{
					Selector->SetSelectionGroup(pDataRow);
					Selector->SetRowAnchor(pDataRow);
					GrColumnList* pColumnList = GridCore->GetColumnList();
					int x = pColumnList->DisplayToGlobal(location.X);

					GrColumn* pColumn = pColumnList->HitTest(x);
					if(pColumn)
						Selector->SetColumnAnchor(pColumn);
					Selector->ClearSelection();
					Focuser->Set(m_pGroupingCell);
				}
				break;
			default:
				{
					if(Selector->CanSelect(pDataRow) == true)
					{
						//Selector->SelectItems(pDataRow, GrSelectionType_Add);
						Focuser->Set(m_pGroupingCell);
						GridCore->SetMousePressed(m_pGroupingCell);
					}
				}
				break;
			}
		}
		Invalidate();
	}

	void GroupingCellPressing::OnMouseUp(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{
		GridCore->SetMouseUnpressed();
		Invalidate();
	}

	void GroupingCellPressing::OnMouseDoubleClick(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{
		GrGroupingRow* pGroupingRow = (GrGroupingRow*)m_pGroupingCell->GetRow();
		pGroupingRow->Expand(!pGroupingRow->IsExpanded());
		Invalidate();
	}

	GroupingInfoPressing::GroupingInfoPressing(_GridControl^ gridControl) 
		: StateBase(gridControl)
	{
		m_brush = gcnew System::Drawing::SolidBrush(_Color::Black);
	}

	bool GroupingInfoPressing::GetHitTest(GrCell* pHitted, _Point /*localLocation*/)
	{
		return pHitted->GetCellType() == GrCellType_GroupingInfo;
	}

	void GroupingInfoPressing::OnPaintAdornments(_Graphics^ g, _Rectangle% /*displayRectangle*/)
	{
		if(m_where == INVALID_INDEX)
			return;

		GrGroupingList* pGroupingList = GridCore->GetGroupingList();
		GrRect rtDisplay;
		if(m_where < pGroupingList->GetGroupingCount())
		{
			GrGroupingInfo* pGroupingInfo = pGroupingList->GetGrouping(m_where);
			rtDisplay = pGroupingInfo->GetDisplayRect();
		}
		else
		{
			uint nLastIndex = pGroupingList->GetGroupingCount() - 1;
			GrGroupingInfo* pGroupingInfo = pGroupingList->GetGrouping(nLastIndex);
			rtDisplay = pGroupingList->GetDisplayRect();
			rtDisplay.left = pGroupingInfo->GetDisplayRect().right;
		}

		GrPoint ptCenter = rtDisplay.GetCenter();

		GrRect rtRender;

		const int padding = 2;
		rtRender = GrRect(rtDisplay.left - padding, rtDisplay.top, rtDisplay.left + padding, rtDisplay.bottom);


		g->FillRectangle(m_brush, rtRender);
	}

	void GroupingInfoPressing::OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ /*service*/)
	{
		m_pGroupingInfo = (GrGroupingInfo*)hitTest.pHittedCell;
		GridCore->SetMousePressed(m_pGroupingInfo);
		m_where		= INVALID_INDEX;
		m_location	= location;
		m_cursor	= nullptr;
	}

	void GroupingInfoPressing::OnMouseDragMove(_Point location, HitTest hitTest)
	{
		GrCell* pHittedCell = hitTest.pHittedCell;
		m_cursor = System::Windows::Forms::Cursors::No;

		if(pHittedCell == nullptr)
			return;

		GrRow* pRow = pHittedCell->GetRow();

		m_where = INVALID_INDEX;
		m_targetType = TargetType::Unknown;

		if(pRow->GetRowType() == GrRowType_GroupingList)
		{
			if(hitTest.CellType == GrCellType_GroupingInfo)
			{
				GrGroupingInfo* pHittedInfo = (GrGroupingInfo*)pHittedCell;
				if(pHittedInfo != m_pGroupingInfo && m_pGroupingInfo->GetGroupingLevel() + 1 != pHittedInfo->GetGroupingLevel())
				{
					m_where = dynamic_cast<GrGroupingInfo*>(pHittedCell)->GetGroupingLevel();
					m_targetType = TargetType::Grouping;
				}
			}
			else
			{
				GrGroupingList* pGroupingList = (GrGroupingList*)pRow;
				uint nLastIndex = pGroupingList->GetGroupingCount() - 1;

				if(m_pGroupingInfo->GetGroupingLevel() != nLastIndex)
				{
					GrGroupingInfo* pLastGrouping = pGroupingList->GetGrouping(nLastIndex);
					if(location.X > pLastGrouping->GetDisplayRect().right)
					{
						m_where = pGroupingList->GetGroupingCount();
						m_targetType = TargetType::Grouping;
					}
				}
			}
		}
		else
		{
			m_targetType = TargetType::Remove;
		}

		switch(m_targetType)
		{
		case TargetType::Grouping:
			m_cursor = Cursors::Movable;
			break;
		case TargetType::Remove:
			m_cursor = Cursors::Remove;
			break;
		}
		Invalidate();
	}

	void GroupingInfoPressing::OnMouseUp(_Point location, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{
		if(m_location == location)
		{
			GrSort::Type sortType = m_pGroupingInfo->GetSortType();
			if(sortType == GrSort::Up)
				m_pGroupingInfo->SetSortType(GrSort::Down);
			else
				m_pGroupingInfo->SetSortType(GrSort::Up);
		}

		GridCore->SetMouseUnpressed();
		Invalidate();
	}

	void GroupingInfoPressing::OnMouseDragEnd(bool cancel, HitTest /*hitTest*/)
	{
		if(cancel == true)
			return;

		switch(m_targetType)
		{
		case TargetType::Remove:
			{
				m_pGroupingInfo->SetGrouped(false);
			}
			break;
		case TargetType::Grouping:
			{
				GrGroupingList* pGroupingList = GridCore->GetGroupingList();
				uint toInsert = m_where;
				GrGroupingInfo* pWhere = nullptr;
				if(toInsert < pGroupingList->GetGroupingCount())
					pWhere = pGroupingList->GetGrouping(toInsert);
				GridCore->GetGroupingList()->ChangeGroupingInfo(m_pGroupingInfo, pWhere);
			}
			break;
		}
	}

	void GroupingInfoPressing::OnEnd(IStateChangeService^ /*service*/)
	{
		m_cursor = nullptr;
		m_where	 = INVALID_INDEX;
	}

	RowPressing::RowPressing(_GridControl^ gridControl)
		: StateBase(gridControl)
	{

	}

	bool RowPressing::GetHitTest(GrCell* pHitted, _Point localLocation)
	{
		if(pHitted->GetCellType() != GrCellType_Row)
			return false;
		if(localLocation.Y < GridControl->RowSplitter || localLocation.Y >= pHitted->GetHeight() - GridControl->RowSplitter)
			return false;
		return true;
	}

	void RowPressing::OnBegin(GrCell* pCell, object^ /*data*/, IStateChangeService^ /*service*/)
	{
		m_pRow = dynamic_cast<GrRow*>(pCell);

		switch(m_pRow->GetRowType())
		{
		case GrRowType_DataRow:
		case GrRowType_InsertionRow:
			{
				GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(m_pRow);

				if(GridControl->SelectionRange == _SelectionRange::Multi)
				{
					if(Selector->CanSelect(pDataRow) == true)
					{
						Selector->SelectDataRows(pDataRow, Selector->GetRowAnchor(), GridControl->NativeSelectionType);
						Focuser->Set(pDataRow);
					}
				}
				else
				{
					switch(GridControl->SelectionType)
					{
					case _SelectionType::Normal:
						{
							Selector->SetSelectionGroup(pDataRow->GetSelectionGroup());
							Selector->SetRowAnchor(pDataRow);
							Selector->SelectItems(pDataRow, GrSelectionType_Normal);
							Focuser->Set(pDataRow);
						}
						break;
					default:
						{
							if(Selector->CanSelect(pDataRow) == true)
							{
								Selector->SelectItems(pDataRow, GrSelectionType_Add);
								Focuser->Set(pDataRow);
							}
						}
						break;
					}
				}
			}
			break;
		case GrRowType_GroupingRow:
			{
				GrGroupingRow* pGroupingRow = (GrGroupingRow*)m_pRow;
				if(GridControl->SelectionRange == _SelectionRange::Multi)
				{
					if(Selector->CanSelect(pGroupingRow) == true)
					{
						Selector->SelectDataRows(pGroupingRow, Selector->GetRowAnchor(), GridControl->NativeSelectionType);
						Focuser->Set(pGroupingRow);
					}
				}
				else
				{
					switch(GridControl->SelectionType)
					{
					case _SelectionType::Normal:
						{
							Selector->SetSelectionGroup(pGroupingRow);
							Selector->SetRowAnchor(pGroupingRow);
							Selector->ClearSelection();
							Focuser->Set(pGroupingRow);
						}
						break;
					default:
						{
							if(Selector->CanSelect(pGroupingRow) == true)
							{
								Focuser->Set(pGroupingRow);
							}
						}
						break;
					}
				}
			}
			break;
		}
		GridCore->SetMousePressed(m_pRow);
	}

	void RowPressing::OnEnd(IStateChangeService^ /*service*/)
	{
		GridCore->SetMouseUnpressed();
	}

	RowResizing::RowResizing(_GridControl^ gridControl)
		: StateBase(gridControl)
	{
		using namespace System::Drawing;
		using namespace System::Drawing::Drawing2D;

		m_pen = gcnew Pen(Color::Black);
		m_pen->DashStyle = DashStyle::Custom;
		m_pen->DashCap	 = DashCap::Round;
		cli::array<System::Single>^ dashPattern = {1.0f, 1.0f, };

		m_pen->DashPattern = dashPattern;
	}

	bool RowResizing::GetHitTest(GrCell* pHitted, _Point localLocation)
	{
		GrRow* pRow = dynamic_cast<GrRow*>(pHitted);
		if(pRow == nullptr)
			return false;
		return GetResizingRow(pRow, localLocation) != nullptr;
	}

	GrRow* RowResizing::GetResizingRow(GrRow* pRow, _Point localPoint)
	{
		int nRowSplitter = GridControl->RowSplitter;

		if(localPoint.Y >= pRow->GetHeight() - nRowSplitter)
		{
			if(pRow->GetResizable() == true)
				return pRow;
		}
		else if(localPoint.Y < nRowSplitter)
		{
			IDataRow* pDataRow = dynamic_cast<IDataRow*>(pRow);
			if(pDataRow && (int)pDataRow->GetVisibleIndex() > 0)
			{
				GrDataRowList* pDataRowList = pDataRow->GetGridCore()->GetDataRowList();
				pDataRow = pDataRowList->GetVisibleRow(pDataRow->GetVisibleIndex() - 1);
				if(pDataRow->GetResizable())
					return pDataRow;
			}
		}
		return nullptr;
	}

	void RowResizing::OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ /*service*/)
	{
		m_pRow = GetResizingRow((GrRow*)hitTest.pHittedCell, hitTest.localPoint);

		GrRect rtDisplay	= m_pRow->GetDisplayRect();

		m_resizingStart		= rtDisplay.bottom;
		m_resizingLocation  = rtDisplay.bottom;
		m_resizingMin		= rtDisplay.top + GridControl->RowSplitter;
		m_resizingMax		= rtDisplay.top + 1000;

		m_downY				= location.Y;
	}

	void RowResizing::OnMouseDoubleClick(_Point /*location*/, HitTest /*hitTest*/, IStateChangeService^ /*service*/)
	{
		GrDataRowList* pDataRowList = GridCore->GetDataRowList();

		IDataRow* p = dynamic_cast<IDataRow*>(m_pRow);
		if(p && p->GetFullSelected() == true)
		{
			for(uint i=0 ; i<pDataRowList->GetVisibleRowCount() ; i++)
			{
				GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(i));
				if(pDataRow == NULL)
					continue;
				if(pDataRow->GetFullSelected() == false)
					continue;
				if(pDataRow->GetResizable() == false)
					continue;
				pDataRow->SetFit();
			}
		}
		else
		{
			m_pRow->SetFit();
		}
		Invalidate();
	}

	void RowResizing::OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle)
	{
		g->DrawLine(m_pen, displayRectangle.Left, m_resizingLocation, displayRectangle.Right, m_resizingLocation);
	}

	void RowResizing::OnMouseDragMove(_Point location, HitTest /*hitTest*/)
	{
		int oldY = m_resizingLocation;
		m_resizingLocation = (location.Y - m_downY) + m_resizingStart;
		m_resizingLocation = System::Math::Max(m_resizingLocation, m_resizingMin);
		m_resizingLocation = System::Math::Min(m_resizingLocation, m_resizingMax);

		if(oldY != m_resizingLocation)
		{
			int y1, y2;
			y1 = System::Math::Min(oldY, m_resizingLocation) - 1;
			y2 = System::Math::Max(oldY, m_resizingLocation) + 1;
			Invalidate(GridControl->DisplayRectangle.Left, y1, GridControl->DisplayRectangle.Right, y2);
		}
	}

	void RowResizing::OnMouseDragEnd(bool /*cancel*/, HitTest /*hitTest*/)
	{
		int nNewHeight = m_pRow->GetHeight() + (m_resizingLocation - m_resizingStart);
		if(m_pRow->GetHeight() != nNewHeight)
		{
			IDataRow* p = dynamic_cast<IDataRow*>(m_pRow);
			if(p && p->GetFullSelected() == true)
			{
				GrDataRowList* pDataRowList = m_pRow->GetGridCore()->GetDataRowList();
				int y = int::MaxValue;
				for(uint i=0 ; i<pDataRowList->GetVisibleRowCount() ; i++)
				{
					GrDataRow* pDataRow = dynamic_cast<GrDataRow*>(pDataRowList->GetVisibleRow(i));
					if(pDataRow == NULL)
						continue;
					if(pDataRow->GetFullSelected() == false)
						continue;
					if(pDataRow->GetResizable() == false)
						continue;
					pDataRow->SetHeight(nNewHeight);

					y = System::Math::Min(pDataRow->GetDisplayY(), y);
				}

				if(y == int::MaxValue)
					y = 0;
				Invalidate(GridControl->DisplayRectangle.Left, y, GridControl->DisplayRectangle.Right, GridControl->DisplayRectangle.Bottom);
			}
			else
			{
				m_pRow->SetHeight(nNewHeight);
				Invalidate(GridControl->DisplayRectangle.Left, m_pRow->GetY(), GridControl->DisplayRectangle.Right, GridControl->DisplayRectangle.Bottom);
			}
		}
		else
		{
			Invalidate();
		}
	}
} /*namespace GridState*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/