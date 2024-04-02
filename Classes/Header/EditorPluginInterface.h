//
// Created by Naren Sadhwani on 02.04.24.
//

#ifndef IMAGEVIEWER_EDITORPLUGININTERFACE_H
#define IMAGEVIEWER_EDITORPLUGININTERFACE_H

#include <QObject>
#include <QString>
#include <opencv2/opencv.hpp>

class EditorPluginInterface {
    public:
    virtual ~EditorPluginInterface() = default;
    virtual QString Name() = 0;
    virtual void editImage(cv::Mat &input, cv::Mat &output) = 0;
};

#define EditorPluginInterface_iid "com.narensadhwani.imageviewer.EditorPluginInterface"
Q_DECLARE_INTERFACE(EditorPluginInterface, EditorPluginInterface_iid);

#endif //IMAGEVIEWER_EDITORPLUGININTERFACE_H
