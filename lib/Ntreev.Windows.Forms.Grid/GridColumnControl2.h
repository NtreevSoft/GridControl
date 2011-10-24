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
#include "GridColumn.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 컨트롤을 이용해 편집할 수 있고 셀을 그릴 수 있는 기능을 제공합니다.
	/// </summary>
	/// <typeparam name="TEditing"> 
	/// 편집할때 사용할 컨트롤의 타입입니다.
	/// </typeparam>
	/// <typeparam name="TView"> 
	/// 셀을 표시할때 사용할 컨트롤의 타입입니다.
	/// </typeparam>
	generic<class TEditing, class TView> where TEditing : _Control where TView : _Control
	public ref class ColumnControl abstract : Column, IColumnControl, IControlProvider, IValuePainter
	{
	public: // methods
		/// <summary>
		/// 컨트롤을 생성하고 <see cref="ColumnControl"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		ColumnControl();

		/// <summary>
		/// <paramref name="controlArgs"/>를 사용하여 컨트롤을 생성하고 <see cref="ColumnControl"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <remarks>
		/// 만약 컨트롤의 특수 생성자가 있을때에는 <paramref name="controlArgs"/>를 사용하여 필요한 인자를 넘길 수 있습니다.
		/// </remarks>
		ColumnControl(... cli::array<object^>^ controlArgs);

	public: // properties
		/// <summary>
		/// 셀을 편집하는 방식을 가져옵니다.
		/// </summary>
		property _EditingType EditingType
		{
			virtual _EditingType get() override sealed { return _EditingType::Control; }
		}

		/// <summary>
		/// 셀이 편집될때 사용되는 컨트롤의 인스턴스를 가져옵니다.
		/// </summary>
		[System::ComponentModel::Browsable(false)]
		property TEditing EditingControl
		{
			TEditing get() { return m_control; } 
		}

		/// <summary>
		/// 셀을 표시할때 사용되는 컨트롤의 인스턴스를 가져옵니다.
		/// </summary>
		[System::ComponentModel::Browsable(false)]
		property TView ViewControl
		{
			TView get() { return m_viewControl; } 
		}

		/// <summary>
		/// 별도의 셀 렌더링 기능을 지원하는지에 대한 여부를 가져옵니다.
		/// </summary>
		property bool PaintValueSupported
		{
			virtual bool get() { return true; }
		}

		/// <summary>
		/// 셀을 표시할때 사용되는 컨트롤이 투명화를 지원하는지를 가져옵니다.
		/// </summary>
		property bool IsViewControlTransparented
		{
			virtual bool get() { return false; }
		}

	public: // events
		/// <summary>
		/// 셀 편집이 시작되고 컨트롤이 보여지기 전에 발생합니다.
		/// </summary>
		[_Description("셀 편집이 시작되고 컨트롤이 보여지기 전에 발생합니다.")]
		event AttachEventHandler^	Attaching
		{
			void add(AttachEventHandler^ p) { m_eventAttaching += p; }
			void remove(AttachEventHandler^ p) { m_eventAttaching += p; }
		private:
			void raise(object^ sender, AttachEventArgs^ e) { if(m_eventAttaching != nullptr) { m_eventAttaching->Invoke(sender, e); } }
		}

		/// <summary>
		/// 셀 편집이 시작되고 컨트롤이 보여진 후에 발생합니다.
		/// </summary>
		[_Description("셀 편집이 시작되고 컨트롤이 보여진 후에 발생합니다.")]
		event AttachEventHandler^	Attached
		{
			void add(AttachEventHandler^ p) { m_eventAttached += p; }
			void remove(AttachEventHandler^ p) { m_eventAttached += p; }
		private:
			void raise(object^ sender, AttachEventArgs^ e) { if(m_eventAttached != nullptr) { m_eventAttached->Invoke(sender, e); } }
		}

		/// <summary>
		/// 셀 편집이 종료되고 컨트롤이 감춰지기 전에 발생합니다.
		/// </summary>
		[_Description("셀 편집이 종료되고 컨트롤이 감춰지기 전에 발생합니다.")]
		event DetachEventHandler^	Detaching
		{
			void add(DetachEventHandler^ p) { m_eventDetaching += p; }
			void remove(DetachEventHandler^ p) { m_eventDetaching += p; }
		private:
			void raise(object^ sender, DetachEventArgs^ e) { if(m_eventDetaching != nullptr) { m_eventDetaching->Invoke(sender, e); } }
		}

		/// <summary>
		/// 셀 편집이 종료되고 컨트롤이 감춰진 후에 발생합니다.
		/// </summary>
		[_Description("셀 편집이 종료되고 컨트롤이 감춰진 후에 발생합니다.")]
		event DetachEventHandler^	Detached
		{
			void add(DetachEventHandler^ p) { m_eventDetached += p; }
			void remove(DetachEventHandler^ p) { m_eventDetached += p; }
		private:
			void raise(object^ sender, DetachEventArgs^ e) { if(m_eventDetached != nullptr) { m_eventDetached->Invoke(sender, e); } }
		}
		
	internal: // methods
		virtual object^						GetValue() sealed = IColumnControl::GetValue { return GetEditingValue(m_control); }
		virtual void						SetValue(object^ value) sealed = IColumnControl::SetValue { SetEditingValue(m_control, value); }

		virtual void						InvokeAttaching(EditingReason^ by, object^ value) = IColumnControl::InvokeAttaching;
		virtual void						InvokeAttached(EditingReason^ by, object^ value) = IColumnControl::InvokeAttached;
		virtual void						InvokeDetaching(bool modified, object^ value) = IColumnControl::InvokeDetaching;
		virtual void						InvokeDetached(bool modified, object^ value) = IColumnControl::InvokeDetached;

	protected: // methods
		/// <summary>
		/// 셀을 그릴때 호출됩니다.
		/// </summary>
		/// <param name="graphics">그리는데 필요한 개체입니다.</param>
		/// <param name="renderRect">그려질 셀의 안쪽 영역을 나타냅니다.</param>
		/// <param name="clipRect">클립 영역을 나타냅니다.</param>
		/// <param name="style">셀의 배경색 및 전경색, 글꼴을 가져오는 개체입니다.</param>
		/// <param name="value">그려질 셀의 값을 나타냅니다.</param>
		virtual void						PaintValue(_Graphics^ graphics, _Rectangle renderRect, _Rectangle clipRect, IStyle^ style, object^ value) = IValuePainter::PaintValue;

		/// <summary>
		/// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
		/// </summary>
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
		virtual object^						GetEditingValue(TEditing control) abstract;

		/// <summary>
		/// 셀의 값을 컨트롤에 적용할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
		virtual void						SetEditingValue(TEditing control, object^ value) abstract;

		/// <summary>
		/// 셀의 값을 그리기 위해 컨트롤에 적용할때 호출됩니다.
		/// </summary>
		/// <param name="viewControl">표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
		virtual void						SetViewValue(TView viewControl, object^ value) abstract;

		/// <summary>
		/// 컨트롤을 셀의 위치로 이동하고 셀의 크기로 조정할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="location">셀의 안쪽영역의 위치입니다.</param>
		/// <param name="size">셀의 안쪽영역의 크기입니다.</param>
		virtual void						SetControlLayout(TEditing control, _Point location, _Size size);

		/// <summary>
		/// 셀을 그리기 전에 표시될 컨트롤을 셀의 위치로 이동하고 셀의 크기로 조정할때 호출됩니다.
		/// </summary>
		/// <param name="viewControl">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="location">셀의 안쪽영역의 위치입니다.</param>
		/// <param name="size">셀의 안쪽영역의 크기입니다.</param>
		virtual void						SetViewControlLayout(TView viewControl, _Point location, _Size size);

		/// <summary>
		/// <see cref="Attaching"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="AttachEventArgs"/>입니다.</param>
		virtual void						OnAttaching(AttachEventArgs^ e);

		/// <summary>
		/// <see cref="Attached"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="AttachEventArgs"/>입니다.</param>
		virtual void						OnAttached(AttachEventArgs^ e);

		/// <summary>
		/// <see cref="Detaching"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="DetachEventArgs"/>입니다.</param>
		virtual void						OnDetaching(DetachEventArgs^ e);

		/// <summary>
		/// <see cref="Detached"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="DetachEventArgs"/>입니다.</param>
		virtual void						OnDetached(DetachEventArgs^ e);

	private: // methods
		virtual void SetControlLayout2(_Control^ control, _Point location, _Size size) sealed = IColumnControl::SetControlLayout
		{
			SetControlLayout(safe_cast<TEditing>(control), location, size);
		}

	private: // properties
		property _Control^ EditingControl2
		{ 
			virtual _Control^ get() sealed = IControlProvider::EditingControl::get { return m_control;  } 
		}

	private: // variables
		TEditing							m_control;
		TView								m_viewControl;

		AttachEventHandler^					m_eventAttaching;
		AttachEventHandler^					m_eventAttached;
		DetachEventHandler^					m_eventDetaching;
		DetachEventHandler^					m_eventDetached;

		Win32::ControlPainter^				m_controlPainter;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/