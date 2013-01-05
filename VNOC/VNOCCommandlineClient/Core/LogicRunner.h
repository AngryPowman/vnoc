/*
 * File : LogicRunner.h
 *
 *  Created on: 2013/1/5
 *      Author: AngryPowman
 *  
 *  Revised from Athena/Utils/Delagate
 */

#ifndef VNOC_LOGIC_RUNNER
#define VNOC_LOGIC_RUNNER

#include "Common.h"

class ExtraData
{
public:
	ExtraData();
	virtual ~ExtraData();

public:
	uint32 id() const
	{
		return _id;
	}

private:
	uint32 _id;
};

template <typename T>
class LogicRunner
{
protected:
	typedef void (T::*RunnerFunctionPtr)(const ExtraData* data);
	typedef std::map<uint32, RunnerFunctionPtr> RunnerFunctionTable;

public:
	LogicRunner()
	{
	}

	virtual ~LogicRunner()
	{
	}

private:
	RunnerFunctionPtr* operator[](uint32 id)
	{
		RunnerFunctionTable::iterator iter = _functionTable.find(id);
		if (iter != _functionTable.end())
		{
			return &iter->second;
		}
	}

protected:
	void bind(uint32 id, RunnerFunctionPtr func)
	{
		_functionTable[id] = func;
	}

public:
	virtual void execute(uint32 id, const ExtraData* extraData)
	{
		RunnerFunctionTable::iterator iter = _functionTable.find(id);
		if (iter != _functionTable.end())
		{
			RunnerFunctionPtr func = iter->second;
			(static_cast<T*>(this)->*func)(extraData);	//unsafe
		}
	}

protected:
	RunnerFunctionTable _functionTable;
};

#endif