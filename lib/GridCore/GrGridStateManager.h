//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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
#include "GrGridCell.h"
#include "GrGridInternal.h"
#include "GrGridWindow.h"

class GrGridCore;
class GrCell;
class GrGridPainter;
class GrItemSelector;
class GrFocuser;
class GrColumnList;
class GrColumnSplitter;
class GrItem;
class IFocusable;
class IDataRow;

enum GrGridState
{
    GrGridState_Normal,
    GrGridState_RootPressing,
    GrGridState_ColumnPressing,
    GrGridState_ColumnResizing,
    GrGridState_ColumnSplitterMoving,
    GrGridState_RowPressing,
    GrGridState_RowResizing,
    GrGridState_GroupPressing,
    GrGridState_GroupHeaderPressing,
    GrGridState_GroupExpandPressing,
    GrGridState_ItemPressing,
    GrGridState_ItemButtonPressing,
    GrGridState_ItemEditing,

    GrGridState_Unknown,
};

class GrStateEventArgs : public GrEventArgs
{
public:
    GrStateEventArgs(GrCell* pCell, GrGridState state, void* userData)
        : m_pCell(pCell), m_state(state), m_userData(userData), m_handled(false)
    {

    }

    GrCell* GetCell() const { return m_pCell; }
    template<typename T>
    T GetCell() const { return dynamic_cast<T>(m_pCell); }
    GrGridState GetNextState() const { return m_state; }
    void* GetUserData() const { return m_userData; }

    void SetNextState(GrGridState state) { m_state = state; }
    void SetUserData(void* userData) { m_userData = userData; }
    void SetCell(GrCell* pCell) { m_pCell = pCell; }
    bool GetHandled() const { return m_handled; }
    void SetHandled(bool b) { m_handled = b; }

private:
    GrCell* m_pCell;
    GrGridState m_state;
    void* m_userData;
    bool m_handled;
};

class GrStateMouseEventArgs : public GrMouseEventArgs, public GrStateEventArgs
{
public:
    GrStateMouseEventArgs(const GrPoint& location, GrKeys modifierKeys, GrCell* pHitted, const GrPoint& localHit, GrGridState state, void* userData)
        : GrMouseEventArgs(location, modifierKeys), GrStateEventArgs(pHitted, state, userData), m_localHit(localHit)
    {

    }

    GrStateMouseEventArgs(const GrPoint& location, GrKeys modifierKeys, GrCell* pHitted, const GrPoint& localHit, GrGridState state)
        : GrMouseEventArgs(location, modifierKeys), GrStateEventArgs(pHitted, state, 0), m_localHit(localHit)
    {

    }

    GrPoint GetLocalHit() const { return m_localHit; }

private:
    GrPoint m_localHit;
};

class GrStateBase : public GrObject
{
public:
    GrStateBase();
    virtual ~GrStateBase();

    virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);

    virtual void OnBegin(GrStateEventArgs* e);
    virtual void OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect);
    virtual void OnEnd(GrStateEventArgs* e);

    virtual void OnMouseDown(GrStateMouseEventArgs* e);
    virtual void OnMouseMove(GrStateMouseEventArgs* e);
    virtual void OnMouseUp(GrStateMouseEventArgs* e);
    virtual void OnMouseClick(GrStateMouseEventArgs* e);
    virtual void OnMouseDoubleClick(GrStateMouseEventArgs* e);
    virtual void OnMouseWheel(GrMouseEventArgs* e);
    virtual void OnKeyDown(GrKeys key, GrKeys modifierKeys);
    virtual void OnKeyUp(GrKeys key, GrKeys modifierKeys);

    virtual void OnMouseDragBegin(const GrPoint& location);
    virtual void OnMouseDragEnd(bool cancel, const GrHitTest& hitTest);
    virtual void OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest);


    virtual GrGridState GetState() const = 0;
    virtual bool GetDragable() const { return false; }
    virtual GrCursor GetCursor() const { return GrCursor_Default; }

protected:
    virtual void OnGridCoreAttached();
    void InvokeMouseEvent(GrCell* pOldCell, GrCell* pNewCell, GrStateMouseEventArgs* e);

protected:
    class GrItemSelectorInternal* m_pItemSelector;
    class GrFocuserInternal* m_pFocuser;
    GrGridWindow* m_pGridWindow;
};

