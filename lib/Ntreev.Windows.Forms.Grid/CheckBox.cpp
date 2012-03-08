#include "StdAfx.h"
#include "CheckBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    CheckBox::CheckBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService)
        : m_editorService(editorService)
    {
        this->CheckAlign = System::Drawing::ContentAlignment::MiddleCenter;
    }

    void CheckBox::OnCheckedChanged(System::EventArgs^ e)
    {
        System::Windows::Forms::CheckBox::OnCheckedChanged(e);
        if(m_editorService != nullptr)
        {
            m_editorService->Close();
        }
    }
} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/