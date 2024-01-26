#pragma once
#include "CommonInt.hpp"
#include "CommonWindows.hpp"

#ifdef _MSC_VER
#define alignedTo(a) __declspec(align(a))
#else //!MSC_VER
#define alignedTo(a) __attribute__((aligned(a)))
#endif //MSC_VER

enum class Color : int32_t;
enum class EJSRetValBool;
enum class EVariableType : int32_t;
enum class EventType : uint64_t;
enum class RValueKind : uint32_t;
enum class YYObjectBaseKind : int32_t;
struct CAPIVars;
struct CCode;
struct CFunctions;
struct CGlobals;
struct CInstance;
struct CInstanceBase;
struct CRoom;
struct CScript;
struct CScriptRef;
struct CStream;
struct CWeakRef;
struct DValue;
struct matrix44;
struct RefDynamicArrayOfRValue;
struct RToken;
struct RValue;
struct RVariableRoutine;
struct vec3;
struct vec4;
struct VMBuffer;
struct VMExec;
struct YYGMLFuncs;
struct YYObjectBase;
struct YYVAR;
template<typename Key, typename Value> struct CHashMap;
template<typename T> struct CDynamicArray;
template<typename T> struct CDynamicArrayRef;
template<typename T> struct RefThing;

typedef RefThing<const char*> RefString;

typedef void (*FNCodeFunctionGetTheFunction)(int id, char **bufName, void **bufRoutine, int *bufArgs, void *unused);
typedef bool (*FNCodeExecute)(YYObjectBase *Self, YYObjectBase *Other, CCode *code, RValue *res, int flags);
typedef void (__cdecl *PFUNC_YYGML)(CInstance *_self, CInstance *_other);
typedef RValue *(*PFUNC_YYGMLScript)(CInstance *_self, CInstance *_other, RValue *_return_value, int _argc, RValue **args);
typedef void (__cdecl *TRoutine)(RValue *_result, CInstance *_self, CInstance *_other, int _argc, RValue *_args);
