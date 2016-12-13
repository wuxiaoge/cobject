#include "object.h"

static Object *float_method_add(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) + (float)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) + (float)LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) + FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)FloatObject_AsFLOAT(self) + DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *float_method_sub(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) - (float)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) - (float)LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) - FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)FloatObject_AsFLOAT(self) - DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *float_method_mul(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) * (float)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) * (float)LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) * FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)FloatObject_AsFLOAT(self) * DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *float_method_div(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) / (float)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) / (float)LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = FloatObject_FromFloat(FloatObject_AsFLOAT(self) / FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble((double)FloatObject_AsFLOAT(self) / DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

MethodDef float_methods[] = {
    {"Add", float_method_add},
    {"Sub", float_method_sub},
    {"Mul", float_method_mul},
    {"Div", float_method_div},
    {Object_NULL, Object_NULL}
};

static int float_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    float _f = *(float *)args;
    FloatObject_VALUE(self) = _f;
    return 0;
}

static int float_deinit(Object *self) {
    return 0;
}

static long float_hash(Object *self) {
    return -1L;
}

static Object *float_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "%f", FloatObject_AsFLOAT(self));
    return StrObject_FromStr(s);
}

TypeObject Float_Type = {
    .tp_name = "float",
    .tp_init = float_init,
    .tp_deinit = float_deinit,
    .tp_hash = float_hash,
    .tp_str = float_str,
    .tp_methods = float_methods
};

Object *FloatObject_FromFloat(float f) {
    Object *_f = Object_Malloc(&Float_Type, sizeof(FloatObject));
    Object_Init(_f, Object_CONVERT(&f));
    return _f;
}



