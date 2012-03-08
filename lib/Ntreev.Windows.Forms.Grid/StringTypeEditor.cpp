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

        switch(editorService->ReasonType)
        {
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Ime:
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Key:
        case Ntreev::Windows::Forms::Grid::EditingReasonType::Char:
            break;
        default:
            {
                if(value != nullptr)
                    textBox->Text = value->ToString();
            }
            break;
        }

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
        return textBox->Text;
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/