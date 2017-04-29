#include "stringformat.h"

/*Base case to break recursion*/
std::string TStringFormat(const char *formatting)
{
    if (!formatting) {
        return "";
    }
    return std::string{formatting};
}

/*Overload base case for formatting*/
std::string TStringFormat(const std::string &format)
{
    return format;
}

/*Overload base case for formatting*/
std::string TStringFormat(std::string format)
{
    return format;
}

std::string toStdString(const char *rhs)
{
    if (!rhs) {
        return "";
    }
    return std::string{rhs};
}

std::string toStdString(const std::string &rhs)
{
    return rhs;
}

std::string toStdString(char *rhs)
{
    if (!rhs) {
        return "";
    }
    return std::string{rhs};
}

std::string toStdString(char rhs)
{
    return std::string{1, rhs};
}

#if defined(QT_CORE_LIB)

std::string toStdString(const QString &str)
{
    return str.toStdString();
}

/*Overload base case for formatting*/
std::string TStringFormat(const QString &format)
{
    return format.toStdString();
}


/*Overload base case for formatting*/
std::string TStringFormat(QString format)
{
    return format.toStdString();
}

/*Overload base case for formatting*/
QString QStringFormat(const QString &format)
{
    return format;
}

/*Overload base case for formatting*/
QString QStringFormat(QString format)
{
    return format;
}

/*Overload base case for formatting*/
QString QStringFormat(const std::string &format)
{
    return QString::fromStdString(format);
}

/*Overload base case for formatting*/
QString QStringFormat(std::string format)
{
    return QString::fromStdString(format);
}

/*Overload base case for formatting*/
QString QStringFormat(const char *format)
{
    if (!format) {
        return "";
    }
    return format;
}

QString toQString(const char *rhs)
{
    if (!rhs) {
        return "";
    }
    return QString{rhs};
}

QString toQString(const std::string &rhs)
{
    return QString::fromStdString(rhs);
}

QString toQString(char *rhs)
{
    if (!rhs) {
        return "";
    }
    return QString{rhs};
}

QString toQString(char rhs)
{
    return QString{1, rhs};
}

#endif //QT_CORE_LIB
