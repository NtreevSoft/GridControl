#pragma once
#include "ICell.h"
#include "IEditorService.h"
#include "EditingReason.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    public ref class TypeEditor abstract
    {
    public: // methods

        TypeEditor(System::Type^ dataType);

        virtual bool CanEdit(Ntreev::Windows::Forms::Grid::ICell^ cell, Ntreev::Windows::Forms::Grid::EditingReason reason);

        virtual System::Object^ EditValue(Ntreev::Windows::Forms::Grid::Design::IEditorService^ editorService, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value);

        virtual void PaintValue(System::Drawing::Graphics^ graphics, System::Drawing::Rectangle paintRect, Ntreev::Windows::Forms::Grid::ICell^ cell, System::Object^ value);

        virtual Ntreev::Windows::Forms::Grid::Design::EditStyle GetEditStyle();

        static Ntreev::Windows::Forms::Grid::Design::TypeEditor^ FromDataType(System::Type^ dataType);

    public: // properties

        property Ntreev::Windows::Forms::Grid::ViewType ViewType
        {
            virtual Ntreev::Windows::Forms::Grid::ViewType get();
        }

    protected: // properties

        property System::Type^ DataType
        {
            System::Type^ get();
        }

    private: // variables

        System::Type^ m_dataType;
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/