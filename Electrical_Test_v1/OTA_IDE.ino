//Configuracao OTA_IDE
void setupOTA_IDE() {
  // A porta fica default como 3232
  //ArduinoOTA.setPort(3232);

  // Define o hostname (opcional)
  ArduinoOTA.setHostname("SGS_Updater");

  // Define a senha (opcional)
  ArduinoOTA.setPassword("PUC2004@2020");

  // É possível definir uma criptografia hash md5 para a senha usando a função "setPasswordHash"
  // Exemplo de MD5 para senha "admin" = 21232f297a57a5a743894a0e4a801fc3
  ArduinoOTA.setPasswordHash("e5f3fd2528e2eb4ac6927a350095193f"); //MD5 para PUC2004@2020

  //define o que será executado quando o ArduinoOTA iniciar
  ArduinoOTA.onStart( startOTA ); //startOTA é uma função criada para simplificar o código

  //define o que será executado quando o ArduinoOTA terminar
  ArduinoOTA.onEnd( endOTA ); //endOTA é uma função criada para simplificar o código

  //define o que será executado quando o ArduinoOTA estiver gravando
  ArduinoOTA.onProgress( progressOTA ); //progressOTA é uma função criada para simplificar o código

  //define o que será executado quando o ArduinoOTA encontrar um erro
  ArduinoOTA.onError( errorOTA );//errorOTA é uma função criada para simplificar o código

  //inicializa ArduinoOTA
  ArduinoOTA.begin();
}

//Loop OTA
void loopOTA_IDE() {
  Dprint("Reset WTD - OTA_IDE");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
  //Handle é descritor que referencia variáveis no bloco de memória
  //Ele é usado como um "guia" para que o ESP possa se comunicar com o computador pela rede
  ArduinoOTA.handle();
}

//funções de exibição dos estágios de upload (start, progress, end e error) do ArduinoOTA
void startOTA()
{
  String type;

  //caso a atualização esteja sendo gravada na memória flash externa, então informa "flash"
  if (ArduinoOTA.getCommand() == U_FLASH) {
    Dprintln("Gravando atualização na Flash");
    type = "flash";
    SPIFFS.end();
  }
  else {  //caso a atualização seja feita pela memória interna (file system), então informa "filesystem"
    Dprintln("Gravando atualização na Memória Interna");
    type = "filesystem"; // U_SPIFFS
    SPIFFS.end();
  }

  //exibe mensagem junto ao tipo de gravação
  Dprintln("Começando atualização: " + type);

  digitalWrite(LED_Self_Test, HIGH);
  delay(300);
  digitalWrite(LED_Self_Test, LOW);
  delay(300);
  digitalWrite(LED_Self_Test, HIGH);
  delay(300);
  digitalWrite(LED_Self_Test, LOW);
  delay(300);
}

//exibe mensagem
void endOTA()
{
  Dprintln("\nFim da Atualização");
}

//exibe progresso em porcentagem
void progressOTA(unsigned int progress, unsigned int total)
{
  Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
}

//caso aconteça algum erro, exibe especificamente o tipo do erro
void errorOTA(ota_error_t error)
{
  Serial.printf("Erro[%u]: ", error);

  if (error == OTA_AUTH_ERROR)
    Dprintln("Autenticação Falhou");
  else if (error == OTA_BEGIN_ERROR)
    Dprintln("Inícialização Falhou");
  else if (error == OTA_CONNECT_ERROR)
    Dprintln("Conexão Falhou");
  else if (error == OTA_RECEIVE_ERROR)
    Dprintln("Recepção Falhou");
  else if (error == OTA_END_ERROR)
    Dprintln("Finalização Falhou");
}
