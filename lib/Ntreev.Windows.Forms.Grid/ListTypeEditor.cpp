#include "stdafx.h"
#include "ListTypeEditor.h"
#include "ListBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    ListTypeEditor::ListTypeEditor(System::Type^ dataType)
        : TypeEditor(dataType)
    {

    }

    System::Object^ ListTypeEditor::EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value)
    {
        System::Drawing::Rectangle rect;
        Ntreev::Windows::Forms::Grid::Design::Controls::ListBox^ listBox = gcnew Ntreev::Windows::Forms::Grid::Design::Controls::ListBox(editorService, this->DataType, value);

        listBox->BackColor = cell->BackColor;
        listBox->ForeColor = cell->ForeColor;
        listBox->Font = cell->Font;

        listBox->Size = listBox->PreferredSize;

        editorService->DropDownControl(listBox);
        return listBox->SelectedItem;
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/