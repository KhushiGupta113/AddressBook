#include "phonenumberdialog.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QDialogButtonBox>

PhoneNumberDialog::PhoneNumberDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Phone Number");
    setStyleSheet("background: #fff;");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QLabel *label = new QLabel("Phone Number:", this);
    mainLayout->addWidget(label);
    lineEdit = new QLineEdit(this);
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression("\\d*"), this);
    lineEdit->setValidator(validator);
    mainLayout->addWidget(lineEdit);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttonBox->setStyleSheet(
        "QPushButton {"
        "  background: #f6ede9;"
        "  color: #22223b;"
        "  border-radius: 8px;"
        "  padding: 8px 20px;"
        "  font-size: 12pt;"
        "}"
        "QPushButton:hover {"
        "  background: #c9ada7;"
        "}"
    );
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(lineEdit, &QLineEdit::textEdited, this, &PhoneNumberDialog::onTextEdited);
}

void PhoneNumberDialog::onTextEdited(const QString &text) {
    if (!text.isEmpty() && !text.back().isDigit()) {
        QMessageBox::warning(this, "Invalid Input", "Only numbers are allowed.");
        QString fixed = text;
        fixed.chop(1);
        lineEdit->setText(fixed);
    }
}

QString PhoneNumberDialog::getPhoneNumber(QWidget *parent, const QString &title, const QString &label) {
    PhoneNumberDialog dialog(parent);
    dialog.setWindowTitle(title);
    dialog.lineEdit->clear();
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.lineEdit->text();
    }
    return QString();
} 