#include "autoclicker.h"
#include "ui_autoclicker.h"
#include <QMessageBox>
#include <windows.h>
#include <QTimer>    // Zamanlayıcı kullanımı için
#include <QWidget>



/// FAREYİ VURGULAYAN KOD EKLE













autoclicker::autoclicker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::autoclicker)

{

    QIcon icon(":/images/mouse32x32.ico");
    this->setWindowIcon(icon);


    ui->setupUi(this);


    ui->LanguageComboBox->addItem("Türkçe", "tr_TR");
    ui->LanguageComboBox->addItem("English", "en_US");
    ui->LanguageComboBox->addItem("Deutsch", "de");
    ui->LanguageComboBox->addItem("中国人", "zh");

    ui->dial->setStyleSheet("background-color: red; border-radius: 100px;");

}

autoclicker::~autoclicker()
{
    delete ui;
}

QTimer *clickTimer = nullptr;

void autoclicker::on_startButton_clicked()
{

    // Tıklama aralıklarını spinbox'lardan al
    int hours = ui->hourSpinBox->value();
    int minutes = ui->minuteSpinBox->value();
    int seconds = ui->secondSpinBox->value();

    // Toplam milisaniye hesapla
    int interval = (hours * 3600 + minutes * 60 + seconds) * 1000;
    if (interval == 0) {
        QMessageBox::warning(this, tr("Hata"), tr("Tıklama aralığı 0 olamaz!"));
        return;
    }

    // Zamanlayıcıyı başlat
    if (!clickTimer) {
        clickTimer = new QTimer(this);
        connect(clickTimer, &QTimer::timeout, this, [this]() {
            simulateClick();
        });
    }
    clickTimer->start(interval);
    ui->dial->setStyleSheet("background-color: green; border-radius: 100px;");
}


void autoclicker::on_stopButton_clicked()
{
    if (clickTimer) {
        clickTimer->stop();
        ui->dial->setStyleSheet("background-color: red; border-radius: 100px;");
    }
}


void autoclicker::on_exitButton_clicked()
{
    if (clickTimer) {
        clickTimer->stop();
    }
    close();

}



void autoclicker::on_LanguageComboBox_currentTextChanged(const QString &arg1)
{


    QString languageCode = ui->LanguageComboBox->currentData().toString();

    // Çeviri yükleme kodu
    QTranslator translator;
    if (translator.load(":/autoclicker_" + languageCode + ".qm")) {
        QApplication::instance()->installTranslator(&translator);
    }
    ui->retranslateUi(this);


}



void autoclicker::simulateClick()
{

    int buttonSelectedId = ui->buttonComboBox->currentIndex();
    int actionnSelectedId = ui->actionComboBox->currentIndex();


    // Hangi tıklama yapılacağını belirle
    DWORD downFlag, upFlag;
    if (buttonSelectedId == 0) {
        downFlag = MOUSEEVENTF_LEFTDOWN;
        upFlag = MOUSEEVENTF_LEFTUP;
    } else if (buttonSelectedId == 1) {
        downFlag = MOUSEEVENTF_RIGHTDOWN;
        upFlag = MOUSEEVENTF_RIGHTUP;
    } else {
        QMessageBox::warning(this, tr("Hata"), tr("Geçersiz işlem türü seçildi!"));
        return;
    }

    animateMouseCursorHighlight();

    // Tek tıklama veya çift tıklama yap
    INPUT input = {0};
    input.type = INPUT_MOUSE;

    if (actionnSelectedId == 0) {
        // Tek tıklama
        input.mi.dwFlags = downFlag;
        SendInput(1, &input, sizeof(INPUT));
        input.mi.dwFlags = upFlag;
        SendInput(1, &input, sizeof(INPUT));
    } else if (actionnSelectedId == 1) {
        // Çift tıklama
        for (int i = 0; i < 2; ++i) {
            input.mi.dwFlags = downFlag;
            SendInput(1, &input, sizeof(INPUT));
            input.mi.dwFlags = upFlag;
            SendInput(1, &input, sizeof(INPUT));
            Sleep(100); // Çift tıklama arasında kısa bir bekleme
        }
    } else {
        QMessageBox::warning(this, tr("Hata"), tr("Geçersiz işlem türü seçildi!"));
    }
}

// Fare animasyon efekti
void autoclicker::animateMouseCursorHighlight()
{
    HDC hdc = GetDC(NULL);
    if (!hdc) return;

    POINT cursorPos;
    GetCursorPos(&cursorPos);

    const int maxRadius = 5; // Maksimum daire boyutu
    const int steps = 5;     // Animasyon adım sayısı
    const int delay = 1;     // Her adım arasındaki gecikme (ms)

    for (int i = 0; i < steps; ++i) {
        int radius = maxRadius * i / steps;

        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0)); // Kırmızı renkte çerçeve
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 0)); // Sarı dolgu

        HGDIOBJ hOldPen = SelectObject(hdc, hPen);
        HGDIOBJ hOldBrush = SelectObject(hdc, hBrush);

        // Daireyi büyüterek çizin
        Ellipse(hdc, cursorPos.x - radius, cursorPos.y - radius, cursorPos.x + radius, cursorPos.y + radius);

        // Orijinal kalem ve fırçayı geri yükle
        SelectObject(hdc, hOldPen);
        SelectObject(hdc, hOldBrush);

        // GDI nesnelerini temizleyin
        DeleteObject(hPen);
        DeleteObject(hBrush);

        // Gecikme ve bir sonraki adım için ekranı temizleme
        Sleep(delay);
        InvalidateRect(NULL, NULL, TRUE); // Ekranı temizle
    }

    ReleaseDC(NULL, hdc);
}
