#include "tcScriptHost.h"

tcScriptHost::tcScriptHost() {
    chai_ = make_unique<chaiscript::ChaiScript>();
    bindTrussCFunctions();
}

tcScriptHost::~tcScriptHost() = default;

void tcScriptHost::bindTrussCFunctions() {
    using namespace chaiscript;

    // ==========================================================================
    // Graphics - Drawing
    // ==========================================================================

    // clear(gray) or clear(r, g, b) or clear(r, g, b, a)
    chai_->add(fun([](float gray) { clear(gray); }), "clear");
    chai_->add(fun([](float r, float g, float b) { clear(r, g, b); }), "clear");
    chai_->add(fun([](float r, float g, float b, float a) { clear(r, g, b, a); }), "clear");

    // setColor - multiple overloads
    chai_->add(fun([](float r, float g, float b) { setColor(r, g, b); }), "setColor");
    chai_->add(fun([](float r, float g, float b, float a) { setColor(r, g, b, a); }), "setColor");
    chai_->add(fun([](float gray) { setColor(gray, gray, gray); }), "setColor");

    // Basic shapes
    chai_->add(fun([](float x, float y, float w, float h) { drawRect(x, y, w, h); }), "drawRect");
    chai_->add(fun([](float x, float y, float r) { drawCircle(x, y, r); }), "drawCircle");
    chai_->add(fun([](float x, float y, float w, float h) { drawEllipse(x, y, w, h); }), "drawEllipse");
    chai_->add(fun([](float x1, float y1, float x2, float y2) { drawLine(x1, y1, x2, y2); }), "drawLine");
    chai_->add(fun([](float x1, float y1, float x2, float y2, float x3, float y3) {
        drawTriangle(x1, y1, x2, y2, x3, y3);
    }), "drawTriangle");

    // Fill/Stroke control
    chai_->add(fun([]() { fill(); }), "fill");
    chai_->add(fun([]() { noFill(); }), "noFill");
    chai_->add(fun([]() { stroke(); }), "stroke");
    chai_->add(fun([]() { noStroke(); }), "noStroke");
    chai_->add(fun([](float w) { setStrokeWeight(w); }), "setStrokeWeight");

    // Text
    chai_->add(fun([](const string& text, float x, float y) {
        drawBitmapString(text, x, y);
    }), "drawText");

    // ==========================================================================
    // Transform
    // ==========================================================================
    chai_->add(fun([](float x, float y) { translate(x, y); }), "translate");
    chai_->add(fun([](float x, float y, float z) { translate(x, y, z); }), "translate");
    chai_->add(fun([](float rad) { rotate(rad); }), "rotate");
    chai_->add(fun([](float deg) { rotateDeg(deg); }), "rotateDeg");
    chai_->add(fun([](float s) { scale(s, s); }), "scale");
    chai_->add(fun([](float sx, float sy) { scale(sx, sy); }), "scale");
    chai_->add(fun([]() { pushMatrix(); }), "pushMatrix");
    chai_->add(fun([]() { popMatrix(); }), "popMatrix");

    // ==========================================================================
    // Window
    // ==========================================================================
    chai_->add(fun([]() { return getWindowWidth(); }), "getWindowWidth");
    chai_->add(fun([]() { return getWindowHeight(); }), "getWindowHeight");

    // ==========================================================================
    // Input - Mouse
    // ==========================================================================
    chai_->add(fun([]() { return getMouseX(); }), "getMouseX");
    chai_->add(fun([]() { return getMouseY(); }), "getMouseY");
    chai_->add(fun([]() { return isMousePressed(); }), "isMousePressed");

    // ==========================================================================
    // Time
    // ==========================================================================
    chai_->add(fun([]() { return getElapsedTime(); }), "getElapsedTime");
    chai_->add(fun([]() { return getDeltaTime(); }), "getDeltaTime");
    chai_->add(fun([]() { return getFrameRate(); }), "getFrameRate");
    chai_->add(fun([]() { return static_cast<int>(getFrameCount()); }), "getFrameCount");

    // ==========================================================================
    // Math
    // ==========================================================================
    chai_->add(fun([](float min, float max) { return random(min, max); }), "random");
    chai_->add(fun([](float max) { return random(0.0f, max); }), "random");
    chai_->add(fun([]() { return random(0.0f, 1.0f); }), "random");

    chai_->add(fun([](float a, float b, float t) { return tc::lerp(a, b, t); }), "lerp");
    chai_->add(fun([](float v, float min, float max) { return tc::clamp(v, min, max); }), "clamp");
    chai_->add(fun([](float v, float inMin, float inMax, float outMin, float outMax) {
        return tc::map(v, inMin, inMax, outMin, outMax);
    }), "map");

    chai_->add(fun([](float x) { return noise(x); }), "noise");
    chai_->add(fun([](float x, float y) { return noise(x, y); }), "noise");
    chai_->add(fun([](float x, float y, float z) { return noise(x, y, z); }), "noise");

    chai_->add(fun([](float deg) -> float { return deg2rad(deg); }), "radians");
    chai_->add(fun([](float rad) -> float { return rad2deg(rad); }), "degrees");

    // Math functions
    chai_->add(fun([](float x) { return std::sin(x); }), "sin");
    chai_->add(fun([](float x) { return std::cos(x); }), "cos");
    chai_->add(fun([](float x) { return std::tan(x); }), "tan");
    chai_->add(fun([](float x) { return std::abs(x); }), "abs");
    chai_->add(fun([](float x) { return std::sqrt(x); }), "sqrt");
    chai_->add(fun([](float x, float y) { return std::pow(x, y); }), "pow");
    chai_->add(fun([](float x, float y) { return std::min(x, y); }), "min");
    chai_->add(fun([](float x, float y) { return std::max(x, y); }), "max");
    chai_->add(fun([](float x) { return std::floor(x); }), "floor");
    chai_->add(fun([](float x) { return std::ceil(x); }), "ceil");
    chai_->add(fun([](float x, float y) { return std::fmod(x, y); }), "fmod");

    // ==========================================================================
    // Constants
    // ==========================================================================
    chai_->add_global_const(chaiscript::const_var(TAU), "TAU");
    chai_->add_global_const(chaiscript::const_var(HALF_TAU), "HALF_TAU");
    chai_->add_global_const(chaiscript::const_var(QUARTER_TAU), "QUARTER_TAU");
    chai_->add_global_const(chaiscript::const_var(static_cast<float>(M_PI)), "PI");

    // ==========================================================================
    // Utility
    // ==========================================================================
    chai_->add(fun([](const string& msg) {
        logNotice("tcScript") << msg;
    }), "print");

    chai_->add(fun([](const string& msg) {
        logNotice("tcScript") << msg;
    }), "log");
}

