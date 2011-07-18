#pragma once

#include "GrGridType.h"


#define	GRRS_FOCUSED		0x00000001
#define	GRRS_MOUSEOVER		0x00000002
#define	GRRS_SELECTED		0x00000004
#define	GRRS_PRESSED		0x00000008
#define	GRRS_NO_BOTTOM_LINE	0x00010000
#define	GRRS_NO_RIGHT_LINE	0x00020000
#define	GRRS_TOP_LINE		0x00040000
#define	GRRS_LEFT_LINE		0x00080000

class GrFont;

typedef std::vector<GrColor>	GrColors;
typedef std::vector<GrFont*>	GrFonts;

class GrStyle
{
public:
	GrStyle();

public:
	GrColor					CellForeColor;
	GrColor					CellBackColor;
	GrFont*					CellFont;

	GrColor					SelectedForeColor;
	GrColor					SelectedBackColor;

	GrColor					FocusedForeColor;
	GrColor					FocusedBackColor;

	GrColor					RowHighlightForeColor;
	GrColor					RowHighlightBackColor;

	GrColors				RowItemBackColors;
	GrColors				RowItemForeColors;
	GrFonts					RowItemFonts;

	GrColors				GroupingBackColors;
	GrColors				GroupingForeColors;
	GrFonts					GroupingFonts;

	GrColor					GetCellForeColor() const;
	GrColor					GetCellBackColor() const;
	GrFont*					GetCellFont() const;

	GrColor					GetRowItemForeColor(uint index);
	GrColor					GetRowItemBackColor(uint index);
	GrFont*					GetRowItemFont(uint index);

	GrColor					GetGroupingForeColor(uint index);
	GrColor					GetGroupingBackColor(uint index);
	GrFont*					GetGroupingFont(uint index);

	static const GrStyle	DefaultStyle;
};

class GrGridInvalidator
{
public:
	virtual void Invalidate() = 0;
	virtual void Invalidate(int x, int y, int width, int height) = 0;

	void Invalidate(const GrRect& rect) { Invalidate(rect.left, rect.top, rect.GetWidth(), rect.GetHeight()); }
};

class GrGridRenderer
{
public:
	GrGridRenderer(void* hWnd);
	virtual ~GrGridRenderer();

	virtual bool			DoubleBuffered() const = 0;

	virtual void			OnBeginRender(void* hdc) = 0;

	virtual void			DrawSplitterRow(const GrRect* pRenderRect) = 0;
	virtual void			DrawDropDown(const GrRect* pRenderRect, int nState) = 0;
	virtual void			DrawModal(const GrRect* pRenderRect, int nState) = 0;
	virtual void			DrawTreeGlyph(const GrRect* pRenderRect, bool bOpened = true) = 0;
	virtual void			DrawSortGlyph(const GrRect* pRenderRect, GrSort::Type sortType) = 0;

	virtual void			DrawHeader(GrFlag renderStyle, const GrRect* pRenderRect, GrColor color = GrColor::White, const GrRect* pClipping = NULL) = 0;
	virtual void			DrawCell(GrFlag renderStyle, GrColor color, const GrRect* pRenderRect, const GrRect* pClipping = NULL) = 0;
	virtual void			DrawText(GrFont* pFont, const wchar_t* strText, int nLen, GrColor color, const GrRect* pRenderRect, const GrRect* pClipping = NULL) = 0;
	virtual void			DrawTextEllipsis(GrFont* pFont, const wchar_t* strText, int nLen, GrColor Color, const GrRect* pRenderRect, const GrRect* pClipping = NULL) = 0;
	virtual void			DrawTooltip(const GrRect* pRenderRect) = 0;

	virtual void			DrawSizingLine(int x1, int y1, int x2, int y2) = 0;
	virtual void			DrawSplitterMovingLine(int x1, int y1, int x2, int y2) = 0;
	virtual void			DrawCellLine(int x1, int y1, int x2, int y2) = 0;
	virtual void			DrawRectangle(const GrRect* pRenderRect, GrColor color, int nWidth) = 0;

	virtual void			FillRectangle(GrRect rect, GrColor color) = 0;
	virtual void			DrawLine(int x1, int y1, int x2, int y2, GrColor color) = 0;

	virtual void			OnEndRender() = 0;
	virtual void			SetTransform(GrPoint pt) = 0;

	virtual void*			GetDC() = 0;
	virtual void			ReleaseDC(void* dc) = 0;
};

class GrFont
{
public:
	GrFont(void* hFont);
	virtual ~GrFont();

	virtual int						GetWidth(wchar_t w) const = 0;
	virtual int						GetHeight() const = 0;
	virtual int						GetStringWidth(const wchar_t* str) const = 0;
	virtual void*					GetFont() const = 0;

	std::wstring					m_strFontName;
#ifdef _MANAGED
	virtual System::Drawing::Font^	ToManaged() const = 0;
#endif
};

class GrFontManager
{
	typedef std::map<std::wstring, GrFont*>	_MapFontCaches;
public:
	GrFontManager();
	~GrFontManager();

	static std::wstring				GetFontName(void* hFont);

	static GrFont*					GetFontDesc(void* hFont);
	static GrFont*					GetDefaultFont();

#ifdef _MANAGED
	static GrFont*					FromManagedFont(System::Drawing::Font^ font);
	static System::Drawing::Font^	ToManagedFont(GrFont* pFont);
#endif

private:
	static _MapFontCaches			m_mapFontCache;
	static GrFont*					m_pDefaultFont;
};

GrFont*				GrFontCreator(void* pFontHandle);
GrGridRenderer*		GrGridRendererCreator(void* pWindowHandle);