#include <cstdint>

#include <fstream>
#include <string>
#include <vector>

#include "endian.h"

template <class T>
class Sound
{
public:

  float sample_rate_;

  // we can do either float, double, etc.
  std::vector< std::vector<T> > data_;

  void ReadWav(const std::string& path)
  {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return;

    // so we can break out and still close the file
    for (;;)
    {
      // get the ChunkID, and check that it's RIFF
      int32_t chunk_id;
      GetBE32(file, &chunk_id);
      if (chunk_id != 0x52494646) // "RIFF"
        break;

      // get the ChunkSize
      int32_t chunk_size;
      GetLE32(file, &chunk_size);

      // get the Format, and check that it's WAVE
      int32_t format;
      GetBE32(file, &format);
      if (format != 0x57415645) // "WAVE"
        break;

      // get Subchunk1ID
      int32_t subchunk_1_id;
      GetBE32(file, &subchunk_1_id);
      if (subchunk_1_id != 0x666d7420) // "fmt "
        break;

      // get Subchunk1Size
      int32_t subchunk_1_size;
      GetLE32(file, &subchunk_1_size);

      // only allow PCM
      if (subchunk_1_size != 16)
        break;

      // get AudioFormat
      int16_t audio_format;
      GetLE16(file, &audio_format);

      // only allow uncompressed
      if (audio_format != 1)
        break;

      // get NumChannels
      int16_t num_channels;
      GetLE16(file, &num_channels);

      // get SampleRate
      int32_t sample_rate;
      GetLE32(file, &sample_rate);
      sample_rate_ = static_cast<float>(sample_rate);

      // get ByteRate
      int32_t byte_rate;
      GetLE32(file, &byte_rate);

      // get BlockAlign
      int16_t block_align;
      GetLE16(file, &block_align);

      // get BitsPerSample
      int16_t bits_per_sample;
      GetLE16(file, &bits_per_sample);

      // get Subchunk2ID
      int32_t subchunk_2_id;
      GetBE32(file, &subchunk_2_id);
      if (subchunk_2_id != 0x64617461) // "data_"
        break;

      // get Subchunk2Size
      int32_t subchunk_2_size;
      GetLE32(file, &subchunk_2_size);

      // get the number of samples
      int num_samples = subchunk_2_size * 8 / num_channels / bits_per_sample;

      // now we can read the actual data_!
      data_.clear();

      // prepare space
      data_.resize(num_channels);
      for (int i = 0; i < num_channels; ++i)
        data_[i].resize(num_samples);

      switch (bits_per_sample)
      {
      case 16:
        for (int i = 0; i < num_samples; ++i)
        {
          for (int j = 0; j < num_channels; ++j)
          {
            int16_t sample;
            GetLE16(file, &sample);
            data_[j][i] = static_cast<T>(sample) / INT16_MAX;
          }
        }
        break;
      case 8:
        break;
      default:
        break;
      }

      // success
      break;
    }

    file.close();
  }

  void WriteWav(const std::string& path)
  {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()) return;

    if (data_.size() <= 0) return;

    int32_t subchunk_1_size = 16; // PCM
    int16_t audio_format = 1; // uncompressed
    int16_t num_channels = data_.size();
    int32_t sample_rate = 44100; // hardcoded for now
    int32_t bits_per_sample = 16; // hardcoded for now
    int16_t block_align = (num_channels * bits_per_sample)>>3;
    int32_t byte_rate = sample_rate * block_align;
    int32_t num_samples = data_[0].size();
    int32_t subchunk_2_size = (num_samples * num_channels * bits_per_sample)>>3;
    int32_t chunk_size = 36 + subchunk_2_size;

    for (;;)
    {
      PutBE32(file, 0x52494646); // "RIFF"
      PutLE32(file, chunk_size);
      PutBE32(file, 0x57415645); // "WAVE"
      PutBE32(file, 0x666d7420); // "fmt "
      PutLE32(file, subchunk_1_size);
      PutLE16(file, audio_format);
      PutLE16(file, num_channels);
      PutLE32(file, sample_rate);
      PutLE32(file, byte_rate);
      PutLE16(file, block_align);
      PutLE16(file, bits_per_sample);
      PutBE32(file, 0x64617461); // "data_"
      PutLE32(file, subchunk_2_size);

      for (int j = 0; j < num_samples; ++j)
        for (int i = 0; i < num_channels; ++i)
          PutLE16(file, static_cast<int16_t>(data_[i][j] * (data_[i][j] > 0 ? INT16_MAX : INT16_MIN)));

      // success
      break;
    }

    file.close();
  }

};



