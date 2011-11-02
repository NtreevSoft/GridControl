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
#include "GrGridBase.h"
#include "GrGridType.h"
#include "GrGridEvent.h"

#ifdef _MANAGED
#include <vcclr.h>
#endif

#define INVALID_INDEX		((uint)-1)
#define INSERTION_ROW		((uint)-8)


#define GRCF_HIDE_TEXT				0x00000020L
#define GRCF_TEXT_CLIPPED			0x00000040L
#define GRCF_CELL_CLIPPED			0x00000080L

#define GRCF_SYSTEM_OBJCT			0x01000000L
#define GRCF_TEXT_IS_NULL			0x02000000L

#define UPDATEPRIORITY_DATAROWLIST	1
#define UPDATEPRIORITY_COLUMNLIST	2
#define UPDATEPRIORITY_GROUPINLIST	3


class GrGridCore;
class GrRow;
class GrDataRow;
class GrColumn;
class GrGroupingInfo;
class GrGridRenderer;

class GrColumnList;
class GrDataRowList;
class GrGroupingRow;
class GrColumnSplitter;
class GrFont;


typedef bool (*FuncSortRow)(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData);

enum GrHorzAlign
{
	GrHorzAlign_Left,
	GrHorzAlign_Center,
	GrHorzAlign_Right,

	GrHorzAlign_Count,
};

enum GrVertAlign
{
	GrVertAlign_Top,
	GrVertAlign_Center,
	GrVertAlign_Bottom,

	GrVertAlign_Count,
};

enum GrMouseOverState
{
	GrMouseOverState_None,
	GrMouseOverState_In,
	GrMouseOverState_Control,

	GrMouseOverState_Count,
};

enum GrItemTypeShow
{
	GrItemTypeShow_FocusedOnly,
	GrItemTypeShow_SelectedOnly,
	GrItemTypeShow_Always,

	GrItemTypeShow_Count,
};

enum GrItemType
{
	GrItemType_Control,
	GrItemType_DropDown,
	GrItemType_Modal,

	GrItemType_Count,
};

class GrSortFunc
{
public:
	static bool SortRowsUp(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData);
	static bool SortRowsDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData);
	static bool SortRowsNone(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData);
	static bool SortDataRowSortUp(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData);
	static bool SortDataRowSortDown(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData);
	static bool SortDataRowSortNone(GrGridCore* pGridCore, const GrRow* pRow1, const GrRow* pRow2, void* dwUserData);
};

class GrRowEventArgs : public GrObject
{
public:
};

class GrColumnEventArgs : public GrObject
{
public:
	GrColumnEventArgs(GrColumn* pColumn) 
		: m_pColumn(pColumn) 
	{

	}

	GrColumn* GetColumn() const
	{
		return m_pColumn;
	}

private:
	GrColumn* m_pColumn;
};

class GrGroupingEventArgs : public GrEventArgs
{
public:
	GrGroupingEventArgs(GrGroupingInfo* pGroupingInfo) : m_pGroupingInfo(pGroupingInfo) {}

	GrGroupingInfo* m_pGroupingInfo;
};

struct GrLineDesc
{
	int	nTextBegin;
	int	nLength;
	int	nWidth;
	int	x;
	int	y;
};

class GrCell : public GrObject
{
public:
	GrCell();
	virtual ~GrCell();

	virtual int					GetX() const = 0;
	virtual int					GetY() const = 0;
	virtual int					GetWidth() const = 0;
	virtual int					GetHeight() const = 0;

	virtual const GrRow*		GetRow() const = 0;
	virtual	GrRow*				GetRow() = 0;

	virtual GrMouseOverState	HitMouseOverTest(GrPoint pt) const = 0;
	virtual	GrCellType			GetCellType() const = 0;
	virtual bool				GetVisible() const = 0;

	bool						IsIn(GrPoint pt) const;
	bool						GetMouseOvered() const;
	bool						GetMousePressed() const;
	GrMouseOverState			GetMouseOverState() const;

	GrRect						GetClientRect() const;
	GrRect						GetRect() const;
	GrPoint						GetPosition() const;
	GrSize						GetSize() const;
	uint						GetID() const;
	int							GetBottom() const { return GetY() + GetHeight(); }
	int							GetRight() const { return GetX() + GetWidth(); }

	void						SetPadding(GrPadding padding);

	ulong						AddFlag(ulong dwAdd);
	ulong						RemoveFlag(ulong dwRemove);
	bool						HasFlag(ulong dwFlag) const;
	ulong						GetFlag(ulong dwMask = 0xffffffff) const;
	virtual	GrFlag				ToRenderStyle() const;

	void						SetFont(GrFont* pFont);

	// text method
	const wchar_t*				GetText() const;
	void						SetText(const wchar_t* strText);
	virtual const wchar_t*		GetTextLineSource() const;
	uint						GetTextLineCount() const;
	const GrLineDesc*			GetTextLine(uint nIndex) const;
	uint						GetTextLineHeight() const;
	const GrRect*				GetTextBound() const;
	void						AlignText();
	void						ComputeTextBound();
	bool						IsTextVisible() const;
	bool						IsTextClipped() const;

	void						RenderText(GrGridRenderer* pRenderer, GrColor foreColor, const GrRect* pDisplayRect, const GrRect* pClipping = NULL) const;

	virtual GrHorzAlign			GetTextHorzAlign() const;
	virtual GrVertAlign			GetTextVertAlign() const;
	virtual bool				GetTextWordWrap() const;
	virtual bool				GetTextMulitiline() const { return false; }

	virtual GrColor				GetRenderingForeColor() const;
	virtual GrColor				GetRenderingBackColor() const;
	virtual GrFont*				GetRenderingFont() const;

	virtual GrColor				GetForeColor(bool inherited = true) const;
	virtual GrColor				GetBackColor(bool inherited = true) const;
	virtual GrPadding			GetPadding(bool inherited = true) const;
	virtual GrFont*				GetFont(bool inherited = true) const;

	void						SetBackColor(GrColor color);
	void						SetForeColor(GrColor color);

	virtual GrRect				GetDisplayRect() const { return GrRect(GetDisplayPosition(), GetDisplaySize());}
	virtual GrSize				GetDisplaySize() const { return GetSize(); }
	virtual GrPoint				GetDisplayPosition() const = 0;
	virtual bool				GetDisplayable() const = 0;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const = 0;

