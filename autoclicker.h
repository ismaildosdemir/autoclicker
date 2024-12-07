#ifndef AUTOCLICKER_H
#define AUTOCLICKER_H

#include "qtranslator.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class autoclicker;
}
QT_END_NAMESPACE

class autoclicker : public QMainWindow
{
    Q_OBJECT

public:
    autoclicker(QWidget *parent = nullptr);
    ~autoclicker();
    void simulateClick();


private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_exitButton_clicked();

    void on_LanguageComboBox_currentTextChanged(const QString &arg1);

    void animateMouseCursorHighlight();


private:
    Ui::autoclicker *ui;
    QTranslator translator;


};
#endif // AUTOCLICKER_H
