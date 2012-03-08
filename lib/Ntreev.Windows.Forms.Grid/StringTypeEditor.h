#pragma once
#include "TypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    ref class StringTypeEditor : Ntreev::Windows::Forms::Grid::Design::TypeEditor
    {
    public: // methods

        StringTypeEditor();

        StringTypeEditor(System::Type^ dataType);

        virtual bool CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason) override;

        virtual System::Object^ EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

    public: // variables

        static StringTypeEditor Default;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/