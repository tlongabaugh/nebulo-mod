//int main()
//{
//  float sample_rate = 44100;
//  int block_size    = 512;
//
//  HMODULE  module = LoadLibrary("Synth1 VST.dll");
//  //HMODULE  module = LoadLibrary("Rez v3.0 - Yellow.dll");
//  Plug     addr   = (Plug)GetProcAddress(module, "main");
//  AEffect* eff    = addr(MyHostCallback);
//
//  float* buffer[2];
//  buffer[0] = new float[block_size];
//  buffer[1] = new float[block_size];
//
//  eff->dispatcher(eff, effOpen, 0, 0, 0, 0);
//
//  for (;;)
//  {
//    eff->dispatcher(eff, effSetSampleRate, 0, 0, 0, sample_rate);
//    eff->dispatcher(eff, effSetBlockSize, 0, block_size, 0, 0);
//    eff->dispatcher(eff, effMainsChanged, 0, 1, 0, 0);
//
//    memset(buffer[0], 0, sizeof(float) * block_size);
//    memset(buffer[1], 0, sizeof(float) * block_size);
//    eff->processReplacing(eff, buffer, buffer, block_size);
//
//    eff->dispatcher(eff, effMainsChanged, 0, 0, 0, 0);
//  }
//
//  eff->dispatcher(eff, effClose, 0, 0, 0, 0);
//
//  delete[] buffer[0];
//  delete[] buffer[1];
//}