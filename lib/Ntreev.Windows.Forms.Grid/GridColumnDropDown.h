#pragma once
#include "GridColumn.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// DropDown형식에 컨트롤을 부착해 편집할 수 있는 기능을 제공합니다.
	/// </summary>
	/// <typeparam name="TControl"> 
	/// 편집할때 사용할 컨트롤의 타입입니다.
	/// </typeparam>
	generic<class TControl> where TControl : _Control
	public ref class ColumnDropDown abstract : Column, IColumnDropDown, IControlProvider
	{
	public: // methods
		/// <summary>
		/// 컨트롤을 생성하고 <see cref="ColumnDropDown"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		ColumnDropDown();

		/// <summary>
		/// <paramref name="controlArgs"/>를 사용하여 컨트롤을 생성하고 <see cref="ColumnDropDown"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		/// <remarks>
		/// 만약 컨트롤의 특수 생성자가 있을때에는 <paramref name="controlArgs"/>를 사용하여 필요한 인자를 넘길 수 있습니다.
		/// </remarks>
		ColumnDropDown(... cli::array<object^>^ controlArgs);

		/// <summary>
		/// 컨트롤이 DropDown에 부착되기전에 원하는 컨트롤의 크기를 조사하기 위해 호출됩니다.
		/// </summary>
		/// <param name="proposedSize">편집되는 셀의 크기입니다.</param>
		/// <returns>DropDown에 부착될 컨트롤의 크기입니다.</returns>
		virtual _Size GetPreferredSize(_Size proposedSize);


	public: // properties
		/// <summary>
		/// DropDown에 사용되는 form의 크기를 사용자가 조절할 수 있는지에 대한 여부를 가져옵니다.
		/// </summary>
		/// <returns>
		/// 사용자가 DropDown의 사용되는 form 크기를 조절할 수 있으면 true를 그렇지 않으면 false를 반환합니다.
		/// </returns>
		[_DefaultValue(false)]
		property bool Sizable
		{
			virtual bool get() { return m_sizable; }
			void set(bool value) { m_sizable = value; }
		}

		/// <summary>
		/// 셀을 편집하는 방식을 가져옵니다.
		/// </summary>
		property _EditingType	EditingType
		{
			virtual _EditingType get() override sealed { return _EditingType::DropDown; } 
		}

		/// <summary>
		/// 편집이 비정상적으로 종료가 되었을때 새로운 값을 갱신하는 기본 방법을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 비정상적인 편집 종료시 값의 갱신 여부를 확인하는 <see cref="_EditingResult"/>입니다.
		/// </returns>
		property _EditingResult DefaultEditingResult
		{
			virtual _EditingResult get() override { return _EditingResult::Cancel; } 
		}

		/// <summary>
		/// 셀이 편집될때 사용되는 컨트롤의 인스턴스를 가져옵니다.
		/// </summary>
		property TControl EditingControl
		{
			TControl get() { return m_control; }
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
		virtual object^						GetValue() sealed = IColumnDropDown::GetValue { return GetEditingValue(m_control); }
		virtual void						SetValue(object^ value) sealed = IColumnDropDown::SetValue { SetEditingValue(m_control, value); }

		virtual void						InvokeAttaching(EditingReason^ by, object^ value) = IColumnDropDown::InvokeAttaching;
		virtual void						InvokeAttached(EditingReason^ by, object^ value) = IColumnDropDown::InvokeAttached;
		virtual void						InvokeDetaching(bool modified, object^ value) = IColumnDropDown::InvokeDetaching;
		virtual void						InvokeDetached(bool modified, object^ value) = IColumnDropDown::InvokeDetached;

	protected: // methods
		/// <summary>
		/// 편집용 컨트롤이 생성된후에 호출됩니다.
		/// </summary>
		/// <param name="control">생성된 컨트롤의 인스턴스입니다.</param>
		virtual void						OnControlCreated(TControl control);

		/// <summary>
		/// 컨트롤에서 셀에 적용할 값을 가져올때 호출됩니다.
		/// </summary>
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <returns>셀에 적용할 값의 <see cref="System::Object"/>입니다.</returns>
		virtual object^						GetEditingValue(TControl control) abstract;

		/// <summary>
		/// 셀의 값을 컨트롤에 적용할때 호출됩니다.
		/// </summary>
		/// <param name="control">편집에 사용되는 컨트롤의 인스턴스입니다.</param>
		/// <param name="value">컨트롤에 적용할 셀의 값입니다.</param>
		virtual void						SetEditingValue(TControl control, object^ value) abstract;

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

	private: // properties
		property _Control^ EditingControl2
		{
			virtual _Control^ get() sealed = IControlProvider::EditingControl::get { return m_control; }
		}

	private:
		TControl						m_control;

		AttachEventHandler<TControl>^	m_eventAttaching;
		AttachEventHandler<TControl>^	m_eventAttached;
		DetachEventHandler<TControl>^	m_eventDetaching;
		DetachEventHandler<TControl>^	m_eventDetached;

		bool							m_sizable;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/