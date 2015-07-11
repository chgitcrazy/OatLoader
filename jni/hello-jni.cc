/*
 * Copyright (C) 2009 The Android Open Source Project

 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include<stdio.h>
#include <jni.h>
#include<dlfcn.h>
#include <android/log.h>
#include <stdlib.h>
#include <string>
#include <vector>
//#include <oat_file.h>

#define printstr(A) __android_log_print(ANDROID_LOG_INFO,"JNI ","string from jni %s ",(A))
#define printint(A) __android_log_print(ANDROID_LOG_INFO,"JNI ","string from jni %p ",(A))

typedef unsigned char uint8_t;
typedef uint8_t byte;
//typedef uint8_t bool;
typedef unsigned int uint32_t;

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
#ifdef __cplusplus
extern "C" {
#endif
jstring Java_com_example_hellojni_HelloJni_stringFromJNI(JNIEnv* env,
		jclass thiz) {
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#define ABI "armeabi-v7a/NEON"
#else
#define ABI "armeabi-v7a"
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__mips__)
#define ABI "mips"
#else
#define ABI "unknown"
#endif
	const std::string filename =
			"/data/data/com.example.hellojni/cachetmp/loaded.dex";
	const std::string location =
			"/data/data/com.example.hellojni/cachetmp/loaded.dex";
	byte* requested_base = NULL;
	bool executable = false;
	std::string error_msg = "hello";

	//OatFile::Open的函数指针
	int* (*pTest)(const std::string, const std::string, byte*, bool);

//OatFile::Begin()的函数指针
	byte* (*pTest1)();

	void* dlopen_handle_ = dlopen("/system/lib/libart.so", RTLD_NOW);

	if (dlopen_handle_ == NULL) {
		return env->NewStringUTF("can't open file");
	}

//	动态调用OatFile* OatFile::Open方法
	pTest = (int* (*)(const std::string, const std::string, byte*, bool))
	dlsym(dlopen_handle_,
	"_ZN3art7OatFile4OpenERKSsS2_Phb");

//动态调用OatFile::Begin方法
pTest1	= (byte*(*)())dlsym(dlopen_handle_,"_ZNK3art7OatFile5BeginEv");

// 打印OatFile* OatFile::Open在内存中得地址
    printint(pTest);
	printint(pTest1);

	if (pTest == NULL) {
		return env->NewStringUTF("can't find symbol");
	}

	if (pTest1 == NULL) {
		return env->NewStringUTF("can't find symbol");
	}

	int* test = pTest(filename, location, requested_base, executable);

	printint(test);

	pTest1();

	return env->NewStringUTF("hello");
}

#ifdef __cplusplus
}
#endif
