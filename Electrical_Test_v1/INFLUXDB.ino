//Configuracao Influx DB
void setupINFLUXDB() {

// Add tags
  sensor.addTag("DHT1_T", String(DHT1_T, 3));
  sensor.addTag("DHT2_T", String(DHT2_T, 3));
  sensor.addTag("DHT3_T", String(DHT3_T, 3));
  sensor.addTag("DHT4_T", String(DHT4_T, 3));
  sensor.addTag("DHT1_U", String(DHT1_U, 3));
  sensor.addTag("DHT2_U", String(DHT2_U, 3));
  sensor.addTag("DHT3_U", String(DHT3_U, 3));
  sensor.addTag("DHT4_U", String(DHT4_U, 3));
  sensor.addTag("TERMOPAR1", String(TERMOPAR1, 3));
  sensor.addTag("TERMOPAR2", String(TERMOPAR2, 3));
  sensor.addTag("TERMOPAR3", String(TERMOPAR3, 3));
  sensor.addTag("TERMOPAR4", String(TERMOPAR4, 3));
  sensor.addTag("TERMOPAR5", String(TERMOPAR5, 3));
  sensor.addTag("TENSAO_BAIXA", String(TENSAO_BAIXA, 3));
  sensor.addTag("TENSAO_ALTA", String(TENSAO_ALTA, 3));
  sensor.addTag("CORRENTE_BAIXA", String(CORRENTE_BAIXA, 3));
  sensor.addTag("CORRENTE_ALTA", String(CORRENTE_ALTA, 3));

  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

//Loop Influx DB
void loopINFLUXDB() {
  Dprint("Reset WTD - INFLUXDB");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
  if (flagSyncInflux) {

    // Limpa os campos
    sensor.clearFields();

    //Define os campos
    sensor.addField("DHT1_T", DHT1_T);
    sensor.addField("DHT2_T", DHT2_T);
    sensor.addField("DHT3_T", DHT3_T);
    sensor.addField("DHT4_T", DHT4_T);
    sensor.addField("DHT1_U", DHT1_U);
    sensor.addField("DHT2_U", DHT2_U);
    sensor.addField("DHT3_U", DHT3_U);
    sensor.addField("DHT4_U", DHT4_U);
    sensor.addField("TERMOPAR1", TERMOPAR1);
    sensor.addField("TERMOPAR2", TERMOPAR2);
    sensor.addField("TERMOPAR3", TERMOPAR3);
    sensor.addField("TERMOPAR4", TERMOPAR4);
    sensor.addField("TERMOPAR5", TERMOPAR5);
    sensor.addField("TENSAO_BAIXA", TENSAO_BAIXA);
    sensor.addField("TENSAO_ALTA", TENSAO_ALTA);
    sensor.addField("CORRENTE_BAIXA", CORRENTE_BAIXA);
    sensor.addField("CORRENTE_ALTA", CORRENTE_ALTA);

    // Mostra o que esta sendo gravado
    Dprint("Writing: ");
    Dprintln(client.pointToLineProtocol(sensor));

    //Verifica se tem WiFi para gravacao
    if (WiFi.status() != WL_CONNECTED) {
      Dprintln("Wifi connection lost");
    }
    //Escreve no Ponto
    if (!client.writePoint(sensor)) {
      Dprint("InfluxDB write failed: ");
      Dprintln(client.getLastErrorMessage());
    }

    //Resseta flag de Sync
    flagSyncInflux = false;
  }
}
