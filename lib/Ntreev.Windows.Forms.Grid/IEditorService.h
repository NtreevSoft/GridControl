#pragma once
#include "DesignTypes.h"
#include "EditingReason.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Design
{
    public interface class IEditorService : System::IServiceProvider
    {
        void ShowControl(System::Windows::Forms::Control^ control);

        void DropDownControl(System::Windows::Forms::Control^ control);

        void ShowDialog(System::Windows::Forms::Control^ control);

        void Close();

        property bool IsDropDownResizable
        {
            void set(bool);
        }

        property Ntreev::Windows::Forms::Grid::EditingReasonType ReasonType
        {
            Ntreev::Windows::Forms::Grid::EditingReasonType get();
        }
    };
} /*namespace Design*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/