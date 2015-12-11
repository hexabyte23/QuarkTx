#include "mainmodel.h"


MainModel::MainModel()
{
}

MainModel::~MainModel()
{
}

void MainModel::Init(QQmlContext *context)
{
    context->setContextProperty("batteryModel", &batteryModel_);
}

