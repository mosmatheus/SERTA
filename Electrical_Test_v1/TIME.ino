//Configuracao do Time Time
void setupTIME() {
  timeClient.begin();               /* Inicia o protocolo */
  timeClient.forceUpdate();    /* Atualização */
  timeClient.setTimeOffset(-3 * 3600);
}

//Loop do Time
void loopTIME() {
  Dprint("Reset WTD - TIME");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)

  // Armazena na data e hora
  formattedDate = timeClient.getFormattedDate();
  Dprintln(formattedDate);     /* Escreve a hora no monitor serial. */
  //delay(1000);              /* Espera 1 segundo. */
}

//Função para Normalizar e passar para String
String normaliza(int num) {
  String str_num = "";
  if (num < 10) {
    str_num = "0" + String(num);
  }
  else
    str_num = String(num);
  return (str_num);
}

//Loop Alive
String time_Alive() {
  //Variável Tempo Ligado
  int64_t ttime = esp_timer_get_time();
  String ttimeAlive;
  int segundos = 0;
  int minutos = 0;
  int horas = 0;
  segundos = ( ( ttime / 1000000 ) % 60 );
  minutos =  ( ( ttime / 1000000 ) % 3600 ) / 60;
  horas = ( ( ttime / 1000000 ) / 3600 );
  //Dprintln("Segundos: " + String(segundos));
  //Dprintln("Minutos: " + String(minutos));
  //Dprintln("Hora: " + String(horas));
  ttimeAlive = String(normaliza(horas)) + ":" + String(normaliza(minutos)) + ":" + String(normaliza(segundos));
  //Dprintln(ttimeAlive);
  return (ttimeAlive);
}
