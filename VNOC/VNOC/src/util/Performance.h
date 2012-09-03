#pragma once
#include "time.h"
#include "vector"
#include "atlstr.h"
#include "windows.h"
#include "atlsync.h"
#include "../base/IRefCountImpl.h"
#include "atlsync.h"

#define PerfDefine(_text)		Performance::CPerformance _perf(_text);
#define PerfMileStone(_text)	_perf.MileStone(_text);
#define PerfPause				_perf.Pause();
#define PerfResume				_perf.Resume();

namespace Performance
{
	typedef std::pair<LARGE_INTEGER,LARGE_INTEGER> TimeStamp;
	typedef std::vector<std::pair<TimeStamp,CString> >	MileStonesVec;

	// 统计运行时间
	class CPerformance : public IRefCountImpl
	{
	public:
		CPerformance(LPCTSTR text=NULL,BOOL threadSafe=FALSE,LPCTSTR logFile=NULL);
		~CPerformance();
	public:
		// 暂停计时
		void Pause();
		// 继续计时
		void Resume();
		// 里程碑
		void MileStone(LPCTSTR text=NULL);
	private:
		void _Enter();
		void _Leave();
		void _CheckFlag();
	private:
		BOOL m_bEnable;
		MileStonesVec m_vec;
		MileStonesVec::value_type m_curr;
		BOOL m_bPaused;
		CString m_taskName;
		CString m_logFile;
		BOOL m_threadSafe;
		ATL::CCriticalSection m_cs;
	};

}

#include "Performance.inl"