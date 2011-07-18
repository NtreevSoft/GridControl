#pragma once

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Win32
{
	ref class API
	{
	public:
		static System::IntPtr	SendMessage(System::IntPtr handle, int message, System::IntPtr wParam, System::IntPtr lParam);
		static System::IntPtr	PostMessage(System::IntPtr handle, int message, System::IntPtr wParam, System::IntPtr lParam);

		static int				HiWord(int l);
		static int				LoWord(int l);

		static System::IntPtr	MakeLParam(int l, int h);

		static System::Char		ImmGetVirtualKey(System::IntPtr handle);

		static bool				ScrollWindow(System::IntPtr handle, int XAmount, int YAmount, System::Drawing::Rectangle^ rect, System::Drawing::Rectangle^ clipRect);

		static void				InvokeLButtonDownEvent(System::Drawing::Point location);


		static const int WM_CREATE			= 0x0001;
		static const int WM_DESTROY			= 0x0002;
		static const int WM_HSCROLL			= 0x0114;
		static const int WM_VSCROLL			= 0x0115;

		static const int WM_MOUSEACTIVATE	= 0x0021;
		static const int WM_LBUTTONDOWN		= 0x0201;
		static const int WM_LBUTTONUP		= 0x0202;
		static const int WM_LBUTTONDBLCLK	= 0x0203;
		static const int WM_RBUTTONDOWN		= 0x0204;
		static const int WM_RBUTTONUP		= 0x0205;
		static const int WM_RBUTTONDBLCLK	= 0x0206;
		static const int WM_MBUTTONDOWN		= 0x0207;
		static const int WM_MBUTTONUP		= 0x0208;
		static const int WM_MBUTTONDBLCLK	= 0x0209;

		static const int WM_NCACTIVATE		= 0x0086;
		static const int WM_ACTIVATEAPP		= 0x001C;

		static const int WM_NCMOUSEMOVE		= 0x00A0;
		static const int WM_NCLBUTTONDOWN	= 0x00A1;
		static const int WM_NCLBUTTONUP		= 0x00A2;
		static const int WM_NCLBUTTONDBLCLK	= 0x00A3;
		static const int WM_NCRBUTTONDOWN	= 0x00A4;
		static const int WM_NCRBUTTONUP		= 0x00A5;
		static const int WM_NCRBUTTONDBLCLK	= 0x00A6;
		static const int WM_NCMBUTTONDOWN	= 0x00A7;
		static const int WM_NCMBUTTONUP		= 0x00A8;
		static const int WM_NCMBUTTONDBLCLK	= 0x00A9;
	};

	ref class BitBlt
	{
		value struct Key
		{
			Key(_Size% size, object^ value)
				: size(size), value(value)
			{

			}
			
			_Size		size;
			object^		value;
		};

		ref class Image
		{
		public:
			void*	m_dc;
		};
	public:
		BitBlt();

		virtual void Do(System::IntPtr hdc, _Control^ control, _Rectangle% renderRect, object^ value);

	private:
		void*							m_dc;
		int								m_width;
		int								m_height;

		System::Collections::Generic::Dictionary<Key, Image^> m_images;
	};
} /*namespace Win32*/} /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/