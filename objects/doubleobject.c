#include "object.h"

static Object *double_method_add(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) + (double)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) + (double)LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) + (double)FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) + DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *double_method_sub(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) - (double)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) - (double)LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) - (double)FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) - DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *double_method_mul(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) * (double)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) * (double)LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) * (double)FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) * DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static Object *double_method_div(Object *self, Object *ob) {
    Object *ret = Object_NULL;
    if(Object_TYPE(ob) == (TypeObject *)&Int_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) / (double)IntObject_AsINT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Long_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) / (double)LongObject_AsLONG(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Float_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) / (double)FloatObject_AsFLOAT(ob));
    } else if(Object_TYPE(ob) == (TypeObject *)&Double_Type) {
        ret = DoubleObject_FromDouble(DoubleObject_AsDOUBLE(self) / DoubleObject_AsDOUBLE(ob));
    }
    return ret;
}

static MethodDef double_methods[] = {
    {"Add", double_method_add},
    {"Sub", double_method_sub},
    {"Mul", double_method_mul},
    {"Div", double_method_div},
    {Object_NULL, Object_NULL}
};

static int double_init(Object *self, Object *args) {
    Object_Extend(self, &Object_Type, sizeof(Object));
    Object_Init(Object_BASE(self), Object_NULL);
    double _d = *(double *)args;
    DoubleObject_VALUE(self) = _d;
    return Object_OK;
}

static int double_deinit(Object *self) {
    return Object_OK;
}

static long double_hash(Object *self) {
    return -1L;
}

static Object *double_str(Object *self) {
    char s[32] = {0};
    sprintf(s, "%lf", DoubleObject_AsDOUBLE(self));
    return StrObject_FromStr((const char *)s);
}

TypeObject Double_Type = {
    .tp_name = "double",
    .tp_init = double_init,
    .tp_deinit = double_deinit,
    .tp_hash = double_hash,
    .tp_str = double_str,
    .tp_methods = double_methods
};

Object *DoubleObject_FromDouble(double d) {
    Object *_d = Object_Malloc(&Double_Type, sizeof(DoubleObject));
    Object_Init(_d, Object_CONVERT(&d));
    return _d;
}
