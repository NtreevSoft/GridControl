#pragma once
#include "stdafx.h"
#include "BindingListTypeEditor.h"
#include "GridControl.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    BindingListTypeEditor::BindingListTypeEditor(System::Type^ dataType)
        : TypeEditor(dataType)
    {

    }

    System::Object^ BindingListTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ /*cell*/, System::Object^ value)
    {
        Ntreev::Windows::Forms::Grid::GridControl^ gridControl = gcnew Ntreev::Windows::Forms::Grid::GridControl();
        gridControl->Dock = System::Windows::Forms::DockStyle::Fill;
        gridControl->DataSource = dynamic_cast<System::ComponentModel::IBindingList^>(value);

        editorService->ShowDialog(gridControl);
        return value;
    }

    Ntreev::Windows::Forms::Grid::Design::EditStyle BindingListTypeEditor::GetEditStyle() 
    {
        return Ntreev::Windows::Forms::Grid::Design::EditStyle::Modal; 
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/