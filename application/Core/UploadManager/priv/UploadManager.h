#ifndef UPLOADMANAGER_UPLOADMANAGER_H
#define UPLOADMANAGER_UPLOADMANAGER_H

#include <IUploadManager.h>

namespace UM
{
   class Uploader;
   class Upload;
   class UploadManager : public IUploadManager
   {
   private:
      QList<Uploader*> uploader;
      QList<Upload*> upload;
   };
}
#endif
