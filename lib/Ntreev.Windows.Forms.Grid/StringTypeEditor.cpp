//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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
#include "StringTypeEditor.h"
#include "TextBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    StringTypeEditor::StringTypeEditor()
        : TypeEditor(System::String::typeid)
    {

    }

    StringTypeEditor::StringTypeEditor(System::Type^ dataType)
        : TypeEditor(dataType)
    {

    }

    bool StringTypeEditor::CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason)
    {
        switch(reason.ReasonType)
        {
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Ime:
            return true;
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Char:
            return true;
        default:
            return TypeEditor::CanEdit(cell, reason);
        }
    }

    System::Object^ StringTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        Ntreev::Windows::Forms::Grid::Design::Controls::TextBox^ textBox = gcnew Ntreev::Windows::Forms::Grid::Design::Controls::TextBox(editorService);
        System::ComponentModel::TypeConverter^ converter = cell->Column->TypeConverter;

        System::String^ oldText = System::String::Empty;
        
        switch(editorService->ReasonType)
        {
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Ime:
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Char:
            break;
        default:
            {
                if(value == nullptr || value == System::DBNull::Value)
                {
                    textBox->Text = System::String::Empty;
                }
                else
                {
                    textBox->Text = converter->ConvertToString(value);
                }
            }
            break;
        }

        oldText = textBox->Text;

        textBox->BackColor = cell->BackColor;
        textBox->ForeColor = cell->ForeColor;
        textBox->Font = cell->Font;
        textBox->Multiline = cell->Multiline;
        textBox->WordWrap = cell->WordWrap;

        if(textBox->Multiline == false)
        {
            textBox->Dock = System::Windows::Forms::DockStyle::Fill;
        }
        else
        {
            System::Drawing::Point pt = textBox->GetPositionFromCharIndex(0);

            System::Drawing::Rectangle rect = cell->ClientRectangle;
            rect.X = -pt.X;
            rect.Y = 0;
            rect.Width += pt.X;
            textBox->Bounds = rect;
        }

        editorService->ShowControl(textBox);

        if(editorService->Result == System::Windows::Forms::DialogResult::Cancel || textBox->Text == oldText)
            return value;

        return converter->ConvertFromString(textBox->Text);
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/