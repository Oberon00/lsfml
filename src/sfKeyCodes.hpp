// No include guards
//
// #define E(name, value) to something meaningful

#ifndef KEY
#   error You have to provide a definiton for the macro KEY(name, value)
#endif

#define K1(n) KEY(#n, n)
        K1(A) K1(B) K1(C) K1(D) K1(E) K1(F) K1(G) K1(H) K1(I) K1(J) K1(K)
        K1(L) K1(M) K1(N) K1(O) K1(P) K1(Q) K1(R) K1(S) K1(T) K1(U) K1(V)
        K1(W) K1(X) K1(Y) K1(Z)
        K1(F1) K1(F2) K1(F3) K1(F4) K1(F5) K1(F6) K1(F7) K1(F8) K1(F9) K1(F10)
        K1(F11) K1(F12) K1(F13) K1(F14) K1(F15)
#undef K1

        KEY("UNKNOWN", Unknown)
        KEY("NUM0", Num0)
        KEY("NUM1", Num1) KEY("NUM2", Num2) KEY("NUM3", Num3)
        KEY("NUM4", Num4) KEY("NUM5", Num5) KEY("NUM6", Num6)
        KEY("NUM7", Num7) KEY("NUM8", Num8) KEY("NUM9", Num9)
        KEY("ESCAPE", Escape)
        KEY("LCONTROL", LControl) KEY("LSHIFT", LShift)
        KEY("LALT", LAlt) KEY("LSYSTEM", LSystem)
        KEY("RCONTROL", RControl) KEY("RSHIFT", RShift)
        KEY("RALT", RAlt) KEY("RSYSTEM", RSystem)
        KEY("MENU", Menu)
        KEY("LBRACKET", LBracket)
        KEY("RBRACKET", RBracket)
        KEY("SEMICOLON", SemiColon) KEY("COMMA", Comma) KEY("PERIOD", Period)
        KEY("QUOTE", Quote)
        KEY("SLASH", Slash) KEY("BACKSLASH", BackSlash)
        KEY("TILDE", Tilde)
        KEY("EQUAL", Equal)
        KEY("DASH", Dash)
        KEY("SPACE", Space)
        KEY("RETURN", Return)
        KEY("BACKSPACE", BackSpace)
        KEY("TAB", Tab)
        KEY("PAGEUP", PageUp) KEY("PAGEDOWN", PageDown)
        KEY("END", End) KEY("HOME", Home)
        KEY("INSERT", Insert) KEY("DELETE", Delete)
        KEY("ADD", Add) KEY("SUBTRACT", Subtract)
        KEY("MULTIPLY", Multiply) KEY("DIVIDE", Divide)
        KEY("LEFT", Left) KEY("RIGHT", Right) KEY("UP", Up) KEY("DOWN", Down)
        KEY("NUMPAD0", Numpad0)
        KEY("NUMPAD1", Numpad1) KEY("NUMPAD2", Numpad2) KEY("NUMPAD3", Numpad3)
        KEY("NUMPAD4", Numpad4) KEY("NUMPAD5", Numpad5) KEY("NUMPAD6", Numpad6)
        KEY("NUMPAD7", Numpad7) KEY("NUMPAD8", Numpad8) KEY("NUMPAD9", Numpad9)
        KEY("PAUSE", Pause)

#undef KEY