	GrGridCore*					GetGridCore() const { return m_pGridCore; }

protected:
	virtual void				OnTextChanged();
	virtual void				OnTextBoundChanged(const GrRect* pCur, const GrRect* pOld);

	virtual void				OnGridCoreAttached();
	virtual void				OnGridCoreDetached();

	void						SetTextBoundChanged();
	void						SetTextAlignChanged();

	void						Invalidate(bool thisOnly = true);

public:
	void*						Tag;

protected:
	class GrTextUpdater*		m_pTextUpdater;

private:
	std::wstring				m_strText;
	ulong						m_dwFlag;

	std::vector<GrLineDesc>		m_vecTextLine;
	int							m_nTextLineHeight;
	GrRect						m_rtTextBound;

	GrColor						m_backColor;
	GrColor						m_foreColor;
	GrFont*						m_pFont;

	GrPadding					m_padding;
	uint						m_nID;

	bool						m_bTextBound;
	bool						m_bTextAlign;

	static	uint				m_snID;
	friend class GrGridCore;
	friend class GrTextUpdater;
};

class IDataRow;
class IFocusable
{
public:
	virtual IDataRow*			GetIDataRow() const = 0;

	static IFocusable*			Null;
};

class GrItem : public GrCell, public IFocusable
{
public:
	GrItem(GrColumn* pColumn, GrDataRow* pDataRow);

	GrRect						GetControlRect() const;
	GrState						GetControlState() const;
	GrDataRow*					GetDataRow() const;
	GrColumn*					GetColumn() const;
	bool						GetSelected() const;
	bool						GetFocused() const;
	bool						IsItemSelecting() const;

	void						SetReadOnly(bool b);
	void						SetSelected(bool b);
	void						SetFocused(bool b);

	void						LockColor(bool b);

	//bool						Compare(const GrItem* pItem) const;
	bool						ShouldEnsureVisible() const;

	virtual GrMouseOverState	HitMouseOverTest(GrPoint pt) const;

	virtual bool				GetVisible() const;
	virtual bool				GetReadOnly() const;
	virtual GrHorzAlign			GetTextHorzAlign() const;
	virtual GrVertAlign			GetTextVertAlign() const;
	virtual bool				GetTextWordWrap() const;
	virtual bool				GetTextMulitiline() const;

	virtual int					GetX() const;
	virtual int					GetY() const;
	virtual int					GetWidth() const;
	virtual int					GetHeight() const;
	virtual	GrCellType			GetCellType() const { return GrCellType_Item; }

	virtual GrPoint				GetDisplayPosition() const;
	virtual bool				GetDisplayable() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual const GrRow*		GetRow() const;
	virtual	GrRow*				GetRow();

	virtual IDataRow*			GetIDataRow() const;
	virtual IFocusable*			GetNextFocusable() const;
	virtual IFocusable*			GetPrevFocusable() const;

	virtual bool				GetClipped() const;

	virtual GrFlag				ToRenderStyle() const;
	virtual GrFont*				GetRenderingFont() const;
	virtual GrColor				GetRenderingForeColor() const;
	virtual GrColor				GetRenderingBackColor() const;

	virtual GrColor				GetForeColor(bool inherited = true) const;
	virtual GrColor				GetBackColor(bool inherited = true) const;
	virtual GrPadding			GetPadding(bool inherited = true) const;
	virtual GrFont*				GetFont(bool inherited = true) const;

protected:
	virtual void				OnTextChanged();
	virtual void				OnTextBoundChanged(const GrRect* pCur, const GrRect* pOld);

private:
	void						DoSelected(bool b);

#ifdef _MANAGED
public:
	gcroot<System::Object^> ManagedRef;
#endif

private:
	GrDataRow*					m_pDataRow;
	GrColumn*					m_pColumn;

	bool						m_bReadOnly;
	bool						m_bSelected;
	bool						m_bColorLocked;

	friend class GrItemSelector;
};

typedef std::set<GrItem*>		GrItems;
typedef std::vector<GrRow*>		GrRowArray;
typedef std::set<GrRow*>		GrRows;

typedef void (*FuncCustomRender)(GrGridRenderer* pRenderer, const GrItem* pItem, GrRect rtRender, void* dwUserData);
typedef bool (*FuncColumnRender)(GrGridRenderer* pRenderer, const GrColumn* pColumn, GrRect rtRender, void* dwUserData);

typedef bool (*FuncComparer)(GrGridCore* pGridCore, const GrDataRow* pRow1, const GrDataRow* pRow2, const GrColumn* pColumn);

class GrColumn : public GrCell
{
	typedef GrEventBase<GrEventArgs, GrColumn> _GrEvent;
public:
	GrColumn();

	GrColumnList*				GetColumnList() const;

	GrItemType					GetItemType() const;
	GrItemTypeShow				GetItemTypeShow() const;
	GrHorzAlign					GetItemHorzAlign() const;
	GrVertAlign					GetItemVertAlign() const;
	bool						GetItemWordWrap() const;
	bool						GetItemMultiline() const;
	GrColor						GetItemForeColor() const;
	GrColor						GetItemBackColor() const;
	GrPadding					GetItemPadding() const;
	GrFont*						GetItemFont() const;

	int							GetDisplayX() const { return m_nDisplayX; }

	uint						GetDisplayIndex() const;
	uint						GetVisibleIndex() const;
	uint						GetFrozenIndex() const;
	uint						GetUnfrozenIndex() const;
	uint						GetIndex() const;
	uint						GetColumnID() const;

	void						SetItemType(GrItemType type);
	void						SetItemHorzAlign(GrHorzAlign horzAlign);
	void						SetItemVertAlign(GrVertAlign vertAlign);
	void						SetItemWordWrap(bool b);
	void						SetItemMultiline(bool b);
	void						SetItemForeColor(GrColor color);
	void						SetItemBackColor(GrColor color);
	void						SetItemPadding(GrPadding padding);
	void						SetItemFont(GrFont* pFont);
	void						SetItemTextSeparator(std::wstring separator);

