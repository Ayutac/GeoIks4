#include "precompiledHeaders.h"
#include "NewViewDialog.h"
#include "ui_NewViewDialog.h"

NewViewDialog::NewViewDialog(QWidget *parent) :
   QDialog(parent),
   ui(new Ui::NewViewDialog)
{
   ui->setupUi(this);
}

NewViewDialog::~NewViewDialog()
{
   delete ui;
}
