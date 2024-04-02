//
// Created by Naren Sadhwani on 01.04.24.
//
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QKeyEvent>
#include <QDebug>
#include "../Header/MainWindow.h"
#include <opencv2/opencv.hpp>
#include <QPluginLoader>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
    , fileMenu(nullptr)
    , viewMenu(nullptr)
    ,currentImage(nullptr){
    imageScene = new QGraphicsScene(this); // Initialize imageScene
    imageView = new QGraphicsView(imageScene, this);
    initUI();
    loadPlugins();
}

void MainWindow::initUI() {

    this->resize(800, 600);

    // Create the menu bar
    fileMenu = menuBar()->addMenu("&File");
    viewMenu = menuBar()->addMenu("&View");
    editMenu = menuBar()->addMenu("&Edit");

    // Create the toolbars
    fileToolBar = addToolBar("File");
    viewToolBar = addToolBar("View");
    editToolBar = addToolBar("Edit");
    setCentralWidget(imageView);

    // set up the status bar
    mainStatusBar = statusBar();
    mainStatusLabel = new QLabel(mainStatusBar);
    mainStatusBar->addPermanentWidget(mainStatusLabel);
    mainStatusLabel->setText("Image will be displayed here");

    createActions();
}

void MainWindow::createActions(){

    // Create the actions and add them to the menu and toolbar
    // Open action
    openAction =  new QAction("Open", this);
    fileMenu->addAction(openAction);
    // Save as action
    saveAsAction = new QAction("Save As", this);
    fileMenu->addAction(saveAsAction);
    // Exit action
    exitAction = new QAction("Exit", this);
    fileMenu->addAction(exitAction);
    // Zoom in action
    zoomInAction = new QAction("Zoom In", this);
    viewMenu->addAction(zoomInAction);
    // Zoom out action
    zoomOutAction = new QAction("Zoom Out", this);
    viewMenu->addAction(zoomOutAction);
    // Previous action
    prevAction = new QAction("Previous", this);
    viewMenu->addAction(prevAction);
    // Next action
    nextAction = new QAction("Next", this);
    viewMenu->addAction(nextAction);
    // Blur action
    blurAction = new QAction("Blur", this);
    editMenu->addAction(blurAction);
    // Undo action
    undoAction = new QAction("Reset", this);
    editMenu->addAction(undoAction);


    // Add the actions to the toolbar

    // File toolbar actions
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAsAction);
    fileToolBar->addAction(exitAction);
    // View toolbar actions
    viewToolBar->addAction(zoomInAction);
    viewToolBar->addAction(zoomOutAction);
    viewToolBar->addAction(prevAction);
    viewToolBar->addAction(nextAction);
    // Edit toolbar actions
    editToolBar->addAction(undoAction);
    editToolBar->addAction(blurAction);


    // Connect the actions to the slots
    connect(exitAction, SIGNAL(triggered(bool)), QApplication::instance(), SLOT(quit()));
    connect(openAction, SIGNAL(triggered(bool)), this, SLOT(openImage()));
    connect(saveAsAction, SIGNAL(triggered(bool)), this, SLOT(saveAs()));
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(zoomIn()));
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(zoomOut()));
    connect(prevAction, SIGNAL(triggered(bool)), this, SLOT(prevImage()));
    connect(nextAction, SIGNAL(triggered(bool)), this, SLOT(nextImage()));
    connect(blurAction, SIGNAL(triggered(bool)), this, SLOT(blurImage()));
    connect(undoAction, SIGNAL(triggered(bool)), this, SLOT(resetImage()));


    // Set up the shortcuts
    setupShortcuts();
}

void MainWindow::openImage(){

    qDebug() << "slot openImage is called";
    QFileDialog dialog(this);
    dialog.setWindowTitle("Open Image");
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    QStringList filePaths;
    if (dialog.exec()) {
        filePaths = dialog.selectedFiles();
        showImage(filePaths.at(0));
    }

}

void MainWindow::showImage(const QString& path)
{
    //qDebug() <<"Path is: " << path;

    imageScene->clear();
    imageView->resetTransform();

    QPixmap image(path);
    currentImage = imageScene->addPixmap(image);
    imageScene->update();
    imageView->setSceneRect(image.rect());
    QString status = QString("%1, %2x%3, %4 Bytes").arg(path).arg(image.width())
            .arg(image.height()).arg(QFile(path).size());
    mainStatusLabel->setText(status);
    currentImagePath = path;

}

void MainWindow::setupShortcuts() {
    QList<QKeySequence> shortcuts;
    shortcuts<< Qt::Key_Plus<< Qt::Key_Equal;
    zoomInAction->setShortcuts(shortcuts);
    shortcuts.clear();

    shortcuts << Qt::Key_Minus << Qt::Key_Underscore;
    zoomOutAction->setShortcuts(shortcuts);
    shortcuts.clear();

    shortcuts << Qt::Key_Left << Qt::Key_Up;
    prevAction->setShortcuts(shortcuts);
    shortcuts.clear();

    shortcuts << Qt::Key_Right << Qt::Key_Down;
    nextAction->setShortcuts(shortcuts);
    shortcuts.clear();

}

void MainWindow::zoomIn() {
    imageView->scale(1.2, 1.2);

}

void MainWindow::zoomOut() {
    imageView->scale(0.8, 0.8);

}

