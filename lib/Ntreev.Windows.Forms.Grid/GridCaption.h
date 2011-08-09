﻿#pragma once
#include "GridCellBase.h"

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid
{
	/// <summary>
	/// 제목을 표시하는 개체입니다.
	/// </summary>
	[System::ComponentModel::TypeConverter(System::ComponentModel::ExpandableObjectConverter::typeid)]
	public ref class CaptionRow : CellBase
	{
	public: // methods
		/// <summary>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>을 반환합니다.
		/// </summary>
		/// <returns>
		/// 현재 <see cref="System:Object"/>를 나타내는 <see cref="System::String"/>입니다.
		/// </returns>
		virtual string^ ToString() override
		{
			return Text; 
		}

	public: // properties
		/// <summary>
		/// 제목을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 제목을 나타내는 문자열입니다.
		/// </returns>
		[_Category("Layout")]
		property string^ Text
		{
			string^ get();
			void set(string^);
		}

		/// <summary>
		/// 제목의 수평 정렬방식을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 수평 정렬방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
		/// </returns>
		[_Category("Layout"), _DefaultValue(_StringAlignment::Near)]
		property _StringAlignment Alignment
		{
			_StringAlignment get(); 
			void set(_StringAlignment);
		}

		/// <summary>
		/// 제목의 수직 정렬방식을 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 수직 정렬방식을 나타내는 <see cref="System::Drawing::StringAlignment"/>입니다.
		/// </returns>
		[_Category("Layout"), _DefaultValue(_StringAlignment::Center)]
		property _StringAlignment LineAlignment
		{
			_StringAlignment get();
			void set(_StringAlignment);
		}

		/// <summary>
		/// 제목의 표시 여부를 가져오거나 설정합니다.
		/// </summary>
		/// <returns>
		/// 제목이 표시되면 true를, 그렇지 않으면 false를 반환합니다.
		/// </returns>
		[_Category("Appearance"), _DefaultValue(true)]
		property bool IsVisible
		{
			bool get(); 
			void set(bool); 
		}

		/// <summary>
		/// 제목의 높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MinHeight"/>보다 작거나 <see cref="MaxHeight"/>보다 클경우
		/// </exception>
		[_Description("제목의 높이입니다.")]
		[_Category("Layout")]
		property int Height
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 제목의 최소높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MaxHeight"/>보다 클경우
		/// </exception>
		[_Description("제목의 최소높이입니다.")]
		[_Category("Layout")]
		property int MinHeight
		{
			int get();
			void set(int);
		}

		/// <summary>
		/// 제목의 최대높이를 가져오거나 설정합니다.
		/// </summary>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 0보다 작을 경우
		/// </exception>
		/// <exception cref="System::ArgumentOutOfRangeException">
		/// 매개변수가 <see cref="MinHeight"/>보다 작을경우
		/// </exception>
		[_Description("제목의 최대높이입니다.")]
		[_Category("Layout")]
		property int MaxHeight
		{
			int get();
			void set(int);
		}

	internal: // methods
		CaptionRow(_GridControl^ gridControl, GrCaption* pCaption);

		static CaptionRow^ FromNative(GrCaption* pCaption);

	private: // methods
		bool ShouldSerializeHeight();
		bool ShouldSerializeMinHeight();
		bool ShouldSerializeMaxHeight();


	private: // variables
		GrCaption*	m_pCaption;
	};
} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/