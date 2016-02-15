#include <QCoreApplication>
#include "../../Radio/Tx.h"
//#include <QtTest>
#include <math.h>

#define QCOMPARE(a, b, c)\
{\
   float diff = std::abs(a - b);\
   if(diff > (float)c)\
   std::cout << __FILE__ <<  "(" << __LINE__ << ") error: comparison failed, value=" \
   << a << " expected " << b << " prec=" << c << " " <<  std::endl;\
}


Tx tx;


void checkOutput()
{
   // Calibration
   tx.getSensor(0)->setMin(ADC_MIN_VALUE);
   tx.getSensor(0)->setMax(ADC_MAX_VALUE);
   tx.getSensor(1)->setMin(ADC_MIN_VALUE);
   tx.getSensor(1)->setMax(ADC_MAX_VALUE);

   // First, check default value without any rcl script on
   tx.onEvaluateExpression();

   uint16_t c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   uint16_t c1 = tx.getOutputPPM(1);
   QCOMPARE(c1, PPM_MIN_VALUE, 0.01);

   uint16_t c2 = tx.getOutputPPM(2);
   QCOMPARE(c2, PPM_MIN_VALUE, 0.01);

   uint16_t c3 = tx.getOutputPPM(3);
   QCOMPARE(c3, PPM_MIN_VALUE, 0.01);

   uint16_t c4 = tx.getOutputPPM(4);
   QCOMPARE(c4, PPM_MIN_VALUE, 0.01);

   uint16_t c5 = tx.getOutputPPM(5);
   QCOMPARE(c5, PPM_MIN_VALUE, 0.01);

   //Check RCL simple constant script
   tx.onNewCommand("s l 0 0");

   analogWrite(tx.getSensor(0)->getPin(), 0);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   analogWrite(tx.getSensor(0)->getPin(), 1000);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   analogWrite(tx.getSensor(0)->getPin(), 1024);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   tx.onNewCommand("s l 0 (10)");

   analogWrite(tx.getSensor(0)->getPin(), 0);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, map(10, ADC_MIN_VALUE, ADC_MAX_VALUE, PPM_MIN_VALUE, PPM_MAX_VALUE), 0.01);

   tx.onNewCommand("s l 0 (1023)[0;10]");

   analogWrite(tx.getSensor(0)->getPin(), 0);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, map(map(1023, 0, 1023, 0, 10), ADC_MIN_VALUE, ADC_MAX_VALUE, PPM_MIN_VALUE, PPM_MAX_VALUE), 0.01);

   // Check RCL input variable
   tx.onNewCommand("s l 0 i0");

   // check lower value
   analogWrite(tx.getSensor(0)->getPin(), ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   // Check middle
   analogWrite(tx.getSensor(0)->getPin(), (ADC_MAX_VALUE-ADC_MIN_VALUE)/2);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, map((ADC_MAX_VALUE-ADC_MIN_VALUE)/2, ADC_MIN_VALUE, ADC_MAX_VALUE, PPM_MIN_VALUE, PPM_MAX_VALUE), 0.01);

   // Check over the limit
   analogWrite(tx.getSensor(0)->getPin(), ADC_MAX_VALUE+10);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MAX_VALUE, 0.01);

   // Check RCL limit
   tx.onNewCommand("s l 0 i0[0;1]");

   analogWrite(tx.getSensor(0)->getPin(), ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   analogWrite(tx.getSensor(0)->getPin(), ADC_MAX_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   // Check RCL addition
   tx.onNewCommand("s l 0 10+10");

   analogWrite(tx.getSensor(0)->getPin(), ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0,(PPM_MIN_VALUE+20), 1.1);

   tx.onNewCommand("s l 0 10+(10)");

   analogWrite(tx.getSensor(0)->getPin(), ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0,(PPM_MIN_VALUE+20), 1.1);

   tx.onNewCommand("s l 0 10+(512)[0;10]");

   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0,(PPM_MIN_VALUE+14), 1.1);

   tx.onNewCommand("s l 0 i0+10");

   analogWrite(tx.getSensor(0)->getPin(), ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, (PPM_MIN_VALUE+10), 1.1);

   tx.onNewCommand("s l 0 i0+i1");

   analogWrite(tx.getSensor(0)->getPin(), ADC_MIN_VALUE);
   analogWrite(tx.getSensor(1)->getPin(), ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, (PPM_MIN_VALUE), 1.1);

   analogWrite(tx.getSensor(0)->getPin(), ADC_MIN_VALUE+(ADC_MAX_VALUE-ADC_MIN_VALUE)/2);
   analogWrite(tx.getSensor(1)->getPin(), ADC_MIN_VALUE+(ADC_MAX_VALUE-ADC_MIN_VALUE)/2);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, (PPM_MAX_VALUE), 1.1);
}

void checkBattery()
{
   BatteryMeter *batt = tx.getBattery();

   analogWrite(batt->getPin(), 512);
   float ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
   ret = batt->getAverageValueInVolt();
}

void Init()
{
   tx.setup();
}

int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   Init();

   checkOutput();
   checkBattery();

   /*
   tx.onNewCommand("s l 1 i1");
   tx.onNewCommand("s l 2 i2");
   tx.onNewCommand("s l 3 i3");
   tx.onNewCommand("s l 4 i4[0;512]+i5[512;0]");
   tx.onNewCommand("s l 5 i6");

   tx.onNewCommand("s l 5 ia");

   tx.onNewCommand("s l 5 (i0)[0;1]");
*/
   //tx.onNewCommand("v");

   //tx.onNewCommand("d l");

   //return a.exec();

   return 0;
}

