﻿#pragma once
#include "GridColumn.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	/// <summary>
	/// ColumnDropDownForm에 대한 요약입니다.
	/// </summary>
	private ref class ColumnDropDownForm : System::Windows::Forms::Form
	{
		typedef System::Windows::Forms::VisualStyles::VisualStyleRenderer	_VisualStyleRenderer;
		typedef System::Windows::Forms::VisualStyles::VisualStyleElement	_VisualStyleElement;
	public: // methods
		ColumnDropDownForm(GridControl^ gridControl);

		void Show(IColumnDropDown^ column, _Cell^ editingCell, _Rectangle rect);

	public:
		property bool Sizable
		{ 
			void set(bool); 
		}
		
		property _Rectangle Rectangle
		{
			void set(_Rectangle); 
		}

		property bool IsCursorIn
		{
			bool get() { return m_isin; }
		}

	protected: // methods
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~ColumnDropDownForm();

		virtual void OnSizeChanged(System::EventArgs^ e) override;
		virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override;
		virtual void OnMouseUp(System::Windows::Forms::MouseEventArgs^ e) override;
		virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) override;
		virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override;
		virtual void OnMouseLeave(System::EventArgs^ e) override;
		virtual void OnVisibleChanged(System::EventArgs^ e) override;

	private: // variables
		_Point					m_lButtonDownPoint;
		_Point					m_mouseMovePoint;
		_Rectangle				m_sizableRect;
		_Size					m_firstSize;
		bool					m_sizing;
		bool					m_sizable;
		bool					m_isin;
		GridControl^			m_gridControl;


		System::Windows::Forms::VisualStyles::VisualStyleRenderer^	visualRenderer;

	private:
		ref class MessageFilter : System::Windows::Forms::NativeWindow, System::Windows::Forms::IMessageFilter
		{
		public:	
			MessageFilter();
			virtual bool PreFilterMessage(System::Windows::Forms::Message% m);

		public:
			property System::Windows::Forms::Form^ DropDownForm
			{
				System::Windows::Forms::Form^ get() { return m_dropDownForm; }
				void set(System::Windows::Forms::Form^ value) { m_dropDownForm = value; }
			}
		public:
			virtual void WndProc(System::Windows::Forms::Message% m) override;

		private: 
			System::Windows::Forms::Form^ m_dropDownForm;
		}^ m_filter;
	};
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/