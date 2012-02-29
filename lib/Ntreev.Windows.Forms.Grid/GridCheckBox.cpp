#include "StdAfx.h"
#include "GridCheckBox.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    GridCheckBox::GridCheckBox()
    {
        this->CheckAlign = System::Drawing::ContentAlignment::MiddleCenter;
    }

    void GridCheckBox::OnCheckedChanged(System::EventArgs^ e)
    {
        CheckBox::OnCheckedChanged(e);
        if(m_editorService != nullptr)
        {
            m_editorService->Close();
        }
    }
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/