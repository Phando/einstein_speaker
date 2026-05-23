#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <ESPAsyncWebServer.h>
#include <Preferences.h>
#include <Update.h>
#include "EinsteinSerial.h"
#include "WebUI.h"

EinsteinSerial einstein;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
DNSServer dnsServer;
Preferences prefs;

bool apMode = false;
unsigned long lastIpDisplay = 0;
const unsigned long IP_DISPLAY_INTERVAL = 600000; // 10 minutes
bool bootOffSent = false;

String savedSSID;
String savedPass;

void handleWebSocketMessage(AsyncWebSocketClient *client, String msg) {
    Serial.println("WS: " + msg);

    if (msg == "OFF") {
        einstein.allOff();
    }
    else if (msg == "ON:all_off") {
        einstein.allOff();
    }
    else if (msg == "ON:white_listen") {
        einstein.whiteListen();
    }
    else if (msg == "ON:color_listen") {
        einstein.colorListen("0176D3");
    }
    else if (msg == "ON:white_speak") {
        einstein.whiteSpeak();
    }
    else if (msg == "ON:color_speak") {
        einstein.colorSpeak("0176D3");
    }
    else if (msg == "ON:speak_coral") {
        einstein.colorSpeak("FF6B5B");
    }
    else if (msg == "ON:speak_amber") {
        einstein.colorSpeak("FFAA00");
    }
    else if (msg == "ON:speak_lime") {
        einstein.colorSpeak("A8E000");
    }
    else if (msg == "ON:speak_cyan") {
        einstein.colorSpeak("00E5CC");
    }
    else if (msg == "ON:speak_turquoise") {
        einstein.colorSpeak("5BC5D4");
    }
    else if (msg == "ON:speak_purple") {
        einstein.colorSpeak("7B2FBE");
    }
    else if (msg == "ON:speak_magenta") {
        einstein.colorSpeak("D4007A");
    }
    else if (msg == "ON:speak_pink") {
        einstein.colorSpeak("FF69B4");
    }
    else if (msg == "ON:speak_lavender") {
        einstein.colorSpeak("B39DDB");
    }
    else if (msg == "ON:crazy") {
        einstein.crazyHair();
        einstein.sendCommand(13);
    }
    else if (msg == "ON:mic_on") {
        einstein.micOn();
    }
    else if (msg == "ON:mic_off") {
        einstein.micOff();
    }
    else if (msg.startsWith("WIFI:")) {
        String payload = msg.substring(5);
        int sep = payload.indexOf(':');
        if (sep > 0) {
            String ssid = payload.substring(0, sep);
            String pass = payload.substring(sep + 1);
            prefs.begin("wifi", false);
            prefs.putString("ssid", ssid);
            prefs.putString("pass", pass);
            prefs.end();
            delay(500);
            ESP.restart();
        }
    }
    else if (msg == "FORGET") {
        prefs.begin("wifi", false);
        prefs.clear();
        prefs.end();
        delay(500);
        ESP.restart();
    }
}

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
               AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        String status;
        if (apMode) {
            status = "STATUS:ap";
        } else {
            status = "STATUS:sta:" + savedSSID + ":" + WiFi.localIP().toString();
        }
        client->text(status);
    }
    else if (type == WS_EVT_DATA) {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
            String msg = String((char *)data, len);
            handleWebSocketMessage(client, msg);
        }
    }
}

void displayIP() {
    if (!apMode) {
        einstein.setText(WiFi.localIP().toString());
    }
}

bool connectToWiFi() {
    prefs.begin("wifi", true);
    savedSSID = prefs.getString("ssid", "");
    savedPass = prefs.getString("pass", "");
    prefs.end();

    if (savedSSID.length() == 0) {
        return false;
    }

    Serial.println("Connecting to: " + savedSSID);
    WiFi.mode(WIFI_STA);
    WiFi.begin(savedSSID.c_str(), savedPass.c_str());

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
        delay(250);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected! IP: " + WiFi.localIP().toString());
        return true;
    }

    Serial.println("Connection failed.");
    WiFi.disconnect();
    return false;
}

void startAP() {
    apMode = true;
    String mac = WiFi.macAddress();
    String suffix = mac.substring(mac.length() - 5);
    suffix.replace(":", "");
    String apName = "Einstein-" + suffix;

    WiFi.mode(WIFI_AP);
    WiFi.softAP(apName.c_str());
    delay(100);

    Serial.println("AP Mode: " + apName);
    Serial.println("IP: " + WiFi.softAPIP().toString());

    dnsServer.start(53, "*", WiFi.softAPIP());
    einstein.setText(apName);
}

void setupServer() {
    ws.onEvent(onWsEvent);
    server.addHandler(&ws);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/html", index_html);
    });

    // OTA firmware update
    server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request) {
        bool success = !Update.hasError();
        request->send(200, "text/plain", success ? "OK" : "FAIL");
        if (success) {
            delay(500);
            ESP.restart();
        }
    }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
        if (index == 0) {
            Serial.println("OTA: " + filename);
            if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {
                Update.printError(Serial);
            }
        }
        if (Update.isRunning()) {
            if (Update.write(data, len) != len) {
                Update.printError(Serial);
            }
        }
        if (final) {
            if (!Update.end(true)) {
                Update.printError(Serial);
            } else {
                Serial.println("OTA complete.");
            }
        }
    });

    // Captive portal detection endpoints
    server.on("/generate_204", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/");
    });
    server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/");
    });
    server.on("/connecttest.txt", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/");
    });
    server.on("/ncsi.txt", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/");
    });
    server.on("/fwlink", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/");
    });

    // Catch-all for captive portal
    server.onNotFound([](AsyncWebServerRequest *request) {
        request->redirect("/");
    });

    server.begin();
    Serial.println("Web server started.");
}

void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("\n--- Einstein Remote ---");

    einstein.begin();
    delay(500);
    einstein.allOff();

    if (connectToWiFi()) {
        apMode = false;
        displayIP();
        lastIpDisplay = millis();
    } else {
        startAP();
    }

    setupServer();
}

void loop() {
    if (apMode) {
        dnsServer.processNextRequest();
    }

    if (!bootOffSent && millis() >= 20000) {
        einstein.allOff();
        bootOffSent = true;
    }

    // Display IP on belly every 10 minutes in STA mode
    if (!apMode && millis() - lastIpDisplay >= IP_DISPLAY_INTERVAL) {
        displayIP();
        lastIpDisplay = millis();
    }

    ws.cleanupClients();
}
