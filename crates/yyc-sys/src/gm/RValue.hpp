#pragma once
#include "Common.hpp"
#include "RefThing.hpp"
#include "YYObjectBase.hpp"

#if 0
#include <string>
#endif

struct vec3
{
	float x, y, z;
};

struct vec4
{
	float x, y, z, w;
};

struct matrix44
{
	vec4 m[4];
};

enum class RValueKind : uint32_t
{
	REAL      = 0,
	STRING    = 1,
	ARRAY     = 2,
	PTR       = 3,
	VEC3      = 4,
	UNDEFINED = 5,
	OBJECT    = 6,
	INT32     = 7,
	VEC4      = 8,
	MATRIX44  = 9,
	INT64     = 10,
	ACCESSOR  = 11,
	NULL      = 12,
	BOOL      = 13,
	ITERATOR  = 14,
	REF       = 15,
	UNSET     = 0x00ffffff,
};

struct alignedTo(4) RValueFlags
{
  bool enumerable: 1 = false;
  bool configurable: 1 = false;
  bool writable: 1 = false;
  bool owned: 1 = false;
};
static_assert(sizeof(RValueFlags) == sizeof(uint32_t));

constexpr RValueFlags JS_BUILTIN_PROPERTY_DEFAULT = {
    .configurable = true,
    .writable = true,
};
constexpr RValueFlags JS_BUILTIN_LENGTH_PROPERTY_DEFAULT = {};

typedef YYObjectBase* (*GetCtxStackTop)(void);
typedef void (*DetPotRoot)(YYObjectBase* _pContainer, YYObjectBase* _yy_object_baseect);
//extern GetCtxStackTop GetContextStackTop;
//extern DetPotRoot DeterminePotentialRoot;

typedef void (*FREE_RVal_Pre)(RValue* p);
typedef void (*COPY_RValue_do__Post_t)(RValue* dest, RValue* src);
typedef void (*YYSetStr)(RValue* _pVal, const char* _pS);
typedef void (*YYCreStr)(RValue* _pVal, const char* _pS);
typedef char* (*YYDupStr)(const char* _pStr);
typedef RValue* (*ARRAYLVal)(RValue* _pV, int _index);

#if 0
#define YYC_DELETE(a) delete a
#endif

#pragma pack(push, 4)
struct DValue
{
	union {
		int64_t __64;
		void* __ptr;
		RefDynamicArrayOfRValue* __arr;
		YYObjectBase* __obj;
	};

	uint32_t flags;
	uint32_t type;
};
#pragma pack(pop)

#pragma pack(push, 4)
struct RValue
{
	union Value {
		// values.
		int32_t i32;
		int64_t i64;
		double f64;
        union Ptr {
		    // pointers.
		    void* opaque;
		    RefString* ref_string;
		    RefDynamicArrayOfRValue* ref_array;
		    YYObjectBase* object;
		    struct vec4* vec4;
		    struct matrix44* matrix44;
        } ptr;
	} value;
	// use for flags (Hijack for Enumerable and Configurable bits in JavaScript)
	RValueFlags flags;
	// kind of value
	RValueKind tag;

    #if 0
	void __localFree(void);
	void __localCopy(const RValue& v);

	~RValue();

	RValue();
	RValue(const RValue& v);

	RValue(double v);
	RValue(float v);
	RValue(int v);
	RValue(long long v);
	RValue(bool v);
	RValue(std::nullptr_t);
	RValue(std::nullptr_t, bool undefined);
	RValue(void* v);
	RValue(YYObjectBase* obj);
	RValue(RefDynamicArrayOfRValue* arr);
	RValue(const char* v);
	RValue(std::string v);
	RValue(std::wstring v);

	RValue operator-();
	RValue operator+();

	RValue& operator=(const RValue& v);
	RValue& operator=(double v);
	RValue& operator=(float v);
	RValue& operator=(int v);
	RValue& operator=(long long v);
	RValue& operator=(void* v);
	RValue& operator=(bool v);
	RValue& operator=(const char* v);
	RValue& operator=(std::string v);
	RValue& operator=(std::wstring v);

	RValue& operator++();
	RValue operator++(int);

	RValue& operator--();
	RValue operator--(int);

	bool operator==(const RValue& rhs) const;
	bool operator!=(const RValue& rhs) const;

	RValue* DoArrayIndex(const int _index);
	RValue* operator[](const int _index);

	std::string asString();
	double asReal() const;
	int asInt32() const;
	bool asBoolean() const;
	long long asInt64() const;
	void* asPointer() const;
	YYObjectBase* asObject() const;
	bool isNumber() const;
	bool isUnset() const;
	bool isArray() const;

	operator double() const;
	operator int() const;
	operator long long() const;
	operator bool() const;
	operator std::string();
	operator void*() const;
    #endif
};
#pragma pack(pop)

// struct DynamicArrayOfRValue
// {
// 	int32_t flags;
// 	RValue* array;
// };

struct RefDynamicArrayOfRValue : YYObjectBase
{
	int32_t ref_count;
	int32_t flags; // flag set = is readonly for example
	RValue* array;
	void* owner;
	int32_t visited;
	int32_t length;

    #if 0
	using value_type = RValue;
	using iterator   = RValue*;
	using reference  = RValue&;
	using size_type  = int;

	iterator begin()
	{
		return iterator(m_Array);
	}
	iterator end()
	{
		return iterator(m_Array + length);
	}
	size_type size() const noexcept
	{
		return length;
	}
	size_type max_size() const noexcept
	{
		return length;
	}
	bool empty() const noexcept
	{
		return length == 0;
	}
    #endif
};

#if 0
// 136
static constexpr auto RefDynamicArrayOfRValue_offset_ref_count = offsetof(RefDynamicArrayOfRValue, m_refCount);
// 144
static constexpr auto RefDynamicArrayOfRValue_offset_array = offsetof(RefDynamicArrayOfRValue, m_Array);
// 152
static constexpr auto RefDynamicArrayOfRValue_offset_owner = offsetof(RefDynamicArrayOfRValue, m_Owner);
// 164
static constexpr auto RefDynamicArrayOfRValue_offset_length = offsetof(RefDynamicArrayOfRValue, length);
#endif
