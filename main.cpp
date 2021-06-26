#include <Controller.h>
#include <QCoreApplication>
int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);
  Controller controller;
  return a.exec();
}
