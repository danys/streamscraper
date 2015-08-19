#ifndef FILENAMESELECTOR_H
#define FILENAMESELECTOR_H

#include <QWidget>

class FilenameSelector
{
public:
    FilenameSelector();
    ~FilenameSelector();
    // selects a new filename
    void NewFilename(QString& name);
    // checks if file with name str exists and returns a filename which does not yet exist
    void CheckFilename(QString& str, bool video);
    void getArtistandTitle(QString filename,QString &artist,QString &title);
};

#endif // FILENAMESELECTOR_H
