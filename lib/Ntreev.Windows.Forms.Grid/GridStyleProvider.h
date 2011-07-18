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