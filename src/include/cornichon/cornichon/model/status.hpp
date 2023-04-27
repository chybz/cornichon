#pragma once

namespace cornichon::model {

enum class status
{
    untested,
    skipped,
    passed,
    failed,
    undefined,
    executing
};

}
