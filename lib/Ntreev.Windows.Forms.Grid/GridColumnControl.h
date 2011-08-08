#pragma once
#include "GridColumn.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 컨트롤을 이용해 편집할 수 있고 셀을 그릴 수 있는 기능을 제공합니다.
	/// </summary>
	/// <typeparam name="TControl"> 
	/// 편집할때 사용할 컨트롤의 타입입니다.
	/// </typeparam>
	generic<class TControl> where TControl : _Control
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
		property TControl EditingControl
		{
			TControl get() { return m_control; } 
		}

		/// <summary>
		/// 별도의 셀 렌더링 기능을 지원하는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <remarks>
		/// <see cref="ColumnControl"/>클래스의 인스턴스 생성시 이 속성이 true이면 셀을 표시하기 위한 별도의 컨트롤을 생성합니다.
		/// </remarks>
		property bool PaintValueSupported
		{
			virtual bool get() { return true; }
		}

	public: // events
		/// <summary>
		/// 셀 편집이 시작되고 컨트롤이 보여지기 전에 발생합니다.
		/// </summary>
		[_Description("셀 편집이 시작되고 컨트롤이 보여지기 전에 발생합니다.")]
		event AttachEventHandler<TControl>^ Attaching
		{
			void add(AttachEventHandler<TControl>^ p) { m_eventAttaching += p; }
			void remove(AttachEventHandler<TControl>^ p) { m_eventAttaching += p; }
		private:
			void raise(object^ sender, AttachEventArgs<TControl>^ e) { if(m_eventAttaching != nullptr) { m_eventAttaching->Invoke(sender, e); } }
		}

		/// <summary>
		/// 셀 편집이 시작되고 컨트롤이 보여진 후에 발생합니다.
		/// </summary>
		[_Description("셀 편집이 시작되고 컨트롤이 보여진 후에 발생합니다.")]
		event AttachEventHandler<TControl>^ Attached
		{
			void add(AttachEventHandler<TControl>^ p) { m_eventAttached += p; }
			void remove(AttachEventHandler<TControl>^ p) { m_eventAttached += p; }
		private:
			void raise(object^ sender, AttachEventArgs<TControl>^ e) { if(m_eventAttached != nullptr) { m_eventAttached->Invoke(sender, e); } }
		}

		/// <summary>
		/// 셀 편집이 종료되고 컨트롤이 감춰지기 전에 발생합니다.
		/// </summary>
		[_Description("셀 편집이 종료되고 컨트롤이 감춰지기 전에 발생합니다.")]
		event DetachEventHandler<TControl>^ Detaching
		{
			void add(DetachEventHandler<TControl>^ p) { m_eventDetaching += p; }
			void remove(DetachEventHandler<TControl>^ p) { m_eventDetaching += p; }
		private:
			void raise(object^ sender, DetachEventArgs<TControl>^ e) { if(m_eventDetaching != nullptr) { m_eventDetaching->Invoke(sender, e); } }
		}

		/// <summary>
		/// 셀 편집이 종료되고 컨트롤이 감춰진 후에 발생합니다.
		/// </summary>
		[_Description("셀 편집이 종료되고 컨트롤이 감춰진 후에 발생합니다.")]
		event DetachEventHandler<TControl>^ Detached
		{
			void add(DetachEventHandler<TControl>^ p) { m_eventDetached += p; }
			void remove(DetachEventHandler<TControl>^ p) { m_eventDetached += p; }
		private:
			void raise(object^ sender, DetachEventArgs<TControl>^ e) { if(m_eventDetached != nullptr) { m_eventDetached->Invoke(sender, e); } }
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
		/// 편집용 컨트롤이 생성된후에 호출됩니다.
		/// </summary>
		/// <param name="control">생성된 컨트롤의 인스턴스입니다.</param>
		virtual void						OnControlCreated(TControl control);

		/// <summary>
		/// <see cref="PaintValueSupported"/>의 값이 true일때 셀 표시용 컨트롤이 생성된후에 호출됩니다.
		/// </summary>
		/// <param name="control">생성된 컨트롤의 인스턴스입니다.</param>
		virtual void						OnViewControlCreated(TControl control);

		/// <summary>
		/// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
		/// </summary>
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
		virtual object^						GetEditingValue(TControl control) abstract;

		/// <summary>
		/// 셀의 값을 컨트롤에 적용할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
		virtual void						SetEditingValue(TControl control, object^ value) abstract;

		/// <summary>
		/// 컨트롤을 셀의 위치로 이동하고 셀의 크기로 조정할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집 또는 표시에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="location">셀의 안쪽영역의 위치입니다.</param>
		/// <param name="size">셀의 안쪽영역의 크기입니다.</param>
		virtual void						SetControlLayout(TControl control, _Point location, _Size size);

		/// <summary>
		/// <see cref="Attaching"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="AttachEventArgs"/>입니다.</param>
		virtual void						OnAttaching(AttachEventArgs<TControl>^ e);

		/// <summary>
		/// <see cref="Attached"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="AttachEventArgs"/>입니다.</param>
		virtual void						OnAttached(AttachEventArgs<TControl>^ e);

		/// <summary>
		/// <see cref="Detaching"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="DetachEventArgs"/>입니다.</param>
		virtual void						OnDetaching(DetachEventArgs<TControl>^ e);

		/// <summary>
		/// <see cref="Detached"/>이벤트를 발생시킵니다.
		/// </summary>
		/// <param name="e">이벤트 데이터가 들어있는 <see cref="DetachEventArgs"/>입니다.</param>
		virtual void						OnDetached(DetachEventArgs<TControl>^ e);

	private: // methods
		virtual void						SetControlLayout2(_Control^ control, _Point location, _Size size) sealed = IColumnControl::SetControlLayout
		{
			SetControlLayout(safe_cast<TControl>(control), location, size);
		}

	private: // properties
		property _Control^ EditingControl2
		{ 
			virtual _Control^ get() sealed = IColumnControl::EditingControl::get
			{
				return m_control; 
			} 
		}

	private: // variables
		TControl							m_control;
		TControl							m_viewControl;

		AttachEventHandler<TControl>^		m_eventAttaching;
		AttachEventHandler<TControl>^		m_eventAttached;
		DetachEventHandler<TControl>^		m_eventDetaching;
		DetachEventHandler<TControl>^		m_eventDetached;

		Win32::BitBlt^						m_controlPainter;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/