#include "z_file_log.h"
#include <QMessageBox>

ZFileLog::ZFileLog(const QString &file_path)
    : m_new_line_symbol("\n"),
      m_log_filepath(file_path),
      m_log_file(file_path)
{
    open();
    m_out_stream.setDevice(&m_log_file);
}

ZFileLog::~ZFileLog()
{
    close();
}

void ZFileLog::operator<<(const QString &msg)
{
    m_mutex.lock();
    m_out_stream << msg << m_new_line_symbol;
    m_mutex.unlock();
}

void ZFileLog::operator<<(const QStringList &msgs)
{
    QString msg = msgs.join(m_new_line_symbol).append(m_new_line_symbol);
    m_mutex.lock();
    m_out_stream << msg;
    m_mutex.unlock();
}

void ZFileLog::open()
{
    if (!m_log_file.open(QIODevice::Append))
    {
        QMessageBox::warning(
            NULL,
            QObject::tr("warning"),
            QObject::tr("Can't not open log file %1! Error Reason: %2.")
                    .arg(m_log_filepath)
                    .arg(m_log_file.errorString())
        );

        return;
    }
    m_out_stream.setDevice(&m_log_file);
}

void ZFileLog::close()
{
    m_out_stream.flush();
    m_log_file.close();
}


//ZFileLog z_file_log;
