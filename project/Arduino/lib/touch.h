#ifndef TOUCH_H
#define TOUCH_H

#include <Nextion.h>
#include "controler.h"

int CurrentPage = 0;  // Create a variable to store which page is currently loaded

long randNumber;
char buf[10];

uint32_t next, myInt = 0;
uint32_t dual_state; 
uint32_t dual_state2; 


/*
* Nextion objects declaration 
* @param NextionObject variableName = NentionObject(pageNumber,objectID,objectAlias)
*/

NexPage p0 = NexPage(0,0,"page0");
NexPage p1 = NexPage(1,0,"page1");
NexPage p2 = NexPage(2,0,"page2");
NexButton p0_b1 = NexButton(0,1,"b1");
NexButton p0_b2 = NexButton(0,2,"b2");
NexButton p1_b2 = NexButton(2,6,"b2");
NexButton p2_b1 = NexButton(1,1,"b0");
NexButton p1_b0 = NexButton(2,44,"b0");
NexDSButton bt2 = NexDSButton(2, 1, "bt2");
NexDSButton bt3 = NexDSButton(2, 5, "bt3");
NexDSButton bt4 = NexDSButton(2, 12, "bt0");
NexDSButton bt5 = NexDSButton(2, 13, "bt1");
NexDSButton bt6 = NexDSButton(2, 2, "bt6");
NexDSButton bt7 = NexDSButton(2, 16, "bt7");
NexDSButton bt8 = NexDSButton(2, 39, "bt8");
NexDSButton bt9 = NexDSButton(2, 14, "bt4");
NexDSButton bt10 = NexDSButton(2, 15, "bt5");
NexDSButton bt11 = NexDSButton(1, 10, "bt0");
NexText t1 = NexText(1, 2, "t0");
NexText t2 = NexText(1, 3, "t1");
NexText t3 = NexText(1, 4, "t2");
NexText t4 = NexText(1, 5, "t3");
NexText t8 = NexText(2, 7, "t0");
NexText t9 = NexText(3, 26, "t4");
NexText t10 = NexText(2, 34, "t25");
NexText t11 = NexText(2, 35, "t26");
NexText t12 = NexText(2, 36, "t27");
NexText t13 = NexText(2, 37, "t28");
NexText t14 = NexText(2, 38, "t29");
NexText t15 = NexText(2, 40, "t30");
NexText t16 = NexText(2, 41, "t31");
NexText t17 = NexText(2, 42, "t32");
NexText t18 = NexText(2, 43, "t33");
NexText t19 = NexText(2, 4, "t1");
NexText t20 = NexText(2, 10, "t2");
NexText t21 = NexText(2, 11, "t3");
NexNumber n0 = NexNumber(2, 17, "n0");
NexNumber n1 = NexNumber(2, 18, "n1");
NexNumber n2 = NexNumber(2, 23, "n5");
NexNumber n3 = NexNumber(2, 21, "n2");
NexNumber n4 = NexNumber(2, 30, "n13");
NexNumber n5 = NexNumber(2, 27, "n10");
NexNumber n6 = NexNumber(2, 22, "n8");
NexNumber n7 = NexNumber(2, 26, "n9");
NexNumber n8 = NexNumber(2, 19, "n3");
NexNumber n9 = NexNumber(2, 20, "n4");
NexNumber n10 = NexNumber(2, 24, "n6");
NexNumber n11 = NexNumber(2, 25, "n7");
NexNumber n12 = NexNumber(2, 31, "n14");
NexNumber n13 = NexNumber(2, 32, "n15");
NexNumber n14 = NexNumber(2, 28, "n11");
NexNumber n15 = NexNumber(2, 29, "n12");
NexNumber n16 = NexNumber(1, 6, "n0");
NexNumber n17 = NexNumber(1, 7, "n1");
NexNumber n18 = NexNumber(1, 8, "n2");
NexNumber n19 = NexNumber(1, 9, "n3");
NexCheckbox c0 = NexCheckbox(2,6,"c0");
NexCheckbox c1 = NexCheckbox(2,3,"c1"); 
NexCheckbox c2 = NexCheckbox(2,8,"c2");
NexCheckbox c3 = NexCheckbox(2,9,"c3");
NexVariable v0 = NexVariable(0,4,"va0");
uint32_t pageVar=0;

/*
* Nextion objects insert 
*/

NexTouch *nex_listen_list[] = {
  &v0,&p0,&p1,&p2,&p0_b1,&p0_b2,&p1_b0,&p2_b1,&p1_b2,&t1,&t2,&t3,&t4,&t8,&t19,&t20,&t21,&bt2,&bt3,&bt4,&bt5,&bt6,&bt7,&bt9,&bt10,&bt8,&bt11,&c0,&c1,&c2,&c3,&n0,&n3,&n6,&n8,&n9,&n10,&n11,&n12,&n13,&n14,&n15,&n16,&n17,&n18,&n19, NULL
};

uint32_t numberu1;
uint32_t numberu2;
uint32_t numberu3;
uint32_t numberu4;

/*
* Press Event for buttons Objects
*/
void p0_b1_Press(void *ptr) {
  CurrentPage = 1; //change page variable
  p2.show(); //change page
}

void p2_b1_Press(void *ptr) {
  CurrentPage = 0;

  p0.show();
}
void p0_b2_Press(void *ptr) {
  CurrentPage = 2;

  p1.show();
}
void p1_b0_Press(void *ptr) {
  CurrentPage = 0;

  p0.show();
}


#endif