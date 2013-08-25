if(fabs( /*target pos*/ - LocationStatus::Position ).Angle() > 10) {
    StrategyStatus::Goal1 = ( /*target pos*/ - LocationStatus::Position ).Angle() >> LocationStatus::Handle;
