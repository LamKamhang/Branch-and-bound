#pragma once

#include <cinttypes>

#define FE_MESSAGE_BUFFER_SIZE 1024

/**
 * Exception class with printf like text formatting
 * capabilities, using variable argument list.
 */
class FException {
private:
    char error_msg[FE_MESSAGE_BUFFER_SIZE];
    int64_t error_code;

protected:

public:
    FException(const char* error_msg, ...);
    FException(int64_t error_code, const char* error_msg, ...);

    void Print() const;
    int64_t getErrorCode() const;
    const char *getMessage() const;
};
