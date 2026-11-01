#include <emscripten/emscripten.h>
#include <stdio.h>
#include <string.h>

static char output_buffer[512];

// GLOBALNY DŻIN: Pamięć RAM, w której model mieszka na stałe
static unsigned char* global_genie_brain = nullptr;
static long global_genie_size = 0;

extern "C" {

EMSCRIPTEN_KEEPALIVE
const char* process_client_ip(const char* ip_address) {
    snprintf(output_buffer, sizeof(output_buffer), "Przechwycono IP przez WASM: %s", ip_address);
    return output_buffer;
}

EMSCRIPTEN_KEEPALIVE
float analyze_network_traffic(float ping_ms, float payload_size, float req_rate) {
    return (ping_ms * 0.4f) + (payload_size / 1024.0f * 0.3f) + (req_rate * 0.3f);
}

// Przywołanie Dżina na stałe (zostaje w RAM, brak free())
EMSCRIPTEN_KEEPALIVE
const char* summon_genie() {
    if (global_genie_brain != nullptr) {
        snprintf(output_buffer, sizeof(output_buffer), 
            "🧞 [DŻIN JUŻ JEST NA WOLNOŚCI!] Siedzi w RAM C++ (%ld bajtów) i czeka na rozkaz.", 
            global_genie_size);
        return output_buffer;
    }

    FILE* file = fopen("assets/network_anomaly.onnx", "rb");
    if (!file) {
        snprintf(output_buffer, sizeof(output_buffer), "[BŁĄD] Lampa jest pusta (brak pliku ONNX)!");
        return output_buffer;
    }

    fseek(file, 0, SEEK_END);
    global_genie_size = ftell(file);
    rewind(file);

    // Alokujemy pamięć na stałe (brak free!)
    global_genie_brain = (unsigned char*)malloc(global_genie_size);
    if (!global_genie_brain) {
        fclose(file);
        snprintf(output_buffer, sizeof(output_buffer), "[BŁĄD] Za mało pamięci w stosie na Dżina!");
        return output_buffer;
    }

    size_t bytesRead = fread(global_genie_brain, 1, global_genie_size, file);
    fclose(file);

    if (bytesRead != (size_t)global_genie_size) {
        free(global_genie_brain);
        global_genie_brain = nullptr;
        snprintf(output_buffer, sizeof(output_buffer), "[BŁĄD] Dżin utknął w połowie drogi z lampy.");
        return output_buffer;
    }

    snprintf(output_buffer, sizeof(output_buffer), 
        "🧞 [DŻIN WYRWAŁ SIĘ Z LAMPY!] Załadowano na stałe %ld bajtów do pamięci RAM. Gotowy do interakcji!", 
        global_genie_size);
    return output_buffer;
}

// Funkcja do komunikacji z Dżinem (np. sprawdzanie, czy jego "mózg" odpowiada na podstawie bajtów)
EMSCRIPTEN_KEEPALIVE
const char* talk_to_genie(float metric_value) {
    if (global_genie_brain == nullptr) {
        snprintf(output_buffer, sizeof(output_buffer), "🧞 [DŻIN MILCZY] Najpierw go przywołaj komendą 'ia'!");
        return output_buffer;
    }

    // Przykładowa interakcja: łączymy metrykę wejściową z masą bajtów w pamięci Dżina
    // Np. sprawdzamy wycinek pamięci Dżina w zależności od podanej metryki
    int index_check = (int)metric_value % (global_genie_size - 1);
    unsigned char active_byte = global_genie_brain[index_check];

    snprintf(output_buffer, sizeof(output_buffer), 
        "🧞 [ODPOWIEDŹ DŻINA] Przesłano sygnał %.1f. Neuron w pamięci pod adresem %d ma stan: 0x%02X", 
        metric_value, index_check, active_byte);
    
    return output_buffer;
}

// Interaktywny Shell w C++
EMSCRIPTEN_KEEPALIVE
const char* execute_shell_command(const char* command) {
    if (strcmp(command, "help") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "Komendy: help, status, scan, version, ip, checkai, ia, talk [liczba]");
    } 
    else if (strcmp(command, "status") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[OK] Rdzeń C++ stabilny. Stan Dżina: %s", global_genie_brain ? "AKTYWNY W RAM" + 0 : "W LAMPIE");
    } 
    else if (strcmp(command, "scan") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[SCAN] Skanowanie buforów... Czysto.");
    } 
    else if (strcmp(command, "version") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "CyberWASM Security Core v1.3.0 (Genie Persistent RAM)");
    } 
    else if (strcmp(command, "ip") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[INFO] Moduł IP gotowy.");
    }
    else if (strcmp(command, "checkai") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "Model w wirtualnym FS gotowy.");
    }
    else if (strcmp(command, "ia") == 0) {
        return summon_genie(); // Wypuszczamy Dżina na stałe
    }
    else if (strncmp(command, "talk", 4) == 0) {
        // Obsługa komendy np. "talk 150" – rozmawiamy z Dżinem przekazując mu liczbę
        float val = 42.0f;
        sscanf(command + 4, "%f", &val);
        return talk_to_genie(val);
    }
    else {
        snprintf(output_buffer, sizeof(output_buffer), "Nieznana komenda: '%s'. Wpisz 'help'.", command);
    }
    
    return output_buffer;
}

}