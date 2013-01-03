#ifndef FILESMANAGER_H
#define FILESMANAGER_H

#include "contentmanager.h"

class FilesManager : public ContentManager
{
    Q_OBJECT
public:
    explicit FilesManager(QObject *parent = 0);
    virtual ~FilesManager();

signals:
    void signalFirstPanelIsLoaded();

private slots:
        void slotSectionClicked(int logicalIndex);

public:
    QString getParentFolderUrl(void) const;
    Items::Data getUpperLevelFolderInfo(void) const;
    QTreeWidget* getPanel(void) const;
    QString back(void);
    void setPanel(QTreeWidget *p, int pn);
    void sort(int column, Qt::SortOrder order);
    Items::Data getCurrentFileInfo(void);
    QStringList getPathesURLs(void) const;
    void setPathesURLs(const QStringList &pathesURLsStrList);

private:
    virtual void show(void);

private:
    void setItems(Items::Data::ESortOrder itemSortOrder, Qt::SortOrder sortOrder);
    void addItem(const Items::Data &itemData);
    QString getDate(const QString &date);
    QString getSize(const QString &size);
    void updateItemsState(void);

private:
    QTreeWidget *panel;
    QStringList pathesURLs;
    Items::Data rootData;
    QList<Items::Data> normalizedItems;
    bool isRoot;
    int panelNum;
};

#endif // FILESMANAGER_H