	bool						GetMovable() const;
	void						SetMovable(bool b);
	bool						GetResizable() const;
	void						SetResizable(bool b);
	bool						GetFrozen() const;
	void						SetFrozen(bool b);

	bool						GetReadOnly() const;
	void						SetReadOnly(bool b);

	void						SetVisible(bool b);
	void						SetMinWidth(int nMinWidth);
	void						SetMaxWidth(int nMaxWidth);
	int							GetMinWidth() const;
	int							GetMaxWidth() const;

	void						SetSelected(bool b);
	bool						GetSelected() const;
	bool						GetFullSelected() const;
	bool						GetSelecting() const;

	bool						HasFocused() const;

	void						AdjustWidth();
	void						SetFit();

	void						EnableSort(bool bEnable = true);
	bool						CanBeSort() const;

	void						EnableGrouping(bool bEnable = true);
	bool						CanBeGrouped() const;

	void						SetClipped(bool b);
	bool						GetClipped() const;

	bool						ShouldEnsureVisible() const;

	void						SetWidth(int width);

	void						SetTooltip(const wchar_t* strTooltip);
	const wchar_t*				GetTooltip() const;

	int							GetPriority() const;

	bool						GetGrouped() const;
	void						SetGrouped(bool b);
	GrGroupingInfo*				GetGroupingInfo() const;

	bool						GetClippedTextVisible() const;
	void						SetClippedTextVisible(bool b);

	void						SetSortType(GrSort sortType);
	GrSort						GetSortType() const;
	void						SetSortComparer(GrSort sortType, FuncComparer comparer);
	FuncComparer				GetSortComparer(GrSort sortType) const;


	virtual	GrCellType			GetCellType() const { return GrCellType_Column; }
	virtual int					GetX() const;
	virtual int					GetY() const;
	virtual int					GetWidth() const;
	virtual int					GetHeight() const;
	virtual GrMouseOverState	HitMouseOverTest(GrPoint pt) const;

	virtual bool				GetVisible() const;

	virtual GrHorzAlign			GetTextHorzAlign() const;
	virtual GrVertAlign			GetTextVertAlign() const;

	virtual bool				Focusable() const { return false; }

	virtual const wchar_t*		GetTextLineSource() const;

	virtual GrRow*				GetRow();
	virtual const GrRow*		GetRow() const;

	virtual GrFlag				ToRenderStyle() const;
	virtual GrColor				GetRenderingBackColor() const;
	virtual GrPoint				GetDisplayPosition() const;
	virtual bool				GetDisplayable() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;
	void						RenderTooltip(GrGridRenderer* pRenderer, GrPoint ptStart) const;
	void						RenderTextEllipsis(GrGridRenderer* pRenderer, GrColor foreColor, GrRect* pDisplayRect) const;

public:
	_GrEvent					GroupingChanged;	


protected:
	virtual void				OnGridCoreAttached();
	virtual void				OnGridCoreDetached();

	virtual void				OnTextChanged();

private:
	virtual ~GrColumn();

	void						SetPriority(int nPriority);

	void						SetX(int x) { m_nX = x; };
	void						SetDisplayX(int x) { m_nDisplayX = x; }
	void						SetDisplayable(bool b);
	void						SetDisplayIndex(uint nIndex);
	void						SetVisibleIndex(uint nIndex);
	void						SetIndex(uint nIndex);
	void						SetColumnID(uint nIndex);

public:
	bool						m_bCustomItemRender;

	FuncColumnRender			m_fnColumnBackgroundRender;
	FuncColumnRender			m_fnColumnContentsRender;
	void*						m_pColumnRenderData;

#ifdef _MANAGED
public:
	gcroot<System::ComponentModel::IComponent^> ManagedRef;
#endif

private:
	GrColumnList*				m_pColumnList;

	int							m_nSelectedCells;	friend class GrItemSelector;
	int							m_nPriority;
	int							m_nX;
	int							m_nWidth;
	int							m_nDisplayX;

	int							m_nDefaultWidth;
	int							m_nMinWidth;
	int							m_nMaxWidth;

	bool						m_bVisible;
	bool						m_bReadOnly;
	bool						m_bCanBeSorted;
	bool						m_bCanBeGrouped;
	bool						m_bMovable;
	bool						m_bResizable;
	bool						m_bFrozen;
	bool						m_bClippedTextVisible;
	bool						m_bSelected;
	bool						m_bFullSelected;
	bool						m_bFitting;
	bool						m_bGrouped;

	uint						m_nDisplayIndex;
	uint						m_nVisibleIndex;
	uint						m_nSelectedIndex;
	uint						m_nIndex;
	uint						m_nColumnID;

	bool						m_bDisplayable;
	bool						m_bClipped;

	GrItemType					m_itemType;
	GrItemTypeShow				m_itemTypeShow;
	GrHorzAlign					m_itemHorzAlign;
	GrVertAlign					m_itemVertAlign;
	bool						m_itemWordWrap;
	bool						m_itemMultiline;

	GrColor						m_itemBackColor;
	GrColor						m_itemForeColor;
	GrPadding					m_itemPadding;
	GrFont*						m_pItemFont;

	GrGroupingInfo*				m_pGroupingInfo;

	GrSort						m_sortType;
	FuncComparer				m_comparer[GrSort_Count];

	std::wstring				m_strTooltip;
	std::wstring				m_strTextLineSource;

	friend class GrGroupingInfo;
	friend class GrColumnList;
};

typedef std::set<GrColumn*>		GrColumns;


enum GrRowUpdate 
{
	GrRowUpdate_Cell	= 1,
	GrRowUpdate_Row		= 2,
	GrRowUpdate_Both	= 3,
};

struct GrUpdateDesc
{
	int nMin;
	int nMax;
};

class GrRow abstract : public GrCell
{
public:
	GrRow();

	void						SetY(int y);
	virtual int					GetY() const { return m_nY; }

	void						SetHeight(int height);
	virtual int					GetHeight() const;

	void						SetResizable(bool b);
	bool						GetResizable() const;

	void						AdjustHeight();

	void						SetMinHeight(int nMinHeight);
	int							GetMinHeight() const;

	void						SetMaxHeight(int nMaxHeight);
	int							GetMaxHeight() const;

	virtual const GrRow*		GetRow() const { return this; }
	virtual	GrRow*				GetRow() { return this; }