class GrStateManager : public GrObject
{
    typedef std::vector<GrStateBase*> _States;
public:
    GrStateManager(void);
    ~GrStateManager(void);

    void ChangeDefaultState();
    void ChangeState(GrGridState gridState, GrCell* pCell, void* userData);
    bool SetCursor();

    GrGridState GetHitTest(const GrPoint& location);
    GrGridState GetGridState() const { return m_state->GetState(); }

    void OnMouseDown(const GrPoint& location, GrKeys modifierKeys);
    void OnMouseMove(const GrPoint& location, GrKeys modifierKeys, bool pressed);
    void OnMouseUp(const GrPoint& location, GrKeys modifierKeys);
    void OnMouseClick(const GrPoint& location, GrKeys modifierKeys);
    void OnMouseDoubleClick(const GrPoint& location, GrKeys modifierKeys);
    void OnMouseWheel(const GrPoint& location, GrKeys modifierKeys, int delta);
    void OnKeyDown(GrKeys key, GrKeys modifierKeys);
    void OnKeyUp(GrKeys key, GrKeys modifierKeys);

    void OnPaint(GrGridPainter* pGridPainter);
    void OnClearing();

protected:
    virtual void OnGridCoreAttached();

private:
    void ChangeState(GrStateBase* state, GrCell* pHitted, void* userData);
    GrStateBase* GetHitTest(GrCell* pHitted, const GrPoint& localHitted);
    GrRect ComputeDragRectangle(const GrPoint& location);

private:
    _States m_states;
    GrStateBase* m_state;
    GrStateBase* m_downState;
    GrStateBase* m_defaultState;

    GrPoint m_downLocation;
    GrRect m_dragRectangle;
    bool m_dragging;
    GrGridWindow* m_pGridWindow;

private:
    class Invalidator
    {
    public:
        Invalidator(GrGridCore* pGridCore);
        ~Invalidator();

    private:
        GrGridCore* m_pGridCore;
    };
};

namespace GridStateClass
{
    class Normal : public GrStateBase
    {
    public:
        Normal();

        virtual void OnBegin(GrStateEventArgs* e);
        virtual void OnMouseMove(GrStateMouseEventArgs* e);
        virtual void OnMouseWheel(GrMouseEventArgs* e);
        virtual void OnKeyDown(GrKeys key, GrKeys modifierKeys);

        virtual GrGridState GetState() const { return GrGridState_Normal; }

    protected:
        virtual void OnGridCoreAttached();

    private:
        void gridCore_Cleared(GrObject* pSender, GrEventArgs* e);

    private:
        GrCell* m_pCell;
    };

    class RootPressing : public GrStateBase
    {
    public:
        RootPressing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);
        virtual void OnBegin(GrStateEventArgs* e);
        virtual void OnEnd(GrStateEventArgs* e);

