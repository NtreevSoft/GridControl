#pragma once
#include "GridTypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    ref class GridCheckBox : System::Windows::Forms::CheckBox
    {
    public:
        GridCheckBox();

    public:
        property IEditorService^ EditorService
        {
            void set(IEditorService^ value) { m_editorService = value; }
        }

    protected:
        virtual void OnCheckedChanged(System::EventArgs^ e) override;

    private:
        IEditorService^ m_editorService;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/