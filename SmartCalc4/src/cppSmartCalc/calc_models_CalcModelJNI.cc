#include "calc_models_CalcModelJNI.h"
#include "SmartCalcModel.h"
#include <iostream>

JNIEXPORT jstring JNICALL Java_calc_models_CalcModelJNI_calcResult
  (JNIEnv *env, jobject, jstring jExpression, jstring jExpressionX) {
    const char *cstr = env->GetStringUTFChars(jExpression, NULL);
    std::string expression = std::string(cstr);

    const char *cstrX = env->GetStringUTFChars(jExpressionX, NULL);
    std::string expressionX = std::string(cstrX);

    s21::CalcModel model;
    std::string result = model.CalcResult(expression, expressionX);
    
    return env->NewStringUTF(result.c_str());
}

JNIEXPORT jobjectArray JNICALL Java_calc_models_CalcModelJNI_calcGraphData
  (JNIEnv *env, jobject obj, jstring jExpression, jdouble xBegin, jdouble xEnd) {
    const char *cstr = env->GetStringUTFChars(jExpression, NULL);
    std::string expression = std::string(cstr);
    
    s21::CalcModel model;
    std::string result = model.CalcGraphData(expression, (double) xBegin, (double) xEnd, 0., 0.);
    std::pair<std::vector<double>, std::vector<double>> graph_data = model.GetGraphData();

    size_t size = graph_data.first.size();

    jclass cls = env->FindClass("[D");

    jdoubleArray iniVal = env->NewDoubleArray(size);
    jobjectArray outer = env->NewObjectArray(2, cls, iniVal);

    jdoubleArray inner = env->NewDoubleArray(size);
    env->SetDoubleArrayRegion(inner, 0, size, graph_data.first.data());
    env->SetObjectArrayElement(outer, 0, inner);
    env->DeleteLocalRef(inner);

    inner = env->NewDoubleArray(size);
    env->SetDoubleArrayRegion(inner, 0, size, graph_data.second.data());
    env->SetObjectArrayElement(outer, 1, inner);
    env->DeleteLocalRef(inner);
    
    return outer;
}