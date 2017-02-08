/*
    Craft Compiler v0.1.0 - The standard compiler for the Craft programming language.
    Copyright (C) 2016  Daniel McCarthy

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* 
 * File:   Stream.cpp
 * Author: Daniel McCarthy
 *
 * Created on 20 June 2016, 01:18
 * 
 * Description: A basic stream class
 * 
 * Note: This stream class may not support big endian machines at its current state.
 * 
 * All write operations push to the back of the stack and all read operations read from the front of the stack.
 */

#include "Stream.h"
#include <string.h>

Stream::Stream()
{
    setPosition(0);
    setOverwriteMode(false);
}

Stream::~Stream()
{
}

void Stream::loadFromFile(std::string filename)
{
    std::ifstream ifs;
    ifs.open(filename, std::ios::binary);
    if (!ifs.is_open())
    {
        throw Exception("Failed to open: " + filename);
    }

    this->loadFrom_ifstream(&ifs);
}

void Stream::loadFrom_ifstream(std::ifstream* stream)
{
    while (stream->good())
    {
        this->write8(stream->get());
    }
}

void Stream::setPosition(size_t position)
{
    this->pos = position;
}

void Stream::setOverwriteMode(bool overwrite_mode)
{
    this->overwrite_mode = overwrite_mode;
}

void Stream::write8(uint8_t c)
{
    if (this->vector.size() < 0)
    {
        throw Exception("uint8_t Stream::write8(): out of bounds");
    }
    if (isOverwriteModeEnabled())
    {
        vector.at(pos) = c;
    }
    else
    {
        vector.insert(vector.begin() + pos, c);
    }
    pos++;
}

void Stream::write16(uint16_t s)
{
    uint8_t c1 = s & 0xff;
    uint8_t c2 = s >> 8;
    write8(c1);
    write8(c2);
}

void Stream::write32(uint32_t i)
{
    uint16_t s1 = i & 0xffff;
    uint16_t s2 = i >> 16;
    write16(s1);
    write16(s2);
}

void Stream::writeStr(std::string str, bool write_null_terminator, size_t fill_to)
{
    writeStr(str.c_str(), write_null_terminator, fill_to);
}

/* Should probably try to write this better when I am a bit
 more awake. A bit sleepy at the moment*/
void Stream::writeStr(const char* str, bool write_null_terminator, size_t fill_to)
{
    int i = 0;
    while (str[i] != 0)
    {
        write8(str[i]);
        i++;
    }

    if (write_null_terminator)
    {
        write8(0);
        i++;
    }

    if (fill_to != -1 && fill_to - i > 0)
    {
        // We need to fill it to a maximum point
        for (; i < fill_to; i++)
        {
            write8(0);
        }
    }
}

void Stream::writeStream(Stream* stream)
{
    int old_pos = stream->getPosition();
    stream->setPosition(0);
    while (stream->hasInput())
    {
        write8(stream->read8());
    }
    stream->setPosition(old_pos);
}

void Stream::writeStream(std::shared_ptr<Stream> stream)
{
    writeStream(stream.get());
}

void Stream::overwrite8(int pos, uint8_t c)
{
    // Work on this what if overwrite mode is already set? You have an issue then
    setOverwriteMode(true);
    int old_pos = getPosition();
    setPosition(pos);
    write8(c);
    setPosition(old_pos);
    setOverwriteMode(false);
}

void Stream::overwrite16(int pos, uint16_t s)
{
    // Work on this what if overwrite mode is already set? You have an issue then
    setOverwriteMode(true);
    int old_pos = getPosition();
    setPosition(pos);
    write16(s);
    setPosition(old_pos);
    setOverwriteMode(false);
}

void Stream::overwrite32(int pos, uint32_t i)
{
    // Work on this what if overwrite mode is already set? You have an issue then
    setOverwriteMode(true);
    int old_pos = getPosition();
    setPosition(pos);
    write32(i);
    setPosition(old_pos);
    setOverwriteMode(false);
}

uint8_t Stream::peek8(int pos)
{
    if (this->vector.size() <= pos)
    {
        throw Exception("uint8_t Stream::peek8(int pos) stream out of bounds");
    }
    uint8_t c = this->vector.at(pos);
    return c;
}

uint16_t Stream::peek16(int pos)
{
    uint8_t c1 = peek8(pos);
    uint8_t c2 = peek8(pos + 1);

    uint16_t result = (c2 << 8 | c1);
    return result;
}

uint32_t Stream::peek32(int pos)
{
    uint16_t s1 = peek16(pos);
    uint16_t s2 = peek16(pos + 2);

    uint32_t result = (s2 << 16 | s1);
    return result;
}

uint8_t Stream::read8()
{
    if (this->vector.size() <= pos)
    {
        throw Exception("uint8_t Stream::read8(): stream out of bounds");
    }
    uint8_t c = peek8(pos);
    pos++;
    return c;
}

uint16_t Stream::read16()
{
    uint16_t result = peek16(pos);
    pos += 2;
    return result;
}

uint32_t Stream::read32()
{
    uint32_t result = peek32(pos);
    pos += 4;
    return result;
}

std::string Stream::readStr()
{
    std::string str = "";
    uint8_t c;
    while ((c = read8()) != 0)
    {
        str += c;
    }

    return str;
}

size_t Stream::getSize()
{
    return vector.size();
}

bool Stream::isEmpty()
{
    return vector.empty();
}

bool Stream::hasInput()
{
    return pos < vector.size();
}

bool Stream::isOverwriteModeEnabled()
{
    return this->overwrite_mode;
}

void Stream::empty()
{
    vector.erase(this->vector.begin(), this->vector.end());
}

int Stream::getPosition()
{
    return pos;
}

char* Stream::getBuf()
{
    return (char*) vector.data();
}

char* Stream::toNewBuf()
{
    int size = getSize();
    char* data = new char[size];
    memcpy(data, getBuf(), size);
    return data;
}