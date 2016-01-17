#include "mainmodel.h"


MainModel::MainModel()
{
}

MainModel::~MainModel()
{
}

void MainModel::init(QQmlContext *context)
{
    context->setContextProperty("batteryModel", &batteryModel_);
    context->setContextProperty("radioLinkModel", &radioLink_);
    //radioLink_.init();
}

