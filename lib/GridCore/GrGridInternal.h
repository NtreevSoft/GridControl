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


#pragma once
#include "GrGridType.h"
#include "GrGridCell.h"
#include "GrGridWindow.h"
#include "GrGridCoreComponents.h"

class GrFont;

struct GrWordDesc
{
    int    pos;
    int    length;
    int    width;
    int    validWidth;
};

typedef std::vector<GrWordDesc>        WordList;

class GrTextUtil
{
public:
    static void         SingleLine(GrLineDesc* pLine, const std::wstring& cellText, const GrFont* pFont);
    static void         MultiLine(_TextLines* pLines, const std::wstring& cellText, int cellWidth, const GrFont* pFont, bool wordWrap);

private:
    static void         WordWrap(WordList* pWordList, const std::wstring& cellText, const GrFont* pFont, int cellWidth);
    static void         DoMultiline(_TextLines* pLines, const std::wstring& cellText, const GrFont* pFont);
    static void         DoMultilineWordWrap(_TextLines* pLines, const std::wstring& cellText, int cellWidth, const GrFont* pFont);

    static WordList     m_sWordList;
};

class GrSelectionTimer : public GrObject
{
    enum GrScrollSpeed
    {
        GrScrollSpeed_Normal,
        GrScrollSpeed_Fast,
        GrScrollSpeed_Fastest,
    };

    typedef GrEvent<GrElapsedEventArgs, GrSelectionTimer> _GrElpasedEvent;

public:
    GrSelectionTimer();
    ~GrSelectionTimer();

    bool                DoScroll();

    void                Start();
    void                Stop();
    void                SetInterval(time_t interval);

    _GrElpasedEvent     Elapsed;

public:
    void                SetMouseLocation(GrPoint location);

    bool                GetHScrollEnabled() const;
    void                SetHScrollEnabled(bool value);

    bool                GetVScrollEnabled() const;
    void                SetVScrollEnabled(bool value);

    bool                GetScrollAcceleration() const;
    void                SetScrollAcceleration(bool value);

    bool                GetIntervalAcceleration() const;
    void                SetIntervalAcceleration(bool value);

    bool                CanHScroll() const;
    bool                CanVScroll() const;

    GrScroll*           GetHorzScroll() const;
    GrScroll*           GetVertScroll() const;

protected:
    virtual GrRect      GetInsideRectangle() const;
    virtual GrRect      GetOutsideRectangle() const;

    virtual void        OnGridCoreAttached();

private:
    int                 ComputeSpeed(int pos, int length) const;
    void                timer_Elapsed(GrObject* pSender, GrElapsedEventArgs* e);

private:
    bool                m_horzEnable;
    bool                m_vertEnable;

    bool                m_scrollAcceleration;
    bool                m_intervalAcceleration;

    int                 m_horzScroll;
    int                 m_vertScroll;

    GrTimer*            m_pTimer;
    GrScroll*           m_pHorzScroll;
    GrScroll*           m_pVertScroll;
};

class GrFocuserInternal : public GrFocuser
{
public:
    GrFocuserInternal();

    void                SetFocusing(IFocusable* pFocusable);

protected:
    virtual void        OnGridCoreAttached();
    virtual IFocusable* GetFocusing() const;

    virtual void        OnFocusChanging(GrFocusChangeArgs* e);

private:
    void                itemSelector_SelectingEnd(GrObject* pSender, GrEventArgs* e);
    void                gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
    void                gridCore_Created(GrObject* pSender, GrEventArgs* e);

private:
    IFocusable*         m_pFocusing;
};

class GrItemSelectorInternal : public GrItemSelector
{
    typedef GrEvent<GrEventArgs, GrItemSelectorInternal>    _GrEvent;
public:
    GrItemSelectorInternal();

    void                BeginSelecting();
    void                Selecting(GrIndexRange visibleColumnIndex, GrIndexRange visibleRowIndex);
    void                EndSelecting(GrSelectionType selectionType);
    bool                IsSelecting() const;
    bool                IsSelecting(const GrColumn* pColumn) const;
    bool                IsSelecting(const GrDataRow* pDataRow) const;

    void                SetColumnSelectingRange(GrIndexRange range);
    void                SetRowSelectingRange(GrIndexRange range);
    GrRect              GetInvalidateRect(GrIndexRange visibleColumnRange, GrIndexRange visibleRowRange) const;

    void                Select(IFocusable* pFocusable);
    void                Selecting(IFocusable* pFocusable);

public:
    _GrEvent            SelectingBegin;
    _GrEvent            SelectingEnd;

protected:
    virtual void        OnSelectingBegin(GrEventArgs* e);
    virtual void        OnSelectingEnd(GrEventArgs* e);

private:
    bool                m_bSelecting;

    GrIndexRange        m_columnSelecting;
    GrIndexRange        m_rowSelecting;
};    