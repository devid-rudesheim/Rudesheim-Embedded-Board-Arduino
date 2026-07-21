#pragma once

#include "Rudesheim-Embedded-Base.hpp"

#include <Arduino.h>

namespace Rudesheim::Embedded
{
  struct Arduino:
    virtual public Board
  {
    virtual auto DigitalSignal() const -> Option::Signal const & override;
    virtual auto Analog8BitSignal() const -> Option::Signal const & override;
    virtual auto Analog12BitSignal() const -> Option::Signal const & override;

    virtual auto DefaultMode() const -> Option::Mode const & override;
    virtual auto PullUpMode() const -> Option::Mode const & override;

    virtual auto SteadyOn() const -> Option::Steady const & override;
    virtual auto SteadyOff() const -> Option::Steady const & override;

    virtual auto Wait( Duration duration ) const -> void override
    {
      delayMicroseconds( duration.AsMicroseconds() );
    }

    virtual auto BeginCommunication( unsigned long baudRate ) const -> void override
    {
      Serial.begin( baudRate );
    }
  };
}
