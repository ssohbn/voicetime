#include <FLAC++/metadata.h>
#include <cstdint>
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
  std::filesystem::path path(std::filesystem::current_path());

  double duration_seconds = 0;

  for (const auto &entry : std::filesystem::directory_iterator(path)) {


    // flac not found as extension
    if (entry.path().extension().string().find("flac") == -1) {
	    std::cout << "file is not a flac: " << entry.path().string() << std::endl;
	    continue;
    };

    std::cout << entry.path().string() << std::endl;

    FLAC::Metadata::StreamInfo streaminfo;
    FLAC::Metadata::get_streaminfo(entry.path().string().c_str(), streaminfo);

    std::cout << "looking at streaminfo" << std::endl;

    uint64_t samples = streaminfo.get_total_samples();
    uint32_t samplerate = streaminfo.get_sample_rate();
    duration_seconds +=
        static_cast<double>(samples) / static_cast<double>(samplerate);

    std::cout << "samples: " << samples << std::endl;
    std::cout << "samplerate: " << samplerate << std::endl;
  }

  std::cout << "total duration voice trained: " << duration_seconds << std::endl;

  return 0;
}
