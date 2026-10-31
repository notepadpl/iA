#include <emscripten/emscripten.h>
#include <cmath>

extern "C" {

// Funkcja dostępna później z poziomu JavaScript w przeglądarce
EMSCRIPTEN_KEEPALIVE
float analyze_network_traffic(float ping_ms, float payload_size, float req_rate) {
    // Prosta heurystyka / normalizator w C++
    float score = (ping_ms * 0.4f) + (payload_size / 1024.0f * 0.3f) + (req_rate * 0.3f);
    return score;
}

}