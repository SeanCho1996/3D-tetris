#ifndef GGDIALOG_H
#define GGDIALOG_H

#include <QDialog>
#include <QtWidgets>

namespace Ui {
class ggDialog;
}

class ggDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ggDialog(QWidget *parent = nullptr);
    ~ggDialog();
    void setscore(int score);

private slots:
    void on_exitbutton_clicked();
    void on_againbutton_clicked();

private:
    Ui::ggDialog *ui;
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // GGDIALOG_H
