#include "Rudesheim-Embedded-Board-Arduino.hpp"

#include <stdexcept>
#include <Arduino.h>

namespace Rudesheim::Embedded
{
  namespace Option
  {
    namespace
    {
      struct Digital:
        virtual public Signal
      {
        virtual auto Write( Location const &location, float value ) const -> void override
        {
          digitalWrite( location.Number(), static_cast< float >( 0.5f <= value ) );
        }

        virtual auto Read( Location const &location ) const -> float override
        {
          return static_cast< float >( digitalRead( location.Number() ) );
        }

        virtual auto Resolution() const -> uint8_t override
        {
          return 1;
        }
      };

      struct Analog:
        virtual public Signal
      {
        virtual auto Write( Location const &location, float value ) const -> void override
        {
          analogWrite( location.Number(), static_cast< uint8_t >( value * MaxValue() + 0.5f ) );
        }

        virtual auto Read( Location const &location ) const -> float override
        {
          return static_cast< float >( analogRead( location.Number() ) ) / MaxValue();
        }
      };

      struct Analog8Bit:
        virtual public Analog
      {
        virtual auto Resolution() const -> uint8_t override
        {
          return 8;
        }
      };

      struct Analog12Bit:
        virtual public Analog
      {
        virtual auto Resolution() const -> uint8_t override
        {
          return 12;
        }
      };

      struct Default:
        virtual public Mode
      {
        virtual auto ConfigureInput( Location const &location ) const -> void override
        {
          pinMode( location.Number(), INPUT );
        }

        virtual auto ConfigureOutput( Location const &location ) const -> void override
        {
          pinMode( location.Number(), OUTPUT );
        }
      };

      struct PullUp:
        virtual public Mode
      {
        virtual auto ConfigureInput( Location const &location ) const -> void override
        {
          pinMode( location.Number(), INPUT_PULLUP );
        }

        virtual auto ConfigureOutput( Location const & ) const -> void override
        {
          throw std::logic_error( "PullUp is not valid for Output pins" );
        }
      };

      struct On:
        virtual public Steady
      {
        virtual auto Name() const -> char const* override
        {
          return "ON";
        }

        virtual auto Level() const -> float override
        {
          return 1.0f;
        }
      };

      struct Off:
        virtual public Steady
      {
        virtual auto Name() const -> char const* override
        {
          return "OFF";
        }

        virtual auto Level() const -> float override
        {
          return 0.0f;
        }
      };
    }
  }

  auto Arduino::DigitalSignal() const -> Option::Signal const &
  {
    return Class< Option::Digital >::SoleObject();
  }

  auto Arduino::Analog8BitSignal() const -> Option::Signal const &
  {
    return Class< Option::Analog8Bit >::SoleObject();
  }

  auto Arduino::Analog12BitSignal() const -> Option::Signal const &
  {
    return Class< Option::Analog12Bit >::SoleObject();
  }

  auto Arduino::DefaultMode() const -> Option::Mode const &
  {
    return Class< Option::Default >::SoleObject();
  }

  auto Arduino::PullUpMode() const -> Option::Mode const &
  {
    return Class< Option::PullUp >::SoleObject();
  }

  auto Arduino::SteadyOn() const -> Option::Steady const &
  {
    return Class< Option::On >::SoleObject();
  }

  auto Arduino::SteadyOff() const -> Option::Steady const &
  {
    return Class< Option::Off >::SoleObject();
  }

  auto Arduino::Wait( Duration const &duration ) const -> void
  {
    delayMicroseconds( static_cast< unsigned long >( duration.AsSecond().AsMicro() ) );
  }

  auto Arduino::BeginCommunication( unsigned long baudRate ) const -> void
  {
    Serial.begin( baudRate );
  }
}

auto setup() -> void
{
  auto
    &device = Rudesheim::Embedded::EntryPoint();

  Rudesheim::Embedded::board.BeginCommunication( device.BaudRate() );

  Rudesheim::Embedded::board.WaitSetupSerialPort();
}

auto loop() -> void
{
  Rudesheim::Embedded::EntryPoint().Step();
}
