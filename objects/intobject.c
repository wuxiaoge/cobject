#include "object.h"

static Object *int_method_add(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = IntObject_FromInt(IntObject_AsINT(self) + IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = LongObject_FromLong((long)IntObject_AsINT(self) + LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat((float)IntObject_AsINT(self) + FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)IntObject_AsINT(self) + DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *int_method_sub(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = IntObject_FromInt(IntObject_AsINT(self) - IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = LongObject_FromLong((long)IntObject_AsINT(self) - LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat((float)IntObject_AsINT(self) - FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)IntObject_AsINT(self) - DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *int_method_mul(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = IntObject_FromInt(IntObject_AsINT(self) * IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = LongObject_FromLong((long)IntObject_AsINT(self) * LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat((float)IntObject_AsINT(self) * FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)IntObject_AsINT(self) * DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *int_method_div(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = IntObject_FromInt(IntObject_AsINT(self) / IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = LongObject_FromLong((long)IntObject_AsINT(self) / LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat((float)IntObject_AsINT(self) / FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)IntObject_AsINT(self) / DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static MethodDef int_methods[] = {
    {"Add", int_method_add},
    {"Sub", int_method_sub},
    {"Mul", int_method_mul},
    {"Div", int_method_div},
    {Object_NULL, Object_NULL}
};

static int int_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL); 
    int _i = *(int *)args;
    IntObject_VALUE(self) = _i;
    return Object_OK;
}

static int int_deinit(Object *self) {
    return Object_OK;
}

static long int_hash(Object *self) {
    return IntObject_VALUE(self);
}

static BOOL int_equal(Object *self, Object *ob) {
    return IntObject_AsINT(self) == IntObject_AsINT(ob) ? TRUE : FALSE;
}

static Object *int_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "%d", IntObject_AsINT(self));
    return StrObject_FromStr((const char *)s);
}

TypeObject Int_Type = {
    .tp_name = "int",
    .tp_init = int_init,
    .tp_deinit = int_deinit,
    .tp_hash = int_hash,
    .tp_equal = int_equal,
    .tp_str = int_str,
    .tp_methods = int_methods
};

Object *IntObject_FromInt(int i) {
    Object *_i = Object_Malloc(&Int_Type, sizeof(IntObject));
    Object_Init(_i, Object_CONVERT(&i));
    return _i;
}


