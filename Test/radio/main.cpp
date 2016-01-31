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

   tx.getSensor(0)->setSimulateValue(0);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   tx.getSensor(0)->setSimulateValue(100);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   tx.getSensor(0)->setSimulateValue(1024);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   // Check RCL input variable
   tx.onNewCommand("s l 0 i0");

   // check lower value
   tx.getSensor(0)->setSimulateValue(ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   // check middle
   tx.getSensor(0)->setSimulateValue((ADC_MAX_VALUE-ADC_MIN_VALUE)/2);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, (PPM_MIN_VALUE+(PPM_MAX_VALUE-PPM_MIN_VALUE)/2), 1.1);

   // check over the limit
   tx.getSensor(0)->setSimulateValue(ADC_MAX_VALUE+10);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MAX_VALUE, 0.01);

   // Check RCL limit
   tx.onNewCommand("s l 0 i0[0;1]");

   tx.getSensor(0)->setSimulateValue(ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   tx.getSensor(0)->setSimulateValue(ADC_MAX_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, PPM_MIN_VALUE, 0.01);

   // Check RCL addition
   tx.onNewCommand("s l 0 10+10");

   tx.getSensor(0)->setSimulateValue(ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0,(PPM_MIN_VALUE+20), 1.1);

   // Check RCL addition
   tx.onNewCommand("s l 0 i0+10");

   tx.getSensor(0)->setSimulateValue(ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, (PPM_MIN_VALUE+10), 1.1);

   tx.onNewCommand("s l 0 i0+i1");

   tx.getSensor(0)->setSimulateValue(ADC_MIN_VALUE);
   tx.getSensor(1)->setSimulateValue(ADC_MIN_VALUE);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, (PPM_MIN_VALUE), 1.1);

   tx.getSensor(0)->setSimulateValue(ADC_MIN_VALUE+(ADC_MAX_VALUE-ADC_MIN_VALUE)/2);
   tx.getSensor(1)->setSimulateValue(ADC_MIN_VALUE+(ADC_MAX_VALUE-ADC_MIN_VALUE)/2);
   tx.onEvaluateExpression();
   c0 = tx.getOutputPPM(0);
   QCOMPARE(c0, (PPM_MAX_VALUE), 1.1);
}

void Init()
{
   tx.setup();
   //
   for(uint8_t i=0; i < MAX_ADC_INPUT_CHANNEL; i++)
      tx.getSensor(i)->setSimulation(true);

   for(uint8_t i=MAX_ADC_INPUT_CHANNEL; i < MAX_ADC_INPUT_CHANNEL+MAX_DIG_INPUT_CHANNEL; i++)
      tx.getSensor(i)->setSimulation(true);
}

int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   Init();

   checkOutput();

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

