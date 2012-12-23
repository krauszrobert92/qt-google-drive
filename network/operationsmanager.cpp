#include "network/operationsmanager.h"
#include "share/commontools.h"
#include "core/driveengine.h"
#include "share/debug.h"
#include "gui/forms/sharedialog.h"
#include <QStringList>

OperationsManager::OperationsManager(QObject *parent):
    NetworkManager(parent),
    currentOperation(ENone),
    isMove(false)
{
}

void OperationsManager::deleteFile(const QString &sourceUrl)
{
    currentOperation = EDelete;

    CommonTools::setHeader(request);
    request.setRawHeader("If-Match", "*");

    init();

    request.setUrl(getDeleteFileQuery(sourceUrl));

    reply = networkManager->deleteResource(request);

    connect(networkManager.data(), SIGNAL(finished(QNetworkReply*)),this, SLOT(slotReplyFinished(QNetworkReply*)));
    connectErrorHandlers();
}

void OperationsManager::copyWebFile(const ItemInfo::Data &source, const QString &destFolder)
{    
    currentOperation = ECopy;

    QString data = QString("{\"kind\": \"drive#file\", \"title\": \"%1\",\"parents\": [{\"id\":\"%2\"}]}").arg(source.name).arg(getIDFromURL(destFolder));

    postData = data.toLatin1();

    CommonTools::setHeader(request);
    request.setRawHeader("Content-Type", "application/json");

    postRequest(COPY_FILE_FIRST_QUERY_PART + getIDFromURL(source.self) + COPY_FILE_LAST_QUERY_PART);
}

void OperationsManager::moveWebFile(const ItemInfo::Data &source, const QString &destFolder)
{
    isMove = true;

    copyWebFile(source, destFolder);
    fileURLToDeleteForMoveOperation = source.self;
}

void OperationsManager::renameWebFile(const ItemInfo::Data &source, const QString &newName)
{
    currentOperation = ERename;

    QString data = QString("{\"title\": \"%1\"}").arg(newName);

    CommonTools::setHeader(request);
    request.setRawHeader("Content-Type", "application/json");

    init();

    request.setUrl(QUrl(QString("https://www.googleapis.com/drive/v2/files/") + getIDFromURL(source.self)));

    reply = networkManager->put(request, data.toLatin1());

    connect(reply, SIGNAL(finished()), this, SLOT(slotPutFinished()));
    connectErrorHandlers();
}

void OperationsManager::shareWebFile(const ItemInfo::Data &source)
{
    CommonTools::msg("Not Implemented yet");
//      ShareDialog *shareDialog = new ShareDialog(SDriveEngine::inst()->getParent());
//      shareDialog->show();
}

//void OperationsManager::setProgressBarSettings(const QString &fileName, const QString &progressBarDialogInfoText)
//{
//    Q_UNUSED(fileName);
//    Q_UNUSED(progressBarDialogInfoText);
//}

void OperationsManager::createFolder(const QString &folderUrl, const QString &name)
{
    currentOperation = ECreateFolder;

    QString data = QString("{\"title\": \"%1\",\"parents\": [{\"id\": \"%2\"}],\"mimeType\": \"application/vnd.google-apps.folder\"}").arg(name).arg(getIDFromURL(folderUrl));

    postData = data.toLatin1();

    CommonTools::setHeader(request);
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QByteArray::number(postData.size()));

    postRequest(QUrl("https://www.googleapis.com/drive/v2/files"));
}

QUrl OperationsManager::getDeleteFileQuery(const QString &url)
{
    return QUrl(QString(DELETE_FILE + getIDFromURL(url)));
}

QUrl OperationsManager::getCopyFileQuery(const QString &url)
{
    return QUrl(QString(COPY_FILE + getIDFromURL(url)));
}

QString OperationsManager::getIDFromURL(const QString &url)
{
    int backParamNum = 1;

    QStringList queryStrs(url.split("/"));

    if(queryStrs[queryStrs.count() - 1].contains(QRegExp("contents")))
    {
        backParamNum = 2;
    }

    QString lastParam(queryStrs[queryStrs.count() - backParamNum]);

    queryStrs = lastParam.split("%3A");

    return queryStrs[queryStrs.count()  - 1];
}

void OperationsManager::slotReplyFinished(QNetworkReply*)
{
    if(currentOperation == EDelete)
    {  
        updatePanelContent(false); 
    }
}

void OperationsManager::slotPostFinished(QNetworkReply* reply)
{
    NetworkManager::slotPostFinished(reply);

     if(currentOperation == ECopy)
     {
         updatePanelContent(true);

         if(isMove)
         {
             deleteFile(fileURLToDeleteForMoveOperation);
             isMove = false;
         }
     }

     if(currentOperation == ECreateFolder)
     {
        updatePanelContent(false);
     }
}

void OperationsManager::slotPutFinished(void)
{
    if(currentOperation == ERename)
    {
        updatePanelContent(false);
    }
}

void OperationsManager::updatePanelContent(bool opposite)
{
    FilesManager* fileManager = SDriveEngine::inst()->getFilesMngr(opposite);
    fileManager->get(fileManager->getUpperLevelFolderURL());
}



