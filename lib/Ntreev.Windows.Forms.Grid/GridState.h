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


#pragma once

#include "GridBase.h"
#include "GridSelectionTimer.h"
#include "GridColumnDropDownForm.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace GridState
{
	enum class State : int
	{
		Normal,
		RootPressing,
		ColumnPressing,
		ColumnResizing,
		ColumnSplitterMoving,
		RowPressing,
		RowResizing,
		GroupingInfoPressing,
		GroupingCellPressing,
		GroupingExpandPressing,
		ItemPressing,
		ItemButtonPressing,
		ItemEditing,

		Unknown,
	};

	typedef Ntreev::Windows::Forms::Grid::GridState::State _State;

	interface class IStateChangeService
	{
		property _State NextState
		{
			void set(_State);
		}

		property GrCell* Reference
		{
			void set(GrCell*);
		}

		property object^ Data
		{
			void set(object^);
		}
	};

	ref class StateBase abstract : GridObject
	{
	public: // methods
		StateBase(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation);

		virtual void OnBegin(GrCell* pCell, object^ data, IStateChangeService^ service);
		virtual void OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle);
		virtual void OnEnd(IStateChangeService^ service);

		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service);
		virtual void OnMouseMove(_Point location, HitTest hitTest);
		virtual void OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ service);

		virtual void OnMouseDoubleClick(_Point location, HitTest hitTest, IStateChangeService^ service);
		
		virtual void OnMouseDragBegin(_Point location);
		virtual void OnMouseDragEnd(bool cancel, HitTest hitTest);
		virtual void OnMouseDragMove(_Point location, HitTest hitTest);

	public: // properties
		property _State State
		{
			virtual _State get() abstract;
		}

		property bool Dragable
		{
			virtual bool get() { return false; }
		}

		property System::Windows::Forms::Cursor^ Cursor
		{
			virtual System::Windows::Forms::Cursor^ get() { return System::Windows::Forms::Cursors::Default; }
		}

	protected: // classes
		ref class Cursors
		{
			static Cursors()
			{
				System::IO::Stream^ stream;
				stream = StateBase::typeid->Assembly->GetManifestResourceStream("Ntreev.Windows.Forms.Grid.Cursors.Movable.cur");
				m_movable = gcnew System::Windows::Forms::Cursor(stream);
				delete stream;

				stream = StateBase::typeid->Assembly->GetManifestResourceStream("Ntreev.Windows.Forms.Grid.Cursors.Add.cur");
				m_add = gcnew System::Windows::Forms::Cursor(stream);
				delete stream;

				stream = StateBase::typeid->Assembly->GetManifestResourceStream("Ntreev.Windows.Forms.Grid.Cursors.Remove.cur");
				m_remove = gcnew System::Windows::Forms::Cursor(stream);
				delete stream;
			}

		public: // properties
			static property System::Windows::Forms::Cursor^ Movable
			{
				System::Windows::Forms::Cursor^ get() { return m_movable; }
			}

			static property System::Windows::Forms::Cursor^ Add
			{
				System::Windows::Forms::Cursor^ get() { return m_add; }
			}

			static property System::Windows::Forms::Cursor^ Remove
			{
				System::Windows::Forms::Cursor^ get() { return m_remove; }
			}

		private: // variables
			static System::Windows::Forms::Cursor^ m_movable;
			static System::Windows::Forms::Cursor^ m_add;
			static System::Windows::Forms::Cursor^ m_remove;
		};
	};

	ref class StateManager : GridObject
	{
	public: // methods
		StateManager(_GridControl^ gridControl);

		void ChangeDefaultState();

		void ChangeState(_State state, GrCell* pCell, object^ userData);

		State GetHitTest(_Point location);

	public: // properties
		property _State State
		{
			_State get() { return m_state->State; }
		}

	private: // methods
		void gridControl_MouseDown(object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void gridControl_MouseMove(object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void gridControl_MouseUp(object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void gridControl_MouseDoubleClick(object^ sender, System::Windows::Forms::MouseEventArgs^ e);
		void gridControl_Paint(object^ sender, System::Windows::Forms::PaintEventArgs^ e);
		void gridControl_PreviewKeyDown(object^ sender, System::Windows::Forms::PreviewKeyDownEventArgs^ e);
		void gridControl_Clearing(object^ sender, _EventArgs^ e);

		void ChangeState(StateBase^ state, GrCell* pHitted, object^ userData);
		StateBase^	GetHitTest(GrCell* pHitted, _Point localHitted);
		_Rectangle	ComputeDragRectangle(_Point location);

	private: // variables
		cli::array<StateBase^>^	m_states;
		StateBase^				m_state;
		StateBase^				m_downState;
		StateBase^				m_defaultState;

		_Point					m_downLocation;
		_Rectangle				m_dragRectangle;

		bool					m_dragging;

	private: // classes
		ref class StateChangeService : IStateChangeService
		{
		public:
			StateChangeService(_State state)
			{
				this->NextState = state;
			}

			StateChangeService(_State state, GrCell* pCell)
			{
				this->NextState = state;
				this->Reference = pCell;
			}
		public:
			virtual property _State NextState;
			virtual property GrCell* Reference;
			virtual property object^ Data;
		};
	};

	ref class Normal : StateBase
	{
	public: // methods
		Normal(_GridControl^ gridControl);

		virtual void OnBegin(GrCell* pCell, object^ data, IStateChangeService^ service) override;
		virtual void OnMouseMove(_Point location, HitTest hitTest) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::Normal; } 
		}

	private:
		GrCell* m_pCell;
	};

	ref class RootPressing : StateBase
	{
	public: // methods
		RootPressing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;
		virtual void OnBegin(GrCell* pCell, object^ data, IStateChangeService^ service) override;
		virtual void OnEnd(IStateChangeService^ service) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::RootPressing; } 
		}
	};

	ref class ColumnPressing : StateBase
	{
	public: // methods
		ColumnPressing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;

		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle) override;

		virtual void OnMouseDragBegin(_Point location) override;
		virtual void OnMouseDragMove(_Point location, HitTest hitTest) override;
		virtual void OnMouseDragEnd(bool cancel, HitTest hitTest) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::ColumnPressing; } 
		}

		property bool Dragable
		{
			virtual bool get() override { return true; }
		}

		property System::Windows::Forms::Cursor^ Cursor
		{
			virtual System::Windows::Forms::Cursor^ get() override
			{ 
				if(m_cursor == nullptr)
					return StateBase::Cursor;

				return m_cursor;
			}
		}

	private: // methods
		void						timer_Elapsed(object^ sender, System::Timers::ElapsedEventArgs^  e);
		bool						EnableSorting(_Point location);

	private: // variables
		GrColumnList*				m_pColumnList;
		GrColumn*					m_pColumn;
		GrColumn*					m_pOldDataColumn;
		bool						m_handled;
		_Point						m_locationStart;
		System::Drawing::SolidBrush^ m_brush;
		System::Windows::Forms::Cursor^ m_cursor;

	private: // classes
		enum class TargetType
		{
			Unknown,

			Frozen,
			Unfrozen,
			GroupingList,
		} m_targetType;

		GrCell* m_targetCell;

		ref class SelectionTimer2 : Private::SelectionTimer
		{
		public:
			SelectionTimer2(_GridControl^ gridControl);

		protected:
			property _Rectangle		InsideRectangle		{ virtual _Rectangle get() override; }
			property _Rectangle		OutsideRectangle	{ virtual _Rectangle get() override; }
		}^ m_timer;
	};

	ref class ColumnResizing : StateBase
	{
	public: // methods
		ColumnResizing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;

		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseDoubleClick(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle) override;

		virtual void OnMouseDragMove(_Point location, HitTest hitTest) override;
		virtual void OnMouseDragEnd(bool cancel, HitTest hitTest) override;

	private: // methods
		GrColumn* GetResizingColumn(GrColumn* pColumn, _Point localPoint);

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::ColumnResizing; } 
		}

		property bool Dragable
		{
			virtual bool get() override { return true; }
		}

		property System::Windows::Forms::Cursor^ Cursor
		{
			virtual System::Windows::Forms::Cursor^ get() override { return System::Windows::Forms::Cursors::VSplit; }
		}
	
	private: // variables
		GrColumn*					m_pColumn;
		int							m_resizingStart;
		int							m_resizingLocation;
		int							m_resizingMin;
		int							m_resizingMax;

		int							m_downX;

		System::Drawing::Pen^		m_pen;
	};

	ref class ColumnSplitterMoving : StateBase
	{
	public: // methods
		ColumnSplitterMoving(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;

		virtual void OnBegin(GrCell* pCell, object^ data, IStateChangeService^ service) override;
		virtual void OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle) override;
		
		virtual void OnMouseDragBegin(_Point location) override;
		virtual void OnMouseDragMove(_Point location, HitTest hitTest) override;
		virtual void OnMouseDragEnd(bool cancel, HitTest hitTest) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::ColumnSplitterMoving; } 
		}

		property bool Dragable
		{
			virtual bool get() override { return true; }
		}

		property System::Windows::Forms::Cursor^ Cursor
		{
			virtual System::Windows::Forms::Cursor^ get() override { return System::Windows::Forms::Cursors::SizeWE; } 
		}

	private: // variables
		GrColumnSplitter*			m_pColumnSplitter;

		int							m_location;
		int							m_frozenIndex;

		System::Drawing::Drawing2D::HatchBrush^ m_brush;
	};

	ref class ItemPressing : StateBase
	{
	public: // methods
		ItemPressing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;
		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseDoubleClick(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ service) override;

		virtual void OnMouseDragBegin(_Point location) override;
		virtual void OnMouseDragMove(_Point location, HitTest hitTest) override;
		virtual void OnMouseDragEnd(bool cancel, HitTest hitTest) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::ItemPressing; } 
		}

		property bool Dragable
		{
			virtual bool get() override { return true; }
		}

	private: // methods
		void timer_Elapsed(System::Object^  sender, System::Timers::ElapsedEventArgs^  e);

	private: // variables
		GrItem*						m_pItem;
		Private::SelectionTimer^	m_timer;

		ref class FindSelection : GridObject
		{
		public:
			FindSelection(_GridControl^ gridControl, _Point location);

		public:
			property IFocusable* Focus
			{
				IFocusable* get() { return m_pFocus; }
			}

			property GrIndexRange ColumnRange
			{
				GrIndexRange get() { return GrIndexRange(m_columnBegin, m_columnEnd); }
			}

			property GrIndexRange RowRange
			{
				GrIndexRange get() { return GrIndexRange(m_rowBegin, m_rowEnd); } 
			}

		private:
			GrIndexRange GetColumnSelections(int x);
			GrIndexRange GetRowSelections(int y);
			IDataRow* GetFocusRow(uint begin, uint end);
			GrColumn* GetFocusColumn(uint begin, uint end);

		private:
			IFocusable*				m_pFocus;
			uint					m_columnBegin;
			uint					m_columnEnd;
			uint					m_rowBegin;
			uint					m_rowEnd;
		};
	};

	ref class ItemEditing : StateBase, System::Windows::Forms::IMessageFilter
	{
		typedef System::Windows::Forms::Keys	_Keys;
		typedef System::Windows::Forms::Form	_Form;
		enum class Result : int
		{
			None,
			OK,
			Cancel,
		};
	public: // methods
		ItemEditing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;
		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnBegin(GrCell* pCell, object^ data, IStateChangeService^ service) override;
		virtual void OnEnd(IStateChangeService^ service) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::ItemEditing; } 
		}

	private: // methods
		_State OnBegin(EditingReason^ by);

	private: // methods
		virtual bool PreFilterMessage(System::Windows::Forms::Message% m) sealed = System::Windows::Forms::IMessageFilter::PreFilterMessage;

		void dropDownForm_VisibleChanged(object^ /*sender*/, _EventArgs^ e);
		void OnEditingResultChanged(object^ sender, ColumnEventArgs^ e);
		void OnColumnControlPreviewKeyDown(object^ sender, System::Windows::Forms::PreviewKeyDownEventArgs^ ke);

		void LeaveEdit(bool update);
		void LeaveEdit();

	private:
		_Column^					m_column;
		_Cell^						m_cell;

		Result						m_result;

		Ntreev::Windows::Forms::Grid::Private::ColumnDropDownForm^ m_dropDownForm;
	};

	ref class ItemButtonPressing : StateBase
	{
	public: // methods
		ItemButtonPressing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;
		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ service) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::ItemButtonPressing; } 
		}

	private: // variables
		GrItem*						m_pItem;
	};

	ref class GroupingExpandPressing : StateBase
	{
	public: // methods
		GroupingExpandPressing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;
		virtual void OnBegin(GrCell* pCell, object^ data, IStateChangeService^ service) override;
		virtual void OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ service) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::GroupingExpandPressing; } 
		}

	private: // variables
		GrGroupingRow* m_pGroupingRow;
	};

	ref class GroupingCellPressing : StateBase
	{
	public: // methods
		GroupingCellPressing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;
		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseDoubleClick(_Point location, HitTest hitTest, IStateChangeService^ service) override;

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::GroupingCellPressing; } 
		}
	private: // variable;
		GrGroupingCell* m_pGroupingCell;
	};

	ref class GroupingInfoPressing : StateBase
	{
	public:
		GroupingInfoPressing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;
		virtual void OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle) override;

		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseUp(_Point location, HitTest hitTest, IStateChangeService^ service) override;

		virtual void OnMouseDragMove(_Point location, HitTest hitTest) override;
		virtual void OnMouseDragEnd(bool cancel, HitTest hitTest) override;

		virtual void OnEnd(IStateChangeService^ service) override;

	public: // properties
		property _State State 
		{ 
			virtual _State get() override { return _State::GroupingInfoPressing; } 
		}

		property bool Dragable 
		{
			virtual bool get() override { return true; }
		}

		property System::Windows::Forms::Cursor^ Cursor
		{
			virtual System::Windows::Forms::Cursor^ get() override
			{
				if(m_cursor == nullptr)
					return StateBase::Cursor;
				return m_cursor;
			}
		}

	private:
		GrGroupingInfo*				m_pGroupingInfo;
		_Point						m_location;
		uint						m_where;
		System::Windows::Forms::Cursor^ m_cursor;

		enum class TargetType
		{
			Grouping,
			Remove,
			Unknown,
		} m_targetType;

		System::Drawing::SolidBrush^ m_brush;
	};
	
	ref class RowPressing : StateBase
	{
	public: // methods
		RowPressing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;
		virtual void OnBegin(GrCell* pCell, object^ data, IStateChangeService^ service) override;
		virtual void OnEnd(IStateChangeService^ service) override;

	public: // properties
		property _State State
		{
			virtual _State get() override { return _State::RowPressing; } 
		}

	private: // variables
		GrRow* m_pRow;
	};

	ref class RowResizing : StateBase
	{
	public: // methods
		RowResizing(_GridControl^ gridControl);

		virtual bool GetHitTest(GrCell* pHitted, _Point localLocation) override;

		virtual void OnMouseDown(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnMouseDoubleClick(_Point location, HitTest hitTest, IStateChangeService^ service) override;
		virtual void OnPaintAdornments(_Graphics^ g, _Rectangle% displayRectangle) override;

		virtual void OnMouseDragMove(_Point location, HitTest hitTest) override;
		virtual void OnMouseDragEnd(bool cancel, HitTest hitTest) override;

	private: // methods
		GrRow* GetResizingRow(GrRow* pRow, _Point localPoint);

	public: // properties
		property _State State 
		{
			virtual _State get() override { return _State::RowResizing; } 
		}

		property bool Dragable 
		{
			virtual bool get() override { return true; } 
		}

		property System::Windows::Forms::Cursor^ Cursor
		{
			virtual System::Windows::Forms::Cursor^ get() override { return System::Windows::Forms::Cursors::HSplit; }
		}

	private: // variables
		GrRow*	m_pRow;
		int		m_resizingStart;
		int		m_resizingLocation;
		int		m_resizingMin;
		int		m_resizingMax;

		int		m_downY;

		System::Drawing::Pen^ m_pen;
	};
} /*namespace GridState*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/