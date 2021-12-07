#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDebug>
#include "QrCode.hpp"
#include "avis.h"
#include <QSqlQueryModel>
#include <QtCharts>
#include <QChartView>
#include <QLineSeries>
using namespace QtCharts;
using namespace qrcodegen;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_client->setModel(Etmp.afficher());
    ui->CIN_2->setValidator(new QIntValidator(0,99999999,this));
    ui->NUMTEL->setValidator(new QIntValidator(0,99999999,this));
    ui->NBRETICKET->setValidator(new QIntValidator(0,100,this));
    ui->ID->setMaxLength(5);
    ui->NOM->setMaxLength(10);
    ui->NOM->setValidator(new QRegExpValidator( QRegExp("[A-Za-z- _]+"), this ));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButtonAjouter_clicked()
{
    int cin=ui->CIN_2->text().toInt(); // .toint() : chaine --> entier
    QString  id=ui->ID->text();
    QString nom=ui->NOM->text();
    int numtel=ui->NUMTEL->text().toInt();
    int nbreticket=ui->NBRETICKET->text().toInt();
    QString destination=ui->DESTINATION->currentText();
    int abonnement=ui->ABONNEMENT->text().toInt();
    QString avis=ui->avis2->text();
    int nombre=ui->nbtspoints->text().toInt();
    Client C(cin,id,nom,numtel,nbreticket,destination,abonnement,nombre);
    if (nom.isEmpty()) //empty vide
    { QMessageBox::critical(0,qApp->tr("erreur"),qApp->tr("le nom est vide."),QMessageBox::Cancel);
    }

    else
    {
bool test=C.ajouter();
if (test)
        { //Actualiser
     ui->tab_client->setModel(Etmp.afficher());
        QMessageBox:: information(nullptr, QObject::tr("DESTINATION"),
                                           QObject::tr("Ajout effectué\n"
                                                       "click cancel to exit."),QMessageBox::Cancel);
        }
    else
        QMessageBox::critical(nullptr, QObject::tr("DESTINATION"),
                              QObject::tr("Ajout non effectué.\n"
                                          "click Cancel to exit."),QMessageBox::Cancel);
}
}
void MainWindow::on_id_supp_clicked()
{
    QItemSelectionModel *select = ui->tab_client->selectionModel(); //pour selctionner la ligne

           int cin =select->selectedRows(0).value(0).data().toString().toInt();
           if(Etmp.supprimer(cin))
           {
               ui->tab_client->setModel(Etmp.afficher());
               QMessageBox:: information(nullptr, QObject::tr("DESTINATION"),
                                                  QObject::tr("Suppression effectué\n"
                                                              "click cancel to exit."),QMessageBox::Cancel);
               }
           else
               QMessageBox::critical(nullptr, QObject::tr("DESTINATION"),
                                     QObject::tr("Suppression non effectué.\n"
                                                 "click Cancel to exit."),QMessageBox::Cancel);
           }
/* void MainWindow::on_pushButtonSupprimer_clicked()
{  Client c;c.setCIN(ui->identifiant->text());
   bool test=Etmp.supprimer(c.get_cin());
    QMessageBox msgBox;

    if (test)
            {
        //actualiser
         ui->tab_client->setModel(Etmp.afficher());
            QMessageBox:: information(nullptr, QObject::tr("OK"),
                                               QObject::tr("Suppression effectué\n"
                                                           "click cancel to exit."),QMessageBox::Cancel);
            }
        else
            QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                  QObject::tr("Suppression non effectué.\n"
                                              "click Cancel to exit."),QMessageBox::Cancel);
}}
  */
void MainWindow::on_nbrjours_modifier_clicked()
{
    int cin=ui->CIN_2->text().toInt();
    QString  id=ui->identifiant->text();
    QString nom=ui->NOM->text();
    int numtel=ui->NUMTEL->text().toInt();
    int nbreticket=ui->NBRETICKET->text().toInt();
    QString destination=ui->destination->text();
    int abonnement=ui->ABONNEMENT->text().toInt();
    int nombre=ui->ABONNEMENT->text().toInt();
         Client C(cin,id,nom,numtel,nbreticket,destination,abonnement,nombre);
         if (destination.isEmpty())
             {
                 QMessageBox::critical(0,qApp->tr("erreur"),qApp->tr("remplir la destination svp."),QMessageBox::Cancel);
             }
         else
         {
     bool test=C.modifier(id);
     if (test)
             { //Actualiser
          ui->tab_client->setModel(Etmp.afficher());
             QMessageBox:: information(nullptr, QObject::tr("OK"),
                                                QObject::tr("Modifier un Client effectué©\n"
                                                            "click cancel to exit."),QMessageBox::Cancel);
             }
         else
             QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                   QObject::tr("Ajout non effectué.\n"

                                       "click Cancel to exit."),QMessageBox::Cancel);

}
}

