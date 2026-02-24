#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <linux/sysinfo.h>
#include <pwd.h>
#include <stdlib.h>
#include <string>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

// Get the host name using gethostname() func
void Hostname() {
  char hostname[HOST_NAME_MAX + 1];
  bool host_check = gethostname(hostname, sizeof(hostname));

  if (host_check == 0) {
    cout << hostname << "\n";
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
      cout << "Host:" << model << "\n";
    }
  }
  File.close();
}

// Well that was simple to get username
void Username() {
  uid_t uid = getuid();

  struct passwd *pw = getpwuid(uid);
  cout << "User:" << pw->pw_name << "\n";
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
      cout << "Os: " << distro + "\n";
    }
  }
  Os_file.close();
}

// TODO: Kernel need some setup
// only needed the kernel version that is like in third word
void Kernel_version() {
  ifstream Kernel_file("/proc/version");

  string version, temp1, temp2;
  Kernel_file >> temp1 >> temp2 >> version;
  cout << "Kernel:" << version << "\n";
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

  cout << "Uptime:" << days << " days," << hours << " hrs," << minutes
       << " mins";
}

void Shell() {
  char *shell_name = getenv("SHELL");
  char *base = strrchr(shell_name, '/');
  cout << "Shell:" << base + 1 << "\n";
}

// TODO: do it later
void Disk() {
  struct statvfs fiData;

  if (statvfs("/", &fiData) == 0) {
    int total = fiData.f_bsize * fiData.f_blocks;
    int free = fiData.f_bsize * fiData.f_bavail;
    int used = total - free;
    cout << used / (1024 * 1024);
  }
  // cout << fiData.f_bavail / (1024 * 1024);
}

void Title() {
  uid_t uid = getuid();

  struct passwd *pw = getpwuid(uid);
  cout << pw->pw_name;
  cout << "@";
  Hostname();
}

int main() {
  Title();
  Osname();
  Host();
  Username();
  Shell();
  Kernel_version();
  Uptime();
  cout << "\n";
  return 0;
}
