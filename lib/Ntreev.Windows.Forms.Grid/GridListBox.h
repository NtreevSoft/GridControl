#pragma once
#include "GridTypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    ref class GridListBox : public System::Windows::Forms::ListBox
    {
    public:
        GridListBox(IEditorService^ editorService);

        GridListBox(IEditorService^ editorService, System::Type^ dataType, System::Object^ value);

    public:
        property System::Object^ Value
        {
            System::Object^ get();
        }

    protected:
        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override;

        virtual void OnMouseClick(System::Windows::Forms::MouseEventArgs^ e) override;

        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;

    private:
        void UpdateValue();

    private:
        IEditorService^ m_editorService;
        System::Object^         m_value;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/