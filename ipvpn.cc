#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "ipvpn.h"

using namespace std;

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
  int i = 0;
  int count = 0;
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

    for (int ip = start; ip <= end; ip++)
    {
      count++;
    }
  }
  cout << "count" << count;
  totalIp = count;
  unsigned int *t = new unsigned int[count];
  ips = t;
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

    for (int ip = start; ip <= end; ip++)
    {
      ips[i++] = ip;
    }
  }
  cout << "start sort" << endl;
  sort_heap(ips, ips + totalIp);
  cout << "end sort" << endl;
  file.close();
}

unsigned int *IpVPN::getIps()
{
  return ips;
}

bool IpVPN::isVPN(string ip)
{
  unsigned int convertedIp = ipv4ToNumber(ip)
  return binary_search(ips, ips + totalIp, convertedIp);
}
