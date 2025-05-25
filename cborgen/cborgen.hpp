/**
 * Project: Phoenix
 * Time-stamp: <2025-05-25 12:42:17 doug>
 * 
 * @file cborgen.hpp
 * @author Douglas Mark Royer
 * @date 08-MAR-2025
 * 
 * @Copyright(C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
 * 
 * Licensed under the MIT License. See LICENSE file
 * or https://opensource.org/licenses/MIT for details.
 * 
 * RiverExplorer is a trademark of Douglas Mark Royer
 */
#ifndef _RIVEREXPLORER_CBORGEN_CBORGEN_HPP_
#define _RIVEREXPLORER_CBORGEN_CBORGEN_HPP_

#include <iostream>
#include <fstream>
#include <string>
#include "antlr4-runtime.h"
#include "cborLexer.h"
#include "cborParser.h"
#include "cborListener.h"
#include "cborBaseListener.h"

#include <rpc/rpc.h>

#include "GenerateCpp.hpp"

using namespace antlr4;
using namespace std;

namespace RiverExplorer::cborgen
{
	/**
	 * The input file name.
	 */
	extern std::string InputFileName;

	/**
	 * Used sometimes for the target output directory.
	 */
	extern std::string CppOutputDirectory;
	
	/**
	 * The input file name without extension.
	 */
	extern std::string InputNoExtension;
	
	/**
	 * The input file name without extension.
	 */
	extern std::string OutputDirectory;

	/**
	 * Shared 'this file was generated ... message.
	 */
	extern std::string ThisFileWasGenerated;
	
	/**
	 * When true, run a quiet as possibe.
	 * Error are still printed.
	 */
	extern bool RunQuiet;

	/**
	 * When not empty, everything goes into this namespace.
	 */
	extern std::string Namespace;
	
	/**
	 * Namespace converted to 'part1::part2::...' format.
	 */
	extern std::string CppNamespace;
	
	/**
	 * Indentation level used when generating code.
	 */
	extern int IndentLevel;

	/**
	 * Test if Type is a built n CBOR type.
	 * (int, unsigned int, string, opaque, ...)
	 *
	 * @param Type The type name to test.
	 *
	 * @return true if Type is a known built in type.
	 */
	extern bool IsBuiltInCborType(const std::string & Type);
	 
	/**
	 * Generate a string given IndenLevel.
	 *
	 * @return The indent string.
	 */
	extern std::string Indent();
	
	/**
	 * Generate a string given Level.
	 *
	 * @param Leven How many levels of indent.
	 *
	 * @return The indent string.
	 */
	extern std::string Indent(int Level);

	/**
	 * Add to UserDefinedTypes, duplicate values are ignored.
	 *
	 * @param AUserType A type defined by the .cbor files.
	 *
	 * @param HFile The header file needed to use AUserType.
	 */
	void AddToUserDefinedTypes(std::string AUserType, std::string HFile);

	enum State	{
		Unknown,
		InVar,
		InVarFixed,
		InVarVariable,
		InOpaqueFixed,
		InOpaqueVariable,
		InString,
		InNamespaceDef,
		InStruct,
		InStructBody,
		InTypeDef,
		InUnion,
		InUnionCase,
		InProcedureDef,
		InVoid,
		InComment,
		InPassThrough,
		InEnum,
		InEnumBody,
		InConstant,
		InMethod,
		InProgram,
		InVersion,
		InVersionMethod,
		InDeclaration,
		InEnumTypeSpec,
		InStructTypeSpec,
		InUnionTypeSpec,
		InIgnoreTag,
		InInternalTag,
		InOverrideTag,
		InPrivateTag,
		InSortableTag,
		InFloatValue,
		InBigNumInt,
		InBigNumUInt,
		InBigNumFloat,
		InMap,
		InMultimap,
		InUnsignedInteger,
		InSignedInteger
	};

	enum BasicType_e	{
		UnknownType_t,
		AmVoid_t,
		AmInt8_t,
		AmInt16_t,
		AmInt32_t,
		AmInt64_t,
		AmIntBigNum_t,
		