void MainWindow::on_rechercher_button_clicked()
{
    QString rech =ui->identifiant->text();
        ui->tab_client->setModel(Etmp.rechercher(rech));
}

void MainWindow::on_trier_button_clicked()
{

        Client *c = new Client();
        ui->tab_client->setModel(c->tri());

}


void MainWindow::on_qrcodegen_clicked()
{
    int tabeq=ui->tab_client->currentIndex().row(); //selectionner le ligne
           QVariant idd=ui->tab_client->model()->data(ui->tab_client->model()->index(tabeq,0)); // selctionner la case
           QString id= idd.toString();
           QSqlQuery qry; // fournir les donnes
           qry.prepare("select * from CLIENT where CIN=:id");
           qry.bindValue(":CIN",id);
           qry.exec(); //execution
              QString  iden,nom,adresse,nbreticket,destination,abonnement ,ide; //convertir les donnes en chaine du caractere

           while(qry.next()){

               id=qry.value(1).toString();
               nom=qry.value(2).toString();
               adresse=qry.value(3).toString();
               nbreticket=qry.value(4).toString();
               destination=qry.value(5).toString();
               abonnement=qry.value(6).toString();

           }
           ide=QString(id);
                  ide="CIN:"+ide+"ID:"+iden+"NOM:"+nom+"ADRESSE:"+adresse +"NBRE_TICKET:"+nbreticket+"DESTINATION:"+destination+"ABONNEMENT:"+abonnement; // importer les donnes depuis la base
           QrCode qr = QrCode::encodeText(ide.toUtf8().constData(), QrCode::Ecc::HIGH);

           // Read the black & white pixels
           QImage im(qr.getSize(),qr.getSize(), QImage::Format_RGB888);
           for (int y = 0; y < qr.getSize(); y++) {
               for (int x = 0; x < qr.getSize(); x++) {
                   int color = qr.getModule(x, y);  // 0 for white, 1 for black

                   // You need to modify this part
                   if(color==0)
                       im.setPixel(x, y,qRgb(254, 254, 254));
                   else
                       im.setPixel(x, y,qRgb(0, 0, 0));
               }
           }
           im=im.scaled(200,200); //size
           ui->qrcodecommande->setPixmap(QPixmap::fromImage(im)); //importation

}


void MainWindow::on_ajouteravis_clicked()
{
    QString idd=ui->id3->text();
     QString AVis=ui->avis2->text();
    avis A( idd, AVis);
    if( AVis.isEmpty()&& AVis=="excellent"&& AVis=="bien"&& AVis=="mauvais"&& AVis=="moyen"&& AVis=="EXCELLENT"&& AVis=="BIEN"&& AVis=="MAUVAIS"&& AVis=="MOYEN")
                         {
                             QMessageBox::critical(0,qApp->tr("erreur"),qApp->tr("remplir l'avis."),QMessageBox::Cancel);
                         }

    else
    {
bool test=A.ajouterv2();
if (test)
        { //Actualiser
    ui->tab_avis->setModel(AA.afficherv2());

        QMessageBox:: information(nullptr, QObject::tr("OK"),
                                           QObject::tr("Ajout effectué\n"
                                                       "click cancel to exit."),QMessageBox::Cancel);
        }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"
                                          "click Cancel to exit."),QMessageBox::Cancel);
    }
}

void MainWindow::on_radioButton_croissant_clicked()
{
     ui->tab_client->setModel(Etmp.tri());
}

void MainWindow::on_radioButton_dcroissant_clicked()
{
     ui->tab_client->setModel(Etmp.trid());
}


void MainWindow::on_supprimeravis_2_clicked()
{
    QItemSelectionModel *select = ui->tab_avis->selectionModel(); //pour selctionner la ligne

           QString idd =select->selectedRows(0).value(0).data().toString();
           if(AA.supprimerv2(idd))
           {
               ui->tab_avis->setModel(AA.afficherv2());
               QMessageBox:: information(nullptr, QObject::tr("DESTINATION"),
                                                  QObject::tr("Suppression effectué\n"
                                                              "click cancel to exit."),QMessageBox::Cancel);
               }
           else
               QMessageBox::critical(nullptr, QObject::tr("DESTINATION"),
                                     QObject::tr("Suppression non effectué.\n"
                                                 "click Cancel to exit."),QMessageBox::Cancel);
}


