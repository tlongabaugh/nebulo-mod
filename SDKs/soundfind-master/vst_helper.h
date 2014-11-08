#ifndef VST_HELPER_H
#define VST_HELPER_H

// for HMODULE, LoadLibrary
#include <Windows.h>

// For AEffectX
#define   WIN32 // to import the right types
#include "pluginterfaces/vst2.x/aeffectx.h"
#include "pluginterfaces/vst2.x/vstfxstore.h"

#include <vector>
#include <string>

class VSTHelper
{
public:

  void GenerateOutput(float sample_rate, VstInt32 block_size, float* output[2]);
  
  void LoadProgram(const std::string& path);
  void SaveCurrentProgram(const std::string& path);
  
  //unsigned int GetNumParams() { return params_.size(); }
  
  int   GetNumParams() { return effect_->numParams; }
  void  SetParam(unsigned int index, float value);
  float GetParam(unsigned int index);

  void Init(const std::string& vst_path);
  void Deinit();

private:

  HMODULE  module_;
  AEffect* effect_;

  //typedef struct
  //{
  //  std::string name_;
  //  std::string label_;
  //  std::string display_;
  //  float value;
  //} Param;

  //std::vector<Param> params_;

};

#endif