/*************************************************
* 实现打印到文件的日志
*************************************************/

#ifndef ZERRORFILELOG_H
#define ZERRORFILELOG_H

#include <QDateTime>
#include <QFile>
#include <QDebug>
#include <QMutex>

class ZFileLog
{
public:
    ZFileLog(const QString &file_path);
    ~ZFileLog();

    void operator<<(const QString &msg);
    void operator<<(const QStringList &msgs);

private:
    void open();            // 打开日志文件
    void close();           // 关闭日志文件

    const QString m_new_line_symbol;
    const QString m_log_filepath;
    QFile  m_log_file;
    QTextStream m_out_stream;
    QMutex m_mutex;
};

#endif // ZERRORFILELOG_H
