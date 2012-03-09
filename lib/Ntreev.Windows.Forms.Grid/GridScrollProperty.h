//=====================================================================================================================
// Ntreev Grid for .Net 2.0.0.0
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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	typedef System::Windows::Forms::ScrollProperties	_ScrollProperties;
	typedef System::Windows::Forms::ScrollOrientation	_ScrollOrientation;
	typedef System::Windows::Forms::ScrollEventType		_ScrollEventType;
	typedef System::Windows::Forms::ScrollEventArgs		_ScrollEventArgs;

	/// <summary>
	/// 스크롤을 조작할 수 있는 방법을 제공합니다.
	/// </summary>
	public ref class ScrollProperty abstract : GridObject
	{
	public: // methods
		/// <summary>
		/// 값의 유효성을 검사하고 수정합니다.
		/// </summary>
		/// <param name="value">값의 유효성을 검사하고자 하는 <see cref="System::Int32"/>형태의 정수 값입니다.</param>
		/// <returns>
		/// 값의 유효성을 검사하고 수정된 <see cref="System::Int32"/>입니다.
		/// </returns>
		int	ValidateValue(int value)
		{
			int nMin = m_scrollProperties->Minimum;
			int nMax = m_scrollProperties->Maximum - m_scrollProperties->LargeChange + 1;

			if(value < nMin)
				value = nMin;
			if(value > nMax)
				value = nMax;
			return value;
		}

		/// <summary>
		/// 값의 유효성을 검사하고 수정한 후에 스크롤합니다.
		/// </summary>
		/// <remarks>
		/// 이 메서드는 Value 속성 값을 설정하는것과 같은 기능을 수행합니다.
		/// 하지만 Value 속성과는 다르게 Scroll 이벤트를 발생하지 않습니다.
		/// </remarks>
		/// <param name="value">값의 유효성을 검사하고 스크롤 할 <see cref="System::Int32"/>형태의 정수 값입니다.</param>
		/// <returns>
		/// 스크롤이 성공하였다면 true를 반환하고, 그렇지 않다면 false를 반환합니다.
		/// 일반적으로 설정 값이 이전 값과 같은 경우 false를 반환합니다.
		/// </returns>
		bool DoScroll(int value);

	public: // properties
		/// <summary>
		/// 스크롤 막대 상자의 현재 위치를 나타내는 숫자 값을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 막대 상자의 위치를 나타내는 <see cref="System:Int32"/>형태의 정수 값입니다.
		/// </returns>
		property int Value
		{
			int get() { return m_scrollProperties->Value; }
			void set(int value);
		}

		/// <summary>
		/// 먼 거리를 이동하는 명령에 대한 응답으로 스크롤 막대를 움직일 거리를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 막대를 움직일 거리를 나타내는 <see cref="System::Int32"/>형태의 정수 값입니다.
		/// </returns>
		property int LargeChange
		{ 
			int get() { return m_scrollProperties->LargeChange; }
		}

		/// <summary>
		/// 스크롤할 수 있는 범위의 상한을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 막대의 최대 범위를 나타내는<see cref="System::Int32"/>형태의 정수 값입니다.
		/// </returns>
		property int Maximum 
		{
			int get() { return m_scrollProperties->Maximum; }
		}

		/// <summary>
		/// 스크롤할 수 있는 범위의 하한을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 막대의 최소 범위를 나타내는<see cref="System::Int32"/>형태의 정수 값입니다.
		/// </returns>
		property int Minimum 
		{ 
			int get() { return m_scrollProperties->Minimum; }
		}

		/// <summary>
		/// 가까운 거리를 이동하는 명령에 대한 응답으로 스크롤 막대를 움직일 거리를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 막대를 움직일 거리를 나타내는 <see cref="System::Int32"/>형태의 정수 값입니다.
		/// </returns>
		property int SmallChange 
		{
			int get() { return m_scrollProperties->SmallChange; }
		}

		/// <summary>
		/// 스크롤 막대를 사용자에게 표시할 수 있는지 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 표시되고 있으면 true이고, 그렇지 않으면 false입니다.
		/// </returns>
		property bool Visible
		{
			bool get() { return m_scrollProperties->Visible; }
		}

		/// <summary>
		/// 스크롤 막대 방향의 형식을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 막대 방향을 나타내는 <see cref="System::Windows::Forms::ScrollOrientation"/>입니다
		/// </returns>
		property _ScrollOrientation	ScrollOrientation
		{
			virtual _ScrollOrientation get() abstract;
		}

	internal: // methods
		ScrollProperty(_GridControl^ gridControl, _ScrollProperties^ scrollProperties);

		void WndProc(int wParam);
		virtual void SetValue(int value, _ScrollEventType scrollEventType);

	internal: // variables
		initonly _ScrollProperties^		m_scrollProperties;
	};

	/// <summary>
	/// 수평 스크롤에 대한 조작 방법을 제공합니다.
	/// </summary>
	public ref class HScrollProperty sealed : ScrollProperty
	{
	private: // typedefs
		typedef System::Windows::Forms::HScrollProperties	_HScrollProperties;

	public: // methods
		/// <summary>
		/// 대상이 되는 열이 화면내에 표시 될 수 있도록 스크롤을 조정합니다.
		/// </summary>
		/// <param name="column">화면내에 표시하려 하는 <see cref="Column"/>의 인스턴스입니다.</param>
		void EnsureVisible(_Column^ column);

		/// <summary>
		/// 즉시 스크롤을 조정합니다.
		/// </summary>
		/// <param name="scrollEventType">스크롤 방식을 지정하는 <see cref="System::Windows::Forms::ScrollEventType"/>입니다.</param>
		/// <returns>
		/// 스크롤이 성공하여 값이 변경되었으면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		bool DoScroll(_ScrollEventType scrollEventType);

	public: // properties
		/// <summary>
		/// 스크롤 막대 방향의 형식을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 막대 방향을 나타내는 <see cref="System::Windows::Forms::ScrollOrientation"/>입니다
		/// </returns>
		property _ScrollOrientation	ScrollOrientation
		{
			virtual _ScrollOrientation get() override { return _ScrollOrientation::HorizontalScroll; }
		}

	internal: // methods
		HScrollProperty(_GridControl^ gridControl, _HScrollProperties^ scrollProperties);

		void EnsureVisible(GrColumn* pColumn);
		void Update();
		void UpdateVisible();

		virtual void SetValue(int value, _ScrollEventType scrollEventType) override;

	private: // methods
		void gridControl_OnClientSizeChanged(object^ sender, _EventArgs^ e);
		void gridControl_OnSizeChanged(object^ sender, _EventArgs^ e);

	private: // variables
		GrColumnList*	m_pColumnList;
	};

	/// <summary>
	/// 수직 스크롤에 대한 조작 방법을 제공합니다.
	/// </summary>
	public ref class VScrollProperty sealed : ScrollProperty
	{
	private: // typedefs
		typedef System::Windows::Forms::VScrollProperties	_VScrollProperties;
		typedef System::Windows::Forms::MouseEventArgs		_MouseEventArgs;
		typedef System::Windows::Forms::MouseEventHandler	_MouseEventHandler;

	public: // methods
		/// <summary>
		/// 대상이 되는 행이 화면내에 표시 될 수 있도록 스크롤을 조정합니다.
		/// </summary>
		/// <param name="row">화면내에 표시하려 하는 <see cref="Row"/>의 인스턴스입니다.</param>
		void EnsureVisible(_Row^ row);

		/// <summary>
		/// 즉시 스크롤을 조정합니다.
		/// </summary>
		/// <param name="scrollEventType">스크롤 방식을 지정하는 <see cref="System::Windows::Forms::ScrollEventType"/>입니다.</param>
		/// <returns>
		/// 스크롤이 성공하여 값이 변경되었으면 true를, 그렇지 않다면 false를 반환합니다.
		/// </returns>
		bool DoScroll(_ScrollEventType scrollEventType);

	public: // properties
		/// <summary>
		/// 스크롤 막대 방향의 형식을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 스크롤 막대 방향을 나타내는 <see cref="System::Windows::Forms::ScrollOrientation"/>입니다
		/// </returns>
		property _ScrollOrientation	ScrollOrientation
		{
			virtual _ScrollOrientation get() override { return _ScrollOrientation::VerticalScroll; }
		}

	internal: // methods
		VScrollProperty(_GridControl^ gridControl, _VScrollProperties^ scrollProperties);

		void EnsureVisible(IDataRow* pDataRow);
		void Update();
		void UpdateVisible();

	private: // methods
		void gridControl_OnClientSizeChanged(object^ sender, _EventArgs^ e);
		void gridControl_OnSizeChanged(object^ sender, _EventArgs^ e);
		void gridControl_OnMouseWheel(object^ sender, _MouseEventArgs^ e);

	private: // variables
		GrDataRowList*	m_pDataRowList;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/