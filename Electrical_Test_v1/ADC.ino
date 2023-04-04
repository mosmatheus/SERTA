//Configuraçao dos ADC
void setupADC() {
  ADC_Baixa.init();
  ADC_Alta.init();

  //Configura o ADC 1
  ADC_Baixa.setVoltageRange_mV(ADS1115_RANGE_6144);
  ADC_Baixa.setMeasureMode(ADS1115_CONTINUOUS);

  //Configura o ADC2
  ADC_Alta.setVoltageRange_mV(ADS1115_RANGE_6144);
  ADC_Alta.setMeasureMode(ADS1115_CONTINUOUS);
}

//Loop dos ADCs
void loopADC() {
  Dprint("Reset WTD - ADC");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
}

//Rotina de Leitura dos ADCs
bool readADC(int N) {
  int16_t adc0;
  //Seleciona qual variavel ler
  switch (N) {
    case 1:
      ADC_Baixa.setCompareChannels(ADS1115_COMP_0_1);
      while (ADC_Baixa.isBusy()) {}
      adc0 = ADC_Baixa.getRawResult();
      TENSAO_BAIXA = TB1_alfa * adc0 + TB1_beta;
      return true;
      break;
    case 2:
      ADC_Baixa.setCompareChannels(ADS1115_COMP_2_3);
      while (ADC_Baixa.isBusy()) {}
      adc0 = ADC_Baixa.getRawResult();
      CORRENTE_BAIXA = CB1_alfa * adc0 + CB1_beta;
      return true;
      break;
    case 3:
      ADC_Alta.setCompareChannels(ADS1115_COMP_0_1);
      while (ADC_Alta.isBusy()) {}
      adc0 = ADC_Alta.getRawResult();
      TENSAO_ALTA = TA1_alfa * adc0 + TA1_beta;
      return true;
      break;
    case 4:
      ADC_Alta.setCompareChannels(ADS1115_COMP_2_3);
      while (ADC_Alta.isBusy()) {}
      adc0 = ADC_Alta.getRawResult();
      CORRENTE_ALTA = CA1_alfa * adc0 + CA1_beta;
      return true;
      break;
    default:
      return false;
  }
}
