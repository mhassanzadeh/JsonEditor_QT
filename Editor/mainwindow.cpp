#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_action_Open_triggered()
{
    QFileDialog* iFileDialog = new QFileDialog(this,"Open Config File","./","Config File (*.json)");

    //TODO Please Fix File open Filter List
//    QStringList filters;
//    filters << "Image files (*.png *.xpm *.jpg)"
//            << "Text files (*.txt)"
//            << "Any files (*)";
//    iFileDialog->setNameFilters(filters);

    iFileDialog->setViewMode(QFileDialog::Detail);
    QStringList iInputFileNames;
    if (iFileDialog->exec())
        iInputFileNames = iFileDialog->selectedFiles();
    foreach (QString item, iInputFileNames) {
        ui->cmbInputFile->addItem(item.split("/").last(),QVariant(item));
    }
//    qDebug() << iInputFileNames;
}

QList<QTreeWidgetItem *>  MainWindow::createConfigTree(QMap<QString, QVariant> tree)
{
    QList<QTreeWidgetItem *> iResultTreeItems;

    foreach (QString iItem, tree.keys())
    {
        QStringList iInsertItem;
        QList<QTreeWidgetItem *> iChildsItems;

        bool hasChild = false;

        iInsertItem.append(iItem);
        iInsertItem.append(tree[iItem].toString());

        if (tree[iItem].type() == QVariant::Int)
            iInsertItem.append("Int");
        else if (tree[iItem].type() == QVariant::String)
        {
            iInsertItem[1]= QString::fromUtf8(tree[iItem].toByteArray());
            iInsertItem.append("String");
        }
        else if (tree[iItem].type() == QVariant::Map)
        {
            hasChild = true;
            iInsertItem.append("Map");
        }
        else if(tree[iItem].type() == QVariant::List)
        {
            hasChild = true;
            iInsertItem.append("List");
        }
        else
            iInsertItem.append("None");

        QTreeWidgetItem *iRootQTreeWidgetItem = new QTreeWidgetItem((QTreeWidget*)0, iInsertItem);
        if(hasChild)
        {
            if (tree[iItem].type() == QVariant::Map)
            {
                iChildsItems.append( createConfigTree(tree[iItem].toMap()));
            }
            else if(tree[iItem].type() == QVariant::List)
            {
                foreach (QVariant item, tree[iItem].toList())
                {
                    iChildsItems.append( createConfigTree(item.toMap()));
                }
            }
            foreach (QTreeWidgetItem *item, iChildsItems) {
                iRootQTreeWidgetItem->addChild(item);
            }
        }

        iResultTreeItems.append(iRootQTreeWidgetItem);
    }
    return iResultTreeItems;
}

void MainWindow::on_cmbInputFile_currentIndexChanged(int index)
{

    //    qDebug() << Q_FUNC_INFO;
        //TODO: Change algo for beter use memory
        QFile iConfigFile(ui->cmbInputFile->itemData(index).toString());
        iConfigFile.open(QIODevice::ReadOnly);
        QString iConfigFileData = iConfigFile.readAll();
        ui->txtJson->setText(iConfigFileData);
        iSyntaxHighlighter = new SyntaxHighlighter(ui->txtJson->document());

//        qDebug() << Q_FUNC_INFO <<  iConfigFileData.simplified();
//        qDebug() << Q_FUNC_INFO <<  jSonParsed;

}

void MainWindow::on_pushButton_clicked()
{
    on_action_Save_triggered();
}

void MainWindow::on_txtJson_textChanged()
{
    jSonParsed = iJson.decode(ui->txtJson->document()->toPlainText());
    ui->twdConfigTree->clear();
    ui->twdConfigTree->insertTopLevelItems(0,createConfigTree(jSonParsed));
    ui->twdConfigTree->expandAll();
}

void MainWindow::on_action_Save_triggered()
{
    QFileDialog* iFileDialog = new QFileDialog();

    QFile iConfigFile(iFileDialog->getSaveFileName(this,"Save Config File","./","Config File (*.json)"));
    iConfigFile.open(QIODevice::WriteOnly);
    iConfigFile.write(ui->txtJson->document()->toPlainText().toLocal8Bit());
    iConfigFile.flush();
    iConfigFile.close();
}
