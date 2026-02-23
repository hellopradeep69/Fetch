#include <climits>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <linux/sysinfo.h>
#include <pwd.h>
#include <stdlib.h>
#include <string>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Get the host name using gethostname() func
void Hostname() {
  char hostname[HOST_NAME_MAX + 1];
  bool host_check = gethostname(hostname, sizeof(hostname));

  if (host_check == 0) {
    cout << hostname;
  } else {
    cout << "localhost";
  }
}

// /run/udev/data/ for runit(void)
// gets the model of the sys
// well seems like the file exist only for runit
// TODO: need a if statement for systemd
void Host() {
  string Host_info;
  ifstream File("/run/udev/data/+dmi:id");

  while (getline(File, Host_info)) {
    if (Host_info.find("E:ID_MODEL") == 0) {
      string model = Host_info.substr(11);
      cout << model << "\n";
    }
  }
  File.close();
}

// Well that was simple to get username
void Username() {
  uid_t uid = getuid();

  struct passwd *pw = getpwuid(uid);
  cout << pw->pw_name;
}

// which distro you use only work if you use linux
// This should work for all the sys so need to change anything
void Osname() {
  string Os_name;
  ifstream Os_file("/etc/os-release");

  while (getline(Os_file, Os_name)) {
    if (Os_name.find("PRETTY") == 0) {
      Os_name.erase(Os_name.length() - 1);
      string distro = Os_name.substr(13);
      cout << distro + "\n";
    }
  }
  Os_file.close();
}

// TODO: Kernel need some setup
// only needed the kernel version that is like in third word
void Kernel_version() {
  string Kernel_data;
  ifstream Kernel_file("/proc/version");

  while (getline(Kernel_file, Kernel_data)) {
    if (Kernel_data.find("Linux") == 0) {
      string version = Kernel_data.substr(14);
      cout << version;
    }
  }
}

// https : // linuxvox.com/blog/what-api-do-i-call-to-get-the-system-uptime/
void Uptime() {
  struct sysinfo info;
  if (sysinfo(&info) == -1) {
    cout << "duck";
  }
  long uptime_sec = info.uptime;

  long days = uptime_sec / 86400;
  long hours = (uptime_sec % 86400) / 3600;
  long minutes = (uptime_sec % 3600) / 60;

  cout << days << " days," << hours << " hrs," << minutes << " mins";
}

int main() {
  // Host();
  // Username();
  // Hostname();
  // Osname();
  // Kernel_version();
  Uptime();
  return 0;
}
