#ifndef MFC_H
#define MFC_H

#include "variables.h"


float MFCRead(int MFC,char* mfcOutputString,NexText t,NexText t_out, float Calibrated_MFC,uint32_t mfcSCCM){
  float gas_factor_in;
  float output_MFC_factor;
  char bufferText[10];

  MFC_V = (mfcSCCM*(sinalMKS/SinalMCU)*((SinalMCU*(analogRead(MFC)))/bitsMCU))/SinalMCU; 
  
  memset(bufferText, 10, sizeof(bufferText));
  t_out.getText(bufferText, sizeof(bufferText));

  gas_factor_in = atof(bufferText);
  
  output_MFC_factor = float(MFC_V)*(gas_factor_in/Calibrated_MFC);

  dtostrf(output_MFC_factor, 1, 2, mfcOutputString); 
  //itoa(output_MFC_factor,mfcOutputString, 10);
  t.setText(mfcOutputString);

  return output_MFC_factor;
}



void MfcPwmON(int PWM_INPUT,int delta,char* buffer,NexText t,NexNumber n){
  uint32_t val;

  n.getValue(&val);
  
  val = map(val, 0, 500, 0, 255);

  
  analogWrite(PWM_INPUT, val);
  
  itoa(delta,buffer, 10);
  t.setText(buffer);
}

void MfcPwmClose(int PWM_INPUT,int delta,char* buffer,NexText t){

  analogWrite(PWM_INPUT, 0);
  
  itoa(delta,buffer, 10);
  t.setText(buffer);
}

void MfcPwmOFF(int PWM_INPUT,NexCheckbox c){
  analogWrite(PWM_INPUT, 0);
  c.setValue(0);
}



#endif