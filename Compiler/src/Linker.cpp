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
 * File:   Linker.cpp
 * Author: Daniel McCarthy
 *
 * Created on 02 July 2016, 03:20
 * 
 * Description: The base linker class for all linkers
 */

#include "Linker.h"
#include "Stream.h"
#include "VirtualObjectFormat.h"
#include "common.h"

Linker::Linker(Compiler* compiler) : CompilerEntity(compiler)
{
}

Linker::~Linker()
{

}
void Linker::addObjectFile(std::shared_ptr<VirtualObjectFormat> obj)
{
    if (hasObjectFile(obj))
    {
        throw Exception("The object file provided has already been added to this linker", "void Linker::addObjectFile(std::shared_ptr<VirtualObjectFormat> obj)");
    }
    this->obj_stack.push_back(obj);
}

bool Linker::hasObjectFile(std::shared_ptr<VirtualObjectFormat> obj)
{
    return std::find(this->obj_stack.begin(), this->obj_stack.end(), obj) != this->obj_stack.end();
}

void Linker::link()
{
    if (this->obj_stack.empty())
    {
        throw Exception("Nothing to link", "void Linker::link()");
    }
    std::shared_ptr<VirtualObjectFormat> main_obj = this->obj_stack.front();
    this->obj_stack.pop_front();
    while (!this->obj_stack.empty())
    {
        std::shared_ptr<VirtualObjectFormat> other_obj = this->obj_stack.front();
        this->obj_stack.pop_front();
        this->link_merge(main_obj, other_obj);
    }


    /* At this point we should have all external references defined in one object file, 
     * so lets make sure external references can be linked up properly */
    
    for (std::string ext_ref : main_obj->getExternalReferences())
    {
        if (!main_obj->hasGlobalReference(ext_ref))
        {
            throw Exception("Undefined reference: " + ext_ref + ", ensure that you are including all your object files and try again", "void Linker::link()");
        }
    }
    
    // Resolve unknown symbols
    this->resolve(main_obj);

#ifdef DEBUG_MODE
    std::cout << "Final Object" << std::endl;
    debug_virtual_object_format(main_obj);
#endif
    // Build the executable
    this->build(&this->executable_stream, main_obj);

}

void Linker::link_merge(std::shared_ptr<VirtualObjectFormat> obj1, std::shared_ptr<VirtualObjectFormat> obj2)
{
    // We need to merge the formats
    obj1->append(obj2);
}

Stream* Linker::getExecutableStream()
{
    return &this->executable_stream;
}