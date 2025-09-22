#include "miniaudio.h"
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

int main(int argc, char *argv[]) {
  std::filesystem::path path(std::filesystem::current_path());

  double elapsed = timeElapsed(path);

  std::cout << "total duration voice trained: " << elapsed << std::endl;

  return 0;
}
