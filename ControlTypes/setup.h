#ifndef SETUP_H
#define SETUP_H

#include <QString>
#include <QMap>
#include <functional>

class Setup
{
private:
public:
    enum CB {
        LOG ,
        DESCRIPTION,
    };

    // Set of callback connectors
    QMap< CB , std::function<void(QString)> > _connectors;

    QString fileName;
    QString _description;
    Setup(QMap< CB , std::function<void(QString)> > cbmap ) : _connectors(cbmap) {}
    enum {SimpleCurve,BistateReg,TristateReg,TempCheck,ExternCurve} SetType;
    //Setup();
    virtual ~Setup();
    virtual double returnTemp(int position/*In seconds*/,double tempValue/*Not needed if temp is not constant*/) =0;
    virtual QString getDescription() { return _description; }
    virtual void changeFileName(QString Name) {
        fileName=Name;
    }
    virtual void processFile() {
        if ( _connectors.contains(LOG) ) _connectors[LOG]("Dummy processFile used");
    };
};

#endif // SETUP_H
