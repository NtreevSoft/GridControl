#include "StdAfx.h"
#include "GridDebug.h"

#define __USE_OUTPUT_FUNCTION

#ifdef _TIME_TEST
#define __USE_WRITE_LINE
#else
//#define __USE_WRITE_LINE
#endif

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Debug
{
	static int n=0;

	//generic<typename T>
#ifdef __USE_WRITE_LINE
	int WriteLine(object^ value)
#else
	int WriteLine(object^ /*value*/)
#endif
	{
#ifdef __USE_WRITE_LINE
		System::Console::Write("{0} : ", n);
		System::Console::WriteLine(value);
#endif
		return n++;
	}

#ifdef __USE_WRITE_LINE
	int WriteLine(string^ format, ... cli::array<object^>^ values)
#else
	int WriteLine(string^ /*format*/, ... cli::array<object^>^ /*values*/)
#endif
	{
#ifdef __USE_WRITE_LINE
		System::Console::Write("{0} : ", n);
		System::Console::WriteLine(format, values);
#endif
		return n++;
	}

	OutputFunction::OutputFunction(string^ function) : m_function(function)
	{
#ifdef __USE_OUTPUT_FUNCTION
		m_messageIndex = WriteLine(function);
#endif
	}

	OutputFunction::~OutputFunction()
	{
#ifdef __USE_OUTPUT_FUNCTION
		if(n == m_messageIndex + 1)
			WriteLine("{0} is end", m_function);
		else
			WriteLine("{0} is end({1})", m_function, m_messageIndex);
#endif
	}
} /*namespace Debug*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/