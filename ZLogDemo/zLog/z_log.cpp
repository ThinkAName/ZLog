#include "z_log.h"
#include <QMutex>
#include <QThread>
#include <QDebug>

// 是否打印消息的线程ID
#define PRINT_THREAD_ID 0

using namespace ZLib;

ZLog::ZLog(QObject *parent)
    : QObject(parent),
      m_z_file_log("z_error.log"),
      m_mutex(QMutex::Recursive),
      m_log_output_level(LL_ALL_LOG)
{
}

ZLog::~ZLog()
{
    flush();
}

ZLog *ZLog::instance()
{
    static QMutex mutex;
    if (!m_instance)
    {
        QMutexLocker locker(&mutex);
        if (!m_instance)
        {
            m_instance = QSharedPointer<ZLog>(new ZLog);
        }
    }

    return m_instance.data();
}

void ZLog::destroyInstance()
{
    if (m_instance)
    {
        m_instance.clear();
    }
}

void ZLog::log(const ZLogMoule &module_id, const ZLogType &log_type,
              const ZLogLevel &log_level, const QString &msg_content,
              const QString &pos)
{
    // 1.生成消息
#if PRINT_THREAD_ID // 日志格式0
    Qt::HANDLE cur_thread_id = QThread::currentThreadId();
    QString final_string = QString("%1 ThreadID: %2, Module:%3, Type:%4, Level:%5, Pos:%6, Detail:\n%7")
            .arg(LDRS_CURRENT_TIME).arg((uint)cur_thread_id)
            .arg(getModuleName(module_id)).arg(getTypeName(type_id))
            .arg(getLogLevelName(log_level)).arg(pos).arg(msg_content);
#elif 0 // 日志格式1
    QString final_string = QString("%1 Module:%2, Type:%3, Level:%4, Pos:%5, Detail:\n%6")
            .arg(LDRS_CURRENT_TIME).arg(getModuleName(module_id))
            .arg(getTypeName(type_id)).arg(getLogLevelName(log_level))
            .arg(pos).arg(msg_content);
#elif 0 // 日志格式2
    QString final_string = QString("%1 LogLevel: %2, Detail: %3, Pos: %4")
            .arg(LDRS_CURRENT_TIME).arg(getLogLevelName(log_level))
            .arg(msg_content).arg(pos);
#elif 0 // 日志格式3
    QString final_string = QString("%1 LogLevel: %2, Detail: %3, Pos: %4")
            .arg(LDRS_CURRENT_TIME).arg(getLogLevelName(log_level))
            .arg(msg_content).arg(pos);
#endif

    QMutexLocker locker(&m_mutex);
    QThread *current_thread = QThread::currentThread();
    ThreadLogData &log_data = m_thread_msg_datas[current_thread];

    // 2.获取对应线程的数据
    if (!m_thread_msg_datas.contains(current_thread))
    {
        log_data.thread_id = QThread::currentThreadId();
        connect(current_thread, SIGNAL(finished()), SLOT(slotThreadFinished()));
    }

    // 3.追加日志
    LogMessage msg;
    msg.thread_id   = current_thread;
    msg.module_id   = module_id;
    msg.log_type    = log_type;
    msg.log_level   = log_level;
    msg.msg_content = msg_content;
    msg.pos         = pos;
    log_data.msgs << msg;

    // 4.输出
    if (!log_data.buffered) // 不缓冲
    {
        QList<LogMessage> tmp_list = log_data.msgs;
        log_data.msgs.clear();
        locker.unlock();

        // 输出
        output(tmp_list);
    }
}

void ZLog::setBuffered(bool buffered)
{
    // 1.获取对应线程的数据
    QThread *current_thread = QThread::currentThread();

    QMutexLocker locker(&m_mutex);
    ThreadLogData &log_data = m_thread_msg_datas[current_thread];
    log_data.buffered = buffered;
}

void ZLog::setLogOutputLevel(int level)
{
    m_log_output_level = level;
}

/**
 * @brief ZLog::flush
 *  不论当前是否是缓冲模式
 */
void ZLog::flush()
{
    // 1.获取对应线程的数据
    QThread *thread = QThread::currentThread();
    flushByThreadId(thread);
}

void ZLog::clearAll()
{
    QMutexLocker locker(&m_mutex);
    QMapIterator<QThread *, ThreadLogData> i(m_thread_msg_datas);
    while (i.hasNext())
    {
        i.next();
        clearByThreadId(i.key());
    }
}

void ZLog::flushAll()
{
    QMutexLocker locker(&m_mutex);
    QMapIterator<QThread *, ThreadLogData> i(m_thread_msg_datas);
    while (i.hasNext())
    {
        i.next();
        flushByThreadId(i.key());
    }
}

void ZLog::clearBufferedLog()
{
    // 1.获取对应线程的数据
    QThread *thread = QThread::currentThread();
    clearByThreadId(thread);
}

/**
 * @brief ZLog::output
 *  最终输出，可以输出到文件，可以输出到网络等
 * @param final_log_msg
 *  最终的日志字符串
 */
void ZLog::output(const LogMessage &log_msg)
{
    // 1.输出到日志文件 =====
    bool do_write_log_file = false;

    // 检查是否需要输出此日志
    if (m_log_output_level < 0)
    {
        return;
    }
    else if (m_log_output_level >= log_msg.log_level)
    {
        do_write_log_file = true;
    }

    QString file_log_str;
    switch (log_msg.log_level)
    {
    case LL_WARNING:
    case LL_CRITICAL:
    case LL_FATAL:
    {
        // 重要日志信息不需要打印出代码位置
        file_log_str = QString("%1 LogModule: %2, LogLevel: %3, Detail: %4, Pos: %5")
                .arg(Z_CURRENT_TIME_STRING).arg(getLogModuleName(log_msg.module_id))
                .arg(getLogLevelName(log_msg.log_level))
                .arg(log_msg.msg_content).arg(log_msg.pos);
        break;
    }
    default:
    {
        // 一般日志信息不需要打印出代码位置
        file_log_str = QString("%1 LogModule: %2, LogLevel: %3, Detail: %4")
                .arg(Z_CURRENT_TIME_STRING).arg(getLogModuleName(log_msg.module_id))
                .arg(getLogLevelName(log_msg.log_level))
                .arg(log_msg.msg_content);
        break;
    }
    }

    if (do_write_log_file)
    {
        m_z_file_log << file_log_str;
    }

    // 输出到其他路径 =====
}

void ZLog::output(const QList<LogMessage> &log_msgs)
{   
    foreach (const LogMessage &msg, log_msgs)
    {
        output(msg);
    }
}

void ZLog::clearByThreadId(QThread *thread)
{
    QMutexLocker locker(&m_mutex);
    ThreadLogData &log_data = m_thread_msg_datas[thread];
    log_data.msgs.clear();
}

void ZLog::flushByThreadId(QThread *thread)
{
    QMutexLocker locker(&m_mutex);
    ThreadLogData &log_data = m_thread_msg_datas[thread];

    QList<LogMessage> tmp_list = log_data.msgs;
    log_data.msgs.clear();
    locker.unlock();

    // 输出
    output(tmp_list);
}


/**
 * @brief ZLog::slotThreadFinished
 *  线程结束时移除相关数据
 */
void ZLog::slotThreadFinished()
{
    QThread *thread = qobject_cast<QThread *>(sender());
    Q_ASSERT(thread);
    m_thread_msg_datas.remove(thread);
}

QSharedPointer<ZLog> ZLog::m_instance;