        virtual GrGridState GetState() const { return GrGridState_RootPressing; }
    };

    class ColumnPressing : public GrStateBase
    {
    public:
        ColumnPressing();
        virtual ~ColumnPressing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);

        virtual void OnMouseDown(GrStateMouseEventArgs* e);
        virtual void OnMouseUp(GrStateMouseEventArgs* e);
        virtual void OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect);

        virtual void OnMouseDragBegin(const GrPoint& location);
        virtual void OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest);
        virtual void OnMouseDragEnd(bool cancel, const GrHitTest& hitTest);

    public:
        virtual GrGridState GetState() const { return GrGridState_ColumnPressing; }
        virtual bool GetDragable() const { return true; }

        virtual GrCursor GetCursor() const { return m_cursor; }

    protected:
        virtual void OnGridCoreAttached();

    private:
        void timer_Elapsed(GrObject* pSender, GrElapsedEventArgs* e);
        bool GetSortable(const GrPoint& location);

    private:
        GrColumnList* m_pColumnList;
        GrColumn* m_pColumn;
        GrColumn* m_pOldDataColumn;
        bool m_handled;
        GrPoint m_locationStart;
        GrCursor m_cursor;

    private:
        enum TargetType
        {
            TargetType_Unknown,

            TargetType_Frozen,
            TargetType_Unfrozen,
            TargetType_GroupList,
        } m_targetType;

        GrCell* m_targetCell;

        class GrSelectionTimer2 : public GrSelectionTimer
        {
        public:
            GrSelectionTimer2();

        protected:
            virtual GrRect GetInsideRectangle() const;
            virtual GrRect GetOutsideRectangle() const;
        }* m_pTimer;
    };

    class ColumnResizing : public GrStateBase
    {
    public:
        ColumnResizing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);

        virtual void OnMouseDown(GrStateMouseEventArgs* e);
        virtual void OnMouseDoubleClick(GrStateMouseEventArgs* e);
        virtual void OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect);

        virtual void OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest);
        virtual void OnMouseDragEnd(bool cancel, const GrHitTest& hitTest);

    private:
        GrColumn* GetResizingColumn(GrColumn* pColumn, const GrPoint& localLocation);

    public:
        virtual GrGridState GetState() const { return GrGridState_ColumnResizing; } 
        virtual bool GetDragable() const { return true; }
        virtual GrCursor GetCursor() const { return GrCursor_VSplit; }

    private:
        GrColumn* m_pColumn;
        int m_resizingStart;
        int m_resizingLocation;
        int m_resizingMin;
        int m_resizingMax;

        int m_downX;
    };

    class ColumnSplitterMoving : public GrStateBase
    {
    public:
        ColumnSplitterMoving();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);

        virtual void OnBegin(GrStateEventArgs* e);
        virtual void OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect);

        virtual void OnMouseDragBegin(const GrPoint& location);
        virtual void OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest);
        virtual void OnMouseDragEnd(bool cancel, const GrHitTest& hitTest);

    public:
        virtual GrGridState GetState () const { return GrGridState_ColumnSplitterMoving; } 
        virtual bool GetDragable() const { return true; }
        virtual GrCursor GetCursor() const { return GrCursor_SizeWE; }

    private:
        GrColumnSplitter* m_pColumnSplitter;

        int m_location;
        int m_freezableIndex;
    };

    class ItemPressing : public GrStateBase
    {
    public:
        ItemPressing();
        virtual ~ItemPressing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);
        virtual void OnMouseDown(GrStateMouseEventArgs* e);
        virtual void OnMouseClick(GrStateMouseEventArgs* e);
        virtual void OnMouseDoubleClick(GrStateMouseEventArgs* e);
        virtual void OnMouseUp(GrStateMouseEventArgs* e);

        virtual void OnMouseDragBegin(const GrPoint& location);
        virtual void OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest);
        virtual void OnMouseDragEnd(bool cancel, const GrHitTest& hitTest);

    public:
        virtual GrGridState GetState() const { return GrGridState_ItemPressing; }
        virtual bool GetDragable() const { return true; }

    protected:
        virtual void OnGridCoreAttached();

    private:
        void timer_Elapsed(GrObject* pSender, GrElapsedEventArgs* e);

    private:
        GrItem* m_pItem;
        GrSelectionTimer* m_pTimer;
        GrPoint m_location;

        class FindSelection
        {
        public:
            FindSelection(GrGridCore* pGridCore, const GrPoint& location);

        public:
            IFocusable* GetFocus() const { return m_pFocus; }
            GrIndexRange GetColumnRange() const { return GrIndexRange(m_columnBegin, m_columnEnd); }
            GrIndexRange GetRowRange() const { return GrIndexRange(m_rowBegin, m_rowEnd); } 

        private:
            IDataRow* GetFocusRow(uint begin, uint end);
            GrColumn* GetFocusColumn(uint begin, uint end);

        private:
            IFocusable* m_pFocus;
            uint m_columnBegin;
            uint m_columnEnd;
            uint m_rowBegin;
            uint m_rowEnd;

            GrItemSelector* m_pItemSelector;
            GrFocuser* m_pFocuser;
            GrGridCore* m_pGridCore;
        };
    };

    class ItemEditing : public GrStateBase
    {
        enum GrEditingResult
        {
            GrEditingResult_None,
            GrEditingResult_OK,
            GrEditingResult_Cancel,
        };
    public:
        ItemEditing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);
        virtual void OnMouseDown(GrStateMouseEventArgs* e);
        virtual void OnMouseMove(GrStateMouseEventArgs* e);
        virtual void OnMouseUp(GrStateMouseEventArgs* e);
        virtual void OnBegin(GrStateEventArgs* e);
        virtual void OnEnd(GrStateEventArgs* e);

    public:
        virtual GrGridState GetState() const {return GrGridState_ItemEditing; } 

    private:
        GrGridState OnBegin(GrEditingReason reason);

    private:
        void LeaveEdit(bool update);
        void LeaveEdit();

    private:
        GrColumn* m_pColumn;
        GrItem* m_pItem;

        GrEditingResult m_result;
    };

    class ItemButtonPressing : public GrStateBase
    {
    public:
        ItemButtonPressing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);
        virtual void OnMouseDown(GrStateMouseEventArgs* e);
        virtual void OnMouseUp(GrStateMouseEventArgs* e);

    public:
        virtual GrGridState GetState() const
        {
            return GrGridState_ItemButtonPressing;
        }

    private:
        GrItem* m_pItem;
        GrPoint m_mouseDownPoint;
    };

    class GroupExpandPressing : public GrStateBase
    {
    public:
        GroupExpandPressing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);
        virtual void OnBegin(GrStateEventArgs* e);
        virtual void OnMouseUp(GrStateMouseEventArgs* e);

    public:
        virtual GrGridState GetState() const
        {
            return GrGridState_GroupExpandPressing;
        }

    private:
        GrGroupRow* m_pGroupRow;
    };

    class GroupCellPressing : public GrStateBase
    {
    public:
        GroupCellPressing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);
        virtual void OnMouseDown(GrStateMouseEventArgs* e);
        virtual void OnMouseUp(GrStateMouseEventArgs* e);
        virtual void OnMouseDoubleClick(GrStateMouseEventArgs* e);

    public:
        virtual GrGridState GetState() const
        {
            return GrGridState_GroupHeaderPressing;
        }
    private: // variable;
        GrGroupHeader* m_pGroupCell;
    };

    class GroupInfoPressing : public GrStateBase
    {
    public:
        GroupInfoPressing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);
        virtual void OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect);

        virtual void OnMouseDown(GrStateMouseEventArgs* e);
        virtual void OnMouseUp(GrStateMouseEventArgs* e);

        virtual void OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest);
        virtual void OnMouseDragEnd(bool cancel, const GrHitTest& hitTest);

        virtual void OnEnd(GrStateEventArgs* e);

    public:
        virtual GrGridState GetState() const { return GrGridState_GroupPressing; }
        virtual bool GetDragable() const { return true; }
        virtual GrCursor GetCursor() const { return m_cursor; }

    private:
        GrGroup* m_pGroup;
        GrPoint m_location;
        uint m_where;
        GrCursor m_cursor;

        enum TargetType
        {
            TargetType_Group,
            TargetType_Remove,
            TargetType_Unknown,
        } m_targetType;
    };

    class RowPressing : public GrStateBase
    {
    public:
        RowPressing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);
        virtual void OnBegin(GrStateEventArgs* e);
        virtual void OnEnd(GrStateEventArgs* e);

    public:
        virtual GrGridState GetState() const { return GrGridState_RowPressing; }

    private:
        GrRow* m_pRow;
    };

    class RowResizing : public GrStateBase
    {
    public:
        RowResizing();

        virtual bool GetHitTest(GrCell* pHitted, const GrPoint& localLocation);

        virtual void OnMouseDown(GrStateMouseEventArgs* e);
        virtual void OnMouseDoubleClick(GrStateMouseEventArgs* e);
        virtual void OnPaintAdornments(GrGridPainter* g, const GrRect& displayRect);

        virtual void OnMouseDragMove(const GrPoint& location, const GrHitTest& hitTest);
        virtual void OnMouseDragEnd(bool cancel, const GrHitTest& hitTest);

    private:
        GrRow* GetResizingRow(GrRow* pRow, const GrPoint& localLocation);

    public:
        virtual GrGridState GetState() const { return GrGridState_RowResizing; }
        virtual bool GetDragable() const { return true; }
        virtual GrCursor GetCursor() const { return GrCursor_HSplit; }

    private:
        GrRow* m_pRow;
        int m_resizingStart;
        int m_resizingLocation;
        int m_resizingMin;
        int m_resizingMax;

        int m_downY;
    };
} /*namespace GridStateClass*/