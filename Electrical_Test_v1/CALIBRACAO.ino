//Configuracao da Calibracao
void setupCALIBRACAO() {
  server.on("/calibracao", HTTP_GET, []() //atende uma solicitação para a raiz
  {
    Dprintln("Acessando Calibração");
    server.sendHeader("Connection", "close");
    calibracao = calibracao_page;
    server.send(200, "text/html", calibracao);
  });

  server.on("/sensorhtml", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Selecionando sensor");
    //server.sendHeader("Connection", "close");
    sensor_html = server.arg("sensor");
    findParametros(sensor_html);
    //calibracao = calibracao_page;
    //server.send(200, "text/html", calibracao);
  });

  server.on("/m1html", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Medidas 1 guardadas");
    //server.sendHeader("Connection", "close");
    x1_html = server.arg("Medida1_x1");
    x2_html = server.arg("Medida1_x2");
    flagm1ready = true;
    //calibracao = calibracao_page;
    //server.send(200, "text/html", calibracao);
  });

  server.on("/m1read", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Atualizacao Medida 1");
    //server.sendHeader("Connection", "close");
    float temp = 0;
    if (!flagm1ready) {
      temp = leSensor(sensor_html);
    }
    server.send(200, "text/html", String(temp));
  });

  server.on("/m2html", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Medidas 2 guardadas");
    //server.sendHeader("Connection", "close");
    y1_html = server.arg("Medida2_y1");
    y2_html = server.arg("Medida2_y2");
    flagm2ready = true;
    //calibracao = calibracao_page;
    //server.send(200, "text/html", calibracao);
  });

  server.on("/m2read", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Atualizacao Medida 2");
    //server.sendHeader("Connection", "close");
    float temp = 0;
    if (!flagm2ready) {
      temp = leSensor(sensor_html);
    }
    server.send(200, "text/html", String(temp));
  });

  server.on("/calculahtml", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Calculando Parametros...");
    //server.sendHeader("Connection", "close");
    calculaCoeficientes(x1_html.toFloat(), x2_html.toFloat(), y1_html.toFloat(), y2_html.toFloat());
    //calibracao = calibracao_page;
    //server.send(200, "text/html", calibracao);
  });

  server.on("/alfaatual", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Atualizacao Medida 2");
    //server.sendHeader("Connection", "close");
    if (sensor_html != "") {
      server.send(200, "text/html", String(alfa_atual));
    }
  });

  server.on("/betaatual", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Atualizacao Medida 2");
    //server.sendHeader("Connection", "close");
    if (sensor_html != "") {
      server.send(200, "text/html", String(beta_atual));
    }
  });

  server.on("/alfacalculado", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Atualizacao Medida 2");
    //server.sendHeader("Connection", "close");
    server.send(200, "text/html", String(alfa_html));
  });

  server.on("/betacalculado", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Atualizacao Medida 2");
    //server.sendHeader("Connection", "close");
    server.send(200, "text/html", String(beta_html));
  });

  server.on("/aceitahtml", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Atualizando parametros");
    server.sendHeader("Connection", "close");
    flagCalibSensor = true;
    raiz = MAIN_page;
    server.send(200, "text/html", raiz);
  });

  server.on("/reloadhtml", HTTP_POST, []() //atende uma solicitação para a raiz
  {
    Dprintln("Recarregando página...");
    server.sendHeader("Connection", "close");
    flagm1ready = false;
    flagm2ready = false;
    x1_html = "";
    x2_html = "";
    y1_html = "";
    y2_html = "";
    alfa_atual = 0;
    beta_atual = 0;
    alfa_html = 0;
    beta_html = 0;
    sensor_html = "";
    calibracao = calibracao_page;
    server.send(200, "text/html", calibracao);
  });

}

