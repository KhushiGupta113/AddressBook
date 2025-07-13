#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QRegularExpression>
#include <QHeaderView>
#include <QApplication>
#include <QScreen>
#include <QGuiApplication>
#include <QRegularExpressionValidator>
#include "phonenumberdialog.h"
#include "editcontactdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , hashTable(TABLE_SIZE)
{
    setWindowIcon(QIcon("E:/AddressBookv1-2345/AddressBookv1/icon.jpg"));
    // setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "Name" << "Phone Number" << "Address");
    // Divide columns equally
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set default window size and center using QScreen (Qt 6+)
    resize(900, 600);
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    move(screenGeometry.center() - rect().center());

    applyLightTheme();
    isDark = false;

    // Connect search button
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::on_searchButton_clicked);
    // Optional: connect Enter key in searchLineEdit
    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, &MainWindow::on_searchButton_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::hashFunction(const QString &key) {
    int hash = 0;
    for (QChar ch : key.toLower())
        hash = (hash * 31 + ch.unicode()) % TABLE_SIZE;
    return hash;
}

void MainWindow::insertContact(const QString &name, const QString &phone, const QString &address) {
    int index = hashFunction(name);
    for (auto &c : hashTable[index]) {
        if (c.name == name) {
            c.phone = phone;
            c.address = address;
            return;
        }
    }
    hashTable[index].append({name, phone, address});
}

bool MainWindow::deleteContact(const QString &name) {
    int index = hashFunction(name);
    for (int i = 0; i < hashTable[index].size(); ++i) {
        if (hashTable[index][i].name == name) {
            hashTable[index].remove(i);
            return true;
        }
    }
    return false;
}

Contact* MainWindow::findContact(const QString &name) {
    int index = hashFunction(name);
    for (auto &c : hashTable[index]) {
        if (c.name == name)
            return &c;
    }
    return nullptr;
}

void MainWindow::refreshTable() {
    ui->tableWidget->setRowCount(0);
    for (const auto &bucket : hashTable) {
        for (const auto &c : bucket) {
            int row = ui->tableWidget->rowCount();
            ui->tableWidget->insertRow(row);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(c.name));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(c.phone));
            ui->tableWidget->setItem(row, 2, new QTableWidgetItem(c.address));
        }
    }
}

QString MainWindow::getNumericInput(const QString &title, const QString &label, const QString &defaultValue) {
    QString input;
    bool ok = false;
    QRegularExpression rx("^\\d+$");
    do {
        input = QInputDialog::getText(this, title, label, QLineEdit::Normal, defaultValue, &ok);
        if (!ok || input.isEmpty()) return QString();
        if (!rx.match(input).hasMatch()) {
            QMessageBox::warning(this, "Invalid Input", "Please enter digits only for the phone number.");
        }
    } while (!rx.match(input).hasMatch());
    return input;
}

void MainWindow::on_addButton_clicked() {
    QString name = QInputDialog::getText(this, "Add Contact", "Name:");
    if (name.isEmpty()) return;
    QString phone;
    QRegularExpression rx("^\\d+$");
    bool ok = false;
    do {
        phone = QInputDialog::getText(this, "Add Contact", "Phone Number:");
        if (phone.isEmpty()) return;
        if (!rx.match(phone).hasMatch()) {
            QMessageBox::warning(this, "Invalid Input", "Please enter digits only for the phone number.");
        } else {
            ok = true;
        }
    } while (!ok);
    QString address = QInputDialog::getText(this, "Add Contact", "Address:");
    if (address.isEmpty()) return;

    insertContact(name, phone, address);
    refreshTable();
}

void MainWindow::on_editButton_clicked() {
    int row = ui->tableWidget->currentRow();
    if (row < 0) return;

    QString oldName = ui->tableWidget->item(row, 0)->text();
    Contact* c = findContact(oldName);
    if (!c) return;

    QString newName = QInputDialog::getText(this, "Edit Contact", "Name:", QLineEdit::Normal, c->name);
    if (newName.isEmpty()) return;
    QString newPhone;
    QRegularExpression rx("^\\d+$");
    bool ok = false;
    do {
        newPhone = QInputDialog::getText(this, "Edit Contact", "Phone Number:", QLineEdit::Normal, c->phone);
        if (newPhone.isEmpty()) return;
        if (!rx.match(newPhone).hasMatch()) {
            QMessageBox::warning(this, "Invalid Input", "Please enter digits only for the phone number.");
        } else {
            ok = true;
        }
    } while (!ok);
    QString newAddress = QInputDialog::getText(this, "Edit Contact", "Address:", QLineEdit::Normal, c->address);
    if (newAddress.isEmpty()) return;

    // If the name was changed, update the contact key
    if (newName != c->name) {
        deleteContact(c->name);
        insertContact(newName, newPhone, newAddress);
    } else {
        c->phone = newPhone;
        c->address = newAddress;
    }
    refreshTable();
}

