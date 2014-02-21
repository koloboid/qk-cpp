#pragma once

#include "formatter.hpp"

namespace Qk {
namespace Core {

class Serializable
{
public:
    Serializable() { }
    virtual ~Serializable() { }

public:
    virtual void serialize(Formatter& pFmt) const = 0;
    virtual void deserialize(const Formatter& pFmt) = 0;
};

}
}
