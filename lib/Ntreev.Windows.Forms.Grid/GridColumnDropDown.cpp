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


#include "StdAfx.h"
#include "GridColumnDropDown.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    generic<class TControl> where TControl : System::Windows::Forms::Control
        ColumnDropDown<TControl>::ColumnDropDown()
    {
        m_control = CreateControlInstance(nullptr);
        OnControlCreated();
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        ColumnDropDown<TControl>::ColumnDropDown(... cli::array<System::Object^>^ controlArgs)
    {
        m_control = CreateControlInstance(controlArgs);
        OnControlCreated();
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        TControl ColumnDropDown<TControl>::CreateControlInstance(... cli::array<System::Object^>^ controlArgs)
    {
        if(controlArgs == nullptr)
            return safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid));
        return safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid, controlArgs));
    }

    //generic<class TControl> where TControl : System::Windows::Forms::Control
    //void ColumnDropDown<TControl>::OnAttaching(AttachEventArgs^ e)
    //{
    // Control->Dock    = System::Windows::Forms::DockStyle::Fill;
    // Control->Visible = true;
    // Attaching(this, e);
    //}

    //generic<class TControl> where TControl : System::Windows::Forms::Control
    //void ColumnDropDown<TControl>::InvokeAttaching(EditingReason by, System::Object^ value)
    //{
    // AttachEventArgs e(by, value);
    // OnAttaching(%e);
    //}

    //generic<class TControl> where TControl : System::Windows::Forms::Control
    //void ColumnDropDown<TControl>::OnAttached(AttachEventArgs^ e)
    //{
    // Attached(this, e);
    //}

    //generic<class TControl> where TControl : System::Windows::Forms::Control
    //void ColumnDropDown<TControl>::InvokeAttached(EditingReason by, System::Object^ value)
    //{
    // AttachEventArgs e(by, value);
    // OnAttached(%e);
    //}

    //generic<class TControl> where TControl : System::Windows::Forms::Control
    //void ColumnDropDown<TControl>::OnDetaching(DetachEventArgs^ e)
    //{
    // Detaching(this, e);
    //}

    //generic<class TControl> where TControl : System::Windows::Forms::Control
    //void ColumnDropDown<TControl>::InvokeDetaching(bool modified, System::Object^ value)
    //{
    // DetachEventArgs de(modified, value);
    // OnDetaching(%de);
    //}

    //generic<class TControl> where TControl : System::Windows::Forms::Control
    //void ColumnDropDown<TControl>::OnDetached(DetachEventArgs^ e)
    //{
    // Detached(this, e);
    //}

    //generic<class TControl> where TControl : System::Windows::Forms::Control
    //void ColumnDropDown<TControl>::InvokeDetached(bool modified, System::Object^ value)
    //{
    // DetachEventArgs de(modified, value);
    // OnDetached(%de);
    //}

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnDropDown<TControl>::OnControlCreated()
    {

    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        System::Object^ ColumnDropDown<TControl>::GetControlValue()
    {
        return nullptr;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnDropDown<TControl>::SetControlValue(System::Object^ /*value*/)
    {

    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnDropDown<TControl>::SetControlLayout(ICell^ cell)
    {
        if(m_control->BackColor != cell->PaintingBackColor)
            m_control->BackColor = cell->PaintingBackColor;
        if(m_control->ForeColor != cell->PaintingForeColor)
            m_control->ForeColor = cell->PaintingForeColor;
        if(m_control->Font != cell->Font)
            m_control->Font = cell->Font;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        System::Object^ ColumnDropDown<TControl>::EditValue(Design::IEditorService^ editorService, ICell^ cell, System::Object^ value)
    {
        SetControlLayout(cell);
        editorService->IsDropDownResizable = m_resizable;
        SetControlValue(value);
        m_editorService = editorService;
        editorService->DropDownControl(m_control);
        m_editorService = nullptr;
        return GetControlValue();
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        Design::EditStyle ColumnDropDown<TControl>::GetEditStyle()
    {
        return Design::EditStyle::DropDown;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnDropDown<TControl>::CloseDropDown()
    {
        if(m_editorService == nullptr)
            throw gcnew System::NotImplementedException();

        m_editorService->Close();
    }
}/*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/