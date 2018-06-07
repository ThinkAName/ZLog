#include "z_log_defines.h"

using namespace ZLib;

const QString ZLogMouleName[] =
{
    "UNKNOWN"
};

const QString &ZLib::getLogModuleName(ZLogMoule module_id)
{
    Q_ASSERT(module_id < LM_COUNT);
    if (module_id >= LM_COUNT)
    {
        module_id = LM_UNKNOWN;
    }

    return ZLogMouleName[module_id];
}

const QString ZLogTypeName[] =
{
    "UNKNOWN"
};

const QString &ZLib::getTypeName(ZLogType type_id)
{
    Q_ASSERT(type_id < LT_COUNT);
    if (type_id >= LT_COUNT)
    {
        type_id = LT_UNKNOWN;
    }

    return ZLogTypeName[type_id];
}

const QString ZLogLevelName[] =
{
    "FATAL",
    "CRITICAL",
    "WARNING",
    "INFO",
    "DEBUG",
    "ALL_LOG"
};

const QString &ZLib::getLogLevelName(ZLogLevel level)
{
    if (level < LL_NO_LOG)
    {
        level = LL_NO_LOG;
    }
    else if (level > LL_ALL_LOG)
    {
        level = LL_ALL_LOG;
    }

    return ZLogLevelName[level];
}
