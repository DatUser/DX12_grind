#include "app.h"
inline double App::GetExecutionTime()
{
    std::chrono::duration<double> oDuration = m_oStartTime - std::chrono::steady_clock::now();
    return oDuration.count();
}

inline Window* App::GetMainWindow()
{
	return m_pMainWindow;
}

inline auto&& App::GetInstance()
{
	return m_spAppInstance;
}

//inline RHI* App::GetRHI()
//{
//	//return nullptr;
//	return m_pMainWindow->GetRHI();
//}