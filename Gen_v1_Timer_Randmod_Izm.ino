
#include <TM1638.h>
#include <TM1638QYF.h>
#include <TM1640.h>
#include <TM16XX.h>
#include <TM16XXFonts.h>
TM1638 module(8, 10, 7);
byte keys = module.getButtons();
int a=0;
static uint32_t reqfreq=600000; //переменная запроса частоты
uint32_t ocr=OCR1A;  uint16_t divider=1;  float freq; 
long randNumber;

int u =  0;
int q =  0;





void setup() {
module.setupDisplay(true, 1);
Serial.begin(9600);
pinMode (9,OUTPUT); // выход генератора
TCCR1A=0;TCCR1B=0;
module.setDisplayToString("Ready");
delay(1000);
}


void loop() {
u = analogRead (A0);
q = analogRead (A1);
Serial.println(u);


noTone(2);

digitalWrite(13, LOW);
for (a; a>0; a--)
 {
 randNumber = random(10000);
 if (a<10) digitalWrite(13, HIGH);
 if (a<10) tone(12, 1000, 1000);
 
 module.setDisplayToDecNumber(a,0,false);
 keys = module.getButtons();
 
 if (keys==32) a=1;
 delay(1000);
 module.setDisplayToString("Timer ON");
 delay(1000);
 tone(2, randNumber, 1000);
 if (a<2) module.setDisplayToString("Ready");
 }
 
keys = module.getButtons();

TCCR1A=0;TCCR1B=0;

module.setLEDs(((keys & 0xFF) << 8) | (keys & 0xFF));
 Serial.println(keys);

if (keys==16) reqfreq=reqfreq+10000;
if (keys==8) reqfreq=reqfreq-10000;

if (keys==2) module.setDisplayToDecNumber(u,0,false);
if (keys==2) goto xxx;
if (keys==4) module.setDisplayToDecNumber(q,0,false);
if (keys==4) goto xxx;
if (keys==1) reqfreq=300000;
if (keys==1) tone(12, 2000, 500);
if (keys==128) a=600;
if (keys==64) a=300;
if (keys!=0) goto zzz;
if (a==0) goto xxx;


zzz:
if (reqfreq==0 || reqfreq>F_CPU/2) {return;}
 ocr = (F_CPU / reqfreq /2 /divider); 
  byte shifts[] = {3,3,2,2};
   for(byte i = 0; i < 4; i++){
     if (ocr > 65536) { divider <<= shifts[i];
       ocr = F_CPU / reqfreq /2 /divider; }
      else { TCCR1B = (i+1)|(1<<WGM12);  break; }  } //Mode4 (CTC)
     OCR1A=ocr-1; TCCR1A=1<<COM1A0;
    freq= (float) F_CPU/2 / (OCR1A+1) /divider;
  if (freq <10000) { Serial.print(freq,1);Serial.println(" Hz "); }
 if (freq >=10000) { Serial.print(freq/1000,3);Serial.println(" kHz");}
 module.setDisplayToDecNumber(freq,0,false);
 
 xxx:
 
 
 delay(100);
} 
