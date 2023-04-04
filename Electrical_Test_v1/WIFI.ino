//Configuracao Wifi
void setupWIFI() {
  String ssid_AP = "MBD_AP_1";
  const char* password_AP = "MBD@2022";
  const char* ssid_WIFI_c     = ssid_WIFI.c_str();
  const char* password_WIFI_c = password_WIFI.c_str();

  for (int i = 0; i <= 5; i++) {
    WiFi.begin(ssid_WIFI_c, password_WIFI_c);
    delay(1000);
    if (WiFi.status() != WL_CONNECTED) {
      Dprintln("Tentativa " + String(i));
      //delay(1000);
    }
    else {
      Dprint("Conectado com Sucesso - IP: ");
      Dprintln(WiFi.localIP());
      //setupWTD();
      break;
    }
  }

  if ((WiFi.status() != WL_CONNECTED)) {
    WiFi.softAP(ssid_AP.c_str(), password_AP);
    Dprint("AP IP address: ");
    Dprintln(WiFi.softAPIP());
  }
}

//Loop Wifi
void loopWIFI() {
  Dprint("Reset WTD - WIFI");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
}