	virtual	GrCellType			GetCellType() const { return GrCellType_Row; }
	virtual int					GetX() const;
	virtual int					GetDisplayX() const;
	virtual int					GetDisplayY() const;
	virtual GrMouseOverState	HitMouseOverTest(GrPoint /*pt*/) const { return GrMouseOverState_In; }

	virtual GrPoint				GetDisplayPosition() const;
	virtual bool				GetDisplayable() const { return true; }
	virtual bool				ShouldEnsureVisible() const { return false; }

	virtual bool				GetVisible() const;

	virtual GrHorzAlign			GetTextHorzAlign() const;
	virtual GrVertAlign			GetTextVertAlign() const;

	virtual GrRowType			GetRowType() const { return GrRowType_Dummy; }

	virtual GrCell*				HitTest(const GrPoint& pt) const;
	virtual GrCell*				HitDisplayTest(const GrPoint& pt) const;

	uint						GetChildCount() const;
	GrRow*						GetChild(uint nIndex) const;
	GrRow*						GetParent() const;
	uint						GetHierarchyLevel() const;

	void						SetFit();
	virtual bool				GetFittable() const { return true; }
	virtual int					GetFitHeight() const;

	virtual void				Render(GrGridRenderer* /*pRenderer*/, const GrRect* /*pClipping*/) const {};

	virtual void				GetVisibleList(GrRowArray* pVisible) const;
	virtual GrRect				GetBound() const { return GetRect(); }
	virtual GrRect				GetDisplayBound() const { return GetDisplayRect(); }
	virtual GrPadding			GetPadding(bool /*inherited*/) const { return GrPadding::Default; }

	virtual void				Sort(GrSort sortType);
	virtual void				Sort(FuncSortRow fnSort, void* dwUserData);

	void						AddChild(GrRow* pRow);
	void						ReserveChild(uint reserve);
	void						ClearChild();

public:
	static int		DefaultHeight;
	static int		DefaultMinHeight;
	static int		DefaultMaxHeight;

protected:
	virtual void				OnHeightChanged() {};
	virtual void				OnYChanged() {};
	virtual void				OnHeightAdjusted() {};

	virtual void				OnFitted();

	int							CellStart() const;

protected:
	bool						m_bVisible;

private:
	GrRowArray					m_vecChilds;
	GrRow*						m_pParent;
	uint						m_nHierarchyLevel;
	int							m_nY;

	bool						m_bResizable;

	int							m_nHeight;
	int							m_nMinHeight;
	int							m_nMaxHeight;

	bool						m_bFitting;

	friend class GrGridCore;
};

class GrUpdatableRow abstract : public GrRow
{
public:
	virtual bool	ShouldUpdate() const;
	virtual void	Update(bool force);
	virtual int		GetUpdatePriority() const;

	virtual bool	ShouldClip(const GrRect* pDisplayRect, uint horizontal, uint vertical) const;
	virtual void	Clip(const GrRect* pDisplayRect, uint horizontal, uint vertical);
	virtual int		GetClipPriority() const;

protected:
	virtual void	OnFitted();
	virtual void	OnHeightChanged();
};

class GrRootRow : public GrRow
{
	typedef std::vector<GrRow*>	_Rows;
	typedef std::vector<GrUpdatableRow*>	_Updatables;
public:
	GrRootRow();

	void						SetVisibleChanged();
	void						SetFitChanged();
	void						SetHeightChanged();

	bool						ShouldUpdate() const;
	void						Update(bool force = false);

	bool						ShouldClip() const;
	void						Clip(const GrRect* pDisplayRect, uint horizontal, uint vertical);
	
	virtual int					GetWidth() const { return m_nWidth; }
	virtual int					GetHeight() const { return m_nHeight; }

	virtual GrSize				GetDisplaySize() const;
	virtual GrPoint				GetDisplayPosition() const;

	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual GrCell*				HitTest(const GrPoint& pt) const;
	virtual GrCell*				HitDisplayTest(const GrPoint& pt) const;

protected:
	virtual void				OnGridCoreAttached();

private:
	void						BuildVisibleList();
	void						AdjustRowHeight();
	void						RepositionVisibleList();

	void						gridCore_Created(GrObject* pSender, GrEventArgs* e);

private:
	_Rows						m_vecVisibleRows;
	_Updatables					m_vecUpdatables;
	_Updatables					m_vecClippables;
	bool						m_bVisibleChanged;
	bool						m_bFitChanged;
	bool						m_bHeightChanged;

	int							m_nWidth;
	int							m_nHeight;

	GrRect						m_rtDisplayBound;

	GrColumnList*				m_pColumnList;
};

class IDataRow abstract : public GrRow
{
public: 
	IDataRow();

	virtual bool				GetDisplayable() const;

	void						SetDisplayable(bool b);

	void						SetDisplayY(int y);
	int							GetDisplayY() const;

	void						SetVisibleIndex(uint nIndex);
	uint						GetVisibleIndex() const;

	void						SetDisplayIndex(uint nIndex);
	uint						GetDisplayIndex() const;

	void						SetClipped(bool b);
	bool						GetClipped() const;

	bool						ShouldEnsureVisible() const;
	bool						HasFocused() const;

	virtual IFocusable*			GetFocusable(GrColumn* pColumn) const = 0;
	virtual uint				GetSelectionGroup() const { return 0; }
	virtual bool				GetFullSelected() const { return false; }
	virtual int					GetWidth() const;

#ifdef _MANAGED
public:
	gcroot<System::Object^> ManagedRef;
#endif

protected:
	virtual void				OnFitted();
	virtual void				OnGridCoreAttached();
	virtual void				OnHeightChanged();

protected:
	GrDataRowList*				m_pDataRowList;

private:
	int							m_nDisplayY;
	bool						m_bDisplayable;
	bool						m_bClipped;
	uint						m_nVisibleIndex;
	uint						m_nDisplayIndex;
};

class GrDataRow	: public IDataRow
{
protected:
	typedef std::vector<GrItem*>	_Items;
public:
	GrDataRow();
	virtual ~GrDataRow();

	GrItem*						GetItem(const GrColumn* pColumn) const;
	GrItem*						GetItem(const GrItem* pOtherItem) const;

	bool						GetReadOnly() const;
	void						SetReadOnly(bool b = true);

