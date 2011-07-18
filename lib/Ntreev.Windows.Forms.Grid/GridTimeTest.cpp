#include "StdAfx.h"
#ifdef _TIME_TEST

#include "GridTimeTest.h"
#include "GridDebug.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	GridTimeTest::GridTimeTest()
	{
		GridTimeTest::GridTimeTest("");
	}

	GridTimeTest::GridTimeTest(string^ text)
	{
		m_name = text;
		m_firstTime = m_oldTime = _DateTime::Now;
		Debug::WriteLine("{0} 시작", m_name);
	}

	GridTimeTest::~GridTimeTest()
	{
		this->!GridTimeTest();
	}

	GridTimeTest::!GridTimeTest()
	{
		ReportAbs(m_name + " 끝");
	}

	void GridTimeTest::Report(string^ text)
	{
		_DateTime time = _DateTime::Now;
		_TimeSpan span = time.Subtract(m_oldTime);
		m_oldTime = time;

		Debug::WriteLine("{0} : {1}", text, span);
	}

	void GridTimeTest::Report(string^ format, ... cli::array<object^>^ values)
	{
		Report(string::Format(format, values));
	}

	void GridTimeTest::ReportAbs(string^ text)
	{
		_DateTime time = _DateTime::Now;
		_TimeSpan span = time.Subtract(m_firstTime);

		Debug::WriteLine("{0} : 총시간 {1}", text, span);
	}

	void GridTimeTest::ReportAbs(string^ format, ... cli::array<object^>^ values)
	{
		ReportAbs(string::Format(format, values));
	}

} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/

#endif _TIME_TEST