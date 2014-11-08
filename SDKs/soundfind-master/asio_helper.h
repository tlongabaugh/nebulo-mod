#ifndef ASIO_HELPER_H
#define ASIO_HELPER_H

#include "asiodrivers.h" // for getDriverNames, loadDriver, etc.
#include "asio.h"        // for all the ASIO types, etc.

typedef void (*AudioCallback)(int index);

class ASIOHelper
{
public:

  void Init(AudioCallback callback);
  void DeInit();

  // these are accessible because audio callbacks need them
  ASIOBufferInfo*  buffer_infos_;
  ASIOChannelInfo* channel_infos_;
  long             input_channels_;
  long             output_channels_;
  long             buffer_size_;
  ASIOSampleRate   sample_rate_;

private:

  static const int kMaxAsioDriverLen     = 8;
  static const int kMaxAsioDriverNameLen = MAXDRVNAMELEN;
  
  // to remove at the end...
  AsioDrivers asio_drivers_;

  // internal callbacks
  ASIOCallbacks callbacks_;
  static ASIOTime* BufferSwitchTimeInfo(ASIOTime* time_info, long index, ASIOBool process_now);
  static void BufferSwitch(long index, ASIOBool process_now);
  static void SampleRateDidChange(ASIOSampleRate sRate);
  static long AsioMessage(long selector, long value, void* message, double* opt);

  // external callback
  static AudioCallback external_callback_;
};

#endif