	void						SetSelected(bool b);
	bool						GetSelected() const;
	bool						GetSelecting() const;

	uint						GetVisibleDataRowIndex() const;
	uint						GetDataRowIndex() const;
	uint						GetDataRowID() const;

	GrColor						GetItemBackColor() const;
	GrColor						GetItemForeColor() const;
	GrFont*						GetItemFont() const;
	void 						SetItemBackColor(GrColor color);
	void 						SetItemForeColor(GrColor color);
	void						SetItemFont(GrFont* pFont);

	virtual bool				GetFullSelected() const;

	virtual bool				GetVisible() const;
	virtual void				SetVisible(bool b);

	virtual GrRowType			GetRowType() const { return GrRowType_DataRow; }
	virtual GrFlag				ToRenderStyle() const;
	virtual GrColor				GetRenderingBackColor() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual IFocusable*			GetFocusable(GrColumn* pColumn) const;
	virtual int					GetFitHeight() const;

	virtual GrCell*				HitTest(const GrPoint& pt) const;
	virtual GrCell*				HitDisplayTest(const GrPoint& pt) const;

protected:
	virtual void				OnGridCoreAttached();
	virtual void				OnGridCoreDetached();
	virtual void				OnHeightAdjusted();

private:
	void						SetVisibleDataRowIndex(uint nIndex);
	void						SetDataRowIndex(uint nIndex);
	void						SetDataRowID(uint nIndex);
	void						AddItem(GrColumn* pColumn);
	void						Reserve(uint count);
	void						ClearItem();
	void						SetFocusedItem(bool b);		// called by GrGridCore
	

private:
	_Items						m_vecItems;
	bool						m_bReadOnly;

	GrColor						m_itemBackColor;
	GrColor						m_itemForeColor;
	GrFont*						m_pItemFont;

	uint						m_nVisibleDataRowIndex;
	uint						m_nDataRowIndex;
	uint						m_nDataRowID;

private:	// friend variables;
	uint						m_nSelectedCells;

	friend class GrItemSelector;
	friend class GrGridCore;
	friend class GrDataRowList;
};

typedef std::set<GrDataRow*>		GrDataRows;

class GrInsertionRow : public GrDataRow
{
public:
	GrInsertionRow();

	virtual GrRowType			GetRowType() const { return GrRowType_InsertionRow; }
	virtual void				SetVisible(bool b);

	virtual uint				GetSelectionGroup() const { return 1; }

protected:
	virtual void				OnGridCoreAttached();
	virtual void				OnHeightChanged();
};

class GrDataRowList	: public GrUpdatableRow
{
	struct GrCache
	{
		int		nHeight;
		bool	bExpanded;
	};

	typedef std::vector<GrColumn*>			_Columns;
	typedef std::vector<GrDataRow*>			_DataRows;
	typedef std::vector<IDataRow*>			_IDataRows;
	typedef std::map<int, IDataRow*>		_MapDataRowPos;
	typedef std::vector<GrGroupingRow*>		_GroupingRows;
	typedef std::map<std::wstring, GrCache>	_MapCaches;

	typedef GrEventBase<GrEventArgs, GrDataRowList> _GrEvent;
public:
	GrDataRowList();
	virtual ~GrDataRowList();

	void						Reserve(uint reserve);

	void						AdjustRowHeight();
	int							GetRowWidth() const;
	void						SetRowWidth(int nWidth);

	uint						GetVisibleRowCount() const;
	IDataRow*					GetVisibleRow(uint nIndex) const;

	uint						GetVisibleDataRowCount() const;
	GrDataRow*					GetVisibleDataRow(uint nIndex) const;

	GrDataRow*					NewDataRowFromInsertion();

	void						AddDataRow(GrDataRow* pDataRow);
	void						RemoveDataRow(GrDataRow* pDataRow);
	void						InsertDataRow(GrDataRow* pDataRow, uint nIndex);
	uint						GetDataRowCount() const;
	GrDataRow*					GetDataRow(uint nIndex) const;
	void						ClearDataRow();

	uint						GetDisplayableRowCount() const;
	IDataRow*					GetDisplayableRow(uint nIndex) const;
	int							GetDisplayableBottom() const;

	uint						ClipFrom(uint nVisibleFrom) const;
	uint						ClipFrom(const GrRect* pDisplayRect, uint nVisibleFrom) const;
	uint						ClipTo(uint nVisibleTo) const;
	uint						ClipTo(const GrRect* pDisplayRect, uint nVisibleTo) const;
	void						HitTest(int y1, int y2, GrDataRows* pTested) const;
	bool						HitTest(int y1, int y2, GrIndexRange* pRange) const;
	IDataRow*					HitTest(int y) const;

	void						Sort(GrColumn* pColumn);

	int							DisplayToGlobal(int y) const;

	void						SetZeroBasedRowIndex(bool b);
	bool						GetZeroBasedRowIndex() const;

	bool						GetRowNumberVisible() const;
	void						SetRowNumberVisible(bool b);

	void						SetFitChanged();
	void						SetVisibleChanged();
	void						SetHeightChanged();
	void						SetListChanged();

	_GrEvent					VisibleChanged;

	virtual GrRect				GetBound() const;
	virtual GrRect				GetDisplayBound() const;
	virtual bool				ShouldClip(const GrRect* pDisplayRect, uint horizontal, uint vertical) const;
	virtual void				Clip(const GrRect* pDisplayRect, uint horizontal, uint vertical);
	virtual int					GetClipPriority() const { return 1; }
	virtual bool				ShouldUpdate() const;
	virtual void				Update(bool force = false);
	virtual int					GetUpdatePriority() const { return UPDATEPRIORITY_DATAROWLIST; }

	virtual GrRowType			GetRowType() const { return GrRowType_DataRowList; }
	virtual GrCell*				HitTest(const GrPoint& pt) const;
	virtual GrCell*				HitDisplayTest(const GrPoint& pt) const;
	virtual int					GetWidth() const { return 0; }
	virtual int					GetHeight() const { return 0; }
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

protected:
	virtual void				OnGridCoreAttached();
	virtual void				OnYChanged();

private:
	GrGroupingRow*				CreateGroupingRow(GrRow* pParent, GrColumn* pColumn, const wchar_t* strText);
	void						BuildGrouping(GrRow* pParent, uint nGroupingLevel);
	void						BuildChildRowList();
	void						BuildVisibleRowList();
	void						RepositionVisibleRowList();
	void						BuildCache();
	void						DeleteObjects();

