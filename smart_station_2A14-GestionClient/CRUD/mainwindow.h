#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"
#include "avis.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButtonAjouter_clicked();

    void on_id_supp_clicked();


    void on_nbrjours_modifier_clicked();

    void on_rechercher_button_clicked();

    void on_trier_button_clicked();

    void on_qrcodegen_clicked();

    void on_ajouteravis_clicked();

    void on_radioButton_croissant_clicked();

    void on_radioButton_dcroissant_clicked();


    void on_supprimeravis_2_clicked();

    void on_modifieravis_3_clicked();

    void on_statsmed_clicked();

    void on_destination_2_clicked();

    void on_tab_client_activated(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    Client Etmp;
    avis AA;
};

#endif // MAINWINDOW_H
