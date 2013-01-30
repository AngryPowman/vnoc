#pragma once
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

namespace Thread
{
	class Tuple0
	{
	public:
	};

	template<class T1>
	class Tuple1
	{
	public:
		Tuple1(): arg(T1()) { }

		Tuple1(const T1& t1)
		{
			arg = t1;
		}
		T1 arg;
	};

	template<class T1, class T2>
	class Tuple2
	{
	public:
		Tuple2() : arg1(T1()), arg2(T2()) { }

		Tuple2(const T1& t1, const T2& t2)
		{
			arg1 = t1;
			arg2 = t2;
		}
		T1 arg1;
		T2 arg2;
	};

	template<class T1, class T2, class T3>
	class Tuple3
	{
	public:
		Tuple3() : arg1(T1()), arg2(T2()), arg3(T3()) {	}

		Tuple3(const T1& t1, const T2& t2, const T3& t3)
		{
			arg1 = t1;
			arg2 = t2;
			arg3 = t3;
		}
		T1 arg1;
		T2 arg2;
		T3 arg3;
	};

	template<class T1, class T2, class T3, class T4>
	class Tuple4
	{
	public:
		Tuple4() : arg1(T1()), arg2(T2()), arg3(T3()), arg4(T4()) {	}

		Tuple4(const T1& t1,const  T2& t2,const T3& t3, const T4& t4)
		{
			arg1 = t1;
			arg2 = t2;
			arg3 = t3;
			arg4 = t4;
		}
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
	};

	template<class T1, class T2, class T3, class T4, class T5>
	class Tuple5
	{
	public:
		Tuple5() : arg1(T1()), arg2(T2()), arg3(T3()), arg4(T4()), arg5(T5()) {	}

		Tuple5(const T1& t1,const  T2& t2,const T3& t3, const T4& t4, const T5& t5)
		{
			arg1 = t1;
			arg2 = t2;
			arg3 = t3;
			arg4 = t4;
			arg5 = t5;
		}
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6>
	class Tuple6
	{
	public:
		Tuple6() : arg1(T1()), arg2(T2()), arg3(T3()), arg4(T4()), arg5(T5()), arg6(T6()) {	}