	void						groupingList_Changed(GrObject* pSender, GrEventArgs* e);
	void						groupingList_Expanded(GrObject* pSender, GrGroupingEventArgs* e);
	void						groupingList_SortChanged(GrObject* pSender, GrGroupingEventArgs* e);
	void						gridCore_Created(GrObject* pSender, GrEventArgs* e);
	void						gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
	void						columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
	void						columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
	void						focuser_FocusedChanged(GrObject* pSender, GrEventArgs* e);

private:
	int							m_nRowWidth;

	_DataRows					m_vecDataRowsRemoved;
	_DataRows					m_vecDataRows;
	_DataRows					m_vecVisibleDataRows;
	_MapDataRowPos				m_mapRowByPosition;
	_IDataRows					m_vecVisibleRows;
	_IDataRows					m_vecDisplayableRows;
	_GroupingRows				m_vecGroupingRows;
	GrInsertionRow*				m_pInsertionRow;

	_MapCaches					m_mapCache;
	_Columns					m_vecColumns;

	uint						m_nUsedGroupingRow;
	uint						m_nGroupingCount;
	uint						m_nDataRowID;

	int							m_nDisplayableBottom;
	int							m_nVisibleBottom;
	int							m_nVisibleHeight;

	GrIndexRange				m_selectingRange;
	GrDataRow*					m_pFocusedDataRow;

	bool						m_bVisibleRowNumber;
	bool						m_bZeroBasedRowIndex;

	int							m_nClippedHeight;
	uint						m_nClippedIndex;

	// flags
	bool						m_bListChanged;
	bool						m_bVisibleChanged;
	bool						m_bFitChanged;
	bool						m_bHeightChanged;
};

class GrColumnList : public GrUpdatableRow
{
	typedef std::vector<GrColumn*>		_Columns;
	typedef std::map<int, GrColumn*>	_MapColumnPos;
	typedef GrEventBase<GrColumnEventArgs, GrColumnList> _GrColumnEvent;
public:
	GrColumnList();
	virtual ~GrColumnList();
	
	void						Reserve(uint reserve);

	void						AddColumn(GrColumn* pColumn);
	void						RemoveColumn(GrColumn* pColumn);
	void						InsertColumn(GrColumn* pColumn, uint nIndex);

	uint						GetColumnCount() const;
	GrColumn*					GetColumn(uint index) const;

	void						AdjustColumnWidth();

	uint						GetVisibleColumnCount() const;
	GrColumn*					GetVisibleColumn(uint index) const;
	int							GetVisibleRight() const;

	uint						GetFrozenColumnCount() const;
	GrColumn*					GetFrozenColumn(uint nIndex) const;

	uint						GetUnfrozenColumnCount() const;
	GrColumn*					GetUnfrozenColumn(uint nIndex) const;
	int							GetUnfrozenX() const { return m_nUnfrozenX; }

	GrColumnSplitter*			GetColumnSplitter() const { return m_pColumnSplitter; }

	uint						GetDisplayableColumnCount() const;
	GrColumn*					GetDisplayableColumn(uint index) const;
	int							GetDisplayableRight() const;

	GrColumn*					GetFirstSortColumn() const;

	int							DisplayToGlobal(int x) const;

	bool						MoveToFrozen(GrColumn* pColumn, GrColumn* pWhere);
	bool						MoveToUnfrozen(GrColumn* pColumn, GrColumn* pWhere);

	uint						ClipFrom(uint nVisibleFrom) const;
	uint						ClipFrom(const GrRect* pDisplayRect, uint nVisibleFrom) const;
	uint						ClipTo(uint nVisibleTo) const;
	uint						ClipTo(const GrRect* pDisplayRect, uint nVisibleTo) const;
	void						HitTest(GrRect rt, GrColumns* pTested) const;
	bool						HitTest(int x1, int x2, GrIndexRange* pRange) const;
	GrColumn*					HitTest(int x) const;

	void						SetFitChanged();
	void						SetVisibleChanged();

	virtual bool				ShouldClip(const GrRect* pDisplayRect, uint horizontal, uint vertical) const;
	virtual void				Clip(const GrRect* pDisplayRect, uint horizontal, uint vertical);
	virtual int					GetClipPriority() const { return 0; }
	virtual bool				ShouldUpdate() const;
	virtual void				Update(bool force = false);
	virtual int					GetUpdatePriority() const { return UPDATEPRIORITY_COLUMNLIST; }

	virtual GrCell*				HitTest(const GrPoint& pt) const;
	virtual GrCell*				HitDisplayTest(const GrPoint& pt) const;

