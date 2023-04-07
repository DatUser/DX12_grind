inline double App::GetExecutionTime()
{
    std::chrono::duration<double> oDuration = m_oStartTime - std::chrono::steady_clock::now();
    return oDuration.count();
}