#pragma once

/**
 * Base class for non-copyable objects.
 */
class NonCopyable
{
protected:
    NonCopyable() {}

private:
    NonCopyable(const NonCopyable &);
    NonCopyable& operator = (const NonCopyable &);
};
