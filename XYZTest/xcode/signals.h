//
//  signals.h
//  XYZTest
//
//  Created by Laura Asher on 2/17/16.
//
//

#pragma once

#include "asio/asio.hpp"

namespace soso {
/// Connections to signals will be severed when this falls out of scope.
/// Allows us to pass back a connection to automatically manage subscription lifetime.
using ScopedConnectionRef = std::shared_ptr<ci::signals::ScopedConnection>;
using Signal = ci::signals::Signal<void()>;
using BufferSignal = ci::signals::Signal<void(std::string)>;
};