//Loop Calibracao
void loopCALIBRACAO() {
  Dprint("Reset WTD - CALIBRACAO");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
  if (flagCalibSensor) {
    //Significa que algum sensor foi calibrado, é necessário atualizar o arquivo TX no SPIFFS
    if (sensor_html = "DHT1_T") {
      DHT1T_alfa = alfa_html;
      DHT1T_beta = beta_html;
      preferences.putFloat("DHT1T_alfa", alfa_html);
      preferences.putFloat("DHT1T_beta", beta_html);
    }
    else if (sensor_html = "DHT2_T") {
      DHT2T_alfa = alfa_html;
      DHT2T_beta = beta_html;
      preferences.putFloat("DHT2T_alfa", alfa_html);
      preferences.putFloat("DHT2T_beta", beta_html);
    }
    else if (sensor_html = "DHT3_T") {
      DHT3T_alfa = alfa_html;
      DHT3T_beta = beta_html;
      preferences.putFloat("DHT3T_alfa", alfa_html);
      preferences.putFloat("DHT3T_beta", beta_html);
    }
    else if (sensor_html = "DHT4_T") {
      DHT4T_alfa = alfa_html;
      DHT4T_beta = beta_html;
      preferences.putFloat("DHT4T_alfa", alfa_html);
      preferences.putFloat("DHT4T_beta", beta_html);
    }
    else if (sensor_html = "DHT1_U") {
      DHT1U_alfa = alfa_html;
      DHT1U_beta = beta_html;
      preferences.putFloat("DHT1U_alfa", alfa_html);
      preferences.putFloat("DHT1U_beta", beta_html);
    }
    else if (sensor_html = "DHT2_U") {
      DHT2U_alfa = alfa_html;
      DHT2U_beta = beta_html;
      preferences.putFloat("DHT2U_alfa", alfa_html);
      preferences.putFloat("DHT2U_beta", 0);
    }
    else if (sensor_html = "DHT3_U") {
      DHT3U_alfa = alfa_html;
      DHT3U_beta = beta_html;
      preferences.putFloat("DHT3U_alfa", alfa_html);
      preferences.putFloat("DHT3U_beta", beta_html);
    }
    else if (sensor_html = "DHT4_U") {
      DHT4U_alfa = alfa_html;
      DHT4U_beta = beta_html;
      preferences.putFloat("DHT4U_alfa", alfa_html);
      preferences.putFloat("DHT4U_beta", beta_html);
    }
    else if (sensor_html = "TERMOPAR1") {
      Termo1_alfa = alfa_html;
      Termo1_beta = beta_html;
      preferences.putFloat("Termo1_alfa", alfa_html);
      preferences.putFloat("Termo1_beta", beta_html);
    }
    else if (sensor_html = "TERMOPAR2") {
      Termo2_alfa = alfa_html;
      Termo2_beta = beta_html;
      preferences.putFloat("Termo2_alfa", alfa_html);
      preferences.putFloat("Termo2_beta", beta_html);
    }
    else if (sensor_html = "TERMOPAR3") {
      Termo3_alfa = alfa_html;
      Termo3_beta = beta_html;
      preferences.putFloat("Termo3_alfa", alfa_html);
      preferences.putFloat("Termo3_beta", beta_html);
    }
    else if (sensor_html = "TERMOPAR4") {
      Termo4_alfa = alfa_html;
      Termo4_beta = beta_html;
      preferences.putFloat("Termo4_alfa", alfa_html);
      preferences.putFloat("Termo4_beta", beta_html);
    }
    else if (sensor_html = "TERMOPAR5") {
      Termo5_alfa = alfa_html;
      Termo5_beta = beta_html;
      preferences.putFloat("Termo5_alfa", alfa_html);
      preferences.putFloat("Termo5_beta", beta_html);
    }
    else if (sensor_html = "TENSAO_BAIXA1") {
      TB1_alfa = alfa_html;
      TB1_beta = beta_html;
      preferences.putFloat("TB1_alfa", alfa_html);
      preferences.putFloat("TB1_beta", beta_html);
    }
    else if (sensor_html = "TENSAO_ALTA1") {
      TA1_alfa = alfa_html;
      TA1_beta = beta_html;
      preferences.putFloat("TA1_alfa", alfa_html);
      preferences.putFloat("TA1_beta", beta_html);
    }
    else if (sensor_html = "CORRENTE_BAIXA1") {
      CB1_alfa = alfa_html;
      CB1_beta = beta_html;
      preferences.putFloat("CB1_alfa", alfa_html);
      preferences.putFloat("CB1_beta", beta_html);
    }
    else if (sensor_html = "CORRENTE_ALTA1") {
      CA1_alfa = alfa_html;
      CA1_beta = beta_html;
      preferences.putFloat("CA1_alfa", alfa_html);
      preferences.putFloat("CA1_beta", beta_html);
    }
    preferences.end();
    flagCalibSensor = false;
  }
}

void calculaCoeficientes(float X1, float X2, float Y1, float Y2) {
  alfa_html = (Y2 - Y1) / (X2 - X1);
  beta_html = Y1 - alfa_html * X1;
}

