#include <emscripten/emscripten.h>
#include <stdio.h>
#include <string.h>

static char output_buffer[256];

extern "C" {

EMSCRIPTEN_KEEPALIVE
const char* process_client_ip(const char* ip_address) {
    // C++ odbiera adres IP i formatuje go w logu/komunikacie
    snprintf(output_buffer, sizeof(output_buffer), "Przechwycono IP przez WASM: %s", ip_address);
    return output_buffer;
}

EMSCRIPTEN_KEEPALIVE
float analyze_network_traffic(float ping_ms, float payload_size, float req_rate) {
    return (ping_ms * 0.4f) + (payload_size / 1024.0f * 0.3f) + (req_rate * 0.3f);
}

}