	virtual bool				GetDisplayable() const { return true; }
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;
	void						RenderSplitter(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual GrCellType			GetCellType() const { return GrCellType_Root; }
	virtual int					GetWidth() const;

	virtual bool				GetVisible() const { return true; }
	virtual bool				GetReadOnly() const { return true; }
	virtual GrItemType			GetItemType() const { return GrItemType_Control; }
	virtual GrRowType			GetRowType() const { return GrRowType_ColumnList; }

	virtual bool				GetFittable() const { return false; }
	virtual GrRect				GetBound() const;
	virtual GrRect				GetDisplayBound() const;

	void						NotifySortTypeChanged(GrColumn* pColumn);
	void 						NotifyWidthChanged(GrColumn* pColumn);
	void 						NotifyFrozenChanged(GrColumn* pColumn);
	void 						NotifyWordwrapChanged(GrColumn* pColumn);
	void 						NotifyMultilineChanged(GrColumn* pColumn);
	void 						NotifyHorzAlignChanged(GrColumn* pColumn);
	void 						NotifyVertAlignChanged(GrColumn* pColumn);
	void 						NotifyPaddingChanged(GrColumn* pColumn);

	_GrColumnEvent				ColumnInserted;
	_GrColumnEvent				ColumnRemoved;
	_GrColumnEvent				ColumnSortTypeChanged;
	_GrColumnEvent				ColumnFrozenChanged;
	_GrColumnEvent				ColumnWidthChanged;
	_GrColumnEvent				ColumnWordwrapChanged;
	_GrColumnEvent				ColumnMultilineChanged;
	_GrColumnEvent				ColumnHorzAlignChanged;
	_GrColumnEvent				ColumnVertAlignChanged;
	_GrColumnEvent				ColumnPaddingChanged;
	_GrColumnEvent				ColumnGroupingChanged;

protected:
	virtual void				OnGridCoreAttached();

	virtual void				OnColumnInserted(GrColumnEventArgs* e);
	virtual void				OnColumnRemoved(GrColumnEventArgs* e);

	virtual void 				OnColumnSortTypeChanged(GrColumnEventArgs* e);
	virtual void 				OnColumnFrozenChanged(GrColumnEventArgs* e);
	virtual void 				OnColumnWidthChanged(GrColumnEventArgs* e);
	virtual void 				OnColumnWordwrapChanged(GrColumnEventArgs* e);
	virtual void 				OnColumnMultilineChanged(GrColumnEventArgs* e);
	virtual void 				OnColumnHorzAlignChanged(GrColumnEventArgs* e);
	virtual void 				OnColumnVertAlignChanged(GrColumnEventArgs* e);
	virtual void 				OnColumnPaddingChanged(GrColumnEventArgs* e);
	virtual void				OnColumnGroupingChanged(GrColumnEventArgs* e);

	void						BuildVisibleColumnList();
	void						RepositionColumnList();

private:
	void						groupingList_Changed(GrObject* pSender, GrEventArgs* e);
	void						gridCore_FocusChanged(GrObject* pSender, GrEventArgs* e);
	void						gridCore_Cleared(GrObject* pSender, GrEventArgs* e);

	void						column_GroupingChanged(GrObject* pSender, GrEventArgs* e);

private:
	bool						IsColumnSelecting(GrColumn* pColumn) const;

private:
	_Columns					m_vecColumns;
	_Columns					m_vecColumnsRemoved;
	_MapColumnPos				m_mapColumnByPosition;
	_Columns					m_vecVisibleColumns;
	_Columns					m_vecDisplayableColumns;
	int							m_nColumnID;

	int							m_nUnfrozenX;			// global좌표 내에서 첫번째 ScrollbleColumn의 위치
	int							m_nDisplayableRight;
	int							m_nVisibleRight;
	int							m_nBoundWidth;

	uint						m_nFrozenCount;
	uint						m_nGroupingCount;

	GrIndexRange				m_selectingRange;
	GrColumn*					m_pSortColumn;
	GrColumnSplitter*			m_pColumnSplitter;

	int							m_nClippedWidth;
	uint						m_nClippedIndex;

	// flag
	bool						m_bVisibleChanged;
	bool						m_bFitChanged;
	bool						m_bWidthChanged;
};

class GrColumnSplitter : public GrCell
{
public:
	GrColumnSplitter(GrColumnList* pColumnList);

	virtual	GrCellType			GetCellType() const { return GrCellType_Splitter; }
	virtual int					GetX() const;
	virtual int					GetY() const;
	virtual int					GetWidth() const;
	virtual int					GetHeight() const;
	virtual bool				GetVisible() const;

	virtual GrHorzAlign			GetTextHorzAlign() const { return GrHorzAlign_Left; }
	virtual GrVertAlign			GetTextVertAlign() const { return GrVertAlign_Top; }

	virtual GrMouseOverState	HitMouseOverTest(GrPoint pt) const;
	virtual GrPoint				GetDisplayPosition() const;
	virtual bool				GetDisplayable() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual const GrRow*		GetRow() const	{ return m_pColumnList; }
	virtual	GrRow*				GetRow()		{ return m_pColumnList; }

	void						SetVisible(bool b);

	GrColumnList*				GetColumnList() const { return m_pColumnList; }

protected:

private:
	GrColumnList*				m_pColumnList;
	int							m_nX;
	int							m_nDisplayX;
	bool						m_bVisible;
	friend class GrColumnList;
};

class GrRowSplitter : public GrRow
{
public:
	GrRowSplitter();

	virtual GrRowType			GetRowType() const { return GrRowType_Splitter; }
	virtual int					GetWidth() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;
	virtual bool				GetFittable() const { return false; }

	virtual bool				GetVisible() const;
};

class GrGroupingCell : public GrCell, public IFocusable
{
public:
	GrGroupingCell(GrGroupingRow* pRow);

	virtual	GrCellType			GetCellType() const { return GrCellType_Grouping; }
	virtual int					GetX() const;
	virtual int					GetY() const;
	virtual int					GetWidth() const;
	virtual int					GetHeight() const;
	virtual bool				GetVisible() const { return true; }

	virtual GrHorzAlign			GetTextHorzAlign() const;
	virtual GrVertAlign			GetTextVertAlign() const;

	virtual GrMouseOverState	HitMouseOverTest(GrPoint pt) const;
	virtual GrPoint				GetDisplayPosition() const;
	virtual bool				GetDisplayable() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual const GrRow*		GetRow() const;
	virtual	GrRow*				GetRow();

	virtual GrFlag				ToRenderStyle() const;
	virtual bool				Focusable() const { return true; }

	virtual IDataRow*			GetIDataRow() const;
	virtual IFocusable*			GetNextFocusable() const;
	virtual IFocusable*			GetPrevFocusable() const;

private:
	GrGroupingRow*				m_pRow;
};

class GrGroupingRow : public IDataRow
{
public:
	GrGroupingRow();
	virtual ~GrGroupingRow();

	GrColumn*					GetColumn() const;

	void						Expand(bool bExpand = true);
	bool						IsExpanded() const;

	GrGroupingCell*				GetLabel() const;
	uint						GetGroupingLevel() const { return m_nGroupingLevel; }

	virtual GrRowType			GetRowType() const { return GrRowType_GroupingRow; }
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual bool				GetVisible() const;
	virtual IFocusable*			GetFocusable(GrColumn* pColumn) const;

