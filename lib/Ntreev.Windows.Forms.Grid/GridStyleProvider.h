//=====================================================================================================================
// Ntreev Grid for .Net 1.1.4324.22060
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

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid 
{
	/// <summary>
	/// 개체의 스타일 속성을 정의합니다.
	/// </summary>
	public interface class IStyle
	{
		/// <summary>
		/// 개체의 전경색을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 전경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
		/// </returns>
		property _Color ForeColor
		{
			_Color get();
		}

		/// <summary>
		/// 개체의 배경색을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 배경색을 나타내는 <see cref="System::Drawing::Color"/>입니다.
		/// </returns>
		property _Color BackColor
		{
			_Color get();
		}

		/// <summary>
		/// 개체의 글꼴을 가져옵니다.
		/// </summary>
		/// <returns>
		/// 글꼴색을 나타내는 <see cref="System::Drawing::Font"/>입니다.
		/// </returns>
		property _Font^ Font
		{
			_Font^ get();
		}
	};

	namespace Private
	{
		ref class RenderingStyle : IStyle
		{
		public:
			RenderingStyle()
			{

			}

			RenderingStyle(_Cell^ cell) : m_cell(cell)
			{

			}

			~RenderingStyle()
			{

			}

			!RenderingStyle()
			{

			}

			property _Color ForeColor
			{
				virtual _Color get();
			}

			property _Color BackColor
			{
				virtual _Color get();
			}

			property _Font^ Font
			{
				virtual _Font^ get();
			}

			property _Cell^ Cell
			{
				void set(_Cell^ value)
				{
					m_cell = value;
				}
			}
		private:
			_Cell^ m_cell;
		};
	}
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/