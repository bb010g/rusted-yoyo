#include <cstdint>

typedef struct vec3 {
    float x, y, z;
} vec3;

typedef struct vec4 {
    float x, y, z, w;
} vec4;

typedef struct matrix44 {
    vec4 m[4];
} matrix44;

template<typename T>
struct _RefThing {
    T m_thing;
    int m_refCount;
    int m_size;
};

typedef _RefThing<const char *> RefString;

struct RefDynamicArrayOfRValue;

struct YYObjectBase;
typedef struct YYObjectBase YYObjectBase;

typedef struct __attribute__((packed, aligned(4))) DValue {
    union {
        int64_t __64;
        void *__ptr;
        RefDynamicArrayOfRValue *__arr;
        YYObjectBase *__obj;
    } value;

    unsigned int flags;
    unsigned int type;
} DValue;

constexpr int MASK_TYPE_RVALUE = 0x0ffffff;
enum RValueType : int {
    REAL = 0,
    STRING = 1,
    ARRAY = 2,
    PTR = 3,
    VEC3 = 4,
    UNDEFINED = 5,
    OBJECT = 6,
    _INT32 = 7,
    VEC4 = 8,
    MATRIX = 9,
    _INT64 = 10,
    ACCESSOR = 11,
    JSNULL = 12,
    _BOOL = 13,
    ITERATOR = 14,
    REF = 15,
    UNSET = MASK_TYPE_RVALUE,
};

struct RefDynamicArrayOfRValue;

#pragma pack(push, 4)
typedef struct RValue {
    union {
        // values.
        int i32;
        long long i64;
        double value;

        // pointers.
        void *ptr;
        RefString *ref_string;
        RefDynamicArrayOfRValue *ref_array;
        YYObjectBase *yy_object_base;
        vec4 *vec4;
        matrix44 *matrix44;
    };
    // use for flags (Hijack for Enumerable and Configurable bits in JavaScript)
    int flags;
    // kind of value
    RValueType type;
} RValue;
#pragma pack(pop)

enum class EJSRetValBool {
    EJSRVB_FALSE,
    EJSRVB_TRUE,
    EJSRVB_TYPE_ERROR
};

template<typename Key, typename Value>
struct CHashMap {
	int m_curSize;
	int m_numUsed;
	int m_curMask;
	int m_growThreshold;
	struct CElement {
		Value v;
		Key k;
		unsigned int Hash;
	}* m_pBuckets;
};

struct CInstanceBase {
    RValue *yyvars;
    virtual ~CInstanceBase() = 0;

    virtual RValue &InternalGetYYVarRef(int index) = 0;
    virtual RValue &InternalGetYYVarRefL(int index) = 0;
};

enum class YYObjectBaseKind : int {
    YYOBJECTBASE = 0,
    CINSTANCE,
    ACCESSOR,
    SCRIPTREF,
    PROPERTY,
    ARRAY,
    WEAKREF,

    CONTAINER,

    SEQUENCE,
    SEQUENCEINSTANCE,
    SEQUENCETRACK,
    SEQUENCECURVE,
    SEQUENCECURVECHANNEL,
    SEQUENCECURVEPOINT,
    SEQUENCEKEYFRAMESTORE,
    SEQUENCEKEYFRAME,
    SEQUENCEKEYFRAMEDATA,
    SEQUENCEEVALTREE,
    SEQUENCEEVALNODE,
    SEQUENCEEVENT,

    NINESLICE,

    MAX
};

struct CWeakRef;

struct YYObjectBase : CInstanceBase {
    virtual bool Mark4GC(unsigned int *_pM, int _numObjects) = 0;
    virtual bool MarkThisOnly4GC(unsigned int *_pM, int _numObjects) = 0;
    virtual bool MarkOnlyChildren4GC(unsigned int *_pM, int _numObjects) = 0;
    virtual void Free(bool preserve_map) = 0;
    virtual void ThreadFree(bool preserve_map, void *_pGCContext) = 0;
    virtual void PreFree() = 0;

