//
// Created by Dmitry Khrykin on 2019-08-12.
//

#ifndef STRATEGR_ABOUTWINDOW_H
#define STRATEGR_ABOUTWINDOW_H

#include <QDialog>
#include <QLayout>
#include <QPushButton>
#include <QLabel>

class AboutWindow : public QDialog {
public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow() override;
};


#endif //STRATEGR_ABOUTWINDOW_H
