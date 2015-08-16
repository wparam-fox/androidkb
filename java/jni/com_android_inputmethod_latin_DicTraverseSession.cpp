/*
 * Copyright (C) 2012 The Android Open Source Project
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
 */

#define LOG_TAG "LatinIME: jni: Session"

#include "com_android_inputmethod_latin_DicTraverseSession.h"

#include "defines.h"
#include "jni.h"
#include "jni_common.h"
#include "suggest/core/session/dic_traverse_session.h"

namespace latinime {
class Dictionary;
static jlong latinime_setDicTraverseSession(JNIEnv *env, jclass clazz, jstring localeJStr,
        jlong dictSize) {
    void *traverseSession = DicTraverseSession::getSessionInstance(env, localeJStr, dictSize);
    return reinterpret_cast<jlong>(traverseSession);
}

static void latinime_initDicTraverseSession(JNIEnv *env, jclass clazz, jlong traverseSession,
        jlong dictionary, jintArray previousWord, jint previousWordLength) {
    DicTraverseSession *ts = reinterpret_cast<DicTraverseSession *>(traverseSession);
    Dictionary *dict = reinterpret_cast<Dictionary *>(dictionary);
    if (!previousWord) {
        DicTraverseSession::initSessionInstance(
                ts, dict, 0 /* prevWord */, 0 /* prevWordLength*/, 0 /* suggestOptions */);
        return;
    }
    int prevWord[previousWordLength];
    env->GetIntArrayRegion(previousWord, 0, previousWordLength, prevWord);
    DicTraverseSession::initSessionInstance(
            ts, dict, prevWord, previousWordLength, 0 /* suggestOptions */);
}

static void latinime_releaseDicTraverseSession(JNIEnv *env, jclass clazz, jlong traverseSession) {
    DicTraverseSession *ts = reinterpret_cast<DicTraverseSession *>(traverseSession);
    DicTraverseSession::releaseSessionInstance(ts);
}

static const JNINativeMethod sMethods[] = {
    {
        const_cast<char *>("setDicTraverseSessionNative"),
        const_cast<char *>("(Ljava/lang/String;J)J"),
        reinterpret_cast<void *>(latinime_setDicTraverseSession)
    },
    {
        const_cast<char *>("initDicTraverseSessionNative"),
        const_cast<char *>("(JJ[II)V"),
        reinterpret_cast<void *>(latinime_initDicTraverseSession)
    },
    {
        const_cast<char *>("releaseDicTraverseSessionNative"),
        const_cast<char *>("(J)V"),
        reinterpret_cast<void *>(latinime_releaseDicTraverseSession)
    }
};

int register_DicTraverseSession(JNIEnv *env) {
    const char *const kClassPathName = "com/wparam/kb/inputmethod/latin/DicTraverseSession";
    return registerNativeMethods(env, kClassPathName, sMethods, NELEMS(sMethods));
}
} // namespace latinime
