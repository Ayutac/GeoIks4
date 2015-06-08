#ifndef NEWVIEWDIALOG_H
#define NEWVIEWDIALOG_H

#include <QDialog>

namespace Ui {
class NewViewDialog;
}

class NewViewDialog : public QDialog
{
   Q_OBJECT

public:
   explicit NewViewDialog(QWidget *parent = 0);
   ~NewViewDialog();

private:
   Ui::NewViewDialog *ui;
};

#endif // NEWVIEWDIALOG_H