		AmUInt8_t,
		AmUInt16_t,
		AmUInt32_t,
		AmUInt64_t,
		AmUIntBigNum_t,

		AmFloat8_t,
		AmFloat16_t,
		AmFloat32_t,
		AmFloat64_t,
		AmFloatBigNum_t,

		AmString_t,
		AmBool_t,
		AmBit_t,
		AmBits_t,
		AmBitMask_t,
		AmEnum_t,
		AmIdentifier_t,
		AmMap_t,
		AmMultimap_t,
		AmStruct_t,
		AmUnion_t,
		AmOpaque_t,

		AmUserDefined_t
	};


	// Is this what I want?
	//
	extern std::vector<std::string> KnownNamespaces;
	
	/**
	 * @class Item
	 * This exists as a base class for evertying, so
	 * we can put them into one vector and keep the
	 * original objcet order.
	 */
	class Item
	{
	public:

		/**
		 * Item - Default Constructor.
		 */
		Item();
		
		/**
		 * Item - Destructor.
		 */
		virtual ~Item();
		
		enum Scope_e {
			UnknownScope_t,
			InternalScope_t,
			PrivateScope_t,
			NamespaceScope_t
		};

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const = 0;
		
		virtual void PrintCppCBOR(ostream & Steam) const = 0;
		virtual void PrintCppStubs(ostream & Stream) const = 0;
		virtual void PrintXSD(ostream & Stream) const = 0;
		virtual void PrintAbnf(ostream & Stream) const = 0;
		virtual void PrintServer(ostream & Stream) const = 0;
		
		bool IsFixedArray = false;
		bool IsVariableArray = false;
		std::string SizeOrValue;

		std::string Type;
		BasicType_e DataType;
		
		std::string Name;

		Scope_e OurScope;
		
		/**
		 * Print the variable for a CPP Header.
		 */
		void PrintCppDeclareVariable(ostream & Stream) const;
	};

	extern std::vector<Item*> OrderedItems;

	extern State CurrentState;
	extern bool InArray;

	/**
	 * @class Constant cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Constant
		: public Item
	{
	public:

		virtual ~Constant();

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};
	extern Constant * CurrentConstant;

	/**
	 * @class EnumValue cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class EnumValue
		: public Item
	{
	public:

		virtual ~EnumValue();

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class Enum cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Enum
		: public Item
	{
	public:

		virtual ~Enum();

		std::vector<Item*> Enums;
		
		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class Variable cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Variable
		: public Item
	{
	public:

		virtual ~Variable();
		
		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class TypeDef cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class TypeDef
		: public Item
	{
	public:

		virtual ~TypeDef();

		Variable * Declaration;
		
		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class StructMember cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class StructMember
		: public Item
	{
	public:

		virtual ~StructMember();

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};
	extern StructMember * CurrentStructMember;

	/**
	 * @class Struct cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Struct
		: public Item
	{
	public:

		virtual ~Struct();
		
		std::vector<Item*> Members;

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};
	extern Struct * CurrentStruct;

	/**
	 * @class UnionCase cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class UnionCase
		: public Item
	{
	public:
		virtual ~UnionCase();

		void CopyFrom(Variable *& Var);
		
		std::string CaseValue;
		bool IsDefault;

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class Union cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Union
		: public Item
	{
	public:

		Union();
		virtual ~Union();
		
		std::string Name;
		std::string SwitchType;
		std::string SwitchVariable;
		std::vector<Item*> Cases;
		
		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};
	extern Union * CurrentUnion;

	/**
	 * @class Procedure cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Procedure
		: public Item
	{
	public:

		virtual ~Procedure();
		
		std::string ReturnType;
		std::vector<Item*> Params;
		int64_t ProcNumber;

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class Program cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Program
		: public Item
	{
	public:

		virtual ~Program();
		
		std::vector<Item*> Versions;
		std::map<std::string, Constant*> Constants;
		std::map<std::string, std::vector<Item*>*> Enums;
		std::map<std::string, TypeDef*> TypeDefs;
		std::map<std::string, Struct*> Structs;
		std::map<std::string, Union*> Unions;

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class Version cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Version
		: public Item
	{
	public:

		Version(Program & Parent);
		virtual ~Version();
		
		std::vector<Item*> Procedures;

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;

		Program & Parent;
	};

	/**
	 * @class Comment cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class Comment
		: public Item
	{
	public:
		
		virtual ~Comment();
		
		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class PassThrough cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 */
	class PassThrough
		: public Item
	{
	public:
		
		virtual ~PassThrough();

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;
	};