void findParametros (String sensor) {
  if (sensor = "DHT1_T") {
    alfa_atual = DHT1T_alfa;
    beta_atual = DHT1T_beta;
  }
  else if (sensor = "DHT2_T") {
    alfa_atual = DHT2T_alfa;
    beta_atual = DHT2T_beta;
  }
  else if (sensor = "DHT3_T") {
    alfa_atual = DHT3T_alfa;
    beta_atual = DHT3T_beta;
  }
  else if (sensor = "DHT4_T") {
    alfa_atual = DHT4T_alfa;
    beta_atual = DHT4T_beta;
  }
  else if (sensor = "DHT1_U") {
    alfa_atual = DHT1U_alfa;
    beta_atual = DHT1U_beta;
  }
  else if (sensor = "DHT2_U") {
    alfa_atual = DHT2U_alfa;
    beta_atual = DHT2U_beta;
  }
  else if (sensor = "DHT3_U") {
    alfa_atual = DHT3U_alfa;
    beta_atual = DHT3U_beta;
  }
  else if (sensor = "DHT4_U") {
    alfa_atual = DHT4U_alfa;
    beta_atual = DHT4U_beta;
  }
  else if (sensor = "TERMOPAR1") {
    alfa_atual = Termo1_alfa;
    beta_atual = Termo1_beta;
  }
  else if (sensor = "TERMOPAR2") {
    alfa_atual = Termo2_alfa;
    beta_atual = Termo2_beta;
  }
  else if (sensor = "TERMOPAR3") {
    alfa_atual = Termo3_alfa;
    beta_atual = Termo3_beta;
  }
  else if (sensor = "TERMOPAR4") {
    alfa_atual = Termo4_alfa;
    beta_atual = Termo4_beta;
  }
  else if (sensor = "TERMOPAR5") {
    alfa_atual = Termo5_alfa;
    beta_atual = Termo5_beta;
  }
  else if (sensor = "TENSAO_BAIXA1") {
    alfa_atual = TB1_alfa;
    beta_atual = TB1_beta;
  }
  else if (sensor = "TENSAO_ALTA1") {
    alfa_atual = TA1_alfa;
    beta_atual = TA1_beta;
  }
  else if (sensor = "CORRENTE_BAIXA1") {
    alfa_atual = CB1_alfa;
    beta_atual = CB1_beta;
  }
  else if (sensor = "CORRENTE_ALTA1") {
    alfa_atual = CA1_alfa;
    beta_atual = CA1_beta;
  }
}

float leSensor(String sensor) {
  if (sensor = "DHT1_T") {
    float temp;
    temp = readDHT(1, 1);
  }
  else if (sensor = "DHT2_T") {
    float temp;
    temp = readDHT(2, 1);
  }
  else if (sensor = "DHT3_T") {
    float temp;
    temp = readDHT(3, 1);
  }
  else if (sensor = "DHT4_T") {
    float temp;
    temp = readDHT(4, 1);
  }
  else if (sensor = "DHT1_U") {
    float temp;
    temp = readDHT(1, 2);
  }
  else if (sensor = "DHT2_U") {
    float temp;
    temp = readDHT(2, 2);
  }
  else if (sensor = "DHT3_U") {
    float temp;
    temp = readDHT(3, 2);
  }
  else if (sensor = "DHT4_U") {
    float temp;
    temp = readDHT(4, 2);
  }
  else if (sensor = "TERMOPAR1") {
    float temp;
    temp = readTERMOPAR(1);
  }
  else if (sensor = "TERMOPAR2") {
    float temp;
    temp = readTERMOPAR(2);
  }
  else if (sensor = "TERMOPAR3") {
    float temp;
    temp = readTERMOPAR(3);
  }
  else if (sensor = "TERMOPAR4") {
    float temp;
    temp = readTERMOPAR(4);
  }
  else if (sensor = "TERMOPAR5") {
    float temp;
    temp = readTERMOPAR(5);
  }
  else if (sensor = "TENSAO_BAIXA1") {
    float temp;
    temp = readADC(1);
  }
  else if (sensor = "TENSAO_ALTA1") {
    float temp;
    temp = readADC(3);
  }
  else if (sensor = "CORRENTE_BAIXA1") {
    float temp;
    temp = readADC(2);
  }
  else if (sensor = "CORRENTE_ALTA1") {
    float temp;
    temp = readADC(4);
  }
}

//Referencia de Ajuste Tela sem refresh completo
//https://circuits4you.com/2018/02/04/esp8266-ajax-update-part-of-web-page-without-refreshing/