bool tcScriptHost::loadScript(const string& code) {
    lastError_.clear();

    // Reset function flags
    hasSetup_ = hasUpdate_ = hasDraw_ = false;
    hasMousePressed_ = hasMouseReleased_ = hasMouseMoved_ = hasMouseDragged_ = false;
    hasKeyPressed_ = hasKeyReleased_ = hasWindowResized_ = false;

    try {
        // Create fresh ChaiScript instance
        chai_ = make_unique<chaiscript::ChaiScript>();
        bindTrussCFunctions();

        // Evaluate user code
        chai_->eval(code);

        // Check which functions are defined
        try { chai_->eval<function<void()>>("setup"); hasSetup_ = true; } catch (...) {}
        try { chai_->eval<function<void()>>("update"); hasUpdate_ = true; } catch (...) {}
        try { chai_->eval<function<void()>>("draw"); hasDraw_ = true; } catch (...) {}
        try { chai_->eval<function<void(float, float, int)>>("mousePressed"); hasMousePressed_ = true; } catch (...) {}
        try { chai_->eval<function<void(float, float, int)>>("mouseReleased"); hasMouseReleased_ = true; } catch (...) {}
        try { chai_->eval<function<void(float, float)>>("mouseMoved"); hasMouseMoved_ = true; } catch (...) {}
        try { chai_->eval<function<void(float, float, int)>>("mouseDragged"); hasMouseDragged_ = true; } catch (...) {}
        try { chai_->eval<function<void(int)>>("keyPressed"); hasKeyPressed_ = true; } catch (...) {}
        try { chai_->eval<function<void(int)>>("keyReleased"); hasKeyReleased_ = true; } catch (...) {}
        try { chai_->eval<function<void(int, int)>>("windowResized"); hasWindowResized_ = true; } catch (...) {}

        return true;
    } catch (const chaiscript::exception::eval_error& e) {
        lastError_ = e.pretty_print();
        logError("tcScript") << lastError_;
        return false;
    } catch (const exception& e) {
        lastError_ = e.what();
        logError("tcScript") << lastError_;
        return false;
    }
}

