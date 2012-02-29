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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    /// <summary>
    /// TypeEditorForm에 대한 요약입니다.
    /// </summary>
    private ref class TypeEditorForm : System::Windows::Forms::Form,
        System::Windows::Forms::Design::IWindowsFormsEditorService, 
        System::IServiceProvider,
        System::ComponentModel::ITypeDescriptorContext,
        Design::IEditorService
    {
    public: // methods
        //TypeEditorForm(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

        TypeEditorForm(Ntreev::Windows::Forms::Grid::GridControl^ gridControl, ICell^ cell, EditingReason reason);

        virtual void ShowControl(System::Windows::Forms::Control^ control);
        virtual void DropDownControl(System::Windows::Forms::Control^ control);
        virtual void ShowDialog(System::Windows::Forms::Control^ control);

        void CancelEdit();

    public: // properties
        property bool IsDropDownResizable
        { 
            virtual void set(bool); 
        }

        property bool DefaultResult
        {
            virtual void set(bool);
        }

        property EditingReasonType ReasonType
        {
            virtual EditingReasonType get();
        }

        property System::Drawing::Rectangle Rectangle
        {
            void set(System::Drawing::Rectangle); 
        }

        property Ntreev::Windows::Forms::Grid::GridControl^ GridControl
        {
            Ntreev::Windows::Forms::Grid::GridControl^ get() { return m_gridControl; }
        }

    public: // events
        event System::EventHandler^ EscapeKeyPress;
        event System::EventHandler^ EnterKeyPress;
        event System::EventHandler^ TabKeyPress;

    protected: // methods
        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        ~TypeEditorForm();

        virtual void OnSizeChanged(System::EventArgs^ e) override;
        virtual void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override;
        virtual void OnMouseUp(System::Windows::Forms::MouseEventArgs^ e) override;
        virtual void OnMouseDown(System::Windows::Forms::MouseEventArgs^ e) override;
        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override;
        virtual void OnMouseLeave(System::EventArgs^ e) override;
        virtual void OnLayout(System::Windows::Forms::LayoutEventArgs^ levent) override;
        virtual void OnVisibleChanged(System::EventArgs^ e) override;
        virtual bool ProcessCmdKey(System::Windows::Forms::Message% msg, System::Windows::Forms::Keys keyData) override;
        virtual bool ProcessDialogKey(System::Windows::Forms::Keys keyData) override;
        //virtual void WndProc(System::Windows::Forms::Message% m) override;
        //virtual bool ProcessDialogKey(System::Windows::Forms::Keys keyData) override;
        //virtual void SetBoundsCore(int x, int y, int width, int height, System::Windows::Forms::BoundsSpecified specified) override
        //{
        //    Form::SetBoundsCore(x, y, width, height, specified);
        //}

    private: // methods
        System::Void InitializeComponent()
        {
            this->SuspendLayout();
            // 
            // TypeEditorForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(284, 262);
            this->ControlBox = false;
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"TypeEditorForm";
            this->ShowInTaskbar = false;
            this->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
            this->TopMost = true;
            this->ResumeLayout(false);

        }

        virtual void CloseDropDown() sealed = System::Windows::Forms::Design::IWindowsFormsEditorService::CloseDropDown;
        virtual System::Windows::Forms::DialogResult ShowDialog2(Form^ dialog) sealed = System::Windows::Forms::Design::IWindowsFormsEditorService::ShowDialog;
        virtual System::Object^ GetService_IServiceProvider(System::Type^ serviceType) sealed = System::IServiceProvider::GetService;
        virtual void OnComponentChanged() sealed = System::ComponentModel::ITypeDescriptorContext::OnComponentChanged;
        virtual bool OnComponentChanging() sealed = System::ComponentModel::ITypeDescriptorContext::OnComponentChanging;

        virtual void CloseEdit() sealed = Design::IEditorService::Close;

        void control_Resize(System::Object^ sender, System::EventArgs^ e);
        void PreProcessEvent(System::Windows::Forms::Control^ control);
        void PostProcessEvent(System::Windows::Forms::Control^ control);

		System::Drawing::Rectangle GetClipScreenRect(System::Windows::Forms::Control^ control);

    private: // properties
        property System::ComponentModel::IContainer^ Container2
        {
            virtual System::ComponentModel::IContainer^ get() sealed = System::ComponentModel::ITypeDescriptorContext::Container::get;
        }

        property System::Object^ Instance
        {
            virtual System::Object^ get() sealed = System::ComponentModel::ITypeDescriptorContext::Instance::get;
        }

        property System::ComponentModel::PropertyDescriptor^ PropertyDescriptor
        {
            virtual System::ComponentModel::PropertyDescriptor^ get() sealed = System::ComponentModel::ITypeDescriptorContext::PropertyDescriptor::get;
        }

    private: // variables
        bool                    m_dragging;
        bool					m_resizing;
        bool					m_resizable;
        bool					m_isin;
        Form^			        m_parentForm;
        Ntreev::Windows::Forms::Grid::GridControl^			m_gridControl;

        System::Drawing::Point			m_mouseDownPoint;
        System::Drawing::Point			m_mouseMovePoint;
        System::Drawing::Rectangle		m_resizableRect;
        System::Drawing::Size			m_firstSize;
        System::Drawing::Rectangle      m_bounds;
        System::Drawing::Rectangle      m_clientBounds;
        System::Windows::Forms::Control^m_control;
        System::Drawing::Size           m_controlSize;
        ICell^                  m_cell;
        EditingReason           m_reason;

        System::Windows::Forms::VisualStyles::VisualStyleRenderer^	m_gripperRenderer;
        bool                    m_closed;
        bool                    m_defaultResult;

        static const int m_paddingSize = 12;

    private:
        ref class MessageFilter : System::Windows::Forms::NativeWindow, System::Windows::Forms::IMessageFilter
        {
        public:	
            void Start(System::Windows::Forms::Form^ mainForm, TypeEditorForm^ dropDownForm);
            void Stop();

        protected:
            virtual void WndProc(System::Windows::Forms::Message% m) override;
        private:
            virtual bool PreFilterMessage(System::Windows::Forms::Message% m) sealed = System::Windows::Forms::IMessageFilter::PreFilterMessage;

        private: 
            TypeEditorForm^ m_dropDownForm;
        } m_filter;

    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/