//
// Created by Naren Sadhwani on 01.04.24.
//

#ifndef IMAGEVIEWER_MAINWINDOW_H
#define IMAGEVIEWER_MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QStatusBar>
#include <QLabel>
#include <QGraphicsPixmapItem>
#include "EditorPluginInterface.h"


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    // Menus
    QMenu *fileMenu{};
    QMenu *viewMenu{};
    QMenu *editMenu{};

    // Toolbars
    QToolBar *fileToolBar{};
    QToolBar *viewToolBar{};
    QToolBar *editToolBar{};

    // Image display area
    QGraphicsScene *imageScene{};
    QGraphicsView *imageView{};

    // Status bar
    QStatusBar *mainStatusBar{};
    QLabel *mainStatusLabel{};

    // Actions
    QAction *openAction{};
    QAction *saveAsAction{};
    QAction *exitAction{};
    QAction *zoomInAction{};
    QAction *zoomOutAction{};
    QAction *prevAction{};
    QAction *nextAction{};
    QAction *blurAction{};
    QAction *undoAction{};

    // Variables
    QString currentImagePath;
    QGraphicsPixmapItem *currentImage;

    QMap<QString, EditorPluginInterface*> editPlugins;

    // Functions
    void initUI();
    void createActions();
    void showImage(const QString& path);
    void setupShortcuts();
    // Plugins
    void loadPlugins();

private slots:
    void openImage();
    void zoomIn();
    void zoomOut();
    void prevImage();
    void nextImage();
    void saveAs();
    void blurImage();
    void resetImage();
    // Plugins
    void pluginPerform();

};


#endif //IMAGEVIEWER_MAINWINDOW_H
