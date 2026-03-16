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
#include <sys/utsname.h>
#include <unistd.h>

#define BOLD(x) "\e[1m" x "\e[0m"
#define RED(x) "\033[31m" << x << "\033[00m"
#define BLUE(x) "\033[34m" << x << "\033[00m"

// Get the host name using gethostname() func
void Hostname() {
  char hostname[HOST_NAME_MAX + 1];
  bool host_check = gethostname(hostname, sizeof(hostname));

  if (host_check == 0) {
    std::cout << BLUE(hostname) << "\n";
  } else {
    std::cout << "localhost";
  }
}

// /run/udev/data/ for runit(void)
// gets the model of the sys
// well seems like the file exist only for runit
// TODO: need a if statement for systemd
void Host() {
  std::string Host_info;
  std::ifstream File("/run/udev/data/+dmi:id");

  while (getline(File, Host_info)) {
    if (Host_info.find("E:ID_MODEL") == 0) {
      std::string model = Host_info.substr(11);
      std::cout << BOLD("Host: ") << model << "\n";
    }
  }
  File.close();
}

// Well that was simple to get username
void Username() {
  uid_t uid = getuid();

  struct passwd *pw = getpwuid(uid);
  std::cout << BOLD("User: ") << pw->pw_name << "\n";
}

// which distro you use only work if you use linux
// This should work for all the sys so need to change anything
void Osname() {
  std::string Os_name;
  std::ifstream Os_file("/etc/os-release");

  while (getline(Os_file, Os_name)) {
    if (Os_name.find("PRETTY") == 0) {
      Os_name.erase(Os_name.length() - 1);
      std::string distro = Os_name.substr(13);
      std::cout << BOLD("Os: ") << distro + "\n";
    }
  }
  Os_file.close();
}

// TODO: Kernel need some setup
// only needed the kernel version that is like in third word
void Kernel_version() {
  std::ifstream Kernel_file("/proc/version");

  std::string version, temp1, temp2;
  Kernel_file >> temp1 >> temp2 >> version;
  std::cout << BOLD("Kernel: ") << version << "\n";
  Kernel_file.close();
}

// https : // linuxvox.com/blog/what-api-do-i-call-to-get-the-system-uptime/
void Uptime() {
  struct sysinfo info;
  if (sysinfo(&info) == -1) {
    std::cout << "duck";
  }
  long uptime_sec = info.uptime;

  long days = uptime_sec / 86400;
  long hours = (uptime_sec % 86400) / 3600;
  long minutes = (uptime_sec % 3600) / 60;

  std::cout << BOLD("Uptime: ") << days << " days," << hours << " hrs,"
            << minutes << " mins";
}

void Shell() {
  char *shell_name = getenv("SHELL");
  char *base = strrchr(shell_name, '/');
  std::cout << BOLD("Shell: ") << base + 1 << "\n";
}

// TODO: do it later
void Disk() {
  struct statvfs fiData;

  if (statvfs("/", &fiData) == 0) {
    int total = fiData.f_bsize * fiData.f_blocks;
    int free = fiData.f_bsize * fiData.f_bavail;
    int used = total - free;
    std::cout << used / (1024 * 1024);
    std::cout << (fiData.f_bavail * fiData.f_bsize);
  }
  // std::cout << fiData.f_bavail / (1024 * 1024);
}

void Memory() {
  // std::string MemoryInfo;
  std::ifstream MemoryFile("/proc/meminfo");
  std::string Key, Unit;
  long Value;
  long Total = 0, Available = 0;

  while (MemoryFile >> Key >> Value >> Unit) {
    if (Key == "MemTotal:") {
      Total = Value / 1024;
    } else if (Key == "MemAvailable:") {
      Available = Value / 1024;
    }
  }

  long Used = Total - Available;

  std::cout << BOLD("Memory : ") << Used << " Mib / " << Total << " Mib \n";
}

void Cpu() {
  std::string CpuInfo;
  std::ifstream CpuFile("/proc/cpuinfo");

  while (getline(CpuFile, CpuInfo)) {
    if (CpuInfo.find("model name") == 0) {
      std::string CpuName = CpuInfo.substr(12);
      std::cout << BOLD("Cpu :") << CpuName << "\n";
      break;
    }
  }
  CpuFile.close();
}

void Title() {
  uid_t uid = getuid();

  struct passwd *pw = getpwuid(uid);
  std::string User = pw->pw_name;
  std::cout << BLUE(User);
  std::cout << "@";
  Hostname();
}

void Seperator() { std::cout << "-----------------\n"; }

int main() {
  Title();
  Seperator();
  Osname();
  Host();
  Username();
  Shell();
  Kernel_version();
  Memory();
  // Disk();
  Cpu();
  Uptime();
  std::cout << "\n";
  return 0;
}
