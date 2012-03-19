//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4461.30274
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
#include "stdafx.h"
#include "BooleanTypeEditor.h"
#include "CheckBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    BooleanTypeEditor::BooleanTypeEditor(System::Type^ dataType)
        : TypeEditor(dataType)
    {
        using namespace System::Windows::Forms::VisualStyles;

        try
        {
            if(System::Windows::Forms::Application::RenderWithVisualStyles == true)
            {
                m_rendererChecked = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::CheckedNormal);
                m_rendererCheckedHot = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::CheckedHot);
                m_rendererUnchecked = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::UncheckedNormal);
                m_rendererUncheckedHot = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::UncheckedHot);
            }
        }
        catch(System::Exception^)
        {

        }

        //System::Windows::Forms::Application::VisualStyleState
    }


    bool BooleanTypeEditor::CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason)
    {
        switch(reason.ReasonType)
        {
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Mouse:
            return cell->ClientRectangle.Contains(reason.Location);
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Key:
            {
                switch(reason.Key)
                {
                case System::Windows::Forms::Keys::Space:
                case System::Windows::Forms::Keys::Enter:
                    return true;
                default:
                    return false;
                }
            }
            break;
        }

        return TypeEditor::CanEdit(cell, reason);

    }

    System::Object^ BooleanTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        Ntreev::Windows::Forms::Grid::Design::Controls::CheckBox^ checkBox = gcnew Ntreev::Windows::Forms::Grid::Design::Controls::CheckBox(editorService);

        System::Drawing::Rectangle rect = cell->ClientRectangle;

        if(value != nullptr && value->ToString() == bool::TrueString)
            checkBox->Checked = true;
        else
            checkBox->Checked = false;

        checkBox->BackColor = cell->BackColor;
        checkBox->ForeColor = cell->ForeColor;
        checkBox->Font = cell->Font;
        checkBox->Bounds = rect;
        checkBox->Dock = System::Windows::Forms::DockStyle::Fill;

        editorService->ShowControl(checkBox);
        return checkBox->Checked;
    }

    void BooleanTypeEditor::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        bool checked = false;
        if(value == nullptr)
        {
            checked = false;
        }
        else if(value->GetType() == bool::typeid)
        {
            checked = (bool)value;
        }
        else
        {
            if(value->ToString() == System::Boolean::TrueString)
                checked = true;
            else
                checked = false;
        }

        if(checked == true)
        {
            if(m_rendererChecked != nullptr)
            {
                System::Drawing::Rectangle rectangle = paintRect;
                if(rectangle.Width % 2 == 1)
                    rectangle.Width--;
                if(rectangle.Height % 2 == 1)
                    rectangle.Height--;
                if((cell->State & CellState::Hot) == CellState::Hot)
                    m_rendererCheckedHot->DrawBackground(graphics, rectangle);
                else
                    m_rendererChecked->DrawBackground(graphics, rectangle);
            }
            else
            {
                int x1 = paintRect.Width/2 - 7 + paintRect.Left;
                int y1 = paintRect.Height/2 - 7 + paintRect.Top;
                System::Windows::Forms::ControlPaint::DrawCheckBox(graphics, x1, y1, 13, 13, System::Windows::Forms::ButtonState::Checked);
            }
        }
        else
        {
            if(m_rendererUnchecked != nullptr)
            {
                System::Drawing::Rectangle rectangle = paintRect;
                if(rectangle.Width % 2 == 1)
                    rectangle.Width--;
                if(rectangle.Height % 2 == 1)
                    rectangle.Height--;
                if((cell->State & CellState::Hot) == CellState::Hot)
                    m_rendererUncheckedHot->DrawBackground(graphics, rectangle);
                else
                    m_rendererUnchecked->DrawBackground(graphics, rectangle);
            }
            else
            {
                int x1 = paintRect.Width/2 - 7 + paintRect.Left;
                int y1 = paintRect.Height/2 - 7 + paintRect.Top;
                System::Windows::Forms::ControlPaint::DrawCheckBox(graphics, x1, y1, 13, 13, System::Windows::Forms::ButtonState::Normal);
            }
        }
    }

    Ntreev::Windows::Forms::Grid::ViewType BooleanTypeEditor::ViewType::get()
    {
        return Ntreev::Windows::Forms::Grid::ViewType::Custom; 
    }

} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/