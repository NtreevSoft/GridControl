#pragma once
#include "IEditorService.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    ref class TextBox : System::Windows::Forms::TextBox
    {
    public: // methods

        TextBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService);

    protected: // methods

        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;

    private: // variables

        Ntreev::Windows::Forms::Grid::Design::IEditorService^ m_editorService;
    };
} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/