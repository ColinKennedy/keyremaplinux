#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <dirent.h> 

#include "keyremaplinux/remapper/remapper.h"
#include "keyremaplinux/remapper/kozikow_layout_remapper.h"
#include "device_remapping_daemon.h"
#include "util/logging.h"

namespace keyremaplinux {

  using namespace std;

  //static const string deviceRoot = "/dev/input/by-id";
  static const string deviceRoot = "/dev/input";

  //static const string keyboardSubstr = "event-kbd";
  static const string keyboardSubstr = "event";

  vector<string> FindKeyboardDevices() {
    vector<string> result;
    DIR *d;
    struct dirent *dir;
    d = opendir(deviceRoot.c_str());
    if (d) {
      while ((dir = readdir(d)) != NULL) {
        if (((string)dir->d_name).find(keyboardSubstr) != string::npos) {
          result.push_back(deviceRoot + "/" + dir->d_name);
        }
      }
      closedir(d);
    }
    return result;
  }

}  // end namespace keyremaplinux

int main(int argc, char* argv[]) {
  std::vector<std::string> devices = keyremaplinux::FindKeyboardDevices();
  if (devices.empty()) {
    LOG(WARNING) << "Did not find any input devices";
  }

  keyremaplinux::Remapper* remapper = new keyremaplinux::KozikowLayoutRemapper();
  std::vector<pthread_t> threads;
  for (auto device : devices) {
    keyremaplinux::DeviceRemappingDaemon* daemon =
        new keyremaplinux::DeviceRemappingDaemon(device, remapper);
    pthread_t thread = daemon->Run();
    threads.push_back(thread);
  }

  LOG(INFO) << "Waiting for threads";
  for (pthread_t thread : threads) {
    CHECK(!pthread_join(thread, nullptr));
  }
  return 0;
}
