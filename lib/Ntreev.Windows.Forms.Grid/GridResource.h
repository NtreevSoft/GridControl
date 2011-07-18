#pragma once

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Properties
{
	private ref class Resources
	{
		static Resources()
		{
			m_resourceManager		= gcnew System::Resources::ResourceManager("GridControl.GridResource", System::Reflection::Assembly::GetExecutingAssembly());

			m_rowEditing			= dynamic_cast<_Bitmap^>(m_resourceManager->GetObject("RowEditing"));
			m_rowFocused			= dynamic_cast<_Bitmap^>(m_resourceManager->GetObject("RowFocused"));
			m_insertionRow			= dynamic_cast<_Bitmap^>(m_resourceManager->GetObject("InsertionRow"));
			m_insertionRowFocused	= dynamic_cast<_Bitmap^>(m_resourceManager->GetObject("InsertionRowFocused"));

			m_defaultColumnName 	= m_resourceManager->GetString("DefaultColumnName");
			m_defaultTooltip		= m_resourceManager->GetString("DefaultTooltip");
			m_setValueException 	= m_resourceManager->GetString("SetValueException");
			m_removeInsertionRowException= m_resourceManager->GetString("RemoveInsertionRowException");
			m_noColumnException		= m_resourceManager->GetString("NoColumnException");
			m_width					= m_resourceManager->GetString("Width");
			m_height				= m_resourceManager->GetString("Height");
		}
	public:
		static property _Bitmap^ RowEditing
		{
			_Bitmap^ get() { return m_rowEditing; } 
		}

		static property _Bitmap^ RowFocused
		{
			_Bitmap^ get() { return m_rowFocused; } 
		}

		static property _Bitmap^ InsertionRow
		{
			_Bitmap^ get() { return m_insertionRow; } 
		}

		static property _Bitmap^ InsertionRowFocused
		{
			_Bitmap^ get() { return m_insertionRowFocused; } 
		}

		static property string^	DefaultColumnName
		{ 
			string^ get() { return m_defaultColumnName; }
		}

		static property string^ DefaultTooltip
		{ 
			string^ get() { return m_defaultTooltip; }
		}

		static property string^ SetValueException
		{ 
			string^ get() { return m_setValueException; }
		}

		static property string^ RemoveInsertionRowException
		{
			string^ get() { return m_removeInsertionRowException; }
		}

		static property string^ NoColumnException 
		{ 
			string^ get() { return m_noColumnException; }
		}

		static property string^ Width
		{
			string^ get() { return m_width; }
		}

		static property string^ Height
		{
			string^ get() { return m_height; }
		}

	private:
		static System::Resources::ResourceManager^ m_resourceManager;

		static _Bitmap^			m_rowEditing;
		static _Bitmap^			m_rowFocused;
		static _Bitmap^			m_insertionRow;
		static _Bitmap^			m_insertionRowFocused;
	
		static string^			m_defaultColumnName;
		static string^			m_defaultTooltip;
		static string^			m_setValueException;
		static string^			m_removeInsertionRowException;
		static string^			m_noColumnException;

		static string^			m_width;
		static string^			m_height;
	};
} /*namespace Properties*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/

typedef Ntreev::Windows::Forms::Grid::Properties::Resources _Resources;
