#ifndef SDCARD_H
#define SDCARD_H

#include "variables.h"

void WriteSD(File myFile,String message){
  if (myFile) {
    Serial.print(String(myFile) + " Writing ...");
    myFile.println(message);
    // close the file:
    myFile.close();
    //Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    //Serial.println("error opening test.txt");
  }
}

#endif
