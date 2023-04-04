//Configuracao WatchDog
void setupWTD() {
  timer = timerBegin(0, 80, true); //timerID 0, div 80
  //timer, callback, interrupção de borda
  timerAttachInterrupt(timer, &resetModule, true);
  //timer, tempo (us), repetição
  timerAlarmWrite(timer, 60000000, true);   //10s
  timerAlarmEnable(timer);                  //habilita a interrupção
}

//Loop WatchDog
void loopWTD() {
  if (WiFi.status() == WL_CONNECTED) {
    Dprint("Reset WTD - WTD");
    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
  }
}

//Função que o temporizador irá chamar, para reiniciar o ESP32
void IRAM_ATTR resetModule() {
  ets_printf("WatchDog Acionado!! SGS Gateway Rebooting...");
  esp_restart();
}
