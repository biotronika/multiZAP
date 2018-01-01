/*
 * multiZAP_programs.h
 *
 *  Created on: 31 dec 2017
 *      Author: elektros230
 */

#ifndef MULTIZAP_PROGRAMS_H_
#define MULTIZAP_PROGRAMS_H_




/*int wait(unsigned long waitPeriod, unsigned long &currentPeriod, unsigned long totalPeriod){
    //Wait waitPeriod and display progress bar
    int steps;

    for( steps = (waitPeriod/1000); steps<=0; steps--) {
      progressBar (100*currentPeriod/totalPeriod);

      delay(1000);

      if (keypad.getKey()=='*') digitalWrite(powerPin, LOW);
      currentPeriod+=1000;
    }
    progressBar (100*totalPeriod/currentPeriod);
    delay(waitPeriod%1000);  //rest period to wait
    return 0;
}*/

/*void freq(unsigned long Freq){
  // Genenrate signal
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(Freq/100);
    lcd.print(".");
    if (Freq % 100 < 10) lcd.print("0");
    lcd.print(Freq % 100);
    lcd.print("Hz ");

    ad9850 << Freq;

}*/

/*void program1 (){

  unsigned long totalPeriod = 1500000;
  unsigned long currentPeriod = 0;
  freq(471050); wait(120000, currentPeriod, totalPeriod);
  freq(47100); wait(60000, currentPeriod, totalPeriod);
  freq(47150); wait(60000, currentPeriod, totalPeriod);
  freq(47200); wait(60000, currentPeriod, totalPeriod);
  freq(47900); wait(120000, currentPeriod, totalPeriod);
  freq(62000); wait(120000, currentPeriod, totalPeriod);
  freq(94010); wait(120000, currentPeriod, totalPeriod);
  freq(94290); wait(120000, currentPeriod, totalPeriod);
  freq(188010); wait(120000, currentPeriod, totalPeriod);
  freq(188590); wait(120000, currentPeriod, totalPeriod);
  freq(376030); wait(120000, currentPeriod, totalPeriod);
  freq(377170); wait(120000, currentPeriod, totalPeriod);
  freq(752050); wait(120000, currentPeriod, totalPeriod);
  freq(754340); wait(120000, currentPeriod, totalPeriod);
}

void program2 (){

  unsigned long totalPeriod = 1800000;
  unsigned long currentPeriod = 0;
  freq(20300); wait(180000, currentPeriod, totalPeriod);
  freq(41200); wait(180000, currentPeriod, totalPeriod);
  freq(41400); wait(180000, currentPeriod, totalPeriod);
  freq(58900); wait(180000, currentPeriod, totalPeriod);
  freq(64000); wait(180000, currentPeriod, totalPeriod);
  freq(66700); wait(180000, currentPeriod, totalPeriod);
  freq(84000); wait(180000, currentPeriod, totalPeriod);
  freq(107200); wait(180000, currentPeriod, totalPeriod);
  freq(108700); wait(180000, currentPeriod, totalPeriod);
  freq(855400); wait(180000, currentPeriod, totalPeriod);
}

void program3 (){

  unsigned long totalPeriod = 1260000;
  unsigned long currentPeriod = 0;
  freq(2048500); wait(180000, currentPeriod, totalPeriod);
  freq(5115500); wait(180000, currentPeriod, totalPeriod);
  freq(5115600); wait(180000, currentPeriod, totalPeriod);
  freq(5394000); wait(180000, currentPeriod, totalPeriod);
  freq(5891400); wait(180000, currentPeriod, totalPeriod);
  freq(5891600); wait(180000, currentPeriod, totalPeriod);
  freq(8874000); wait(180000, currentPeriod, totalPeriod);

}

void program0 (){

  unsigned long totalPeriod = 120000;
  unsigned long currentPeriod = 0;
  freq(1000); wait(20000, currentPeriod, totalPeriod);
  freq(10000); wait(20000, currentPeriod, totalPeriod);
  freq(100000); wait(20000, currentPeriod, totalPeriod);
  freq(10000000); wait(20000, currentPeriod, totalPeriod);
  freq(40000000); wait(20000, currentPeriod, totalPeriod);
  freq(90000000); wait(20000, currentPeriod, totalPeriod);
}*/





#endif /* MULTIZAP_PROGRAMS_H_ */
