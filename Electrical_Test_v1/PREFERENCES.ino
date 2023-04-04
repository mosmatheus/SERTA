//Setup Preferences (guarda configuracoes)
void setupPREFERENCES() {
  preferences.begin("config", false);

  //Criacao das chaves de configuracao
  bool flagnvs;
  flagnvs = preferences.getBool("flagnvs", false);
  if (!flagnvs) {
    //preferences.putString("INFLUXDB_URL", "https://us-central1-1.gcp.cloud2.influxdata.com");
    //preferences.putString("INFLUXDB_TOKEN", "ajZA94dKkYG3vwdtfAtdcKPwbo1YUgD7eweMSpwRxidxui3zmFL_Cnj4sJOmVuEKO4_pIP4ejVxw1e5CHNRhYA==");
    //preferences.putString("INFLUXDB_ORG", "bruno@dangelomachado.com");
    //preferences.putString("INFLUXDB_BUCKET", "Test Electrical");
    preferences.putString("ssid_WIFI", "");
    preferences.putString("password_WIFI", "");
    preferences.putInt("TempTime", 10);
    preferences.putInt("TempMAX", 130);
    preferences.putBool("StopRele", false);
    preferences.putFloat("DHT1T_alfa", 1);
    preferences.putFloat("DHT1T_beta", 0);
    preferences.putFloat("DHT2T_alfa", 1);
    preferences.putFloat("DHT2T_beta", 0);
    preferences.putFloat("DHT3T_alfa", 1);
    preferences.putFloat("DHT3T_beta", 0);
    preferences.putFloat("DHT4T_alfa", 1);
    preferences.putFloat("DHT4T_beta", 0);
    preferences.putFloat("DHT1U_alfa", 1);
    preferences.putFloat("DHT1U_beta", 0);
    preferences.putFloat("DHT2U_alfa", 1);
    preferences.putFloat("DHT2U_beta", 0);
    preferences.putFloat("DHT3U_alfa", 1);
    preferences.putFloat("DHT3U_beta", 0);
    preferences.putFloat("DHT4U_alfa", 1);
    preferences.putFloat("DHT4U_beta", 0);
    preferences.putFloat("Termo1_alfa", 1);
    preferences.putFloat("Termo1_beta", 0);
    preferences.putFloat("Termo2_alfa", 1);
    preferences.putFloat("Termo2_beta", 0);
    preferences.putFloat("Termo3_alfa", 1);
    preferences.putFloat("Termo3_beta", 0);
    preferences.putFloat("Termo4_alfa", 1);
    preferences.putFloat("Termo4_beta", 0);
    preferences.putFloat("Termo5_alfa", 1);
    preferences.putFloat("Termo5_beta", 0);
    preferences.putFloat("TB1_alfa", 1);
    preferences.putFloat("TB1_beta", 0);
    preferences.putFloat("TA1_alfa", 1);
    preferences.putFloat("TA1_beta", 0);
    preferences.putFloat("CB1_alfa", 1);
    preferences.putFloat("CB1_beta", 0);
    preferences.putFloat("CA1_alfa", 1);
    preferences.putFloat("CA1_beta", 0);
    preferences.putString("currentFileLog", "");
    preferences.putBool("flagStartLog", true);
    preferences.putBool("flagnvs", true);
  }
  else {
    //INFLUXDB_URL = preferences.getString("INFLUXDB_URL", "https://us-central1-1.gcp.cloud2.influxdata.com");
    //INFLUXDB_TOKEN = preferences.getString("INFLUXDB_TOKEN", "ajZA94dKkYG3vwdtfAtdcKPwbo1YUgD7eweMSpwRxidxui3zmFL_Cnj4sJOmVuEKO4_pIP4ejVxw1e5CHNRhYA==");
    //INFLUXDB_ORG = preferences.getString("INFLUXDB_ORG", "bruno@dangelomachado.com");
    //INFLUXDB_BUCKET = preferences.getString("INFLUXDB_BUCKET", "Test Electrical");
    ssid_WIFI = preferences.getString("ssid_WIFI", "");
    password_WIFI = preferences.getString("password_WIFI", "");
    TempTime = preferences.getInt("TempTime", 10);
    TempMAX = preferences.getInt("TempMAX", 130);
    StopRele = preferences.getBool("StopRele", false);
    DHT1T_alfa = preferences.getFloat("DHT1T_alfa", 1);
    DHT1T_beta = preferences.getFloat("DHT1T_beta", 0);
    DHT2T_alfa = preferences.getFloat("DHT2T_alfa", 1);
    DHT2T_beta = preferences.getFloat("DHT2T_beta", 0);
    DHT3T_alfa = preferences.getFloat("DHT3T_alfa", 1);
    DHT3T_beta = preferences.getFloat("DHT3T_beta", 0);
    DHT4T_alfa = preferences.getFloat("DHT4T_alfa", 1);
    DHT4T_beta = preferences.getFloat("DHT4T_beta", 0);
    DHT1U_alfa = preferences.getFloat("DHT1U_alfa", 1);
    DHT1U_beta = preferences.getFloat("DHT1U_beta", 0);
    DHT2U_alfa = preferences.getFloat("DHT2U_alfa", 1);
    DHT2U_beta = preferences.getFloat("DHT2U_beta", 0);
    DHT3U_alfa = preferences.getFloat("DHT3U_alfa", 1);
    DHT3U_beta = preferences.getFloat("DHT3U_beta", 0);
    DHT4U_alfa = preferences.getFloat("DHT4U_alfa", 1);
    DHT4U_beta = preferences.getFloat("DHT4U_beta", 0);
    Termo1_alfa = preferences.getFloat("Termo1_alfa", 1);
    Termo1_beta = preferences.getFloat("Termo1_beta", 0);
    Termo2_alfa = preferences.getFloat("Termo2_alfa", 1);
    Termo2_beta = preferences.getFloat("Termo2_beta", 0);
    Termo3_alfa = preferences.getFloat("Termo3_alfa", 1);
    Termo3_beta = preferences.getFloat("Termo3_beta", 0);
    Termo4_alfa = preferences.getFloat("Termo4_alfa", 1);
    Termo4_beta = preferences.getFloat("Termo4_beta", 0);
    Termo5_alfa = preferences.getFloat("Termo5_alfa", 1);
    Termo5_beta = preferences.getFloat("Termo5_beta", 0);
    TB1_alfa = preferences.getFloat("TB1_alfa", 1);
    TB1_beta = preferences.getFloat("TB1_beta", 0);
    TA1_alfa = preferences.getFloat("TA1_alfa", 1);
    TA1_beta = preferences.getFloat("TA1_beta", 0);
    CB1_alfa = preferences.getFloat("CB1_alfa", 1);
    CB1_beta = preferences.getFloat("CB1_beta", 0);
    CA1_alfa = preferences.getFloat("CA1_alfa", 1);
    CA1_beta = preferences.getFloat("CA1_beta", 0);
    currentFileLog = preferences.getString("currentFileLog", "");
    flagStartLog = preferences.getBool("flagStartLog", true);
    flagnvs = preferences.getBool("flagnvs", true);
  }
  preferences.end();
}

//Loop Preferences
void loopPREFERENCES() {
  Dprint("Reset WTD - PREFERENCES");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
}