	/**
	 * @class Method cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 * A 'Method' is of the form:
	 *
	 *   returnType MethodName(Parameter1, ...);
	 *
	 * Or if wihin a program/version:
	 *
	 *   returnType MethodName(Parameter1, ...) = value;
	 */
	class Method
		: public Item
	{
	public:

		Method(Struct & Parent);
		virtual ~Method();

		std::vector<std::string> Parameters;

		/**
		 * Only used inside of a program/version.
		 */
		std::string VersionValue;

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;

		Struct & Parent;
	};
	
	/**
	 * @class Method cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 * A 'Method' is of the form:
	 *
	 *   returnType MethodName(Parameter1, ...);
	 *
	 * Or if wihin a program/version:
	 *
	 *   returnType MethodName(Parameter1, ...) = value;
	 */
	class VersionMethod
		: public Item
	{
	public:

		VersionMethod(Version & Parent);
		virtual ~VersionMethod();

		std::vector<std::string> Parameters;

		/**
		 * Only used inside of a program/version.
		 */
		std::string MethodValue;

		/**
		 * Print this item for use in a C++ header file.
		 *
		 * @param Stream Where to send the data to.
		 *
		 * @param WithExtern When false, do not print 'extern'.
		 */
		virtual void PrintCppHeader(ostream & Stream,
																bool WithExtern = true) const;
		
		virtual void PrintCppCBOR(ostream & Stream) const;
		virtual void PrintCppStubs(ostream & Stream) const;
		virtual void PrintXSD(ostream & Stream) const;
		virtual void PrintAbnf(ostream & Stream) const;
		virtual void PrintServer(ostream & Stream) const;

		Version & Parent;
	};
	
