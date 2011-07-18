#include "StdAfx.h"
#include "GrGridRenderer.h"
#include <windows.h>


GrFontManager::_MapFontCaches GrFontManager::m_mapFontCache;
GrFont* GrFontManager::m_pDefaultFont = NULL;

const GrFontManager FontManager;
const GrStyle		GrStyle::DefaultStyle;

GrStyle::GrStyle()
{
	CellForeColor = GrColor::Black;
	CellBackColor = GrColor::White;
	CellFont	  = GrFontManager::GetDefaultFont();

	SelectedForeColor = GrColor::White;
	SelectedBackColor = GrColor::CornflowerBlue;

	FocusedForeColor = GrColor::White;
	FocusedBackColor = GrColor::LightSkyBlue;

	RowHighlightForeColor = GrColor::Black;
	RowHighlightBackColor = GrColor::LightCyan;

	GroupingBackColors.push_back(GrColor::Red);
	GroupingBackColors.push_back(GrColor::Green);
	GroupingBackColors.push_back(GrColor::Blue);
	GroupingBackColors.push_back(GrColor::Yellow);
	GroupingBackColors.push_back(GrColor::Purple);
	GroupingBackColors.push_back(GrColor::SkyBlue);
}

GrColor GrStyle::GetCellForeColor() const
{
	if(CellForeColor == GrColor::Empty)
		return DefaultStyle.CellForeColor;
	return CellForeColor;
}

GrColor GrStyle::GetCellBackColor() const
{
	if(CellBackColor == GrColor::Empty)
		return DefaultStyle.CellBackColor;
	return CellBackColor;
}

GrFont* GrStyle::GetCellFont() const
{
	if(CellFont == NULL)
		return DefaultStyle.CellFont;
	return CellFont;
}

GrColor GrStyle::GetRowItemForeColor(uint index)
{
	if(RowItemForeColors.size() == 0)
		return CellForeColor;
	return RowItemForeColors[index % RowItemForeColors.size()];
}

GrColor GrStyle::GetRowItemBackColor(uint index)
{
	if(RowItemBackColors.size() == 0)
		return CellBackColor;

	return RowItemBackColors[index % RowItemBackColors.size()];
}

GrFont* GrStyle::GetRowItemFont(uint index)
{
	if(RowItemFonts.size() == 0)
		return NULL;
	return RowItemFonts[index % RowItemFonts.size()];
}

GrColor GrStyle::GetGroupingForeColor(uint index)
{
	if(GroupingForeColors.size() == 0)
		return CellForeColor;
	return GroupingForeColors[index % GroupingForeColors.size()];
}

GrColor GrStyle::GetGroupingBackColor(uint index)
{
	if(GroupingBackColors.size() == 0)
		return CellBackColor;

	return GroupingBackColors[index % GroupingBackColors.size()];
}

GrFont* GrStyle::GetGroupingFont(uint index)
{
	if(GroupingFonts.size() == 0)
		return NULL;
	return GroupingFonts[index % GroupingFonts.size()];
}

#ifdef _MANAGED
GrFont* GrFontManager::FromManagedFont(System::Drawing::Font^ font)
{
	if(font == nullptr)
		return NULL;
	return GrFontManager::GetFontDesc(font->ToHfont().ToPointer());
}

System::Drawing::Font^ GrFontManager::ToManagedFont(GrFont* pFont)
{
	if(pFont == NULL)
		return nullptr;
	if(pFont == m_pDefaultFont)
		return System::Windows::Forms::Control::DefaultFont;
	System::IntPtr ptr(pFont->GetFont());
	System::Drawing::Font^ font = System::Drawing::Font::FromHfont(ptr);
	System::Drawing::Font^ font1 = gcnew System::Drawing::Font(font->FontFamily, font->SizeInPoints, font->Style, System::Windows::Forms::Control::DefaultFont->Unit, font->GdiCharSet);
	return font1;
	
	//return gcnew System::Drawing::Font(font->
}
#endif

GrGridRenderer::GrGridRenderer(void* /*hWnd*/)
{

}

GrGridRenderer::~GrGridRenderer()
{

}

GrFont::GrFont(void* /*hFont*/)
{

}

GrFont::~GrFont()
{

}

GrFontManager::GrFontManager()
{

}

GrFontManager::~GrFontManager()
{
	for_stl(_MapFontCaches, m_mapFontCache, itor)
	{
		delete (*itor).second;
	}
}

GrFont* GrFontManager::GetDefaultFont()
{
	if(m_pDefaultFont == NULL)
#ifdef _MANAGED
		m_pDefaultFont = FromManagedFont(System::Windows::Forms::Control::DefaultFont);
#else
		m_pDefaultFont = GetFontDesc((void*)GetStockObject(SYSTEM_FIXED_FONT));
#endif
	return m_pDefaultFont;
}

GrFont* GrFontManager::GetFontDesc(void* hFont)
{
	std::wstring strFontName = GetFontName(hFont);
	_MapFontCaches::const_iterator itor = m_mapFontCache.find(strFontName);

	GrFont* pFontDesc = NULL;
	if(itor == m_mapFontCache.end())
	{
		pFontDesc = GrFontCreator(hFont);
		m_mapFontCache.insert(_MapFontCaches::value_type(strFontName, pFontDesc));
	}
	else
	{
		pFontDesc = (*itor).second;
	}
	return pFontDesc;
}

std::wstring GrFontManager::GetFontName(void* hFont)
{
	LOGFONTW logfont;
	GetObjectW(hFont, sizeof(logfont), &logfont);

	wchar_t strFileName[MAX_PATH];
	wsprintf(strFileName, L"%s%d%d%d%d%d%d%d%d%d%d%d%d%d.data", logfont.lfFaceName,
		logfont.lfCharSet, logfont.lfClipPrecision, logfont.lfEscapement, logfont.lfHeight,
		logfont.lfItalic, logfont.lfOrientation, logfont.lfOutPrecision, logfont.lfPitchAndFamily,
		logfont.lfQuality, logfont.lfStrikeOut, logfont.lfUnderline, logfont.lfWeight, logfont.lfWidth);

	return std::wstring(strFileName);
}