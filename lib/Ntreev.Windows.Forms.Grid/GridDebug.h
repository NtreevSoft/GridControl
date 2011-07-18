#pragma once

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Debug
{
	//generic<typename T>
	extern int WriteLine(object^ value);
	extern int WriteLine(string^ format, ... cli::array<object^>^ values);

	private enum class OutputFunctionState
	{

	};

	private ref class OutputFunction
	{
	public:
		OutputFunction(string^ function);
		~OutputFunction();
	private:
		string^	m_function;
		int		m_messageIndex;
	};
} /*namespace Debug*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/



