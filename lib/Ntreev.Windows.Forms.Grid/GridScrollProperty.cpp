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
#include "GridScrollProperty.h"
#include "GridWin32.h"
#include "GridControl.h"
#include "GridColumn.h"
#include "GridRow.h"

#include "GrGridCore.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
#define SB_LINEUP           0
#define SB_LINELEFT         0
#define SB_LINEDOWN         1
#define SB_LINERIGHT        1
#define SB_PAGEUP           2
#define SB_PAGELEFT         2
#define SB_PAGEDOWN         3
#define SB_PAGERIGHT        3
#define SB_THUMBPOSITION    4
#define SB_THUMBTRACK       5
#define SB_TOP              6
#define SB_LEFT             6
#define SB_BOTTOM           7
#define SB_RIGHT            7
#define SB_ENDSCROLL        8

	ScrollProperty::ScrollProperty(_GridControl^ gridControl, _ScrollProperties^ scrollProperties)
		: GridObject(gridControl), m_scrollProperties(scrollProperties)
	{

	}

	void ScrollProperty::Value::set(int value)
	{
		SetValue(value, _ScrollEventType::ThumbPosition);
	}

	void ScrollProperty::WndProc(int wParam)
	{
		int nValue		= m_scrollProperties->Value;				;

		_ScrollEventType ScrollType;

		switch(Win32::API::LoWord(wParam))
		{
		case SB_ENDSCROLL:
			{
				ScrollType = _ScrollEventType::EndScroll;
			}
			break;
		case SB_LEFT:
			{
				nValue = m_scrollProperties->Minimum;
				ScrollType = _ScrollEventType::First;
			}
			break;
		case SB_RIGHT:
			{
				nValue = m_scrollProperties->Maximum;
				ScrollType = _ScrollEventType::Last;
			}
			break;
		case SB_LINELEFT:
			{
				nValue -= m_scrollProperties->SmallChange;
				ScrollType = _ScrollEventType::SmallDecrement;
			}
			break;
		case SB_LINERIGHT:
			{
				nValue += m_scrollProperties->SmallChange;
				ScrollType = _ScrollEventType::SmallIncrement;
			}
			break;
		case SB_PAGELEFT:
			{
				nValue -= m_scrollProperties->LargeChange;
				ScrollType = _ScrollEventType::LargeDecrement;
			}
			break;
		case SB_PAGERIGHT:
			{
				nValue += m_scrollProperties->LargeChange;
				ScrollType = _ScrollEventType::LargeIncrement;
			}
			break;
		case SB_THUMBPOSITION:
			{
				nValue = Win32::API::HiWord(wParam);
				ScrollType = _ScrollEventType::ThumbPosition;
			}
			break;
		case SB_THUMBTRACK:
			{
				nValue = Win32::API::HiWord(wParam);
				ScrollType = _ScrollEventType::ThumbTrack;
			}
			break;
		default:
			return;
		}

		SetValue(nValue, ScrollType);
	}

	void ScrollProperty::SetValue(int value, _ScrollEventType scrollEventType)
	{
		int oldValue = m_scrollProperties->Value;
		int newValue = ValidateValue(value);

		if(oldValue == newValue)
			return;

		_ScrollEventArgs se(scrollEventType, oldValue, newValue, ScrollOrientation);
		m_scrollProperties->Value = newValue;
		GridControl->InvokeScroll(%se);
		Invalidate();
	}

	bool ScrollProperty::DoScroll(int value)
	{
		int newValue = ValidateValue(value);

		if(newValue != Value)
		{
			m_scrollProperties->Value = newValue;
			return true;
		}
		return false;
	}

	HScrollProperty::HScrollProperty(_GridControl^ gridControl, _HScrollProperties^ scrollProperties)
		: ScrollProperty(gridControl, scrollProperties)
	{
		m_pColumnList = GridCore->GetColumnList();
		gridControl->ClientSizeChanged	+= gcnew _EventHandler(this, &HScrollProperty::gridControl_OnClientSizeChanged);
		gridControl->SizeChanged		+= gcnew _EventHandler(this, &HScrollProperty::gridControl_OnSizeChanged);
	}

	void HScrollProperty::EnsureVisible(GrColumn* pColumn)
	{
		if(pColumn->NeedToEnsure() == false)
			return;
		
		uint nVisibleIndex = pColumn->GetScrollableIndex();

		uint nNewValue;
		if(nVisibleIndex < (uint)Value)
		{
			nNewValue = nVisibleIndex;
		}
		else
		{
			GrColumnList* pColumnList = GridCore->GetColumnList();
			nNewValue = pColumnList->ClipTo(nVisibleIndex);
		}

		Value = nNewValue;
	}

	void HScrollProperty::EnsureVisible(_Column^ column)
	{
		EnsureVisible(column->NativeRef);
	}

	bool HScrollProperty::DoScroll(_ScrollEventType scrollEventType)
	{
		int value = Value;
		switch(scrollEventType)
		{
		case _ScrollEventType::First:
			value = Minimum;
			break;
		case _ScrollEventType::SmallDecrement:
			value -= SmallChange;
			break;
		case _ScrollEventType::LargeDecrement:
			{
				int oldValue = value;
				value = (int)m_pColumnList->ClipTo((uint)value);
				if(value == oldValue)
				{
					if(value != 0)
						value--;
				}
			}
			break;
		case _ScrollEventType::LargeIncrement:
			{
				int oldValue = value;
				value = (int)m_pColumnList->ClipFrom((uint)value);
				if(value == oldValue)
				{
					if((uint)value + 1 != m_pColumnList->GetVisibleColumnCount())
						value++;
				}
			}
			break;
		case _ScrollEventType::SmallIncrement:
			value += SmallChange;
			break;
		case _ScrollEventType::Last:
			value = ValidateValue(Maximum);
			break;
		default:
			throw gcnew System::ArgumentException();
		}

		return DoScroll(value);
	}

	void HScrollProperty::Update()
	{
		if(m_scrollProperties->Visible == false)
			return;

		GrColumnList* pColumnList = GridCore->GetColumnList();
		int nMinWidth = pColumnList->GetBound()->right - pColumnList->GetUnfrozenX();

		//_Rectangle displayRectangle = GridControl->DisplayRectangle;

		//int nDisplayWidth = displayRectangle.Width - pColumnList->GetUnfrozenX();
		//m_scrollProperties->Visible = nMinWidth > nDisplayWidth ? true : false;
		int nDisplayWidth = GridControl->DisplayRectangle.Width - pColumnList->GetUnfrozenX();

		uint nScrollableCount = pColumnList->GetUnfrozenColumnCount();

		//if(nDisplayWidth > 0 && m_scrollProperties->Visible == true)
		{
			int nCountPerPage = 0;
			int nTempWidth = nDisplayWidth;
			for(int i=nScrollableCount-1 ; i>=0 ; i--)
			{
				const GrColumn* pCol = pColumnList->GetUnfrozenColumn(i);
				int nColWidth = pCol->GetWidth();
				if(nColWidth < nTempWidth)
				{
					nCountPerPage++;
					nTempWidth -= nColWidth;
				}
				else
				{
					break;
				}
			}
			nCountPerPage = System::Math::Max(1, nCountPerPage);

			int nLargeChange = (int)((float)nScrollableCount * (float)nDisplayWidth / (float)nMinWidth);
			nLargeChange = System::Math::Max(1, nLargeChange);

			m_scrollProperties->Minimum = 0;
			m_scrollProperties->Maximum = nScrollableCount - nCountPerPage + nLargeChange - 1;
			m_scrollProperties->LargeChange = nLargeChange;
		}
	}

	void HScrollProperty::UpdateVisible()
	{
		GrColumnList* pColumnList = GridCore->GetColumnList();
		bool visible = false;
		if(pColumnList->GetUnfrozenColumnCount() != 0)
		{
			visible = m_pColumnList->GetBound()->right > GridControl->DisplayRectangle.Width ? true : false;
		}

		m_scrollProperties->Visible = visible;
	}

	void HScrollProperty::SetValue(int value, _ScrollEventType scrollEventType)
	{
		ScrollProperty::SetValue(value, scrollEventType);
	}

	void HScrollProperty::gridControl_OnClientSizeChanged(object^ /*sender*/, _EventArgs^ /*e*/)
	{
		Update();
	}

	void HScrollProperty::gridControl_OnSizeChanged(object^ /*sender*/, _EventArgs^ /*e*/)
	{
		UpdateVisible();
	}

	VScrollProperty::VScrollProperty(_GridControl^ gridControl, _VScrollProperties^ scrollProperties)
		: ScrollProperty(gridControl, scrollProperties)
	{
		m_pDataRowList = GridCore->GetDataRowList();
		gridControl->ClientSizeChanged	+= gcnew _EventHandler(this, &VScrollProperty::gridControl_OnClientSizeChanged);
		gridControl->SizeChanged		+= gcnew _EventHandler(this, &VScrollProperty::gridControl_OnSizeChanged);
		gridControl->MouseWheel			+= gcnew _MouseEventHandler(this, &VScrollProperty::gridControl_OnMouseWheel);
	}

	void VScrollProperty::EnsureVisible(_Row^ row)
	{
		EnsureVisible(row->NativeRef);
	}

	void VScrollProperty::EnsureVisible(IDataRow* pDataRow)
	{
		if(pDataRow->NeedToEnsure() == false)
			return;

		if(pDataRow->GetHeight() > GridControl->DataRectangle.Height)
		{
			Value = pDataRow->GetVisibleIndex();
		}
		else
		{
			uint nVisibleIndex = pDataRow->GetVisibleIndex();

			uint nNewValue;
			if(nVisibleIndex < (uint)Value)
			{
				nNewValue = nVisibleIndex;
			}
			else
			{
				GrDataRowList* pDataRowList = GridCore->GetDataRowList();
				nNewValue = pDataRowList->ClipTo(nVisibleIndex);
			}
			Value = nNewValue;
		}
	}

	bool VScrollProperty::DoScroll(_ScrollEventType scrollEventType)
	{
		int value = Value;
		switch(scrollEventType)
		{
		case _ScrollEventType::First:
			value = Minimum;
			break;
		case _ScrollEventType::SmallDecrement:
			value -= SmallChange;
			break;
		case _ScrollEventType::LargeDecrement:
			{
				int oldValue = value;
				value = (int)m_pDataRowList->ClipTo((uint)value);
				if(value == oldValue)
				{
					if(value != 0)
						value--;
				}
			}
			break;
		case _ScrollEventType::LargeIncrement:
			{
				int oldValue = value;
				value = (int)m_pDataRowList->ClipFrom((uint)value);
				if(value == oldValue)
				{
					if((uint)value + 1 != m_pDataRowList->GetVisibleRowCount())
						value++;
				}
			}
			break;
		case _ScrollEventType::SmallIncrement:
			value += SmallChange;
			break;
		case _ScrollEventType::Last:
			value = ValidateValue(Maximum);
			break;
		default:
			throw gcnew System::ArgumentException();
		}

		return DoScroll(value);
	}

	void VScrollProperty::Update()
	{
		if(m_scrollProperties->Visible == false)
			return;

		GrDataRowList* pDataRowList = GridCore->GetDataRowList();
		GrRect rtRow = *pDataRowList->GetBound();



		uint nVisibleRowCount = pDataRowList->GetVisibleRowCount();
		int nDataHeight = GridControl->DisplayRectangle.Height - rtRow.top;


		int nCountPerPage = 0;
		int nTempHeight = nDataHeight;
		for(int i=nVisibleRowCount-1 ; i>=0 ; i--)
		{
			const IDataRow* pDataRow = pDataRowList->GetVisibleRow(i);
			int nColHeight = pDataRow->GetHeight();
			if(nColHeight < nTempHeight)
			{
				nCountPerPage++;
				nTempHeight -= nColHeight;
			}
			else
			{
				break;
			}
		}
		nCountPerPage = System::Math::Max(1, nCountPerPage);

		int nLargeChange = (int)((float)nVisibleRowCount * (float)nDataHeight / (float)rtRow.GetHeight());
		nLargeChange = System::Math::Max(1, nLargeChange);

		m_scrollProperties->Minimum = 0;
		m_scrollProperties->Maximum = nVisibleRowCount - nCountPerPage + nLargeChange - 1;
		m_scrollProperties->LargeChange = nLargeChange;
	}

	void VScrollProperty::UpdateVisible()
	{
		bool visible = false;
		if(m_pDataRowList->GetVisibleRowCount() != 0)
		{
			const GrRect* pBound = m_pDataRowList->GetBound();
			visible = pBound->bottom > GridControl->DisplayRectangle.Bottom ? true : false;
		}

		m_scrollProperties->Visible = visible;
	}

	void VScrollProperty::gridControl_OnClientSizeChanged(object^ /*sender*/, _EventArgs^ /*e*/)
	{
		Update();
	}

	void VScrollProperty::gridControl_OnSizeChanged(object^ /*sender*/, _EventArgs^ /*e*/)
	{
		UpdateVisible();
	}

	void VScrollProperty::gridControl_OnMouseWheel(object^ /*sender*/, _MouseEventArgs^ e)
	{
		using namespace System::Windows::Forms;
		if(m_scrollProperties->Visible == false)
			return;

		int value = e->Delta / SystemInformation::MouseWheelScrollDelta * SystemInformation::MouseWheelScrollLines;
		int rowCount = (int)m_pDataRowList->GetDisplayableRowCount();

		if(rowCount > 1)
		{
			IDataRow* pDataRow = m_pDataRowList->GetDisplayableRow(rowCount - 1);
			if(pDataRow->GetClipped() == true)
				rowCount--;
		}

		if(rowCount < System::Math::Abs(value))
		{
			if(value < 0)
				value = -rowCount;
			else
				value = rowCount;
		}
		

		this->Value = m_scrollProperties->Value - value;
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/