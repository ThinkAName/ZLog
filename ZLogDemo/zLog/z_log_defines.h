#ifndef Z_LOG_DEFINES_H
#define Z_LOG_DEFINES_H

#include <QDateTime>

namespace ZLib {

/**
 * @brief The LDRSLogMoule enum
 *  日志所属模块，便于定位代码
 */
enum ZLogMoule
{
    LM_UNKNOWN = 0,                     // 未知
    LM_COUNT                            // 模块个数
};

/**
 * @brief The LDRSLogType enum
 *  日志类型，便于推断打印此日志的原因
 */
enum ZLogType
{
    // 类型，不要过细，因为还有描述语句
    LT_UNKNOWN = 0,                     // 未知
    LT_COUNT                            // 日志类型个数
};

/**
 * @brief The LDRSLogLevel enum
 *  日志等级
 *
 */
enum ZLogLevel
{
    LL_NO_LOG = -1,                     // 无日志
    LL_FATAL,                           // 致命信息日志
    LL_CRITICAL,                        // 严重信息日志
    LL_WARNING,                         // 警告信息日志
    LL_INFO,                            // 提示信息日志
    LL_DEBUG,                           // 调试信息日志
    LL_ALL_LOG                          // 所有日志
};

const QString &getLogModuleName(ZLogMoule module_id);
const QString &getTypeName(ZLogType type_id);
const QString &getLogLevelName(ZLogLevel level);


// 定位代码位置
#define Z_CODE_POS (QString("%1, %2, %3").arg(__FILE__).arg(__LINE__).arg(Q_FUNC_INFO))

// 当前时间
#define Z_CURRENT_TIME_STRING  (QDateTime::currentDateTime().toString(QStringLiteral("yyyy/MM/dd hh:mm:ss")))

}

#endif // Z_LOG_DEFINES_H
