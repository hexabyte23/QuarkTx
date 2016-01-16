#include <QCoreApplication>
#include "../../Radio/Tx.h"

Tx tx;


void checkOutput()
{
   tx.onEvaluateExpression();

   uint16_t c0 = tx.getOutputPPM(0);
   if(c0 != 1500) std::cout << "error c0" << std::endl;

   uint16_t c1 = tx.getOutputPPM(1);
   if(c1 != 1500) std::cout << "error c1" << std::endl;

   uint16_t c2 = tx.getOutputPPM(2);
   if(c2 != 1500) std::cout << "error c2" << std::endl;

   uint16_t c3 = tx.getOutputPPM(3);
   if(c3 != 1500) std::cout << "error c3" << std::endl;

   uint16_t c4 = tx.getOutputPPM(4);
   if(c4 != 1500) std::cout << "error c4" << std::endl;

   uint16_t c5 = tx.getOutputPPM(5);
   if(c5 != 1500) std::cout << "error c5" << std::endl;
}


int main(int argc, char *argv[])
{
   QCoreApplication a(argc, argv);

   tx.setup();
   tx.onNewCommand("r");


   tx.onNewCommand("s l 0 i0");
   tx.onNewCommand("s l 1 i1");
   tx.onNewCommand("s l 2 i2");
   tx.onNewCommand("s l 3 i3");
   tx.onNewCommand("s l 4 i4[0;512]+i5[512;0]");

   tx.onNewCommand("v");

   //tx.onNewCommand("d l");

   return a.exec();
}


