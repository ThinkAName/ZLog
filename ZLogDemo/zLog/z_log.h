#ifndef ZLOG_H
#define ZLOG_H

/******************************************************
** 具有线程识别、线程独立、日志缓冲功能
** 多线程安全
** 可简单修改后复用
******************************************************/

#include "z_log_defines.h"
#include "z-file-log/z_file_log.h"
#include <QSharedPointer>

using namespace ZLib;

class ZLog : public QObject
{
    Q_OBJECT

public:
    ~ZLog();
    static ZLog *instance();
    static void destroyInstance();

    // 添加一条日志
    void log(const ZLogMoule &, const ZLogType &log_type, const ZLogLevel &,
             const QString &detail, const QString &pos);

    // 设置日志的缓冲性，默认为非缓冲
    void setBuffered(bool);

    // 设置日志输出等级，低于某等级不输出
    void setLogOutputLevel(int level);

    // 线程相关
    void clearBufferedLog();                                                    // 清除已缓冲的日志（不输出）
    void flush();                                                               // 输出已缓冲的日志

    // 线程无关
    void clearAll();
    void flushAll();

private:
    // 一条日志的消息内容
    struct LogMessage
    {
        Qt::HANDLE   thread_id;
        ZLogMoule    module_id;
        ZLogType     log_type;
        ZLogLevel    log_level;
        QString      msg_content;
        QString      pos;

        LogMessage()
        {
            thread_id = 0;
            module_id = LM_UNKNOWN;
            log_level = LL_DEBUG;
            log_type  = LT_UNKNOWN;
        }
    };

    // 线程对应的日志数据
    struct ThreadLogData
    {
        Qt::HANDLE          thread_id;          // 线程ID
        bool                buffered;           // 是否缓冲日志到内存中
        QList<LogMessage>   msgs;               // 消息缓冲区

        ThreadLogData()
        {
            buffered = false;
        }
    };

    ZLog(QObject *parent = NULL);

    // 使用共享指针，防止忘记调用destroyInsance()导致不析构
    static QSharedPointer<ZLog> m_instance;

    void output(const LogMessage &log_msg);
    void output(const QList<LogMessage> &log_msgs);

    void clearByThreadId(QThread *thread);
    void flushByThreadId(QThread *);

private:
    QMap<QThread *, ThreadLogData> m_thread_msg_datas;
    ZFileLog m_z_file_log;
    QMutex m_mutex;
    int m_log_output_level;

private slots:
    void slotThreadFinished();
};

// 简写宏
#define ZLogger(a,b,c,d) (ZLog::instance()->log((a),(b),(c),(d),(Z_CODE_POS)))

#endif // ZLOG_H
