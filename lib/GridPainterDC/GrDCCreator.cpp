﻿//=====================================================================================================================
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


#include "GrDCCreator.h"
#include "GrGridPainterDC.h"

GrFontCreator::Fonts GrFontCreator::m_fonts;
static GrFontCreator fontCreator;

#ifdef _MANAGED

GrFont* GrFont::GetDefaultFont()
{
	static GrFontDC defaultFont(System::Windows::Forms::Control::DefaultFont->ToHfont().ToPointer());
	return &defaultFont;
}

GrFont* GrFont::FromManaged(System::Drawing::Font^ font)
{
	if(font == nullptr)
		return nullptr;
	if(System::Windows::Forms::Control::DefaultFont == font)
		return GetDefaultFont();
	return GrFontCreator::Create(font->ToHfont().ToPointer());
}

System::Drawing::Font^ GrFont::ToManaged(GrFont* pFont)
{
	if(pFont == nullptr)
		return nullptr;
	if(pFont == GetDefaultFont())
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
}

#endif

GrFontCreator::GrFontCreator()
{

}

GrFontCreator::~GrFontCreator()
{
    for(Fonts::iterator itor = m_fonts.begin() ; itor != m_fonts.end() ; itor++)
    {
        delete (*itor).second;
    }
}

std::wstring GrFontCreator::GetFontKey(void* fontHandle)
{
    LOGFONTW logfont;
    GetObjectW(fontHandle, sizeof(logfont), &logfont);

    wchar_t strFileName[MAX_PATH];
    wsprintf(strFileName, L"%s%d%d%d%d%d%d%d%d%d%d%d%d%d.data", logfont.lfFaceName,
        logfont.lfCharSet, logfont.lfClipPrecision, logfont.lfEscapement, logfont.lfHeight,
        logfont.lfItalic, logfont.lfOrientation, logfont.lfOutPrecision, logfont.lfPitchAndFamily,
        logfont.lfQuality, logfont.lfStrikeOut, logfont.lfUnderline, logfont.lfWeight, logfont.lfWidth);

    return std::wstring(strFileName);
}

GrFont* GrFontCreator::Create(void* fontHandle)
{
    //std::wstring fontName= GetFontKey(fontHandle);
    Fonts::const_iterator itor = m_fonts.find(fontHandle);

    GrFont* pFont = nullptr;
    if(itor == m_fonts.end())
    {
        pFont = new GrFontDC(fontHandle);
        m_fonts.insert(Fonts::value_type(fontHandle, pFont));
    }
    else
    {
        pFont = (*itor).second;
    }
    return pFont;
}

void* GrFontCreator::GetFontHandle(GrFont* pFont)
{
    return dynamic_cast<GrFontDC*>(pFont)->GetFontHandle();
}