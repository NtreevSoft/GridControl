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


#include "StdAfx.h"
#include "ColumnDropDown.h"

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
        bool ColumnDropDown<TControl>::Resizable::get()
    {
        return m_resizable;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnDropDown<TControl>::Resizable::set(bool value)
    {
        m_resizable = value;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        TControl ColumnDropDown<TControl>::Control::get()
    {
        return m_control; 
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        TControl ColumnDropDown<TControl>::CreateControlInstance(... cli::array<System::Object^>^ controlArgs)
    {
        if(controlArgs == nullptr)
            return safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid));
        return safe_cast<TControl>(System::Activator::CreateInstance(TControl::typeid, controlArgs));
    }

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
        void ColumnDropDown<TControl>::SetControlLayout(Ntreev::Windows::Forms::Grid::ICell^ cell)
    {
        if(m_control->BackColor != cell->PaintingBackColor)
            m_control->BackColor = cell->PaintingBackColor;
        if(m_control->ForeColor != cell->PaintingForeColor)
            m_control->ForeColor = cell->PaintingForeColor;
        if(m_control->Font != cell->Font)
            m_control->Font = cell->Font;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        System::Object^ ColumnDropDown<TControl>::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
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
        Ntreev::Windows::Forms::Grid::Design::EditStyle ColumnDropDown<TControl>::GetEditStyle()
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::DropDown;
    }

    generic<class TControl> where TControl : System::Windows::Forms::Control
        void ColumnDropDown<TControl>::CloseDropDown()
    {
        if(m_editorService == nullptr)
            return;

        m_editorService->Close();
    }
}/*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/