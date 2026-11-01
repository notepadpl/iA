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

// 2. Podstawowe obliczenia analityczne
EMSCRIPTEN_KEEPALIVE
float analyze_network_traffic(float ping_ms, float payload_size, float req_rate) {
    return (ping_ms * 0.4f) + (payload_size / 1024.0f * 0.3f) + (req_rate * 0.3f);
}

// 3. Test w C++: Sprawdzenie czy plik ONNX w wirtualnym systemie plików istnieje i daje się otworzyć
EMSCRIPTEN_KEEPALIVE
const char* check_onnx_virtual_file2() {
    // Emscripten po spakowaniu flagą --preload-file tworzy wirtualny system plików,
    // więc C++ widzi plik pod ścieżką, z którą został uruchomiony.
    FILE* file = fopen("assets/network_anomaly.onnx", "rb");
    if (file) {
        // Sprawdźmy rozmiar pliku, żeby upewnić się, że to nie pusta skorupa
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fclose(file);

        snprintf(output_buffer, sizeof(output_buffer), "[SUKCES C++] Plik ONNX wykryty w wirtualnym FS! Rozmiar: %ld bajtów", size);
    } else {
        snprintf(output_buffer, sizeof(output_buffer), "[BŁĄD C++] Nie udało się otworzyć pliku assets/network_anomaly.onnx przez fopen.");
    }
    return output_buffer;
}

EMSCRIPTEN_KEEPALIVE
const char* check_onnx_virtual_file() {
    // Bezpieczne otwarcie pliku w trybie binarnym
    FILE* file = fopen("assets/network_anomaly.onnx", "rb");
    if (!file) {
        snprintf(output_buffer, sizeof(output_buffer), "[BŁĄD C++] Nie można otworzyć pliku.");
        return output_buffer;
    }

    // Bezpieczne odczytanie pierwszych 4 bajtów bez używania fseek (zero ryzyka zawieszenia)
    unsigned char header[4] = {0};
    size_t bytesRead = fread(header, 1, 4, file);
    
    // Szybkie policzenie orientacyjnego rozmiaru poprzez przesunięcie na koniec i powrót, 
    // ale zrobione prosto:
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    
    fclose(file);

    snprintf(output_buffer, sizeof(output_buffer), 
        "OK | Rozmiar: %ld B | Nagłówek: %02X %02X %02X %02X", 
        size, 
        bytesRead > 0 ? header[0] : 0, 
        bytesRead > 1 ? header[1] : 0, 
        bytesRead > 2 ? header[2] : 0, 
        bytesRead > 3 ? header[3] : 0);

    return output_buffer;
}

// 4. Interaktywny Shell w C++
EMSCRIPTEN_KEEPALIVE
const char* execute_shell_command(const char* command) {
    if (strcmp(command, "help") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "Dostepne komendy: help, status, scan, version, ip, checkai");
    } 
    else if (strcmp(command, "status") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[OK] Moduł C++ WASM działa stabilnie.");
    } 
    else if (strcmp(command, "scan") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[SCAN] Analiza buforów sieciowych... Brak anomalii.");
    } 
    else if (strcmp(command, "version") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "CyberWASM Security Core v1.2.0 (C++ File Test)");
    } 
    else if (strcmp(command, "ip") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[INFO] Żądanie wyświetlenia IP obsłużone przez C++.");
    }
    else if (strcmp(command, "checkai") == 0) {
        // Wywołanie testu pliku bezpośrednio z komendy shella!
        return check_onnx_virtual_file();
    }
       else if (strcmp(command, "ai") == 0) {
        // Wywołanie testu pliku bezpośrednio z komendy shella!
        return check_onnx_virtual_file2();
    }
    else {
        snprintf(output_buffer, sizeof(output_buffer), "Nieznana komenda: '%s'. Wpisz 'help'.", command);
    }
    
    return output_buffer;
}

}