//Setup SD
void setupSD() {
  Dprint("\nInitializing SD card...");
  SPISD.begin(HSPI_CLK, HSPI_MISO, HSPI_MOSI);
  if (!SD.begin(CS_SD, SPISD)) {
    Dprintln("initialization failed");
    while (1);
  } else {
    Dprintln("Wiring is correct and a card is present.");
  }

  // print the type of card
  Dprintln();
  Dprint("Card type:         ");
  switch (SD.cardType()) {
    case CARD_NONE:
      Dprintln("NONE");
      break;
    case CARD_MMC:
      Dprintln("MMC");
      break;
    case CARD_SD:
      Dprintln("SD");
      break;
    case CARD_SDHC:
      Dprintln("SDHC");
      break;
    default:
      Dprintln("Unknown");
  }

  // print the type and size of the first FAT-type volume
  Dprint("Card size:  ");
  Dprintln((float)SD.cardSize() / 1000);
  Dprint("Total bytes: ");
  Dprintln((float)SD.totalBytes());
  Dprint("Used bytes: ");
  Dprintln((float)SD.usedBytes());
  File dir =  SD.open("/");
}

//Loop SD
void loopSD() {
  Dprint("Reset WTD - SD");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
}

void listDir_SD(fs::FS &fs, const char * dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Dprintln("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Dprintln("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Dprint("  DIR : ");
      Dprintln(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Dprint("  FILE: ");
      Dprint(file.name());
      Dprint("  SIZE: ");
      Dprintln(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir_SD(fs::FS &fs, const char * path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Dprintln("Dir created");
  } else {
    Dprintln("mkdir failed");
  }
}

void removeDir_SD(fs::FS &fs, const char * path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Dprintln("Dir removed");
  } else {
    Dprintln("rmdir failed");
  }
}

void readFile_SD(fs::FS &fs, const char * path) {
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Dprintln("Failed to open file for reading");
    return;
  }

  Dprint("Read from file: ");
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile_SD(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Dprintln("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Dprintln("File written");
  } else {
    Dprintln("Write failed");
  }
  file.close();
}

void appendFile_SD(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Dprintln("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Dprintln("Message appended");
  } else {
    Dprintln("Append failed");
  }
  file.close();
}

void renameFile_SD(fs::FS &fs, const char * path1, const char * path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Dprintln("File renamed");
  } else {
    Dprintln("Rename failed");
  }
}

void deleteFile_SD(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Dprintln("File deleted");
  } else {
    Dprintln("Delete failed");
  }
}

void testFileIO_SD(fs::FS &fs, const char * path) {
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if (file) {
    len = file.size();
    size_t flen = len;
    start = millis();
    while (len) {
      size_t toRead = len;
      if (toRead > 512) {
        toRead = 512;
      }
      file.read(buf, toRead);
      len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Dprintln("Failed to open file for reading");
  }


  file = fs.open(path, FILE_WRITE);
  if (!file) {
    Dprintln("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for (i = 0; i < 2048; i++) {
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}
