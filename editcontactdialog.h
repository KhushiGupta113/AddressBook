#ifndef EDITCONTACTDIALOG_H
#define EDITCONTACTDIALOG_H

#include <QDialog>
#include <QString>

class QLineEdit;
class QDialogButtonBox;

class EditContactDialog : public QDialog {
    Q_OBJECT
public:
    explicit EditContactDialog(QWidget *parent = nullptr);
    void setValues(const QString &name, const QString &phone, const QString &address);
    static bool getContact(QWidget *parent, QString &name, QString &phone, QString &address);
private slots:
    void onPhoneEdited(const QString &text);
private:
    QLineEdit *nameEdit;
    QLineEdit *phoneEdit;
    QLineEdit *addressEdit;
    QDialogButtonBox *buttonBox;
};

#endif // EDITCONTACTDIALOG_H 