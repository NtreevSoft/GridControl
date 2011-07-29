﻿#pragma once
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