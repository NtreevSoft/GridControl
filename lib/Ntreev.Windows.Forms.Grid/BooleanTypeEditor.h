#pragma once
#include "TypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    ref class BooleanTypeEditor : Ntreev::Windows::Forms::Grid::Design::TypeEditor
    {
    public: // methods

        BooleanTypeEditor(System::Type^ dataType);

        virtual bool CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason) override;

        virtual System::Object^ EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

    public: // properties

        property Ntreev::Windows::Forms::Grid::ViewType ViewType
        {
            virtual Ntreev::Windows::Forms::Grid::ViewType get() override;
        }

    private: // variables

        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererChecked;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererCheckedHot;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererUnchecked;
        System::Windows::Forms::VisualStyles::VisualStyleRenderer^ m_rendererUncheckedHot;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/