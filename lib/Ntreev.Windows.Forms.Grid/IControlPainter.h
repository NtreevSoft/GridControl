#pragma once

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    interface class IControlPainter
    {
        void BitBlt(System::IntPtr hdc, System::Windows::Forms::Control^ control, System::Drawing::Rectangle paintRect, System::Object^ value);
        void TransparentBlt(System::IntPtr hdc, System::Windows::Forms::Control^ control, System::Drawing::Rectangle paintRect, System::Object^ value);
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/