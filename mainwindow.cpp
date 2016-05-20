/**
 * @file mainwindow.cpp
 * @brief le fichier mainwindow.cpp est le fichier ou toute les fonctions sont codé
 * @author Vieux Thomas
 * @date 04/09/15
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
/**
 * @brief noId est déclarer ici car il doit servir dans 2 fonction
 * il va servir a dans un premier temp de prendre la valeur d'une case selectioner
 * et ensuite etre implanter dans une requete SQL
 */

int noId;

/**
 * @brief MainWindow::MainWindow
 * @param parent
 * MainWindow est la fonction principal du programme
 * tout ce qui est a l'interieur sera efectuer avant les autres fonctions
 * il va servir dans un premier temp a demander a l'utilisateur le nom d'utilisateur et le mot de passe de la base de donnée SQL pour ensuite etablir une connexion avec
 * si un des deux champ rentrer et faux l'utilisateur sera avertie par un message d'erreur
 * si une connexion est établie le programme va créer les tables est insérer les données de la base de donnée au différentes table créer
 * les données pris dans la bases sont trier grace a des requetes SQL
 * a chaque fois qu'une action se passe le programme repasse par ce code pour faire les modifications necesaire
 */

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->lineEdit_MDP->setText("lotop27890");
    ui->lineEdit_Utilisateur->setText("thomasvieux");
    ui->pushButton_Deconnexion->setVisible(false);
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    maBase=new QSqlDatabase(db);
    maBase->setHostName("localhost");
    maBase->setDatabaseName("NewWorld");
         maBase->setUserName("tvieux");
         maBase->setPassword("ini01");
         bool ok = maBase->open();
         if(!ok)
         {
             QMessageBox::warning(this,"New World BackOffice","la connexion a la base de donnée a echouer \n Veuillez verifier que le service mysql est lancer sur localhost");
         }
         else
         {
             modelProduit=new QSqlRelationalTableModel(this,*maBase);
             modelProduit->setTable("produit");
             modelProduit->setRelation(1,QSqlRelation("surType","no","libelle"));
             modelProduit->setRelation(4,QSqlRelation("typeP","noType","libelle"));


             ui->tableView_Produit->setItemDelegate(new QSqlRelationalDelegate(modelProduit));
             ui->tableView_Produit->setModel(modelProduit);
             ui->tableView_Produit->hideColumn(3);
             //ui->tableView_Produit->hideColumn(0);

             modelProduit->setFilter("modere=true");
             modelProduit->setHeaderData(1, Qt::Horizontal, tr("Rayon"));
             modelProduit->setHeaderData(2, Qt::Horizontal, tr("Produit"));
             modelProduit->setHeaderData(4, Qt::Horizontal, tr("TypeProduit"));
             modelProduit->select();

             QSqlQuery reqMin;
             reqMin.prepare("select min(no) from produit where modere=false");
             reqMin.exec();
             reqMin.first();
             int min=reqMin.value(0).toInt();

             QSqlQuery reqNum;
             reqNum.exec("select count(*) from produit where modere=false");
             reqNum.first();
             int num=reqNum.value(0).toInt();

             ui->label_Demande->setNum(num);



             modelProduitF=new QSqlRelationalTableModel(this,*maBase);
             modelProduitF->setTable("produit");
             ui->tableView_ProduitF->setModel(modelProduitF);
             //modelProduitF->setRelation(1,QSqlRelation("surType","no","libelle"));
             //modelProduitF->setRelation(4,QSqlRelation("typeP","noType","libelle"));
             ui->tableView_ProduitF->setItemDelegate(new QSqlRelationalDelegate(modelProduitF));
             ui->tableView_ProduitF->setModel(modelProduitF);
             modelProduitF->setFilter("modere=false and no="+QString::number(min)+"");
             //ui->tableView_ProduitF->hideColumn(0);
             modelProduitF->setHeaderData(1, Qt::Horizontal, tr("Rayon"));
             //ui->tableView_ProduitF->hideColumn(3);

             modelProduitF->select();

             modelRayon=new QSqlRelationalTableModel(this,*maBase);
             modelRayon->setTable("surType");
             ui->tableView_Rayon->setModel(modelRayon);
             ui->tableView_Rayon->hideColumn(0);
             modelRayon->select();


             modelTypeDeProduit=new QSqlRelationalTableModel(this,*maBase);
             modelTypeDeProduit->setTable("typeP");
             ui->tableView_TypeDeProduit->setModel(modelTypeDeProduit);

             modelTypeDeProduit->setRelation(1,QSqlRelation("surType","no","libelle"));
             ui->tableView_TypeDeProduit->setItemDelegate(new QSqlRelationalDelegate(modelTypeDeProduit));
             ui->tableView_TypeDeProduit->setModel(modelTypeDeProduit);

             ui->tableView_TypeDeProduit->hideColumn(0);
             modelTypeDeProduit->select();


         }
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @fn MainWindow::on_action_Fermer_triggered
 * @brief cette fonction est associé au bouton fermer
 * de ce fait l'utilisateur poura quitter sont application grace a ce bouton
 */

