#pragma once
#include "stdafx.h"
#include "EnumTypeEditor.h"
#include "ListBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    EnumTypeEditor::EnumTypeEditor(System::Type^ dataType) 
        : TypeEditor(dataType)
    {

    }

    System::Object^ EnumTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        Ntreev::Windows::Forms::Grid::Design::Controls::ListBox^ listBox = gcnew Ntreev::Windows::Forms::Grid::Design::Controls::ListBox(editorService, this->DataType, value);

        listBox->BackColor = cell->BackColor;
        listBox->ForeColor = cell->ForeColor;
        listBox->Font = cell->Font;

        listBox->Size = listBox->PreferredSize;

        editorService->DropDownControl(listBox);
        return listBox->Value;;
    }

    Ntreev::Windows::Forms::Grid::Design::EditStyle EnumTypeEditor::GetEditStyle() 
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::DropDown;
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/