	/**
	 * @class MyCborListener cborgen.hpp <RiverExplorer/cborgen/cborgen.hpp>
	 * A support class for the antlr4 generated code.
	 */
	class MyCborListener
		: public cborListener
	{
	public:
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::ConstantContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::TypeSpecifierContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::EnumTypeSpecContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::EnumBodyContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::StructTypeSpecContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::StructBodyContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::UnionTypeSpecContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::UnionBodyContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::CaseSpecContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::TypeDefContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::ConstantDefContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::ValueContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::CborSpecificationContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::NamespaceDefContext  * Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::DefinitionContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::SpecsContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::CommentContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::PassThroughContext* Ctx);

		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::MethodContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::ProcFirstArgContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::ProgramContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::VersionContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::VersionMethodContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 cborParser::IdentifierContext* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 tree::TerminalNode* Ctx);
		
		void ProcessNode(bool Enter,
										 std::string From,
										 ParserRuleContext* Ctx);
		

		//////////////////////////////////////////////////////////////////////
		virtual void enterValue(cborParser::ValueContext *Ctx);
		virtual void exitValue(cborParser::ValueContext *Ctx);
		
		virtual void enterConstant(cborParser::ConstantContext *Ctx);
		virtual void exitConstant(cborParser::ConstantContext *Ctx);
		
		virtual void enterTypeSpecifier(cborParser::TypeSpecifierContext *Ctx);
		virtual void exitTypeSpecifier(cborParser::TypeSpecifierContext *Ctx);
		
		virtual void enterEnumTypeSpec(cborParser::EnumTypeSpecContext *Ctx);
		virtual void exitEnumTypeSpec(cborParser::EnumTypeSpecContext *Ctx);
		
		virtual void enterEnumBody(cborParser::EnumBodyContext *Ctx);
		virtual void exitEnumBody(cborParser::EnumBodyContext *Ctx);
		
		virtual void enterStructTypeSpec(cborParser::StructTypeSpecContext *Ctx);
		virtual void exitStructTypeSpec(cborParser::StructTypeSpecContext *Ctx);
		
		virtual void enterStructBody(cborParser::StructBodyContext *Ctx);
		virtual void exitStructBody(cborParser::StructBodyContext *Ctx);
		
		virtual void enterUnionTypeSpec(cborParser::UnionTypeSpecContext *Ctx);
		virtual void exitUnionTypeSpec(cborParser::UnionTypeSpecContext *Ctx);
		
		virtual void enterUnionBody(cborParser::UnionBodyContext *Ctx);
		virtual void exitUnionBody(cborParser::UnionBodyContext *Ctx);
		
		virtual void enterCaseSpec(cborParser::CaseSpecContext *Ctx);
		virtual void exitCaseSpec(cborParser::CaseSpecContext *Ctx);
		
		virtual void enterConstantDef(cborParser::ConstantDefContext *Ctx);
		virtual void exitConstantDef(cborParser::ConstantDefContext *Ctx);
		
		virtual void enterTypeDef(cborParser::TypeDefContext *Ctx);
		virtual void exitTypeDef(cborParser::TypeDefContext *Ctx);
		
		virtual void enterDefinition(cborParser::DefinitionContext *Ctx);
		virtual void exitDefinition(cborParser::DefinitionContext *Ctx);
		
		virtual void enterCborSpecification(cborParser::CborSpecificationContext *Ctx);
		virtual void exitCborSpecification(cborParser::CborSpecificationContext *Ctx);
		
		virtual void enterNamespaceDef(cborParser::NamespaceDefContext * Ctx);
		virtual void exitNamespaceDef(cborParser::NamespaceDefContext * Ctx);
		
		virtual void enterSpecs(cborParser::SpecsContext * Ctx);
		virtual void exitSpecs(cborParser::SpecsContext * Ctx);
		
		virtual void enterEveryRule(ParserRuleContext * Ctx);
		virtual void exitEveryRule(ParserRuleContext * Ctx);

		virtual void enterMethod(cborParser::MethodContext * Ctx);
		virtual void exitMethod(cborParser::MethodContext * Ctx);

		virtual void enterProcFirstArg(cborParser::ProcFirstArgContext * Ctx);
		virtual void exitProcFirstArg(cborParser::ProcFirstArgContext * Ctx);

		virtual void enterComment(cborParser::CommentContext * Ctx);
		virtual void exitComment(cborParser::CommentContext * Ctx);

		virtual void enterPassThrough(cborParser::PassThroughContext * Ctx);
		virtual void exitPassThrough(cborParser::PassThroughContext * Ctx);

		virtual void enterProgram(cborParser::ProgramContext * Ctx);
		virtual void exitProgram(cborParser::ProgramContext * Ctx);

		virtual void enterVersion(cborParser::VersionContext * Ctx);
		virtual void exitVersion(cborParser::VersionContext * Ctx);

		virtual void enterVersionMethod(cborParser::VersionMethodContext * Ctx);
		virtual void exitVersionMethod(cborParser::VersionMethodContext * Ctx);

		virtual void enterDataType(cborParser::DataTypeContext * Ctx);
		virtual void exitDataType(cborParser::DataTypeContext * Ctx);

		virtual void enterDeclaration(cborParser::DeclarationContext * Ctx);
		virtual void exitDeclaration(cborParser::DeclarationContext * Ctx);

		virtual void enterWidth(cborParser::WidthContext * Ctx);
		virtual void exitWidth(cborParser::WidthContext * Ctx);

		virtual void enterIdentifier(cborParser::IdentifierContext * Ctx);
		virtual void exitIdentifier(cborParser::IdentifierContext * Ctx);

		virtual void enterHexadecimal(cborParser::HexadecimalContext * Ctx);
		virtual void exitHexadecimal(cborParser::HexadecimalContext * Ctx);

		virtual void enterOctal(cborParser::OctalContext * Ctx);
		virtual void exitOctal(cborParser::OctalContext * Ctx);

		virtual void enterDecimal(cborParser::DecimalContext * Ctx);
		virtual void exitDecimal(cborParser::DecimalContext * Ctx);

		virtual void enterFloat(cborParser::FloatContext * Ctx);
		virtual void exitFloat(cborParser::FloatContext * Ctx);

		virtual void enterTags(cborParser::TagsContext * Ctx);
		virtual void exitTags(cborParser::TagsContext * Ctx);

		virtual void enterIgnoreTag(cborParser::IgnoreTagContext * Ctx);
		virtual void exitIgnoreTag(cborParser::IgnoreTagContext * Ctx);

		virtual void enterOverrideTag(cborParser::OverrideTagContext * Ctx);
		virtual void exitOverrideTag(cborParser::OverrideTagContext * Ctx);

		virtual void enterSortableTag(cborParser::SortableTagContext * Ctx);
		virtual void exitSortableTag(cborParser::SortableTagContext * Ctx);

		virtual void enterPrivateTag(cborParser::PrivateTagContext * Ctx);
		virtual void exitPrivateTag(cborParser::PrivateTagContext * Ctx);

		virtual void enterInternalTag(cborParser::InternalTagContext * Ctx);
		virtual void exitInternalTag(cborParser::InternalTagContext * Ctx);

		virtual void enterPublicTag(cborParser::PublicTagContext * Ctx);
		virtual void exitPublicTag(cborParser::PublicTagContext * Ctx);

		virtual void enterString(cborParser::StringContext * Ctx);
		virtual void exitString(cborParser::StringContext * Ctx);

		virtual void enterNamespaceTag(cborParser::NamespaceTagContext * Ctx);
		virtual void exitNamespaceTag(cborParser::NamespaceTagContext * Ctx);

		virtual void enterInternal_identifier(cborParser::Internal_identifierContext * Ctx);
		virtual void exitInternal_identifier(cborParser::Internal_identifierContext * Ctx);

		virtual void enterNamespace_identifier(cborParser::Namespace_identifierContext * Ctx);
		virtual void exitNamespace_identifier(cborParser::Namespace_identifierContext * Ctx);

		virtual void enterFloatValue(cborParser::FloatValueContext * Ctx);
		virtual void exitFloatValue(cborParser::FloatValueContext * Ctx);

		virtual void enterMap(cborParser::MapContext * Ctx);
		virtual void exitMap(cborParser::MapContext * Ctx);

		virtual void enterMultimap(cborParser::MultimapContext * Ctx);
		virtual void exitMultimap(cborParser::MultimapContext * Ctx);

		virtual void enterUnsignedInteger(cborParser::UnsignedIntegerContext * Ctx);
		virtual void exitUnsignedInteger(cborParser::UnsignedIntegerContext * Ctx);

		virtual void enterSignedInteger(cborParser::SignedIntegerContext * Ctx);
		virtual void exitSignedInteger(cborParser::SignedIntegerContext * Ctx);

		virtual void enterBigNumUInt(cborParser::BigNumUIntContext * Ctx);
		virtual void exitBigNumUInt(cborParser::BigNumUIntContext * Ctx);

		virtual void enterBigNumInt(cborParser::BigNumIntContext * Ctx);
		virtual void exitBigNumInt(cborParser::BigNumIntContext * Ctx);

		virtual void enterBigNumFloat(cborParser::BigNumFloatContext * Ctx);
		virtual void exitBigNumFloat(cborParser::BigNumFloatContext * Ctx);

		virtual void enterBitmaskBody(cborParser::BitmaskBodyContext * Ctx);
		virtual void exitBitmaskBody(cborParser::BitmaskBodyContext * Ctx);

		virtual void enterBitmaskTypeSpec(cborParser::BitmaskTypeSpecContext * Ctx);
		virtual void exitBitmaskTypeSpec(cborParser::BitmaskTypeSpecContext * Ctx);

		virtual void enterBitsTypeSpec(cborParser::BitsTypeSpecContext * Ctx);
		virtual void exitBitsTypeSpec(cborParser::BitsTypeSpecContext * Ctx);

		virtual void visitTerminal(tree::TerminalNode * Node);
		virtual void visitErrorNode(tree::ErrorNode * Node);
		
	};
}
#endif // _RIVEREXPLORER_CBORGEN_CBORGEN_HPP_