void MainWindow::on_modifieravis_3_clicked()
{
    QString  idd=ui->id3->text();
    QString AVis=ui->avis2->text();
         avis A( idd, AVis);
         if (AVis.isEmpty()&& AVis=="excellent"&& AVis=="bien"&& AVis=="mauvais"&& AVis=="moyen"&& AVis=="EXCELLENT"&& AVis=="BIEN"&& AVis=="MAUVAIS"&& AVis=="MOYEN")
             {
                 QMessageBox::critical(0,qApp->tr("erreur"),qApp->tr("remplir la destination svp."),QMessageBox::Cancel);
             }
         else
         {
     bool test=A.modifierv2(idd);
     if (test)
             { //Actualiser
          ui->tab_avis->setModel(AA.afficherv2());
             QMessageBox:: information(nullptr, QObject::tr("OK"),
                                                QObject::tr("Modifier un Client effectué©\n"
                                                            "click cancel to exit."),QMessageBox::Cancel);
             }
         else
             QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                                   QObject::tr("Ajout non effectué.\n"

                                       "click Cancel to exit."),QMessageBox::Cancel);

}

}
// stat sur les destinations
void MainWindow::on_statsmed_clicked()
{
    QSqlQueryModel * model= new QSqlQueryModel();
     model->setQuery("select * from CLIENT where DESTINATION='Rades' ");
     int number1=model->rowCount(); // calculer la somme where destination rades
     model->setQuery("select * from CLIENT where DESTINATION='Ezzahra' ");
     int number2=model->rowCount();
     model->setQuery("select * from CLIENT where DESTINATION='Hammem lif' ");
     int number3=model->rowCount(); // calculer le nombre de voyage pour hammem lif
     model->setQuery("select * from CLIENT where DESTINATION='Autre' ");
     int number4=model->rowCount();
     int total=number1+number2+number3+number4;
     QString a = QString("rades  "+QString::number((number1*100)/total,'f',2)+"%" ); // 12.00
     QString b = QString("ezzahra "+QString::number((number2*100)/total,'f',2)+"%" );
     QString c = QString("hammem lif "+QString::number((number3*100)/total,'f',2)+"%" );
     QString d = QString("autre "+QString::number((number4*100)/total,'f',2)+"%" );
     QPieSeries *series = new QPieSeries();
     series->append(a,number1); // te9sem char gra
     series->append(b,number2);
     series->append(c,number3);
     series->append(d,number4);
     if (number1!= 0)
     {
         QPieSlice *slice = series->slices().at(0); //  bib doura
         slice->setLabelVisible();
         slice->setPen(QPen());
     }
     if (number2!=0)
     {
              // Add label, explode and define brush for 2nd slice
              QPieSlice *slice1 = series->slices().at(1);
              //slice1->setExploded();
              slice1->setLabelVisible();
     }
     if(number3!=0)
     {
              // Add labels to rest of slices
              QPieSlice *slice2 = series->slices().at(2);
              //slice1->setExploded();
              slice2->setLabelVisible();
     }
     if(number4!=0)
     {
              // Add labels to rest of slices
              QPieSlice *slice3 = series->slices().at(3);
              //slice1->setExploded();
              slice3->setLabelVisible();
     }
             // Create the chart widget
             QChart *chart = new QChart();
             // Add data to chart with title and hide legend
             chart->addSeries(series);
             chart->setTitle("pourcentage sur les destinations "+ QString::number(total));
             chart->legend()->hide();
             // Used to display the chart
             QChartView *chartView = new QChartView(chart);
             chartView->setRenderHint(QPainter::Antialiasing);
             chartView->resize(1000,500);
             chartView->show();

}


void MainWindow::on_destination_2_clicked()
{
    Client *c = new Client();
    ui->tab_client->setModel(c->tri());
}

void MainWindow::on_tab_client_activated(const QModelIndex &index)
{

        QSqlQuery query ;

        QString val=ui->tab_client->model()->data(index).toString();
          query.prepare("Select ID,NBRE_TICKET,NOMBRE from CLIENT where ID=:ID ");
          query.bindValue(":ID",val);


          if(query.exec())
             {
                 while(query.next())
                 {
                     QString ch=query.value(1).toString() ;//CH NOMBRE TICKET

                         int i=ch.toInt() ;
                         float  ff=query.value(2).toFloat() ;

                          if(i<10)
                            { ff=i+10; }
                          else if((i>20)&&(i<30))
                           {ff=i+20 ;}
                          else if((i>30)&&(i<40))
                           {ff=i+30 ;}
                          else if((i>40)&&(i<50))
                           {ff=i+40 ;}
                          else if((i>50)&&(i<60))
                           {ff=i+50 ;}
                          else if((i>60)&&(i<70))
                           {ff=i+60 ;}
                          else if((i>70)&&(i<80))
                           {ff=i+70 ;}
                          else if((i>80)&&(i<90))
                           {ff=i+80 ;}
                              else
                              ff=i+90;





                   ui->nbtspoints-> QLineEdit::setText(QString::number(ff)) ;


                 }
             }

}
