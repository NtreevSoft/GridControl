#pragma once

#ifdef _TIME_TEST

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	public ref class GridTimeTest
	{
		typedef System::DateTime	_DateTime;
		typedef System::TimeSpan	_TimeSpan;
	public:
		GridTimeTest();
		GridTimeTest(string^ text);
		~GridTimeTest();
		!GridTimeTest();

		void				Report(string^ text);
		void				Report(string^ format, ... cli::array<object^>^ values);

		void				ReportAbs(string^ text);
		void				ReportAbs(string^ format, ... cli::array<object^>^ values);

	private:
		_DateTime			m_firstTime;
		_DateTime			m_oldTime;
		string^			m_name;
	};

} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/

#endif _TIME_TEST