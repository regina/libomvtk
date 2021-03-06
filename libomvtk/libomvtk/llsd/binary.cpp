#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable: 4702)
#	pragma warning(disable: 4512)
#endif

#include "binary.h"
#include "traits.h"
#include "../types/exceptions.h"
#include "../types/assert.h"

namespace omvtk
{
	namespace detail
	{
		namespace Binary
		{
			typedef Parser::iterator iterator;

			void ThrowParseError()
			{ 
				boost::throw_exception(SyntaxException("LLSD Binary ERROR"));
			} 

			template<typename ValT>
			inline ValT MakeVal(iterator iter)
			{
				return Poco::ByteOrder::fromNetwork(*reinterpret_cast<ValT const*>(&*iter));
			}

			template<typename Ret, typename Cont, typename Val>
			inline Ret MakePtr(iterator iter)
			{				
				return Ret(new Cont(MakeVal<Val>(iter)));
			}

			iterator ParseSizedImpl(iterator & iter, iterator end)
			{
				if(end - iter < 4)
				{
					ThrowParseError();
				}
				UInt32 len = MakeVal<UInt32>(iter);
				iter += 4;
				if(UInt32(end - iter) < len)
				{
					ThrowParseError();
				}
				return (iter += len);
			}

			template<LLSDValue::Types TypeID>
			inline void ParseSized(LLSDValue & val, iterator & iter, iterator end)
			{
				iterator start = iter;
				val.binary_decode(TypeID,start,ParseSizedImpl(iter,end));
			}

			template<typename T>
			inline T ParseSized(iterator & iter, iterator end)
			{
				iterator start = iter;
				return T(start,ParseSizedImpl(iter,end));
			}


			inline iterator Parse(iterator iter, iterator end, LLSDValue & val)
			{
				if(iter == end)
					return end;

				switch(*iter)
				{
				case '0':
				case '1':
					val = *iter == '1';
					return ++iter;
				case 'i':
					if(end - iter < 5)
					{
						ThrowParseError();		
					}
					val.binary_decode(LLSDValue::VT_INTEGER,iter+1,iter+5);
					return iter + 5;
				case 'r':		
					if(end - iter < 9)
					{
						ThrowParseError();
					}
					val.binary_decode(LLSDValue::VT_REAL,iter+1,iter+9);					
					return iter + 9;
				case 's':
					ParseSized<LLSDValue::VT_STRING>(val,++iter,end);
					return iter;
				case 'b':
					{						
						ParseSized<LLSDValue::VT_BINARY>(val,++iter,end);
						return iter;
					}					
				case 'u':
					if(end - iter < 17)
					{
						ThrowParseError();
					}					
					val.binary_decode(LLSDValue::VT_UUID,iter+1,iter+17);
					return iter + 17;
				case 'd':
					if(end - iter < 9)
					{
						ThrowParseError();
					}					
					val.binary_decode(LLSDValue::VT_DATE,iter+1,iter+9);
					return iter + 9;
				case 'l':
					{						
						ParseSized<LLSDValue::VT_URI>(val,++iter,end);
						return iter;
					}
				case '[':
					{
						if(end - iter < 6)
						{
							ThrowParseError();
						}
						LLSDValue arr;
						arr.binary_decode(LLSDValue::VT_ARRAY,byte_sub_range());
						UInt32 len = MakeVal<UInt32>(++iter);
						iter += 4;
						for(UInt32 i = 0; i < len; ++i)
						{
							if(iter == end)
							{
								ThrowParseError();
							}
							LLSDValue elem;								
							iter = Parse(iter, end, elem);							
							if(!elem)
							{
								ThrowParseError();
							}
							arr.array().push_back(elem);
						}
						if(iter == end || *iter != ']')
						{
							ThrowParseError();
						}
						val.swap(arr);
						return ++iter;
					}
				case '{':
					{
						if(end - iter < 6)
						{
							ThrowParseError();
						}

						LLSDValue map = LLSDValue::Map();
						map.binary_decode(LLSDValue::VT_MAP,byte_sub_range());

						UInt32 len = MakeVal<UInt32>(++iter);
						iter += 4;
						for(UInt32 i = 0; i < len; ++i)
						{
							if(iter == end)
								ThrowParseError();
							String k = ParseSized<String>(iter, end);														
							if(iter == end)
							{
								ThrowParseError();
							}
							LLSDValue elem;								
							iter = Parse(iter, end, elem);							
							if(!elem)
							{
								ThrowParseError();
							}
							map.map()[k] = elem;
						}
						if(iter == end || *iter != '}')
						{
							ThrowParseError();
						}
						val.swap(map);
						return ++iter;
					}
				case '!':
					val.clear();
					return ++iter;
				default:
					break;
				}

				ThrowParseError();
				// Fake. To silence the compilers but it will never be returned
				return iter; 
			}

			LLSDValue Parser::Parse(iterator iter, iterator end)
			{
				LLSDValue ptr;
				if(detail::Binary::Parse(iter, end, ptr) != end || !ptr)
				{
					ThrowParseError();
				}
				return ptr;
			}
		}
	}
}

#if _MSC_VER
#	pragma warning(pop)
#endif
