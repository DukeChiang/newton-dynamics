/* Copyright (c) <2009> <Newton Game Dynamics>
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#include "dCILstdafx.h"
#include "dCIL.h"
#include "dCILInstr.h"
#include "dCILInstrBranch.h"
#include "dCILInstrMiscellaneous.h"


dCILInstrEnter::dCILInstrEnter(dCIL& program, dCILInstrFunction* const parent, int registerMask, int localMemorySize)
	:dCILInstr(program)
	,m_registerMask(registerMask)
	,m_localMemorySize(localMemorySize)
{
	program.InsertAfter(parent->GetNode(), m_myNode);
}

void dCILInstrEnter::Serialize(char* const textOut) const
{
	sprintf(textOut, "\tenter %d, %d\n", m_registerMask, m_localMemorySize);
}

dVirtualMachine::dOpCode dCILInstrEnter::EmitOpcode() const
{
	dVirtualMachine::dOpCode code;
	code.m_type1.m_opcode = unsigned(dVirtualMachine::m_enter);
	code.m_type1.m_imm1 = m_registerMask;
	code.m_type1.m_imm2 = m_localMemorySize;
	return code;
}

dCILInstrExit::dCILInstrExit(dCILInstrEnter* const enter, dCILInstrReturn* const successor)
	:dCILInstr(*enter->GetCil())
	,m_registerMask (enter->m_registerMask)
	,m_localMemorySize(enter->m_localMemorySize)
{
	enter->GetCil()->InsertAfter(successor->GetNode()->GetPrev(), m_myNode);
}


void dCILInstrExit::Serialize(char* const textOut) const
{
	sprintf(textOut, "\texit %d, %d\n", m_registerMask, m_localMemorySize);
}


dCILInstrNop::dCILInstrNop()
	:dCILInstr()
{
}

dCILInstrNop::dCILInstrNop(dCIL& program)
	:dCILInstr (program)
{
}

void dCILInstrNop::Serialize(char* const textOut) const
{
	//	sprintf(textOut, "\tnop\n");
	textOut[0] = 0;
}



dCILInstrFunction::dCILInstrFunction (dCIL& program, const dString& name, const dArgType& type)
	:dCILInstr (program)
	,m_name (name, type)
	,m_parameters()
{
}


dList<dCILInstr::dArg>::dListNode* dCILInstrFunction::AddParameter (const dString& name, const dArgType& type)
{
	return m_parameters.Append (dArg (name, type));
}


void dCILInstrFunction::Serialize(char* const textOut) const
{
	sprintf (textOut, "\nfunction %s %s (", m_name.GetTypeName().GetStr(), m_name.m_label.GetStr());
	for (dList<dArg>::dListNode* node = m_parameters.GetFirst(); node; node = node->GetNext()) { 
		char text[2048];
		const dArg& arg = node->GetInfo();
		if (node->GetNext()) {
			sprintf (text, "%s %s, ", arg.GetTypeName().GetStr(), arg.m_label.GetStr());
		} else {
			sprintf (text, "%s %s", arg.GetTypeName().GetStr(), arg.m_label.GetStr());
		}
		strcat (textOut, text);
	}

	strcat (textOut, ")\n");
}


void dCILInstrFunction::AddGeneratedAndUsedSymbols (dDataFlowPoint& datFloatPoint) const
{
	dAssert (0);
}