void MainWindow::on_action_Fermer_triggered()
{
    close();
}

/**
 * @fn MainWindow::on_pushButton_Connexion_clicked
 * @brief la fonction est activée des que l'utilisateur cliquera sur le bouton connexion qui est dans le menu pour se connecter
 * les deux variables identifiant et mdp prendront pour valeur ce que l'utilisateur auras taper
 * ensuite la fonction comparera ce que l'utilisateur auras taper et les données des utilisateurs dans la bases de donnée grace a une requete SQL
 * si l'utilisateur a rentrer de mauvaise donnée il sera informer par un message d'erreur et l'acces lui sera refuser
 * si l'utilisateur a rentre de bonne donnée il poura acceder au programme
 */

void MainWindow::on_pushButton_Connexion_clicked()
{
    QString identifiant=ui->lineEdit_Utilisateur->text();
    QString mdp=ui->lineEdit_MDP->text();

    QSqlQuery verif;
    verif.prepare("select count(*) from user where userIdentifiant='"+identifiant+"' and userMDP='"+mdp+"'");

    verif.exec();
    verif.first();
    int req = verif.value(0).toInt();

    if(req==0)
    {
    #include <QSqlDatabase>    ui->statusBar->showMessage("Votre identifiant ou votre mot de passe est faux",3000);
    }
    else
    {
        ui->statusBar->showMessage("Vous vous ete bien connecter !!!",3000);
        ui->pushButton_Connexion->setVisible(false);
        ui->pushButton_Deconnexion->setVisible(true);
        ui->tabWidget_Moderation->setEnabled(true);
        ui->tableView_Produit->setEnabled(true);
        ui->tableView_Rayon->setEnabled(true);
        ui->lineEdit_Utilisateur->setEnabled(false);
        ui->lineEdit_MDP->setEnabled(false);


    }


}

/**
 * @fn MainWindow::on_pushButton_Deconnexion_clicked
 * @brief la fonction est activée des que l'utilisateur cliquera sur le bouton deconnexion
 * tout les champs du programme seront griser et il ne pouras plus faire quoi que ce soit
 * les champs utilisateur et mdp seront effacer pour que le prochain utilisateur puisse taper ses identifiants
 */

void MainWindow::on_pushButton_Deconnexion_clicked()
{

    ui->pushButton_Connexion->setVisible(true);
    ui->pushButton_Deconnexion->setVisible(false);
    ui->tabWidget_Moderation->setEnabled(false);
    ui->tableView_Produit->setEnabled(false);
    ui->lineEdit_Utilisateur->setEnabled(true);
    ui->lineEdit_MDP->setEnabled(true);
    ui->lineEdit_Utilisateur->setText("");
    ui->lineEdit_MDP->setText("");
}

/**
 * @fn MainWindow::on_pushButton_AccepterProduit_clicked
 * @brief cette fonction va permettre a l'utilisateur de trier les nouveaux produits qui souhaite avoir dans sa bases de donnée
 * pour ce faire, la fonction va prendre la valeur du produit présent et l'insérer dans une requete SQL pour modifier une valeur dans la base
 */

void MainWindow::on_pushButton_AccepterProduit_clicked()
{
    QSqlQuery reqMin;
    reqMin.prepare("select min(no) from produit where modere=false");
    reqMin.exec();
    reqMin.first();
    int min=reqMin.value(0).toInt();

    QSqlQuery accepter;
    accepter.prepare("update produit set modere=1 where no="+QString::number(min)+";");
    accepter.exec();
    modelProduit=new QSqlRelationalTableModel(this,*maBase);
    modelProduit->setTable("produit");
    ui->tableView_Produit->setModel(modelProduit);
    modelProduit->setFilter("modere=true");
    modelProduit->select();

    reqMin.exec();
    reqMin.first();
    min=reqMin.value(0).toInt();

    modelProduitF=new QSqlRelationalTableModel(this,*maBase);
    modelProduitF->setTable("produit");
    ui->tableView_ProduitF->setModel(modelProduitF);
    modelProduitF->setFilter("modere=false and no="+QString::number(min)+"");

    modelProduitF->select();

}

