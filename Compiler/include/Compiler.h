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
 * File:   Compiler.h
 * Author: Daniel McCarthy
 *
 * Created on 18 June 2016, 18:53
 */

#ifndef COMPILER_H
#define COMPILER_H

#include <memory>
#include <string>
#include <map>
#include "Lexer.h"
#include "Parser.h"
#include "preprocessor.h"
#include "SemanticValidator.h"
#include "TreeImprover.h"
#include "ASTAssistant.h"
#include "CodeGenerator.h"
#include "Exception.h"
#include "Linker.h"
#include "common.h"
#include "def.h"

class VDEFBranch;
class STRUCTBranch;
class ArrayIndexBranch;
class EXPORT Compiler
{
public:
    Compiler();
    virtual ~Compiler();

    void setArgument(std::string name, std::string value);
    void setCodeGenerator(std::shared_ptr<CodeGenerator> codegen);
    void setLinker(std::shared_ptr<Linker> linker);
    Lexer* getLexer();
    Parser* getParser();
    Preprocessor* getPreprocessor();
    SemanticValidator* getSemanticValidator();
    TreeImprover* getTreeImprover();
    ASTAssistant* getASTAssistant();
    std::shared_ptr<CodeGenerator> getCodeGenerator();
    std::shared_ptr<Linker> getLinker();
    std::string getArgumentValue(std::string name);
    bool hasArgument(std::string name);

    std::shared_ptr<VDEFBranch> getVariableFromStructure(std::shared_ptr<STRUCTBranch> structure, std::string var_name);
    int getSizeOfVarDef(std::shared_ptr<VDEFBranch> vdef_branch);

    static bool isPrimitiveDataType(std::string type);
    static int getPrimitiveDataTypeSize(std::string type);
    int getDataTypeSizeFromVarDef(std::shared_ptr<VDEFBranch> vdef_branch, bool no_pointer = false);
    int getSizeOfStructure(std::shared_ptr<STRUCTBranch> structure);
    int getSumOfArrayIndexes(std::shared_ptr<ArrayIndexBranch> root_array_index_branch);

    std::string getTypeFromNumber(int number);
    int getSizeFromType(std::string type);
    bool canFit(std::string fit_to, std::string fit_from);
    long evaluate(long n1, long n2, std::string op);
    bool isCompareOperator(std::string value);
    bool isLogicalOperator(std::string value);
    bool isAssignmentOperator(std::string value);
    long getNumberFromString(std::string str, char formatting_symbol);
    long getNumberFromString(std::string str);

private:
    /* Ideally these do not have to be pointers but since arguments now exist in the constructors of these objects I have ran into issues calling them.
     * I plan to change this soon */
    Lexer* lexer;
    Parser* parser;
    Preprocessor* preprocessor;
    SemanticValidator* semanticValidator;
    TreeImprover* treeImprover;
    ASTAssistant* astAssistant;
    
    std::map<std::string, std::string> arguments;
    int pointer_size;

    // The code generator must be shared as we are not the sole owner of it and it would be unsafe to not have this as a shared pointer.
    std::shared_ptr<CodeGenerator> codeGenerator;

    std::shared_ptr<Linker> linker;
};

#endif /* COMPILER_H */

