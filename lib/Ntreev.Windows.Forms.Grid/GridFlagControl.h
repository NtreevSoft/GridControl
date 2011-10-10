﻿//=====================================================================================================================
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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
	/// <summary>
	/// 열거형을 비트 필드 형태를 나타내는 사용자 정의 컨트롤입니다.
	/// </summary>
	public ref class FlagControl : public System::Windows::Forms::UserControl
	{
	public:
		/// <summary>
		/// <see cref="FlagControl"/>클래스의 새 인스턴스를 초기화합니다.
		/// </summary>
		FlagControl()
		{
			InitializeComponent();
		}

	public:
		/// <summary>
		/// 열거형 비트 필드 형태의 값을 가져오거나 설정합니다.
		/// </summary>
		property int Value
		{
			int get();
			void set(int value);
		}

		/// <summary>
		/// 열거형 비트 필드 형태의 타입을 설정합니다.
		/// </summary>
		property _Type^ FlagType
		{
			void set(_Type^ value);
		}

	public:
		event System::EventHandler^ EditOK;

		event System::EventHandler^ EditCanceled;

	protected:
		~FlagControl()
		{
			if (components)
			{
				delete components;
			}
		}

		virtual bool ProcessCmdKey(System::Windows::Forms::Message% msg, System::Windows::Forms::Keys keyData) override;

	private:
		_Type^			m_flagType;
		int				m_oldValue;

	private: 
		System::Windows::Forms::Button^  buttonOk;
		System::Windows::Forms::Button^  buttonCancel;

	private:

		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code

		void InitializeComponent(void)
		{
			this->buttonOk = (gcnew System::Windows::Forms::Button());
			this->buttonCancel = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// buttonOk
			// 
			this->buttonOk->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->buttonOk->Location = System::Drawing::Point(8, 163);
			this->buttonOk->Name = L"buttonOk";
			this->buttonOk->Size = System::Drawing::Size(55, 23);
			this->buttonOk->TabIndex = 0;
			this->buttonOk->Text = L"&Ok";
			this->buttonOk->UseVisualStyleBackColor = true;
			this->buttonOk->Click += gcnew System::EventHandler(this, &FlagControl::buttonOk_Click);
			// 
			// buttonCancel
			// 
			this->buttonCancel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->buttonCancel->Location = System::Drawing::Point(102, 163);
			this->buttonCancel->Name = L"buttonCancel";
			this->buttonCancel->Size = System::Drawing::Size(55, 23);
			this->buttonCancel->TabIndex = 1;
			this->buttonCancel->Text = L"&Cancle";
			this->buttonCancel->UseVisualStyleBackColor = true;
			this->buttonCancel->Click += gcnew System::EventHandler(this, &FlagControl::buttonCancel_Click);
			// 
			// FlagControl
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->Controls->Add(this->buttonCancel);
			this->Controls->Add(this->buttonOk);
			this->Name = L"FlagControl";
			this->Padding = System::Windows::Forms::Padding(5);
			this->Size = System::Drawing::Size(165, 194);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: 
		System::Void buttonOk_Click(System::Object^ sender, System::EventArgs^ e) ;
		System::Void buttonCancel_Click(System::Object^ sender, System::EventArgs^ e) ;
	};
} /*namespace Columns*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/