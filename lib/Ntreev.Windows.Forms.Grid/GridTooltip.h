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
#include "GridBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 툴팁 사용방법에 대한 메서드를 정의합니다.
	/// </summary>
	public interface class IToolTip
	{
		/// <summary>
		/// 툴팁을 표시합니다.
		/// </summary>
		/// <remarks>
		/// 툴팁의 내용은 현재 마우스 커서 기준으로 표시 됩니다.
		/// </remarks>
		/// <param name="text">툴팁에 표시할 문자열입니다.</param>
		void Show(string^ text);

		/// <summary>
		/// 툴팁을 감춥니다.
		/// </summary>
		void Hide();
	};

	namespace Private
	{
		private ref class ToolTip : GridObject
		{
		public:
			ToolTip(_GridControl^ gridControl);

			void				Show(string^ text);
			void				Hide();

		private:
			void*				m_tooltip;
			bool				m_created;
		};

		ref class GridTooltip : IToolTip, Rotator<Private::ToolTip^>
		{
		internal:
			GridTooltip(_GridControl^ gridControl, int count);
		public:
			virtual void Hide();
			virtual void Show(string^ text);

		private:
			bool	m_showed;
		};

	} /*namespace Private*/ 
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/