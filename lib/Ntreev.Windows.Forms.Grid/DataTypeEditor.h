#pragma once
#include "TypeEditor.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    ref class DataTypeEditor : Ntreev::Windows::Forms::Grid::Design::TypeEditor
    {
    public: // methods

        DataTypeEditor(System::Type^ dataType);

        virtual System::Object^ EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

        virtual Ntreev::Windows::Forms::Grid::Design::EditStyle GetEditStyle() override;

        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value) override;

    public: // properties

        property Ntreev::Windows::Forms::Grid::ViewType ViewType
        {
            virtual Ntreev::Windows::Forms::Grid::ViewType get() override;
        }

    private: // variables

        System::Drawing::Design::UITypeEditor^ m_typeEditor;
    }; 
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/