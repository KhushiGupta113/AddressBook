#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QVector>

#include <QString>
#include <QFile>
#include <QTextStream>

    QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Contact {
    QString name;
    QString phone;
    QString address;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_sortButton_clicked();
    void on_exportButton_clicked();
    void on_themeButton_clicked();
    void on_searchButton_clicked();

private:
    Ui::MainWindow *ui;

    static const int TABLE_SIZE = 101;
    QVector<QVector<Contact>> hashTable;

    void insertContact(const QString &name, const QString &phone, const QString &address);
    bool deleteContact(const QString &name);
    Contact* findContact(const QString &name);
    int hashFunction(const QString &key);
    void refreshTable();
    void applyDarkTheme();
    void applyLightTheme();
    bool sortAscending = true;
    bool isDark = false;

    QString getNumericInput(const QString &title, const QString &label, const QString &defaultValue = QString());
};

#endif // MAINWINDOW_H
