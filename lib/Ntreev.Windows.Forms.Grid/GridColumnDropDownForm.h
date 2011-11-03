//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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
		ColumnDropDownForm(_GridControl^ gridControl);

		void Show(IColumnDropDown^ column, _Cell^ editingCell, _Rectangle rect);

	public: // properties
		property bool Sizable
		{ 
			void set(bool); 
		}
		
		property _Rectangle Rectangle
		{
			void set(_Rectangle); 
		}

		property _GridControl^ GridControl
		{
			_GridControl^ get() { return m_gridControl; }
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
		_GridControl^			m_gridControl;


		System::Windows::Forms::VisualStyles::VisualStyleRenderer^	visualRenderer;

	private:
		ref class MessageFilter : System::Windows::Forms::NativeWindow, System::Windows::Forms::IMessageFilter
		{
		public:	
			MessageFilter();
			virtual bool PreFilterMessage(System::Windows::Forms::Message% m);

		public:
			property ColumnDropDownForm^ DropDownForm
			{
				ColumnDropDownForm^ get() { return m_dropDownForm; }
				void set(ColumnDropDownForm^ value) { m_dropDownForm = value; }
			}
		public:
			virtual void WndProc(System::Windows::Forms::Message% m) override;

		private: 
			ColumnDropDownForm^ m_dropDownForm;
		}^ m_filter;
	};
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/