template<typename Func>
void tcScriptHost::tryCall(const string& funcName, Func&& func) {
    try {
        func();
    } catch (const exception& e) {
        lastError_ = string(funcName) + ": " + e.what();
        logError("tcScript") << lastError_;
    }
}

void tcScriptHost::callSetup() {
    if (!hasSetup_) return;
    tryCall("setup", [this]() {
        auto func = chai_->eval<function<void()>>("setup");
        func();
    });
}

void tcScriptHost::callUpdate() {
    if (!hasUpdate_) return;
    tryCall("update", [this]() {
        auto func = chai_->eval<function<void()>>("update");
        func();
    });
}

void tcScriptHost::callDraw() {
    if (!hasDraw_) return;
    tryCall("draw", [this]() {
        auto func = chai_->eval<function<void()>>("draw");
        func();
    });
}

void tcScriptHost::callMousePressed(float x, float y, int button) {
    if (!hasMousePressed_) return;
    tryCall("mousePressed", [this, x, y, button]() {
        auto func = chai_->eval<function<void(float, float, int)>>("mousePressed");
        func(x, y, button);
    });
}

void tcScriptHost::callMouseReleased(float x, float y, int button) {
    if (!hasMouseReleased_) return;
    tryCall("mouseReleased", [this, x, y, button]() {
        auto func = chai_->eval<function<void(float, float, int)>>("mouseReleased");
        func(x, y, button);
    });
}

void tcScriptHost::callMouseMoved(float x, float y) {
    if (!hasMouseMoved_) return;
    tryCall("mouseMoved", [this, x, y]() {
        auto func = chai_->eval<function<void(float, float)>>("mouseMoved");
        func(x, y);
    });
}

void tcScriptHost::callMouseDragged(float x, float y, int button) {
    if (!hasMouseDragged_) return;
    tryCall("mouseDragged", [this, x, y, button]() {
        auto func = chai_->eval<function<void(float, float, int)>>("mouseDragged");
        func(x, y, button);
    });
}

void tcScriptHost::callKeyPressed(int key) {
    if (!hasKeyPressed_) return;
    tryCall("keyPressed", [this, key]() {
        auto func = chai_->eval<function<void(int)>>("keyPressed");
        func(key);
    });
}

void tcScriptHost::callKeyReleased(int key) {
    if (!hasKeyReleased_) return;
    tryCall("keyReleased", [this, key]() {
        auto func = chai_->eval<function<void(int)>>("keyReleased");
        func(key);
    });
}

void tcScriptHost::callWindowResized(int width, int height) {
    if (!hasWindowResized_) return;
    tryCall("windowResized", [this, width, height]() {
        auto func = chai_->eval<function<void(int, int)>>("windowResized");
        func(width, height);
    });
}
