#include "include/miniaudio.h"
#include <FLAC++/metadata.h>
#include <cstdint>
#include <filesystem>
#include <iostream>

double timeElapsed(std::filesystem::path directoryPath) {

  double duration_seconds = 0;

  for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {

    // flac not found as extension
    if (entry.path().extension().string().find("flac") == -1) {
      continue;
    };

    FLAC::Metadata::StreamInfo streaminfo;
    FLAC::Metadata::get_streaminfo(entry.path().string().c_str(), streaminfo);

    uint64_t samples = streaminfo.get_total_samples();
    uint32_t samplerate = streaminfo.get_sample_rate();
    duration_seconds +=
        static_cast<double>(samples) / static_cast<double>(samplerate);
  }

  return duration_seconds;
};

void data_callback(ma_device *pDevice, void *pOutput, const void *pInput,
                   ma_uint32 frameCount) {
  ma_encoder_write_pcm_frames((ma_encoder *)pDevice->pUserData, pInput,
                              frameCount, NULL);

  (void)pOutput;
}

int main(int argc, char *argv[]) {
  std::filesystem::path path(std::filesystem::current_path());

  double elapsed = timeElapsed(path);

  std::cout << "total duration voice trained: " << elapsed << std::endl;

  ma_result result;
  ma_encoder_config encoderConfig;
  ma_encoder encoder;
  ma_device_config deviceConfig;
  ma_device device;

  encoderConfig =
      ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 44100);

  auto outputfile = std::filesystem::path("./output.wav");
  if (ma_encoder_init_file(outputfile.string().c_str(), &encoderConfig,
                           &encoder) != MA_SUCCESS) {
    printf("Failed to initialize output file.\n");
    return -1;
  }
  deviceConfig = ma_device_config_init(ma_device_type_capture);
  deviceConfig.capture.format = encoder.config.format;
  deviceConfig.capture.channels = encoder.config.channels;
  deviceConfig.sampleRate = encoder.config.sampleRate;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &encoder;

  result = ma_device_init(NULL, &deviceConfig, &device);
  if (result != MA_SUCCESS) {
    printf("Failed to initialize capture device.\n");
    return -2;
  }

  result = ma_device_start(&device);
  if (result != MA_SUCCESS) {
    ma_device_uninit(&device);
    printf("Failed to start device.\n");
    return -3;
  }

  printf("Press Enter to stop recording...\n");
  getchar();

  ma_device_uninit(&device);
  ma_encoder_uninit(&encoder);

  return 0;
}
