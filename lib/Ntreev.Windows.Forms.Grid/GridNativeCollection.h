//=====================================================================================================================
// Ntreev Grid for .Net 1.0.4300.26762
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

//#include <vector>
//#include <algorithm>

namespace Ntreev { namespace Windows { namespace Forms { namespace Grid { namespace Private
{
	template<typename T, typename S>
	class DefaultConverter
	{
	public:
		static S TtoS(T item)
		{
			return (S)item;
		}

		static T StoT(S item)
		{
			return (T)item;
		}
	};

	template<typename T, typename S, typename C = DefaultConverter<T, S> >
	ref class NativeCollection : System::Collections::Generic::ICollection<T>
	{
	public:
		typename typedef std::vector<S> _NativeType;
		NativeCollection()
		{
			m_internal = new _NativeType();
			m_attached = false;
		}

		NativeCollection(_NativeType* attached)
			: m_internal(attached)
		{
			m_attached = true;
		}

		~NativeCollection()
		{
			if(m_attached == false)
				delete m_internal;
			m_internal = nullptr;
		}

	public:
		virtual void	Add(T item)
		{
			if(m_internal->size() >= int::MaxValue)
				throw gcnew System::OverflowException("담을수 있는 범위를 넘어섰습니다.");
			m_internal->push_back(C::TtoS(item));
		}

		virtual void	Clear()
		{
			m_internal->clear();
		}

		virtual bool	Contains(T item)
		{
			_NativeType::const_iterator itor = std::find(m_internal->begin(), m_internal->end(), C::TtoS(item));
			return m_internal->end() == itor ? false : true;
		}

		virtual void	CopyTo(cli::array<T>^ array, int arrayIndex)
		{
			if(array == nullptr)
				throw gcnew System::ArgumentNullException("array");

			if(arrayIndex < 0)
				throw gcnew System::ArgumentOutOfRangeException("arrayIndex");

			if(arrayIndex + (int)m_internal->size() > array->Length)
				throw gcnew System::ArgumentException("대상 배열의 길이가 짧습니다. destIndex, length 및 배열의 하한을 확인하십시오.");

			_NativeType::const_iterator itor = m_internal->begin();
			int index = arrayIndex;
			while(itor != m_internal->end())
			{
				array[index] = C::StoT(*itor);
				index++;
				itor++;
			}
		}

		property int	Count
		{
			virtual  int get()
			{
				return (int)m_internal->size();
			}
		}

		property bool	IsReadOnly
		{
			virtual bool get()
			{
				return false;	
			}
		}

		virtual bool	Remove(T item)
		{
			_NativeType::const_iterator itor = std::find(m_internal->begin(), m_internal->end(), C::TtoS(item));
			if(m_internal->end() == itor)
				return false;
			m_internal->erase(itor);
			return true;
		}

		property T		default[int] 
		{ 
			T get(int index)
			{
				return C::StoT((*m_internal)[index]);
			} 
			void set(int index, T item)
			{
				(*m_internal)[index] = C::TtoS(item);
			}
		}

		ref class Enumerator : System::Collections::Generic::IEnumerator<T>
		{
		public:
			Enumerator(_NativeType* collections)
				: m_collections(collections)
			{
				Reset();	
			}

			~Enumerator()
			{

			}

			!Enumerator()
			{

			}

			virtual bool MoveNext()
			{
				m_index++;
				if(m_index > m_collections->size())
					return false;

				return true;
			}

			virtual void Reset()
			{
				m_index = 0;
			}

			virtual property T Current
			{
				T get()
				{
					return C::StoT((*m_collections)[m_index - 1]);
				}
			}

		private:
			virtual property System::Object^ Current2
			{
				System::Object^ get() sealed = System::Collections::IEnumerator::Current::get
				{
					return Current;
				}
			}

		private:
			_NativeType*		m_collections;
			unsigned int		m_index;
		};

		virtual System::Collections::Generic::IEnumerator<T>^ GetEnumerator()
		{
			return gcnew Enumerator(m_internal);
		}

	private:
		virtual System::Collections::IEnumerator^ GetEnumerator2() sealed = System::Collections::IEnumerable::GetEnumerator
		{
			return GetEnumerator();
		}

	private:
		_NativeType*			m_internal;
		bool					m_attached;
	};
} /*namespace Private*/ } /*namespace Grid*/ } /*namespace Forms*/ } /*namespace Windows*/ } /*namespace Ntreev*/