/**
 * @fn MainWindow::on_pushButton_RefuserProduit_clicked
 * @brief cette fonction va permettre a l'utilisateur de trier les nouveaux produits qui souhaite ne pas avoir dans sa bases de donnée
 * pour ce faire, la fonction va prendre la valeur du produit présent et l'insérer dans une requete SQL pour modifier une valeur dans la base
 */

void MainWindow::on_pushButton_RefuserProduit_clicked()
{
    QSqlQuery reqMin;
    reqMin.prepare("select min(no) from produit where modere=false");
    reqMin.exec();
    reqMin.first();
    int min=reqMin.value(0).toInt();

    QSqlQuery refuser;
    QString texteReq="delete from produit where no="+QString::number(min);

    qDebug()<<texteReq;
    refuser.exec(texteReq);
    modelProduit=new QSqlRelationalTableModel(this,*maBase);
    modelProduit->setTable("produit");
    ui->tableView_Produit->setModel(modelProduit);
    modelProduit->setFilter("modere=true");
    modelProduit->select();

    reqMin.exec();
    reqMin.first();
    min=reqMin.value(0).toInt();

    modelProduitF=new QSqlRelationalTableModel(this,*maBase);
    modelProduitF->setTable("produit");
    ui->tableView_ProduitF->setModel(modelProduitF);
    modelProduitF->setFilter("modere=false and no="+QString::number(min)+"");

    modelProduitF->select();

    QSqlQuery reqNum;
    reqNum.exec("select count(*) from produit where modere=false");
    reqNum.first();
    int num=reqNum.value(0).toInt();

    ui->label_Demande->setNum(num);


}

/**
 * @fn MainWindow::on_tableView_Produit_clicked
 * @param index
 * @brief la fonction va prendre la valeur dans la case ou l'utilisateur a cliquer
 * ensuite il va le mettre dans une variable qui activera deux bouton : supprimer ou modifier
 */

void MainWindow::on_tableView_Produit_clicked(const QModelIndex &index)
{
    qDebug()<<index.data(0).toInt();
    noId=index.data(0).toInt();
    ui->label_mdr->setNum(noId);
    ui->pushButton_Supprimer->setEnabled(true);
    ui->pushButton_Modifier->setEnabled(true);




}

/**
 * @fn MainWindow::on_pushButton_Supprimer_clicked
 * @brief la fonction va suprimmer de la bases les données selectionner par l'utilisateur grace a l'id qui a ete prise dans une autre fonction
 * pour ce faire une requete SQL est envoiler a la base de données et va supprimer toute les données concernant l'id qui a ete prise
 */

void MainWindow::on_pushButton_Supprimer_clicked()
{
    QSqlQuery reqMin;
    reqMin.prepare("select min(no) from produit where modere=false");
    reqMin.exec();
    reqMin.first();
    int min=reqMin.value(0).toInt();
    QSqlQuery reqSupr;
    reqSupr.exec("delete from produit where no="+QString::number(noId));

    modelProduitF=new QSqlRelationalTableModel(this,*maBase);
    modelProduitF->setTable("produit");
    ui->tableView_ProduitF->setModel(modelProduitF);
    modelProduitF->setFilter("modere=false and no="+QString::number(min)+"");
    modelProduitF->select();
}

/**
 * @fn MainWindow::on_pushButton_AjoutRayon_clicked
 * @brief la fonction va ajouter un nouveau rayon a la bases de donnée grace a des requete SQL
 */

void MainWindow::on_pushButton_AjoutRayon_clicked()
{
    QString nvRayon;
    nvRayon=ui->lineEdit_AjoutRayon->text();

    QSqlQuery reqNumMax;
    reqNumMax.exec("select max(no)+1 from surType");
    reqNumMax.first();
    int max=reqNumMax.value(0).toInt();

    QSqlQuery reqRayon;
    reqRayon.exec("insert into surType values ("+QString::number(max)+",'"+nvRayon+"');");



    modelRayon=new QSqlRelationalTableModel(this,*maBase);
    modelRayon->setTable("surType");
    ui->tableView_Rayon->setModel(modelRayon);
    ui->tableView_Rayon->hideColumn(0);
    modelRayon->select();
}

/**
 * @fn MainWindow::on_pushButton_AjoutTypeDeRayon_2_clicked
 * @bug cette fonction n'est pas terminer
 */

//void MainWindow::on_pushButton_AjoutTypeDeRayon_2_clicked()
//{
    //QString produitT,RayonT;
    //produitT=ui->lineEdit_AjoutTypeDeProduit->setText();
    //RayonT=ui->lineEdit_AjoutTypeDeRayon->setText();

//}
