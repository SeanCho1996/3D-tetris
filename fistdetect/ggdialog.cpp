#include "ggdialog.h"
#include "ui_ggdialog.h"

ggDialog::ggDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ggDialog)
{
    ui->setupUi(this);
}

ggDialog::~ggDialog()
{
    delete ui;
}

void ggDialog::on_exitbutton_clicked()
{
    QApplication::exit();
}

void ggDialog::on_againbutton_clicked()
{
    QApplication::closeAllWindows();
    QProcess::startDetached(QApplication::applicationFilePath(),QStringList());
}

void ggDialog::setscore(int score)
{
    ui->score->setText(QString::number(score));
}

void ggDialog::closeEvent(QCloseEvent *event)
{
    event->accept();
    QApplication::exit();
}
