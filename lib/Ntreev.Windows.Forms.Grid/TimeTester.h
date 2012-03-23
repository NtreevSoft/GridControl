//=====================================================================================================================
// Ntreev Grid for .Net 2.0.4464.32161
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

#ifdef _TIME_TEST

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    ref class TimeTester
    {
    public: // methods

        TimeTester();
        TimeTester(System::String^ text);
        ~TimeTester();
        !TimeTester();

        void Report(System::String^ text);
        void Report(System::String^ format, ... cli::array<System::Object^>^ values);

        void ReportAbs(System::String^ text);
        void ReportAbs(System::String^ format, ... cli::array<System::Object^>^ values);

    private: // variables

        System::DateTime m_firstTime;
        System::DateTime m_oldTime;
        System::String^ m_name;
    };
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/

#endif _TIME_TEST