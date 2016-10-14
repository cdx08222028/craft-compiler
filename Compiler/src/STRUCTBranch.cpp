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
 * File:   STRUCTBranch.cpp
 * Author: Daniel McCarthy
 *
 * Created on 13 October 2016, 05:47
 * 
 * Description: 
 */

#include "STRUCTBranch.h"

STRUCTBranch::STRUCTBranch(Compiler* compiler) : CustomBranch(compiler, "STRUCT", "")
{
}

STRUCTBranch::~STRUCTBranch()
{
}

void STRUCTBranch::setStructNameBranch(std::shared_ptr<Branch> branch)
{
    CustomBranch::registerBranch("struct_name_branch", branch);
}

void STRUCTBranch::setStructBodyBranch(std::shared_ptr<Branch> branch)
{
    CustomBranch::registerBranch("struct_body_branch", branch);
}

std::shared_ptr<Branch> STRUCTBranch::getStructNameBranch()
{
    return CustomBranch::getRegisteredBranchByName("struct_name_branch");
}

std::shared_ptr<Branch> STRUCTBranch::getStructBodyBranch()
{
    return CustomBranch::getRegisteredBranchByName("struct_body_branch");
}