#ifndef TJLUTILS_TRACEENTRY_H
#define TJLUTILS_TRACEENTRY_H

#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include <sstream>

enum class LogLevel {
    Verbose,
    Info,
    Warning,
    Error,
};

class TraceEntry
{
public:
    TraceEntry( , LogLevel logLevel);

private:
};

#endif //TJLUTILS_TRACEENTRY_H
