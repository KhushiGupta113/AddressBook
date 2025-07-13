#ifndef PHONENUMBERDIALOG_H
#define PHONENUMBERDIALOG_H

#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;

class PhoneNumberDialog : public QDialog {
    Q_OBJECT
public:
    explicit PhoneNumberDialog(QWidget *parent = nullptr);
    static QString getPhoneNumber(QWidget *parent = nullptr, const QString &title = "Phone Number", const QString &label = "Phone Number:");
private slots:
    void onTextEdited(const QString &text);
private:
    QLineEdit *lineEdit;
    QPushButton *okButton;
};

#endif // PHONENUMBERDIALOG_H 