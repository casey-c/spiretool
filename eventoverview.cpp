#include "eventoverview.h"
#include "ui_eventoverview.h"

#include "src/h/carddatabase.h"

#include <QShortcut>
#include <QStandardItemModel>
#include <QDebug>

enum HEADER {
    NAME,
    SEEN,
    PICKS,
    SKIPPED
};
#define HEADER_COUNT 4

EventOverview::EventOverview(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventOverview)
{
    ui->setupUi(this);
    connect( new QShortcut(QKeySequence("Q"), this), &QShortcut::activated, this, &EventOverview::close );

    CardDatabase* cardDB = new CardDatabase();
    QList<CardStatistics*> cardStats = cardDB->getCardStats();

    int numCards = cardStats.length();
    qDebug() << "numTotalCards" << numCards;

    QStandardItemModel* model = new QStandardItemModel(1000, HEADER_COUNT);
    model->setHeaderData(NAME, Qt::Horizontal, "Name");
    model->setHeaderData(SEEN, Qt::Horizontal, "Seen");
    model->setHeaderData(PICKS, Qt::Horizontal, "Picks");
    //model->setHeaderData(CHOSE_OTHER, Qt::Horizontal, "Chose Other");
    model->setHeaderData(SKIPPED, Qt::Horizontal, "Skipped");

    ui->tableView->setModel(model);
    //ui->tableView->setSortingEnabled(true);
    ui->tableView->setColumnWidth(NAME, 250);
    ui->tableView->setColumnWidth(SEEN, 80);
    ui->tableView->setColumnWidth(PICKS, 80);
    ui->tableView->setColumnWidth(SKIPPED, 80);


    int index = 0;
    for (CardStatistics* card : cardStats) {
        qDebug() << card->getClassName();

        if (card->getClassName() == "Watcher") {
            model->setData(model->index(index, NAME), card->getName());
            model->setData(model->index(index, SEEN), card->getTimesSeen());
            model->setData(model->index(index, PICKS), card->getTimesPicked());
            //model->setData(model->index(index, CHOSE_OTHER), card->getTimesChoseOther());
            model->setData(model->index(index, SKIPPED), card->getTimesSkipped());
            //model->setData(model->index(index, 1), );
            ++index;
        }

    }
    model->setRowCount(index);


}

EventOverview::~EventOverview()
{
    delete ui;
}