    YYObjectBase *m_pNextObject;
    YYObjectBase *m_pPrevObject;
    YYObjectBase *m_prototype;
    //void *m_pcre;
    //void *m_pcreExtra;
    const char *m_class;
    void (*m_getOwnProperty)(YYObjectBase *obj, RValue *val, const char *name);
    void (*m_deleteProperty)(YYObjectBase *obj, RValue *val, const char *name, bool dothrow);
    EJSRetValBool (*m_defineOwnProperty)(YYObjectBase *obj, const char *name, RValue *val, bool dothrow);
    CHashMap<int, RValue *> *m_yyvarsMap;
    CWeakRef **m_pWeakRefs;
    unsigned int m_numWeakRefs;
    unsigned int m_nvars;
    unsigned int m_flags;
    unsigned int m_capacity;
    unsigned int m_visited;
    unsigned int m_visitedGC;
    int m_GCgen;
    int m_GCcreationframe;
    int m_slot; // offset 30(int) | 120(byte)
    YYObjectBaseKind type; // m_kind
    int m_rvalueInitType;
    int m_curSlot;
};

struct CWeakRef : YYObjectBase {
    YYObjectBase *pWeakRef;
};

struct CCode;

struct CInstance : YYObjectBase {
    int64_t m_CreateCounter;
    void *m_pObject;
    void *m_pPhysicsObject;
    void *m_pSkeletonAnimation;
    void *m_pControllingSeqInst;
    unsigned int m_Instflags;
    int id;
    int object_index;
    int sprite_index;
    float i_sequencePos;
    float i_lastSequencePos;
    float i_sequenceDir;
    float image_index;
    float image_speed;
    float image_xscale;
    float image_yscale;
    float image_angle;
    float image_alpha;
    unsigned int image_blend;
    float x;
    float y;
    float xstart;
    float ystart;
    float xprevious;
    float yprevious;
    float direction;
    float speed;
    float friction;
    float gravity_direction;
    float gravity;
    float hspeed;
    float vspeed;
    int bbox[4];
    int timer[12];
    void *m_pPathAndTimeline;
    CCode *i_initcode;
    CCode *i_precreatecode;
    void *m_pOldObject;
    int layer;
    int mask_index;
    int16_t m_nMouseOver;
    CInstance *m_pNext;
    CInstance *m_pPrev;
    void *m_collisionLink[3]; // SLink
    void *m_dirtyLink[3];     // SLink
    void *m_withLink[3];      // SLink
    float depth;
    float i_currentdepth;
    float i_lastImageNumber;
    unsigned int m_collisionTestNumber;
};

struct RefDynamicArrayOfRValue : YYObjectBase {
    int m_refCount;
    int m_flags; // flag set = is readonly for example
    RValue *m_Array;
    void *m_Owner;
    int visited;
    int length;
};

// // 136
// static constexpr auto RefDynamicArrayOfRValue_offset_ref_count = offsetof(RefDynamicArrayOfRValue, m_refCount);
// // 144
// static constexpr auto RefDynamicArrayOfRValue_offset_array = offsetof(RefDynamicArrayOfRValue, m_Array);
// // 152
// static constexpr auto RefDynamicArrayOfRValue_offset_owner = offsetof(RefDynamicArrayOfRValue, m_Owner);
// // 164
// static constexpr auto RefDynamicArrayOfRValue_offset_length = offsetof(RefDynamicArrayOfRValue, length);

struct VMBuffer {
	void **vTable;
	int m_size;
	int m_numLocalVarsUsed;
	int m_numArguments;
	char *m_pBuffer;
	void **m_pConvertedBuffer;
	char *m_pJumpBuffer;
};

struct RToken {
	int kind;
	unsigned int type;
	int ind;
	int ind2;
	RValue value;
	int itemnumb;
	RToken *items;
	int position;
};

struct YYGMLFuncs;

struct CCode {
    int (**_vptr$CCode)(void);
    CCode *m_pNext;
    int i_kind;
    int i_compiled;
    const char *i_str;
    RToken i_token;
    RValue i_value;
    VMBuffer *i_pVM;
    VMBuffer *i_pVMDebugInfo;
    char *i_pCode;
    const char *name;
    int index;
    YYGMLFuncs *i_pFunc;
    bool i_watch;
    int i_offset;
    int i_locals;
    int i_args;
    int i_flags;
    YYObjectBase *i_pPrototype;
};


typedef bool (*RVariableRoutineGetter)(CInstance *self, void *a2, RValue *out);
typedef bool (*RVariableRoutineSetter)(CInstance *self, void *a2, RValue *new_value);

struct RVariableRoutine {
    const char *name;
    RVariableRoutineGetter getter;
    RVariableRoutineSetter setter;
    void *has_setter;
};
