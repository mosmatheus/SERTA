// Setup Serial
void setupSERIAL() {
  Serial.begin(115200);
}

//Loop Serial
void loopSERIAL() {
  Dprint("Reset WTD - SERIAL");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
}
