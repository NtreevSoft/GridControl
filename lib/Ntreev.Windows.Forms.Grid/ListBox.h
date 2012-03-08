#pragma once
#include "IEditorService.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design { namespace Controls
{
    ref class ListBox : public System::Windows::Forms::ListBox
    {
    public: // methods

        ListBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService);

        ListBox(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, System::Type^ dataType, System::Object^ value);

    public: // properties

        property System::Object^ Value
        {
            System::Object^ get();
        }

    protected: // methods

        virtual void OnMouseMove(System::Windows::Forms::MouseEventArgs^ e) override;

        virtual void OnMouseClick(System::Windows::Forms::MouseEventArgs^ e) override;

        virtual void OnKeyDown(System::Windows::Forms::KeyEventArgs^ e) override;

    private: // methods

        void UpdateValue();

    private: // variables

        Ntreev::Windows::Forms::Grid::Design::IEditorService^ m_editorService;
        System::Object^ m_value;
    };
} /*namespace Controls*/ } /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/