void MainWindow::prevImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();
    QStringList nameFilters;
    nameFilters << "*.png" << "*.jpg" << "*.bmp";
    qDebug() << "Name filters: " << nameFilters;
    QStringList images = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    int index = images.indexOf(QRegExp(QRegExp::escape(current.fileName())));
    if(index > 0) {
        showImage(dir.absoluteFilePath(images.at(index - 1)));
    }
    else {
        QMessageBox::information(this, "Information", "Current image is the first one.");
    }
}

void MainWindow::nextImage() {
    QFileInfo current(currentImagePath);
    QDir dir = current.absoluteDir();
    QStringList nameFilters;
    nameFilters << "*.png" << "*.jpg" << "*.bmp";

    QStringList images = dir.entryList(nameFilters, QDir::Files, QDir::Name);
    int index = images.indexOf(QRegExp(QRegExp::escape(current.fileName())));
    if(index < images.size() - 1) {
        showImage(dir.absoluteFilePath(images.at(index + 1)));
    }
    else {
        QMessageBox::information(this, "Information", "Current image is the last one.");
    }

}

void MainWindow::saveAs() {
    if(currentImage == nullptr){
        QMessageBox::information(this, "Error", "No image to save");
        return;
    }
    QFileDialog dialog(this);
    dialog.setWindowTitle("Save Image As ...");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Images (*.png *.bmp *.jpg)"));
    dialog.setDefaultSuffix("png");
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        if(QRegExp(".+\\.(png|bmp|jpg)").exactMatch(fileNames.at(0))) {
            currentImage->pixmap().save(fileNames.at(0));
        } else {
            QMessageBox::information(this, "Information", "Save error: bad format or filename.");
        }
    }
}

void MainWindow::blurImage() {
    if (currentImage == nullptr) {
        QMessageBox::information(this, "Error", "No image to blur");
        return;
    }
    QPixmap pixmap = currentImage->pixmap();
    QImage image = pixmap.toImage();

    image = image.convertToFormat(QImage::Format_RGB888);

    // Convert QImage to cv::Mat
    cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC3, image.bits(), image.bytesPerLine());
    cv::Mat blurred;
    // Apply blur
    cv::blur(mat, blurred, cv::Size(8, 8));
    // Convert cv::Mat to QImage
    mat = blurred;

    // Convert cv::Mat to QImage
    QImage result(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);

    // Convert QImage to QPixmap
    QPixmap resultPixmap = QPixmap::fromImage(result);
    imageScene->clear();
    imageView->resetTransform();
    currentImage = imageScene->addPixmap(resultPixmap);
    imageScene->update();
    imageView->setSceneRect(resultPixmap.rect());
    QString status = QString("Blurred image, %1x%2, %3 Bytes").arg(result.width())
            .arg(result.height()).arg(result.sizeInBytes());
    mainStatusLabel->setText(status);

}

void MainWindow::resetImage() {
    if (currentImage == nullptr) {
        QMessageBox::information(this, "Error", "No image to undo");
        return;
    }

    // Reset all the changes and show the original image
    showImage(currentImagePath);

}

void MainWindow::loadPlugins() {
    QDir pluginsDir(QApplication::instance()->applicationDirPath()+"/plugins");
    QStringList nameFilters;
    nameFilters << "*.dll" << "*.so" << "*.dylib";
    QFileInfoList plugins =pluginsDir.entryInfoList(nameFilters, QDir::Files|QDir::NoDotAndDotDot, QDir::Name);
    foreach (QFileInfo plugin, plugins){
        QPluginLoader loader(plugin.absoluteFilePath(), this);
        auto *plugin_ptr = dynamic_cast<EditorPluginInterface*>(loader.instance());
        if(plugin_ptr){
            auto *action = new QAction(plugin_ptr->Name(), this);
            editMenu->addAction(action);
            editToolBar->addAction(action);
            editPlugins[plugin_ptr->Name()] = plugin_ptr;
            connect(action, SIGNAL(triggered(bool)), this, SLOT(pluginPerform()));
        } else {
            qDebug() << "Error loading plugin: " << loader.errorString();
            qDebug() << "Plugin file: " << plugin.absoluteFilePath();
        }
    }

}

void MainWindow::pluginPerform()
{
    if (currentImage == nullptr) {
        QMessageBox::information(this, "Information", "No image to edit.");
        return;
    }

    auto *active_action = qobject_cast<QAction*>(sender());
    EditorPluginInterface *plugin_ptr = editPlugins[active_action->text()];
    if(!plugin_ptr) {
        QMessageBox::information(this, "Information", "No plugin is found.");
        return;
    }

    QPixmap pixmap = currentImage->pixmap();
    QImage image = pixmap.toImage();
    image = image.convertToFormat(QImage::Format_RGB888);
    cv::Mat mat = cv::Mat(
            image.height(),
            image.width(),
            CV_8UC3,
            image.bits(),
            image.bytesPerLine());

    plugin_ptr->editImage(mat, mat);

    QImage image_edited(
            mat.data,
            mat.cols,
            mat.rows,
            mat.step,
            QImage::Format_RGB888);
    pixmap = QPixmap::fromImage(image_edited);
    imageScene->clear();
    imageView->resetTransform();
    currentImage = imageScene->addPixmap(pixmap);
    imageScene->update();
    imageView->setSceneRect(pixmap.rect());
    QString status = QString("(edited image), %1x%2")
            .arg(pixmap.width()).arg(pixmap.height());
    mainStatusLabel->setText(status);
}




MainWindow::~MainWindow() = default;
