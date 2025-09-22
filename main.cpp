#include <FLAC++/metadata.h>
#include <cstdint>
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
  std::filesystem::path path("./");

  double duration_seconds = 0;

  for (const auto &entry : std::filesystem::directory_iterator(path)) {

    // flac not found as stem
    if (entry.path().stem().string().find("flac") == -1)
      continue;

    FLAC::Metadata::StreamInfo streaminfo;
    FLAC::Metadata::get_streaminfo(entry.path().string().c_str(), streaminfo);
    uint64_t samples = streaminfo.get_total_samples();
    uint32_t samplerate = streaminfo.get_sample_rate();
    duration_seconds +=
        static_cast<double>(samples) / static_cast<double>(samplerate);
  }

  std::cout << "total duration voice trained: " << duration_seconds << std::endl;

  return 0;
}
