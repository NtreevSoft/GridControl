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
#include "ColumnCheckBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Columns
{
    ColumnCheckBox::ColumnCheckBox()
    {
        using namespace System::Windows::Forms::VisualStyles;

        this->Control->CheckAlign = System::Drawing::ContentAlignment::MiddleCenter;
        this->Control->CheckedChanged += gcnew System::EventHandler(this, &ColumnCheckBox::checkBox_CheckedChanged);

        try
        {
            m_rendererChecked = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::CheckedNormal);
            m_rendererUnchecked = gcnew VisualStyleRenderer(VisualStyleElement::Button::CheckBox::UncheckedNormal);
        }
        catch(System::Exception^)
        {

        }
    }

    void ColumnCheckBox::checkBox_CheckedChanged(System::Object^ /*sender*/, System::EventArgs^ /*e*/)
    {

    }

    void ColumnCheckBox::SetControlValue(System::Windows::Forms::CheckBox^ control, System::Object^ value)
    {
        if(value == nullptr)
        {
            control->Checked = false;
            return;
        }

        if(value->GetType() == bool::typeid)
            control->Checked = (bool)value;
        else
            control->Checked = value->ToString() == System::Boolean::TrueString;
    }

    System::Object^ ColumnCheckBox::GetControlValue(System::Windows::Forms::CheckBox^ control)
    {
        if(DataType == bool::typeid)
        {
            return control->Checked;
        }
        return control->Checked.ToString();
    }

    void ColumnCheckBox::PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ /*style*/, System::Object^ value)
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

                m_rendererChecked->DrawBackground(graphics, rectangle);
            }
            else
            {
                int x1 = paintRect.Width/2 - 6 + paintRect.Left;
                int y1 = paintRect.Height/2 - 6 + paintRect.Top;
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
                m_rendererUnchecked->DrawBackground(graphics, rectangle);
            }
            else
            {
                int x1 = paintRect.Width/2 - 6 + paintRect.Left;
                int y1 = paintRect.Height/2 - 6 + paintRect.Top;
                System::Windows::Forms::ControlPaint::DrawCheckBox(graphics, x1, y1, 13, 13, System::Windows::Forms::ButtonState::Normal);
            }
        }
    }
} /*namespace Extensiton*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/