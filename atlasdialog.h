#ifndef ATLASDIALOG_H
#define ATLASDIALOG_H

#include <QDialog>

namespace Ui {
class AtlasDialog;
}

class AtlasDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AtlasDialog(QWidget *parent = 0);
    ~AtlasDialog();

private:
    Ui::AtlasDialog *ui;
};

#endif // ATLASDIALOG_H
