#include <emscripten/emscripten.h>
#include <stdio.h>
#include <string.h>

static char output_buffer[512];

// GLOBALNY DŻIN: Pamięć RAM, w której model mieszka na stałe
static unsigned char* global_genie_brain = nullptr;
static long global_genie_size = 0;

// Struktura wpisu na blogu
struct BlogPost {
    const char* keyword;
    const char* title;
    const char* url;
};

// Automatycznie wygenerowana baza wiedzy na podstawie Twojego sitemap.xml
static BlogPost blog_index[] = {
    {"bomberman", "Bomberman Play (WASM/C++)", "https://notepadpl.blogspot.com/2026/10/bomberman-play.html"},
    {"server", "Server C / TCP Console", "https://notepadpl.blogspot.com/2024/08/tcp-c-server-console.html"},
    {"tinyc", "TinyC 2026", "https://notepadpl.blogspot.com/2026/05/tinyc-2026-v.html"},
    {"sip", "SIP/C++ Serwer & Klient", "https://notepadpl.blogspot.com/2025/08/sipcpp_29.html"},
    {"fbx", "SDL Assimp GL FBX YML", "https://notepadpl.blogspot.com/2025/08/sdl-assimp-gl-fbx-yml.html"},
    {"wasm", "WebAssembly & OpenGL", "https://notepadpl.blogspot.com/2025/07/webasembly-gl.html"},
    {"tetris", "Tetris w WebAssembly", "https://notepadpl.blogspot.com/2025/07/tetris-wasm.html"},
    {"snake", "Snake w WebAssembly", "https://notepadpl.blogspot.com/2025/07/snake-wasm.html"},
    {"telnet", "Telnet / Klient FTP w C", "https://notepadpl.blogspot.com/2024/09/telnetc-clijent-ftp.html"},
    {"obj", "SDL2 & OpenGL - Ładowanie modeli OBJ", "https://notepadpl.blogspot.com/2024/09/sdl2-opengl-load-obj.html"},
    {"texture", "Próby tekstur w OpenGL", "https://notepadpl.blogspot.com/2024/09/try-texture.html"},
    {"nmap", "Nmap w C", "https://notepadpl.blogspot.com/2024/08/nmapc.html"},
    {"shellsock", "Shellshock w C", "https://notepadpl.blogspot.com/2023/05/shellsockc.html"},
    {"clang", "Clang, LLVM i kompilacja", "https://notepadpl.blogspot.com/2023/04/clang.html"}
};

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

// 1. Przywołanie Dżina do pamięci RAM na stałe
EMSCRIPTEN_KEEPALIVE
const char* summon_genie() {
    if (global_genie_brain != nullptr) {
        snprintf(output_buffer, sizeof(output_buffer), 
            "🧞 [DŻIN AKTYWNY] Siedzi w RAM C++ (%ld bajtów) i indeksuje bloga.", 
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

    global_genie_brain = (unsigned char*)malloc(global_genie_size);
    if (!global_genie_brain) {
        fclose(file);
        snprintf(output_buffer, sizeof(output_buffer), "[BŁĄD] Za mało pamięci na stercie dla Dżina!");
        return output_buffer;
    }

    size_t bytesRead = fread(global_genie_brain, 1, global_genie_size, file);
    fclose(file);

    if (bytesRead != (size_t)global_genie_size) {
        free(global_genie_brain);
        global_genie_brain = nullptr;
        snprintf(output_buffer, sizeof(output_buffer), "[BŁĄD] Dżin utknął podczas wychodzenia z lampy.");
        return output_buffer;
    }

    snprintf(output_buffer, sizeof(output_buffer), 
        "🧞 [DŻIN WYRWAŁ SIĘ Z LAMPY!] Załadowano model (%ld bajtów). Baza bloga gotowa!", 
        global_genie_size);
    return output_buffer;
}
// Dodaj tę funkcję w bloku extern "C"
EMSCRIPTEN_KEEPALIVE
const char* ask_external_ai(const char* prompt) {
    snprintf(output_buffer, sizeof(output_buffer), "🤖 [C++ -> PHP cURL Proxy]: %s", prompt);
    return output_buffer;
}

// W funkcji execute_shell_command dodaj obsługę komendy 'askai':
else if (strncmp(command, "askai ", 6) == 0) {
    const char* prompt = command + 6;
    while (*prompt == ' ') prompt++;
    return ask_external_ai(prompt);
}
// 2. Rozmowa z pamięcią modelową Dżina
EMSCRIPTEN_KEEPALIVE
const char* talk_to_genie(float metric_value) {
    if (global_genie_brain == nullptr) {
        snprintf(output_buffer, sizeof(output_buffer), "🧞 [DŻIN MILCZY] Najpierw przywołaj go komendą 'ia'!");
        return output_buffer;
    }

    int index_check = (int)metric_value % (global_genie_size - 1);
    unsigned char active_byte = global_genie_brain[index_check];

    snprintf(output_buffer, sizeof(output_buffer), 
        "🧞 [STAN NEURONU] Adres pamięci %d: 0x%02X", index_check, active_byte);
    
    return output_buffer;
}

// 3. Inteligentne wyszukiwanie w mapie bloga
EMSCRIPTEN_KEEPALIVE
const char* ask_genie_blog(const char* query) {
    for (int i = 0; i < sizeof(blog_index) / sizeof(BlogPost); i++) {
        // Proste dopasowanie słowa kluczowego (case-insensitive lub bezpośrednie)
        if (strstr(query, blog_index[i].keyword) != NULL) {
            snprintf(output_buffer, sizeof(output_buffer), 
                "🧞 [Dżin poleca artykuł]: <b>%s</b> -> <a href='%s' target='_blank'>Otwórz wpis</a>", 
                blog_index[i].title, blog_index[i].url);
            return output_buffer;
        }
    }
    
    snprintf(output_buffer, sizeof(output_buffer), "🧞 [Dżin szuka w sitemap]: Nie znalazłem artykułu dla zapytania '%s'. Wpisz 'help'.", query);
    return output_buffer;
}

// 4. Interaktywny Shell w C++
EMSCRIPTEN_KEEPALIVE
const char* execute_shell_command(const char* command) {
    if (strcmp(command, "help") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "Komendy: help, status, ip, ia, talk [liczba], ask [tetris/wasm/server/obj/nmap]");
    } 
    else if (strcmp(command, "status") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[OK] Rdzeń C++ stabilny. Dżin w RAM: %s", global_genie_brain ? "AKTYWNY" : "W LAMPIE");
    } 
    else if (strcmp(command, "ip") == 0) {
        snprintf(output_buffer, sizeof(output_buffer), "[INFO] Moduł sieciowy gotowy.");
    }
    else if (strcmp(command, "ia") == 0) {
        return summon_genie();
    }
    else if (strncmp(command, "talk", 4) == 0) {
        float val = 42.0f;
        sscanf(command + 4, "%f", &val);
        return talk_to_genie(val);
    }
    else if (strncmp(command, "ask", 3) == 0) {
        const char* query = command + 3;
        while (*query == ' ') query++;
        return ask_genie_blog(query);
    }
    else {
        snprintf(output_buffer, sizeof(output_buffer), "Nieznana komenda: '%s'. Wpisz 'help'.", command);
    }
    
    return output_buffer;
}

}