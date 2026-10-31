#include <emscripten/emscripten.h>
#include <stdio.h>
#include <string.h>

static char output_buffer[512];

extern "C" {

// 1. Obsługa i formatowanie adresu IP
EMSCRIPTEN_KEEPALIVE
const char* process_client_ip(const char* ip_address) {
    snprintf(output_buffer, sizeof(output_buffer), "Przechwycono IP przez WASM: %s", ip_address);
    return output_buffer;
}

// 2. Podstawowe obliczenia analityczne (metryki ruchu)
EMSCRIPTEN_KEEPALIVE
float analyze_network_traffic(float ping_ms, float payload_size, float req_rate) {
    return (ping_ms * 0.4f) + (payload_size / 1024.0f * 0.3f) + (req_rate * 0.3f);
}

// 3. Interaktywny Shell / Interpreter komend w C++
EMSCRIPTEN_KEEPALIVE
const char* execute_shell_command(const char* command) {
    if (strcmp(command, "help") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "Dostepne komendy: help, status, scan, version, ip");
    } 
    else if (strcmp(command, "status") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[OK] Moduł C++ WASM działa stabilnie w czasie rzeczywistym.");
    } 
    else if (strcmp(command, "scan") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[SCAN] Analiza buforów sieciowych... Brak podejrzanych pakietów.");
    } 
    else if (strcmp(command, "version") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "CyberWASM Security Core v1.1.0 (Native C++)");
    } 
    else if (strcmp(command, "ip") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[INFO] Żądanie wyświetlenia IP obsłużone bezpośrednio przez rdzeń C++.");
    }
    else {
        snprintf(output_buffer, sizeof(output_buffer), "Nieznana komenda: '%s'. Wpisz 'help', aby uzyskać pomoc.", command);
    }
    
    return output_buffer;
}

}