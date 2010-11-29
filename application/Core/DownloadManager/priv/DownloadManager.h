#ifndef DOWNLOADMANAGER_DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_DOWNLOADMANAGER_H

#include <QList>
#include <QSet>
#include <QSharedPointer>
#include <QTimer>

#include <Core/FileManager/IFileManager.h>
#include <Core/PeerManager/IPeerManager.h>

#include <IDownloadManager.h>
#include <priv/OccupiedPeers.h>

namespace PM
{
   class IPeer;
}

namespace DM
{
   class Download;
   class FileDownload;

   class DownloadManager : public QObject, public IDownloadManager
   {
      Q_OBJECT
   public:
      DownloadManager(QSharedPointer<FM::IFileManager> fileManager, QSharedPointer<PM::IPeerManager> peerManager);
      ~DownloadManager();

      void addDownload(const Protos::Common::Entry& entry, Common::Hash peerSource);
      void addDownload(const Protos::Common::Entry& entry, Common::Hash peerSource, bool complete);
      void addDownload(const Protos::Common::Entry& entry, Common::Hash peerSource, bool complete, QMutableListIterator<Download*> iterator);

      QList<IDownload*> getDownloads();
      void moveDownloads(quint64 downloadIDRef, const QList<quint64>& downloadIDs);
      QList< QSharedPointer<IChunkDownload> > getUnfinishedChunks(int n);

      int getDownloadRate() const;

   private slots:
      void fileCacheLoaded();

      void newEntries(const Protos::Common::Entries& entries);

      void downloadDeleted(Download* download);

      void peerNoLongerAskingForHashes(PM::IPeer* peer);
      void peerNoLongerDownloadingChunk(PM::IPeer* peer);

      void scanTheQueueToRetrieveEntries();
      void scanTheQueue();
      void chunkDownloadFinished();

   private:
      void loadQueueFromFile();
      void saveQueueToFile();
      bool isEntryAlreadyQueued(const Protos::Common::Entry& entry);

      const int NUMBER_OF_DOWNLOADER;

      QSharedPointer<FM::IFileManager> fileManager;
      QSharedPointer<PM::IPeerManager> peerManager;

      OccupiedPeers occupiedPeersAskingForHashes;
      OccupiedPeers occupiedPeersDownloadingChunk;

      QList<Download*> downloads;

      int numberOfDownload;
      QMutex mutexNumberOfDownload;

      bool retrievingEntries; // TODO : if the socket is closed then retrievingEntries = false

      QTimer timer; // When a download has an error status, the queue will be rescaned periodically.
   };
}
#endif
