//Setup OTA WEB
void setupOTA_WEB() {
  //atende uma solicitação para OTA
  server.on("/verificaOTA", HTTP_GET, []() //atende uma solicitação para a OTA
  {
    Dprintln("Acessando Verifica Autorização");
    server.sendHeader("Connection", "close");
    verificaOTA = verificaOTA_page;
    server.send(200, "text/html", verificaOTA);
  });

  //atende uma solicitação para a página avalia autorizacao OTA
  server.on("/avalia", HTTP_POST, [] ()
  {
    Dprintln("Verificando Autorização: " + String(server.arg("autorizacao"))); //somente para debug

    if (server.arg("autorizacao") != "MBD@2022") // confere se o dado de autorização atende a avaliação
    {
      //se não atende, serve a página indicando uma falha
      server.sendHeader("Connection", "close");
      Resultado_Falha = Resultado_Falha_page;
      server.send(200, "text/html", Resultado_Falha);
      //ESP.restart();
    }
    else
    {
      //se atende, solicita a página de índice do servidor
      // e sinaliza que o OTA está autorizado
      OTA_AUTORIZADO = true;
      server.sendHeader("Connection", "close");
      serverIndex = serverIndex_page;
      server.send(200, "text/html", serverIndex);
    }
  });

  //serve a página de indice do servidor
  //para seleção do arquivo
  server.on("/serverIndex", HTTP_GET, []()
  {
    Dprintln("Acessando Página Atualização");
    server.sendHeader("Connection", "close");
    serverIndex = serverIndex_page;
    server.send(200, "text/html", serverIndex);
  });

  //tenta iniciar a atualização . . .
  server.on("/update", HTTP_POST, []()
  {
    Dprintln("Acessando Update");
    //verifica se a autorização é false.
    //Se for falsa, serve a página de erro e cancela o processo.
    if (OTA_AUTORIZADO == false)
    {
      server.sendHeader("Connection", "close");
      Resultado_Falha = Resultado_Falha_page;
      server.send(200, "text/html", Resultado_Falha);
      return;
    }
    //Serve uma página final que depende do resultado da atualização
    server.sendHeader("Connection", "close");
    Resultado_Falha = Resultado_Falha_page;
    Resultado_Ok = Resultado_Ok_page;
    server.send(200, "text/html", (Update.hasError()) ? Resultado_Falha : Resultado_Ok);
    delay(1000);
    ESP.restart();
  }, []() {
    //Mas estiver autorizado, inicia a atualização
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START)
    {
      Serial.setDebugOutput(true);
      Serial.printf("Atualizando: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN))
      {
        //se a atualização não iniciar, envia para serial mensagem de erro.
        Update.printError(Serial);
      }
    }
    else if (upload.status == UPLOAD_FILE_WRITE)
    {
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
      {
        //se não conseguiu escrever o arquivo, envia erro para serial
        Update.printError(Serial);
      }
    }
    else if (upload.status == UPLOAD_FILE_END)
    {
      if (Update.end(true))
      {
        //se finalizou a atualização, envia mensagem para a serial informando
        Serial.printf("Atualização bem sucedida! %u\nReiniciando...\n", upload.totalSize);
      }
      else
      {
        //se não finalizou a atualização, envia o erro para a serial.
        Update.printError(Serial);
      }
      Serial.setDebugOutput(true);
    }
    else
    {
      //se não conseguiu identificar a falha no processo, envia uma mensagem para a serial
      Serial.printf("Atualização falhou inesperadamente! (possivelmente a conexão foi perdida.): status=%d\n", upload.status);
    }
  });
}

//Loop OTA WEB
void loopOTA_WEB() {
  Dprint("Reset WTD - OTA_WEB");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)

  //Dprintln("WEBPage em Loop");
  //server.handleClient();

  //Informacoes atualizadas
  //chipID = (uint32_t)(ESP.getEfuseMac() >> 32); //um ID exclusivo do Chip...
  //CHIP_ID = "<p> Chip ID: " + String(chipID) + "</p>"; // montado para ser usado no HTML
  //VERSION = "<p> Versão: 1.0 </p>"; //Exemplo de um controle de versão
  //RunningTime = "<p> Tempo Funcionando: " + time_Alive() + "</p>";
  //INFOS = VERSION + CHIP_ID + RunningTime;

  //Páginas do OTA
  //verificaOTA = "<!DOCTYPE html><html><head><title>Atualização MBD Ensaio Temperatura</title><meta charset='UTF-8'><style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} .input[type=button], input[type=submit], input[type=reset] { background-color: #4CAF50; border: none; color: white; padding: 5px 30px;text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}</style></head><body><h1>MBD Ensaio Temperatura OTA</h1><h2>Digite a chave de verificação.<p>Clique em ok para continuar. . .</p></h2>" + INFOS + "<form method='POST' action='/avalia 'enctype='multipart/form-data'> <p><label>Autorização: </label><input type='password' name='autorizacao'></p><input type='submit' value='Ok'></form></body></html>";
  //serverIndex = "<!DOCTYPE html><html><head><title>Atualização MBD Ensaio Temperatura</title><meta charset='UTF-8'><style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} .input[type=button], input[type=submit], input[type=reset], input[type=file] { background-color: #4CAF50; border: none; color: white; padding: 5px 30px;text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}</style></head><body><h1>MBD Ensaio Temperatura OTA</h1><h2>Selecione o arquivo para a atualização e clique em atualizar.</h2>" + INFOS + "<form method='POST' action='/update' enctype='multipart/form-data'><p><input type='file' name='update'></p><p><input type='submit' value='Atualizar'></p></form></body></html>";
  //Resultado_Ok = "<!DOCTYPE html><html><head><title>Atualização MBD Ensaio Temperatura</title><meta charset='UTF-8'><style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} .input[type=button], input[type=submit], input[type=reset] { background-color: #4CAF50; border: none; color: white; padding: 5px 30px;text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}</style></head><body><h1>MBD Ensaio Temperatura OTA</h1><h2>Atualização bem sucedida!</h2>" + INFOS + "</body></html>";
  //Resultado_Falha = "<!DOCTYPE html><html><head><title>Atualização MBD Ensaio Temperatura</title><meta charset='UTF-8'><style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} .input[type=button], input[type=submit], input[type=reset] { background-color: #4CAF50; border: none; color: white; padding: 5px 30px;text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}</style></head><body><h1>MBD Ensaio Temperatura OTA</h1><h2>Falha durante a atualização. A versão anterior será recarregado.</h2>" + INFOS + "</body></html>";

}
