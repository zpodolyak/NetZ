#pragma once

#include "common.h"

struct SampleRecord
{
  int id;
  std::string name;
  int msgSize = 0;

  SampleRecord()
    : id(0)
    , name("")
  {}

  SampleRecord(int _id, std::string _name)
    : id(_id)
    , name(_name)
  {}

  void Load(const char* buff)
  {
    const char* read = buff;
    if (buff)
    {
      id = *(int*)buff;
      read = buff + sizeof(int);
      std::size_t strLen = *(std::size_t*)read;
      read += sizeof(std::size_t);
      name.assign(read, strLen);
    }
  }

  void Save(char* buff)
  {
    char* write = buff;
    if (buff)
    {
      memcpy(buff, &id, sizeof(int));
      write = buff + sizeof(int);
      std::size_t len = name.length();
      memcpy(write, &len, sizeof(std::size_t));
      write += sizeof(std::size_t);
      memcpy(write, name.c_str(), len);
    }
  }

  auto MessageLength() const
  {
    return sizeof(int) + sizeof(std::size_t) + name.length();
  }

  void DumpRecord() const
  {
    DebugMessage("SampleRecord id = %d, name = %s", id, name.c_str());
  }
};