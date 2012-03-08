#pragma once
#include "stdafx.h"
#include "FlagTypeEditor.h"
#include "FlagControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    FlagTypeEditor::FlagTypeEditor(System::Type^ dataType) 
        : TypeEditor(dataType)
    {

    }

    System::Object^ FlagTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        Ntreev::Windows::Forms::Grid::Design::Controls::FlagControl^ flagControl = gcnew Ntreev::Windows::Forms::Grid::Design::Controls::FlagControl(editorService, this->DataType);

        flagControl->Value = value;
        flagControl->BackColor = cell->BackColor;
        flagControl->ForeColor = cell->ForeColor;
        flagControl->Font = cell->Font;

        editorService->DropDownControl(flagControl);
        return flagControl->Value;
    }

    Ntreev::Windows::Forms::Grid::Design::EditStyle FlagTypeEditor::GetEditStyle()
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::DropDown; 
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/