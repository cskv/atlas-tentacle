#ifndef KERNDIALOG_H
#define KERNDIALOG_H

#include <QDialog>

namespace Ui {
class KernDialog;
}

class KernDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KernDialog(QWidget *parent = 0);
    ~KernDialog();

private:
    Ui::KernDialog *ui;
};

#endif // KERNDIALOG_H
