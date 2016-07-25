/*
    Goblin compiler v1.0 - The standard compiler for the Goblin language.
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
 * File:   VDEFBranch.h
 * Author: Daniel McCarthy
 *
 * Created on 25 June 2016, 02:31
 */

#ifndef VDEFBRANCH_H
#define VDEFBRANCH_H

#include "CustomBranch.h"
struct array_def
{
    int dimensions;
    size_t t_size;
    std::vector<size_t> sizes;
};
class VDEFBranch : public CustomBranch {
public:
    VDEFBranch(Compiler* compiler);
    virtual ~VDEFBranch();
    
    std::shared_ptr<Branch> getDefinitionTypeBranch();
    std::shared_ptr<Branch> getDefinitionNameBranch();
    bool isArray();
    struct array_def getArray();
private:

};

#endif /* VDEFBRANCH_H */

