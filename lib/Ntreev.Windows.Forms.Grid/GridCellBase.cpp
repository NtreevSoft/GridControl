#include "StdAfx.h"
#include "GridCellBase.h"
#include "GridControl.h"

#include <vcclr.h>

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	CellBase::CellBase(_GridControl^ gridControl, GrCell* pCell)
		: GridObject(gridControl), m_pCell(pCell)
	{

	}

	CellBase::CellBase(GrCell* pCell) : GridObject(), m_pCell(pCell)
	{

	}

	CellBase::~CellBase()
	{

	}

	_Color CellBase::BackColor::get()
	{
		return m_pCell->GetBackColor();
	}

	void CellBase::BackColor::set(_Color color)
	{
		m_pCell->SetBackColor(color);
		Invalidate(this);
	}

	_Color CellBase::ForeColor::get()
	{
		return m_pCell->GetForeColor();
	}

	void CellBase::ForeColor::set(_Color color)
	{
		m_pCell->SetForeColor(color);
		Invalidate(this);
	}

	_Font^ CellBase::Font::get()
	{
		GrFont* pFont = (GrFont*)m_pCell->GetRenderingFont();
		return GrFontManager::ToManagedFont(pFont);
	}

	void CellBase::Font::set(_Font^ font)
	{
		GrFont* pFont = GridCore->GetFont(font->ToHfont().ToPointer());
		m_pCell->SetFont(pFont);
		Invalidate(this);
	}

	bool CellBase::IsDisplayable::get()
	{
		return m_pCell->GetDisplayable();
	}

	void CellBase::ResetBackColor()
	{
		m_pCell->SetBackColor(GrColor::Empty);
		Invalidate(this);
	}

	void CellBase::ResetForeColor()
	{
		m_pCell->SetForeColor(GrColor::Empty);
		Invalidate(this);
	}

	bool CellBase::ShouldSerializeForeColor()
	{
		return m_pCell->GetForeColor(false) != GrColor::Empty;
	}

	bool CellBase::ShouldSerializeBackColor()
	{
		return m_pCell->GetBackColor(false) != GrColor::Empty;
	}

	bool CellBase::ShouldSerializeFont()
	{
		GrFont* pFont = m_pCell->GetFont(false);
		if(pFont == nullptr)
			return false;
		return pFont != GrFontManager::GetDefaultFont();
	}

	bool CellBase::ShouldSerializePadding()
	{
		return m_pCell->GetPadding() != GrPadding::Default;
	}

	void CellBase::ResetPadding()
	{
		m_pCell->SetPadding(GrPadding::Default);
		Invalidate(this);
	}

	void CellBase::ResetFont()
	{
		m_pCell->SetFont(NULL);
		Invalidate(this);
	}

	_Padding CellBase::Padding::get()
	{
		return m_pCell->GetPadding();
	}

	void CellBase::Padding::set(_Padding value)
	{
		m_pCell->SetPadding(value);		
		Invalidate(this);
	}

	void CellBase::OnBackColorChanged(_EventArgs^ e)
	{
		BackColorChanged(this, e);
	}

	void CellBase::OnForeColorChanged(_EventArgs^ e)
	{
		ForeColorChanged(this, e);
	}

	void CellBase::OnFontChanged(_EventArgs^ e)
	{
		FontChanged(this, e);
	}

	void CellBase::Invalidate(CellBase^ cellBase)
	{
		if(cellBase == nullptr)
			throw gcnew System::ArgumentNullException("cellBase");
		if(IsDisplayable == false)
			return;

		GridControl->Invalidate(DisplayRectangle, false);
	}

	_Color CellBase::RenderingBackColor::get()
	{
		GrColor clr = m_pCell->GetRenderingBackColor();
		return _Color::FromArgb(255, clr.r, clr.g, clr.b);
	}

	_Color CellBase::RenderingForeColor::get()
	{
		GrColor clr = m_pCell->GetRenderingForeColor();
		return _Color::FromArgb(255, clr.r, clr.g, clr.b);
	}

	_Rectangle CellBase::ClientRectangle::get()
	{
		GrRect rtDisplayRect = m_pCell->GetDisplayRect();
		rtDisplayRect.Contract(m_pCell->GetPadding());
		return rtDisplayRect;
	}

	_Rectangle CellBase::DisplayRectangle::get()
	{
		return m_pCell->GetDisplayRect();
	}

	void CellBase::ForeColorChanged::add(_EventHandler^ p)
	{
		m_eventForeColorChanged += p; 
	}

	void CellBase::ForeColorChanged::remove(_EventHandler^ p)
	{
		m_eventForeColorChanged -= p; 
	}

	void CellBase::ForeColorChanged::raise(object^ sender, _EventArgs^ e)
	{
		if(m_eventForeColorChanged != nullptr) 
		{
			m_eventForeColorChanged->Invoke(sender, e); 
		} 
	}

	void CellBase::BackColorChanged::add(_EventHandler^ p)
	{
		m_eventBackColorChanged += p; 
	}

	void CellBase::BackColorChanged::remove(_EventHandler^ p)
	{
		m_eventBackColorChanged -= p; 
	}

	void CellBase::BackColorChanged::raise(object^ sender, _EventArgs^ e)
	{
		if(m_eventBackColorChanged != nullptr) 
		{
			m_eventBackColorChanged->Invoke(sender, e); 
		} 
	}

	void CellBase::FontChanged::add(_EventHandler^ p)
	{
		m_eventFontChanged += p; 
	}

	void CellBase::FontChanged::remove(_EventHandler^ p)
	{
		m_eventFontChanged -= p; 
	}

	void CellBase::FontChanged::raise(object^ sender, _EventArgs^ e)
	{
		if(m_eventFontChanged != nullptr) 
		{
			m_eventFontChanged->Invoke(sender, e); 
		} 
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/