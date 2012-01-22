#ifndef QTOMATOCONFIG_H
#define QTOMATOCONFIG_H

class QTomatoConfig
{
public:
  QTomatoConfig();
  void operator=( const QTomatoConfig &pConfig );

  int mPomodoroLength;
  int mShortBreakLength;
  int mLongBreakLength;
  int mLongBreakInterval;
};

#endif // QTOMATOCONFIG_H
