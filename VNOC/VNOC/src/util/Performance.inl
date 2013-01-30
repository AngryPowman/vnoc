
#include "util.h"

namespace Performance
{

	inline CPerformance::CPerformance( LPCTSTR text/*=NULL*/,BOOL threadSafe/*=FALSE*/,LPCTSTR logFile )
	{
		_CheckFlag();
		if (m_bEnable)
		{
			m_vec.reserve(16);		// 申请内存会耗时。规模暂定这么大。
			m_bPaused = FALSE;
			m_taskName = text;
			if (!logFile)
			{
				m_logFile = L"Performance";
			}
			else
			{
				m_logFile = logFile;
			}

			QueryPerformanceCounter(&m_curr.first.first);
		}
	}

	inline CPerformance::~CPerformance()
	{
		if (m_bEnable)
		{
			MileStone();

			CString log;
			double total = 0;
			double useTime;
			LARGE_INTEGER freq;
			freq.QuadPart = 0;
			QueryPerformanceFrequency(&freq);
			double dfreq = freq.QuadPart==0? 1: static_cast<double>(freq.QuadPart);

			MileStonesVec::iterator i = m_vec.begin();
			if (i!=m_vec.end() && i+1 == m_vec.end())
			{
				total = ((i->first.second.QuadPart - i->first.first.QuadPart)
					/ dfreq)*1000;
			}
			else
			{
				while (i != m_vec.end())
				{
					LONGLONG during = i->first.second.QuadPart - i->first.first.QuadPart;
					useTime = (during/dfreq)*1000;

					log.AppendFormat(_T("\t\t%s : %.6lfms\n")
						, (i->second.IsEmpty()? (i+1==m_vec.end()? _T("剩余"): _T("N/A")): i->second)
						, useTime);
					total += useTime;
					++i;
				}
			}
			CString head;
			head.Format(_T("    性能统计: %-50s\t共用时%lfms\n"),m_taskName,total);
			head += log;
			head += L"\n";
			//global::log2(m_logFile,head);
		}
	}

	inline void CPerformance::Pause()
	{
		if (m_bEnable)
		{
			_Enter();
			QueryPerformanceCounter(&m_curr.first.second);
			m_bPaused = TRUE;
			_Leave();
		}
	}

	inline void CPerformance::Resume()
	{
		if (m_bEnable)
		{
			_Enter();
			m_bPaused = FALSE;
			LARGE_INTEGER tm;
			QueryPerformanceCounter(&tm);
			tm.QuadPart += m_curr.first.second.QuadPart - m_curr.first.first.QuadPart;
			m_curr.first.first = tm;
			_Leave();
		}
	}

	inline void CPerformance::MileStone( LPCTSTR text/*=NULL*/ )
	{
		if (m_bEnable)
		{
			if (m_bPaused)
			{
				ATLASSERT(FALSE && "step with paused task");
			}
			else
			{
				_Enter();
				QueryPerformanceCounter(&m_curr.first.second);
				m_curr.second = text;
				m_vec.push_back(m_curr);
				m_curr.second.Empty();

				QueryPerformanceCounter(&m_curr.first.first);
				_Leave();
			}
		}
	}

	inline void CPerformance::_Enter()
	{
		if (m_threadSafe)
		{
			m_cs.Enter();
		}
	}

	inline void CPerformance::_Leave()
	{
		if (m_threadSafe)
		{
			m_cs.Leave();
		}
	}

	inline void CPerformance::_CheckFlag()
	{
		m_bEnable = TRUE;
	}

}

