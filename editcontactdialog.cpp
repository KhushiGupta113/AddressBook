#include "editcontactdialog.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include <QPalette>

EditContactDialog::EditContactDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Edit Contact");
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *nameLabel = new QLabel("Name:", this);
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Name");
    QLabel *phoneLabel = new QLabel("Phone Number:", this);
    phoneEdit = new QLineEdit(this);
    phoneEdit->setPlaceholderText("Phone Number");
    QLabel *addressLabel = new QLabel("Address:", this);
    addressEdit = new QLineEdit(this);
    addressEdit->setPlaceholderText("Address");

    // Detect dark mode
    bool isDarkMode = (palette().color(QPalette::Window).lightness() < 128);
    if (isDarkMode) {
        setStyleSheet("background: #221f44; color: #fff;");
        QString lineEditStyle =
            "QLineEdit {"
            " background: #221f44;"
            " color: #fff;"
            " border-radius: 8px;"
            " padding: 6px 10px;"
            " font-size: 12pt;"
            " border: 1px solid #fff;"
            "}";
        nameLabel->setStyleSheet("color: #fff;");
        phoneLabel->setStyleSheet("color: #fff;");
        addressLabel->setStyleSheet("color: #fff;");
        nameEdit->setStyleSheet(lineEditStyle);
        phoneEdit->setStyleSheet(lineEditStyle);
        addressEdit->setStyleSheet(lineEditStyle);
    } else {
        setStyleSheet("background: #fff; color: #22223b;");
        QString lineEditStyle =
            "QLineEdit {"
            " background: #fff;"
            " color: #22223b;"
            " border-radius: 8px;"
            " padding: 6px 10px;"
            " font-size: 12pt;"
            " border: 1px solid #22223b;"
            "}";
        nameLabel->setStyleSheet("color: #22223b;");
        phoneLabel->setStyleSheet("color: #22223b;");
        addressLabel->setStyleSheet("color: #22223b;");
        nameEdit->setStyleSheet(lineEditStyle);
        phoneEdit->setStyleSheet(lineEditStyle);
        addressEdit->setStyleSheet(lineEditStyle);
    }

    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(nameEdit);
    mainLayout->addWidget(phoneLabel);
    mainLayout->addWidget(phoneEdit);
    mainLayout->addWidget(addressLabel);
    mainLayout->addWidget(addressEdit);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
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
}

void EditContactDialog::setValues(const QString &name, const QString &phone, const QString &address) {
    nameEdit->setText(name);
    phoneEdit->setText(phone);
    addressEdit->setText(address);
}

void EditContactDialog::onPhoneEdited(const QString &text) {
    if (!text.isEmpty() && !text.back().isDigit()) {
        QMessageBox::warning(this, "Invalid Input", "Only numbers are allowed.");
        QString fixed = text;
        fixed.chop(1);
        phoneEdit->setText(fixed);
    }
}

bool EditContactDialog::getContact(QWidget *parent, QString &name, QString &phone, QString &address) {
    EditContactDialog dialog(parent);
    dialog.setValues(name, phone, address);
    if (dialog.exec() == QDialog::Accepted) {
        name = dialog.nameEdit->text();
        phone = dialog.phoneEdit->text();
        address = dialog.addressEdit->text();
        return true;
    }
    return false;
} 