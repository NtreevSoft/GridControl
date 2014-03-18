//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5190.32793
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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    /// <summary>
    /// About에 대한 요약입니다.
    ///
    /// 경고: 이 클래스의 이름을 변경하면 이 클래스가 의존하는
    /// 모든 .resx 파일과 관련된 관리되는 리소스 컴파일러 도구의
    /// '리소스 파일 이름' 속성도 변경해야 합니다. 그렇지 않은 경우
    /// 디자이너는 이 폼과 관련된 지역화된 리소스와
    /// 올바르게 상호 작용할 수 없습니다.
    /// </summary>
    ref class About : System::Windows::Forms::Form
    {
    public:
        About(void)
        {
            InitializeComponent();
            //
            //TODO: 생성자 코드를 여기에 추가합니다.
            //
        }

    protected:
        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        ~About()
        {
            if (components)
            {
                delete components;
            }
        }
    private: System::Windows::Forms::Button^ button1;
    protected: 
    private: System::Windows::Forms::Label^ label1;

    protected: 

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
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->SuspendLayout();
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(108, 68);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(75, 23);
            this->button1->TabIndex = 0;
            this->button1->Text = L"OK";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &About::button1_Click);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(24, 30);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(255, 24);
            this->label1->TabIndex = 1;
            this->label1->Text = L"GridControl\r\nⓒ2011 NtreevSoft co.,Ltd All rights reserved.";
            this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
            // 
            // About
            // 
            this->AcceptButton = this->button1;
            this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(292, 103);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->button1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"About";
            this->ShowInTaskbar = false;
            this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
            this->Text = L"About";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void button1_Click(System::Object^ /*sender*/, System::EventArgs^ /*e*/) {
                 this->DialogResult = System::Windows::Forms::DialogResult::OK;
             }
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/