#include "mainmodel.h"


MainModel::MainModel()
{
}

MainModel::~MainModel()
{
}

void MainModel::init(QQmlContext *context)
{
    radioLink_.init();
    txCommand_.init(&radioLink_);

    //context->setContextProperty("batteryModel", &batteryModel_);
    context->setContextProperty("radioLinkModel", &radioLink_);
    context->setContextProperty("txCommandModel", &txCommand_);
}

