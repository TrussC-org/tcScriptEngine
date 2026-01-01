#pragma once

#include <TrussC.h>
#include <string>
#include <functional>
#include "libs/chaiscript/chaiscript/chaiscript.hpp"

using namespace std;
using namespace tc;

class tcScriptHost {
public:
    tcScriptHost();
    ~tcScriptHost();

    // Load and execute script code
    bool loadScript(const string& code);

    // Get last error message
    string getLastError() const { return lastError_; }

    // Lifecycle calls (call from tcApp)
    void callSetup();
    void callUpdate();
    void callDraw();

    // Event calls
    void callMousePressed(float x, float y, int button);
    void callMouseReleased(float x, float y, int button);
    void callMouseMoved(float x, float y);
    void callMouseDragged(float x, float y, int button);
    void callKeyPressed(int key);
    void callKeyReleased(int key);
    void callWindowResized(int width, int height);

private:
    void bindTrussCFunctions();

    template<typename Func>
    void tryCall(const string& funcName, Func&& func);

    unique_ptr<chaiscript::ChaiScript> chai_;
    string lastError_;

    // Cached function existence flags
    bool hasSetup_ = false;
    bool hasUpdate_ = false;
    bool hasDraw_ = false;
    bool hasMousePressed_ = false;
    bool hasMouseReleased_ = false;
    bool hasMouseMoved_ = false;
    bool hasMouseDragged_ = false;
    bool hasKeyPressed_ = false;
    bool hasKeyReleased_ = false;
    bool hasWindowResized_ = false;
};
