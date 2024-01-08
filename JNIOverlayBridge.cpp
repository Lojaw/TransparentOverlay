#include "de_lojaw_jni_NativeOverlayRenderer.h"
#include "TransparentOverlay.h"
#include <jni.h>

// Globale Instanz des TransparentOverlay
TransparentOverlay* overlayInstance = nullptr;

// Implementierung der initOverlay-Methode
JNIEXPORT void JNICALL Java_de_lojaw_jni_NativeOverlayRenderer_initOverlay(JNIEnv*, jclass) {
    if (!overlayInstance) {
        overlayInstance = new TransparentOverlay();
    }
}

// Implementierung der renderOverlay-Methode
JNIEXPORT void JNICALL Java_de_lojaw_jni_NativeOverlayRenderer_renderOverlay(JNIEnv*, jclass) {
    if (overlayInstance) {
        overlayInstance->Run();
    }
}

// Implementierung der destroyOverlay-Methode
JNIEXPORT void JNICALL Java_de_lojaw_jni_NativeOverlayRenderer_destroyOverlay(JNIEnv*, jclass) {
    if (overlayInstance) {
        delete overlayInstance;
        overlayInstance = nullptr;
    }
}