	virtual GrCell*				HitTest(const GrPoint& pt) const;
	virtual GrCell*				HitDisplayTest(const GrPoint& pt) const;

protected:
	virtual void				OnHeightChanged();
	virtual void				OnUpdatePositionCell(int x, GrRect* pBound);
	virtual void				OnGridCoreAttached();

private:
	void						SetRefColumn(GrColumn* pColumn);		// called by GrDataRowList;
	void						ProcessAfterGrouping();								// called by GrDataRowList;
	friend class GrDataRowList;

private:
	bool						m_bExpanded;
	uint						m_nGroupingLevel;

	GrColumn*					m_pColumn;
	std::wstring				m_strKey;
private:
	GrGroupingCell*				m_pLabel;
};

class GrCaption : public GrUpdatableRow
{
public:
	GrCaption();

	virtual GrHorzAlign			GetTextHorzAlign() const;
	virtual GrVertAlign			GetTextVertAlign() const;

	void						SetTextHorzAlign(GrHorzAlign align);
	void						SetTextVertAlign(GrVertAlign align);

	virtual GrRowType			GetRowType() const { return GrRowType_Caption; }
	virtual int					GetWidth() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual bool				GetVisible() const;
	void						SetVisible(bool b);

protected:
	virtual void				OnTextChanged();

private:
	GrHorzAlign					m_horzAlign;
	GrVertAlign					m_vertAlign;

	bool						m_bVisible;

#ifdef _MANAGED
public:
	gcroot<System::Object^>		ManagedRef;
#endif
};

class GrGroupingList : public GrUpdatableRow
{
	typedef std::vector<GrGroupingInfo*>				_Groupings;
	typedef GrEventBase<GrEventArgs, GrGroupingList>			_GrEvent;
	typedef GrEventBase<GrGroupingEventArgs, GrGroupingList>	_GrGroupingEvent;
public:
	GrGroupingList();
	virtual ~GrGroupingList();

	uint						GetGroupingCount() const;
	GrGroupingInfo*				GetGrouping(uint nLevel) const;

	void						ExpandGrouping(uint nLevel, bool bExpand);
	void						SetGroupingSortState(uint nLevel, GrSort sortType);

	void						SetVisible(bool b);

	bool						CanBeGrouped() const;
	void						EnableGrouping(bool b);

	_GrEvent					Changed;
	_GrGroupingEvent			Expanded;
	_GrGroupingEvent			SortChanged;

	void						NotifyExpanded(GrGroupingInfo* pGroupingInfo);
	void						NotifySortChanged(GrGroupingInfo* pGroupingInfo);

	virtual bool				ShouldUpdate() const;
	virtual void				Update(bool force);
	virtual int					GetUpdatePriority() const { return UPDATEPRIORITY_GROUPINLIST; }

	virtual GrRowType			GetRowType() const { return GrRowType_GroupingList; }
	virtual int					GetWidth() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual GrHorzAlign			GetTextHorzAlign() const { return GrHorzAlign_Left; }
	virtual bool				GetTextMultiline() const { return false; }

	virtual bool				GetVisible() const;
	virtual int					GetFitHeight() const;

	virtual GrCell*				HitTest(const GrPoint& pt) const;
	virtual GrCell*				HitDisplayTest(const GrPoint& pt) const;

protected:
	virtual void				OnGridCoreAttached();
	virtual void				OnYChanged();

private:
	void						gridCore_Cleared(GrObject* pSender, GrEventArgs* e);
	void						gridCore_Created(GrObject* pSender, GrEventArgs* e);

	void						columnList_ColumnInserted(GrObject* pSender, GrColumnEventArgs* e);
	void						columnList_ColumnRemoved(GrObject* pSender, GrColumnEventArgs* e);
	void						columnList_ColumnGroupingChanged(GrObject* pSender, GrColumnEventArgs* e);

	void						groupingInfo_LevelChanged(GrObject* pSender, GrEventArgs* e);

	void						ResetGroupingLevel();
	void						AddGrouping(GrGroupingInfo* pGroupingInfo);
	void						RemoveGrouping(GrGroupingInfo* pGroupingInfo);
	void						RepositionGrouping();

private:
	_Groupings					m_vecGroupings;
	bool						m_bEnableGrouping;
	bool						m_bGroupingChanged;
};

class GrGroupingInfo : public GrCell
{
	typedef std::map<uint, GrGroupingRow*> _MapGroupingRows;
	typedef GrEventBase<GrEventArgs, GrGroupingInfo> _GrEvent;
public:
	GrGroupingInfo(GrColumn* pColumn);

	GrColumn*					GetColumn() const;

	bool						GetGrouped() const;
	void						SetGrouped(bool b);

	void						SetExpanded(bool b);
	bool						GetExpanded() const;

	void						SetSortType(GrSort sortType);
	GrSort						GetSortType() const;

	uint						GetGroupingLevel() const;
	void						SetGroupingLevel(uint level);

	void						SetText();

public: 
	virtual	GrCellType			GetCellType() const { return GrCellType_GroupingInfo; }

	virtual int					GetX() const;
	virtual int					GetY() const;
	virtual int					GetWidth() const;
	virtual int					GetHeight() const;
	virtual bool				GetVisible() const { return true; }

	virtual GrHorzAlign			GetTextHorzAlign() const;
	virtual GrVertAlign			GetTextVertAlign() const;

	virtual GrMouseOverState	HitMouseOverTest(GrPoint pt) const;
	virtual GrPoint				GetDisplayPosition() const;
	virtual bool				GetDisplayable() const;
	virtual void				Render(GrGridRenderer* pRenderer, const GrRect* pClipping) const;

	virtual const GrRow*		GetRow() const { return m_pGroupingList; }
	virtual	GrRow*				GetRow() { return m_pGroupingList; }

public:
	_GrEvent					LevelChanged;

protected:
	virtual void				OnGridCoreAttached();
	virtual void				OnGridCoreDetached();

private:
	void						SetPosition(GrPoint pt);
	void						SetGroupingLevelCore(uint level);

private:
	GrGroupingList*				m_pGroupingList;
	GrColumn*					m_pColumn;
	GrPoint						m_pt;
	bool						m_bExpanded;
	bool						m_bGrouped;
	GrSort						m_sortType;

	_MapGroupingRows			m_mapGroupingRows;
	uint						m_nLevel;

	friend class GrGroupingList;
};

class SortRowNormal
{
public:
	bool operator () (const GrRow* pRow1, const GrRow* pRow2);
};