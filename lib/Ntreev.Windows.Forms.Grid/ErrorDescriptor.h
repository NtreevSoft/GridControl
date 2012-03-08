//=====================================================================================================================
// Ntreev Grid for .Net 2.0.0.0
// https://github.com/NtreevSoft/GridControl
// 
// Released under the MIT License.
// 
// Copyright (c) 2010 Ntreev Soft co., Ltd.
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
// persons to whom the Software is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
// Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//=====================================================================================================================


#pragma once
#include "GridObject.h"
#include "Events.h"
#include "Cell.h"
#include "Row.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class ErrorDescriptor : Ntreev::Windows::Forms::Grid::GridObject
    {
    public: // methods

        ErrorDescriptor(Ntreev::Windows::Forms::Grid::GridControl^ gridControl);

        void Paint(System::Drawing::Graphics^ g);
        void Add(Ntreev::Windows::Forms::Grid::Cell^ cell);
        void Remove(Ntreev::Windows::Forms::Grid::Cell^ cell);
        void Add(Ntreev::Windows::Forms::Grid::Row^ row);
        void Remove(Ntreev::Windows::Forms::Grid::Row^ row);

    private: // methods

        void errorTimer_Elapsed(System::Object^ sender, System::Timers::ElapsedEventArgs^ e);
        void gridControl_Cleared(System::Object^ sender, Ntreev::Windows::Forms::Grid::ClearEventArgs^ e);
        void gridControl_VisibleChanged(System::Object^ sender, System::EventArgs^ e);

    private: // variables

        System::Collections::Generic::List<Ntreev::Windows::Forms::Grid::Cell^>^ m_cells;
        System::Collections::Generic::List<Ntreev::Windows::Forms::Grid::Row^>^ m_rows;
        System::Timers::Timer^ m_timer;
        int m_errorCount;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/