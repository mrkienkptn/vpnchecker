#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "ipvpn.h"

using namespace std;

bool compareIpRange(const IPRange &range1, const IPRange &range2) {
  if (range2.start > range1.start) return true;
  return false;
}

IpVPN::IpVPN(string path)
{
  dataPath = path;
}

unsigned int IpVPN::ipv4ToNumber(string ip)
{

  unsigned int rs = 0;
  short part;
  short leftPart = 4;
  istringstream ipstream(ip);
  while (ipstream >> part)
  {
    rs |= (part << (--leftPart * 8));
    if (ipstream.peek() == '.')
    {
      ipstream.ignore();
    }
  }
  return rs;
}

void IpVPN::initArray()
{
  cout << "init array" << endl;
  ifstream file(dataPath);
  if (!file.is_open())
  {
    cerr << "Load data failed\n";
    return;
  }
  string line;
  int count = 0;
  while (getline(file, line))
  {
    count++;
  }
  cout << "count" << count;
  totalIp = count;
  ips = new IPRange[count];
  file.close();
}

void IpVPN::preProcessData()
{
  cout << "preprocess data" << endl;
  ifstream file(dataPath);
  if (!file.is_open())
  {
    cerr << "Load data failed\n";
    return;
  }
  string line;
  int i = 0;
  while (getline(file, line))
  {
    int start;
    int end;
    istringstream lineStream(line);
    string prop;

    getline(lineStream, prop, ',');
    start = stoul(prop.erase(0, 1));
    getline(lineStream, prop, ',');
    end = stoul(prop.erase(0, 1));

    ips[i].start = start;
    ips[i].end = end;
    i++;
  }
  file.close();
  sort(ips, ips + totalIp, &compareIpRange);
  cout << "end sort" << endl;
}

bool IpVPN::isVPN(string ip)
{
  unsigned int convertedIp = ipv4ToNumber(ip);
  int left = 0;
  int right = totalIp - 1;
  int nearestLeft = -1;
  while (left <= right) {
    int mid = left + (right - left) / 2;
    if (ips[mid].start <= convertedIp) {
      nearestLeft = mid;
      left = nearestLeft + 1;
    } else {
      right = mid - 1;
    }
  }
  if (nearestLeft == -1) return false;
  if (convertedIp <= ips[nearestLeft].end) return true;
  return false;
}
