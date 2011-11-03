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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	/// <summary>
	/// ColumnUITypeEditorForm에 대한 요약입니다.
	///
	/// 경고: 이 클래스의 이름을 변경하면 이 클래스가 의존하는
	///          모든 .resx 파일과 관련된 관리되는 리소스 컴파일러 도구의
	///          '리소스 파일 이름' 속성도 변경해야 합니다. 그렇지 않은 경우
	///          디자이너는 이 폼과 관련된 지역화된 리소스와
	///          올바르게 상호 작용할 수 없습니다.
	/// </summary>
	private ref class ColumnUITypeEditorForm : System::Windows::Forms::Form
	{
	public:
		ColumnUITypeEditorForm(System::Windows::Forms::Control^ control)
			: m_control(control)
		{
			InitializeComponent();
			//
			//TODO: 생성자 코드를 여기에 추가합니다.
			//

			//m_control->SizeChanged += gcnew _EventHandler(this, &ColumnUITypeEditorForm::control_SizeChanged);
		}

	protected:
		/// <summary>
		/// 사용 중인 모든 리소스를 정리합니다.
		/// </summary>
		~ColumnUITypeEditorForm()
		{
			if (components)
			{
				delete components;
			}
		}

		virtual void WndProc(System::Windows::Forms::Message% m) override
		{
			const int WM_NCACTIVATE = 0x086;
			System::IntPtr value(0x00000000);
            if (m.Msg == WM_NCACTIVATE && m.WParam == value)
            {
				Controls->Clear();
                Close();
            }

			Form::WndProc(m);
		}

	protected:
		//virtual void OnControlAdded(System::Windows::Forms::ControlEventArgs^ e) override
		//{
		//	Form::OnControlAdded(e);
		//	this->Size = m_control->Size;
		//}

		//virtual void OnShown(System::EventArgs^ e) override
		//{
		//	Form::OnShown(e);
		//	this->Size = m_control->Size;
		//}


	private:
		System::Windows::Forms::Control^ m_control;

	private:
		void control_SizeChanged(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
		{
			//int width = m_control->Width;
			//int height = m_control->Height;
			////this->Size = System::Drawing::Size(width, height);
			//this->Size = m_control->Size;
		}

	private:
		/// <summary>
		/// 필수 디자이너 변수입니다.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 디자이너 지원에 필요한 메서드입니다.
		/// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
		/// </summary>
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// ColumnUITypeEditorForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			//this->ClientSize = System::Drawing::Size(323, 445);
			this->ControlBox = false;
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ColumnUITypeEditorForm";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
			this->TopMost = true;
			this->ResumeLayout(false);

		}
#pragma endregion
	};
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/