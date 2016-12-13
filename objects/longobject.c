#include "object.h"

static Object *long_method_add(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = LongObject_FromLong(LongObject_AsLONG(self) + (long)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = LongObject_FromLong(LongObject_AsLONG(self) + LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat((float)LongObject_AsLONG(self) + FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)LongObject_AsLONG(self) + DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *long_method_sub(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = LongObject_FromLong(LongObject_AsLONG(self) - (long)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = LongObject_FromLong(LongObject_AsLONG(self) - LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat((float)LongObject_AsLONG(self) - FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)LongObject_AsLONG(self) - DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *long_method_mul(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = LongObject_FromLong(LongObject_AsLONG(self) * (long)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = LongObject_FromLong(LongObject_AsLONG(self) * LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat((float)LongObject_AsLONG(self) * FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)LongObject_AsLONG(self) * DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *long_method_div(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = LongObject_FromLong(LongObject_AsLONG(self) / (long)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = LongObject_FromLong(LongObject_AsLONG(self) / LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat((float)LongObject_AsLONG(self) / FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)LongObject_AsLONG(self) / DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static MethodDef long_methods[] = {
    {"Add", long_method_add},
    {"Sub", long_method_sub},
    {"Mul", long_method_mul},
    {"Div", long_method_div},
    {Object_NULL, Object_NULL}
};

static int long_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    long _l = *(long *)args;
    LongObject_VALUE(self) = _l;
    return 0;
}

static int long_deinit(Object *self) {
    return 0;
}

static long long_hash(Object *self) {
    return LongObject_VALUE(self);
}

static Object *long_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "%ld", LongObject_AsLONG(self));
    return StrObject_FromStr((const char *)s);
}

TypeObject Long_Type = {
    .tp_name = "long",
    .tp_init = long_init,
    .tp_deinit = long_deinit,
    .tp_hash = long_hash,
    .tp_str = long_str,
    .tp_methods = long_methods
};

Object *LongObject_FromLong(long l) {
    Object *_l = Object_Malloc(&Long_Type, sizeof(LongObject));
    Object_Init(_l, Object_CONVERT(&l));
    return _l;
}


