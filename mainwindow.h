#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_action_Fermer_triggered();

    void on_pushButton_Connexion_clicked();

    void on_pushButton_Deconnexion_clicked();

    void on_pushButton_AccepterProduit_clicked();

    void on_pushButton_RefuserProduit_clicked();

    void on_tableView_Produit_clicked(const QModelIndex &index);

    void on_pushButton_Supprimer_clicked();

    void on_pushButton_AjoutRayon_clicked();

    //void on_pushButton_AjoutTypeDeRayon_2_clicked();

    /**
     * @brief c'est ici que toute les tables sont declarer
     */

private:
    Ui::MainWindow *ui;
    QSqlDatabase * maBase;
    QSqlRelationalTableModel * modelProduit;
    QSqlRelationalTableModel * modelProduitF;
    QSqlRelationalTableModel * modelRayon;
    QSqlRelationalTableModel * modelTypeDeProduit;
};

#endif // MAINWINDOW_H
