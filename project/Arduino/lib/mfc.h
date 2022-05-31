#ifndef MFC_H
#define MFC_H

#include "controler.h"

/*
* This read MFC and applies gas correction factor
* @param MFC_number T_print textMFCOut GasFactor mfcSCCM
* @return MeasureMFC 
*/
float MFCRead(int MFC,NexText t,NexText t_out, float Calibrated_MFC,uint32_t mfcSCCM){
  char mfcOutputString[10];
  float gas_factor_in;
  float output_MFC_factor;
  char bufferText[10];

  MFC_V = (mfcSCCM*(sinalMKS/SinalMCU)*((SinalMCU*(analogRead(MFC)))/bitsMCU))/SinalMCU; 
  
  memset(bufferText, 10, sizeof(bufferText));
  t_out.getText(bufferText, sizeof(bufferText));

  gas_factor_in = atof(bufferText);
  output_MFC_factor = float(MFC_V)*(gas_factor_in/Calibrated_MFC);

  dtostrf(output_MFC_factor, 1, 2, mfcOutputString); 
  t.setText(mfcOutputString);

  return output_MFC_factor;
}


/*
* Send PWM signal to open valve
* @param PinPWM timer timerText SCCMValveOpen CalibratedSCCM
*/
void MfcPwmON(int PWM_INPUT,int delta,NexText t,NexNumber n,uint32_t mfcSCCM ){
  char buffer[100] = {0};
  uint32_t val;

  n.getValue(&val);

  val = map(val, 0, mfcSCCM, 0, 255);
  analogWrite(PWM_INPUT, val);
  
  itoa(delta,buffer, 10);
  t.setText(buffer);
}

/*
* This sends PWM zero to close valve 
* @param t_calibrate MFC SCCM value
* @return new float MFC SCCM calibration 
*/
void MfcPwmClose(int PWM_INPUT,int delta,NexText t,int Close_MFC){
  char bufferclose[100] = {0};
  
  digitalWrite(Close_MFC, HIGH);
  analogWrite(PWM_INPUT, 0);

  itoa(delta,bufferclose, 10);
  t.setText(bufferclose);
}


#endif