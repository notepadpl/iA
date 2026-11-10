<?php
// ai-proxy.php

// Ustawienie nagłówków (obsługa CORS, jeśli blog stoi na innej domenie/porcie)
header('Content-Type: application/json; charset=utf-8');
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Methods: POST, OPTIONS');
header('Access-Control-Allow-Headers: Content-Type');

// Obsługa zapytania wstępnego OPTIONS (CORS preflight)
if ($_SERVER['REQUEST_METHOD'] === 'OPTIONS') {
    http_response_code(200);
    exit();
}

// Sprawdzenie czy to żądanie POST
if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    echo json_encode(['error' => 'Dozwolona jest tylko metoda POST']);
    exit();
}

// Odczytanie surowego JSON-a wysłanego z przeglądarki
$inputJSON = file_get_contents('php://input');
$input = json_decode($inputJSON, true);
$prompt = $input['prompt'] ?? '';

if (empty(trim($prompt))) {
    echo json_encode(['error' => 'Brak zapytania (prompt)']);
    exit();
}

// Konfiguracja danych do zewnętrznego API (np. OpenAI lub kompatybilnego z OpenAI jak Groq / DeepSeek)
$apiKey = 'TWÓJ_TAJNY_KLUCZ_API_TUTAJ'; // Wklej tutaj swój klucz API
$apiUrl = 'https://api.openai.com/v1/chat/completions'; // Możesz zmienić na np. Groq lub inny endpoint

$payload = json_encode([
    'model' => 'gpt-4o-mini', // Możesz zmienić na inny model
    'messages' => [
        [
            'role' => 'system', 
            'content' => 'Jesteś pomocnym asystentem AI w terminalu bloga technicznego.'
        ],
        [
            'role' => 'user', 
            'content' => $prompt
        ]
    ],
    'temperature' => 0.7
]);

// Inicjalizacja cURL w PHP
$ch = curl_init($apiUrl);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch, CURLOPT_POST, true);
curl_setopt($ch, CURLOPT_POSTFIELDS, $payload);
curl_setopt($ch, CURLOPT_HTTPHEADER, [
    'Content-Type: application/json',
    'Authorization: Bearer ' . $apiKey
]);

// Wykonanie zapytania
$response = curl_exec($ch);
$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
$curlError = curl_error($ch);
curl_close($ch);

// Obsługa błędów cURL
if ($curlError) {
    echo json_encode(['error' => 'Błąd cURL: ' . $curlError]);
    exit();
}

// Zwrócenie odpowiedzi z zewnętrznego API do przeglądarki
http_response_code($httpCode);
echo $response;
?>