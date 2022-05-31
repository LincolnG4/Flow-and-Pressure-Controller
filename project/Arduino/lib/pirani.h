#ifndef PIRANI_H
#define PIRANI_H

#include "controler.h"


/*
* Read Pirani
* This get signal from Pirani, converts from V to Torr and applies a correction factor considering the gas %.
* If no MFC is turned ON, it won't apply a correction factor
* @params Checkbox1_4, GasFactor1_4, MFC_valve
* @return Pirani in torr 
*/
float PiraniRead(uint32_t checkbox1,uint32_t checkbox2,uint32_t checkbox3,uint32_t checkbox4,NexText t_gas1,NexText t_gas2,NexText t_gas3,NexText t_gas4,int mcfOutput1,int mcfOutput2,int mcfOutput3,int mcfOutput4){
  float Pirani_V = 0;
  float totalFlow=0;
  float correctionFactor=1;
  char bufferText[10];
  char bufferText2[10];
  char bufferText3[10];
  char bufferText4[10];
  float gas_factor_1 = 1;
  float gas_factor_2 = 1;
  float gas_factor_3 = 1;
  float gas_factor_4 = 1;

  Pirani_V = (sinalPirani/SinalMCU)*((SinalMCU*(analogRead(Read_Pirani)))/bitsMCU); 
  

  if(checkbox1==0 && checkbox2==0 && checkbox3==0 && checkbox4==0){ //if mfc's are turned off
      Pirani_Torr = 10000.00*(pow(10,(Pirani_V-6.125)));
      return Pirani_Torr;
  } 
  else{ //mfc's are turned off, calculate % gas and apply correction factor to Pirani measure
    if(checkbox1==1){ 
      
      totalFlow = totalFlow + mcfOutput1; 
    
      memset(bufferText, 10, sizeof(bufferText)); 
      t8.getText(bufferText, sizeof(bufferText));
      gas_factor_1 = atof(bufferText); 
    }
    if(checkbox2==1){
      
      totalFlow = totalFlow + mcfOutput2;

      memset(bufferText2, 10, sizeof(bufferText2));
      t19.getText(bufferText2, sizeof(bufferText2));
      gas_factor_2 = atof(bufferText2);
    }
    if(checkbox3==1){
      
      totalFlow = totalFlow + mcfOutput3;
      
      memset(bufferText3, 10, sizeof(bufferText3));
      t20.getText(bufferText3, sizeof(bufferText3));
      gas_factor_3 = atof(bufferText3);
    }
    if(checkbox4==1){
      
      totalFlow = totalFlow + mcfOutput4;

      memset(bufferText4, 10, sizeof(bufferText4));
      t21.getText(bufferText4, sizeof(bufferText4));
      gas_factor_4 = atof(bufferText4);
    }
    
    correctionFactor = checkbox1*(mcfOutput1/totalFlow)*gas_factor_1 + checkbox2*(mcfOutput2/totalFlow)*gas_factor_2 + checkbox3*(mcfOutput3/totalFlow)*gas_factor_3+ checkbox4*(mcfOutput4/totalFlow)*gas_factor_4;
    Pirani_Torr = 10000.00*(pow(10,((Pirani_V*correctionFactor)-6.125)));  // convert V to Torr and applies correction Factor
    return Pirani_Torr;
  }
} 

void piraniUpdateScreen(float Pirani_V){
  dtostrf(Pirani_V, 4, 2, buff);
  t10.setText(buff); // Print on screen the Pirani value
  if(Pirani_V<0.7498 && Pirani_V>7498942.0933){
    t10.Set_font_color_pco(57959);
  }
  else{
    t10.Set_font_color_pco(61342);
  }
}


#endif