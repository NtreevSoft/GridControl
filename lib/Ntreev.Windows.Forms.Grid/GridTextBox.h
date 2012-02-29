#pragma once
#include "GridTypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    ref class GridTextBox : System::Windows::Forms::TextBox
    {
    public:
        GridTextBox(IEditorService^ editorService);

    protected:
        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;

    private:
        IEditorService^ m_editorService;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/