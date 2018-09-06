#include <QCoreApplication>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLibrary>

struct ST_VIDEO_REORD
{
    QString qstrStartTime;
    QString qstrStopTime;
    QString qstrURL;
};

static QSqlDatabase db;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    //动态加载oci动态库
    QLibrary *oracle_lib = NULL;
    //oracle_lib = new QLibrary(qApp->applicationDirPath()+"/oracle_lib/instantclient_11_1/oci.dll");
    QString LibPath = "D:/01-Code/test_ocidriver_qt4.8.4_ora11G1/oracle_lib/instantclient_11_1/oci.dll";
    QFileInfo file(LibPath);
    if(file.exists()){
        oracle_lib = new QLibrary(LibPath);
    }
    oracle_lib->load();
    if (!oracle_lib->isLoaded())
    {
        qDebug() << "load Oracle failed!\n";
        return -1;
    }
    QList<ST_VIDEO_REORD> listVideoRecord;
    listVideoRecord.clear();

    QString qstrStartTime("2019-05-05 12:12:15");
    QString qstrStopTime("2019-06-18 12:12:15");
    QString qstrIP("192.169.1.170");
    QString qstrQuery;
//    qstrQuery   =   QString("select to_char(start_time,'yyyy-mm-dd hh24:mi:ss') as start_time,to_char(stop_time,'yyyy-mm-dd hh24:mi:ss') as stop_time,url from vts_video_record \
//                      where ip = '%1'and \
//                      (start_time<= to_date('%2','yyyy-mm-dd hh24:mi:ss') and stop_time >= to_date('%3','yyyy-mm-dd hh24:mi:ss') ) or \
//                      (start_time>= to_date('%4','yyyy-mm-dd hh24:mi:ss') and stop_time <= to_date('%5','yyyy-mm-dd hh24:mi:ss') ) or \
//                      (stop_time>= to_date('%6','yyyy-mm-dd hh24:mi:ss') and start_time <= to_date('%7','yyyy-mm-dd hh24:mi:ss') )    \
//                      order by start_time").arg(qstrIP).arg(qstrStartTime).arg(qstrStartTime).arg(qstrStartTime).arg(qstrStopTime).arg(qstrStopTime).arg(qstrStopTime);

    qstrQuery = QString("select * from vts_ais");

    /*QSqlDatabase */db = QSqlDatabase::addDatabase("QOCI");
    db.setHostName("192.169.1.15");
    db.setPort(1521);
    db.setDatabaseName("orcl");
    db.setUserName("bwdba");
    db.setPassword("123456");

    bool ok = db.open();
    if(ok == true)
    {
        qDebug()<<"open database ok";

        QSqlQuery sqlQuery  =    db.exec(qstrQuery);

        if(!db.lastError().isValid())
        {
            qDebug()<<"query success";
            qDebug()<<sqlQuery.isActive();
            qDebug()<<sqlQuery.size();
            while (sqlQuery.next())
            {
                //qDebug() << sqlQuery.value(0).toString()<<";"<<sqlQuery.value(1).toString()<<";"<<sqlQuery.value(2).toString();
                ST_VIDEO_REORD stVideoRecord;
                stVideoRecord.qstrStartTime =   sqlQuery.value(0).toString();
                stVideoRecord.qstrStopTime  =   sqlQuery.value(1).toString();
                stVideoRecord.qstrURL   =   sqlQuery.value(2).toString();
                listVideoRecord.append(stVideoRecord);
            }


        }
        else
        {
            qDebug()<<db.lastError().text()<<" "<<"query failed";
        }
    }
    else
    {
        qDebug()<<db.lastError().text()<<" "<<"open database failed";
    }

    return a.exec();
}