		Tuple6(const T1& t1,const  T2& t2,const T3& t3, const T4& t4, const T5& t5, const T6& t6)
		{
			arg1 = t1;
			arg2 = t2;
			arg3 = t3;
			arg4 = t4;
			arg5 = t5;
			arg6 = t6;
		}
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		T6 arg6;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	class Tuple7
	{
	public:
		Tuple7() : arg1(T1()), arg2(T2()), arg3(T3()), arg4(T4()), arg5(T5()), arg6(T6()), arg7(T7()) {	}

		Tuple7(const T1& t1,const  T2& t2,const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7)
		{
			arg1 = t1;
			arg2 = t2;
			arg3 = t3;
			arg4 = t4;
			arg5 = t5;
			arg6 = t6;
			arg7 = t7;
		}
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		T6 arg6;
		T7 arg7;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	class Tuple8
	{
	public:
		Tuple8() : arg1(T1()), arg2(T2()), arg3(T3()), arg4(T4()), arg5(T5()), arg6(T6()), arg7(T7()), arg8(T8()) {	}

		Tuple8(const T1& t1,const  T2& t2,const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8)
		{
			arg1 = t1;
			arg2 = t2;
			arg3 = t3;
			arg4 = t4;
			arg5 = t5;
			arg6 = t6;
			arg7 = t7;
			arg8 = t8;
		}
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		T6 arg6;
		T7 arg7;
		T8 arg8;
	};

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	class Tuple9
	{
	public:
		Tuple9() : arg1(T1()), arg2(T2()), arg3(T3()), arg4(T4()), arg5(T5()), arg6(T6()), arg7(T7()), arg8(T8()), arg9(T9()) {	}

		Tuple9(const T1& t1,const  T2& t2,const T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9)
		{
			arg1 = t1;
			arg2 = t2;
			arg3 = t3;
			arg4 = t4;
			arg5 = t5;
			arg6 = t6;
			arg7 = t7;
			arg8 = t8;
			arg9 = t9;
		}
		T1 arg1;
		T2 arg2;
		T3 arg3;
		T4 arg4;
		T5 arg5;
		T6 arg6;
		T7 arg7;
		T8 arg8;
		T9 arg9;
	};

	inline Tuple0 * GetTuple()
	{
		return new Tuple0;
	}

	template<class T1>
	Tuple1<T1> * GetTuple(const T1 &t1)
	{
		return new Tuple1<T1>(t1);
	}

	template<class T1, class T2>
	Tuple2<T1, T2> * GetTuple(const T1 &t1, const T2 &t2)
	{
		return new Tuple2<T1, T2>(t1, t2);
	}

	template<class T1, class T2, class T3>
	Tuple3<T1, T2, T3> * GetTuple(const T1 &t1, const T2 &t2, const T3& t3)
	{
		return new Tuple3<T1, T2, T3>(t1, t2, t3);
	}

	template<class T1, class T2, class T3, class T4>
	Tuple4<T1, T2, T3, T4> * GetTuple(const T1 &t1, const T2 &t2,const  T3& t3, const T4& t4)
	{
		return new Tuple4<T1, T2, T3, T4>(t1, t2, t3, t4);
	}

	template<class T1, class T2, class T3, class T4, class T5>
	Tuple5<T1, T2, T3, T4, T5> * GetTuple(const T1 &t1, const T2 &t2,const  T3& t3, const T4& t4, const T5& t5)
	{
		return new Tuple5<T1, T2, T3, T4, T5>(t1, t2, t3, t4, t5);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6>
	Tuple6<T1, T2, T3, T4, T5, T6> * GetTuple(const T1 &t1, const T2 &t2,const  T3& t3, const T4& t4, const T5& t5, const T6& t6)
	{
		return new Tuple6<T1, T2, T3, T4, T5, T6>(t1, t2, t3, t4, t5, t6);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	Tuple7<T1, T2, T3, T4, T5, T6, T7> * GetTuple(const T1 &t1, const T2 &t2,const  T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7)
	{
		return new Tuple7<T1, T2, T3, T4, T5, T6, T7>(t1, t2, t3, t4, t5, t6, t7);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	Tuple8<T1, T2, T3, T4, T5, T6, T7, T8> * GetTuple(const T1 &t1, const T2 &t2,const  T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8)
	{
		return new Tuple8<T1, T2, T3, T4, T5, T6, T7, T8>(t1, t2, t3, t4, t5, t6, t7, t8);
	}

	template<class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	Tuple9<T1, T2, T3, T4, T5, T6, T7, T8, T9> * GetTuple(const T1 &t1, const T2 &t2,const  T3& t3, const T4& t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9)
	{
		return new Tuple9<T1, T2, T3, T4, T5, T6, T7, T8, T9>(t1, t2, t3, t4, t5, t6, t7, t8, t9);
	}

	class CTask
	{
	public:
		CTask(VOID) {}
		virtual ~CTask(VOID) {}

	public:
		virtual VOID Run() = 0;
	};

	template<class TObj, class TpFunc, class TParam>
	VOID RunFunction(TObj* pObj, TpFunc pFunc , TParam* param)
	{

	}

	template<class TObj>
	class CPtrTaskBase : public Thread::CTask
	{
	public:
		CPtrTaskBase(TObj * pObj) : m_pObj(pObj)
		{
		}
		virtual ~CPtrTaskBase() 
		{
		}

		VOID SetObj(TObj * pObj) 
		{ 
			m_pObj = pObj;
		}

	protected:
		TObj*		m_pObj;
	};

	template<class TObj, class TpFunc, class TParam>
	class CPtrTask : public CPtrTaskBase<TObj>
	{
	public:
		CPtrTask(TObj* pObj, TpFunc pFunc , TParam* param)
			: CPtrTaskBase(pObj)
		{
			m_pFunc = pFunc;
			m_param = param;
		}
		~CPtrTask()
		{
			if (m_param)
			{
				delete m_param;
			}
		}
		VOID Run()
		{
			RunFunction(m_pObj, m_pFunc, m_param );
		}
	private:
		TpFunc		m_pFunc;
		TParam*		m_param;
	};

	template<class TObj, class TpFunc>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple0* pTuple0)
	{
		(pObj->*pFunc)();
	}

	template<class TObj, class TpFunc, class T1>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple1<T1>* pTuple1)
	{
		(pObj->*pFunc)(pTuple1->arg);
	}

	template<class TObj, class TpFunc, class T1, class T2>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple2<T1, T2>* pTuple2)
	{
		(pObj->*pFunc)(pTuple2->arg1, pTuple2->arg2);
	}

	template<class TObj, class TpFunc, class T1, class T2, class T3>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple3<T1, T2, T3>* pTuple3)
	{
		(pObj->*pFunc)(pTuple3->arg1, pTuple3->arg2, pTuple3->arg3);
	}

	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple4<T1, T2, T3, T4>* pTuple4)
	{
		(pObj->*pFunc)(pTuple4->arg1, pTuple4->arg2, pTuple4->arg3, pTuple4->arg4);
	}

	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple5<T1, T2, T3, T4, T5>* pTuple5)
	{
		(pObj->*pFunc)(pTuple5->arg1, pTuple5->arg2, pTuple5->arg3, pTuple5->arg4, pTuple5->arg5);
	}

	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5, class T6>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple6<T1, T2, T3, T4, T5, T6>* pTuple6)
	{
		(pObj->*pFunc)(pTuple6->arg1, pTuple6->arg2, pTuple6->arg3, pTuple6->arg4, pTuple6->arg5, pTuple6->arg6);
	}

	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple7<T1, T2, T3, T4, T5, T6, T7>* pTuple7)
	{
		(pObj->*pFunc)(pTuple7->arg1, pTuple7->arg2, pTuple7->arg3, pTuple7->arg4, pTuple7->arg5, pTuple7->arg6, pTuple7->arg7);
	}

	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple8<T1, T2, T3, T4, T5, T6, T7, T8>* pTuple8)
	{
		(pObj->*pFunc)(pTuple8->arg1, pTuple8->arg2, pTuple8->arg3, pTuple8->arg4, pTuple8->arg5, pTuple8->arg6, pTuple8->arg7, pTuple8->arg8);
	}

	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	VOID RunFunction(TObj* pObj, TpFunc pFunc, Tuple9<T1, T2, T3, T4, T5, T6, T7, T8, T9>* pTuple9)
	{
		(pObj->*pFunc)(pTuple9->arg1, pTuple9->arg2, pTuple9->arg3, pTuple9->arg4, pTuple9->arg5, pTuple9->arg6, pTuple9->arg7, pTuple9->arg8, pTuple9->arg9);
	}

	// 0参数
	template<class TObj, class TpFunc >
	CTask* CreatePtrTask(TObj* pObj, TpFunc func)
	{
		return new CPtrTask< TObj , TpFunc , Tuple0>( pObj, func, GetTuple());
	}

	// 1参数
	template<class TObj, class TpFunc, class T1>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1)
	{
		return new CPtrTask<TObj, TpFunc, Tuple1<T1> >(pObj, func, GetTuple(t1));
	}

	// 2参数
	template<class TObj, class TpFunc, class T1, class T2>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1, const T2&t2)
	{
		return new CPtrTask<TObj, TpFunc, Tuple2<T1, T2> >(pObj, func, GetTuple(t1, t2));
	}

	// 3参数
	template<class TObj, class TpFunc, class T1, class T2, class T3>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1, const T2&t2, const T3&t3)
	{
		return new CPtrTask<TObj, TpFunc, Tuple3<T1, T2, T3> >(pObj, func, GetTuple(t1, t2,t3));
	}

	// 4参数
	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1, const T2&t2, const T3&t3, const T4&t4)
	{
		return new CPtrTask<TObj, TpFunc, Tuple4<T1, T2, T3, T4> >(pObj, func, GetTuple(t1, t2, t3, t4));
	}

	// 5参数
	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1, const T2&t2, const T3&t3, const T4&t4, const T5& t5)
	{
		return new CPtrTask<TObj, TpFunc, Tuple5<T1, T2, T3, T4, T5> >(pObj, func, GetTuple(t1, t2, t3, t4, t5));
	}

	// 6参数
	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5, class T6>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1, const T2&t2, const T3&t3, const T4&t4, const T5& t5, const T6& t6)
	{
		return new CPtrTask<TObj, TpFunc, Tuple6<T1, T2, T3, T4, T5, T6> >(pObj, func, GetTuple(t1, t2, t3, t4, t5, t6));
	}

	// 7参数
	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1, const T2&t2, const T3&t3, const T4&t4, const T5& t5, const T6& t6, const T7& t7)
	{
		return new CPtrTask<TObj, TpFunc, Tuple7<T1, T2, T3, T4, T5, T6, T7> >(pObj, func, GetTuple(t1, t2, t3, t4, t5, t6, t7));
	}

	// 8参数
	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1, const T2&t2, const T3&t3, const T4&t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8)
	{
		return new CPtrTask<TObj, TpFunc, Tuple8<T1, T2, T3, T4, T5, T6, T7, T8> >(pObj, func, GetTuple(t1, t2, t3, t4, t5, t6, t7, t8));
	}

	// 9参数
	template<class TObj, class TpFunc, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
	CTask* CreatePtrTask(TObj* pObj, TpFunc func, const T1& t1, const T2&t2, const T3&t3, const T4&t4, const T5& t5, const T6& t6, const T7& t7, const T8& t8, const T9& t9)
	{
		return new CPtrTask<TObj, TpFunc, Tuple9<T1, T2, T3, T4, T5, T6, T7, T8, T9> >(pObj, func, GetTuple(t1, t2, t3, t4, t5, t6, t7, t8, t9));
	}

} // end namespace