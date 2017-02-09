﻿//=====================================================================================================================
// Ntreev Grid for .Net 2.0.5792.31442
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


#include "StdAfx.h"
#ifdef _TIME_TEST

#include "TimeTester.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
    TimeTester::TimeTester()
    {
        TimeTester::TimeTester("");
    }

    TimeTester::TimeTester(System::String^ text)
    {
        m_name = text;
        m_firstTime = m_oldTime = System::DateTime::Now;
        Debug::WriteLine("{0} 시작", m_name);
    }

    TimeTester::~TimeTester()
    {
        this->!TimeTester();
    }

    TimeTester::!TimeTester()
    {
        ReportAbs(m_name + " 끝");
    }

    void TimeTester::Report(System::String^ text)
    {
        System::DateTime time = System::DateTime::Now;
        System::TimeSpan span = time.Subtract(m_oldTime);
        m_oldTime = time;

        Debug::WriteLine("{0} : {1}", text, span);
    }

    void TimeTester::Report(System::String^ format, ... cli::array<System::Object^>^ values)
    {
        Report(System::String::Format(format, values));
    }

    void TimeTester::ReportAbs(System::String^ text)
    {
        System::DateTime time = System::DateTime::Now;
        System::TimeSpan span = time.Subtract(m_firstTime);

        Debug::WriteLine("{0} : 총시간 {1}", text, span);
    }

    void TimeTester::ReportAbs(System::String^ format, ... cli::array<System::Object^>^ values)
    {
        ReportAbs(System::String::Format(format, values));
    }

} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/

#endif _TIME_TEST