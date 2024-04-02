
//
// Created by Naren Sadhwani on 02.04.24.
//

#ifndef IMAGEVIEWER_AFFINEPLUGIN_H
#define IMAGEVIEWER_AFFINEPLUGIN_H


#include <QObject>
#include <QtPlugin>
#include "../../Classes/Header/EditorPluginInterface.h"

class AffinePlugin:public QObject, public EditorPluginInterface {
Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.narensadhwani.EditorPluginInterface")
    Q_INTERFACES(EditorPluginInterface)
public:
    QString Name() override;

    void editImage(cv::Mat &input, cv::Mat &output) override ;

};


#endif //IMAGEVIEWER_AFFINEPLUGIN_H