void MainWindow::on_deleteButton_clicked() {
    int row = ui->tableWidget->currentRow();
    if (row < 0) return;

    QString name = ui->tableWidget->item(row, 0)->text();
    if (deleteContact(name)) {
        refreshTable();
    }
}

void MainWindow::on_sortButton_clicked() {
    QVector<Contact> allContacts;
    for (auto &bucket : hashTable)
        allContacts += bucket;

    // Toggle sort order
    sortAscending = !sortAscending;

    // Sort based on the flag
    if (sortAscending) {
        std::sort(allContacts.begin(), allContacts.end(), [](const Contact &a, const Contact &b) {
            return a.name.toLower() < b.name.toLower();
        });
    } else {
        std::sort(allContacts.begin(), allContacts.end(), [](const Contact &a, const Contact &b) {
            return a.name.toLower() > b.name.toLower();
        });
    }

    ui->tableWidget->setRowCount(0);
    for (const auto &c : allContacts) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(c.name));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(c.phone));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(c.address));
    }
}

void MainWindow::on_exportButton_clicked() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save CSV", "", "*.csv");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << "Name,Phone Number,Address\n";
        for (auto &bucket : hashTable) {
            for (auto &c : bucket) {
                out << "\"" << c.name << "\",\"" << c.phone << "\",\"" << c.address << "\"\n";
            }
        }
        file.close();
        QMessageBox::information(this, "Export", "Exported successfully.");
    }
}

void MainWindow::on_searchButton_clicked() {
    QString searchName = ui->searchLineEdit->text().trimmed();
    if (searchName.isEmpty()) {
        refreshTable();
        return;
    }
    ui->tableWidget->setRowCount(0);
    Contact* c = findContact(searchName);
    if (c) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(c->name));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(c->phone));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(c->address));
    }
}

void MainWindow::applyDarkTheme() {
    setStyleSheet(R"(
        QMainWindow { background-color: #090933; border-radius: 40px; border: 1.5px solid #444; }
        QWidget { color: white; }
        QPushButton {
            background: #22223b;
            color: #fff;
            border-radius: 8px;
            padding: 8px 20px;
            font-size: 12pt;
        }
        QPushButton:hover, QPushButton:pressed {
            background: #4a4e69;
        }
        QLineEdit {
            color: white;
            background: #22223b;
            border-radius: 8px;
            border: 1px solid #444;
        }
        QInputDialog QLabel, QMessageBox QLabel {
            color: white;
        }
        QInputDialog, QMessageBox {
            background: #22223b;
        }
        QDialog QPushButton, QMessageBox QPushButton {
            background: #22223b;
            color: #fff;
            border-radius: 8px;
            padding: 8px 20px;
            font-size: 12pt;
        }
        QDialog QPushButton:hover, QDialog QPushButton:pressed,
        QMessageBox QPushButton:hover, QMessageBox QPushButton:pressed {
            background: #4a4e69;
        }
    )");
}

void MainWindow::applyLightTheme() {
    setStyleSheet(R"(
        QMainWindow { background-color: #D3D3FF; border-radius: 40px; border: 1.5px solid #444; }
        QWidget { color: black; }
        QPushButton {
            background: #f2e9e4;
            color: #22223b;
            border-radius: 8px;
            padding: 8px 20px;
            font-size: 12pt;
        }
        QPushButton:hover, QPushButton:pressed {
            background: #c9ada7;
        }
        QLineEdit {
            color: #22223b;
            background: #fff;
            border-radius: 8px;
            border: 1px solid #b0b0b0;
        }
        QInputDialog QLabel, QMessageBox QLabel {
            color: #22223b;
        }
        QInputDialog, QMessageBox {
            background: #fff;
        }
        QDialog QPushButton, QMessageBox QPushButton {
            background: #f2e9e4;
            color: #22223b;
            border-radius: 8px;
            padding: 8px 20px;
            font-size: 12pt;
        }
        QDialog QPushButton:hover, QDialog QPushButton:pressed,
        QMessageBox QPushButton:hover, QMessageBox QPushButton:pressed {
            background: #c9ada7;
        }
    )");
}

void MainWindow::on_themeButton_clicked() {
    isDark = !isDark;
    if (isDark)
        applyDarkTheme();
    else
        applyLightTheme();
}
