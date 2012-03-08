#pragma once
#include "TypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    ref class FlagTypeEditor : Ntreev::Windows::Forms::Grid::Design::TypeEditor
    {
    public: // methods

        FlagTypeEditor(System::Type^ dataType);

        virtual System::Object^ EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

        virtual Ntreev::Windows::Forms::Grid::Design::EditStyle GetEditStyle() override;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/